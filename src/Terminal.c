#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pipe.h>
#include <Sched/Task.h>
#include <Sched/Timer.h>
#include <Graphics/Graphics.h>
#include <Graphics/WinMan.h>

#define TWIDTH  80
#define THEIGHT 60

static struct Window *term_win = NULL;

static uint16_t term_buf[4800] = { 0 };

static uint32_t term_colors[] =
{
	0x00000000, // BLACK
	0x0027347C, // BLUE
	0x00357C31, // GREEN
	0x00137593, // CYAN
	0x006B1111, // RED
	0x0053116B, // MAGENTA
	0x006B4511, // BROWN
	0x00C9C4BB, // LGRAY
	0x00635F58, // DGRAY
	0x004861EA, // LBLUE
	0x0073E06D, // LGREEN
	0x0046BEE2, // LCYAN
	0x00EF5F5F, // LRED
	0x00C662EA, // LMAGENTA
	0x00AF864B, // LBROWN
	0x00F7F4EF, // WHITE
};

static int32_t term_x = 0;
static int32_t term_y = 0;

static void TermPutChar(uint8_t ch)
{
	switch(ch)
	{
	case '\f':
		for(int i = 0; i < 4800; i++)
			term_buf[i] = 0x0F00;

		term_x = 0;
		term_y = 0;
		break;
	case '\r':
		term_x = 0;
		break;
	case '\n':
		term_y++;
		term_x = 0;
		break;
	case '\b':
		term_x--;
		term_buf[term_x + term_y * TWIDTH] &= 0xFF00;
		break;
	case '\t':
		for(int i = 0; i < 4; i++)
			TermPutChar(' ');
		break;
	default:
		term_buf[term_x + term_y * TWIDTH] &= 0xFF00;
		term_buf[(term_x++) + term_y * TWIDTH] |= ch;
		break;
	}

	if(term_x >= TWIDTH) {
		term_y++;
		term_x = 0;
	}

	if(term_y >= THEIGHT) {
		for(int y = 0; y < (THEIGHT - 1); y++)
			for(int x = 0; x < TWIDTH; x++)
				term_buf[x + y * TWIDTH] = term_buf[x + (y + 1) * TWIDTH];
		for(int x = 0; x < TWIDTH; x++)
			term_buf[x + ((THEIGHT - 1) * TWIDTH)] = 0x0F00;
		term_x = 0;
		term_y = (THEIGHT - 1);
	}

	if(term_x < 0) term_x = 0;
	if(term_y < 0) term_y = 0;

}
static void TermDraw(struct Window *win, void *arg)
{
	while(pstat(stdin)) {
		uint8_t ch = 0;
		read(&ch, 1, stdin);
		TermPutChar(ch);
	}
	while(pstat(stdout)) {
		uint8_t ch = 0;
		read(&ch, 1, stdout);
		TermPutChar(ch);
	}

	for(int y = 0; y < THEIGHT; y++) {
		for(int x = 0; x < TWIDTH; x++) {
			uint16_t entry = term_buf[x + y * TWIDTH];
			uint32_t bg = term_colors[entry >> 12];
			uint32_t fg = term_colors[(entry >> 8) & 0x0F];

			GrFillRect(x * 8, y * 8, 8, 8, bg);

			if((entry & 0x00FF) != 0)
				GrDrawChar(entry & 0xFF, x * 8, y * 8, fg);
		}
	}
}

void TerminalEmulator()
{
	term_win = WindowNew();
	term_win->pos_x  = 192;
	term_win->pos_y  = 174;
	term_win->width  = 640;
	term_win->height = 480;
	term_win->title  = "Terminal Emulator";
	term_win->on_draw = TermDraw;
	term_win->arg = NULL;

	for(int i = 0; i < 4800; i++)
		term_buf[i] = 0x0F00;

	while(1) {
		if(term_win->should_render)
			GrRender(term_win);

		TaskYield();
	}
}
