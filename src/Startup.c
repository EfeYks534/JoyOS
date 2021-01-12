#include <stdint.h>
#include <stddef.h>
#include "Sched/Task.h"
#include "stivale.h"

static uint8_t stack[3145728] = {0};

void _start(struct stivale_struct *bootloader_data);

__attribute__((section(".stivalehdr"), used))
struct stivale_header header = {
	.stack = (uintptr_t)stack + sizeof(stack),
	.framebuffer_bpp = 32,
	.framebuffer_width = 1024,
	.framebuffer_height = 768,
	.flags = 1,
	.entry_point = (uint64_t)(uintptr_t)_start
};

extern void KernelMain(struct stivale_struct *boot_data, uint8_t *kern_stack);

void _start(struct stivale_struct *bootloader_data)
{
	KernelMain(bootloader_data, stack);
	asm volatile("hlt");
}
