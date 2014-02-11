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

#include <vector>

namespace dosbox {

typedef void (*LDB_INIT_FUN)(void*);

typedef struct ALDB_CPU {
	enum {
		LDB_CPU_AUTO = 0,
		LDB_CPU_386,
		LDB_CPU_386S,
		LDB_CPU_486S,
		LDB_CPU_586S
	} family;
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

typedef struct ALDB_SETTINGS {
	LDB_CPUType cpu;
	uint32_t mem;
} LDB_Settings;

class CLDBConf {
public:
	CLDBConf(LDB_Settings* p);
	virtual ~CLDBConf();
	void AddInitializer(LDB_INIT_FUN f);
	void InitAll();
	LDB_Settings* GetSettings();
	LDB_CPUType* GetCPU();
	uint32_t GetMem();
private:
	std::vector<LDB_INIT_FUN> inits;
	LDB_Settings set;
};

} /* namespace dosbox */

#endif /* CLDBCONF_H_ */
