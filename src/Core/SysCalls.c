#include "IDTLib.h"
#include <Common/Macros.h>
#include <Common/KernelLib.h>
#include <stdint.h>
#include <stddef.h>

void SysCallHandler(struct Registers *reg)
{
	KernelPanic("SysCalls aren't supported yet!");
}

