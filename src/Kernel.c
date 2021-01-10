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
#include "Common/Registers.h"

void KernelPanic(char *fmt, ...)
{
	char str[4096] = { 0 };

	va_list ap;
	va_start(ap, fmt);

	vsnprintf(str, 4096, fmt, ap);

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

void KernelMain(struct stivale_struct *boot_data)
{
	asm volatile("cli");

	LoadGDT();

	SetupInterrupts();
	PICSetupInterrupts();
	IDTInstall();

	IRQSetHandler(8, KBHandler0);
	KBInit();

	GrInit(boot_data);

	asm volatile("sti");

	while(1)
		asm volatile("hlt");
}
