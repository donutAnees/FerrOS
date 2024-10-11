#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H

#include <stdint.h>

/*
 * Functions to provide access to programmed I/O instructions.
 */

/*
*@brief Read a byte from the specified port.
*/
void outb(uint16_t port, uint8_t val);

/*
*@brief Read a word from the specified port.
*/
void outw(uint16_t port, uint16_t val);

/*
*@brief Read a double word from the specified port.
*/
void outl(uint16_t port, uint32_t val);

/*
*@brief Write a byte to the specified port.
*/
void inb(uint16_t port, uint8_t* val);

/*
*@brief Write a word to the specified port.
*/
void inw(uint16_t port, uint16_t* val);

/*
*@brief Write a double word to the specified port.
*/
void inl(uint16_t port, uint32_t* val);

/*
 * Adds a brief (1 to 4 µs) delay, useful for PIC remapping on older hardware.
 * Uses an I/O operation on an unused port (often 0x80) to create this wait.
 * Port 0x80 is a common port used in the Linux kernel as well for this.
 */
void io_wait();

#endif