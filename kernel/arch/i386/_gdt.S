.section .text
.global _lgdt
.type _lgdt, @function

_lgdt:
    # Load GDTR from the provided pointer in ESP
    mov 4(%esp), %eax  # Get pointer to GDTR from stack
    lgdt (%eax)        # Load GDTR

    # Reload data segment registers
    mov $0x10, %ax     # Set AX to the data segment selector
    mov %ax, %ds       # Load data segment
    mov %ax, %es       # Load extra segment
    mov %ax, %fs       # Load FS segment
    mov %ax, %gs       # Load GS segment
    mov %ax, %ss       # Load stack segment

    # Jump to code segment selector
    jmp $0x08, $reload_cs

reload_cs:
    ret                 # Return from the function
