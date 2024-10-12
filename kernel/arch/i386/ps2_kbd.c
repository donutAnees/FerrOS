#include <kernel/ps2_kbd.h>
#include <kernel/pic.h>
#include <kernel/idt.h>

#include <stdio.h>
#include "io.h"

extern void keyboard_isr_wrapper(void);

/**
 * @brief Modifies the controller configuration to translate to set 1.
 *
 * This function reads the current controller configuration byte by sending
 * command 0x20 to port 0x64. The configuration byte is read from port 0x60. 
 * The 6th bit (bit 5, zero-indexed) of the configuration byte is cleared. 
 * The modified configuration is then written back by sending command 0x60 to 
 * port 0x64 and writing the byte to port 0x60.
 */

static void disable_translation() {
    outb(PS2_COMMAND_PORT, 0x20);
    uint8_t config_byte = inb(PS2_DATA_PORT);
    config_byte &= ~(1 << 5);
    outb(PS2_COMMAND_PORT, 0x60);
    outb(PS2_DATA_PORT, config_byte);
}

void ps2_kbd_init() {
    IRQ_clear_mask(PS2_KBD_IRQ_LINE);
    idt_install_irq_handler(keyboard_isr_wrapper);
    idt_install_irq_handler(keyboard_isr_wrapper);
}