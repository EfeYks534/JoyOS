#pragma once

struct Window
{
	char     *title;
	uint32_t  width;
	uint32_t height;
	uint32_t  pos_x;
	uint32_t  pos_y;

	void (*on_draw)(struct Window*);

	struct Window *last;
	struct Window *next;
};

struct Window *WindowNew();

void WindowRender(struct Window *win);

void WindowDelete(struct Window *win);
