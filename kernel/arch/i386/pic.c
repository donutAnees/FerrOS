#include <kernel/pic.h>
#include "io.h"

void pic_disable(void) {
    outb(PIC_MASTER_DATA, 0xff);
    outb(PIC_SLAVE_DATA, 0xff);
}

// Masks (disables) the specified IRQ line
void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;  // Port address for PIC data
    uint8_t value;  // Current value of IMR

    if(IRQline < 8) {
        port = PIC_MASTER_DATA;  // Use Master PIC for IRQ lines 0-7
    } else {
        port = PIC_SLAVE_DATA;  // Use Slave PIC for IRQ lines 8-15
        IRQline -= 8;      /* Adjust the IRQ line for the Slave PIC by subtracting 8,
                            because the Slave PIC's IRQ lines (8-15) map to 0-7 internally */
    }

    value = inb(port) | (1 << IRQline);  // Read current IMR and set the bit for the given IRQ
    outb(port, value);                   // Write back the new IMR value to mask the IRQ
}

// Unmasks (enables) the specified IRQ line
void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;  // Port address for PIC data
    uint8_t value;  // Current value of IMR

    if(IRQline < 8) {
        port = PIC_MASTER_DATA;  // Use Master PIC for IRQ lines 0-7
    } else {
        port = PIC_SLAVE_DATA;  // Use Slave PIC for IRQ lines 8-15
        IRQline -= 8;      /* Adjust the IRQ line for the Slave PIC by subtracting 8,
                            because the Slave PIC's IRQ lines (8-15) map to 0-7 internally */
    }

    value = inb(port) & ~(1 << IRQline);  // Read current IMR and clear the bit for the given IRQ
    outb(port, value);                    // Write back the new IMR value to unmask the IRQ
}

// Remap the PIC to the specified offsets, them being 0x20 and 0x28
void PIC_remap(uint8_t offset1, uint8_t offset2)
{
	uint8_t a1, a2;
	
	a1 = inb(PIC_MASTER_DATA); // Save the interrupt mask of the master PIC (IRQs 0-7)
	a2 = inb(PIC_SLAVE_DATA);  // Save the interrupt mask of the slave PIC (IRQs 8-15)
	
	outb(PIC_MASTER_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC_SLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC_MASTER_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC_SLAVE_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC_MASTER_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC_SLAVE_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(PIC_MASTER_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC_SLAVE_DATA, ICW4_8086);
	io_wait();
	
	outb(PIC_MASTER_DATA, a1);   // restore saved masks.
	outb(PIC_SLAVE_DATA, a2);
}

void PIC_sendEOI(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC_SLAVE_COMMAND,PIC_EOI);
	
	outb(PIC_MASTER_COMMAND,PIC_EOI);
}

static uint16_t __pic_get_irq_reg(uint8_t ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC_MASTER_COMMAND, ocw3);
    outb(PIC_SLAVE_COMMAND, ocw3);
    return (inb(PIC_SLAVE_COMMAND) << 8) | inb(PIC_MASTER_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}