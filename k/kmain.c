//stop compiling if the toolchain is fucked up
#include "ccheck.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "inx.h"
#include "outx.h"
#include "term.h"

#include "gdt.h"

extern void _irq_handler();
void set_irq_handler(int irq, uint32_t handler);
void init_timer();
bool toggle = 0;
void timer_handler(){
    toggle = 1;
}

void kernel_main(void) 
{
	gdt_init();
	term_init();
    init_timer();

    printf("hello mr.mac, this is the greatest opotating system ever");

    while (1==1)
    {
        if(toggle == 1){
            printf("TIMER!");
            toggle = 0;
        }
    }
    

}

void set_irq_handler(int irq, uint32_t handler) {
    // Calculate the address of the IDT entry for the given IRQ
    uint32_t idt_entry = 0x80000000 + (irq + 32) * 8;

    // Set the handler address in the IDT entry
    *(uint16_t*)idt_entry = (uint16_t)(handler & 0xFFFF);
    *(uint16_t*)(idt_entry + 2) = 0x08;  // Code segment selector
    *(uint8_t*)(idt_entry + 4) = 0x8E;   // Interrupt gate
    *(uint16_t*)(idt_entry + 5) = (uint16_t)((handler >> 16) & 0xFFFF);
}

void init_timer() {
    // Configure the master and slave PICs

    // Master PIC initialization
    outb(0x20, 0x11);  // Start initialization
    outb(0x21, 0x20);  // Set interrupt vector offset to 32
    outb(0x21, 0x04);  // Tell the master PIC that there is a slave PIC at IRQ2
    outb(0x21, 0x01);  // Set 8086/88 (MCS-80/85) mode
    outb(0x21, 0xFF);  // Mask all interrupts

    // Slave PIC initialization
    outb(0xA0, 0x11);  // Start initialization
    outb(0xA1, 0x28);  // Set interrupt vector offset to 40
    outb(0xA1, 0x02);  // Tell the slave PIC its cascade identity
    outb(0xA1, 0x01);  // Set 8086/88 (MCS-80/85) mode
    outb(0xA1, 0xFF);  // Mask all interrupts

    // Install the IRQ handler
    set_irq_handler(0, (uint32_t)_irq_handler);

    // Unmask the timer interrupt on the master PIC
    outb(0x21, inb(0x21) & 0xFD);  // 0xFD = 1111 1101 (bitmask to enable IRQ0)

    // Unmask the timer interrupt on the slave PIC
    outb(0xA1, inb(0xA1) & 0xFB);  // 0xFB = 1111 1011 (bitmask to enable IRQ2)

    // Enable interrupts
    asm volatile("sti");
}
