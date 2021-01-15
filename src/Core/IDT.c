#include "IDTLib.h"
#include <Common/Macros.h>
#include <Common/KernelLib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

struct IDTEntry
{
	uint16_t  off_low;
	uint16_t selector;
	uint8_t    zero_0;
	uint8_t     flags;
	uint16_t  off_mid;
	uint32_t off_high;
	uint32_t   zero_1;
} PACKED;

struct IDTPointer
{
	uint16_t size;
	uint64_t addr;
} PACKED;

static struct IDTEntry entries[256] = { 0 };
static struct IDTPointer   ipointer = { 0 };

void IDTRegisterISR(uint16_t index, uint8_t attributes, ISR handler)
{
	if(handler == NULL) {
		//KernelFatal("Invalid ISR at index %xl", index);
	}

	struct IDTEntry *entry = &entries[index];

	entry->off_low =   INTCAST(handler) & 0x0000FFFF;
	entry->off_mid =  (INTCAST(handler) & 0xFFFF0000) >> 16;
	entry->off_high = (INTCAST(handler) & 0xFFFFFFFF00000000) >> 32;

	entry->zero_0 = 0;
	entry->zero_1 = 0;

	entry->flags = attributes;
	entry->selector = KernelCodeSelector;
}

void IDTRegisterIntHandler(uint16_t index, ISR handler)
{
	IDTRegisterISR(index, ATTR_INTR | ATTR_RING0 | ATTR_PRESENT, handler);
}

void IDTInstall()
{
	ipointer.size = sizeof(entries) - 1;
	ipointer.addr = INTCAST(entries);

	asm volatile("lidt %0" ::"m"(ipointer));
}

const char *exception_str[] = {
"Divide By Zero",
"Debug",
"Non-maskable Interrupt",
"Breakpoint",
"Overflow",
"Bound Range Exceeded",
"Invalid Opcode",
"Device Not Available",
"Double Fault",
"Coprocessor Segment Overrun",
"Invalid TSS",
"Segment Not Present",
"Stack-Segment Fault",
"General Protection Fault",
"Page Fault",
"RESERVED",
"x87 Floating-Point",
"Alignment Check",
"Machine Check",
"SIMD Floating-Point",
"Virtualization",
"Security Exception"
};

void ExceptionHandler(struct Registers *regs)
{
	uint64_t exception_num = regs->exception_num;
	if(exception_num > 20)
		exception_num = 21;

	KernelPanic("Exception: %s on %xl", exception_str[exception_num], regs->rip);
}

#define EXCEPTION(x) void ExceptionInterrupt##x();

EXCEPTION(DE)
EXCEPTION(DB)
EXCEPTION(NMI)
EXCEPTION(BP)
EXCEPTION(OF)
EXCEPTION(BR)
EXCEPTION(UD)
EXCEPTION(NM)
EXCEPTION(DF)
EXCEPTION(CPSO)
EXCEPTION(TS)
EXCEPTION(NP)
EXCEPTION(SS)
EXCEPTION(GP)
EXCEPTION(PF) // 0xE

EXCEPTION(MF) // 0x10
EXCEPTION(AC)
EXCEPTION(MC)
EXCEPTION(XM)
EXCEPTION(VE) // 0x14

EXCEPTION(SX) // 0x1E

void SysCallInterrupt();
void SaveConInt();

void SetupInterrupts()
{
	IDTRegisterIntHandler(0x0, ExceptionInterruptDE);
	IDTRegisterIntHandler(0x1, ExceptionInterruptDB);
	IDTRegisterIntHandler(0x2, ExceptionInterruptNMI);
	IDTRegisterIntHandler(0x3, ExceptionInterruptBP);
	IDTRegisterIntHandler(0x4, ExceptionInterruptOF);
	IDTRegisterIntHandler(0x5, ExceptionInterruptBR);
	IDTRegisterIntHandler(0x6, ExceptionInterruptUD);
	IDTRegisterIntHandler(0x7, ExceptionInterruptNM);
	IDTRegisterIntHandler(0x8, ExceptionInterruptDF);
	IDTRegisterIntHandler(0x9, ExceptionInterruptCPSO);
	IDTRegisterIntHandler(0xA, ExceptionInterruptTS);
	IDTRegisterIntHandler(0xB, ExceptionInterruptNP);
	IDTRegisterIntHandler(0xC, ExceptionInterruptSS);
	IDTRegisterIntHandler(0xD, ExceptionInterruptGP);
	IDTRegisterIntHandler(0xE, ExceptionInterruptPF);
	IDTRegisterIntHandler(0x10, ExceptionInterruptMF);
	IDTRegisterIntHandler(0x11, ExceptionInterruptAC);
	IDTRegisterIntHandler(0x12, ExceptionInterruptMC);
	IDTRegisterIntHandler(0x13, ExceptionInterruptXM);
	IDTRegisterIntHandler(0x14, ExceptionInterruptVE);
	IDTRegisterIntHandler(0x1E, ExceptionInterruptSX);

	IDTRegisterIntHandler(0x80, SysCallInterrupt);
	IDTRegisterIntHandler(0x70, SaveConInt);
}
