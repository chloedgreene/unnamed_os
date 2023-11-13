#pragma once

#include "outx.h"
#include "inx.h"

#define SERIAL_PORT 0x3f8          // COM1
static int serial_init() {
	outb(SERIAL_PORT + 1, 0x00);    // Disable all interrupts
	outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(SERIAL_PORT + 1, 0x00);    //                  (hi byte)
	outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(SERIAL_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	outb(SERIAL_PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
	outb(SERIAL_PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
	if(inb(SERIAL_PORT + 0) != 0xAE) {
		return 1;
	}
	outb(SERIAL_PORT + 4, 0x0F);
	return 0;
}
int serial_received() {
   return inb(SERIAL_PORT + 5) & 1;
}
char serial_read() {
   while (serial_received() == 0);
 
   return inb(SERIAL_PORT);
}
int serial_is_transmit_empty() {
   return inb(SERIAL_PORT + 5) & 0x20;
}
void serial_write(char a) {
   while (serial_is_transmit_empty() == 0);
 
   outb(SERIAL_PORT,a);
}