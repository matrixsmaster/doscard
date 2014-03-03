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

static void verb(const char* fmt,...)
{
	va_list l;
	if (!verbose) return;
	va_start(l,fmt);
	vfprintf(stdout,fmt,l);
	va_end(l);
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
	memset(verstr,0,VERSTRMAXLEN);
	if (autoload) TryLoad(NULL);
}

CDosCard::~CDosCard()
{
	if (verstr) free(verstr);
	if (phld->funcs) delete phld->funcs;
//	if (phld->engine) delete phld->engine;
	if (phld->module) delete phld->module;
	if (phld->engbld) delete phld->engbld;
	delete phld->context;
	delete phld;
}

bool CDosCard::TryLoad(const char* filename)
{
	if ((state != DOSCRD_NOT_READY) && (state != DOSCRD_LOADFAIL))
		return false;
	SMDiagnostic err;
	string fn;
	string errstr;
	state = DOSCRD_LOADFAIL;

	//Load file
	fn = (filename)? filename:DEFAULTLIBNAME;
	phld->module = ParseIRFile(fn,err,(*(phld->context)));
	if (!phld->module) return false;
	if (phld->module->MaterializeAllPermanently(&errstr)) {
		verb("TryLoad(): bitcode didn't read correctly (%s)\n",errstr.c_str());
		return false;
	}

	//Create Engine
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

	//Load Functions
	if (!LoadFunctions()) return false;
	for (DCFuncs::iterator I = phld->funcs->begin(); I != phld->funcs->end(); I++)
		printf("$%x\n",*I);
	phld->engine->runFunction(phld->funcs->at(LDBWRAP_FUNCS_Q-1),GenArgs(verstr,1024));
	printf("\n# %s\n",verstr);

	//Check API
//	if (phld->engine->runFunction())

	//OK
	state = DOSCRD_LOADED;
	return true;
}

bool CDosCard::LoadFunctions()
{
	int i,n;
	Module::iterator I;
	DCFuncs::iterator j;
	string cnm;
	Function* Fn;
	phld->funcs = new DCFuncs;
	for (i=0; i < LDBWRAP_FUNCS_Q; i++)
		phld->funcs->push_back(NULL);
	j = phld->funcs->begin();
	i = 0;
	for (I = phld->module->begin(); I != phld->module->end(); ++I) {
		cnm = I->getName().str();
		printf("-> %s\n",cnm.c_str());
		if ((cnm[0] == 'D') && (cnm[1] == 'C') && (cnm[3] == '_')) {
			Fn = &*I; //construction got from llvm's tools/lli
			n = static_cast<int> (cnm[2] - 'A');
			if ((n < 0) || (n >= LDBWRAP_FUNCS_Q)) continue;
			j = phld->funcs->begin() + n;
			phld->funcs->insert(j,Fn);
			phld->funcs->erase(++j);
			if (++i >= LDBWRAP_FUNCS_Q) return true;
		}
	}
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
	//TODO: get settings from machine
	return settings;
}

bool CDosCard::ApplySettings(LDB_Settings* pset)
{
	if (!pset) return false;
	return false;
}

bool CDosCard::Prepare()
{
	if (state != DOSCRD_LOADED) return false;
	//
	state = DOSCRD_INITED;
	return true;
}

int CDosCard::Run()
{
	if (state != DOSCRD_INITED) return -1;
	state = DOSCRD_RUNNING;
	//
	state = DOSCRD_SHUTDOWN;
	return 0;
}

} //namespace doscard
