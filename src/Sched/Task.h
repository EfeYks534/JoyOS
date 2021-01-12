#pragma once

#include <stddef.h>
#include <stdint.h>
#include <Common/Registers.h>

struct Task
{
	struct Registers regs;

	uint8_t  *task_stack;
	uint64_t sleep_until;

	struct Task *next;
};

void TaskInit(uint8_t *main_stack);

void TaskNew(struct Task *task, void(*entry)(void*), void *arg);

void TaskDelete(struct Task *task);

void TaskSleep(uint64_t nsecs);

void TaskYield();
