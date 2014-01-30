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

#include "xshell.h"
#include "xsupport.h"

/*
 * The Basic model:
 * 1. Run SDL window
 * 2. Start Box thread
 * 2.a. Box thread calls UpdateScreenBuffer() every it's own frame
 * 2.b. Box thread calls UpdateSoundBuffer() every time the buffer is ready
 * 2.c. Box thread calls QueryUIEvents() every time the thread ready to process them
 * 2.note. This mechanism is close enough to what will happen on a real chip
 * (video generator may be a separate chip, which connected to main chip via UART/SPI/I2C
 * or may be just a timer's interrupt handler; audio output controller may be implemented
 * as simple timer interrupt, which updates the other timer's values to change duty
 * cycle of PWM generated; keyboard handler is a pin change interrupt handler too;
 * all of this parts will never directly affect the main code)
 *
 * Excessive debug output needed for future stress tests and profiling. So better I'll
 * write it now :)
 */

SDL_Window *wnd;
SDL_Renderer *ren;

int XS_UpdateScreenBuffer(void* buf, size_t len)
{
#ifdef XSHELL_VERBOSE
	xnfo(0,2,"len=%d",len);
#endif
	return 0;
}

int XS_UpdateSoundBuffer(void* buf, size_t len)
{
#ifdef XSHELL_VERBOSE
	xnfo(0,3,"len=%d",len);
#endif
	return 0;
}

int XS_QueryUIEvents(void* buf, size_t len)
{
#ifdef XSHELL_VERBOSE
	xnfo(0,4,"len=%d",len);
#endif
	return 0;
}

int XS_GetTicks(void* buf, size_t len)
{
#ifdef XSHELL_VERBOSE
	xnfo(0,5,"len=%d",len);
#endif
	return 0;
}

static void XS_ldb_register()
{
	Dosbox_RegisterCallback(DBCB_GetTicks,&XS_GetTicks);
	Dosbox_RegisterCallback(DBCB_PushScreen,&XS_UpdateScreenBuffer);
	Dosbox_RegisterCallback(DBCB_PushSound,&XS_UpdateSoundBuffer);
	Dosbox_RegisterCallback(DBCB_PullUIEvents,&XS_QueryUIEvents);
#ifdef XSHELL_VERBOSE
	xnfo(0,6,"finished");
#endif
}

static int XS_SDLInit()
{
	wnd = NULL;
	ren = NULL;
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		xnfo(1,7,"SDL2 Init Error");
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(XSHELL_DEF_WND_W,XSHELL_DEF_WND_H,
			SDL_WINDOW_OPENGL,&wnd,&ren)) {
		xnfo(1,7,"CreateWindowAndRenderer() failed.");
		return 2;
	}
	if (SDL_SetRenderDrawColor(ren,0,0,0,255)) {
		xnfo(1,7,"SetRenderDrawColor() failed.");
		return 10;
	}
	SDL_SetWindowTitle(wnd,XSHELL_CAPTION);
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);
#ifdef XSHELL_VERBOSE
	xnfo(0,7,"Init OK");
#endif
	return 0;
}

static void XS_SDLKill()
{
#ifdef XSHELL_VERBOSE
	xnfo(0,8,"enter");
#endif
	if (ren) SDL_DestroyRenderer(ren);
	if (wnd) SDL_DestroyWindow(wnd);
	SDL_Quit();
}

void* XS_SDLoop(void* p)
{
	SDL_Event e;
	for(;;) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) return 0;
		}
		SDL_RenderClear(ren);
		SDL_RenderPresent(ren);
	}
}

int main(int argc, char* argv[])
{
	pthread_t dosbox,sdloop;
	xnfo(0,1,"ALIVE!");

	if (XS_SDLInit()) xnfo(-1,1,"Unable to create SDL2 context!");
	xnfo(0,1,"SDL2 context created successfully");

	XS_ldb_register();

	if (pthread_create(&dosbox,NULL,Dosbox_Run,NULL))
		xnfo(-1,1,"Unable to create DOS thread!");
	xnfo(0,1,"DOSBox Thread running!");
	if (pthread_create(&sdloop,NULL,XS_SDLoop,NULL))
		xnfo(-1,1,"Unable to create SDL thread!");
	xnfo(0,1,"SDLoop Thread running!");

	if (pthread_join(dosbox,NULL)) xnfo(-1,1,"Threading error!");
	xnfo(0,1,"DOSBox Thread Exited");

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"XShell",
			"Main thread exited. You can close the window now!",NULL);

	if (pthread_join(sdloop,NULL)) xnfo(-1,1,"Threading error!");
	xnfo(0,1,"SDLoop Thread Exited");

	XS_SDLKill();
	xnfo(0,1,"QUIT");
	return (EXIT_SUCCESS);
}
