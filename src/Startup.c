#include <stdint.h>
#include <stddef.h>
#include "stivale.h"

static uint8_t stack[25165824] = {0};


void _start(struct stivale_struct *bootloader_data);

__attribute__((section(".stivalehdr"), used))
struct stivale_header header = {
	.stack = (uintptr_t)stack + sizeof(stack),
	.framebuffer_bpp = 0,
	.framebuffer_width = 0,
	.framebuffer_height = 0,
	.flags = 1,
	.entry_point = (uint64_t)(uintptr_t)_start
};

extern void KernelMain();

void _start(struct stivale_struct *bootloader_data)
{
	uint16_t *vga = (uint16_t*)0xB8000;

	for(int i = 0; i < 2000; i++)
		vga[i] = (uint16_t)0x0F00 | ' ';

	KernelMain(bootloader_data);
	asm volatile("HLT");
}
