#ifndef _KERNEL_SEGMENTATION_H
#define _KERNEL_SEGMENTATION_H

#include <stdint.h>

// Get the value of the Data Segment register (DS)
static inline uint16_t ds(void) {
    uint16_t seg;
	asm("movw %%ds,%0" : "=rm" (seg));
	return seg;
}

#endif