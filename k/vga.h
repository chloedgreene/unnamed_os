#pragma once

//IO functions
#include "outx.h"
#include "inx.h"

#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void vga_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(FB_COMMAND_PORT, 0x0A);
	outb(FB_DATA_PORT, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(FB_COMMAND_PORT, 0x0B);
	outb(FB_DATA_PORT, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vga_update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(FB_COMMAND_PORT, 0x0F);
	outb(FB_DATA_PORT, (uint8_t) (pos & 0xFF));
	outb(FB_COMMAND_PORT, 0x0E);
	outb(FB_DATA_PORT, (uint8_t) ((pos >> 8) & 0xFF));
}
uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(FB_COMMAND_PORT, 0x0F);
    pos |= inb(FB_DATA_PORT);
    outb(FB_COMMAND_PORT, 0x0E);
    pos |= ((uint16_t)inb(FB_DATA_PORT)) << 8;
    return pos;
}
void vga_disable_cursor()
{
	outb(FB_COMMAND_PORT, 0x0A);
	outb(FB_DATA_PORT, 0x20);
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void vga_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void vga_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void vga_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	vga_update_cursor((int)x,(int)y);
}
 
void vga_putchar(char c) 
{
	if (c == '\n'){
		terminal_column = 0;
		terminal_row+=1;
		return;
	}
	vga_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}
 
void vga_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		vga_putchar(data[i]);
}
 
void vga_writestring(const char* data) 
{
	vga_write(data, strlen(data));
}