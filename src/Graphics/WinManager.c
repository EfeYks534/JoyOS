#include "WinMan.h"
#include "Graphics.h"
#include <Sched/Task.h>
#include <string.h>

static struct Task win_task = { 0 };

static struct Window windows[64] = { 0 };

static uint64_t av_windows = -1;

struct Window *WindowNew()
{
	uint8_t i = __builtin_ffsll(av_windows) - 1;
	if(i > 63) return NULL;

	av_windows &= ~(1 << i);
	memset(&windows[i], 0, sizeof(struct Window));
	return &windows[i];
}

void WindowDelete(struct Window *win)
{
	if(win < &windows[0] || win > &windows[63]) return;

	uint64_t id = win - &windows[0];
	av_windows |= (1 << id);
}

void WinMan()
{
	while(1) {
		GrFillRect(0, 0, GrWidth(), GrHeight(), 0x00AABBCC);

		for(int i = 0; i < 64; i++) {
			if((av_windows & (1 << i)) == 0) {
				WindowRender(&windows[i]);
				windows[i].should_render = 1;
				TaskYield();
			}
		}

		GrFlush();
		TaskYield();
	}
}

void WinManStart()
{
	TaskNew(&win_task, WinMan);
}
