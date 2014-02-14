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
#include "setup.h"
#include "control.h"
#include "cross.h"
#include "programs.h"
#include "support.h"
#include "int10.h"
#include "render.h"
#include "pci_bus.h"
#include "box_inits.h"
#include "ldb.h"
#include "ldbconf.h"
#include "xshell.h"

namespace dosbox {

CDosBox* myldbi;

extern uint32_t frameskip;

static void DOSBOX_RealInit(Section *)
{
//	Section_prop * section=static_cast<Section_prop *>(sec);
	/* Initialize some dosbox internals */
	myldbi->ticksRemain=0;
	myldbi->ticksLast=GetTicks();
	myldbi->ticksLocked = false;
//	MSG_Init(section);
	frameskip = 3;
	int10.vesa_nolfb = false;
	int10.vesa_oldvbe = false;
	//inits
	IO_Init(NULL);
	PAGING_Init(NULL);
	MEM_Init(NULL);
	CALLBACK_Init(NULL);
	PIC_Init(NULL);
	PROGRAMS_Init(NULL);
	TIMER_Init(NULL);
	CMOS_Init(NULL);
	RENDER_Init(NULL);
	CPU_Init(NULL);
#if C_FPU
	FPU_Init(NULL);
#endif
	DMA_Init(NULL);
	VGA_Init(NULL);
	KEYBOARD_Init(NULL);
#if defined(PCI_FUNCTIONALITY_ENABLED)
	PCI_Init(); //PCI bus
#endif
	MIXER_Init(NULL);
#if C_DEBUG
	DEBUG_Init();
#endif
	SBLASTER_Init(NULL);
	PCSPEAKER_Init(NULL);
}

static void DOSBOX_RealDelete()
{
	//TODO: move all destructor runners here
}

void CDosBox::RunMachine()
{
	while (!NormalLoop());
}

Bitu CDosBox::NormalLoop()
{
	Bits ret;
	while (1) {
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

CDosBox::CDosBox()
{
	Section_prop * secprop;
	Section_line * secline;
	Prop_int* Pint;
	Prop_hex* Phex;
	Prop_string* Pstring;
	Prop_bool* Pbool;
	Prop_multival_remain* Pmulti_remain;

	LDB_Settings myset;
	memset(&myset,0,sizeof(myset));

	//FIXME: destroy this chain
	com_line = new CommandLine(0,NULL);
	control = new Config(com_line);
	//FIXME: don't use globals! :)
	myldbi = this;

	// Some frequently used option sets
	const char *rates[] = {  "44100", "48000", "32000","22050", "16000", "11025", "8000", "49716", 0 };
	const char *oplrates[] = {   "44100", "49716", "48000", "32000","22050", "16000", "11025", "8000", 0 };
	const char *ios[] = { "220", "240", "260", "280", "2a0", "2c0", "2e0", "300", 0 };
	const char *irqssb[] = { "7", "5", "3", "9", "10", "11", "12", 0 };
	const char *dmassb[] = { "1", "5", "0", "3", "6", "7", 0 };

	secprop=control->AddSection_prop("dosbox",&DOSBOX_RealInit);

#if C_DEBUG	
	LOG_StartUp();
#endif

	myset.mem = 31;
	myset.cpu.core = ALDB_CPU::LDB_CPU_NORMAL;
	myset.cpu.family = CPU_ARCHTYPE_486NEWSLOW;
	myset.cpu.cycles_change = ALDB_CPU::LDB_CPU_CYCLE_AUTO;


//	secprop=control->AddSection_prop("mixer",&MIXER_Init);
//	Pbool = secprop->Add_bool("nosound",Property::Changeable::OnlyAtStart,false);
//	Pbool->Set_help("Enable silent mode, sound is still emulated though.");

	myset.snd.enabled = true;
	myset.snd.sample_freq = 44100;

//	Pint = secprop->Add_int("rate",Property::Changeable::OnlyAtStart,44100);
//	Pint->Set_values(rates);
//	Pint->Set_help("Mixer sample rate, setting any device's rate higher than this will probably lower their sound quality.");

//	const char *blocksizes[] = { "1024", "2048", "4096", "8192", "512", "256", 0};
//	Pint = secprop->Add_int("blocksize",Property::Changeable::OnlyAtStart,256);
//	Pint->Set_values(blocksizes);
//	Pint->Set_help("Mixer block size, larger blocks might help sound stuttering but sound will also be more lagged.");

	myset.snd.blocks = 256;
	myset.snd.prebuf = 20;

//	Pint = secprop->Add_int("prebuffer",Property::Changeable::OnlyAtStart,20);
//	Pint->SetMinMax(0,100);
//	Pint->Set_help("How many milliseconds of data to keep on top of the blocksize.");


//	secprop=control->AddSection_prop("sblaster",&SBLASTER_Init,true);//done
	
//	const char* sbtypes[] = { "sb1", "sb2", "sbpro1", "sbpro2", "sb16", "gb", "none", 0 };
//	Pstring = secprop->Add_string("sbtype",Property::Changeable::WhenIdle,"sb16");
//	Pstring->Set_values(sbtypes);
//	Pstring->Set_help("Type of Soundblaster to emulate. gb is Gameblaster.");

//	Phex = secprop->Add_hex("sbbase",Property::Changeable::WhenIdle,0x220);
//	Phex->Set_values(ios);
//	Phex->Set_help("The IO address of the soundblaster.");

	myset.snd.sb_base = 0x220;
	myset.snd.sb_irq = 7;

//	Pint = secprop->Add_int("irq",Property::Changeable::WhenIdle,7);
//	Pint->Set_values(irqssb);
//	Pint->Set_help("The IRQ number of the soundblaster.");

//	Pint = secprop->Add_int("dma",Property::Changeable::WhenIdle,1);
//	Pint->Set_values(dmassb);
//	Pint->Set_help("The DMA number of the soundblaster.");

	myset.snd.sb_dma = 1;
	myset.snd.sb_hdma = 5;

//	Pint = secprop->Add_int("hdma",Property::Changeable::WhenIdle,5);
//	Pint->Set_values(dmassb);
//	Pint->Set_help("The High DMA number of the soundblaster.");

//	Pbool = secprop->Add_bool("sbmixer",Property::Changeable::WhenIdle,true);
//	Pbool->Set_help("Allow the soundblaster mixer to modify the DOSBox mixer.");

	myset.snd.sb_mix = true;

//	Pint = secprop->Add_int("oplrate",Property::Changeable::WhenIdle,44100);
//	Pint->Set_values(oplrates);
//	Pint->Set_help("Sample rate of OPL music emulation. Use 49716 for highest quality (set the mixer rate accordingly).");

	myset.snd.sb_opl_freq = 44100;

//	secprop = control->AddSection_prop("speaker",&PCSPEAKER_Init,true);//done
//	Pbool = secprop->Add_bool("pcspeaker",Property::Changeable::WhenIdle,true);
//	Pbool->Set_help("Enable PC-Speaker emulation.");

	myset.snd.pcsp_en = true;
	myset.snd.pcsp_freq = 44100;

//	Pint = secprop->Add_int("pcrate",Property::Changeable::WhenIdle,44100);
//	Pint->Set_values(rates);
//	Pint->Set_help("Sample rate of the PC-Speaker sound generation.");

	secprop=control->AddSection_prop("joystick",&BIOS_Init,false);//done
	secprop->AddInitFunction(&INT10_Init);
	secprop->AddInitFunction(&MOUSE_Init); //Must be after int10 as it uses CurMode

	secprop=control->AddSection_prop("serial",&SERIAL_Init,true);
	const char* serials[] = { "dummy", "disabled", "modem", "nullmodem",
	                          "directserial",0 };
   
	Pmulti_remain = secprop->Add_multiremain("serial1",Property::Changeable::WhenIdle," ");
	Pstring = Pmulti_remain->GetSection()->Add_string("type",Property::Changeable::WhenIdle,"dummy");
	Pmulti_remain->SetValue("dummy");
	Pstring->Set_values(serials);
	Pstring = Pmulti_remain->GetSection()->Add_string("parameters",Property::Changeable::WhenIdle,"");

	Pmulti_remain = secprop->Add_multiremain("serial2",Property::Changeable::WhenIdle," ");
	Pstring = Pmulti_remain->GetSection()->Add_string("type",Property::Changeable::WhenIdle,"dummy");
	Pmulti_remain->SetValue("dummy");
	Pstring->Set_values(serials);
	Pstring = Pmulti_remain->GetSection()->Add_string("parameters",Property::Changeable::WhenIdle,"");
	Pmulti_remain->Set_help("see serial1");

	Pmulti_remain = secprop->Add_multiremain("serial3",Property::Changeable::WhenIdle," ");
	Pstring = Pmulti_remain->GetSection()->Add_string("type",Property::Changeable::WhenIdle,"disabled");
	Pmulti_remain->SetValue("disabled");
	Pstring->Set_values(serials);
	Pstring = Pmulti_remain->GetSection()->Add_string("parameters",Property::Changeable::WhenIdle,"");
	Pmulti_remain->Set_help("see serial1");

	Pmulti_remain = secprop->Add_multiremain("serial4",Property::Changeable::WhenIdle," ");
	Pstring = Pmulti_remain->GetSection()->Add_string("type",Property::Changeable::WhenIdle,"disabled");
	Pmulti_remain->SetValue("disabled");
	Pstring->Set_values(serials);
	Pstring = Pmulti_remain->GetSection()->Add_string("parameters",Property::Changeable::WhenIdle,"");
	Pmulti_remain->Set_help("see serial1");


	/* All the DOS Related stuff, which will eventually start up in the shell */
	secprop=control->AddSection_prop("dos",&DOS_Init,false);//done

	secprop->AddInitFunction(&XMS_Init,true);//done

	Pbool = secprop->Add_bool("xms",Property::Changeable::WhenIdle,true);
	myset.xms = true;

	Pbool->Set_help("Enable XMS support.");

	secprop->AddInitFunction(&EMS_Init,true);//done

	const char* ems_settings[] = { "true", "emsboard", "emm386", "false", 0};
	Pstring = secprop->Add_string("ems",Property::Changeable::WhenIdle,"true");
	Pstring->Set_values(ems_settings);
	Pstring->Set_help("Enable EMS support. The default (=true) provides the best\n"
		"compatibility but certain applications may run better with\n"
		"other choices, or require EMS support to be disabled (=false)\n"
		"to work at all.");

	Pbool = secprop->Add_bool("umb",Property::Changeable::WhenIdle,true);
	Pbool->Set_help("Enable UMB support.");

	secprop->AddInitFunction(&DOS_KeyboardLayout_Init,true);
	Pstring = secprop->Add_string("keyboardlayout",Property::Changeable::WhenIdle, "auto");
	Pstring->Set_help("Language code of the keyboard layout (or none).");

	// Mscdex
	secprop->AddInitFunction(&MSCDEX_Init);
	secprop->AddInitFunction(&DRIVES_Init);
	secprop->AddInitFunction(&CDROM_Image_Init);

	secline=control->AddSection_line("autoexec",&AUTOEXEC_Init);
	MSG_Add("AUTOEXEC_CONFIGFILE_HELP",
		"Lines in this section will be run at startup.\n"
		"You can put your MOUNT lines here.\n"
	);
	MSG_Add("CONFIG_SUGGESTED_VALUES", "Possible values");

	control->SetStartUp(&SHELL_Init);

	config = new CLDBConf(&myset);
}

CDosBox::~CDosBox()
{
	if (config) delete config;
	if (control) delete control;
	if (com_line) delete com_line;
}

int CDosBox::RegisterCallback(LDB_CallbackType t, LDB_CallbackFunc f)
{
	//TODO: checks
	ldb_callbacks[t] = f;
	return 0;
}

int CDosBox::Callback(LDB_CallbackType t, void* p, size_t l)
{
	//TODO: checks
	return ((*ldb_callbacks[t])(p,l));
}

void CDosBox::Execute()
{
	LOG_MSG("CDosBox::Execute(): Enter");
	if (!control) return;
	LOG_MSG("DOSCARD version %s bld %s",VERSION,BUILDNUMBER);
	LOG_MSG(COPYRIGHT_STRING_ORIGINAL);
	LOG_MSG(COPYRIGHT_STRING_NEW);
	LOG_MSG("Compiled with %s %s",COMPILERNAME,BUILDATE);
	LOG_MSG("CDosBox::Execute(): Initializing subsystems...");
	control->Init();
	LOG_MSG("CDosBox::Execute(): Startup...");
	/*that's really ugly way to control the quit state of VM
	 * but original Box was written in that way in mind, so...*/
	try {
		control->StartUp();
	}
	catch (int) {
		LOG_MSG("CDosBox::Execute(): killer exception catch");
	}
	LOG_MSG("CDosBox::Execute(): Exit");
	return;
}

} //namespace dosbox
