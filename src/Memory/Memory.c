#include "MemMng.h"
#include <Common/KernelLib.h>
#include <Graphics/Graphics.h>

static uint8_t memory[25165824] = { 0 };

static uint64_t mmap[96] = { 0 };

uint64_t *GetMMap() { return mmap; }

void MemInit()
{
	for(int i = 0; i < 96; i++)
		mmap[i] = -1;
}

void *MemAlloc()
{
	// Find free block

	for(int i = 0; i < 96; i++) {
		if(mmap[i] == 0) continue;

		uint64_t page = __builtin_ffsll(mmap[i]) - 1;

		mmap[i] &= ~(1 << page);

		return (void*)&memory[(i * 262144) + (page * 4096)];
	}

	return NULL;
}

void MemFree(void *_ptr)
{
	uint64_t ptr = (uint64_t) _ptr - (uint64_t) memory;

	uint64_t block = ptr / 262144;
	uint64_t page = (ptr % 262144) / 4096;

	if(block > 95 || page > 63)
		KernelPanic("Tried to free unkown memory page %l:%l", block, page);

	if( ( mmap[block] & (1 << page) ) != 0)
		KernelPanic("Tried to free full memory page %l:%l", block, page);

	mmap[block] |= (1 << page);
}
