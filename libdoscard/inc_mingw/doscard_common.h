/*
 *  Copyright (C) 2015  Dmitry Soloviov
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

#ifndef DOSCARD_COMMON_H_
#define DOSCARD_COMMON_H_


/* Common Inclusion */
#include <vector>
#include <string>
#include <stdarg.h>

#include "fake_inttypes.h"
#include "keyboard.h"
#include "ldbconf.h"


/* ****************** Events Pipe Data Structures ****************** */

namespace dosbox {

enum LDB_UIEventE {
	LDB_UIE_NONE = 0,
	LDB_UIE_QUIT = 1,
	LDB_UIE_KBD = 2,
	LDB_UIE_MOUSE = 3
};

typedef struct {
	float x,y;
} LDBFloat2D;

typedef struct {
	LDBFloat2D rel,abs;
	uint8_t button;
} LDB_MOUSEINF;

typedef struct {
	LDB_UIEventE t; 	//event type
	KBD_KEYS key;		//keyboard key
	bool pressed;		//is pressed?
	LDB_MOUSEINF m;		//mouse info
} LDB_UIEvent;

typedef struct {
	uint8_t width,channels;
	uint32_t freq,blocksize;
	bool sign,silent;
} LDB_SoundInfo;

} // namespace dosbox


/* ****************** DosCard data types ****************** */

#include "..\include\doscard_types.h"


#endif /* DOSCARD_COMMON_H_ */
