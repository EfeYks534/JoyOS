#include "PIC.h"
#include "IOLib.h"
#include "IDTLib.h"
#include <stdio.h>

static void (*handlers[128])(struct Registers *regs);

void IRQSetHandler(uint8_t index, void (*handler)(struct Registers *reg))
{
	handlers[index] = handler;
}


void IRQHandler(struct Registers *regs)
{
	uint8_t irq = regs->exception_num;

	if(irq == 7 || irq == 15) {
		// TODO: Handle spurious IRQ
	}

	for(int i = irq * 8; i < irq + 8; i++)
		if(handlers[i] != NULL)
			handlers[i](regs);

	if(irq >= 8)
		Out8(PIC2, 0x20);
	Out8(PIC1, 0x20);
}

#define IRQ(x) void IRQInterrupt##x();

IRQ(0)
IRQ(1)
IRQ(2)
IRQ(3)
IRQ(4)
IRQ(5)
IRQ(6)
IRQ(7)
IRQ(8)
IRQ(9)
IRQ(10)
IRQ(11)
IRQ(12)
IRQ(13)
IRQ(14)
IRQ(15)

void PICSetupInterrupts()
{
	IDTRegisterIntHandler(0x20, IRQInterrupt0);
	IDTRegisterIntHandler(0x21, IRQInterrupt1);
	IDTRegisterIntHandler(0x22, IRQInterrupt2);
	IDTRegisterIntHandler(0x23, IRQInterrupt3);
	IDTRegisterIntHandler(0x24, IRQInterrupt4);
	IDTRegisterIntHandler(0x25, IRQInterrupt5);
	IDTRegisterIntHandler(0x26, IRQInterrupt6);
	IDTRegisterIntHandler(0x27, IRQInterrupt7);

	IDTRegisterIntHandler(0x28, IRQInterrupt8);
	IDTRegisterIntHandler(0x29, IRQInterrupt9);
	IDTRegisterIntHandler(0x2A, IRQInterrupt10);
	IDTRegisterIntHandler(0x2B, IRQInterrupt11);
	IDTRegisterIntHandler(0x2C, IRQInterrupt12);
	IDTRegisterIntHandler(0x2D, IRQInterrupt13);
	IDTRegisterIntHandler(0x2E, IRQInterrupt14);
	IDTRegisterIntHandler(0x2F, IRQInterrupt15);

	PICRemap(0x20, 0x28);

	for(int i = 0; i < 16; i++)
		IRQClearMask(i);
}

void PICRemap(uint32_t off1, uint32_t off2)
{
	uint8_t a1 = 0;
	uint8_t a2 = 0;

	a1 = In8(PIC1_DATA);
	a2 = In8(PIC2_DATA);

	Out8W(PIC1, 0x11);
	Out8W(PIC2, 0x11);

	Out8W(PIC1_DATA, off1);
	Out8W(PIC2_DATA, off2);

	Out8W(PIC1_DATA, 4);
	Out8W(PIC2_DATA, 2);


	Out8W(PIC1_DATA, 1);
	Out8W(PIC2_DATA, 1);

	Out8(PIC1_DATA, a1);
	Out8(PIC2_DATA, a2);
}

void IRQSetMask(uint8_t line)
{
	uint16_t port = 0;
	uint8_t value = 0;

	port = line < 8 ? PIC1_DATA : PIC2_DATA;
	line = line > 7 ? line - 8 : line;

	value = In8(port) | (1 << line);
	Out8(port, value);
}

void IRQClearMask(uint8_t line)
{
	uint16_t port = 0;
	uint8_t value = 0;

	port = line < 8 ? PIC1_DATA : PIC2_DATA;
	line = line > 7 ? line - 8 : line;

	value = In8(port) & ~(1 << line);
	Out8(port, value);
}

uint16_t PICGetIRR()
{
	Out8(PIC1, 0x0A);
	Out8(PIC2, 0x0A);

	return (In8(PIC2) << 8) | In8(PIC1);
}

uint16_t PICGetISR()
{
	Out8(PIC1, 0x0B);
	Out8(PIC2, 0x0B);

	return (In8(PIC2) << 8) | In8(PIC1);	
}
