#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <pipe.h>
#include <stddef.h>
#include <Core/IOLib.h>
#include <Core/PIC.h>
#include <Common/Registers.h>
#include "KBController.h"
#include "PS2Controller.h"

static const uint8_t single_bytes_key[] =
{
	-1,
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
	VK_ASTERISK,
	VK_DASH,
	VK_BACKSPACE,
	VK_TAB,
	VK_Q,
	VK_W,
	VK_E,
	VK_R,
	VK_T,
	VK_Y,
	VK_U,
	VK_I,
	VK_O,
	VK_P,
	VK_SOFT_G,
	VK_SOFT_U,
	VK_ENTER,
	VK_LCTRL,
	VK_A,
	VK_S,
	VK_D,
	VK_F,
	VK_G,
	VK_H,
	VK_J,
	VK_K,
	VK_L,
	VK_SOFT_S,
	VK_I,
	VK_QUOTE,
	VK_LSHIFT,
	VK_COMMA,
	VK_Z,
	VK_X,
	VK_C,
	VK_V,
	VK_B,
	VK_N,
	VK_M,
	VK_SOFT_O,
	VK_SOFT_C,
	VK_DOT,
	VK_RSHIFT,
	VK_NP_ASTERISK,
	VK_LALT,
	VK_SPACE,
	VK_CAPSLOCK,
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
	VK_NUMLOCK,
	VK_SCROLL_LK,
	VK_NP7,
	VK_NP8,
	VK_NP9,
	VK_NP_DASH,
	VK_NP4,
	VK_NP5,
	VK_NP6,
	VK_NP_PLUS,
	VK_NP1,
	VK_NP2,
	VK_NP3,
	VK_NP0,
	VK_NP_DOT,
	-1, -1,
	VK_NP7,
	VK_F11,
	VK_F12
};

static uint8_t state = 0;

static uint8_t kb_keys[256] = { 0 };

static uint64_t kbpipe = -1;

int64_t KBPipe()
{
	return kbpipe;
}

static uint8_t capslock = 0;

uint8_t TranslateKey(uint8_t key)
{
	uint8_t shift = kb_keys[VK_LSHIFT] || kb_keys[VK_RSHIFT];
	uint8_t ctrl = kb_keys[VK_LCTRL];
	if(capslock)
		shift = !shift;

	switch(key)
	{
	case VK_1: return (!shift) ? '1' : '!';
	case VK_2: return (!shift) ? '2' : '\'';
	case VK_3: return (!shift) ? '3' : '^';
	case VK_4: return (!shift) ? '4' : '+';
	case VK_5: return (!shift) ? '5' : '%';
	case VK_6: return (!shift) ? '6' : '&';
	case VK_7: return (!shift) ? '7' : '/';
	case VK_8: return (!shift) ? '8' : '(';
	case VK_9: return (!shift) ? '9' : ')';
	case VK_0: return (!shift) ? '0' : '=';
	case VK_ASTERISK: return (!shift) ? '*' : '?';
	case VK_DASH: return (!shift) ? '-' : '_';
	case VK_A: return (!shift) ? 'a' : 'A';
	case VK_S: return (!shift) ? 's' : 'S';
	case VK_D: return (!shift) ? 'd' : 'D';
	case VK_F: return (!shift) ? 'f' : 'F';
	case VK_G: return (!shift) ? 'g' : 'G';
	case VK_H: return (!shift) ? 'h' : 'H';
	case VK_J: return (!shift) ? 'j' : 'J';
	case VK_K: return (!shift) ? 'k' : 'K';
	case VK_L: return (ctrl)   ?'\f' : (!shift) ? 'l' : 'L';
	case VK_Q: return (!shift) ? 'q' : 'Q';
	case VK_W: return (!shift) ? 'w' : 'W';
	case VK_E: return (!shift) ? 'e' : 'E';
	case VK_R: return (!shift) ? 'r' : 'R';
	case VK_T: return (!shift) ? 't' : 'T';
	case VK_Y: return (!shift) ? 'y' : 'Y';
	case VK_U: return (!shift) ? 'u' : 'U';
	case VK_I: return (!shift) ? 'i' : 'I';
	case VK_O: return (!shift) ? 'o' : 'O';
	case VK_P: return (!shift) ? 'p' : 'P';
	case VK_Z: return (!shift) ? 'z' : 'Z';
	case VK_X: return (!shift) ? 'x' : 'X';
	case VK_C: return (!shift) ? 'c' : 'C';
	case VK_V: return (!shift) ? 'v' : 'V';
	case VK_B: return (!shift) ? 'b' : 'B';
	case VK_N: return (!shift) ? 'n' : 'N';
	case VK_M: return (!shift) ? 'm' : 'M';
	case VK_NP7: return (!shift) ? '<' : '>';
	case VK_SOFT_G: return (!shift) ? '\xF0' : '\xD0';
	case VK_SOFT_U: return (!shift) ? '\xFC' : '\xDC';
	case VK_SOFT_S: return (!shift) ? '\xFE' : '\xDE';
	case VK_SOFT_O: return (!shift) ? '\xF6' : '\xD6';
	case VK_SOFT_C: return (!shift) ? '\xE7' : '\xC7';
	case VK_COMMA: return (!shift) ? ',' : ';';
	case VK_DOT: return (!shift) ? '.' : ':';
	case VK_SPACE: return ' ';
	case VK_ENTER: return '\n';
	case VK_TAB: return '\t';
	case VK_BACKSPACE: return '\b';
	default: return 0;
	}
}

void KBHandler0(struct Registers *regs)
{
	while(DataAvailable(CHANNEL_ONE)) {
		uint8_t scancode = PS2ReadData();

		uint8_t released = scancode & 0x80;
		uint8_t raw_key = scancode & 0x7F;

		uint8_t tr_key = single_bytes_key[raw_key];

		if((tr_key == VK_CAPSLOCK) && released && !kb_keys[tr_key])
			capslock = !capslock;

		kb_keys[tr_key] = !released;

		tr_key = TranslateKey(tr_key);

		if(tr_key != 0 && !released)
			write(&tr_key, 1, stdin);

		if(!released)
			write(&tr_key, 1, kbpipe);
	}
}

void KBInit()
{
	kbpipe = popen();
}
