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

#include <string.h>
#include <math.h>
#include "dosbox.h"
#include "inout.h"
#include "vga.h"
#include "mem.h"
#include "pic.h"
#include "render.h"

namespace dosbox {

static Bit8u herc_pal = 0;
	
void Herc_Palette(void) {	
	switch (herc_pal) {
	case 0:	// White
		VGA_DAC_SetEntry(0x7,0x2a,0x2a,0x2a);
		VGA_DAC_SetEntry(0xf,0x3f,0x3f,0x3f);
		break;
	case 1:	// Amber
		VGA_DAC_SetEntry(0x7,0x34,0x20,0x00);
		VGA_DAC_SetEntry(0xf,0x3f,0x34,0x00);
		break;
	case 2:	// Green
		VGA_DAC_SetEntry(0x7,0x00,0x26,0x00);
		VGA_DAC_SetEntry(0xf,0x00,0x3f,0x00);
		break;
	}
}

Bitu read_herc_status(Bitu /*port*/,Bitu /*iolen*/) {
	// 3BAh (R):  Status Register
	// bit   0  Horizontal sync
	//       1  Light pen status (only some cards)
	//       3  Video signal
	//     4-6	000: Hercules
	//			001: Hercules Plus
	//			101: Hercules InColor
	//			111: Unknown clone
	//       7  Vertical sync inverted

	double timeInFrame = PIC_FullIndex()-vga.draw.delay.framestart;
	Bit8u retval=0x72; // Hercules ident; from a working card (Winbond W86855AF)
					// Another known working card has 0x76 ("KeysoGood", full-length)
	if (timeInFrame < vga.draw.delay.vrstart ||
		timeInFrame > vga.draw.delay.vrend) retval |= 0x80;

	double timeInLine=fmod(timeInFrame,vga.draw.delay.htotal);
	if (timeInLine >= vga.draw.delay.hrstart &&
		timeInLine <= vga.draw.delay.hrend) retval |= 0x1;

	// 688 Attack sub checks bit 3 - as a workaround have the bit enabled
	// if no sync active (corresponds to a completely white screen)
	if ((retval&0x81)==0x80) retval |= 0x8;
	return retval;
}


void VGA_SetupOther(void)
{
	memset( &vga.tandy, 0, sizeof( vga.tandy ));
	vga.attr.disabled = 0;
	vga.config.bytes_skip=0;
	//Initialize values common for most machines, can be overwritten
	vga.tandy.draw_base = vga.mem.linear;
	vga.tandy.mem_base = vga.mem.linear;
	vga.tandy.addr_mask = 8*1024 - 1;
	vga.tandy.line_mask = 3;
	vga.tandy.line_shift = 13;
}

} //namespace dosbox
