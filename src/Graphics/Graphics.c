#include "Graphics.h"
#include <Memory/MemMng.h>
#include "WinMan.h"
#include <Sched/Task.h>
#include <stivale.h>
#include <string.h>
#include "Font8x8.h"

static int64_t lim_width;
static int64_t lim_height;

static int64_t start_x;
static int64_t start_y;

static uint64_t  fb_width;
static uint64_t  fb_height;
static uint64_t  fb_bpp;
static uint64_t  fb_pitch;
static uint32_t *fb_addr;

static uint32_t *framebuffer;

void GrRestoreLimits()
{
	start_x = 0;
	start_y = 0;
	lim_width = fb_width;
	lim_height = fb_height;
}

void GrRender(struct Window *win)
{
	win->should_render = 0;

	start_x = win->pos_x;
	start_y = win->pos_y;

	lim_width  = win->width;
	lim_height = win->height;

	void (*func)(struct Window *win, void *arg) = win->on_draw;
	if(func != NULL)
		func(win, win->arg);

	start_x = 0;
	start_y = 0;

	lim_width  = fb_width;
	lim_height = fb_height;
}

void GrFlush()
{
	int size = fb_width * fb_height;
	for(int i = 0; i < size; i++)
		fb_addr[i] = framebuffer[i];
}

void WindowRender(struct Window *win)
{
	GrFillRect(win->pos_x-1, win->pos_y-14, win->width+1, 14, 0x00347499);
	GrRect(win->pos_x-1, win->pos_y-14, win->width+1, 14, 0x004A1212);
	GrRect(win->pos_x-1, win->pos_y-1, win->width+1, win->height+1, 0x004A1212);

	GrDrawText(win->title, win->pos_x + 2, win->pos_y - 11, 0x00FFFFFF);
}

void GrPlot(int32_t x, int32_t y, uint32_t color)
{
	int32_t px = start_x + x;
	int32_t py = start_y + y;
	if((x >= lim_width) || (y >= lim_height) || (x < 0) || (y < 0)) return;
	if((px >= fb_width) || (py >= fb_height) || (px < 0) || (py < 0)) return;

	framebuffer[px + py * fb_width] = color;
}

void GrInit(struct stivale_struct *data)
{
	fb_width = data->framebuffer_width;
	fb_height = data->framebuffer_height;
	fb_bpp = data->framebuffer_bpp;
	fb_pitch = data->framebuffer_pitch;
	fb_addr = (uint32_t*) data->framebuffer_addr;
	framebuffer = MemCAlloc(fb_width * fb_height * 4 / 4096);

	if(fb_bpp != 32 || framebuffer == NULL) {
		asm volatile("cli\nhlt");
	}

	lim_width  = fb_width;
	lim_height = fb_height;
	start_x = 0;
	start_y = 0;
}

uint32_t GrWidth()
{
	return lim_width;
}

uint32_t GrHeight()
{
	return lim_height;
}

void GrSprite(uint32_t *sprite, uint32_t xx, uint32_t yy, uint32_t w, uint32_t h)
{
	for(uint32_t x = xx; x < xx + w; x++)
		for(uint32_t y = yy; y < yy + h; y++)
			GrPlot(x, y, sprite[(x-xx) + (y-yy) * w]);
}

void GrRect(uint32_t xx, uint32_t yy, uint32_t w, uint32_t h, uint32_t color)
{
	for(uint32_t x = xx; x < xx + w; x++) {
		GrPlot(x, yy, color);
		GrPlot(x, yy+h-1, color);
	}

	for(uint32_t y = yy; y < yy + h; y++) {
		GrPlot(xx, y, color);
		GrPlot(xx+w-1, y, color);
	}
}

void GrFillRect(uint32_t xx, uint32_t yy, uint32_t w, uint32_t h, uint32_t color)
{
	for(uint32_t x = xx; x < xx + w; x++)
		for(uint32_t y = yy; y < yy + h; y++)
			GrPlot(x, y, color);
}

static char (*font)[8] = font8x8_latin3;

void GrDrawChar(uint8_t c, uint32_t xx, uint32_t yy, uint32_t color)
{
	uint8_t set = 0;
	for(uint32_t x = 0; x < 8; x++)
		for(uint32_t y = 0; y < 8; y++) {
			set = font[c][x] & (1 << y);
			if(set) GrPlot(xx + y, yy + x, color);
		}
}

void GrDrawText(uint8_t *text, uint32_t xx, uint32_t yy, uint32_t color)
{
	size_t i = 0;
	while(text[i] != 0)
		GrDrawChar(text[i++], xx + 8 * i, yy, color);
}
