#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stivale.h>

void *PhysAlloc();

void PhysReserve(void *addr, size_t size);

void PhysFree(void *addr, size_t size);

void PhysInit(struct stivale_struct *boot_data);

void *MMap(void *vir_addr, void *phys_addr);

void MemInit(struct stivale_struct *boot_data);

void *MemAlloc();

void *MemCAlloc(size_t size);

void MemFree(void *ptr);
