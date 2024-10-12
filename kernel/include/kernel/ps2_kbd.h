#ifndef _KERNEL_ISR_H
#define _KERNEL_ISR_H

#include <stdint.h>
#include <stdbool.h>

#define PS2_KBD_IRQ_LINE 1

#define PS2_DATA_PORT 0x60
#define PS2_COMMAND_PORT 0x64

/*
*@brief Initialize the PS/2 keyboard
*/
void ps2_kbd_init();

/*
*@brief Set the mask for the PS/2 keyboard
*/
void ps2_kbd_set_mask(bool mask);\

#endif