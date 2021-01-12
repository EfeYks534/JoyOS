#pragma once

#include <stddef.h>
#include <stdint.h>

void MemInit();

void *MemAlloc();

void *MemCAlloc(size_t size);

void MemFree(void *ptr);
