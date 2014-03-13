/*
 *  Copyright (C) 2002-2013  The DOSBox Team
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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "dosbox.h"
#include "cpu.h"
#include "video.h"
#include "pic.h"
#include "cpu.h"
#include "callback.h"
#include "inout.h"
#include "mixer.h"
#include "timer.h"
#include "dos_inc.h"
#include "cross.h"
#include "programs.h"
#include "support.h"
#include "int10.h"
#include "render.h"
#include "pci_bus.h"
#include "shell.h"
#include "box_inits.h"
#include "ldb.h"
#include "ldbconf.h"

namespace dosbox {

CDosBox* myldbi;

void CDosBox::RunMachine()
{
	while ((!quit) && (!NormalLoop()));
}

Bitu CDosBox::NormalLoop()
{
	Bits ret;
	for(;;) {
		loopcount++;
		if (PIC_RunQueue()) {
			ret = (*cpudecoder)();
			if (GCC_UNLIKELY(ret<0)) return 1;
			if (ret>0) {
				if (GCC_UNLIKELY(ret >= CB_MAX)) return 0;
				Bitu blah = (*CallBack_Handlers[ret])();
				if (GCC_UNLIKELY(blah)) return blah;
			}
#if C_DEBUG
			if (DEBUG_ExitLoop()) return 0;
#endif
		} else {
			GFX_Events();
			if (ticksRemain>0) {
				TIMER_AddTick();
				ticksRemain--;
			} else break;
		}
	}
	if (GCC_UNLIKELY(ticksLocked)) {
		ticksRemain=5;
		/* Reset any auto cycle guessing for this frame */
		ticksLast = GetTicks();
		ticksAdded = 0;
		ticksDone = 0;
		ticksScheduled = 0;
	} else {
		Bit32u ticksNew;
		ticksNew=GetTicks();
		ticksScheduled += ticksAdded;
		if (ticksNew > ticksLast) {
			ticksRemain = ticksNew-ticksLast;
			ticksLast = ticksNew;
			ticksDone += ticksRemain;
			if ( ticksRemain > 20 ) {
				ticksRemain = 20;
			}
			ticksAdded = ticksRemain;
			if (CPU_CycleAutoAdjust && !CPU_SkipCycleAutoAdjust) {
				if (ticksScheduled >= 250 || ticksDone >= 250 || (ticksAdded > 15 && ticksScheduled >= 5) ) {
					if(ticksDone < 1) ticksDone = 1; // Protect against div by zero
					/* ratio we are aiming for is around 90% usage*/
					Bit32s ratio = (ticksScheduled * (CPU_CyclePercUsed*90*1024/100/100)) / ticksDone;
					Bit32s new_cmax = CPU_CycleMax;
					Bit64s cproc = (Bit64s)CPU_CycleMax * (Bit64s)ticksScheduled;
					if (cproc > 0) {
						/* ignore the cycles added due to the IO delay code in order
						   to have smoother auto cycle adjustments */
						double ratioremoved = (double) CPU_IODelayRemoved / (double) cproc;
						if (ratioremoved < 1.0) {
							ratio = (Bit32s)((double)ratio * (1 - ratioremoved));
							/* Don't allow very high ratio which can cause us to lock as we don't scale down
							 * for very low ratios. High ratio might result because of timing resolution */
							if (ticksScheduled >= 250 && ticksDone < 10 && ratio > 20480) 
								ratio = 20480;
							Bit64s cmax_scaled = (Bit64s)CPU_CycleMax * (Bit64s)ratio;
							/* The auto cycle code seems reliable enough to disable the fast cut back code.
							 * This should improve the fluency of complex games.
							if (ratio <= 1024) 
								new_cmax = (Bit32s)(cmax_scaled / (Bit64s)1024);
							else 
							 */
							new_cmax = (Bit32s)(1 + (CPU_CycleMax >> 1) + cmax_scaled / (Bit64s)2048);
						}
					}

					if (new_cmax<CPU_CYCLES_LOWER_LIMIT)
						new_cmax=CPU_CYCLES_LOWER_LIMIT;

					/* ratios below 1% are considered to be dropouts due to
					   temporary load imbalance, the cycles adjusting is skipped */
					if (ratio>10) {
						/* ratios below 12% along with a large time since the last update
						   has taken place are most likely caused by heavy load through a
						   different application, the cycles adjusting is skipped as well */
						if ((ratio>120) || (ticksDone<700)) {
							CPU_CycleMax = new_cmax;
							if (CPU_CycleLimit > 0) {
								if (CPU_CycleMax>CPU_CycleLimit) CPU_CycleMax = CPU_CycleLimit;
							}
						}
					}
					CPU_IODelayRemoved = 0;
					ticksDone = 0;
					ticksScheduled = 0;
				} else if (ticksAdded > 15) {
					/* ticksAdded > 15 but ticksScheduled < 5, lower the cycles
					   but do not reset the scheduled/done ticks to take them into
					   account during the next auto cycle adjustment */
					CPU_CycleMax /= 3;
					if (CPU_CycleMax < CPU_CYCLES_LOWER_LIMIT)
						CPU_CycleMax = CPU_CYCLES_LOWER_LIMIT;
				}
			}
		} else {
			ticksAdded = 0;
			TIMER_Delay(1);
			ticksDone -= GetTicks() - ticksNew;
			if (ticksDone < 0)
				ticksDone = 0;
		}
	}
	return 0;
}
/*
static void DOSBOX_UnlockSpeed( bool pressed ) {
	static bool autoadjust = false;
	if (pressed) {
		LOG_MSG("Fast Forward ON");
		ticksLocked = true;
		if (CPU_CycleAutoAdjust) {
			autoadjust = true;
			CPU_CycleAutoAdjust = false;
			CPU_CycleMax /= 3;
			if (CPU_CycleMax<1000) CPU_CycleMax=1000;
		}
	} else {
		LOG_MSG("Fast Forward OFF");
		ticksLocked = false;
		if (autoadjust) {
			autoadjust = false;
			CPU_CycleAutoAdjust = true;
		}
	}
}
*/

