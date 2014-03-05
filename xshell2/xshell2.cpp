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

static CDosCard* card = NULL;
static XSSDL sdl;

static int SDLInit()
{
	sdl.wnd = NULL;
	sdl.ren = NULL;
	sdl.frame = NULL;
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
	if (sdl.frame) SDL_DestroyTexture(sdl.frame);
	SDL_Quit();
}

static void DrawUI()
{
	SDL_RenderClear(sdl.ren);
	//
	SDL_RenderPresent(sdl.ren);
}

static void SDLoop()
{
	SDL_Event e;
	LDB_UIEvent mye;
	uint32_t i;
	bool quit = false;
	xnfo(0,9,"Loop begins");
	do {

		/* Event Processing*/
		while (SDL_PollEvent(&e)) {

			memset(&mye,0,sizeof(mye));
			switch (e.type) {

			case SDL_QUIT:
				mye.t = LDB_UIE_QUIT;
				quit = true;
				break;

			default: continue;
			}

//			evt_fifo.insert(evt_fifo.begin(),mye);
		}

		/* Frame Processing*/

		/* Update Window*/
		DrawUI();
		SDL_Delay(5);
	} while (!quit);
}

void exit_override(void)
{
	printf("exit_override()\n");
	LibDosCardExit();
	_exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
	int r;
	LibDosCardInit(1);
	xnfo(0,1,"ALIVE!");
	card = new CDosCard(true);
	xnfo(0,1,"CDosCard created");

	r = 0;
	if (card->GetCurrentState() != DOSCRD_LOADED) {
		r = 1;
		xnfo(2,1,"Default loading failed. Trying with another path...");
		if (card->TryLoad("../libdoscard/libdbwrap.bc")) {
			xnfo(0,1,"Loaded successfully!");
			r = 0;
		}
	}
	if (r) {
		xnfo(1,1,"Unable to load ldbw!");
		goto quit;
	}

	xnfo(0,1,"Version info:\n%s\n",card->GetVersionStringSafe());
//	if (!card->Prepare()) {
//		xnfo(1,1,"Prepare() failed.");
//		goto quit;
//	}
	xnfo(0,1,"Prepared successfully");

//	SDLInit();
//	SDLoop();
//	SDLKill();
quit:
	xnfo(0,1,"Finalizing...");
	delete card;
	LibDosCardExit();
	xnfo(0,1,"QUIT");
	atexit(exit_override);
	return 0; //to make compiler happy
}
