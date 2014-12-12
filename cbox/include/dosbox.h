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

#ifndef DOSBOX_EXTERNAL_INCLUDE
#include "version.h"
#include "logging.h"
#endif

#include "ldb.h"
#include "ldbconf.h"

#define DOSBOX_MESSAGE_LEN 512
#define DOSBOX_IDLE_LEN 100

//FIXME: delete that after all
#define IS_EGAVGA_ARCH 1
#define IS_VGA_ARCH 1

class CDosBox {
private:
	//FIXME: move more vars from public
	LDB_CallbackFunc ldb_callbacks[LDB_CALLBACKSQ];
	bool callbacksReady;
	CLDBConf* config;
	bool init_ok;
	uint64_t loopcount;
	bool pause_mode;

public:
	CDosBox();
	~CDosBox();
	int32_t RegisterCallback(LDB_CallbackType t, LDB_CallbackFunc f);
	int32_t UnregisterCallback(LDB_CallbackType t, bool unreg_all);
	int32_t Callback(LDB_CallbackType t, void* p, size_t l);
	Bitu NormalLoop();
	void RunMachine();
	void Init();
	void Clear();
	void Execute();
	void SetConfig(LDB_Settings* c);
	inline LDB_Settings* GetConfig() { return config->GetSettings(); }
	void SetQuit();
	void SetPause(bool paused);

	Bit32u ticksRemain,ticksLast,ticksAdded,ticksScheduled;
	Bit32s ticksDone;
	bool ticksLocked;
	bool quit;
};

GCC_ATTRIBUTE(noreturn) void E_Exit(const char * message,...) GCC_ATTRIBUTE( __format__(__printf__, 1, 2));

#endif /* DOSBOX_DOSBOX_H */
