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


#include <sys/types.h>
#include <assert.h>
#include <math.h>
#include "dosbox.h"
#include "video.h"
#include "render.h"
#include "setup.h"
#include "control.h"
#include "mapper.h"
#include "cross.h"
#include "hardware.h"
#include "support.h"

Render_t render;
RenderLineHandler_t RENDER_DrawLine;

static void RENDER_CallBack(GFX_CallBackFunctions_t function);

static inline void LDB_SendDWord(Bit32u x)
{
	(*libdosbox_callbacks[DBCB_PushScreen])(&x,4);
}

static inline void LDB_SendWord(Bit16u x)
{
	(*libdosbox_callbacks[DBCB_PushScreen])(&x,2);
}

static inline void LDB_SendByte(Bit8u x)
{
	(*libdosbox_callbacks[DBCB_PushScreen])(&x,1);
}

void RENDER_SetPal(Bit8u entry,Bit8u red,Bit8u green,Bit8u blue)
{
	render.pal.rgb[entry].red=red;
	render.pal.rgb[entry].green=green;
	render.pal.rgb[entry].blue=blue;
	if (render.pal.first>entry) render.pal.first=entry;
	if (render.pal.last<entry) render.pal.last=entry;
}

static void RENDER_StartLineHandler(const void * src)
{
	if (!src) return;
	Bit8u* px8 = ((Bit8u*)src);
	Bit32u x,s;
	for (x=0; x<render.src.width; x++) {
		switch (render.src.bpp) {
		case 8:
			s = px8[x];
			LDB_SendByte(render.pal.rgb[s].blue);
			LDB_SendByte(render.pal.rgb[s].green);
			LDB_SendByte(render.pal.rgb[s].red);
			break;
		case 15:
			s = ((Bit16u*)px8)[x];
			LDB_SendByte(((s & 0x001f) * 0x21) >> 2);
			LDB_SendByte(((s & 0x03e0) * 0x21) >> 7);
			LDB_SendByte(((s & 0x7c00) * 0x21) >> 12);
			break;
		case 16:
			s = ((Bit16u*)px8)[x];
			LDB_SendByte(((s & 0x001f) * 0x21) >> 2);
			LDB_SendByte(((s & 0x07e0) * 0x41) >> 9);
			LDB_SendByte(((s & 0xf800) * 0x21) >> 13);
			break;
		case 32:
			LDB_SendByte((((Bit32u*)src)[x]) & 255);
			LDB_SendByte((((Bit32u*)src)[x] >> 8) & 255);
			LDB_SendByte((((Bit32u*)src)[x] >> 16) & 255);
			break;
		}
	}
}

bool RENDER_StartUpdate(void)
{
	//TODO: return false if busy (can we be busy and want to draw simultaneously in
	//a single-threaded env??
	RENDER_DrawLine = RENDER_StartLineHandler;
	Bit32u hdr = ((Bit16u)render.src.width) << 16;
	hdr |= ((Bit16u)render.src.height);
	LDB_SendDWord(hdr);
	return true;
}

void RENDER_EndUpdate(bool abort)
{
	if (abort) {
		//Do NOT update anything
		LDB_SendByte(0);
	} else {
		//normal update
		LDB_SendByte(0xff);
	}
}

void RENDER_SetSize(Bitu width,Bitu height,Bitu bpp,float fps,double ratio,bool dblw,bool dblh)
{
	render.src.width=width;
	render.src.height=height;
	render.src.bpp=bpp;
	render.src.dblw=dblw;
	render.src.dblh=dblh;
	render.src.fps=fps;
	render.src.ratio=ratio;
}

void RENDER_Init(Section * sec)
{
	LDB_SendDWord(0xAABBCCDD);
}
