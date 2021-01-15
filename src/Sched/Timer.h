#pragma once

#include <stddef.h>
#include <stdint.h>

#define TIMER_SEC_NSEC 1000000000

uint64_t *TimerClock();

uint64_t TimerRand();

void TimerStart();

void TimerStop();

void TimerDiscover();
