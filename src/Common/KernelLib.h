#pragma once

#include "Macros.h"
#include <stddef.h>
#include <stdint.h>

struct RSDP {
	char   signature[8];
	uint8_t    checksum;
	char       oemid[6];
	uint8_t    revision;
	uint32_t  rsdt_addr;
	uint32_t     length;
	uint64_t  xsdt_addr;
	uint8_t  exchecksum;
	uint8_t reserved[3];
} PACKED;

void Hang();

void KernelPanic(char *str, ...);
