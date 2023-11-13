//stop compiling if the toolchain is fucked up
#include "ccheck.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "inx.h"
#include "outx.h"

#define TERM_VGA_ENABLE //enable VGA driver
#define TERM_SERIAL_ENABLE //enable serial driver
#include "term.h"


#include "gdt.h"

GDT g_gdt[NO_GDT_DESCRIPTORS];
GDT_PTR g_gdt_ptr;

/**
 * fill entries of GDT 
 */
void gdt_set_entry(int index, uint32 base, uint32 limit, uint8 access, uint8 gran) {
    GDT *this = &g_gdt[index];

    this->segment_limit = limit & 0xFFFF;
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->access = access;

    this->granularity = (limit >> 16) & 0x0F;
    this->granularity = this->granularity | (gran & 0xF0);

    this->base_high = (base >> 24 & 0xFF);
}

// initialize GDT
void gdt_init() {
    g_gdt_ptr.limit = sizeof(g_gdt) - 1;
    g_gdt_ptr.base_address = (uint32)g_gdt;

    // NULL segment
    gdt_set_entry(0, 0, 0, 0, 0);
    // code segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // data segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // user code segment
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // user data segment
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    load_gdt((uint32)&g_gdt_ptr);
}

void kernel_main(void) 
{
	gdt_init();
	term_init();


	printf("Hello!");
}