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

#include "xshell2.h"
#include "xsupport.h"

using namespace std;
using namespace dosbox;
using namespace doscard;

static XSSDL sdl;				//some SDL2-related runtime data
static DOSMachines cc;			//our computing center :)
static int active;				//currently active machine index
static LDBI_PostProcess ppset;	//post-processing runtime settings (if any)

static int SDLInit()
{
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
	SDL_AudioSpec want,have;
	SDL_zero(want);
	if (req->silent) return;
	want.freq = req->freq;
	if (req->sign && (req->width == 16))
		want.format = AUDIO_S16SYS;
	else
		xnfo(-1,12,"Unsupported audio format");
	want.channels = req->channels;
	want.samples = req->blocksize;
	want.callback = XS_AudioCallback;
	sdl.audio = SDL_OpenAudioDevice(NULL,0,&want,&have,SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	if (!sdl.audio) {
		xnfo(1,12,"Audio device couldn't be opened!\nSilent mode.");
		return;
	}
	xnfo(0,12,"Audio opened successfully");
//	memset(sndring.data,0,sizeof(sndring.data));
//	sndring.read = 0;
//	sndring.write = 0;
//	sndring.paused = true;
	SDL_PauseAudioDevice(sdl.audio,0);
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
	int rw = 0;
	int rh = 0;

	for (i=0; i<w; i++) {
		for (j=0; j<h; j++) {
			SDL_SetRenderDrawColor(sdl.ren,255,0,0,255);
			SDL_RenderDrawRect(sdl.ren,&cur);

			rw = cur.x;
			rh = cur.y;

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
				mach = cc[k];
				if (k == active) {
					if (mach->m->GetCurrentState() == DOSCRD_PAUSED)
						SDL_SetRenderDrawColor(sdl.ren,255,255,0,255);
					else
						SDL_SetRenderDrawColor(sdl.ren,0,255,0,255);
					SDL_RenderFillRect(sdl.ren,&cur);
				}
				if (mach->frame) {
					SDL_RenderCopy(sdl.ren,mach->frame,NULL,&tmp);
					rw += mach->rrect.w;
					rh += mach->rrect.h;
				}
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
	if (sdl.autosize)
		SDL_SetWindowSize(sdl.wnd,rw,rh);
}

static void SDLoop()
{
	SDL_Event e;
	unsigned int i;
	bool ppupd, quit = false;

	xnfo(0,5,"Loop begins");
	do {
		ppupd = false;

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

				case SDL_SCANCODE_KP_1:
					ModILeaveActive(-1000);
					break;

				case SDL_SCANCODE_KP_3:
					ModILeaveActive(1000);
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

				case SDL_SCANCODE_KP_PERIOD:
					sdl.autosize ^= true;
					break;

				case SDL_SCANCODE_KP_DIVIDE:
					ppset.on ^= true;
					ppupd = true;
					break;

				case SDL_SCANCODE_KP_MULTIPLY:
					switch (ppset.typ) {
					case DOSCRD_VID_COLOR: ppset.typ = DOSCRD_VID_GS; break;
					case DOSCRD_VID_GS: ppset.typ = DOSCRD_VID_BW; break;
					case DOSCRD_VID_BW: ppset.typ = DOSCRD_VID_COLOR; break;
					}
					ppupd = true;
					break;

				case SDL_SCANCODE_PAUSE:
					ToggleUnlockActive();
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
		for (i=0; i<cc.size(); i++) {
			UpdateMachine(i);
			if ((i == active) && ppupd) cc[i]->m->ApplyPostProcess(&ppset);
		}

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

	mach->m->ApplyPostProcess(&ppset);

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
	uint32_t fmt;
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
		//create rectangle and destroy old texture
		mach->rrect.w = w;
		mach->rrect.h = h;
		if (mach->frame)
			SDL_DestroyTexture(mach->frame);
		//select format
		fmt = SDL_PIXELFORMAT_ARGB8888; //by default
		mach->cdepth = 4;
		switch (ppset.fmt) {
		case DOSCRD_VIDFMT_DWORD: break;
		case DOSCRD_VIDFMT_WORD:
			fmt = SDL_PIXELFORMAT_RGB555;
			mach->cdepth = 2;
			break;
		case DOSCRD_VIDFMT_BYTE:
			fmt = SDL_PIXELFORMAT_RGB332;
			mach->cdepth = 1;
			break;
		default:
			printf("Unsupported frame format!\n");
			break;
		}
		//create
		mach->frame = SDL_CreateTexture(sdl.ren,fmt,SDL_TEXTUREACCESS_STREAMING,w,h);
	}
	if (mach->frame)
		SDL_UpdateTexture(mach->frame,NULL,frmbuf,mach->rrect.w*mach->cdepth);

#ifndef XSHELL2_TTFOUT
	//Message Processing
	char* outstr = NULL;//LinearMachineOutput(n,true);
	if (outstr) {
		xnfo(0,9,"[MACHINE %d MSG]: %s",n,outstr);
		free(outstr);
	}
#endif

/*
	//FIXME
	uint32_t wrlen = (mach->sound.wr + SNDRING_ONESHOT >= SNDRING_BUFLEN)?
						(SNDRING_BUFLEN - mach->sound.wr):SNDRING_ONESHOT;
	uint32_t r = mach->m->FillSound(mach->sound.buf+mach->sound.wr,wrlen);
	if (r > 0) {
		 if (!sdl.audio) {
			 LDB_SoundInfo fmt;
			 if (mach->m->GetSoundFormat(&fmt)) {
				 xnfo(0,9,"Setting audio. F=%d",fmt.freq);
				 SDLInitAudio(&fmt);
			 } else {
				 xnfo(1,9,"Unable to get sound format");
				 return;
			 }
		 }
		 mach->sound.wr += r;
		 //TODO
	}
*/

	if (!sdl.audio) {
		LDB_SoundInfo fmt;
		if (mach->m->GetSoundFormat(&fmt)) {
			xnfo(0,9,"Setting audio. F=%d",fmt.freq);
			SDLInitAudio(&fmt);
		}
	}
}

void XS_AudioCallback(void* userdata, uint8_t* stream, int len)
{
	xnfo(0,13,"acal: %d",len);
	MACH_INBOUND(active);
	memset(stream,0,len);
	LDBI_SndSample* buf = reinterpret_cast<LDBI_SndSample*> (stream);
	cc[active]->m->FillSound(buf,len/sizeof(LDBI_SndSample));
}

void AddMachineEvents(int n, SDL_Event e)
{
	MACH_INBOUND(n);
	EDOSCRDState mst = cc[n]->m->GetCurrentState();
	if ((mst != DOSCRD_RUNNING) && (mst != DOSCRD_PAUSED)) return;
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

void ModILeaveActive(int32_t mod)
{
	MACH_INBOUND(active);
	uint32_t r = cc[active]->ileave;
	if ((int32_t)r + mod < 0) mod = -r;
	r += mod;
	xnfo(0,14,"active = %d, mod by %d, result = %u",active,mod,r);
	cc[active]->ileave = r;
	cc[active]->m->SetInterleave(r);
}

void ToggleUnlockActive()
{
	MACH_INBOUND(active);
	cc[active]->fastfwd ^= true;
	if (cc[active]->fastfwd)
		xnfo(0,15,"unlocking speed of #%d",active);
	else
		xnfo(0,15,"locking speed of #%d",active);
	cc[active]->m->UnlockSpeed(cc[active]->fastfwd);
}

bool ParseArgs(int argc, char* argv[])
{
	int i;
	int fsm = 0;
	float tmpf;
	for (i = 1; i < argc; i++) {
		switch (fsm) {
		case 0:
			if ((strlen(argv[i]) < 2) || (argv[i][0] != '-')) {
				xnfo(0,16,"Error parsing arguments. '%s' is not a valid switch. Abort.",argv[i]);
				return false;
			}
			switch (argv[i][1]) {
			case 'p': fsm = 1; break;	//post-process on/off
			case 'w': fsm = 2; break;	//width
			case 'h': fsm = 3; break;	//height
			case 'a': fsm = 4; break;	//auto-size on/off
			case 'g': fsm = 5; break;	//gamma value
			case 'r': fsm = 6; break;	//gRayscale mode
			case 'b': fsm = 7; break;	//Black/white threshold
			case 'f': fsm = 8; break;	//pixel Format
			default:
				xnfo(0,16,"Unknown switch %c. Skipping...",argv[i][1]);
				fsm = 999;
				break;
			}
			break;
		case 1:
			if (atoi(argv[i])) {
				xnfo(0,16,"Post-processing ON.");
				ppset.on = true;
			} else {
				xnfo(0,16,"Post-processing OFF.");
				ppset.on = false;
			}
			fsm = 0;
			break;
		case 2:
			ppset.w = atoi(argv[i]);
			if (ppset.w < 1) ppset.w = 1;
			if (ppset.w > 9999) ppset.w = 9999; //unreal
			xnfo(0,16,"Post-process frame width set to %d",ppset.w);
			fsm = 0;
			break;
		case 3:
			ppset.h = atoi(argv[i]);
			if (ppset.h < 1) ppset.h = 1;
			if (ppset.h > 9999) ppset.h = 9999; //unreal
			xnfo(0,16,"Post-process frame height set to %d",ppset.h);
			fsm = 0;
			break;
		case 4:
			if (atoi(argv[i])) {
				xnfo(0,16,"Auto window size ON.");
				sdl.autosize = true;
			} else {
				xnfo(0,16,"Auto window size OFF.");
				sdl.autosize = false;
			}
			fsm = 0;
			break;
		case 5:
			tmpf = atof(argv[i]);
			ppset.gamma[0] = tmpf;
			ppset.gamma[1] = tmpf;
			ppset.gamma[2] = tmpf;
			xnfo(0,16,"Post-process gamma correction value = %f",tmpf);
			fsm = 0;
			break;
		case 6:
			switch (argv[i][0]) {
			case 'G': ppset.typ = DOSCRD_VID_GS; xnfo(0,16,"Grayscale mode."); break;
			case 'B': ppset.typ = DOSCRD_VID_BW; xnfo(0,16,"Black and white mode."); break;
			default:
				xnfo(0,16,"Invalid grayscaling flag %c. Skipping...",argv[i][0]);
			}
			fsm = 0;
			break;
		case 7:
			ppset.threshold = (uint8_t)atoi(argv[i]);
			xnfo(0,16,"Black/white mode threshold set to %hhu",ppset.threshold);
			fsm = 0;
			break;
		case 8:
			switch (atoi(argv[i])) {
			case 32:
			case 24:
				ppset.fmt = DOSCRD_VIDFMT_DWORD;
				break;
			case 16:
				ppset.fmt = DOSCRD_VIDFMT_WORD;
				break;
			case 8:
				ppset.fmt = DOSCRD_VIDFMT_BYTE;
				break;
			default:
				xnfo(0,16,"Unsupported bit depth of %s. Aborted.",argv[i]);
				return false;
			}
			xnfo(0,16,"%s-bit color depth used.",argv[i]);
			fsm = 0;
			break;
		default:
			//just trash a parameter
			fsm = 0;
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	xnfo(0,1,"ALIVE!");
	active = 0;
	memset(&sdl,0,sizeof(sdl));
	memset(&ppset,0,sizeof(ppset));
	if (!ParseArgs(argc,argv)) _exit(EXIT_FAILURE);

	LibDosCardInit(1);
	xnfo(0,1,"Library initialized");

	if (!PushMachine()) _exit(EXIT_FAILURE);

	SDLInit();
	SDLoop();

	xnfo(0,1,"Finalizing...");
	ClearMachines();
	SDLKill();

	xnfo(0,1,"Calling library exit()...");
	LibDosCardExit();

	xnfo(0,1,"QUIT");
	_exit(EXIT_SUCCESS);
}
