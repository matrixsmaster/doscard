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

static void RENDER_CallBack( GFX_CallBackFunctions_t function );

void RENDER_SetPal(Bit8u entry,Bit8u red,Bit8u green,Bit8u blue)
{
	render.pal.rgb[entry].red=red;
	render.pal.rgb[entry].green=green;
	render.pal.rgb[entry].blue=blue;
	if (render.pal.first>entry) render.pal.first=entry;
	if (render.pal.last<entry) render.pal.last=entry;
}

static void RENDER_EmptyLineHandler(const void * src) { }

static void RENDER_StartLineHandler(const void * s)
{
	if (!s) return;
}

static void RENDER_FinishLineHandler(const void * s)
{

}

bool RENDER_StartUpdate(void)
{
	//TODO: return false if busy (can we be busy and want to draw simultaneously in
	//a single-threaded env??
	RENDER_DrawLine = RENDER_StartLineHandler;
	return true;
}

void RENDER_EndUpdate(bool abort)
{
	if (abort) {
		//Do NOT update anything
	} else {
		//normal update
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

}
