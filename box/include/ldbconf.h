/*
 *  Copyright (C) 2013-2014  Soloviov Dmitry
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

#ifndef CLDBCONF_H_
#define CLDBCONF_H_

//#include <vector>

namespace dosbox {

//typedef void (*LDB_INIT_FUN)(void*);

typedef struct ALDB_CPU {
	uint8_t family;
	enum {
		LDB_CPU_CYCLE_AUTO = 0,
		LDB_CPU_CYCLE_FIX,
		LDB_CPU_CYCLE_MAX
	} cycles_change;
	uint32_t fix_cycles;
	enum {
		LDB_CPU_NORMAL = 0,
		LDB_CPU_SIMPLE,
		LDB_CPU_FULL
	} core;
	int32_t cycle_perc, cycle_limit;
} LDB_CPUType;

typedef struct ALDB_MEM {
	enum {
		LDB_MEM_EMS_DISABLE = 0,
		LDB_MEM_EMS_AUTO = 1,
		LDB_MEM_EMS_EMSBRD = 2,
		LDB_MEM_EMS_EMM386 = 3
	} ems;
	uint32_t total_ram;
	bool xms, umb;
} LDB_MemorySet;

typedef struct ALDB_SND {
	bool enabled;
	uint32_t sample_freq, blocks;
	uint32_t prebuf; //FIXME: cut it
	//TODO: maybe place SB16/SBPro2 switch here?
	uint16_t sb_base, sb_irq, sb_dma, sb_hdma;
	uint32_t sb_opl_freq;
	bool sb_mix;
	bool pcsp_en;
	uint32_t pcsp_freq;
} LDB_Sound;

typedef struct ALDB_RS232 {
	bool enabled;
	bool dummy;
} LDB_RS232;

typedef struct ALDB_SETTINGS {
	LDB_CPUType cpu;
	LDB_MemorySet mem;
	LDB_Sound snd;
	LDB_RS232 tty[4];
	uint32_t frameskip;
	bool secure;
} LDB_Settings;

class CLDBConf {
public:
	CLDBConf(LDB_Settings* p);
	virtual ~CLDBConf();
	inline LDB_Settings* GetSettings() { return &set; }
//	inline LDB_CPUType* GetCPU() { return &set.cpu; }
//	uint32_t GetMem();
private:
	LDB_Settings set;
};

} /* namespace dosbox */

#endif /* CLDBCONF_H_ */
