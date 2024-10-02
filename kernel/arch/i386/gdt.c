#include <kernel/gdt.h>

// GDT Entry Macros (Definitions)
#define GDT_ENTRY_NULL_DESCRIPTOR 0x00  // Null descriptor

// Kernel Mode Segments
#define GDT_ENTRY_KERNEL_CODE 1       // Kernel Mode Code Segment
#define GDT_ENTRY_KERNEL_DATA 2       // Kernel Mode Data Segment

// User Mode Segments
#define GDT_ENTRY_USER_CODE 3         // User Mode Code Segment
#define GDT_ENTRY_USER_DATA 4         // User Mode Data Segment

// Task State Segment
#define GDT_ENTRY_TSS 5               // Task State Segment

// GDT Entry Macro
#define GDT_ENTRY(base, limit, flags) \
    (((limit) & 0xFFFF) | (((base) & 0xFFFF) << 16) | \
    (((base) & 0xFF0000) << 16) | ((flags) << 40) | \
    (((limit) & 0xF0000) << 32) | (((base) & 0xFF000000) << 32))

// GDT pointer structure
struct gdt_ptr {
    uint16_t len;
    uint32_t ptr;
} __attribute__((packed));


void setup_gdt() {
    // Define the Global Descriptor Table (GDT) with a size of 64 bits.
    // The GDT entries are aligned to 16 bytes to satisfy architecture requirements.
    static const uint64_t gdt[] __attribute__((aligned(16))) = {
        // Null Descriptor: A required entry, typically unused.
        [GDT_ENTRY_NULL_DESCRIPTOR] = 0,  
        [GDT_ENTRY_KERNEL_CODE] = GDT_ENTRY(0, 0xFFFFF, (GDT_CODE_PL0)),  
        [GDT_ENTRY_KERNEL_DATA] = GDT_ENTRY(0, 0xFFFFF, (GDT_DATA_PL0)),  
        [GDT_ENTRY_USER_CODE] = GDT_ENTRY(0, 0xFFFFF, (GDT_CODE_PL3)),  
        [GDT_ENTRY_USER_DATA] = GDT_ENTRY(0, 0xFFFFF, (GDT_DATA_PL3)),
        // Task State Segment (TSS): 
        // TODO: This entry is currently not implemented and is not required for the kernel.
        [GDT_ENTRY_TSS] = 0,
    };

    // Structure to hold the pointer to the GDT and its length.
    static struct gdt_ptr gdt_ptr;

    // Set the length of the GDT (size of gdt array minus 1 for the null descriptor).
    gdt_ptr.len = sizeof(gdt) - 1; 
    
    // Set the pointer to the GDT; adds the data segment value (DS) shifted left by 4 bits.
    // This is done to account for the segmentation in protected mode.
    gdt_ptr.ptr = (uint32_t)&gdt + (ds() << 4); 

    // Load the GDT into the CPU using the lgdt instruction.
    asm volatile("lgdtl %0" : : "m" (gdt_ptr));
}