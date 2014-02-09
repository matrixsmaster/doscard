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

#ifndef GUS_H_
#define GUS_H_

namespace dosbox {

//Extra bits of precision over normal gus
#define WAVE_BITS 2
#define WAVE_FRACT (9+WAVE_BITS)
#define WAVE_FRACT_MASK ((1 << WAVE_FRACT)-1)
#define WAVE_MSWMASK ((1 << (16+WAVE_BITS))-1)
#define WAVE_LSWMASK (0xffffffff ^ WAVE_MSWMASK)

//Amount of precision the volume has
#define RAMP_FRACT (10)
#define RAMP_FRACT_MASK ((1 << RAMP_FRACT)-1)

#define GUS_BASE myGUS.portbase
#define GUS_RATE myGUS.rate
#define LOG_GUS 0

Bit8u adlib_commandreg;
static MixerChannel * gus_chan;
static Bit8u irqtable[8] = { 0, 2, 5, 3, 7, 11, 12, 15 };
static Bit8u dmatable[8] = { 0, 1, 3, 5, 6, 7, 0, 0 };
static Bit8u GUSRam[1024*1024]; // 1024K of GUS Ram
static Bit32s AutoAmp = 512;
static Bit16u vol16bit[4096];
static Bit32u pantable[16];

class GUSChannels;
static void CheckVoiceIrq(void);

struct GFGus {
	Bit8u gRegSelect;
	Bit16u gRegData;
	Bit32u gDramAddr;
	Bit16u gCurChannel;

	Bit8u DMAControl;
	Bit16u dmaAddr;
	Bit8u TimerControl;
	Bit8u SampControl;
	Bit8u mixControl;
	Bit8u ActiveChannels;
	Bit32u basefreq;

	struct GusTimer {
		Bit8u value;
		bool reached;
		bool raiseirq;
		bool masked;
		bool running;
		float delay;
	} timers[2];
	Bit32u rate;
	Bitu portbase;
	Bit8u dma1;
	Bit8u dma2;

	Bit8u irq1;
	Bit8u irq2;

	bool irqenabled;
	bool ChangeIRQDMA;
	// IRQ status register values
	Bit8u IRQStatus;
	Bit32u ActiveMask;
	Bit8u IRQChan;
	Bit32u RampIRQ;
	Bit32u WaveIRQ;
} myGUS;

Bitu DEBUG_EnableDebugger(void);

static void GUS_DMA_Callback(DmaChannel * chan,DMAEvent event);

} //namespace dosbox

#endif /* GUS_H_ */
