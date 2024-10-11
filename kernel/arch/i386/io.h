#ifndef ARCH_I386_IO_H
#define ARCH_I386_IO_H

#include <stdint.h>

/**
 * Sends an 8-bit value (val) to the specified 16-bit I/O port (port).
 *
 * The `outb` instruction can be encoded in two ways:
 * 1. If the port number is a compile-time constant that fits in 8 bits, the 
 *    instruction uses the `outb %al, $imm8` encoding, where `imm8` is an 8-bit
 *    immediate value. The "N" constraint ensures this.
 * 2. If the port number is dynamic or doesn't fit in 8 bits, the instruction 
 *    falls back to `outb %al, %dx`, which uses the `DX` register to hold the 
 *    port number.
 *
 * `%1` expands to `%dx` because `port` is a 16-bit `uint16_t`. If the port 
 * number were wider, `%w1` could be used to enforce a larger operand size.
 * 
 * The `volatile` keyword prevents the compiler from reordering or optimizing 
 * this I/O operation, ensuring it is executed exactly as written.
 *
 * **`AL`** is the lower 8 bits of the `AX` register. In this function, `AL` 
 * holds the 8-bit value (`val`) that is sent to the I/O port. The `"a"(val)` 
 * constraint places `val` into the `AL` register, as required by the `outb`
 * instruction to work with 8-bit values.
 */

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// Sends a 16-bit value to the specified I/O port.
static inline void outw(uint16_t port, uint16_t val){
    __asm__ volatile ( "outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// Sends a 32-bit value to the specified I/O port.
static inline void outl(uint16_t port, uint32_t val){
    __asm__ volatile ( "outl %0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

/**
 * Reads an 8-bit value from the specified 16-bit I/O port (port).
 *
 * The `inb` instruction reads the byte from the specified port into 
 * the `AL` register. The "=a" constraint indicates that the return 
 * value will be stored in the `ret` variable, which is assigned to 
 * the `AL` register after the I/O operation.
 */
static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0": "=a"(ret): "Nd"(port): "memory");
    return ret;
}

// Reads a 16-bit value from the specified I/O port.
static inline uint16_t inw(uint16_t port){
    uint16_t ret;
    __asm__ volatile ( "inw %w1, %w0": "=a"(ret): "Nd"(port): "memory");
    return ret;
}

// Reads a 32-bit value from the specified I/O port.
static inline uint32_t inl(uint16_t port){
    uint32_t ret;
    __asm__ volatile ( "inl %w1, %0": "=a"(ret): "Nd"(port): "memory");
    return ret;
}

/*
 * Adds a brief (1 to 4 µs) delay, useful for PIC remapping on older hardware.
 * Uses an I/O operation on an unused port (often 0x80) to create this wait.
 * Port 0x80 is a common port used in the Linux kernel as well for this.
 */
static inline void io_wait(void)
{
    outb(0x80, 0);
}

#endif