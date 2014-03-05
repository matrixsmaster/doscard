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

#include "xshell2.h"
#include "xsupport.h"

using namespace std;
using namespace dosbox;
using namespace doscard;

static XSSDL sdl;
static DOSMachines cc; //computing centre :)
static int active;

static int SDLInit()
{
	sdl.wnd = NULL;
	sdl.ren = NULL;
	sdl.audio = 0;
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) {
		xnfo(1,2,"SDL2 Init Error");
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(XSHELL_DEF_WND_W,XSHELL_DEF_WND_H,
			SDL_WINDOW_RESIZABLE,&sdl.wnd,&sdl.ren)) {
		xnfo(1,2,"CreateWindowAndRenderer() failed.");
		return 2;
	}
	if (SDL_SetRenderDrawColor(sdl.ren,0,0,0,255)) {
		xnfo(1,2,"SetRenderDrawColor() failed.");
		return 10;
	}
	SDL_SetWindowTitle(sdl.wnd,XSHELL_CAPTION);
	SDL_RenderClear(sdl.ren);
	SDL_RenderPresent(sdl.ren);
	return 0;
}

static void SDLKill()
{
	if (sdl.audio) SDL_CloseAudioDevice(sdl.audio);
	if (sdl.ren) SDL_DestroyRenderer(sdl.ren);
	if (sdl.wnd) SDL_DestroyWindow(sdl.wnd);
	SDL_Quit();
}

inline void ClearUI()
{
	SDL_SetRenderDrawColor(sdl.ren,0,0,0,255);
	SDL_RenderClear(sdl.ren);
}

static void DrawUI()
{
	int i,j;
	unsigned int k;
	SDL_Rect frm,cur,tmp;
	XSDOSM mach;
	if (cc.empty()) {
		ClearUI();
		SDL_RenderPresent(sdl.ren);
		return;
	}
	float nf = static_cast<float> (cc.size());
	int h = static_cast<int> (floor(sqrt(nf)));
	int w = static_cast<int> (ceil(nf / h));

	int ww,wh;
	SDL_GetWindowSize(sdl.wnd,&ww,&wh);
	NSDLRECT(frm,0,0,(ww/w),(wh/h));

	ClearUI();
	SDL_SetRenderDrawColor(sdl.ren,255,0,0,255);
	cur = frm;
	k = 0;
	for (i=0; i<w; i++) {
		for (j=0; j<h; j++) {
			SDL_RenderDrawRect(sdl.ren,&cur);
			tmp = cur;
			tmp.x--;
			tmp.y--;
			tmp.w--;
			tmp.h--;
			if (k < cc.size()) {
				mach = cc[k];
				if (mach.frame)
					SDL_RenderCopy(sdl.ren,mach.frame,NULL,&tmp);
			} else {
				SDL_RenderFillRect(sdl.ren,&tmp);
			}
			k++;
			cur.y += frm.h + 1;
		}
		cur.x += frm.w + 1;
		cur.y = frm.y;
	}
	SDL_RenderPresent(sdl.ren);
}

static void SDLoop()
{
	SDL_Event e;
	bool quit = false;
	xnfo(0,5,"Loop begins");
	do {
		/* Event Processing*/
		while (SDL_PollEvent(&e)) {
			AddMachineEvents(active,e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYUP:
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_KP_PLUS:
					if (PushMachine()) xnfo(0,5,"Machine added");
					else xnfo(1,5,"Unable to add new machine");
					break;

				case SDL_SCANCODE_KP_MINUS:
					PopMachine();
					break;

				default: break;
				}
				break;

			default:
				break;
			}
		}
		/* Update Window*/
		DrawUI();
		SDL_Delay(5);
	} while (!quit);
}

bool PushMachine()
{
	int r;
	XSDOSM mach;
	memset(&mach,0,sizeof(mach));

//	CDosCard* card = new CDosCard(true);
//	xnfo(0,6,"Machine created");
//
//	r = 0;
//	if (card->GetCurrentState() != DOSCRD_LOADED) {
//		r = 1;
//		xnfo(2,6,"Default loading failed. Trying with another path...");
//		if (card->TryLoad(BITFILE_ALTPATH)) {
//			xnfo(0,6,"Loaded successfully!");
//			r = 0;
//		}
//	}
//	if (r) {
//		xnfo(1,6,"Unable to load ldbw!");
//		return false;
//	}
//
//	xnfo(0,6,"Version info:\n%s\n",card->GetVersionStringSafe());
//	if (!card->Prepare()) {
//		xnfo(1,1,"Prepare() failed.");
//		return false;
//	}
//	xnfo(0,6,"Prepared successfully");

//	mach.m = card;
	mach.frame = SDL_CreateTexture(sdl.ren,SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,640,400);
	cc.push_back(mach);
	return true;
}

void PopMachine()
{
	if (cc.empty()) return;
	XSDOSM mach = cc.back();
	cc.pop_back();
	if (mach.frame) SDL_DestroyTexture(mach.frame);
	if (mach.m) delete mach.m;
	xnfo(0,7,"Machine destroyed");
}

void ClearMachines()
{
	xnfo(0,8,"Cleaning machines (%d)...",cc.size());
	while (!cc.empty()) PopMachine();
}

void UpdateMachine(int n)
{
	//
}

void AddMachineEvents(int n, SDL_Event e)
{
	//
}

int main(int /*argc*/, char** /*argv*/)
{
	LibDosCardInit(1);
	xnfo(0,1,"ALIVE!");
	if (!PushMachine()) _exit(EXIT_FAILURE);
	active = 0;

	SDLInit();
	SDLoop();
	SDLKill();

	xnfo(0,1,"Finalizing...");
	ClearMachines();

	xnfo(0,1,"QUIT");
	LibDosCardExit();
	_exit(EXIT_SUCCESS);
}
