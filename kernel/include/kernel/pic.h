#ifndef _KERNEL_PIC_H_
#define _KERNEL_PIC_H_

#include <stdint.h>

// PIC Ports
#define PIC_MASTER          0x20
#define PIC_SLAVE		    0xA0
#define PIC_MASTER_COMMAND	PIC_MASTER
#define PIC_MASTER_DATA	    (PIC_MASTER+1)
#define PIC_SLAVE_COMMAND	PIC_SLAVE
#define PIC_SLAVE_DATA	    (PIC_SLAVE+1)

// PIC Commands
#define PIC_EOI		        0x20

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04	/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C	/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC_EOI		0x20		/* OW2 End-of-interrupt command code */
#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */

/*
*@brief Disable the Programmable Interrupt Controller (PIC)
*/
void pic_disable(void);

/*
*@brief Remap the Programmable Interrupt Controller (PIC) to the specified offsets
*/
void PIC_remap(uint8_t offset1, uint8_t offset2);

/*
*@brief Mask the specified IRQ
*/
void IRQ_set_mask(uint8_t irq);

/*
*@brief Unmask the specified IRQ
*/
void IRQ_clear_mask(uint8_t irq);

/*
*@brief Send the End of Interrupt (EOI) signal to the PIC
*/
void PIC_sendEOI(uint8_t irq);

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_get_irr(void);

/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_get_isr(void);

#endif