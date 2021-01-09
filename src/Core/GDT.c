#include "GDTLib.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define PACKED __attribute__((packed))

struct GDTEntry
{
	uint16_t  limit_low;
	uint16_t   base_low;
	uint8_t    base_mid;
	uint8_t      access;
	uint8_t limit_upper : 4;
	uint8_t       flags : 4;
	uint8_t  base_upper;
} PACKED;

struct GDTPointer
{
	uint16_t size;
	uint64_t addr;
} PACKED;

static struct GDTEntry entries[3];
static uint8_t            ent_ptr = 0;
static struct GDTPointer gpointer;

static void GDTInstall()
{
	asm volatile("lgdt %0\n"
		"pushq %%rbp\n"
		"movq %%rsp, %%rbp\n"
		"pushq %1\n"
		"pushq %%rbp\n"
		"pushfq\n"
		"pushq %2\n"
		"pushq $1f\n"
		"iretq\n"
		"1:\n"
		"popq %%rbp\n"
		"mov %1, %%ds\n"
		"mov %1, %%es\n"
		"mov %1, %%fs\n"
		"mov %1, %%gs\n"
		"mov %1, %%ss\n"
		:: "m"(gpointer), "r"((uint64_t)0x10), "r"((uint64_t)0x08)
		: "memory");
}

static void GDTEntryNew(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
	struct GDTEntry *ent = &entries[ent_ptr++];

	ent->base_low = base &    0x0000FFFF;
	ent->base_mid = base &   (0x00FF0000) >> 16;
	ent->base_upper = base & (0xFF000000) >> 24;

	ent->access = access;
	ent->flags  =  flags;

	ent->limit_low = limit &    0x0000FFFF;
	ent->limit_upper = limit & (0x000F0000) >> 16;
}

void LoadGDT()
{
	// NULL
	GDTEntryNew(0, 0, ACCESS_ATTR_NULL, FLAG_ATTR_NULL);

	// CODE
	uint8_t access = ACCESS_ATTR_CODEDATA | ACCESS_ATTR_PRESENT |
					ACCESS_ATTR_EXEC | ACCESS_ATTR_READABLE;
	GDTEntryNew(0, 0xFFFFFFFF, access, FLAG_ATTR_GRAN4K | FLAG_ATTR_S64B);

	// DATA
	access = ACCESS_ATTR_CODEDATA | ACCESS_ATTR_PRESENT |
			ACCESS_ATTR_WRITABLE;
	GDTEntryNew(0, 0xFFFFFFFF, access, FLAG_ATTR_GRAN4K | FLAG_ATTR_S32B);

	gpointer.addr = (uint64_t)(uintptr_t)entries;
	gpointer.size = sizeof(entries) - 1;

	GDTInstall();
}
