/*
 *  Copyright (C) 2013-2014  Soloviov Dmitry
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

#ifndef XSHELL2_H_
#define XSHELL2_H_

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "doscard.h"

#define XSHELL_DEF_WND_W 800
#define XSHELL_DEF_WND_H 600
#define XSHELL_CAPTION "DosCard XSHELL II"
#define BITFILE_ALTPATH "../libdoscard/libdbwrap.bc"

typedef struct SxsSDL {
	SDL_AudioDeviceID audio;
	SDL_Window* wnd;
	SDL_Renderer* ren;
} XSSDL;

typedef struct SsxDOSM {
	SDL_Texture* frame;
	doscard::CDosCard* m;
	SDL_Rect rrect;
} XSDOSM;

typedef std::vector<XSDOSM*> DOSMachines;

bool PushMachine();
void PopMachine();
void ClearMachines();
void UpdateMachine(int n);
void AddMachineEvents(int n, SDL_Event e);

#define NSDLRECT(R,A,B,C,D) {R.x = A; R.y = B; R.w = C; R.h = D;}

#endif /* XSHELL2_H_ */
