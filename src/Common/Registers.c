#include "Registers.h"
#include <string.h>

static struct Registers current = { 0 };

void SaveConInt();

void SaveConInfo(struct Registers *reg)
{
	memcpy(&current, reg, sizeof(struct Registers));
}

struct Registers GetRegisters()
{
	asm volatile("int $0x70");
	return current;
}

