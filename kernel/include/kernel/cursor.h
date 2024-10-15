#ifndef _KERNEL_CURSOR_H
#define _KERNEL_CURSOR_H

#include <stdint.h>
#include <stddef.h>

/* 
 * Function to disable the cursor.
 */
void disable_cursor();

/* 
 * Function to enable the cursor.
 * cursor_start: The scanline start of the cursor.
 * cursor_end: The scanline end of the cursor.
 */
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

/* 
 * Function to update the cursor position.
 */
void update_cursor(size_t pos);

#endif