/*
 *  Copyright (C) 2015  Dmitry Soloviov
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

#ifndef DOSCARD_COMMON_H_
#define DOSCARD_COMMON_H_


/* Common Inclusion */
#include <vector>
#include <string>
#include <stdarg.h>

#include "fake_inttypes.h"
#include "keyboard.h"
#include "ldbconf.h"

/* ****************** DosCard Generic Constants ****************** */

#define VERSTRMAXLEN 1536
#define VERSIONSTR "0.0.11"
#define VERINFOTEMPL "libDosCard ver. %s build %s\nCompiled with %s on %s\nwrapper: %s"

/* ****************** DosCard data types ****************** */

namespace dosbox {

/* ****************** Events Pipe Data Structures ****************** */

enum LDB_UIEventE {
	LDB_UIE_NONE = 0,
	LDB_UIE_QUIT = 1,
	LDB_UIE_KBD = 2,
	LDB_UIE_MOUSE = 3
};

typedef struct {
	float x,y;
} LDBFloat2D;

typedef struct {
	LDBFloat2D rel,abs;
	uint8_t button;
} LDB_MOUSEINF;

typedef struct {
	LDB_UIEventE t; 	//event type
	KBD_KEYS key;		//keyboard key
	bool pressed;		//is pressed?
	LDB_MOUSEINF m;		//mouse info
} LDB_UIEvent;

typedef struct {
	uint8_t width,channels;
	uint32_t freq,blocksize;
	bool sign,silent;
} LDB_SoundInfo;

} // namespace dosbox

namespace doscard {

enum EDOSCRDState {
	DOSCRD_NOT_READY = 0,
	DOSCRD_LOADED,
	DOSCRD_INITED,
	DOSCRD_RUNNING,
	DOSCRD_SHUTDOWN,
	DOSCRD_LOADFAIL,
	DOSCRD_PAUSED
};

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
} LDBI_RuntimeData;

typedef struct SExtendedData {
	//TODO
} LDBI_ExtData;

typedef std::vector<dosbox::LDB_UIEvent> LDBI_EventVec;
typedef std::vector<std::string> LDBI_MesgVec;
typedef std::vector<LDBI_ExtData> LDBI_EDFIFO;
typedef int16_t LDBI_SndSample;

//Fillers
typedef int DCPHolder;
typedef std::vector<std::string> DCArgs;

} // namespace doscard

#endif /* DOSCARD_COMMON_H_ */
