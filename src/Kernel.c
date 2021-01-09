#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pipe.h>
#include "Common/Macros.h"
#include "Core/IOLib.h"
#include "Scheduler/Scheduler.h"
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

#if defined(__linux__)
#error "use cross compiler shithead"
#endif

void STI() { asm volatile("sti"); }
void CLI() { asm volatile("cli"); }

void KernelPanic(char *str, ...)
{
	printf("\x1B\x0Fjoy:\x1B\x04 panic:\x1B\x0F ");

	va_list ap;
	va_start(ap, str);

	vprintf(str, ap);

	va_end(ap);

	CLI();
	Hang();
}


void Hang()
{
	asm volatile(
		"cli\n"
		"hlt"
	);
}

void KBHandler0(struct Registers *regs);

void Test(struct Window *win)
{
}

void KernelMain(struct stivale_struct *boot_data)
{
	CLI();

	LoadGDT();

	SetupInterrupts();
	PICSetupInterrupts();
	IDTInstall();

	SchInit();

	IRQSetHandler(8, KBHandler0);
	KBInit();

	GrInit(boot_data);

	STI();

	struct Window *win = WindowNew();
	win->pos_x = 150;
	win->pos_y = 150;
	win->width = 640;
	win->height = 480;
	win->on_draw = Test;
	win->title = "Terminal Emulator";

	while(1) {
		SchSleep(416);
		WindowRender(win);
		asm volatile("hlt");
	}
}
