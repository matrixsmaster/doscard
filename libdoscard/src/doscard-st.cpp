/*
 *  Copyright (C) 2014-2015  Soloviov Dmitry
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

namespace doscard {

static int verbose;

void LibDosCardInit(int verb)
{
	verbose = verb;
}

void LibDosCardExit(void)
{
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
	state = DOSCRD_NOT_READY;
	settings = NULL;
	phld = new DCPHolder;
	phld->context = new LLVMContext();
	phld->module = NULL;
	phld->engbld = NULL;
	phld->engine = NULL;
	phld->funcs = NULL;
	verstr = reinterpret_cast<char*> (malloc(VERSTRMAXLEN));
	snprintf(verstr,VERSTRMAXLEN-1,VERINFOTEMPL,VERSIONSTR,BUILDNUMBER,COMPILERNAME,BUILDATE,"<none>");
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
		memset(&etmp,0,sizeof(etmp));
		etmp.t = LDB_UIE_QUIT;
		PutEvent(etmp);
		pthread_join(dosthread,NULL);
		//no break
	case DOSCRD_SHUTDOWN:
	case DOSCRD_INITED:
		phld->engine->runFunction(GFUNCL('C'),GenArgs());
		break;
	default:
		//just to make compiler happy
		break;
	}
	FreeModule();
	delete phld->context;
	delete phld;
	if (verstr) free(verstr);
	if (framebuffer) free(framebuffer);
}

bool CDosCard::TryLoad(const char* filename)
{
	if ((state != DOSCRD_NOT_READY) && (state != DOSCRD_LOADFAIL))
		return false;
	verb("TryLoad(): Loading OK.\n");
	state = DOSCRD_LOADED;
	return true;
}

void CDosCard::FreeModule()
{
	if (phld->funcs) delete phld->funcs;
	phld->funcs = NULL;
	if (phld->engbld) delete phld->engbld;
	phld->engbld = NULL;
	if (phld->engine) {
		//FIXME: this method won't do anything really!
		//I tested it carefully, but nothing happens after call to this function
		//with argument <true>. LLVM uses it's own code in atexit() instead
		//http://lists.cs.uiuc.edu/pipermail/llvmdev/2011-September/043106.html
		phld->engine->runStaticConstructorsDestructors(true);
//		delete phld->engine;
	}
	phld->engine = NULL;
	if (phld->module) {
		//Just to make sure we really destroy all code
		Module::iterator I;
		for (I = phld->module->begin(); I != phld->module->end(); ++I)
			I->Dematerialize();
		delete phld->module; //in lli they didn't delete module, but the engine instead
	}
	phld->module = NULL;
}

bool CDosCard::LoadFunctions()
{
	return true;
}

#if 0
DCArgs CDosCard::GenArgs(void)
{
	DCArgs ret;
	return ret;
}

DCArgs CDosCard::GenArgs(dosbox::LDB_Settings* pset)
{
	DCArgs ret;
	GenericValue p;
	p.PointerVal = pset;
	ret.push_back(p);
	return ret;
}

DCArgs CDosCard::GenArgs(void* ptr, uint64_t len)
{
	DCArgs ret;
	GenericValue x;
	x.PointerVal = ptr;
	ret.push_back(x);
	GenericValue y;
	y.IntVal = APInt(64,len,false);
	ret.push_back(y);
	return ret;
}
#endif

EDOSCRDState CDosCard::GetCurrentState()
{
	return state;
}

LDB_Settings* CDosCard::GetSettings()
{
	if ((state == DOSCRD_INITED) || (state == DOSCRD_RUNNING)) {
		LDB_Settings set;
		GenericValue r = phld->engine->runFunction(GFUNCL('E'),GenArgs(&set));
		if (r.IntVal == 0) {
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
	if ((state != DOSCRD_INITED) && (state != DOSCRD_RUNNING)) return false;
	GenericValue r = phld->engine->runFunction(GFUNCL('F'),GenArgs(pset));
	if (r.IntVal != 0) return false;
	return true;
}

bool CDosCard::Prepare()
{
	if (state != DOSCRD_LOADED) return false;
	GenericValue r = phld->engine->runFunction(GFUNCL('B'),GenArgs(NULL));
	if (r.IntVal != 0) {
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
	if ((state == DOSCRD_RUNNING) || (state == DOSCRD_PAUSED)) {
		GenericValue r = phld->engine->runFunction(GFUNCL('Q'),GenArgs(NULL,(paused?1:0)));
		if ((r.IntVal == 0) && (paused != (state == DOSCRD_PAUSED)))
			state = (state == DOSCRD_RUNNING)? DOSCRD_PAUSED:DOSCRD_RUNNING;
	}
}

void CDosCard::DoNotCallRunner()
{
	verb("Runner() executed!\n");
	GenericValue r = phld->engine->runFunction(GFUNCL('D'),GenArgs());
	verb("Runner() finished natively!\n");
	state = DOSCRD_SHUTDOWN;
}

int CDosCard::SetCapabilities(LDBI_caps flags)
{
	if ((state != DOSCRD_INITED) && (state != DOSCRD_RUNNING)) return -1;
	uint64_t caps = static_cast<uint64_t> (flags);
	GenericValue r = phld->engine->runFunction(GFUNCL('M'),GenArgs(NULL,caps));
	if (r.IntVal != 0) return -1;
	return 0;
}

uint32_t* CDosCard::GetFramebuffer(int* w, int* h)
{
	LDBI_RuntimeData buf;
	if ((state != DOSCRD_RUNNING) || (!w) || (!h)) return NULL;
	GenericValue r = phld->engine->runFunction(GFUNCL('G'),GenArgs(&buf,sizeof(buf)));
	if (r.IntVal != 0) {
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
	r = phld->engine->runFunction(GFUNCL('H'),GenArgs(framebuffer,sz));
	framebufsz = sz;
	return framebuffer;
}

void CDosCard::PutEvent(dosbox::LDB_UIEvent e)
{
	if (state != DOSCRD_RUNNING) return;
	GenericValue r = phld->engine->runFunction(GFUNCL('J'),GenArgs(&e,sizeof(e)));
}

void CDosCard::PutString(char* str)
{
	if (state != DOSCRD_RUNNING) return;
	uint64_t l = 0;
	if (str) l = static_cast<uint64_t> (strlen(str));
	GenericValue r = phld->engine->runFunction(GFUNCL('P'),GenArgs(str,l));
}

LDBI_MesgVec* CDosCard::GetMessages()
{
	if (state != DOSCRD_RUNNING) return NULL;
	GenericValue r = phld->engine->runFunction(GFUNCL('K'),GenArgs(&msgbuff,sizeof(void*)));
	return &msgbuff;
}

 bool CDosCard::GetSoundFormat(dosbox::LDB_SoundInfo* format)
{
	LDBI_RuntimeData buf;
	if (!format) return false;
	if ((state != DOSCRD_RUNNING) && (state != DOSCRD_PAUSED)) return false;
	GenericValue r = phld->engine->runFunction(GFUNCL('G'),GenArgs(&buf,sizeof(buf)));
	if (r.IntVal != 0) {
		verb("GetSoundFormat(): Unable to collect runtime data\n");
		return false;
	}
	memcpy(format,&buf.sound_req,sizeof(dosbox::LDB_SoundInfo));
	return buf.sound_fmt_ok;
}

uint32_t CDosCard::FillSound(LDBI_SndSample* buf, uint32_t samples)
{
	if ((!buf) || (samples < 2) || (state != DOSCRD_RUNNING)) return 0;
	GenericValue r = phld->engine->runFunction(GFUNCL('I'),GenArgs(buf,samples*sizeof(LDBI_SndSample)));
	uint32_t ret = static_cast<uint32_t> (*r.IntVal.getRawData() / sizeof(LDBI_SndSample));
	verb("FillSound(): ret=%u\n",ret);
	return ret;
}

} //namespace doscard
