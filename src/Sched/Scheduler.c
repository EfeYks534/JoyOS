#include <pipe.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <Memory/MemMng.h>
#include <Core/IDTLib.h>
#include <Common/Registers.h>
#include <Common/KernelLib.h>
#include <Graphics/Graphics.h>
#include "Task.h"
#include "Timer.h"

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

next:
	if(tasks.running->next == NULL)
		tasks.running = &tasks.main;
	else {
		tasks.running = tasks.running->next;
		if(tasks.running->state == TASK_SLEEPING) {
			if(*TimerClock() < tasks.running->info) goto next;
			else tasks.running->state = TASK_RUNNING;
		}
		if(tasks.running->state == TASK_PIPING) {
			if(!pstat(tasks.running->info)) goto next;
			else tasks.running->state = TASK_RUNNING;
		}
	}

	TaskSwitch(tasks.running->stack_top);
}

void TaskInit(uint8_t *main_stack)
{
	tasks.running = &tasks.main;
}

static void NewTaskEntry(void (*entry)())
{
	entry();
	TaskDelete(tasks.running);

	while(1) asm volatile("hlt");
}

struct Task *TaskGet()
{
	return tasks.running;
}

void TaskNew(struct Task *task, void(*entry)())
{
	uint8_t *stack = MemCAlloc(4);

	if(stack == NULL) KernelPanic("Can't allocate memory for stack");

	stack = (uint8_t*) ((uint64_t)stack + 16383);

	uint8_t *adj_stack = stack - sizeof(struct Registers);

	task->stack_top = adj_stack;
	struct Registers *frame = (struct Registers*) adj_stack;

	frame->cs = 0x08;
	frame->ss = 0x10;
	frame->flags = 1 << 9;
	frame->rsp = (uint64_t) stack;
	frame->rip = (uint64_t) NewTaskEntry;
	frame->rdi = (uint64_t) entry;

	tasks.running->next = task;
	task->last = tasks.running;
}

void TaskDelete(struct Task *task)
{
	if(task == &tasks.main) return;

	task->last->next = task->next;
	task->next->last = task->last;
}

void TaskSleep(uint64_t nsecs)
{
	tasks.running->state = TASK_SLEEPING;
	tasks.running->info = *TimerClock() + nsecs;
	while(*TimerClock() < tasks.running->info) TaskYield();
}
