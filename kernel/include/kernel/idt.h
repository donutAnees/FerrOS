#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256
#define IDT_CPU_EXCEPTION_COUNT 32

// IDT Entry Structure
typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

// IDT Pointer Structure for 32 bit
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

// IDT Initialization Function
void setup_idt(void);

// Function to set an IDT descriptor
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);

// Function to install an IRQ handler
void idt_install_irq_handler(void* handler);

#endif


