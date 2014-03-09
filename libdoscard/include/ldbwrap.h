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

#define DOSBOX_EXTERNAL_INCLUDE
#include "../../box/include/dosbox.h"

namespace doscard {

#define LDBWINTVER 2
#define FRAMESKIP_MAX 10

typedef struct {
	bool on;
	uint16_t lcdw,lcdh;
	uint64_t sndsize;
	struct timespec* clkres;
	uint32_t* clkbeg;
	uint8_t disp_fsm;
	uint32_t frame_cnt;
	bool frame_dirty;
	uint8_t frameskip_cnt;
	uint32_t* framebuf;
	uint32_t crc;
} LDBI_RuntimeData;

typedef std::vector<dosbox::LDB_UIEvent> LDBI_EventVec;
typedef std::vector<std::string> LDBI_MesgVec;

int LDBCB_LCD(void* buf, size_t len);
int LDBCB_SND(void* buf, size_t len);
int LDBCB_UIE(void* buf, size_t len);
int LDBCB_TCK(void* buf, size_t len);
int LDBCB_MSG(void* buf, size_t len);
int LDBCB_FIO(void* buf, size_t len);

extern dosbox::CDosBox* DOS;
extern LDBI_RuntimeData* Runtime;
extern uint32_t* Screen;
extern int16_t* Sound;
extern LDBI_EventVec* Events;
extern LDBI_MesgVec* Messages;
extern volatile int mutex;

#define MUTEX_LOCK do {while (mutex) usleep(1); mutex = 1;} while(0)
#define MUTEX_UNLOCK mutex = 0

extern "C" {
int DCA_WrapperInit(void);
int DCB_CreateInstance(dosbox::LDB_Settings*);
int DCC_TryDestroyInstance(void);
int DCD_RunInstance(void);
int DCE_GetInstanceSettings(dosbox::LDB_Settings*);
int DCF_SetInstanceSettings(dosbox::LDB_Settings*);
int DCG_GetInstanceRuntime(void*,uint64_t);
int DCH_GetInstanceScreen(void*,uint64_t);
int DCI_GetInstanceSound(void*,uint64_t);
int DCJ_AddInstanceEvents(void*,uint64_t);
int DCK_GetInstanceMessages(void*,uint64_t);
int DCL_GetVersionString(void*,uint64_t);
}

#define LDBWRAP_FUNCS_Q 12

} //namespace doscard

#endif /* LDBWRAP_H_ */
