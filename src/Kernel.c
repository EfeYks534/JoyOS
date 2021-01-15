#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pipe.h>
#include "Common/Macros.h"
#include "Core/IOLib.h"
#include "Core/IDTLib.h"
#include "stivale.h"
#include "Core/PIC.h"
#include "Graphics/Graphics.h"
#include "Graphics/WinMan.h"
#include "Core/GDTLib.h"
#include "Drivers/PS2Controller.h"
#include "Drivers/KBController.h"
#include "Common/KernelLib.h"
#include "Memory/MemMng.h"
#include "Sched/Task.h"
#include "Sched/Timer.h"
#include "Common/Registers.h"

void KernelPanic(char *fmt, ...)
{
	char str[4096] = { 0 };

	va_list ap;
	va_start(ap, fmt);

	vsnprintf(str, 4096, fmt, ap);

	va_end(ap);

	GrRestoreLimits();

	GrFillRect(0, 0, GrWidth(), GrHeight(), 0x000404B4);
	GrDrawText(str, 24, 156, 0x00FFFFFF);

	Hang();
}

void Hang()
{
	while(1) {
		asm volatile(
			"cli\n"
			"hlt"
		);
	}
}

void TerminalEmulator();

void KernelMain(struct stivale_struct *boot_data, uint8_t *stack)
{
	asm volatile("cli");

	LoadGDT();

	SetupInterrupts();
	PICSetupInterrupts();

	IDTInstall();

	MemInit(boot_data);

	asm volatile("sti");

	IRQSetHandler(8, KBHandler0);
	KBInit();
	TaskInit(stack);
	GrInit(boot_data);
	TimerStart();

	WinManStart();

	if((boot_data->flags & 1) == 0)
		KernelPanic("Joy OS currently doesn't support UEFI");

	TerminalEmulator();
}
