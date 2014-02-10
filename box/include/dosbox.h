/*
 *  Copyright (C) 2002-2013  The DOSBox Team
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


#ifndef DOSBOX_DOSBOX_H
#define DOSBOX_DOSBOX_H

#include "config.h"
#include "version.h"
#include "logging.h"
#include "ldb.h"

namespace dosbox {

GCC_ATTRIBUTE(noreturn) void E_Exit(const char * message,...) GCC_ATTRIBUTE( __format__(__printf__, 1, 2));

void MSG_Add(const char*,const char*); //add messages to the internal languagefile
const char* MSG_Get(char const *);     //get messages from the internal languagefile

class CommandLine;
class Section;
class Config;

enum MachineType {
MCH_HERC,
MCH_CGA,
MCH_TANDY,
MCH_PCJR,
MCH_EGA,
MCH_VGA
};
enum SVGACards {
SVGA_None,
SVGA_S3Trio,
SVGA_TsengET4K,
SVGA_TsengET3K,
SVGA_ParadisePVGA1A
};
extern Config * control;
extern SVGACards svgaCard;
extern MachineType machine;
extern LDB_CallbackFunc libdosbox_callbacks[LDB_CALLBACKSQ];
void DOSBOX_RunMachine();

//FIXME: delete that after all
//#define IS_EGAVGA_ARCH 1
//#define IS_VGA_ARCH 1
#define IS_TANDY_ARCH ((machine==MCH_TANDY) || (machine==MCH_PCJR))
#define IS_EGAVGA_ARCH ((machine==MCH_EGA) || (machine==MCH_VGA))
#define IS_VGA_ARCH (machine==MCH_VGA)
#define TANDY_ARCH_CASE MCH_TANDY: case MCH_PCJR
#define EGAVGA_ARCH_CASE MCH_EGA: case MCH_VGA
#define VGA_ARCH_CASE MCH_VGA

//typedef Bitu (LoopHandler)(void);

class CDosBox {
private:
	//FIXME: move vars from public
	LDB_CallbackFunc ldb_callbacks[LDB_CALLBACKSQ];
	CommandLine* com_line;
	bool callbacksReady;
public:
	CDosBox();
	~CDosBox();
	int RegisterCallback(LDB_CallbackType t, LDB_CallbackFunc f);
	int Callback(LDB_CallbackType t, void* p, size_t l);
	Bitu NormalLoop();
	void RunMachine();
	void Execute();
//	void E_Exit(const char * message,...);
	//
	Config* control;
	Bit32u ticksRemain,ticksLast,ticksAdded,ticksScheduled;
	Bit32s ticksDone;
	bool ticksLocked;
};

//FIXME: local dosbox class instance pointer
extern CDosBox* myldbi;

} //namespace dosbox

#endif /* DOSBOX_DOSBOX_H */
