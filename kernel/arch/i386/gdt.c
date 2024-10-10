#include <kernel/gdt.h>

// GDT Entry Macros (Definitions)
#define GDT_ENTRY_NULL_DESCRIPTOR 0  // Null descriptor

// Kernel Mode Segments
#define GDT_ENTRY_KERNEL_CODE 1       // Kernel Mode Code Segment
#define GDT_ENTRY_KERNEL_DATA 2       // Kernel Mode Data Segment

// User Mode Segments
#define GDT_ENTRY_USER_CODE 3         // User Mode Code Segment
#define GDT_ENTRY_USER_DATA 4         // User Mode Data Segment

// Task State Segment
#define GDT_ENTRY_TSS 5               // Task State Segment

// GDT Create Entry 
static uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;

    // Create the high 32-bit segment
    descriptor  =  (limit & 0x000F0000);         // Set limit bits 19:16
    descriptor |= ((flag) << 8) & 0x00F0FF00;    // Set type, p, dpl, s, g, d/b, l, and avl fields
    descriptor |= ((base) >> 16) & 0x000000FF;    // Set base bits 23:16
    descriptor |= (base & 0xFF000000);            // Set base bits 31:24

    // Shift by 32 to allow for the low part of the segment
    descriptor <<= 32;

    // Create the low 32-bit segment
    descriptor |= (base << 16);                   // Set base bits 15:0
    descriptor |= (limit & 0x0000FFFF);           // Set limit bits 15:0

    return descriptor;                             // Return the created descriptor
}

// GDT pointer structure
struct gdt_ptr {
    uint16_t len;
    uint32_t ptr;
} __attribute__((packed));

// Assembly functions to load gdt and reload the segment registers
extern void _lgdt();

void setup_gdt() {
    // Define the Global Descriptor Table (GDT) with a size of 64 bits.
    // The GDT entries are aligned to 16 bytes to satisfy architecture requirements.
    static uint64_t _gdt[6] __attribute__((aligned(16)));
    // Null Descriptor: A required entry.
    _gdt[GDT_ENTRY_NULL_DESCRIPTOR] = 0,  
    _gdt[GDT_ENTRY_KERNEL_CODE] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0)); 
    _gdt[GDT_ENTRY_KERNEL_DATA] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));  
    _gdt[GDT_ENTRY_USER_CODE] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    _gdt[GDT_ENTRY_USER_DATA] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
    // Task State Segment (TSS): 
    // TODO: This entry is currently not implemented and is not required for the kernel as of now.
    _gdt[GDT_ENTRY_TSS] = create_descriptor(0, 0, 0);

    // Structure to hold the pointer to the GDT and its length.
    static struct gdt_ptr gdt;

    // Set the length of the GDT (size of gdt array minus 1 for the null descriptor).
    gdt.len = sizeof(_gdt) - 1; 
    
    // Set the pointer to the GDTa
    gdt.ptr = (uint32_t)&_gdt;

    // Load the GDT into the CPU using the lgdt instruction and reload the segment registers.
    _lgdt(&gdt);

}