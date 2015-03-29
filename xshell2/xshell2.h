/*
 *  Copyright (C) 2013-2015  Dmitry Soloviov
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
#include "../xshell/xskbd.h"

#ifdef XSHELL2_TTFOUT
#include <SDL2/SDL_ttf.h>
#endif

#define XSHELL_DEF_WND_W 800
#define XSHELL_DEF_WND_H 600
#define XSHELL_CAPTION "DosCard XSHELL II"
#define BITFILE_ALTPATH "../libdoscard/src/libdbwrap.bc"
#define XSHELL_FONTFILE "LiberationMono-Regular.ttf"
#define SNDRING_BUFLEN (44100*2)
#define SNDRING_ONESHOT 1024

typedef struct SxsSDL {
	SDL_AudioDeviceID audio;
	SDL_Window* wnd;
	SDL_Renderer* ren;
#ifdef XSHELL2_TTFOUT
	TTF_Font* fnt;
#else
	void* fnt;
#endif
	SDL_Color txtcol;
	bool autosize;
} XSSDL;

typedef struct SxsSndRing {
	doscard::LDBI_SndSample buf[SNDRING_BUFLEN];
	uint32_t rd,wr;
} XSSndRing;

typedef struct SxsDOSM {
	SDL_Texture* frame;
	doscard::CDosCard* m;
	SDL_Rect rrect;
	XSSndRing sound;
	uint32_t ileave;
	int cdepth;
} XSDOSM;

typedef std::vector<XSDOSM*> DOSMachines;

bool PushMachine();
void PopMachine();
void ClearMachines();
void UpdateMachine(int n);
void AddMachineEvents(int n, SDL_Event e);
void PauseActive(bool p);
void ModILeaveActive(int32_t mod);
void XS_AudioCallback(void* userdata, uint8_t* stream, int len);

#define NSDLRECT(R,A,B,C,D) {R.x = A; R.y = B; R.w = C; R.h = D;}
#define MACH_INBOUND(N) if ((N < 0) || (N >= (int)cc.size())) return

#endif /* XSHELL2_H_ */
