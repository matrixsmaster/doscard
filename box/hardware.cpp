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


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dosbox.h"
#include "hardware.h"
#include "setup.h"
#include "support.h"
#include "mem.h"
#include "mapper.h"
#include "pic.h"
#include "render.h"
#include "cross.h"

extern const char* RunningProgram;

class HARDWARE:public Module_base{
public:
	HARDWARE(Section* configuration):Module_base(configuration){
		Section_prop * section = static_cast<Section_prop *>(configuration);
		Prop_path* proppath= section->Get_path("captures");
		capturedir = proppath->realpath;
		CaptureState = 0;
		MAPPER_AddHandler(CAPTURE_WaveEvent,MK_f6,MMOD1,"recwave","Rec Wave");
		MAPPER_AddHandler(CAPTURE_MidiEvent,MK_f8,MMOD1|MMOD2,"caprawmidi","Cap MIDI");
#if (C_SSHOT)
		MAPPER_AddHandler(CAPTURE_ScreenShotEvent,MK_f5,MMOD1,"scrshot","Screenshot");
		MAPPER_AddHandler(CAPTURE_VideoEvent,MK_f5,MMOD1|MMOD2,"video","Video");
#endif
	}
	~HARDWARE(){
#if (C_SSHOT)
		if (capture.video.handle) CAPTURE_VideoEvent(true);
#endif
		if (capture.wave.handle) CAPTURE_WaveEvent(true);
		if (capture.midi.handle) CAPTURE_MidiEvent(true);
	}
};

static HARDWARE* test;

void HARDWARE_Destroy(Section * sec) {
	delete test;
}

void HARDWARE_Init(Section * sec) {
	test = new HARDWARE(sec);
	sec->AddDestroyFunction(&HARDWARE_Destroy,true);
}
