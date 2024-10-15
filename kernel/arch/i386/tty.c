#include <stdbool.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/cursor.h>

#include "vga.h"

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static void inline fill_screen(){
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_initialize(void) {
	enable_cursor(14, 15);
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_MAGENTA);
	terminal_buffer = VGA_MEMORY;
	fill_screen();
}

static void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y, bool flag) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	if(flag) 
		update_cursor(index + 1);
	else 
		update_cursor(index);
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row, true);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_newline(){
	// Initialized to one for '>' prompt
	terminal_column = 1;
	// If we reach the end, we reset the screen
	if(++terminal_row == VGA_HEIGHT) {
		terminal_row = 0;
		fill_screen();
	}
	terminal_putentryat('>', terminal_color, 0, terminal_row, false);
}

void terminal_rmchar(){
	if(terminal_row == 0 && terminal_column == 0)
		return;
	if(terminal_column == 0){
		terminal_column = VGA_WIDTH - 1;
		terminal_row--;
	} else {
		terminal_column--;
	}
	terminal_putentryat(' ', terminal_color, terminal_column, terminal_row, false);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
