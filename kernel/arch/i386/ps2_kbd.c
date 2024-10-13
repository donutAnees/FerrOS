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
    // Set the status mask to 0
    for(uint8_t i = 0; i < MAX_KEYB_BUFFER_SIZE; i++) {
        keyboard_buffer[i].status_mask = 0;
    }
    IRQ_clear_mask(PS2_KBD_IRQ_LINE);
    idt_install_irq_handler(keyboard_isr_wrapper);
    idt_install_irq_handler(keyboard_isr_wrapper);
}

char ps2_kbd_return_ascii(kernel_scancode_set1_t scancode){
    switch (scancode) {
        case A: return 'a';
        case B: return 'b';
        case C: return 'c';
        case D: return 'd';
        case E: return 'e';
        case F: return 'f';
        case G: return 'g';
        case H: return 'h';
        case I: return 'i';
        case J: return 'j';
        case K: return 'k';
        case L: return 'l';
        case M: return 'm';
        case N: return 'n';
        case O: return 'o';
        case P: return 'p';
        case Q: return 'q';
        case R: return 'r';
        case S: return 's';
        case T: return 't';
        case U: return 'u';
        case V: return 'v';
        case W: return 'w';
        case X: return 'x';
        case Y: return 'y';
        case Z: return 'z';
        case ONE: return '1';
        case TWO: return '2';
        case THREE: return '3';
        case FOUR: return '4';
        case FIVE: return '5';
        case SIX: return '6';
        case SEVEN: return '7';
        case EIGHT: return '8';
        case NINE: return '9';
        case ZERO: return '0';
        case MINUS: return '-';
        case EQUAL: return '=';
        case BACKSPACE: return '\b'; // Backspace character
        case TAB: return '\t'; // Tab character
        case LEFT_BRACE: return '[';
        case RIGHT_BRACE: return ']';
        case SEMICOLON: return ';';
        case APOSTROPHE: return '\'';
        case BACKSLASH: return '\\';
        case COMMA: return ',';
        case PERIOD: return '.';
        case SLASH: return '/';
        case SPACE: return ' ';
        default: return '\0'; // Non-printable characters
    }
}