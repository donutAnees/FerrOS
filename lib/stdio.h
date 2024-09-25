#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

// __restrict specifies that the pointer is not going to be aliased by any other pointer
// allowing for compiler to optimize
int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#endif
