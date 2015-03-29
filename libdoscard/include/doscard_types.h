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

/* WARNING: Do NOT include this file directly. Use doscard.h instead! */

#ifndef DOSCARD_TYPES_H_
#define DOSCARD_TYPES_H_

namespace doscard {

/* ****************** DosCard data types ****************** */

enum EDOSCRDState {
	DOSCRD_NOT_READY = 0,
	DOSCRD_LOADED,
	DOSCRD_INITED,
	DOSCRD_RUNNING,
	DOSCRD_SHUTDOWN,
	DOSCRD_LOADFAIL,
	DOSCRD_PAUSED
};

#ifdef DOSCARD_MT_SOURCE

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

typedef uint32_t LDBI_caps;

typedef struct SRuntimeData {
	bool on;
	uint16_t lcdw,lcdh;
	uint64_t sndsize;
	struct timespec* clkres;
	uint32_t* clkbeg;
	uint8_t disp_fsm;
	uint32_t frame_cnt;
	bool frame_dirty;
	uint8_t frameskip_cnt;
	uint32_t* framebuf;		//FIXME: Use Screen instead!
//	uint32_t crc;
	dosbox::LDB_SoundInfo sound_req;
	bool sound_fmt_ok;
	uint32_t sound_avail,sound_pos,sound_rec;
	uint32_t interleave;
	bool paused;
} LDBI_RuntimeData;

typedef struct SExtendedData {
	//TODO
} LDBI_ExtData;

enum EDOSCRDVidOut {
	DOSCRD_VID_COLOR = 0,
	DOSCRD_VID_GS, //grayscale
	DOSCRD_VID_BW //black-n-white
};

enum EDOSCRDVidFmt {
	DOSCRD_VIDFMT_DWORD = 0, //default ARGB8888
	DOSCRD_VIDFMT_WORD,		//16bit: xRGB1555
	DOSCRD_VIDFMT_BYTE,		//8bit: grayscale or RGB332
	DOSCRD_VIDFMT_BIT		//1bit black/white
};

typedef struct SPostProcess {
	bool on;
	EDOSCRDVidOut typ;
	EDOSCRDVidFmt fmt;
	int w,h;
	float gamma[3];
	uint8_t threshold;
} LDBI_PostProcess;

typedef std::vector<dosbox::LDB_UIEvent> LDBI_EventVec;
typedef std::vector<std::string> LDBI_MesgVec;
typedef std::vector<LDBI_ExtData> LDBI_EDFIFO;
typedef int16_t LDBI_SndSample;

} //namespace doscard

#endif /* DOSCARD_TYPES_H_ */
