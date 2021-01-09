#include "pipe.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <Core/IOLib.h>
#include <Scheduler/Scheduler.h>
#include <Common/KernelLib.h>

typedef struct Pipe
{
	uint8_t buf[8192];
	uint32_t     read;
	uint32_t    write;
	uint8_t   changed;
} Pipe;

static Pipe pipes[64] = { 0 };
static int  pipe_top = 2;

int64_t popen()
{
	if(pipe_top > 63) return -1;

	return pipe_top++;
}

void flush(int64_t pid)
{
	Pipe *pipe = &pipes[pid];

	pipe->read = 0;
	pipe->write = 0;
	pipe->changed = 0;
}

int pstat(int64_t pid)
{
	return pipes[pid].write != pipes[pid].read;
}

void poll(int64_t pid)
{
	SchWaitUntil(SCHACT_PIPE, pid);
}

static void putpipe(uint8_t data, int64_t pid)
{
	Pipe *pipe = &pipes[pid];

	pipe->write = (pipe->write + 1) % 8192;
	if(pipe->write == pipe->read)
		KernelPanic("Pipe overflow on pipe %d", pid);

	pipe->buf[pipe->write - 1] = data;
}

static uint8_t readpipe(int64_t pid)
{
	Pipe *pipe = &pipes[pid];

	if(pipe->read == pipe->write) {
		uint8_t data = pipe->buf[pipe->read];
		pipe->changed = 0;
		return data;
	}

	pipe->read = (pipe->read + 1) % 8192;

	return pipe->buf[pipe->read - 1];
}


#define INTCAST(x) ((uint8_t*)x)

void write(const void *buf, size_t len, int64_t pid)
{
	for(size_t i = 0; i < len; i++)
		putpipe(INTCAST(buf)[i], pid);

	pipes[pid].changed = 1;
}

void read(void *buf, size_t len, int64_t pid)
{
 	for(size_t i = 0; i < len; i++)
		INTCAST(buf)[i] = readpipe(pid);
}
