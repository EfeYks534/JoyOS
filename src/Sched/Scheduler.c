#include <stdint.h>
#include <stddef.h>
#include <Memory/MemMng.h>
#include <Common/Registers.h>
#include "Task.h"

static struct
{
	struct Task *running;
	struct Task     main;
} tasks;

void TaskInit(uint8_t *main_stack)
{
	tasks.running = &tasks.main;

	tasks.main.task_stack = main_stack;
}

void TaskNew(struct Task *task, void(*entry)(void*), void *arg)
{
	task->task_stack = MemCAlloc(2);

	tasks.running->next = task;
	task->next = NULL;
}

void TaskDelete(struct Task *task);

void TaskSleep(uint64_t nsecs);

void TaskYield();
