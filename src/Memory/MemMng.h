#pragma once

#include <stddef.h>
#include <stdint.h>

void MemInit();

void *MemAlloc();

void MemFree(void *ptr);
