#include <kernel/isr.h>
#include <kernel/pic.h>
#include <stdint.h>
#include <stdio.h>
#include "io.h"

void exception_handler(int num) {
    printf("Hit Exception Handler: %d\n", num);
    __asm__ __volatile__("cli; hlt");
}

void keyboard_irq_handler() {
    uint8_t scancode = inb(0x60);
    printf("Scancode: %d\n", scancode);
    PIC_sendEOI(1);
}