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

#define DOSCARD_SOURCE
#include "doscard.h"

#ifndef BUILDATE
#define BUILDATE "unknown date"
#endif

#ifndef COMPILERNAME
#define COMPILERNAME "unknown compiler"
#endif

#ifndef BUILDNUMBER
#define BUILDNUMBER "0"
#endif

//Get function by Letter Macro
#define GFUNCL(x) phld->funcs->at(x - 'A')

using namespace std;
using namespace dosbox;
using namespace llvm;

namespace doscard {

static int verbose;

void LibDosCardInit(int verb)
{
	InitializeNativeTarget();
	llvm_start_multithreaded();
	verbose = verb;
}

void LibDosCardExit(void)
{
	llvm_shutdown();
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
	SMDiagnostic err;
	string fn;
	string errstr;
	char* ostr;
	state = DOSCRD_LOADFAIL;
	FreeModule();

	//Load file
	fn = (filename)? filename:DEFAULTLIBNAME;
	phld->module = ParseIRFile(fn,err,(*(phld->context)));
	if (!phld->module) return false;
	if (phld->module->MaterializeAllPermanently(&errstr)) {
		verb("TryLoad(): bitcode didn't read correctly (%s)\n",errstr.c_str());
		return false;
	}

	//Create Engine
	verb("TryLoad(): Bitcode file loaded. Trying to build engine...\n");
	phld->engbld = new EngineBuilder(phld->module);
	if (!phld->engbld) return false;
	phld->engbld->setErrorStr(&errstr);
	phld->engbld->setEngineKind(EngineKind::JIT);
	phld->engbld->setOptLevel(CodeGenOpt::Default);
	phld->engine = phld->engbld->create();
	if (!phld->engine) {
		verb("TryLoad(): couldn't create EE (%s)\n",errstr.c_str());
		return false;
	}

	verb("TryLoad(): Loading functions...\n");
	if (!LoadFunctions()) return false;

	//Check API
	GenericValue ret = phld->engine->runFunction(GFUNCL('A'),GenArgs());
	if (ret.IntVal != static_cast<uint64_t> (LDBWINTVER)) {
		verb("TryLoad(): Wrong API version\n");
		return false;
	}

	//Run static constructors
	verb("TryLoad(): Enabling statics...\n");
	phld->engine->runStaticConstructorsDestructors(false);

	//Get Version String
	ostr = reinterpret_cast<char*> (malloc(VERSTRMAXLEN/2));
	ret = phld->engine->runFunction(GFUNCL('L'),GenArgs(ostr,VERSTRMAXLEN/2));
	if (ret.IntVal != 0) {
		verb("TryLoad(): Failed to get version information\n");
		free(ostr);
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
	int i,n;
	Module::iterator I;
	DCFuncs::iterator j;
	string cnm;
	Function* Fn;
	phld->funcs = new DCFuncs;
//	for (i=0; i<LDBWRAP_FUNCS_Q; i++)
//		phld->funcs->push_back(NULL);
	phld->funcs->resize(LDBWRAP_FUNCS_Q,NULL);
	for (j = phld->funcs->begin(); j != phld->funcs->end(); ++j)
		verb(">$0x%x\n",*j);
	i = 0;
	for (I = phld->module->begin(); I != phld->module->end(); ++I) {
		cnm = I->getName().str();
		printf(">%s\n",cnm.c_str());
		if (cnm.length() < 5) continue;
		if ((cnm[0] == 'D') && (cnm[1] == 'C') && (cnm[3] == '_')) {
			Fn = &*I; //construction got from llvm's tools/lli
			n = static_cast<int> (cnm[2] - 'A');
			if ((n < 0) || (n >= LDBWRAP_FUNCS_Q)) continue;
			printf("->\t#%d\t0x%x\t(from %d)\n",n,Fn,LDBWRAP_FUNCS_Q);
//			j = phld->funcs->begin() + n;
//			phld->funcs->insert(j,Fn);
//			phld->funcs->erase(++j);
//			DCFuncs tmp = *phld->funcs;
//			tmp[n] = Fn;
			(*phld->funcs)[n] = Fn;
			if (++i >= LDBWRAP_FUNCS_Q) {
				for (j = phld->funcs->begin(); j != phld->funcs->end(); ++j)
					verb(">$0x%x\n",*j);
				verb("LoadFunctions(): Done!\n");
//				return true;
				return false;
			}
		}
	}
	verb("LoadFunctions(): FAILED!\n");
	return false;
}

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

EDOSCRDState CDosCard::GetCurrentState()
{
	return state;
}

LDB_Settings* CDosCard::GetSettings()
{
	if ((state == DOSCRD_INITED) || (state == DOSCRD_RUNNING)) {
		LDB_Settings set;
		GenericValue r = phld->engine->runFunction(GFUNCL('E'),GenArgs(&set,sizeof(set)));
		if (r.IntVal == 0)
			memcpy(settings,&set,sizeof(set));
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
	//TODO
	return false;
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

void CDosCard::DoNotCallRunner()
{
	verb("Runner() executed!\n");
	GenericValue r = phld->engine->runFunction(GFUNCL('D'),GenArgs());
	verb("Runner() finished natively!\n");
	state = DOSCRD_SHUTDOWN;
}

int CDosCard::SetCapabilities(LDBI_caps flags)
{
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

uint32_t CDosCard::FillSound(uint16_t* buf, uint32_t maxlen)
{
	//TODO
	return 0;
}

} //namespace doscard
