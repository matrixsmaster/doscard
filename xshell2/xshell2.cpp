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


static SDL_Texture* frame_sdl = NULL;
static SDL_AudioDeviceID audio = 0;

using namespace std;
using namespace dosbox;

static int XS_SDLInit()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) {
		xnfo(1,7,"SDL2 Init Error");
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(XSHELL_DEF_WND_W,XSHELL_DEF_WND_H,
			SDL_WINDOW_RESIZABLE,&wnd,&ren)) {
		xnfo(1,7,"CreateWindowAndRenderer() failed.");
		return 2;
	}
	lcd_w = XSHELL_DEF_WND_W;
	lcd_h = XSHELL_DEF_WND_H;
	if (SDL_SetRenderDrawColor(ren,0,0,0,255)) {
		xnfo(1,7,"SetRenderDrawColor() failed.");
		return 10;
	}
	SDL_SetWindowTitle(wnd,XSHELL_CAPTION);
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);
	SDL_AtomicSet(&at_flag,0);
#if XSHELL_VERBOSE
	xnfo(0,7,"Init OK");
#endif
	return 0;
}

static void XS_SDLKill()
{
#if XSHELL_VERBOSE
	xnfo(0,8,"killing...");
#endif
	if (audio) SDL_CloseAudioDevice(audio);
	if (ren) SDL_DestroyRenderer(ren);
	if (wnd) SDL_DestroyWindow(wnd);
	if (frame_sdl) SDL_DestroyTexture(frame_sdl);
	SDL_Quit();
}

static void XS_SDLoop()
{
	SDL_Event e;
	LDB_UIEvent mye;
	uint32_t i;
	bool quit = false;
	xnfo(0,9,"Loop begins");
	do {
		while (!SDL_AtomicGet(&at_flag))
			if (!doscard) return;			//DOS thread had finished

		/* Event Processing*/
		while (SDL_PollEvent(&e)) {

			memset(&mye,0,sizeof(mye));
			switch (e.type) {

			case SDL_QUIT:
				mye.t = LDB_UIE_QUIT;
				quit = true;
				break;

			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_PAUSE) {
					frame_block ^= 1;
					xnfo(0,9,"frame_block=%d",frame_block);
					continue;
				}
				//no break
			case SDL_KEYUP:
				mye.t = LDB_UIE_KBD;
				mye.pressed = (e.type == SDL_KEYDOWN);
				mye.key = KBD_NONE;
				//FIXME: use key-list!
				for (i=0; i<(sizeof(XShellKeyboardMap)/sizeof(XShellKeyboardPair)); i++)
					if (XShellKeyboardMap[i].sdl == e.key.keysym.scancode) {
						mye.key = XShellKeyboardMap[i].db;
						break;
					}
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				mye.pressed = (e.button.state == SDL_PRESSED);
				switch (e.button.button) {
				case SDL_BUTTON_LEFT:	mye.m.button = 1; break;
				case SDL_BUTTON_RIGHT:	mye.m.button = 2; break;
				default:				mye.m.button = 3; break;
				}
				//no break
			case SDL_MOUSEMOTION:
				mye.t = LDB_UIE_MOUSE;
				mye.m.rel.x = static_cast<float>(e.motion.xrel);
				mye.m.rel.y = static_cast<float>(e.motion.yrel);
				mye.m.abs.x = static_cast<float>(e.motion.x);
				mye.m.abs.y = static_cast<float>(e.motion.y);
				break;

			default: continue;
			}

			evt_fifo.insert(evt_fifo.begin(),mye);
#if XSHELL_VERBOSE
			xnfo(0,9,"evt_fifo[] size = %d",evt_fifo.size());
#endif
		}

		/* Frame Processing*/
		if ((!frame_block) && (framebuf)) {
			if (frame_dirty) {
#if XSHELL_VERBOSE
				xnfo(0,9,"frame is dirty");
#endif
				if (frame_sdl) SDL_DestroyTexture(frame_sdl);
				frame_sdl = SDL_CreateTexture(ren,SDL_PIXELFORMAT_ARGB8888,
						SDL_TEXTUREACCESS_STREAMING,lcd_w,lcd_h);
				frame_dirty = false;
			}
			if (frame_sdl) {
				SDL_UpdateTexture(frame_sdl,NULL,framebuf,lcd_w*sizeof(uint32_t));
				SDL_RenderClear(ren);
				SDL_RenderCopy(ren,frame_sdl,NULL,NULL);
			}
		}
		SDL_AtomicSet(&at_flag,0);

		/* Update Window*/
		SDL_RenderPresent(ren);
		SDL_Delay(5);
	} while (!quit);
	SDL_AtomicSet(&at_flag,-10);
}

void XS_AudioCallback(void* userdata, uint8_t* stream, int len)
{
	int i,p;
	int16_t* buf = reinterpret_cast<int16_t*>(stream);
	len >>= 1;
	//FIXME: better use memcpy()
	p = sndring.read;
	for (i=0; i<len; i++) {
		buf[i] = sndring.data[p];
		if (++p >= XSHELL_SOUND_LENGTH) p = 0;
	}
	sndring.read = p;
#if XSHELL_VERBOSE > 1
	xnfo(0,12,"%d : %d",sndring.read,sndring.write);
#endif
}

int DosRun(void* p)
{
	doscard->Execute();
	delete doscard;
	doscard = NULL;
	return 0;
}

int main(int argc, char* argv[])
{
	int r;
	xnfo(0,1,"ALIVE!");

	doscard = new CDosBox();
	if (doscard) xnfo(0,1,"Class instance created");
	else abort();

	if (XS_SDLInit()) xnfo(-1,1,"Unable to create SDL2 context!");
	xnfo(0,1,"SDL2 context created successfully");

	XS_ldb_register();

	dosboxthr = SDL_CreateThread(DosRun,"DosThread",NULL);
	if (!dosboxthr) xnfo(-1,1,"Unable to create DOS thread!");
	xnfo(0,1,"DOS Thread running!");

	XS_SDLoop();
	xnfo(0,1,"SDLoop() Exited");

	SDL_WaitThread(dosboxthr,&r);
	xnfo(0,1,"DOS Thread Exited (%d)",r);

	XS_SDLKill();

	xnfo(0,1,"QUIT");
	return (EXIT_SUCCESS);
}
