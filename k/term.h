#pragma once

#include "vga.h"
#include "serial.h"
void _putchar(char character);
#include "printf.h"

void term_init(){
	#if defined(TERM_VGA_ENABLE)
	    vga_initialize();
    #endif
    #if defined(TERM_SERIAL_ENABLE)
	    serial_init();
    #endif
}

void _putchar(char character)
{
    #if defined(TERM_SERIAL_ENABLE)
	    serial_write(character);
    #endif
    #if defined(TERM_VGA_ENABLE)
	    vga_putchar(character);
    #endif
}