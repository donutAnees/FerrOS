#include <kernel/exception.h>

void exception_handler() {
    __asm__ __volatile__("cli; hlt");
}