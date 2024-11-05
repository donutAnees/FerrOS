#include <kernel/pit.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/pic.h>
#include <stdio.h>
#include "io.h"

extern void pit_isr_wrapper(void);

void pit_init(uint16_t frequency) {
    uint32_t reload_value;
    uint32_t remainder;
    // Calculate the reload value
    if(frequency <= MIN_FREQUENCY) {
        reload_value = 0x10000; // 65536, this is the value we split our frequency by
    } else if(frequency >= MAX_FREQUENCY) {
        reload_value = 1; // We use the entire frequency
    }
    else{
        // First, calculate the initial reload value using integer division
        reload_value = PIT_BASE_CLOCK_RATE / frequency;
        remainder = PIT_BASE_CLOCK_RATE % frequency;
        if (remainder >= (PIT_BASE_CLOCK_RATE / 2)) {
            reload_value++;  // Round up if remainder is greater than or equal to half
        }
    }
    PIT_reload_value = reload_value;
    // Calculate the actual frequency
    frequency = PIT_BASE_CLOCK_RATE / reload_value;
    remainder = PIT_BASE_CLOCK_RATE % reload_value;
    if (remainder >= (PIT_BASE_CLOCK_RATE / 2)) {
        frequency++;  // Round up if remainder is greater than or equal to half
    }
    PIT_frequency = frequency;
    // We dont want to lose precision, so we use 32.32 format
    // Here we multiply by 2^32, think of it as shifting the bits left by 32,
    // then divide by the base clock rate, this way we get the fraction with
    // the precision we need, once done we can use the high 32 bits as the ms
    // and the low 32 bits as the fraction
    const uint64_t factor = 0xDBB3A062; // Equivalent to (3000 * (2^42) / 3579545)
    
    // Perform the multiplication in 64-bit space
    uint64_t result = (uint64_t)reload_value * factor;
    
    // Perform a right shift by 10 to scale down, dividing by 2^10 = 1024
    result >>= 10;

    // Split the result into whole milliseconds and fractional milliseconds
    IRQ0_ms = (uint32_t)(result >> 32);       // Higher 32 bits for whole ms
    IRQ0_fraction = (uint32_t)(result & 0xFFFFFFFF); // Lower 32 bits for fractional ms
    // Command byte, 0x36 is 00110110 in binary, which means:
    // Bits 0-3: 0b110, which is channel 0
    // Bits 4-5: 0b11, which is access mode lobyte/hibyte
    // Bits 6-7: 0b10, which is operating mode rate generator
    outb(PIT_COMMAND_PORT, 0x36); 
    outb(PIT_CHANNEL0_DATA_PORT, (uint8_t)(reload_value & 0xFF)); // Low byte
    outb(PIT_CHANNEL0_DATA_PORT, (uint8_t)(reload_value >> 8)); // High byte 
    idt_install_irq_handler(pit_isr_wrapper);
    IRQ_clear_mask(PIT_IRQ_LINE);
}

void pit_increment_tick() {
    uint32_t new_fraction = system_timer_fraction + IRQ0_fraction;
    // If the new fraction is greater than 1, we increment the ms
    // that is overflowed
    if (new_fraction < system_timer_fraction) {
        system_timer_ms += IRQ0_ms + 1;
    } else {
        system_timer_ms += IRQ0_ms;
    }
    system_timer_fraction = new_fraction;
}

