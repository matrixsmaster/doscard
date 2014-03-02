/*
 *  Copyright (C) 2014  Soloviov Dmitry
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

#include "ldbwrap.h"

#ifndef BUILDATE
#define BUILDATE "unknown date"
#endif

#ifndef COMPILERNAME
#define COMPILERNAME "unknown compiler"
#endif

#define FA_TEST if ((!ptr) || (!len)) return -1

using namespace dosbox;

namespace doscard {

CDosBox* DOS;
LDBI_RuntimeData* Runtime;
uint32_t* Screen;
int16_t* Sound;
LDBI_EventVec* Events;
LDBI_MesgVec* Messages;

int LDBWrapperInit(void)
{
	DOS = NULL;
	Runtime = NULL;
	Screen = NULL;
	Sound = NULL;
	Events = NULL;
	Messages = NULL;
	//return magic value, which is simply an internal version
	return LDBWINTVER;
}

int CreateInstance(LDB_Settings* set)
{
	if (DOS) return -1;
	DOS = new CDosBox();
	Runtime = new LDBI_RuntimeData;
	Events = new LDBI_EventVec;
	Messages = new LDBI_MesgVec;
	return 0;
}

int TryDestroyInstance(void)
{
	if ((!DOS) && (!Runtime)) return 0;
	if (Runtime->on) return -1;
	delete DOS;
	delete Runtime;
	if (Screen) free(Screen);
	if (Sound) free(Sound);
	if (Events) delete Events;
	if (Messages) delete Messages;
	return 0;
}

int RunInstance(void)
{
	if ((!DOS) || (!Runtime)) return -1;
	Runtime->on = true;
	//
	Runtime->on = false;
	return 0;
}

int GetInstanceSettings(LDB_Settings* set)
{
	if ((!DOS) || (!set)) return -1;
	LDB_Settings* tmp = DOS->GetConfig();
	memcpy(set,tmp,sizeof(LDB_Settings));
	return 0;
}

int SetInstanceSettings(LDB_Settings* set)
{
	if ((!DOS) || (!Runtime) || (Runtime->on) || (!set)) return -1;
	DOS->SetConfig(set);
	return 0;
}

int GetInstanceRuntime(void* ptr, uint64_t len)
{
	if ((!Runtime) || (!ptr) || (len != sizeof(LDBI_RuntimeData)))
		return -1;
	memcpy(ptr,Runtime,len);
	return 0;
}

int GetInstanceScreen(void* ptr, uint64_t len)
{
	FA_TEST;
	return -1;
}

int GetInstanceSound(void* ptr, uint64_t len)
{
	FA_TEST;
	return -1;
}

int AddInstanceEvents(void* ptr, uint64_t len)
{
	FA_TEST;
	return -1;
}

int GetInstanceMessages(void* ptr, uint64_t len)
{
	FA_TEST;
	return -1;
}

int GetVersionString(void* ptr, uint64_t len)
{
	FA_TEST;
	char* tmp = reinterpret_cast<char*> (malloc(1024));
	char* out = reinterpret_cast<char*> (ptr);
	if (!tmp) return -10;
	snprintf(tmp,1023,"libdoscard wrapper [API %d]\nCompiled with %s on %s\n",
			LDBWINTVER,COMPILERNAME,BUILDATE);
	strncpy(out,tmp,len-1);
	out[len-1] = 0;
	return 0;
}

} //namespace doscard
