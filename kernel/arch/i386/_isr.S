.section .text
.extern exception_handler   # Declare the external exception handler

# ----------------------------
# Macro for ISRs with error codes
# ----------------------------
.macro isr_err_stub num
isr_stub_\num:
    call exception_handler   # Call the common exception handler
    iret                      # Return from the interrupt (use iretq for 64-bit)
.endm

# ----------------------------
# Macro for ISRs without error codes
# ----------------------------
.macro isr_no_err_stub num
isr_stub_\num:
    call exception_handler   # Call the common exception handler
    iret                      # Return from the interrupt (use iretq for 64-bit)
.endm

# ----------------------------
# Define ISRs for the 32 Exceptions
# ----------------------------
isr_no_err_stub 0           # ISR 0: Divide by Zero
isr_no_err_stub 1           # ISR 1: Debug
isr_no_err_stub 2           # ISR 2: Non-maskable Interrupt (NMI)
isr_no_err_stub 3           # ISR 3: Breakpoint
isr_no_err_stub 4           # ISR 4: Overflow
isr_no_err_stub 5           # ISR 5: Bound Range Exceeded
isr_no_err_stub 6           # ISR 6: Invalid Opcode
isr_no_err_stub 7           # ISR 7: Device Not Available
isr_err_stub    8           # ISR 8: Double Fault
isr_no_err_stub 9           # ISR 9: Coprocessor Segment Overrun
isr_err_stub    10          # ISR 10: Invalid TSS
isr_err_stub    11          # ISR 11: Segment Not Present
isr_err_stub    12          # ISR 12: Stack Segment Fault
isr_err_stub    13          # ISR 13: General Protection Fault
isr_err_stub    14          # ISR 14: Page Fault
isr_no_err_stub 15          # ISR 15: Reserved
isr_no_err_stub 16          # ISR 16: Reserved
isr_err_stub    17          # ISR 17: Floating Point Error
isr_no_err_stub 18          # ISR 18: Alignment Check
isr_no_err_stub 19          # ISR 19: Machine Check
isr_no_err_stub 20          # ISR 20: SIMD Floating Point Exception
isr_no_err_stub 21          # ISR 21: Reserved
isr_no_err_stub 22          # ISR 22: Reserved
isr_no_err_stub 23          # ISR 23: Reserved
isr_no_err_stub 24          # ISR 24: Reserved
isr_no_err_stub 25          # ISR 25: Reserved
isr_no_err_stub 26          # ISR 26: Reserved
isr_no_err_stub 27          # ISR 27: Reserved
isr_no_err_stub 28          # ISR 28: Reserved
isr_no_err_stub 29          # ISR 29: Reserved
isr_err_stub    30          # ISR 30: Security Exception
isr_no_err_stub 31          # ISR 31: Reserved

# ----------------------------
# Define the ISR Stub Table
# ----------------------------
.global isr_stub_table      # Make the ISR table globally accessible
isr_stub_table:                  
    .long isr_stub_0
    .long isr_stub_1
    .long isr_stub_2
    .long isr_stub_3
    .long isr_stub_4
    .long isr_stub_5
    .long isr_stub_6
    .long isr_stub_7
    .long isr_stub_8
    .long isr_stub_9
    .long isr_stub_10
    .long isr_stub_11
    .long isr_stub_12
    .long isr_stub_13
    .long isr_stub_14
    .long isr_stub_15
    .long isr_stub_16
    .long isr_stub_17
    .long isr_stub_18
    .long isr_stub_19
    .long isr_stub_20
    .long isr_stub_21
    .long isr_stub_22
    .long isr_stub_23
    .long isr_stub_24
    .long isr_stub_25
    .long isr_stub_26
    .long isr_stub_27
    .long isr_stub_28
    .long isr_stub_29
    .long isr_stub_30
    .long isr_stub_31
