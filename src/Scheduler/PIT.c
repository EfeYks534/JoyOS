#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pipe.h>
#include <Common/Macros.h>
#include <Core/IOLib.h>
#include <Core/IDTLib.h>
#include <Core/PIC.h>
#include <Core/GDTLib.h>
#include <Common/KernelLib.h>
#include <Common/Registers.h>
#include "Scheduler.h"

static void CLI() { asm volatile("cli"); }
static void STI() { asm volatile("sti"); }

static uint64_t num_ticks = 0;

static void WaitTick()
{
	while(1) {
		uint64_t last_tick = num_ticks;
		asm volatile("hlt");
		if(num_ticks != last_tick) return;
	}
}

void TickHandler(struct Registers *regs)
{
	num_ticks++;
}

void SchInit()
{
	CLI();

	uint16_t val = 119; // 10000 times per sec

	Out8(0x43, 0x36);

	Out8(0x40, val & 0x00FF);
	Out8(0x40, (val & 0xFF00) >> 8);

	IRQSetHandler(0, TickHandler);
}

void SchSleep(uint64_t ticks)
{
	uint64_t first_tick = num_ticks;
	while(1) {
		WaitTick();
		if((num_ticks - first_tick) >= ticks) break;
	}
}

void SchWaitUntil(uint8_t act, uint64_t data)
{
	switch(act)
	{
	case SCHACT_PIPE:
		while(1) {
			WaitTick();
			if(pstat(data)) return;
		}
		break;
	default:
		KernelPanic("Unknown scheduler action");
	}
}
