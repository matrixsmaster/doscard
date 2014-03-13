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
#include <string.h>
#include <stdio.h> //vsprintf
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include "dosbox.h"
#include "mouse.h"
#include "vga.h"
#include "keyboard.h"
#include "logging.h"
#include "render.h"

namespace dosbox {

extern const char* RunningProgram;
extern bool CPU_CycleAutoAdjust;
extern Render_t render;
static bool mouse_wanted = false;

void GFX_SetTitle(Bit32s cycles,Bits frameskip,bool paused)
{
	char title[200]={0};
	static Bit32s internal_cycles=0;
	static Bits internal_frameskip=0;
	if(cycles != -1) internal_cycles = cycles;
	if(frameskip != -1) internal_frameskip = frameskip;
	if(CPU_CycleAutoAdjust) {
		sprintf(title,"DOSCARD %s, CPU speed: max %3d%% cycles, Frameskip %2d, Program: %8s",VERSION,internal_cycles,internal_frameskip,RunningProgram);
	} else {
		sprintf(title,"DOSCARD %s, CPU speed: %8d cycles, Frameskip %2d, Program: %8s",VERSION,internal_cycles,internal_frameskip,RunningProgram);
	}

	if(paused) strcat(title," PAUSED");
	GFX_ShowMsg("TITLE [%s]",title);
}

void GFX_Events()
{
	LDB_UIEvent evt;
	if (myldbi->Callback(DBCB_PullUIEvents,NULL,0) == LDB_CALLBACK_RET_NOT_FOUND)
		return;
	while (myldbi->Callback(DBCB_PullUIEvents,&evt,sizeof(LDB_UIEvent))) {
		switch (evt.t) {
		case LDB_UIE_KBD:
			KEYBOARD_AddKey(evt.key,evt.pressed);
			break;
		case LDB_UIE_MOUSE:
			if (!mouse_wanted) break;
			if (evt.m.button--) {
				if (evt.pressed) Mouse_ButtonPressed(evt.m.button);
				else Mouse_ButtonReleased(evt.m.button);
			} else {
				//FIXME: mouse absolute position calculation
				evt.m.abs.x /= static_cast<float>(render.src.width);
				evt.m.abs.y /= static_cast<float>(render.src.height);
				Mouse_CursorMoved(evt.m.rel.x,evt.m.rel.y,evt.m.abs.x,evt.m.abs.y,true);
			}
			break;
		case LDB_UIE_QUIT:
//			throw 1;
			myldbi->SetQuit();
			break;
		default:
			LOG_MSG("Unknown event (type %d) received",static_cast<int>(evt.t));
			break;
		}
	}
}

void GFX_ShowMsg(char const* format,...)
{
	char buf[DOSBOX_MESSAGE_LEN];
	va_list msg;
	va_start(msg,format);
	vsnprintf(buf,511,format,msg);
	va_end(msg);
	myldbi->Callback(DBCB_PushMessage,buf,sizeof(buf));
}

void Mouse_AutoLock(bool enable)
{
	LOG_MSG("Mouse_AutoLock(): mouse wanted = %d\n",enable);
	mouse_wanted = enable;
}

}
