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

#include "ldbconsts.h"
#include "doscard_types.h"

namespace doscard {

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
