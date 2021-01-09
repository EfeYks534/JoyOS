#pragma once

#include <stddef.h>
#include <stdint.h>

void *memset(void *dest, uint8_t src, size_t size);

void *memrep(void *dest, const void *src, size_t size, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

void *memcpy(void *dest, const void *src, size_t n);

void *memmove(void *dest, const void *src, size_t n);

size_t strlen(const char *str);
