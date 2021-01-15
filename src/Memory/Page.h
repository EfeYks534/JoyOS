#pragma once

#include <stddef.h>
#include <stdint.h>
#include <Common/Macros.h>

struct PageDir
{
	uint32_t    addr : 20;
	uint32_t   avail :  3;
	uint32_t  ignore :  1;
	uint32_t    size :  1;
	uint32_t    zero :  1;
	uint32_t  access :  1;
	uint32_t   cache :  1;
	uint32_t  writet :  1;
	uint32_t    user :  1;
	uint32_t    rdwr :  1;
	uint32_t present :  1;
} PACKED;

struct PageTab
{
	uint32_t    addr : 20;
	uint32_t   avail :  3;
	uint32_t  global :  1;
	uint32_t    zero :  1;
	uint32_t   dirty :  1;
	uint32_t  access :  1;
	uint32_t   cache :  1;
	uint32_t  writet :  1;
	uint32_t    user :  1;
	uint32_t    rdwr :  1;
	uint32_t present :  1;
} PACKED;

#define PAGE_DIR_IGNORED (1 << 8)
#define PAGE_DIR_SIZE    (1 << 7)
#define PAGE_DIR_ACCESS  (1 << 5)
#define PAGE_DIR_CACHE   (1 << 4)
#define PAGE_DIR_WRITE_T (1 << 3)
#define PAGE_DIR_USER    (1 << 2)
#define PAGE_DIR_RDWR    (1 << 1)
#define PAGE_DIR_PRESENT ( 0x01 )

#define PAGE_GLOBAL  (1 << 8)
#define PAGE_DIRTY   (1 << 6)
#define PAGE_ACCESS  (1 << 5)
#define PAGE_CACHE   (1 << 4)
#define PAGE_WRITE_T (1 << 3)
#define PAGE_USER    (1 << 2)
#define PAGE_RDWR    (1 << 1)
#define PAGE_PRESENT ( 0x01 )
