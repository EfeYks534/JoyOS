#pragma once

#include <stdint.h>
#include <stddef.h>
#include <Common/Registers.h>

#define ATTR_RING0   (0x00)
#define ATTR_RING3   ( (1 << 5) | (1 << 6) )
#define ATTR_TASK    (1 | (1 << 2))
#define ATTR_INTR    ( (1 << 1) | (1 << 2) | (1 << 3) )
#define ATTR_TRAP    ( (1) | (1 << 1) | (1 << 2) | (1 << 3) )
#define ATTR_PRESENT (1 << 7)


typedef void (*ISR)();

void IDTRegisterISR(uint16_t index, uint8_t attr, ISR handler);

void IDTRegisterIntHandler(uint16_t index, ISR handler);

void SetupInterrupts();

void IDTInstall();
