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

void kernel_main(void) 
{
	term_init();

	create_descriptor(0, 0, 0);
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
 

	printf("Hello!");
}