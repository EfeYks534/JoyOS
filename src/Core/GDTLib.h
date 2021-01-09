#pragma once

#include <stddef.h>
#include <stdint.h>

#define ACCESS_ATTR_NULL     0x00
#define ACCESS_ATTR_PRESENT  (1 << 7)
#define ACCESS_ATTR_RING_3   (1 << 5 | 1 << 6)
#define ACCESS_ATTR_CODEDATA (1 << 4)
#define ACCESS_ATTR_EXEC     (1 << 3)
#define ACCESS_ATTR_READABLE (1 << 1)
#define ACCESS_ATTR_WRITABLE (1 << 1)
#define ACCESS_ATTR_GROWDOWN (1 << 2)
#define ACCESS_ATTR_ALLOWLOW (1 << 2)

#define FLAG_ATTR_NULL   0x00
#define FLAG_ATTR_GRAN1B 0x00
#define FLAG_ATTR_GRAN4K (1 << 3)
#define FLAG_ATTR_S16B   0x00
#define FLAG_ATTR_S32B   (1 << 2)
#define FLAG_ATTR_S64B   (1 << 1)

void LoadGDT();
