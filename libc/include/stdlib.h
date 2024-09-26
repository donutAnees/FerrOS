#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

// This attribute specifies to the compiler that the function 'abort' will not return to the calling function.
__attribute__((__noreturn__))
void abort(void);

#endif
