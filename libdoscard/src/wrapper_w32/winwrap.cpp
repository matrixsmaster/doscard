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

#include "doscard.h"
#include "doscard_vs.h"

using namespace doscard;

static CDosCard* g_card = NULL;

void WinDosCardInit(int verb)
{
	LibDosCardInit(verb);
	g_card = new CDosCard(true);
	if (g_card) puts("WinDosCard: Ready");
	else puts("WinDosCard: Failed to initialize!");
}

void WinDosCardExit(void)
{
	if (g_card) delete g_card;
	LibDosCardExit();
	puts("WinDosCard: Bye!");
}

bool WinDosCardTryLoad(const char* filename)
{
	return true;
}

doscard::EDOSCRDState WinDosCardGetCurrentState()
{
	if (g_card) return (g_card->GetCurrentState());
	else return doscard::DOSCRD_NOT_READY;
}

dosbox::LDB_Settings* WinDosCardGetSettings()
{
	if (g_card) return (g_card->GetSettings());
	else return NULL;
}

char* WinDosCardGetVersionStringSafe()
{
	if (g_card) return (g_card->GetVersionStringSafe());
	else return NULL;
}

bool WinDosCardApplySettings(dosbox::LDB_Settings* pset)
{
	if (g_card) return (g_card->ApplySettings(pset));
	else return false;
}

bool WinDosCardPrepare()
{
	if (g_card) return (g_card->Prepare());
	else return false;
}

int WinDosCardRun()
{
	if (g_card) return (g_card->Run());
	else return 0;
}

void WinDosCardSetPause(bool paused)
{
	if (g_card) return (g_card->SetPause(paused));
}

int WinDosCardSetCapabilities(doscard::LDBI_caps flags)
{
	if (g_card) return (g_card->SetCapabilities(flags));
	else return 0;
}

uint32_t* WinDosCardGetFramebuffer(int* w, int* h)
{
	if (g_card) return (g_card->GetFramebuffer(w,h));
	else return NULL;
}

void WinDosCardPutEvent(dosbox::LDB_UIEvent e)
{
	if (g_card) return (g_card->PutEvent(e));
}

void WinDosCardPutString(char* str)
{
	if (g_card) return (g_card->PutString(str));
}

doscard::LDBI_MesgVec* WinDosCardGetMessages()
{
	if (g_card) return (g_card->GetMessages());
	else return NULL;
}

bool WinDosCardGetSoundFormat(dosbox::LDB_SoundInfo* format)
{
	if (g_card) return (g_card->GetSoundFormat(format));
	else return false;
}

uint32_t WinDosCardFillSound(doscard::LDBI_SndSample* buf, uint32_t samples)
{
	if (g_card) return (g_card->FillSound(buf,samples));
	else return 0;
}
