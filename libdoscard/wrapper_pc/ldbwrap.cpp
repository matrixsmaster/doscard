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
char* StringInput;
LDBI_EDFIFO* ExtendedData;
LDBI_caps Caps;
volatile int mutex;

int DCA_WrapperInit(void)
{
	DOS = NULL;
	Runtime = NULL;
	Screen = NULL;
	Sound = NULL;
	Events = NULL;
	Messages = NULL;
	StringInput = NULL;
	ExtendedData = NULL;
	Caps = 0;
	mutex = 0;
	//return API version
	return LDBWINTVER;
}

int DCB_CreateInstance(dosbox::LDB_Settings* set)
{
	if (DOS) return -1;
	DOS = new CDosBox();
	Runtime = new LDBI_RuntimeData;
	Events = new LDBI_EventVec;
	Messages = new LDBI_MesgVec;
	StringInput = reinterpret_cast<char*> (malloc(LDBW_STRINGBUF_SIZE));
	memset(StringInput,0,LDBW_STRINGBUF_SIZE);
	ExtendedData = new LDBI_EDFIFO;
	Runtime->on = false;
	Runtime->lcdw = 0;
	Runtime->lcdh = 0;
	Runtime->sndsize = 0;
	Runtime->clkres = NULL;
	Runtime->clkbeg = NULL;
	Runtime->disp_fsm = 0;
	Runtime->frame_cnt = 0;
	Runtime->frame_dirty = true;
	Runtime->frameskip_cnt = 0;
	Runtime->framebuf = NULL;
	return DCM_SetInstanceCaps(NULL,DOSCRD_CAPS_STANDARD);
}

int DCC_TryDestroyInstance(void)
{
	if ((!DOS) && (!Runtime)) return 0;
	if (Runtime->on) return -1;
	if (Runtime->framebuf) free(Runtime->framebuf);
	delete DOS;
	delete Runtime;
	if (Screen) free(Screen);
	if (Sound) free(Sound);
	if (Events) delete Events;
	if (Messages) delete Messages;
	if (StringInput) free(StringInput);
	if (ExtendedData) delete ExtendedData;
	return 0;
}

int DCD_RunInstance(void)
{
	if ((!DOS) || (!Runtime)) return -1;
	Runtime->on = true;
	DOS->Execute();
	Runtime->on = false;
	return 0;
}

int DCE_GetInstanceSettings(dosbox::LDB_Settings* set)
{
	if ((!DOS) || (!set)) return -1;
	LDB_Settings* tmp = DOS->GetConfig();
	memcpy(set,tmp,sizeof(LDB_Settings));
	return 0;
}

int DCF_SetInstanceSettings(dosbox::LDB_Settings* set)
{
	if ((!DOS) || (!Runtime) || (Runtime->on) || (!set)) return -1;
	DOS->SetConfig(set);
	return 0;
}

int DCG_GetInstanceRuntime(void* ptr, uint64_t len)
{
	if ((!Runtime) || (!ptr) || (len != sizeof(LDBI_RuntimeData)))
		return -1;
	MUTEX_LOCK;
	memcpy(ptr,Runtime,len);
	MUTEX_UNLOCK;
	return 0;
}

int DCH_GetInstanceScreen(void* ptr, uint64_t len)
{
	FA_TEST;
	MUTEX_LOCK;
	memcpy(ptr,Runtime->framebuf,len);
	MUTEX_UNLOCK;
	return 0;
}

int DCI_GetInstanceSound(void* ptr, uint64_t len)
{
	FA_TEST;
	//TODO
	return -1;
}

int DCJ_AddInstanceEvents(void* ptr, uint64_t len)
{
	LDB_UIEvent* pe;
	FA_TEST;
	if (len != sizeof(LDB_UIEvent)) return -1;
	pe = reinterpret_cast<LDB_UIEvent*> (ptr);
	MUTEX_LOCK;
	Events->insert(Events->begin(),*pe);
	MUTEX_UNLOCK;
	return 0;
}

int DCK_GetInstanceMessages(void* ptr, uint64_t len)
{
	FA_TEST;
	LDBI_MesgVec* dst = reinterpret_cast<LDBI_MesgVec*> (ptr);
	MUTEX_LOCK;
	while (!Messages->empty()) {
		dst->push_back(*Messages->begin());
		Messages->erase(Messages->begin());
	}
	MUTEX_UNLOCK;
	return 0;
}

int DCL_GetVersionString(void* ptr, uint64_t len)
{
	FA_TEST;
	char* tmp = reinterpret_cast<char*> (malloc(1024));
	char* out = reinterpret_cast<char*> (ptr);
	if (!tmp) return -10;
	snprintf(tmp,1023,"libdoscard wrapper [API v%d]\nCompiled with %s on %s",
			LDBWINTVER,COMPILERNAME,BUILDATE);
	strncpy(out,tmp,len-1);
	out[len-1] = 0;
	return 0;
}

int DCM_SetInstanceCaps(void* ptr, uint64_t len)
{
	if ((len > DOSCRD_CAPS_EVERYTHN) || (ptr)) return -1;
	Caps = static_cast<LDBI_caps> (len);
	DOS->RegisterCallback(DBCB_GetTicks,LDBCB_TCK);
	DC_REG_CAP_MACRO(DOSCRD_CAP_VIDEO, DBCB_PushScreen,		LDBCB_LCD);
	DC_REG_CAP_MACRO(DOSCRD_CAP_AUDIO, DBCB_PushSound,		LDBCB_SND);
	DC_REG_CAP_MACRO(DOSCRD_CAP_EVENT, DBCB_PullUIEvents,	LDBCB_UIE);
	DC_REG_CAP_MACRO(DOSCRD_CAP_MESSG, DBCB_PushMessage,	LDBCB_MSG);
	DC_REG_CAP_MACRO(DOSCRD_CAP_FILIO, DBCB_FileIOReq,		LDBCB_FIO);
	DC_REG_CAP_MACRO(DOSCRD_CAP_SERIO, DBCB_COMIO,			LDBCB_CIO);
	DC_REG_CAP_MACRO(DOSCRD_CAP_PARIO, DBCB_LPTIO,			LDBCB_LIO);
	DC_REG_CAP_MACRO(DOSCRD_CAP_EHOUT, DBCB_LogSTDOUT,		LDBCB_STO);
	DC_REG_CAP_MACRO(DOSCRD_CAP_TTYIN, DBCB_PullTTYInput,	LDBCB_STI);
	return 0;
}

int DCN_GetInstanceExtData(void* ptr, uint64_t len)
{
	FA_TEST;
	//TODO
	return -1;
}

int DCO_AddInstanceExtData(void* ptr, uint64_t len)
{
	FA_TEST;
	//TODO
	return -1;
}

int DCP_AddInstanceString(void* ptr, uint64_t len)
{
	FA_TEST;
	MUTEX_LOCK;
	char* inp = reinterpret_cast<char*> (ptr);
	uint32_t l = strlen(StringInput);
	if ((len+l) >= LDBW_STRINGBUF_SIZE)
		len = LDBW_STRINGBUF_SIZE - l - 1;
	strncpy(StringInput+l,inp,len);
	StringInput[len+l] = 0;
	MUTEX_UNLOCK;
	return 0;
}

} //namespace doscard
