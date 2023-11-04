#include "include/io.h"
#include "include/pic.h"

void pic_acknowledge(unsigned int interrupt)
{
	if (interrupt < PIC_1_OFFSET || interrupt > PIC_2_END) {
		return;
	}

	if (interrupt < PIC_2_OFFSET) {
		i686_outb(PIC_1_COMMAND_PORT, PIC_ACKNOWLEDGE);
	} else {
		i686_outb(PIC_2_COMMAND_PORT, PIC_ACKNOWLEDGE);
	}
}

void pic_remap(int offset1, int offset2)
{
	i686_outb(PIC_1_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4);	// starts the initialization sequence (in cascade mode)
	i686_outb(PIC_2_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4);
	i686_outb(PIC_1_DATA, offset1);				// ICW2: Master PIC vector offset
	i686_outb(PIC_2_DATA, offset2);				// ICW2: Slave PIC vector offset
	i686_outb(PIC_1_DATA, 4);					// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	i686_outb(PIC_2_DATA, 2);					// ICW3: tell Slave PIC its cascade identity (0000 0010)

	i686_outb(PIC_1_DATA, PIC_ICW4_8086);
	i686_outb(PIC_2_DATA, PIC_ICW4_8086);

    // Setup Interrupt Mask Register (IMR)
	i686_outb(PIC_1_DATA, 0xFD); // 1111 1101 - Enable IRQ 1 only (keyboard).
	i686_outb(PIC_2_DATA, 0xFF);

	asm("sti"); // Enable interrupts.
}
