#include "lib8001.h"
#include <string.h>

#define ATTR

__sfr __at 0x08 KBD8;
__sfr __at 0xf0 NET;

static void process(char c) {
	static u8 e, y;
	u8 *p, *q;
	switch (e) {
	case 0:
		switch (c) {
		case 8:
			cursX--;
			break;
		case 0x1a:
			cls();
			break;
		case 0x1b:
			e = 1;
			break;
		default:
			putchar(c);
			break;
		}
		break;
	case 1:
		switch(c) {
		case '(':
#ifdef ATTR
			color(0);
#endif
			e = 0;
			break;
		case ')':
#ifdef ATTR
			color(4);
#endif
			e = 0;
			break;
		case '=':
			e = 2;
			break;
		case 'E':
			memmove(vramLine(cursY + 1), vramLine(cursY), VRAM_XN * (23 - cursY));
			e = 0;
			break;
		case 'R':
			memmove(vramLine(cursY), vramLine(cursY + 1), VRAM_XN * (23 - cursY));
			e = 0;
			break;
		case 'T':
			p = vramLine(cursY);
			q = p + SCREEN_XN;
			for (p += cursX; p < q;) *p++ = 0;
			e = 0;
			break;
		}
		break;
	case 2:
		y = c;
		e = 3;
		break;
	case 3:
		locate(c - ' ', y - ' ');
		e = 0;
		break;
	}
}

void main() {
	baseInit();
	setupScreen(80, 25, 0, 0, 0);
#ifndef ATTR
	color(-1);
#endif
	while (1) {
		char c = NET;
		if (c) {
			process(c);
			cursorOn();
		}
		else {
			c = keyDown(1);
			if (c) {
				switch (c) { // convert cursor key to WordStar style
				case 0x1c:
					c = 4;
					break;
				case 0x1d:
					c = 19;
					break;
				case 0x1e:
					c = 5;
					break;
				case 0x1f:
					c = 24;
					break;
				}
				NET = KBD8 & 0x80 ? c : c - 0x60;
			}
			idle();
		}
	}
}
