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
static DOSMachines cc; //computing center :)
static int active;

static int SDLInit()
{
	sdl.wnd = NULL;
	sdl.ren = NULL;
	sdl.audio = 0;
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) {
		xnfo(-1,2,"SDL2 Init Error");
		return 1;
	}

#ifdef XSHELL2_TTFOUT
	if (TTF_Init()) {
		xnfo(1,2,"SDL2 TTF Init Error");
		return 2;
	}
#endif

	if (SDL_CreateWindowAndRenderer(XSHELL_DEF_WND_W,XSHELL_DEF_WND_H,
			SDL_WINDOW_RESIZABLE,&sdl.wnd,&sdl.ren)) {
		xnfo(1,2,"CreateWindowAndRenderer() failed.");
		return 3;
	}

#ifdef XSHELL2_TTFOUT
	sdl.fnt = TTF_OpenFont(XSHELL_FONTFILE,12);
	if (!sdl.fnt)
		xnfo(1,2,"Couldn't load font %s",XSHELL_FONTFILE);
	sdl.txtcol.a = 0xff;
	sdl.txtcol.r = 127;
	sdl.txtcol.g = 127;
	sdl.txtcol.b = 127;
#else
	sdl.fnt = NULL;
#endif

	SDL_SetWindowTitle(sdl.wnd,XSHELL_CAPTION);
	SDL_RenderClear(sdl.ren);
	SDL_RenderPresent(sdl.ren);
	return 0;
}

/*This function shouldn't be used until at least one machine have
 * valid sound data. Otherwise, we are silent, and won't init
 * audio subsystem.
 */
static void SDLInitAudio(LDB_SoundInfo* req)
{
	//
}

static void SDLKill()
{
	if (sdl.audio) SDL_CloseAudioDevice(sdl.audio);
	if (sdl.ren) SDL_DestroyRenderer(sdl.ren);
	if (sdl.wnd) SDL_DestroyWindow(sdl.wnd);
#ifdef XSHELL2_TTFOUT
	if (sdl.fnt) TTF_CloseFont(sdl.fnt);
	TTF_Quit();
#endif
	SDL_Quit();
}

inline void ClearUI()
{
	SDL_SetRenderDrawColor(sdl.ren,0,0,0,255);
	SDL_RenderClear(sdl.ren);
}

static char* LinearMachineOutput(int n, bool clear)
{
	int i = 0;
	LDBI_MesgVec::iterator I;
	LDBI_MesgVec* msgs = cc[n]->m->GetMessages();
	if ((!msgs) || (msgs->empty())) return NULL;

	for (I=msgs->begin(); I != msgs->end(); ++I)
		i += I->length() + 1;

	if (i < 1) return NULL;

	char* res = reinterpret_cast<char*> (malloc(++i));
	memset(res,0,i);

	for (I=msgs->begin(); I != msgs->end(); ++I) {
		strncat(res,I->c_str(),i-1);
		i -= I->length();
		if (i > 0) strncat(res,"\n",i-1);
	}

	if (clear) msgs->clear();
	return res;
}

static void DrawUI()
{
	int i,j;
	unsigned int k;
	SDL_Rect frm,cur,tmp;
	XSDOSM* mach;
	SDL_Surface* surf;
	SDL_Texture* txd;
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
	NSDLRECT(frm,0,0,(ww/w)-1,(wh/h)-1);

	ClearUI();
	cur = frm;
	k = 0;
	for (i=0; i<w; i++) {
		for (j=0; j<h; j++) {
			SDL_SetRenderDrawColor(sdl.ren,255,0,0,255);
			SDL_RenderDrawRect(sdl.ren,&cur);
			tmp = cur;
			tmp.x++;
			tmp.y++;
#ifdef XSHELL2_TTFOUT
			tmp.w = (tmp.w - 2);
			tmp.h = (tmp.h - 2) / 3 * 2;
#else
			tmp.w -= 2;
			tmp.h -= 2;
#endif
			if (k < cc.size()) {
				if (k == active) {
					SDL_SetRenderDrawColor(sdl.ren,0,255,0,255);
					SDL_RenderFillRect(sdl.ren,&cur);
				}
				mach = cc[k];
				if (mach->frame)
					SDL_RenderCopy(sdl.ren,mach->frame,NULL,&tmp);
				if (sdl.fnt) {
#ifdef XSHELL2_TTFOUT
					char* lin = LinearMachineOutput(k,false);
					if (lin) {
						printf("lin = '%s'\n",lin);
						surf = TTF_RenderText_Solid(sdl.fnt,lin,sdl.txtcol);
						free(lin);
					}
#endif
				} else
					surf = NULL;
				if (surf) {
					tmp.y += tmp.h;
					tmp.h /= 2;
					txd = SDL_CreateTextureFromSurface(sdl.ren,surf);
					if (txd) {
						SDL_RenderCopy(sdl.ren,txd,NULL,&tmp);
						SDL_DestroyTexture(txd);
					}
					SDL_FreeSurface(surf);
				}
			} else {
				SDL_SetRenderDrawColor(sdl.ren,30,0,0,255);
				SDL_RenderFillRect(sdl.ren,&cur);
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
	unsigned int i;
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

				case SDL_SCANCODE_KP_4:
					active--;
					break;

				case SDL_SCANCODE_KP_6:
					active++;
					break;

				case SDL_SCANCODE_KP_7:
					PauseActive(true);
					break;

				case SDL_SCANCODE_KP_9:
					PauseActive(false);
					break;

				default: break;
				}
				break;

			default:
				break;
			}

			if (static_cast<unsigned> (active) >= cc.size())
				active = cc.size() - 1;
			if (active < 0) active = 0;
		}

		/* Machines */
		for (i=0; i<cc.size(); i++) UpdateMachine(i);

		/* Update Window*/
		DrawUI();
		SDL_Delay(5);

	} while (!quit);
}

