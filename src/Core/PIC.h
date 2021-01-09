#pragma once

#include <stdint.h>
#include <stddef.h>
#include <Common/Registers.h>

#define PIC1      0x20
#define PIC2      0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1
#define PIC_EOI   0x20

void PICRemap(uint32_t off1, uint32_t off2);

void IRQSetMask(uint8_t line);

void IRQClearMask(uint8_t line);

void PICSetupInterrupts();

uint16_t PICGetIRR();

uint16_t PICGetISR();

void IRQSetHandler(uint8_t index, void (*handler)(struct Registers *reg));
