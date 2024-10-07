#ifdef _KERNEL_EXCEPTION_H
#define _KERNEL_EXCEPTION_H

#include <stdint.h>

__attribute__((noreturn))
void exception_handler();

#endif