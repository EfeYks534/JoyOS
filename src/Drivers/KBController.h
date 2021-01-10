#pragma once

#include "PS2Controller.h"

int64_t KBPipe();

void KBInit();

void KBHandler0(struct Registers *regs);
