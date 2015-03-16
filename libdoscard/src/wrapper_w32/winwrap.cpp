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

CDosCard* g_card = NULL;

void WinDosCardInit(int verb)
{
	LibDosCardInit(verb);
	g_card = new CDosCard(true);
}

void WinDosCardExit(void)
{
	delete g_card;
	LibDosCardExit();
}

bool WinDosCardTryLoad(const char* filename)
{
	//
}

doscard::EDOSCRDState WinDosCardGetCurrentState()
{
	//
}

dosbox::LDB_Settings* WinDosCardGetSettings()
{
	//
}

char* WinDosCardGetVersionStringSafe()
{
	//
}

bool WinDosCardApplySettings(dosbox::LDB_Settings* pset)
{
	//
}

bool WinDosCardPrepare()
{
	//
}

int WinDosCardRun()
{
	//
}

void WinDosCardSetPause(bool paused)
{
	//
}

int WinDosCardSetCapabilities(doscard::LDBI_caps flags)
{
	//
}

uint32_t* WinDosCardGetFramebuffer(int* w, int* h)
{
	//
}

void WinDosCardPutEvent(dosbox::LDB_UIEvent e)
{
	//
}

void WinDosCardPutString(char* str)
{
	//
}

doscard::LDBI_MesgVec* WinDosCardGetMessages()
{
	//
}

bool WinDosCardGetSoundFormat(dosbox::LDB_SoundInfo* format)
{
	//
}

uint32_t WinDosCardFillSound(doscard::LDBI_SndSample* buf, uint32_t samples)
{
	//
}
