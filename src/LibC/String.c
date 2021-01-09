#include <stdint.h>
#include <stddef.h>
#include "string.h"

#define uint8p(ptr) ((uint8_t*)ptr)

void *memset(void *dest, uint8_t src, size_t size)
{
	for(size_t i = 0; i < size; i++)
		((uint8_t*)dest)[i] = src;

	return dest;
}

void *memrep(void *dest, const void *src, size_t size, size_t n)
{
	for(size_t i = 0; i < n; i++)
		((uint8_t*)dest)[i] = ((const uint8_t*)src)[i % size];

	return dest;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	if(s1 == s2)
		return 0;

	for(size_t i = 0; i < n; i++)
		if(uint8p(s1)[i] != uint8p(s2)[i])
			return ( uint8p(s1)[i] > uint8p(s2)[i] ) ? 1 : -1;

	return 0;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	for(size_t i = 0; i < n; i++)
		uint8p(dest)[i] = uint8p(src)[i];
}

void *memmove(void *dest, const void *src, size_t n)
{
	uint8_t arr[n];

	for(size_t i = 0; i < n; i++)
		arr[i] = uint8p(src)[i];

	for(size_t i = 0; i < n; i++)
		uint8p(dest)[i] = arr[i];
}

size_t strlen(const char *str)
{
	size_t len = 0;
	while(str[len] != 0) len++;
	return len;
}
