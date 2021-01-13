#include <stdint.h>
#include <stddef.h>
#include <Memory/MemMng.h>
#include <Core/IDTLib.h>
#include <Common/Registers.h>
#include <Common/KernelLib.h>
#include "Task.h"

extern void TaskSwitch(uint8_t *stack_top);

static struct
{
	struct Task *running;
	struct Task     main;
} tasks = { 0 };

void Schedule(struct Registers *regs)
{
	struct Task *last = tasks.running;

	last->stack_top = (uint8_t*) regs;

	if(tasks.running->next == NULL)
		tasks.running = &tasks.main;
	else
		tasks.running = tasks.running->next;

	TaskSwitch(tasks.running->stack_top);
}

void TaskInit(uint8_t *main_stack)
{
	tasks.running = &tasks.main;
}

void TaskNew(struct Task *task, void(*entry)())
{
	uint8_t *stack = (uint8_t*)((uint64_t)MemCAlloc(2) + 8192);
	uint8_t *adj_stack = stack - sizeof(struct Registers);

	task->stack_top = adj_stack;
	struct Registers *frame = (struct Registers*) adj_stack;

	frame->cs = 0x08;
	frame->ss = 0x10;
	frame->flags = 1 << 9;
	frame->rsp = (uint64_t) stack;
	frame->rip = (uint64_t) entry;

	tasks.running->next = task;
	task->last = tasks.running;
}

void TaskDelete(struct Task *task)
{
	if(task == &tasks.main) return;

	if(tasks.running == task)
		TaskYield();

	task->last->next = task->next;
	task->next->last = task->last;
}

void TaskSleep(uint64_t nsecs);
