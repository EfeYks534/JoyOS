#pragma once

#include <stdint.h>
#include <stddef.h>

void Out8(uint16_t port, uint8_t data);

void Out16(uint16_t port, uint16_t data);

void Out32(uint16_t port, uint32_t data);

uint8_t In8(uint16_t port);

uint16_t In16(uint16_t port);

uint32_t In32(uint16_t port);

void Out8W(uint16_t port, uint8_t data);

void Out16W(uint16_t port, uint16_t data);

void Out32W(uint16_t port, uint32_t data);

void IOWait();
