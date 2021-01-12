#include "MemMng.h"
#include <string.h>
#include <Common/KernelLib.h>
#include <Graphics/Graphics.h>

#define MEM_SIZE  24 // Memory size in megs
#define MMAP_SIZE (MEM_SIZE * 1048576 / (4096 * 64))
#define MBLK_SIZE (MEM_SIZE * 1048576 / MMAP_SIZE)

static uint8_t memory[MEM_SIZE] = { 0 };

static uint64_t mmap[MMAP_SIZE] = { 0 };

static uint64_t mhdr[MMAP_SIZE * 64] = { 0 };

uint64_t *GetMMap() { return mmap; }

void MemInit()
{
	for(int i = 0; i < MMAP_SIZE; i++)
		mmap[i] = -1;
}

void *MemCAlloc(size_t size)
{
	uint64_t block = 0;
	uint64_t page = 0;

	for(int i = 0; i < MMAP_SIZE * 64; i++) {
		if(( mmap[block] & (1 << page) ) != 0 ) {
			for(int j = 0; j < size; j++) {
				if( ( mmap[(block + (j / 64))] & (1 << (page + (j % 64))) ) == 0 ) {
					page += size;
					block += page / 64;
					page = page % 64;
					goto next;
				}
			}

			for(int j = 0; j < size; j++)
				mmap[block + (j / 64)] &= ~(1 << (page + (j % 64)));

			mhdr[block * 64 + page] = size;

			return &memory[block * MBLK_SIZE + page * 4096];
		}
		page++;
		block += page / 64;
		page = page % 64;
next:
		continue;
	}

	return NULL;
}

void *MemAlloc()
{
	for(int i = 0; i < MMAP_SIZE; i++) {
		if(mmap[i] == 0) continue;

		uint64_t page = __builtin_ffsll(mmap[i]) - 1;

		mmap[i] &= ~(1 << page);
		mhdr[i * 64 + page] = 1;

		return (void*)&memory[(i * MBLK_SIZE) + (page * 4096)];
	}

	return NULL;
}

void MemFree(void *_ptr)
{
	if(_ptr == NULL) return;
	uint64_t ptr = (uint64_t) _ptr - (uint64_t) memory;

	uint64_t block = ptr / MBLK_SIZE;
	uint64_t page = (ptr % MBLK_SIZE) / 4096;

	if(block > (MMAP_SIZE - 1) || page > 63)
		KernelPanic("Tried to free unkown memory page %l:%l", block, page);

	uint64_t hdr = mhdr[(block * 64) + page];

	if( ( mmap[block] & (1 << page) ) != 0 || hdr == 0)
		KernelPanic("Tried to free empty memory page %l:%l", block, page);

	if(hdr > (MMAP_SIZE * 64))
		KernelPanic("Tried to free a big amount of memory pages");

	for(int i = 0; i < hdr; i++) {
		uint64_t blk = i / 64;
		uint64_t pg = ((page + i) % 64);

		mmap[block + blk] |= (1 << pg);
	}

	mhdr[(block * 64) + page] = 0;
}
