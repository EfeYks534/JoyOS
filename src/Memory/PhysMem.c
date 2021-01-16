#include "MemMng.h"
#include <string.h>
#include <stdio.h>
#include <Common/Macros.h>
#include <Common/KernelLib.h>

#define KERNEL_BASE 0xFFFFFFFF80100000

#define TYPE_USABLE   0x0001
#define TYPE_RESERVED 0x0002
#define TYPE_ACPI_REC 0x0003
#define TYPE_ACPI_NVS 0x0004
#define TYPE_BAD_MEM  0x0005
#define TYPE_KERNEL   0x000A
#define TYPE_BOOT     0x1000

struct
{
	uint64_t *bmap;
	uint64_t  size;

	uint64_t   total_mem;
	uint64_t total_pages;
} pmm = { 0 };

struct MMapEntry
{
	uint64_t   base;
	uint64_t length;
	uint32_t   type;
	uint32_t unused;
} PACKED;

void *PhysAlloc()
{
	for(uint64_t i = 0; i < pmm.size; i++) {
		if(pmm.bmap[i] == -1) continue;

		uint64_t page = __builtin_ffsll(pmm.bmap[i]);

		pmm.bmap[i] &= ~(1 << page);
		return (void*) ( i * pmm.total_pages * 8 + page * 4096 );
	}
}

void PhysReserve(void *addr, size_t size)
{
	uint64_t start = ((uint64_t) addr + size) / 4096;

	for(uint64_t i = (uint64_t)addr / 4096; i < (start + (size % 4096 ? 1 : 0)); i++)
		pmm.bmap[(i / 8)] &= ~(1 << (i % 8));
}

void PhysFree(void *addr, size_t size)
{
	uint64_t start = ((uint64_t) addr + size) / 4096;

	for(uint64_t i = (uint64_t)addr / 4096; i < (start + (size % 4096 ? 1 : 0)); i++)
		pmm.bmap[(i / 8)] |= 1 << (i % 8);
}

void PhysInit(struct stivale_struct *boot_data)
{
	struct MMapEntry *mmap = (struct MMapEntry*) boot_data->memory_map_addr;

	printf("Stivale memory map address: %xl\n", boot_data->memory_map_addr);

	for(uint64_t i = 0; i < boot_data->memory_map_entries; i++) {
		struct MMapEntry *ent = &mmap[i];
		pmm.total_mem += ent->length;
	}

	uint64_t bmap_base = 0;

	pmm.total_pages = pmm.total_mem / 4096;

	printf("A total %d bytes of memory has been found.", pmm.total_mem);

	for(uint64_t i = 0; i < boot_data->memory_map_entries; i++) {
		struct MMapEntry *ent = &mmap[i];

		if(( ent->type & TYPE_USABLE ) && ent->length >= (pmm.total_pages / 8)) {
			pmm.bmap = (uint64_t*) (ent->base + KERNEL_BASE);
			pmm.size = pmm.total_pages / 8;
			bmap_base = ent->base;

			for(int i = 0; i < (pmm.total_pages / 8); i++)
				pmm.bmap[i] = 0;

			break;
		}
	}

	for(uint64_t i = 0; i < boot_data->memory_map_entries; i++) {
		struct MMapEntry *ent = &mmap[i];

		if((ent->type & TYPE_USABLE) != 0)
			PhysFree((uint8_t*)ent->base, ent->length);
	}

	PhysReserve((uint8_t*)bmap_base, pmm.total_pages);
	PhysReserve(0, 0x100000);
}
