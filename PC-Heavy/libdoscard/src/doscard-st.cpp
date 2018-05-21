/*
 *  Copyright (C) 2014-2015  Dmitry Soloviov
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
#include "doscard-head.h"

//Check VM state is active
#define VMACTIVE ((state == DOSCRD_RUNNING) || (state == DOSCRD_PAUSED))

//TODO: make it work :)
//#define CHECK_INSTANCE(X) if (instance > 0) return (X)
//#define CHECK_INSTANCE	  if (instance > 0) return

namespace doscard {

static int verbose,instance;

void LibDosCardInit(int verb)
{
	verbose = verb;
	instance = 0;
}

void LibDosCardExit(void)
{
	instance = 0;
}

static void verb(const char* fmt,...)
{
	va_list l;
	if (!verbose) return;
	va_start(l,fmt);
	vfprintf(stdout,fmt,l);
	va_end(l);
}

void* DosCardThread(void* ptr)
{
	if (!ptr) return NULL;
	CDosCard* parent = reinterpret_cast<CDosCard*> (ptr);
	parent->DoNotCallRunner();
	return NULL;
}

CDosCard::CDosCard(bool autoload)
{
//	CHECK_INSTANCE;
	instance++;
	state = DOSCRD_NOT_READY;
	settings = NULL;
	framebuffer = NULL;
	procedframe = NULL;
	phld = NULL;
	verstr = reinterpret_cast<char*> (malloc(VERSTRMAXLEN));
	snprintf(verstr,VERSTRMAXLEN-1,VERINFOTEMPL,VERSIONSTR,BUILDNUMBER,COMPILERNAME,BUILDATE,"<none>");
	memset(&pproc,0,sizeof(pproc));
	if (autoload) TryLoad(NULL);
}

CDosCard::~CDosCard()
{
	LDB_UIEvent etmp;
	switch (state) {
	case DOSCRD_PAUSED:
		SetPause(false);
		//no break
	case DOSCRD_RUNNING:
		SetInterleave(0);
		SetPause(false);
		memset(&etmp,0,sizeof(etmp));
		etmp.t = LDB_UIE_QUIT;
		PutEvent(etmp);
		pthread_join(dosthread,NULL);
		//no break
	case DOSCRD_SHUTDOWN:
	case DOSCRD_INITED:
		DCC_TryDestroyInstance();
		break;
	default:
		//just to make compiler happy
		break;
	}
	FreeModule();
	if (verstr) free(verstr);
	if (framebuffer) free(framebuffer);
	if (procedframe) free(procedframe);
}

bool CDosCard::TryLoad(const char* filename)
{
	char* ostr;

	if ((state != DOSCRD_NOT_READY) && (state != DOSCRD_LOADFAIL))
		return false;

	state = DOSCRD_LOADFAIL;

	//Check API
	if (DCA_WrapperInit() != LDBWINTVER) {
		verb("TryLoad(): Incorrect internal API version. Please rebuild libdoscard!\n");
		return false;
	}

	//Get Version String
	ostr = reinterpret_cast<char*> (malloc(VERSTRMAXLEN/2));
	if (DCL_GetVersionString(ostr,VERSTRMAXLEN/2) != 0) {
		verb("TryLoad(): Failed to get version information\n");
		return false;
	}
	snprintf(verstr,VERSTRMAXLEN-1,VERINFOTEMPL,VERSIONSTR,BUILDNUMBER,COMPILERNAME,BUILDATE,ostr);
	free(ostr);
	verstr[VERSTRMAXLEN-1] = 0;

	//OK
	verb("TryLoad(): Loading OK.\n");
	state = DOSCRD_LOADED;
	return true;
}

void CDosCard::FreeModule()
{
}

bool CDosCard::LoadFunctions()
{
	return true;
}

uint32_t* CDosCard::DoPostProcess(int* w, int* h)
{
	if (convert_frame(framebuffer,procedframe,*w,*h,&pproc)) {
		*w = pproc.w;
		*h = pproc.h;
		return procedframe;
	} else
		return framebuffer;
}

EDOSCRDState CDosCard::GetCurrentState()
{
	if ((state == DOSCRD_INITED) || VMACTIVE) {
		LDBI_RuntimeData dat;
		if (DCG_GetInstanceRuntime(&dat,sizeof(dat)) == 0) {
			if (dat.paused) state = DOSCRD_PAUSED;
			else if (dat.on) state = DOSCRD_RUNNING;
		}
	}
	return state;
}

LDB_Settings* CDosCard::GetSettings()
{
	if ((state == DOSCRD_INITED) || VMACTIVE) {
		LDB_Settings set;
		if (DCE_GetInstanceSettings(&set) == 0) {
			if (!settings) {
				settings = reinterpret_cast<LDB_Settings*> (malloc(sizeof(LDB_Settings)));
				memset(settings,0,sizeof(LDB_Settings)); //just to make sure there's no random data
			}
			if (settings) memcpy(settings,&set,sizeof(set));
		}
	}
	return settings;
}

char* CDosCard::GetVersionStringSafe()
{
	return verstr;
}

bool CDosCard::ApplySettings(LDB_Settings* pset)
{
	if (!pset) return false;
	if ((state != DOSCRD_INITED) && (!VMACTIVE)) return false;
	if (DCF_SetInstanceSettings(pset) != 0) return false;
	return true;
}

void CDosCard::ApplyPostProcess(LDBI_PostProcess* pset)
{
	if (pset) {
		memcpy(&pproc,pset,sizeof(pproc));
		if (!pproc.on) return;
		uint32_t sz = get_buffer_size(pset);
		if (sz) {
			procedframe = reinterpret_cast<uint32_t*> (realloc(procedframe,sz));
			if (!procedframe) pproc.on = false;
		} else {
			if (procedframe) free(procedframe);
			procedframe = NULL;
			pproc.on = false;
		}
	}
	else pproc.on = false;
}

bool CDosCard::Prepare()
{
	if (state != DOSCRD_LOADED) return false;
	if (DCB_CreateInstance(NULL) != 0) {
		verb("Prepare(): Error while creating new instance\n");
		return false;
	}
	state = DOSCRD_INITED;
	return true;
}

int CDosCard::Run()
{
	if (state != DOSCRD_INITED) return -1;
	state = DOSCRD_RUNNING;
	pthread_create(&dosthread,NULL,DosCardThread,this);
	return 0;
}

void CDosCard::SetPause(bool paused)
{
	if (VMACTIVE) {
		if ((DCQ_SetInstancePause(NULL,(paused?1:0)) == 0) && (paused != (state == DOSCRD_PAUSED)))
			state = (state == DOSCRD_RUNNING)? DOSCRD_PAUSED:DOSCRD_RUNNING;
	}
}

void CDosCard::SetInterleave(uint32_t cycles)
{
	if (VMACTIVE)
		DCR_SetInstanceInterleave(NULL,static_cast<uint64_t>(cycles));
}

void CDosCard::UnlockSpeed(bool on)
{
	if (VMACTIVE)
		DCS_UnlockSpeed(NULL,(on)?1:0);
}

void CDosCard::DoNotCallRunner()
{
	verb("Runner() executed!\n");
	DCD_RunInstance();
	verb("Runner() finished natively!\n");
	state = DOSCRD_SHUTDOWN;
}

int CDosCard::SetCapabilities(LDBI_caps flags)
{
	if ((state != DOSCRD_INITED) && (!VMACTIVE)) return -1;
	uint64_t caps = static_cast<uint64_t> (flags);
	if (DCM_SetInstanceCaps(NULL,caps) != 0) return -1;
	return 0;
}

uint32_t* CDosCard::GetFramebuffer(int* w, int* h)
{
	LDBI_RuntimeData buf;
	if ((!VMACTIVE) || (!w) || (!h)) return NULL;
	if (DCG_GetInstanceRuntime(&buf,sizeof(buf)) != 0) {
		verb("GetFramebuffer(): Unable to collect runtime data\n");
		return NULL;
	}
	*w = buf.lcdw;
	*h = buf.lcdh;
	uint64_t sz = (*w) * (*h) * 4;
	if ((!framebuffer) || (framebufsz != sz))
		framebuffer = reinterpret_cast<uint32_t*> (realloc(framebuffer,sz));
	if (!framebuffer) {
		verb("GetFramebuffer: Unable to reallocate memory for new framebuffer size (%u)\n",sz);
		return NULL;
	}
	DCH_GetInstanceScreen(framebuffer,sz);
	framebufsz = sz;
	if (pproc.on) return (DoPostProcess(w,h));
	else return framebuffer;
}

void CDosCard::PutEvent(dosbox::LDB_UIEvent e)
{
	if (!VMACTIVE) return;
	DCJ_AddInstanceEvents(&e,sizeof(e));
}

void CDosCard::PutString(char* str)
{
	if (!VMACTIVE) return;
	uint64_t l = 0;
	if (str) l = static_cast<uint64_t> (strlen(str));
	DCP_AddInstanceString(str,l);
}

LDBI_MesgVec* CDosCard::GetMessages()
{
	if (!VMACTIVE) return NULL;
	DCK_GetInstanceMessages(&msgbuff,sizeof(void*));
	return &msgbuff;
}

 bool CDosCard::GetSoundFormat(dosbox::LDB_SoundInfo* format)
{
	LDBI_RuntimeData buf;
	if ((!format) || (!VMACTIVE)) return false;
	if (DCG_GetInstanceRuntime(&buf,sizeof(buf)) != 0) {
		verb("GetSoundFormat(): Unable to collect runtime data\n");
		return false;
	}
	memcpy(format,&buf.sound_req,sizeof(dosbox::LDB_SoundInfo));
	return buf.sound_fmt_ok;
}

uint32_t CDosCard::FillSound(LDBI_SndSample* buf, uint32_t samples)
{
	if ((!buf) || (samples < 2) || (state != DOSCRD_RUNNING)) return 0;
	int32_t r = DCI_GetInstanceSound(buf,samples*sizeof(LDBI_SndSample));
	uint32_t ret = static_cast<uint32_t> (r / sizeof(LDBI_SndSample));
	verb("FillSound(): ret=%u\n",ret);
	return ret;
}

} //namespace doscard
