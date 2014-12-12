/*
 * globals.h
 *
 *  Copyright (C) 2002-2013  The DOSBox Team
 *  Copyright (C) 2013-2014  Dmitry Soloviov
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
 *
 *
 * The main reason of this file is to keep tracking
 * of ALL project-global variables at one place.
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "bios_disk.h"
#include "callback.h"
#include "cdrom.h"
#include "cpu.h"
#include "dos_inc.h"
#include "dosbox.h"
#include "fpu.h"
#include "inout.h"
#include "int10.h"
#include "mem.h"
#include "mixer.h"
#include "paging.h"
#include "pic.h"
#include "regs.h"
#include "render.h"
#include "serialport.h"
#include "shell.h"
#include "timer.h"
#include "vga.h"
#include "video.h"

//bios_disk.h
extern diskGeo DiskGeometryList[];
extern imageDisk *imageDiskList[2 + MAX_HDD_IMAGES];
extern imageDisk *diskSwap[20];
extern Bits swapPosition;
extern Bit16u imgDTASeg; /* Real memory location of temporary DTA pointer for fat image disk access */
extern RealPt imgDTAPtr; /* Real memory location of temporary DTA pointer for fat image disk access */
extern DOS_DTA *imgDTA;

//callback.h
extern CallBack_Handler CallBack_Handlers[];
extern Bit8u lastint; //poor naming
extern Bitu call_priv_io;

//cdrom.h
extern int CDROM_GetMountType(char* path/*, int force*/);

//cpu.h
/* CPU Cycle Timing */
extern Bit32s CPU_Cycles;
extern Bit32s CPU_CycleLeft;
extern Bit32s CPU_CycleMax;
extern Bit32s CPU_OldCycleMax;
extern Bit32s CPU_CyclePercUsed;
extern Bit32s CPU_CycleLimit;
extern Bit64s CPU_IODelayRemoved;
extern bool CPU_CycleAutoAdjust;
extern bool CPU_SkipCycleAutoAdjust;
extern Bitu CPU_AutoDetermineMode;
extern Bitu CPU_ArchitectureType;
extern Bitu CPU_PrefetchQueueSize;
extern CPU_Decoder * cpudecoder;
extern Bit16u parity_lookup[256];
extern CPUBlock cpu;

//dos_inc.h
/* internal Dos Tables */
extern DOS_File * Files[DOS_FILES];
extern DOS_Drive * Drives[DOS_DRIVES];
extern DOS_Device * Devices[DOS_DEVICES];
extern Bit8u dos_copybuf[0x10000];
extern DOS_InfoBlock dos_infoblock;
extern DOS_Block dos;

//dosbox.h
//FIXME: local dosbox class instance pointer
extern CDosBox* myldbi;

//fpu.h
extern FPU_rec fpu;

//inout.h
extern IO_WriteHandler * io_writehandlers[3][IO_MAX];
extern IO_ReadHandler * io_readhandlers[3][IO_MAX];

//int10.h
extern Bit8u int10_font_08[256 * 8];
extern Bit8u int10_font_14[256 * 14];
extern Bit8u int10_font_16[256 * 16];
extern VideoModeBlock ModeList_VGA[];
extern VideoModeBlock * CurMode;
extern Int10Data int10;

//mem.h
extern HostPt MemBase;

//mixer.h
extern Bit8u MixTemp[MIXER_BUFSIZE];

//paging.h
extern PagingBlock paging;

//pic.h
/* CPU Cycle Timing (dup)
extern Bit32s CPU_Cycles;
extern Bit32s CPU_CycleLeft;
extern Bit32s CPU_CycleMax;
*/
/* PIC */
extern Bitu PIC_IRQCheck;
extern Bitu PIC_Ticks;

//regs.h
extern Segments Segs;
extern CPU_Regs cpu_regs;

//render.h
extern Render_t render;
extern RenderLineHandler_t RENDER_DrawLine;

//serialport.h
extern CSerial* serialports[];

//shell.h
/* first_shell is used to add and delete stuff from the shell env
 * by "external" programs. (config) */
extern Program * first_shell;
extern Bitu call_shellstop;

//timer.h
extern LDB_CallbackFunc libdosbox_callbacks[];

//vga.h
extern VGA_Type vga;
extern SVGA_Driver svga;
extern Bit32u ExpandTable[256];
extern Bit32u FillTable[16];
extern Bit32u CGA_2_Table[16];
extern Bit32u CGA_4_Table[256];
extern Bit32u CGA_4_HiRes_Table[256];
extern Bit32u CGA_16_Table[256];
extern Bit32u TXT_Font_Table[16];
extern Bit32u TXT_FG_Table[16];
extern Bit32u TXT_BG_Table[16];
extern Bit32u Expand16Table[4][16];
extern Bit32u Expand16BigTable[0x10000];

//video.h
extern bool mouselocked; //true if mouse is confined to window

#endif /* GLOBALS_H_ */