bool PushMachine()
{
	int r;
	XSDOSM* mach = new XSDOSM;
	memset(mach,0,sizeof(XSDOSM));

	CDosCard* card = new CDosCard(true);
	xnfo(0,6,"Machine created");

	r = 0;
	if (card->GetCurrentState() != DOSCRD_LOADED) {
		r = 1;
		xnfo(2,6,"Default loading failed. Trying with another path...");
		if (card->TryLoad(BITFILE_ALTPATH)) {
			xnfo(0,6,"Loaded successfully!");
			r = 0;
		}
	}
	if (r) {
		xnfo(1,6,"Unable to load ldbw!");
		return false;
	}

	xnfo(0,6,"Version info:\n%s\n",card->GetVersionStringSafe());
	if (!card->Prepare()) {
		xnfo(1,1,"Prepare() failed.");
		return false;
	}
	xnfo(0,6,"Prepared successfully");

	mach->m = card;

	memset(mach->sound.buf,0,SNDRING_BUFLEN*sizeof(LDBI_SndSample));
	mach->sound.rd = 0;
	mach->sound.wr = 0;

	cc.push_back(mach);
	return true;
}

void PopMachine()
{
	if (cc.empty()) return;
	XSDOSM* mach = cc.back();
	cc.pop_back();
	if (mach->m) delete mach->m;
	if (mach->frame) SDL_DestroyTexture(mach->frame);
	delete mach;
	xnfo(0,7,"Machine destroyed");
}

void ClearMachines()
{
	xnfo(0,8,"Cleaning machines (%d)...",cc.size());
	while (!cc.empty()) PopMachine();
}

void UpdateMachine(int n)
{
	MACH_INBOUND(n);
	XSDOSM* mach = cc[n];
	EDOSCRDState stat = mach->m->GetCurrentState();
	if (stat == DOSCRD_INITED)
		mach->m->Run();
	else if (stat != DOSCRD_RUNNING)
		return;

	//Video Update
	int w,h;
	uint32_t* frmbuf = mach->m->GetFramebuffer(&w,&h);
	if ((w != mach->rrect.w) || (h != mach->rrect.h)) {
		mach->rrect.w = w;
		mach->rrect.h = h;
		if (mach->frame)
			SDL_DestroyTexture(mach->frame);
		mach->frame = SDL_CreateTexture(sdl.ren,SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,w,h);
	}
	if (mach->frame)
		SDL_UpdateTexture(mach->frame,NULL,frmbuf,mach->rrect.w*sizeof(uint32_t));

#ifndef XSHELL2_TTFOUT
	//Message Processing
	char* outstr = LinearMachineOutput(n,true);
	if (outstr) {
		xnfo(0,9,"[MACHINE %d MSG]: %s",n,outstr);
		free(outstr);
	}
#endif

	//FIXME
	uint32_t r = mach->m->FillSound(mach->sound.buf+mach->sound.wr,SNDRING_BUFLEN);
	if (r > 0) {
		 if (!sdl.audio) {
			 LDB_SoundInfo fmt;
			 mach->m->GetSoundFormat(&fmt);
			 xnfo(0,9,"Setting audio. F=%d",fmt.freq);
			 SDLInitAudio(&fmt);
		 }
		 mach->sound.wr += r;
		 //TODO
	}
}

void AddMachineEvents(int n, SDL_Event e)
{
	MACH_INBOUND(n);
	if (cc[n]->m->GetCurrentState() != DOSCRD_RUNNING) return;
	unsigned int i;
	LDB_UIEvent mye;
	memset(&mye,0,sizeof(mye));
	switch (e.type) {
	case SDL_KEYDOWN:
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
	default:
		return;
	}
//	xnfo(0,10,"Adding event {%hx} to machine %d",mye.t,n);
	cc[n]->m->PutEvent(mye);
}

void PauseActive(bool p)
{
	MACH_INBOUND(active);
	xnfo(0,11,"active = %d, pause req = %hd",active,p);
	cc[active]->m->SetPause(p);
}

int main(int /*argc*/, char** /*argv*/)
{
	LibDosCardInit(1);
	xnfo(0,1,"ALIVE!");
	if (!PushMachine()) _exit(EXIT_FAILURE);
	active = 0;

	SDLInit();
	SDLoop();

	xnfo(0,1,"Finalizing...");
	ClearMachines();
	SDLKill();

	xnfo(0,1,"QUIT");
	LibDosCardExit();
	_exit(EXIT_SUCCESS);
}
