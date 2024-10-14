#include <kernel/isr.h>
#include <kernel/pic.h>
#include <kernel/ps2_kbd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <kernel/tty.h>
#include "io.h"

key_event keyboard_buffer[MAX_KEYB_BUFFER_SIZE];
uint8_t keyboard_buf_position = 0;

static uint8_t current_state = NORMAL_STATE;
// Global flag to keep track of key presses like shift, control, etc.
static uint8_t status_flag = 0;

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
        // Do nothing for prefix keys for now
        current_state = NORMAL_STATE;
    }
    else  {
        // If we are in the current state, we need to handle the scancode.
        // If the scan code is 0x80, it is a key release, where the msb is set to 1.
        // We need to remove this bit to get the actual scancode, since this is the break code.
        // Or we can use the make code, which is the scancode without the break bit. I find
        // the latter method making more sense and easier to handle during someone pressing the
        // key continuously without releasing.
        if(scancode == 0XAA || scancode == 0xB6) {
            // Shift is realeased we invert our flag
            status_flag &= ~SHIFT_FLAG;
        }
        else if(scancode == 0x9D) {
            // Control is released we invert our flag
            status_flag &= ~CONTROL_FLAG;
        }
        else if(scancode == 0xB8) {
            // Alt is released we invert our flag
            status_flag &= ~ALT_FLAG;
        }
        else if((scancode & 0x80) == 0) {
            if (scancode == BACKSPACE) { // Backspace
                terminal_rmchar();
                keyboard_buf_position = (keyboard_buf_position - 1) % MAX_KEYB_BUFFER_SIZE;
            } 
            else if (scancode == CTRL) { // Control
                status_flag |= CONTROL_FLAG;
            } 
            else if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT) { // Left or Right Shift
                status_flag |= SHIFT_FLAG;
            } 
            else if (scancode == ALT) { // Alt
                status_flag |= ALT_FLAG;
            }
            else if (scancode == ENTER) { // Enter
                terminal_newline();
                keyboard_buf_position = 0;
            }
            else {
                // Alphanumeric characters
                keyboard_buffer[keyboard_buf_position].code = scancode;
                // Save the status mask
                keyboard_buffer[keyboard_buf_position].status_mask = status_flag;
                keyboard_buf_position = (keyboard_buf_position + 1) % MAX_KEYB_BUFFER_SIZE;
                terminal_putchar(ps2_kbd_return_ascii(scancode_mapping[scancode], &status_flag));
            }
        }
       
    }
    PIC_sendEOI(1);
}