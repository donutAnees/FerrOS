#ifdef _KERNEL_ISR_H
#define _KERNEL_ISR_H

#include <stdint.h>

__attribute__((noreturn))
void exception_handler();

#endif