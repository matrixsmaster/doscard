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

#ifndef DOSCARD_H_
#define DOSCARD_H_

#define VERSIONSTR "v.0.0.2"
#define DEFAULTLIBNAME "libdbwrap.bc"

#ifdef DOSCARD_SOURCE
#include <llvm/IR/LLVMContext.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/CodeGen/LinkAllCodegenComponents.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/ExecutionEngine/JITMemoryManager.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/TypeBuilder.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/Support/Memory.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/PluginLoader.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Process.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Instrumentation.h>
#include "ldbwrap.h"
#endif

#include <vector>
#include <string>
#include "dosbox.h"

#define VERSTRMAXLEN 1535

namespace doscard {

enum EDOSCRDState {
	DOSCRD_NOT_READY = 0,
	DOSCRD_LOADED,
	DOSCRD_INITED,
	DOSCRD_RUNNING,
	DOSCRD_SHUTDOWN,
	DOSCRD_LOADFAIL
};

#ifdef DOSCARD_SOURCE

typedef std::vector<llvm::Function*> DCFuncs;

typedef struct {
	llvm::LLVMContext* context;
	llvm::Module* module;
	llvm::EngineBuilder* engbld;
	llvm::ExecutionEngine* engine;
	DCFuncs* funcs;
} DCPHolder;

typedef std::vector<llvm::GenericValue> DCArgs;

#else

//Fillers
typedef int DCPHolder;
typedef std::vector<std::string> DCArgs;

#endif


class CDosCard {
public:
	CDosCard(bool autoload);
	~CDosCard();
	bool TryLoad(const char* filename);
	EDOSCRDState GetCurrentState();
	dosbox::LDB_Settings* GetSettings();
	char* GetVersionStringSafe();
	bool ApplySettings(dosbox::LDB_Settings* pset);
	bool Prepare();
	int Run();

private:
	EDOSCRDState state;
	dosbox::LDB_Settings* settings;
	char* verstr;
	DCPHolder* phld;

	bool LoadFunctions();
	//(void)
	DCArgs GenArgs(void);
	//(LDB_Settings*)
	DCArgs GenArgs(dosbox::LDB_Settings* pset);
	//(void*,uint64_t)
	DCArgs GenArgs(void* ptr, uint64_t len);
};

void LibDosCardInit(int verb);

} //namespace doscard

#endif /* DOSCARD_H_ */