void CDosBox::Init()
{
	if (init_ok) return;
	myldbi->ticksRemain=0;
	myldbi->ticksLast=GetTicks();
	myldbi->ticksLocked = false;
	int10.vesa_nolfb = false;
	int10.vesa_oldvbe = false;
	IO_Init();
	PAGING_Init();
	MEM_Init();
	CALLBACK_Init();
	PIC_Init();
	PROGRAMS_Init();
	TIMER_Init();
	CMOS_Init();
	RENDER_Init();
	CPU_Init();
#if C_FPU
	FPU_Init();
#endif
	DMA_Init();
	VGA_Init();
	KEYBOARD_Init();
#if defined(PCI_FUNCTIONALITY_ENABLED)
	PCI_Init(); //PCI bus
#endif
	MIXER_Init();
#if C_DEBUG
	DEBUG_Init();
#endif
	SBLASTER_Init();
	PCSPEAKER_Init();
	BIOS_Init();
	INT10_Init();
	MOUSE_Init();
	SERIAL_Init();
	DOS_Init();
	XMS_Init();
	EMS_Init();
	DOS_KeyboardLayout_Init();
	MSCDEX_Init();
	DRIVES_Init();
	CDROM_Image_Init();
	AUTOEXEC_Init();
	init_ok = true;
}

void CDosBox::Clear()
{
	if (!init_ok) return;
	LOG_MSG("Cleaning subsystems...");
	SBLASTER_Clear();
	PCSPEAKER_Clear();
	XMS_Clear();
	EMS_Clear();
	DOS_Clear();
	DOS_KeyboardLayout_Clear();
	MSCDEX_Clear();
	BIOS_Clear();
	SERIAL_Clear();
	IO_Clear();
	PAGING_Clear();
	MEM_Clear();
	PIC_Clear();
	TIMER_Clear();
	CMOS_Clear();
	CPU_Clear();
	DMA_Clear();
	MIXER_Clear();
	VGA_Memory_Clear();
	init_ok = false;
}

