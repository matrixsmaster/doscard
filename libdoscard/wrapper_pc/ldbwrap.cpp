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

#ifndef BUILDNUMBER
#define BUILDNUMBER "0"
#endif

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
	return -1;
}

int TryDestroyInstance(void)
{
	return -1;
}

int RunInstance(void)
{
	return -1;
}

int GetInstanceSettings(dosbox::LDB_Settings* set)
{
	return -1;
}

int GetInstanceRuntime(void* ptr, uint64_t len)
{
	return -1;
}

int GetInstanceScreen(void* ptr, uint64_t len)
{
	return -1;
}

int GetInstanceSound(void* ptr, uint64_t len)
{
	return -1;
}

int AddInstanceEvents(void* ptr, uint64_t len)
{
	return -1;
}

int GetInstanceMessages(void* ptr, uint64_t len)
{
	return -1;
}

int GetVersionString(void* ptr, uint64_t len)
{
	if ((!ptr) || (!len)) return -1;
	char* tmp = reinterpret_cast<char*> (malloc(1024));
	char* out = reinterpret_cast<char*> (ptr);
	if (!tmp) return -10;
	snprintf(tmp,1023,"libdoscard ver.%s build %s [API %d]\nCompiled with %s on %s\n",
			LDBWVERSIONSTRING,BUILDNUMBER,LDBWINTVER,COMPILERNAME,BUILDATE);
	strncpy(out,tmp,len-1);
	out[len-1] = 0;
	return 0;
}

} //namespace doscard
