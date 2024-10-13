#ifndef _KERNEL_ISR_H
#define _KERNEL_ISR_H

#include <stdint.h>
#include <stdbool.h>

#define PS2_KBD_IRQ_LINE 1

#define PS2_DATA_PORT 0x60
#define PS2_COMMAND_PORT 0x64

#define MAX_KEYB_BUFFER_SIZE    255

// Keyboard state - Normal or Prefix
// Normal state is when no prefix key is pressed
// Prefix state is when a prefix key is pressed
#define NORMAL_STATE 0
#define PREFIX_STATE 1

#define CONTROL_FLAG (1 << 0)
#define SHIFT_FLAG (1 << 1)
#define ALT_FLAG (1 << 2)

#define SET_MODIFIER(modifiers, flag) ((modifiers) |= (flag))
#define CLEAR_MODIFIER(modifiers, flag) ((modifiers) &= ~(flag))

typedef struct {
    uint8_t code;
    // control, shift, alt, etc.
    uint8_t status_mask; 
} key_event;

// Circular Keyboard buffer
extern key_event keyboard_buffer[MAX_KEYB_BUFFER_SIZE];
extern uint8_t keyboard_buf_position;

typedef enum kernel_scancodes_set1 {
    KEY_UNKNOWN = 0x00,
    // Control Keys
    ESC         = 0x01,
    F1          = 0x3B,
    F2          = 0x3C,
    F3          = 0x3D,
    F4          = 0x3E,
    F5          = 0x3F,
    F6          = 0x40,
    F7          = 0x41,
    F8          = 0x42,
    F9          = 0x43,
    F10         = 0x44,
    F11         = 0x57,
    F12         = 0x58,
    
    // Number Row Keys
    TILDE       = 0x29,
    ONE         = 0x02,
    TWO         = 0x03,
    THREE       = 0x04,
    FOUR        = 0x05,
    FIVE        = 0x06,
    SIX         = 0x07,
    SEVEN       = 0x08,
    EIGHT       = 0x09,
    NINE        = 0x0A,
    ZERO        = 0x0B,
    MINUS       = 0x0C,
    EQUAL       = 0x0D,
    BACKSPACE   = 0x0E,
    
    // Tab and Enter
    TAB         = 0x0F,
    Q           = 0x10,
    W           = 0x11,
    E           = 0x12,
    R           = 0x13,
    T           = 0x14,
    Y           = 0x15,
    U           = 0x16,
    I           = 0x17,
    O           = 0x18,
    P           = 0x19,
    LEFT_BRACE  = 0x1A,
    RIGHT_BRACE = 0x1B,
    ENTER       = 0x1C,
    
    // Control Keys
    CTRL        = 0x1D,
    A           = 0x1E,
    S           = 0x1F,
    D           = 0x20,
    F           = 0x21,
    G           = 0x22,
    H           = 0x23,
    J           = 0x24,
    K           = 0x25,
    L           = 0x26,
    SEMICOLON   = 0x27,
    APOSTROPHE  = 0x28,
    
    // Shift and Special Keys
    LEFT_SHIFT  = 0x2A,
    BACKSLASH   = 0x2B,
    Z           = 0x2C,
    X           = 0x2D,
    C           = 0x2E,
    V           = 0x2F,
    B           = 0x30,
    N           = 0x31,
    M           = 0x32,
    COMMA       = 0x33,
    PERIOD      = 0x34,
    SLASH       = 0x35,
    RIGHT_SHIFT = 0x36,
    
    // Control Keys
    KEYPAD_NUM  = 0x37,
    KEYPAD_DIV  = 0x38,
    ALT         = 0x38,
    KEYPAD_MINUS= 0x4A,
    KEYPAD_PLUS = 0x4E,
    
    // Arrow and Function Keys
    SPACE       = 0x39,
    CAPS_LOCK   = 0x3A,
    LEFT_ARROW  = 0x4B,
    RIGHT_ARROW = 0x4D,
    UP_ARROW    = 0x48,
    DOWN_ARROW  = 0x50,
    PAGE_UP     = 0x49,
    PAGE_DOWN   = 0x51,
    HOME        = 0x47,
    END         = 0x4F,
    INSERT      = 0x52,
    DELETE      = 0x53,
} kernel_scancode_set1_t;

static kernel_scancode_set1_t scancode_mapping[] = {
    [0x00] = KEY_UNKNOWN,
    [0x01] = ESC,
    [0x02] = ONE,
    [0x03] = TWO,
    [0x04] = THREE,
    [0x05] = FOUR,
    [0x06] = FIVE,
    [0x07] = SIX,
    [0x08] = SEVEN,
    [0x09] = EIGHT,
    [0x0A] = NINE,
    [0x0B] = ZERO,
    [0x0C] = MINUS,
    [0x0D] = EQUAL,
    [0x0E] = BACKSPACE,
    [0x0F] = TAB,
    [0x10] = Q,
    [0x11] = W,
    [0x12] = E,
    [0x13] = R,
    [0x14] = T,
    [0x15] = Y,
    [0x16] = U,
    [0x17] = I,
    [0x18] = O,
    [0x19] = P,
    [0x1A] = LEFT_BRACE,
    [0x1B] = RIGHT_BRACE,
    [0x1C] = ENTER,
    [0x1D] = CTRL,
    [0x1E] = A,
    [0x1F] = S,
    [0x20] = D,
    [0x21] = F,
    [0x22] = G,
    [0x23] = H,
    [0x24] = J,
    [0x25] = K,
    [0x26] = L,
    [0x27] = SEMICOLON,
    [0x28] = APOSTROPHE,
    [0x29] = TILDE,
    [0x2A] = LEFT_SHIFT,
    [0x2B] = BACKSLASH,
    [0x2C] = Z,
    [0x2D] = X,
    [0x2E] = C,
    [0x2F] = V,
    [0x30] = B,
    [0x31] = N,
    [0x32] = M,
    [0x33] = COMMA,
    [0x34] = PERIOD,
    [0x35] = SLASH,
    [0x36] = RIGHT_SHIFT,
    [0x37] = KEYPAD_NUM,
    [0x38] = ALT,
    [0x39] = SPACE,
    [0x3A] = CAPS_LOCK,
    [0x3B] = F1,
    [0x3C] = F2,
    [0x3D] = F3,
    [0x3E] = F4,
    [0x3F] = F5,
    [0x40] = F6,
    [0x41] = F7,
    [0x42] = F8,
    [0x43] = F9,
    [0x44] = F10,
    [0x45] = KEYPAD_NUM,
    [0x46] = KEYPAD_DIV,
    [0x47] = HOME,
    [0x48] = UP_ARROW,
    [0x49] = PAGE_UP,
    [0x4A] = KEYPAD_MINUS,
    [0x4B] = LEFT_ARROW,
    [0x4D] = RIGHT_ARROW,
    [0x4E] = KEYPAD_PLUS,
    [0x4F] = END,
    [0x50] = DOWN_ARROW,
    [0x51] = PAGE_DOWN,
    [0x52] = INSERT,
    [0x53] = DELETE,
};

/*
*@brief Initialize the PS/2 keyboard
*/
void ps2_kbd_init();

/*
*@brief Set the mask for the PS/2 keyboard
*/
void ps2_kbd_set_mask(bool mask);\

/*
*@brief Print the scancode of the PS/2 keyboard in ascii
*/
char ps2_kbd_return_ascii(kernel_scancode_set1_t scancode);

#endif