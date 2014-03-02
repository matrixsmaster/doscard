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

#include "doscard.h"

using namespace std;
using namespace dosbox;
using namespace llvm;

namespace doscard {

//http://stackoverflow.com/questions/2437914/llvm-jit-segfaults-what-am-i-doing-wrong
//http://stackoverflow.com/questions/1838304/call-the-llvm-jit-from-c-program/1976378#1976378
//http://llvm.org/docs/ProgrammersManual.html#entering-and-exiting-multithreaded-mode
//http://llvm.org/docs/ProgrammersManual.html#jitthreading

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
	context = new LLVMContext();
	module = NULL;
	if (autoload) TryLoad(NULL);
}

CDosCard::~CDosCard()
{
	delete context;
}

bool CDosCard::TryLoad(const char* filename)
{
	if ((state != DOSCRD_NOT_READY) && (state != DOSCRD_LOADFAIL))
		return false;
	SMDiagnostic err;
	std::string fn;
	fn = (filename)? filename:DEFAULTLIBNAME;
	module = ParseIRFile(fn,err,(*context));
	state = (module)? DOSCRD_LOADED:DOSCRD_LOADFAIL;
	return (module != NULL);
}

bool CDosCard::ApplySettings(LDB_Settings* pset)
{
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
