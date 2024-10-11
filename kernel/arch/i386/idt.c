#include <kernel/idt.h>
#include <stdbool.h>

static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];

// IDT Structure
__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

idt_ptr_t idt_ptr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    // this value can be whatever offset your kernel code selector is in 
    // your GDT, since each entry is 8 bytes and this is second entry, it is 0x08.
    descriptor->kernel_cs      = 0x08; 
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void setup_idt() {
    idt_ptr.base = (uintptr_t)&idt[0];
    idt_ptr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < IDT_CPU_EXCEPTION_COUNT; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idt_ptr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}