#ifndef _KERNEL_PIT_H
#define _KERNEL_PIT_H

#include <stdint.h>

#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_CHANNEL1_DATA_PORT 0x41
#define PIT_CHANNEL2_DATA_PORT 0x42
#define PIT_COMMAND_PORT 0x43

#define PIT_IRQ_LINE 0

#define PIT_BASE_CLOCK_RATE 1193182
#define MAX_FREQUENCY 1193181
#define MIN_FREQUENCY 18

/*
 * We use ms to keep track of the time, for some frequencies
 * the increments have a decimal value, so we need to keep track of the
 * which is maintained by fraction. Whenever the fraction reaches 1, we increment
 * the tick count. For example if the PIT is set for 700 Hz it'd work out
 * to (roughly) 1.42857 ms, we have to take care of the decimal value.
 */
static volatile uint32_t system_timer_fraction;
static volatile uint32_t system_timer_ms;

/*
 * The IRQ0_ms and IRQ0_fraction denote the ms and fraction
 * current frequency of the PIT
 */
static uint32_t IRQ0_ms;
static uint32_t IRQ0_fraction;
/*
 * The PIT_reload_value is the value we load into the PIT
 */
static uint16_t PIT_reload_value;

/*
 * This is the actual frequency of the PIT, for the rounded
 * Reload Value
 */
static uint16_t PIT_frequency;
/*
 *@brief Initialize the Programmable Interval Timer (PIT)
 *@param frequency The frequency to set the PIT to
 */
void pit_init(uint16_t frequency);

/*
 *@brief Increment the tick count
 */
void pit_increment_tick();

/*
 *@brief Get the current tick count
 */
uint32_t pit_get_tick_count();

/*
 *@brief Set the current tick count
 */
void pit_set_tick_count(uint32_t tick_count);

#endif