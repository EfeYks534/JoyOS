#pragma once

#define SCHACT_PIPE 0x0

void SchInit();

void SchSleep(uint64_t ticks);

void SchWaitUntil(uint8_t act, uint64_t data);
