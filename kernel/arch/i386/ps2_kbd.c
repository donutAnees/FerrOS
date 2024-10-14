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

char ps2_kbd_return_ascii(kernel_scancode_set1_t scancode, uint8_t* status_mask) {
    switch (scancode) {
        case A: return (*status_mask & SHIFT_FLAG) ? 'A' : 'a';
        case B: return (*status_mask & SHIFT_FLAG) ? 'B' : 'b';
        case C: return (*status_mask & SHIFT_FLAG) ? 'C' : 'c';
        case D: return (*status_mask & SHIFT_FLAG) ? 'D' : 'd';
        case E: return (*status_mask & SHIFT_FLAG) ? 'E' : 'e';
        case F: return (*status_mask & SHIFT_FLAG) ? 'F' : 'f';
        case G: return (*status_mask & SHIFT_FLAG) ? 'G' : 'g';
        case H: return (*status_mask & SHIFT_FLAG) ? 'H' : 'h';
        case I: return (*status_mask & SHIFT_FLAG) ? 'I' : 'i';
        case J: return (*status_mask & SHIFT_FLAG) ? 'J' : 'j';
        case K: return (*status_mask & SHIFT_FLAG) ? 'K' : 'k';
        case L: return (*status_mask & SHIFT_FLAG) ? 'L' : 'l';
        case M: return (*status_mask & SHIFT_FLAG) ? 'M' : 'm';
        case N: return (*status_mask & SHIFT_FLAG) ? 'N' : 'n';
        case O: return (*status_mask & SHIFT_FLAG) ? 'O' : 'o';
        case P: return (*status_mask & SHIFT_FLAG) ? 'P' : 'p';
        case Q: return (*status_mask & SHIFT_FLAG) ? 'Q' : 'q';
        case R: return (*status_mask & SHIFT_FLAG) ? 'R' : 'r';
        case S: return (*status_mask & SHIFT_FLAG) ? 'S' : 's';
        case T: return (*status_mask & SHIFT_FLAG) ? 'T' : 't';
        case U: return (*status_mask & SHIFT_FLAG) ? 'U' : 'u';
        case V: return (*status_mask & SHIFT_FLAG) ? 'V' : 'v';
        case W: return (*status_mask & SHIFT_FLAG) ? 'W' : 'w';
        case X: return (*status_mask & SHIFT_FLAG) ? 'X' : 'x';
        case Y: return (*status_mask & SHIFT_FLAG) ? 'Y' : 'y';
        case Z: return (*status_mask & SHIFT_FLAG) ? 'Z' : 'z';
        case ONE: return (*status_mask & SHIFT_FLAG) ? '!' : '1';
        case TWO: return (*status_mask & SHIFT_FLAG) ? '@' : '2';
        case THREE: return (*status_mask & SHIFT_FLAG) ? '#' : '3';
        case FOUR: return (*status_mask & SHIFT_FLAG) ? '$' : '4';
        case FIVE: return (*status_mask & SHIFT_FLAG) ? '%' : '5';
        case SIX: return (*status_mask & SHIFT_FLAG) ? '^' : '6';
        case SEVEN: return (*status_mask & SHIFT_FLAG) ? '&' : '7';
        case EIGHT: return (*status_mask & SHIFT_FLAG) ? '*' : '8';
        case NINE: return (*status_mask & SHIFT_FLAG) ? '(' : '9';
        case ZERO: return (*status_mask & SHIFT_FLAG) ? ')' : '0';
        case MINUS: return (*status_mask & SHIFT_FLAG) ? '_' : '-';
        case EQUAL: return (*status_mask & SHIFT_FLAG) ? '+' : '=';
        case BACKSPACE: return '\b'; // Backspace character
        case TAB: return '\t'; // Tab character
        case LEFT_BRACE: return (*status_mask & SHIFT_FLAG) ? '{' : '[';
        case RIGHT_BRACE: return (*status_mask & SHIFT_FLAG) ? '}' : ']';
        case SEMICOLON: return (*status_mask & SHIFT_FLAG) ? ':' : ';';
        case APOSTROPHE: return (*status_mask & SHIFT_FLAG) ? '"' : '\'';
        case BACKSLASH: return (*status_mask & SHIFT_FLAG) ? '|' : '\\';
        case COMMA: return (*status_mask & SHIFT_FLAG) ? '<' : ',';
        case PERIOD: return (*status_mask & SHIFT_FLAG) ? '>' : '.';
        case SLASH: return (*status_mask & SHIFT_FLAG) ? '?' : '/';
        case SPACE: return ' ';
        default: return '\0'; // Non-printable characters
    }
}