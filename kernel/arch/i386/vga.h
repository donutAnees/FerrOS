#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>
#include <stddef.h>

/**
 * @file vga.h
 *
 * A VGA entry combines a character and a color attribute into a single 16-bit value,
 * used for rendering.
 *
 * The Lower 8 bits represent the ASCII character and the Upper 8 bit represents the color.
 * |COLOR|ASCII|
 */

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Pointer to the VGA text buffer
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC03FF000;

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

/*
 * @brief Combines the foreground and the background into a single byte.
 *
 * Takes the foreground color which is the text color and the background color, which is
 * the text color and combines them into a single byte.
 *
 * Lower 4 bits represent the foreground color.
 * Upper 4 bits represent the background color.
 *
 * @param fg The foreground color.
 * @param bg The background color.
 * @return Combined 8 bit integer.
 */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

/*
 * @brief Create a VGA entry for a character with color.
 *
 * @param uc The character.
 * @param color The color value from vga_entry_color.
 * @return Combined entry as 16 bit integer.
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

#endif
