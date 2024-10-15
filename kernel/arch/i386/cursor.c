#include <kernel/cursor.h>
#include "io.h"
#include "vga.h"

// These ports correspond to the VGA controller registers.
#define CURSOR_PORT_START 0x3D4
#define CURSOR_PORT_END 0x3D5

void disable_cursor() {
    // Disable the cursor by setting the scanline start and end to 0x20.
    outb(CURSOR_PORT_START, 0x0A);
    outb(CURSOR_PORT_END, 0x20);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    // Select cursor start register (0x0A) by writing to port 0x3D4
    outb(CURSOR_PORT_START, 0x0A);
    outb(CURSOR_PORT_END, (inb(CURSOR_PORT_END) & 0xC0) | cursor_start);
    // Select cursor end register (0x0B) by writing to port 0x3D4
    outb(CURSOR_PORT_START, 0x0B);
    outb(CURSOR_PORT_END, (inb(CURSOR_PORT_END) & 0xE0) | cursor_end);
}

void update_cursor(size_t pos) {
    // Select the high byte of the cursor position.
    outb(CURSOR_PORT_START, 0x0E);
    outb(CURSOR_PORT_END, (uint8_t) ((pos >> 8) & 0xFF));
    // Select the low byte of the cursor position.
    outb(CURSOR_PORT_START, 0x0F);
    outb(CURSOR_PORT_END, (uint8_t) (pos & 0xFF));
}