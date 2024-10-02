# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # Align loaded modules on page boundaries
.set MEMINFO,  1<<1             # Provide memory map to the kernel
.set FLAGS,    ALIGN | MEMINFO  # Combine flags for the multiboot header
.set MAGIC,    0x1BADB002       # Multiboot magic number to identify the header
.set CHECKSUM, -(MAGIC + FLAGS) # Ensure the sum of magic, flags, and checksum is zero

# Declare a multiboot header that marks the program as a multiboot-compliant kernel.
.section .multiboot.data, "aw"
.align 4
.long MAGIC                      # Multiboot magic number
.long FLAGS                      # Multiboot flags (ALIGN and MEMINFO)
.long CHECKSUM                   # Multiboot checksum

# Allocate the initial stack for the kernel.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384                      # Allocate 16 KiB for the stack
stack_top:

# Preallocate pages used for paging. These pages should not overlap with memory areas
# used by the bootloader (GRUB structures or modules), so the bootloader can avoid them.
.section .bss, "aw", @nobits
    .align 4096
boot_page_directory:
    .skip 4096                   # Allocate one page for the page directory
boot_page_table1:
    .skip 4096                   # Allocate one page for the first page table
# Further page tables may be added if the kernel size grows beyond 3 MiB.

# The kernel entry point, where execution starts after bootloader loads the kernel.
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
    # Set up paging by mapping virtual memory to physical memory.
    # First, compute the physical address of boot_page_table1.
    movl $(boot_page_table1 - 0xC0000000), %edi

    # Start mapping pages from physical address 0.
    movl $0, %esi

    # We will map 1023 pages (each page is 4 KiB). The 1024th page is reserved for the VGA buffer.
    movl $1023, %ecx

1:
    # Only map the kernel, not other memory regions.
    cmpl $_kernel_start, %esi
    jl 2f                     # If we haven't reached the start of the kernel, skip mapping

    cmpl $(_kernel_end - 0xC0000000), %esi
    jge 3f                    # If we've passed the end of the kernel, stop mapping

    # Map the physical address in %esi to virtual memory as "present, writable" (flags = 0x003).
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)         # Write the entry to the page table

2:
    addl $4096, %esi          # Move to the next physical page (4 KiB per page)
    addl $4, %edi             # Move to the next entry in the page table (each entry is 4 bytes)
    loop 1b                   # Repeat until all pages are mapped

3:
    # Map VGA video memory (physical address 0x000B8000) to virtual address 0xC03FF000.
    movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4

    # Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
    # This creates an identity mapping for the kernel (virtually from 0x0 to 0x3FFFFF)
    # and a higher-half mapping (virtually from 0xC0000000 to 0xC03FFFFF).
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

    # Set cr3 to point to the boot_page_directory, enabling paging.
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3

    # Enable paging by setting the paging bit (PG) in cr0.
    movl %cr0, %ecx
    orl $0x80010000, %ecx      # Set paging bit (PG) and write-protect bit (WP) in cr0
    movl %ecx, %cr0

    # Jump to the higher-half kernel with an absolute jump (since paging is now active).
    lea 4f, %ecx
    jmp *%ecx

.section .text

4:
    # Paging is now fully set up, and we are running in the higher half.

    # Unmap the identity mapping (since we no longer need it once paging is active).
    movl $0, boot_page_directory + 0

    # Reload cr3 to flush the TLB and apply the new mapping.
    movl %cr3, %ecx
    movl %ecx, %cr3

    # Set up the kernel stack to use the allocated space.
    mov $stack_top, %esp

    # Call the high-level kernel entry point.
    call kernel_main

    # If kernel_main returns, halt the CPU in an infinite loop.
    cli                     # Disable interrupts
1:  hlt                     # Halt the CPU
    jmp 1b                  # Jump back to the halt instruction to loop indefinitely