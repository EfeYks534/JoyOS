#pragma once

#include <stddef.h>
#include <stdint.h>

#define KernelCodeSelector ((uint64_t) 0x8)

#define KernelDataSelector ((uint64_t) 0x10)

#define INTCAST(x) ((uint64_t)x)

#define PACKED __attribute__((packed))

#define INTERRUPT_HANDLER __attribute__((interrupt))