CDosBox::CDosBox()
{
	LDB_Settings myset;
	memset(&myset,0,sizeof(myset));
	init_ok = false;
	myldbi = this;
	quit = false;

#if C_DEBUG	
	LOG_StartUp();
#endif

	myset.mem.total_ram = 31;
	myset.cpu.core = ALDB_CPU::LDB_CPU_NORMAL;
	myset.cpu.family = CPU_ARCHTYPE_486NEWSLOW;
	myset.cpu.cycles_change = ALDB_CPU::LDB_CPU_CYCLE_AUTO;

	myset.snd.enabled = true;
	myset.snd.sample_freq = 44100;

	myset.snd.blocks = 256;
	myset.snd.prebuf = 20;

	myset.snd.sb_base = 0x220;
	myset.snd.sb_irq = 7;

	myset.snd.sb_dma = 1;
	myset.snd.sb_hdma = 5;

	myset.snd.sb_mix = true;

	myset.snd.sb_opl_freq = 44100;

	myset.snd.pcsp_en = true;
	myset.snd.pcsp_freq = 44100;

	myset.tty[0].enabled = true;
	myset.tty[0].dummy = true;
	myset.tty[1] = myset.tty[0];
	myset.tty[2].enabled = false;
	myset.tty[3].enabled = false;

	myset.mem.xms = true;
	myset.mem.ems = ALDB_MEM::LDB_MEM_EMS_AUTO;
	myset.mem.umb = true;

	myset.frameskip = 0;
	myset.secure = false;

	config = new CLDBConf(&myset);
	UnregisterCallback(DBCB_GetTicks,true);
}

CDosBox::~CDosBox()
{
	if (config) delete config;
	if (init_ok) Clear();
}

int32_t CDosBox::RegisterCallback(LDB_CallbackType t, LDB_CallbackFunc f)
{
	if ((t < 0) || (t >= LDB_CALLBACKSQ)) return -1;
	ldb_callbacks[t] = f;
	return 0;
}

int32_t CDosBox::UnregisterCallback(LDB_CallbackType t, bool unreg_all)
{
	int i;
	if ((t < 0) || (t >= LDB_CALLBACKSQ)) return -1;
	if (unreg_all) {
		for (i=0; i<LDB_CALLBACKSQ; i++)
			ldb_callbacks[i] = NULL;
	} else
		ldb_callbacks[t] = NULL;
	return 0;
}

int32_t CDosBox::Callback(LDB_CallbackType t, void* p, size_t l)
{
	if (GCC_UNLIKELY(!ldb_callbacks[t]))
		return LDB_CALLBACK_RET_NOT_FOUND;
	return ((*ldb_callbacks[t])(p,l));
}

void CDosBox::SetConfig(LDB_Settings* c)
{
	if ((!c) || init_ok) return;
	if (config) delete config;
	config = new CLDBConf(c);
}

void CDosBox::SetQuit()
{
	LOG_MSG("CDosBox::SetQuit(): Called");
	quit = true;
	if (first_shell) {
		DOS_Shell* ptr = reinterpret_cast<DOS_Shell*> (first_shell);
		ptr->exit = true;
	}
}

void CDosBox::Execute()
{
	LOG_MSG("CDosBox::Execute(): Enter");
	if (!config) return;
	LOG_MSG("DOSCARD version %s build %s",VERSION,BUILDNUMBER);
	LOG_MSG(COPYRIGHT_STRING_ORIGINAL);
	LOG_MSG(COPYRIGHT_STRING_NEW);
	LOG_MSG("Compiled with %s %s",COMPILERNAME,BUILDATE);
	LOG_MSG("CDosBox::Execute(): Initializing subsystems...");
	Init();
	loopcount = 0;
	LOG_MSG("CDosBox::Execute(): Run!");
	SHELL_Init();
	LOG_MSG("CDosBox::Execute(): loopcount = %lu",loopcount);
	Clear();
	LOG_MSG("CDosBox::Execute(): Exit");
}

} //namespace dosbox
