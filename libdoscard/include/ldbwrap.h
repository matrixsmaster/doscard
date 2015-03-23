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

#ifndef LDBWRAP_H_
#define LDBWRAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <string>

#include "llvm_sync.h"

#define DOSBOX_EXTERNAL_INCLUDE
#include "../../box/include/dosbox.h"

namespace doscard {

/* ****************** DosCard Generic Constants ****************** */
/// API Version.
#define LDBWINTVER 4

/// Letter index of last wrapper function.
#define LDBWRAP_LAST_FUNC_L 'R'

/// Number of wrapper functions accessible.
#define LDBWRAP_FUNCS_Q ((int)(LDBWRAP_LAST_FUNC_L-'A'+1))

/// Frameskip absolute maximum.
#define LDBW_FRAMESKIP_MAX 10

/// Input string buffer size
#define LDBW_STRINGBUF_SIZE 2048

/// Buffer holds at most this number of samples.
#define LDBW_SNDBUF_SAMPLES 4096

/* ****************** Capabilities Data Constants ****************** */
/// Video output.
#define DOSCRD_CAP_VIDEO 0x001

/// Audio Output.
#define DOSCRD_CAP_AUDIO 0x002

/// Events input.
#define DOSCRD_CAP_EVENT 0x004

/// Internal messages output.
#define DOSCRD_CAP_MESSG 0x008

/// File I/O and control.
/** Wrapper-embedded functionality. */
#define DOSCRD_CAP_FILIO 0x010

/// Directory I/O and control.
/** Wrapper-embedded functionality. */
#define DOSCRD_CAP_DIRIO 0x020

/// Serial (RS232) I/O and control.
/** Maps through extended IO. */
#define DOSCRD_CAP_SERIO 0x040

/// Parallel (LPT) I/O and control.
/** Maps through extended IO. */
#define DOSCRD_CAP_PARIO 0x080

/// STDOUT echo output.
/** Maps through message system IO. */
#define DOSCRD_CAP_EHOUT 0x100
/// STDOUT echo output prefix (marker).
#define DOSCRD_EHOUT_MARKER "{!}"

/// STDIN input.
#define DOSCRD_CAP_TTYIN 0x200
/// STDIN blocking input.
#define DOSCRD_TTYIN_BLK 0x400

/// Interleaved mode auto-restart
#define DOSCRD_ILV_AUTOR 0x800

/// Basic functionality macro.
#define DOSCRD_CAPS_BASIC	 (	DOSCRD_CAP_MESSG | \
								DOSCRD_CAP_FILIO | \
								DOSCRD_CAP_DIRIO )

/// Standard functionality macro.
#define DOSCRD_CAPS_STANDARD (	DOSCRD_CAPS_BASIC | \
								DOSCRD_CAP_VIDEO  | \
								DOSCRD_CAP_AUDIO  | \
								DOSCRD_CAP_EVENT  | \
								DOSCRD_ILV_AUTOR  )

/// Macro for headless mode.
#define DOSCRD_CAPS_HEADLESS (	DOSCRD_CAPS_BASIC | \
								DOSCRD_CAP_TTYIN  | \
								DOSCRD_TTYIN_BLK  | \
								DOSCRD_CAP_EHOUT  )

/// Everything is ON.
#define DOSCRD_CAPS_EVERYTHN 0x0fff

/// This macro is for internal use only.
#define DC_REG_CAP_MACRO(X,Y,Z) if (Caps & X) { \
									if (DOS->RegisterCallback(Y,Z)) \
										return -1; \
								} else { \
									DOS->UnregisterCallback(Y,false); \
								}

/* ****************** DosCard data types ****************** */

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

/* ****************** Callbacks prototypes ****************** */

int32_t LDBCB_LCD(void* buf, size_t len);
int32_t LDBCB_SND(void* buf, size_t len);
int32_t LDBCB_UIE(void* buf, size_t len);
int32_t LDBCB_TCK(void* buf, size_t len);
int32_t LDBCB_MSG(void* buf, size_t len);
int32_t LDBCB_FIO(void* buf, size_t len);
int32_t LDBCB_CIO(void* buf, size_t len);
int32_t LDBCB_LIO(void* buf, size_t len);
int32_t LDBCB_STO(void* buf, size_t len);
int32_t LDBCB_STI(void* buf, size_t len);
int32_t LDBCB_NOP(void* buf, size_t len);

/* ****************** DosCard Wrap Internal Globals ****************** */

extern dosbox::CDosBox* DOS;
extern LDBI_RuntimeData* Runtime;
extern uint32_t* Screen;
extern LDBI_SndSample* Sound;
extern LDBI_EventVec* Events;
extern LDBI_MesgVec* Messages;
extern char* StringInput;
extern LDBI_EDFIFO* ExtendedData;
extern LDBI_caps Caps;
extern LDBI_Mutex mutex;

/* ****************** DosCard Wrap Export Functions ****************** */

extern "C" {
int32_t DCA_WrapperInit(void);
int32_t DCB_CreateInstance(dosbox::LDB_Settings*);
int32_t DCC_TryDestroyInstance(void);
int32_t DCD_RunInstance(void);
int32_t DCE_GetInstanceSettings(dosbox::LDB_Settings*);
int32_t DCF_SetInstanceSettings(dosbox::LDB_Settings*);
int32_t DCG_GetInstanceRuntime(void*,uint64_t);
int32_t DCH_GetInstanceScreen(void*,uint64_t);
int32_t DCI_GetInstanceSound(void*,uint64_t);
int32_t DCJ_AddInstanceEvents(void*,uint64_t);
int32_t DCK_GetInstanceMessages(void*,uint64_t);
int32_t DCL_GetVersionString(void*,uint64_t);
int32_t DCM_SetInstanceCaps(void*,uint64_t);
int32_t DCN_GetInstanceExtData(void*,uint64_t);
int32_t DCO_AddInstanceExtData(void*,uint64_t);
int32_t DCP_AddInstanceString(void*,uint64_t);
int32_t DCQ_SetInstancePause(void*,uint64_t);
int32_t DCR_SetInstanceInterleave(void* ptr, uint64_t len);
}

} //namespace doscard

#endif /* LDBWRAP_H_ */
