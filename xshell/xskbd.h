/*
 *  Copyright (C) 2013-2014  Dmitry Soloviov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef XSKBD_H_
#define XSKBD_H_

#include <SDL2/SDL.h>
#include "keyboard.h"

namespace dosbox {

typedef struct {
	KBD_KEYS db; //dosbox key
	SDL_Scancode sdl; //sdl key
} XShellKeyboardPair;

static const XShellKeyboardPair XShellKeyboardMap[] = {

		{KBD_0,			SDL_SCANCODE_0},
		{KBD_1,			SDL_SCANCODE_1},
		{KBD_2,			SDL_SCANCODE_2},
		{KBD_3,			SDL_SCANCODE_3},
		{KBD_4,			SDL_SCANCODE_4},
		{KBD_5,			SDL_SCANCODE_5},
		{KBD_6,			SDL_SCANCODE_6},
		{KBD_7,			SDL_SCANCODE_7},
		{KBD_8,			SDL_SCANCODE_8},
		{KBD_9,			SDL_SCANCODE_9},

		{KBD_a,			SDL_SCANCODE_A},
		{KBD_b,			SDL_SCANCODE_B},
		{KBD_c,			SDL_SCANCODE_C},
		{KBD_d,			SDL_SCANCODE_D},
		{KBD_e,			SDL_SCANCODE_E},
		{KBD_f,			SDL_SCANCODE_F},
		{KBD_g,			SDL_SCANCODE_G},
		{KBD_h,			SDL_SCANCODE_H},
		{KBD_i,			SDL_SCANCODE_I},
		{KBD_j,			SDL_SCANCODE_J},
		{KBD_k,			SDL_SCANCODE_K},
		{KBD_l,			SDL_SCANCODE_L},
		{KBD_m,			SDL_SCANCODE_M},
		{KBD_n,			SDL_SCANCODE_N},
		{KBD_o,			SDL_SCANCODE_O},
		{KBD_p,			SDL_SCANCODE_P},
		{KBD_q,			SDL_SCANCODE_Q},
		{KBD_r,			SDL_SCANCODE_R},
		{KBD_s,			SDL_SCANCODE_S},
		{KBD_t,			SDL_SCANCODE_T},
		{KBD_u,			SDL_SCANCODE_U},
		{KBD_v,			SDL_SCANCODE_V},
		{KBD_w,			SDL_SCANCODE_W},
		{KBD_x,			SDL_SCANCODE_X},
		{KBD_y,			SDL_SCANCODE_Y},
		{KBD_z,			SDL_SCANCODE_Z},

		{KBD_grave,		SDL_SCANCODE_GRAVE},
		{KBD_minus,		SDL_SCANCODE_MINUS},
		{KBD_equals,	SDL_SCANCODE_EQUALS},
		{KBD_backslash,	SDL_SCANCODE_BACKSLASH},
		{KBD_leftbracket,SDL_SCANCODE_LEFTBRACKET},
		{KBD_rightbracket,SDL_SCANCODE_RIGHTBRACKET},
		{KBD_semicolon,	SDL_SCANCODE_SEMICOLON},
		//{KBD_quote,		},
		{KBD_period,	SDL_SCANCODE_PERIOD},
		{KBD_comma,		SDL_SCANCODE_COMMA},
		{KBD_slash,		SDL_SCANCODE_SLASH},
		//{KBD_extra_lt_gt,},

		{KBD_enter,		SDL_SCANCODE_RETURN},
		{KBD_backspace,	SDL_SCANCODE_BACKSPACE},
		{KBD_space,		SDL_SCANCODE_SPACE},
		{KBD_tab,		SDL_SCANCODE_TAB},
		{KBD_esc,		SDL_SCANCODE_ESCAPE},

		{KBD_f1,		SDL_SCANCODE_F1},
		{KBD_f2,		SDL_SCANCODE_F2},
		{KBD_f3,		SDL_SCANCODE_F3},
		{KBD_f4,		SDL_SCANCODE_F4},
		{KBD_f5,		SDL_SCANCODE_F5},
		{KBD_f6,		SDL_SCANCODE_F6},
		{KBD_f7,		SDL_SCANCODE_F7},
		{KBD_f8,		SDL_SCANCODE_F8},
		{KBD_f9,		SDL_SCANCODE_F9},
		{KBD_f10,		SDL_SCANCODE_F10},
		{KBD_f11,		SDL_SCANCODE_F11},
		{KBD_f12,		SDL_SCANCODE_F12},

		{KBD_leftalt,	SDL_SCANCODE_LALT},
		{KBD_rightalt,	SDL_SCANCODE_RALT},
		{KBD_leftctrl,	SDL_SCANCODE_LCTRL},
		{KBD_rightctrl, SDL_SCANCODE_RCTRL},
		{KBD_leftshift,	SDL_SCANCODE_LSHIFT},
		{KBD_rightshift,SDL_SCANCODE_RSHIFT},

		{KBD_printscreen,SDL_SCANCODE_PRINTSCREEN},
		{KBD_pause,		SDL_SCANCODE_PAUSE},
		{KBD_insert,	SDL_SCANCODE_INSERT},

		{KBD_home,		SDL_SCANCODE_HOME},
		{KBD_end,		SDL_SCANCODE_END},
		{KBD_pageup,	SDL_SCANCODE_PAGEUP},
		{KBD_pagedown,	SDL_SCANCODE_PAGEDOWN},
		{KBD_delete,	SDL_SCANCODE_DELETE},
	    {KBD_right,		SDL_SCANCODE_RIGHT},
	    {KBD_left,		SDL_SCANCODE_LEFT},
	    {KBD_down,		SDL_SCANCODE_DOWN},
	    {KBD_up,		SDL_SCANCODE_UP}
};

}// namespace dosbox

#endif /* XSKBD_H_ */
