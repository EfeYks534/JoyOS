#pragma once

#include <stddef.h>
#include <stdint.h>
#include <Common/Registers.h>

#define TASK_RUNNING  0x0
#define TASK_SLEEPING 0x1
#define TASK_PIPING   0x2

struct Task
{
	uint8_t *stack_top;
	uint8_t      state;
	uint64_t      info;
	struct Task  *next;
	struct Task  *last;
};

struct Task *TaskGet();

void Schedule(struct Registers *regs);

void TaskInit(uint8_t *main_stack);

void TaskNew(struct Task *task, void(*entry)());

void TaskDelete(struct Task *task);

void TaskSleep(uint64_t nsecs);

void TaskYield();
