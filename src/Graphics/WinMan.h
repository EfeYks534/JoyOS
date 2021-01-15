#pragma once

#include <stddef.h>
#include <stdint.h>

struct Window
{
	char     *title;
	int32_t   width;
	int32_t  height;
	int32_t   pos_x;
	int32_t   pos_y;

	uint8_t should_render;

	void (*on_draw)(struct Window*, void*);
	void *arg;
};

struct Window *WindowNew();

void WindowRender(struct Window *win);

void WindowDelete(struct Window *win);

void WinManStart();
