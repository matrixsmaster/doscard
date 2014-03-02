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

using namespace std;
using namespace dosbox;
using namespace llvm;

namespace doscard {

void LibDosCardInit()
{
	InitializeNativeTarget();
	llvm_start_multithreaded();
	fprintf(stderr,"INIT()\n");
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
	verstr = reinterpret_cast<char*> (malloc(1024));
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
		fprintf(stderr,"TryLoad(): bitcode didn't read correctly (%s)\n",errstr.c_str());
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
		fprintf(stderr,"TryLoad(): couldn't create EE (%s)\n",errstr.c_str());
		return false;
	}

	//Load Functions
	phld->funcs = new DCFuncs;
//	phld->engine->getPointerToFunction()
	for (Module::iterator I = phld->module->begin(), E = phld->module->end(); I != E; ++I) {
		printf("-> %s\n",I->getName().str().c_str());
//		Function *Fn = &*I;
	}
	phld->funcs->getVersionString = phld->module->getFunction("GetVersionString");
	if (!phld->funcs->getVersionString) {
		fprintf(stderr,"TryLoad(): GetVersionString not found!\n");
		return false;
	}

	//OK
	state = DOSCRD_LOADED;
	return true;
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
