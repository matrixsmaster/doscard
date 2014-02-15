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


#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include "adlib.h"
#include "mem.h"
#include "dbopl.h"

namespace dosbox {

#define RAW_SIZE 1024


/*
	Main Adlib implementation

*/

namespace Adlib {


/*
Chip
*/

bool Chip::Write( Bit32u reg, Bit8u val ) {
	switch ( reg ) {
	case 0x02:
		timer[0].counter = val;
		return true;
	case 0x03:
		timer[1].counter = val;
		return true;
	case 0x04:
		double time;
		time = PIC_FullIndex();
		if ( val & 0x80 ) {
			timer[0].Reset( time );
			timer[1].Reset( time );
		} else {
			timer[0].Update( time );
			timer[1].Update( time );
			if ( val & 0x1 ) {
				timer[0].Start( time, 80 );
			} else {
				timer[0].Stop( );
			}
			timer[0].masked = (val & 0x40) > 0;
			if ( timer[0].masked )
				timer[0].overflow = false;
			if ( val & 0x2 ) {
				timer[1].Start( time, 320 );
			} else {
				timer[1].Stop( );
			}
			timer[1].masked = (val & 0x20) > 0;
			if ( timer[1].masked )
				timer[1].overflow = false;

		}
		return true;
	}
	return false;
}


Bit8u Chip::Read( ) {
	double time( PIC_FullIndex() );
	timer[0].Update( time );
	timer[1].Update( time );
	Bit8u ret = 0;
	//Overflow won't be set if a channel is masked
	if ( timer[0].overflow ) {
		ret |= 0x40;
		ret |= 0x80;
	}
	if ( timer[1].overflow ) {
		ret |= 0x20;
		ret |= 0x80;
	}
	return ret;

}

void Module::CacheWrite( Bit32u reg, Bit8u val ) {
	//capturing?
	/*
	if ( capture ) {
		capture->DoWrite( reg, val );
	}*/
	//Store it into the cache
	cache[ reg ] = val;
}

void Module::DualWrite( Bit8u index, Bit8u reg, Bit8u val ) {
	//Make sure you don't use opl3 features
	//Don't allow write to disable opl3		
	if ( reg == 5 ) {
		return;
	}
	//Only allow 4 waveforms
	if ( reg >= 0xE0 ) {
		val &= 3;
	} 
	//Write to the timer?
	if ( chip[index].Write( reg, val ) ) 
		return;
	//Enabling panning
	if ( reg >= 0xc0 && reg <=0xc8 ) {
		val &= 0x0f;
		val |= index ? 0xA0 : 0x50;
	}
	Bit32u fullReg = reg + (index ? 0x100 : 0);
	handler->WriteReg( fullReg, val );
	CacheWrite( fullReg, val );
}


void Module::PortWrite( Bitu port, Bitu val, Bitu iolen ) {
	//Keep track of last write time
	lastUsed = PIC_Ticks;
	//Maybe only enable with a keyon?
	if ( !mixerChan->enabled ) {
		mixerChan->Enable(true);
	}
	if ( port&1 ) {
		if ( !chip[0].Write( reg.normal, val ) ) {
			handler->WriteReg( reg.normal, val );
			CacheWrite( reg.normal, val );
		}
	} else {
		reg.normal = handler->WriteAddr( port, val ) & 0x1ff;
	}
}


Bitu Module::PortRead( Bitu port, Bitu iolen ) {

	//We allocated 4 ports, so just return -1 for the higher ones
	if ( !(port & 3 ) ) {
		return chip[0].Read();
	} else {
		return 0xff;
	}

	return 0;
}


void Module::Init( Mode m ) {
	mode = m;
}

}; //namespace

//TODO: purge all Chinese Code from here

static Adlib::Module* module = 0;

static void OPL_CallBack(Bitu len) {
	module->handler->Generate( module->mixerChan, len );
	//Disable the sound generation after 30 seconds of silence
	if ((PIC_Ticks - module->lastUsed) > 30000) {
		Bitu i;
		for (i=0xb0;i<0xb9;i++)
			if ((module->cache[i]&0x20) || (module->cache[i+0x100]&0x20))
				break;
		if (i==0xb9) module->mixerChan->Enable(false);
		else module->lastUsed = PIC_Ticks;
	}
}

static Bitu OPL_Read(Bitu port,Bitu iolen) {
	return module->PortRead( port, iolen );
}

void OPL_Write(Bitu port,Bitu val,Bitu iolen) {
	module->PortWrite( port, val, iolen );
}


namespace Adlib {

Module::Module(void*)
{
	reg.dual[0] = 0;
	reg.dual[1] = 0;
	reg.normal = 0;
	handler = 0;
//	capture = 0;

//	Section_prop * section=static_cast<Section_prop *>(configuration);
	Bitu base = myldbi->GetConfig()->snd.sb_base; //section->Get_hex("sbbase");
	Bitu rate = myldbi->GetConfig()->snd.sb_opl_freq; //section->Get_int("oplrate");
	//Make sure we can't select lower than 8000 to prevent fixed point issues
	if ( rate < 8000 )
		rate = 8000;

	mixerChan = mixerObject.Install(OPL_CallBack,rate,"FM");
	mixerChan->SetScale( 2.0 );
	handler = new DBOPL::Handler();
	handler->Init( rate );
	bool single = false;
	Init( Adlib::MODE_OPL3 );
	//0x388 range
	WriteHandler[0].Install(0x388,OPL_Write,IO_MB, 4 );
	ReadHandler[0].Install(0x388,OPL_Read,IO_MB, 4 );
	//0x220 range
	if ( !single ) {
		WriteHandler[1].Install(base,OPL_Write,IO_MB, 4 );
		ReadHandler[1].Install(base,OPL_Read,IO_MB, 4 );
	}
	//0x228 range
	WriteHandler[2].Install(base+8,OPL_Write,IO_MB, 2);
	ReadHandler[2].Install(base+8,OPL_Read,IO_MB, 1);
}

Module::~Module() {
	/*
	if ( capture ) {
		delete capture;
	}*/
	if ( handler ) {
		delete handler;
	}
}

//Initialize static members
OPL_Mode Module::oplmode=OPL_none;

};	//Adlib Namespace


void OPL_Init(/*Section* sec,*/OPL_Mode oplmode) {
	Adlib::Module::oplmode = oplmode;
	module = new Adlib::Module(NULL);
}

void OPL_ShutDown(Section* /*sec*/){
	delete module;
	module = 0;

}

}
