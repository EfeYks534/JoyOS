#include "Timer.h"
#include <Common/KernelLib.h>
#include <Core/PIC.h>
#include <Core/IOLib.h>

#define MAX_FREQ 1193180
#define PIT_DATA 0x40
#define PIT_CMD  0x43

static uint64_t frequency = 0;

static uint64_t pit_clock = 0;

static uint64_t pit_rand = 2;

static uint64_t pit_rand2 = 0;

static void SetFrequency(uint32_t ticks)
{
	if(ticks > MAX_FREQ)
		KernelPanic("PIT: Can't set timer frequency to %d", ticks);

	frequency = ticks;

	uint32_t div = MAX_FREQ / ticks;

	if(div > 0xFFFF)
		KernelPanic("PIT: Divisor %d is too big", div);

	Out8(PIT_CMD, 0x36);

	Out8(PIT_DATA, div & 0x00FF);
	Out8(PIT_DATA, (div & 0xFF00) >> 8);
}

static void PITHandler()
{
	pit_clock++;
	if(((pit_clock & pit_rand) & 2) == 0) {
		uint64_t r1 = ( (pit_rand << 3) | (pit_rand >> 61) ) + (pit_clock & 3);
		pit_rand = r1 ^ ( (pit_rand >> 3) | (pit_rand << 61) ) + (pit_clock & 3) ^ pit_rand2;
	} else {
		pit_rand2 = ((pit_rand << 8) | (pit_rand >> 56)) ^ pit_rand;
	}
}

uint64_t TimerRand()
{
	return pit_rand ^ pit_rand2;
}

uint64_t *TimerClock()
{
	return &pit_clock;
}

void TimerStart()
{
	SetFrequency(10000);
	IRQSetHandler(0, PITHandler);
}

void TimerStop();

void TimerDiscover();
