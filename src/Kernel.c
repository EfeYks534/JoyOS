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
#include "Common/Registers.h"

void KernelPanic(char *fmt, ...)
{
	char str[32768] = { 0 };

	va_list ap;
	va_start(ap, fmt);

	vsnprintf(str, 32768, fmt, ap);

	va_end(ap);

	GrFillRect(0, 0, GrWidth(), GrHeight(), 0x000404B4);
	GrDrawText(str, 24, 156, 0x00FFFFFF);

	Hang();
}

void Hang()
{
	asm volatile(
		"cli\n"
		"hlt"
	);
}

uint64_t *GetMMap();


void TestMem()
{
	uint64_t term_y = 10;
	MemCAlloc(4);
	void *ptr = MemCAlloc(4);
	MemCAlloc(4);
	MemFree(ptr);

	ptr = MemCAlloc(5);

	uint64_t *mmap = GetMMap();

	for(int i = 0; i < 96 / 3; i++) {
		char str[4096] = { 0 };

		snprintf(str, 4096, "%xl %xl %xl", mmap[i*3], mmap[i*3+1], mmap[i*3+2]);

		GrDrawText(str, 10, term_y, 0);
		term_y += 10;
	}
}

void Thread2Entry()
{
	
}


void KernelMain(struct stivale_struct *boot_data, uint8_t *stack)
{
	asm volatile("cli");

	LoadGDT();

	SetupInterrupts();
	PICSetupInterrupts();

	IRQSetHandler(8, KBHandler0);
	KBInit();

	IDTInstall();

	GrInit(boot_data);

	MemInit();
	TaskInit(stack);

	asm volatile("sti");

	TestMem();
}
