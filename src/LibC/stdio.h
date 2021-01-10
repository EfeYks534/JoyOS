#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

enum VirKey
{
	VK_ESC,
	VK_1,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,
	VK_0,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,
	VK_PRINTSC,
	VK_SCROLL_LK,
	VK_PAUSE,
	VK_BACKTICK,
	VK_DASH,
	VK_EQUAL,
	VK_BACKSPACE,
	VK_INSERT,
	VK_HOME,
	VK_PAGE_UP,
	VK_PAGE_DOWN,
	VK_NUMLOCK,
	VK_NP_SLASH,
	VK_NP_ASTERISK,
	VK_NP_ENTER,
	VK_NP_DASH,
	VK_NP_PLUS,
	VK_TAB,
	VK_A,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z,
	VK_OPEN_BRACK,
	VK_CLOSE_BRACK,
	VK_BACKSLASH,
	VK_PIPE,
	VK_DEL,
	VK_END,
	VK_NP1,
	VK_NP2,
	VK_NP3,
	VK_NP4,
	VK_NP5,
	VK_NP6,
	VK_NP7,
	VK_NP8,
	VK_NP9,
	VK_NP0,
	VK_SEMICOLON,
	VK_APOSTROPHE,
	VK_ENTER,
	VK_LSHIFT,
	VK_RSHIFT,
	VK_COMMA,
	VK_DOT,
	VK_SLASH,
	VK_UP,
	VK_LEFT,
	VK_RIGHT,
	VK_DOWN,
	VK_LCTRL,
	VK_LLOGO,
	VK_LALT,
	VK_SPACE,
	VK_RALT,
	VK_RLOGO,
	VK_CONMENU,
	VK_RCTRL,
	VK_NP_DOT,
	VK_SOFT_G,
	VK_UPPER_I,
	VK_SOFT_S, // ş
	VK_SOFT_C, // ç
	VK_SOFT_U, // ü
	VK_SOFT_O, // ö
	VK_QUOTE,
	VK_CAPSLOCK,
	VK_ASTERISK
};


#define stdin  ((int64_t) 0)
#define stdout ((int64_t) 1)

void putchar(char ch);

int printf(const char *fmt, ...);

int vprintf(const char *fmt, va_list ap);

int sprintf(char *str, const char *fmt, ...);

int snprintf(char *str, int len, const char *fmt, ...);

int vsprintf(char *str, const char *fmt, va_list ap);

int vsnprintf(char *str, int len, const char *fmt, va_list ap);
