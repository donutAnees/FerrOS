#include <kernel/isr.h>
#include <kernel/pic.h>
#include <kernel/ps2_kbd.h>
#include <stdint.h>
#include <stdio.h>
#include <kernel/tty.h>
#include "io.h"

key_event keyboard_buffer[MAX_KEYB_BUFFER_SIZE];
uint8_t keyboard_buf_position = 0;

static uint8_t current_state = NORMAL_STATE;

void exception_handler(int num) {
    printf("Hit Exception Handler: %d\n", num);
    __asm__ __volatile__("cli; hlt");
}

void keyboard_irq_handler() {
    uint8_t scancode = inb(0x60);
    if (scancode == 0xE0) {
        current_state = PREFIX_STATE;
        // We have a prefix key, so we don't need to do anything with it.
        // instead, we just return and wait for the next scancode.
        return;
    }
    if (current_state == PREFIX_STATE) {
        // Store the next part of the scancode, then return to normal state.
        if (scancode == 0x1D) { // Control
            keyboard_buffer[keyboard_buf_position].status_mask |= CONTROL_FLAG;
        } else if (scancode == 0x2A || scancode == 0x36) { // Left or Right Shift
            keyboard_buffer[keyboard_buf_position].status_mask |= SHIFT_FLAG;
        } else if (scancode == 0x38) { // Alt
            keyboard_buffer[keyboard_buf_position].status_mask |= ALT_FLAG;
        } else if (scancode == 0x1C) { // Enter
            keyboard_buffer[keyboard_buf_position].status_mask |= ENTER_FLAG;
        } else {
            // Do nothing for other keys for now
        }
        current_state = NORMAL_STATE;
    }
    else  {
        // If we are in the prefix state, we need to handle the next part of the scancode.
        // If the scan code is 0x80, it is a key release, where the msb is set to 1.
        // We need to remove this bit to get the actual scancode.
        if((scancode & 0x80) == 0) {
            if (scancode == 0x0E) { // Backspace
                terminal_rmchar();
                if(keyboard_buf_position != -1) keyboard_buf_position = (keyboard_buf_position - 1) % MAX_KEYB_BUFFER_SIZE;
            } else {
                keyboard_buffer[keyboard_buf_position].code = scancode;
                keyboard_buf_position = (keyboard_buf_position + 1) % MAX_KEYB_BUFFER_SIZE;
                terminal_putchar(ps2_kbd_return_ascii(scancode_mapping[scancode]));
            }
        }
       
    }
    PIC_sendEOI(1);
}