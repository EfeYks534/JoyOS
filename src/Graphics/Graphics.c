#include "Graphics.h"
#include "WinMan.h"
#include <Core/IOLib.h>
#include <stivale.h>
#include <string.h>
#include "Font8x8.h"

static uint64_t lim_width;
static uint64_t lim_height;

static uint64_t start_x;
static uint64_t start_y;

static uint64_t  fb_width;
static uint64_t  fb_height;
static uint64_t  fb_bpp;
static uint64_t  fb_pitch;
static uint32_t *fb_addr;

static struct Window windows[16] = { 0 };

static uint8_t windows_top = 0;

static struct Window *context = &windows[0];

struct Window *WindowNew()
{
	return &windows[windows_top++];
}

void WindowRender(struct Window *win)
{
	start_x = win->pos_x;
	start_y = win->pos_y;

	lim_width  = win->width;
	lim_height = win->height;

	win->on_draw(win);

	start_x = start_y = 0;

	lim_width  = fb_width;
	lim_height = fb_height;

	GrFillRect(win->pos_x-1, win->pos_y-14, win->width+1, 14, 0x00347499);
	GrRect(win->pos_x-1, win->pos_y-14, win->width+1, 14, 0x004A1212);
	GrRect(win->pos_x-1, win->pos_y-1, win->width+1, win->height+1, 0x004A1212);

	GrDrawText(win->title, win->pos_x + 2, win->pos_y - 11, 0x00FFFFFF);
}

void WindowDelete(struct Window *win)
{
	win->pos_x = win->pos_x;
}

void GrPlot(int32_t x, int32_t y, uint32_t color)
{
	if(x >= lim_width || y >= lim_height || x < 0 || y < 0) return;
	fb_addr[(start_x + x) + (start_y + y) * fb_width] = color;
}

void GrInit(struct stivale_struct *data)
{
	fb_width = data->framebuffer_width;
	fb_height = data->framebuffer_height;
	fb_bpp = data->framebuffer_bpp;
	fb_pitch = data->framebuffer_pitch;
	fb_addr = (uint32_t*) data->framebuffer_addr;

	if(fb_bpp != 32) {
		while(1)
			asm volatile("cli\nhlt");
	}

	uint32_t color = 0x00AABBCC;

	memrep(fb_addr, &color, 4, fb_width * fb_height * 4);

	lim_width  = fb_width;
	lim_height = fb_height;
	start_x = 0;
	start_y = 0;
}

uint32_t GrWidth()
{
	return fb_width;
}

uint32_t GrHeight()
{
	return fb_height;
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
