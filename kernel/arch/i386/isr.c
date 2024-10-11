#include <kernel/isr.h>
#include <stdio.h>

void exception_handler() {
    printf("Hit Exception Handler\n");
    __asm__ __volatile__("cli; hlt");
}