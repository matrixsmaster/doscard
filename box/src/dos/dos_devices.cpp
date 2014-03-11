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
#include "dosbox.h"
#include "callback.h"
#include "regs.h"
#include "mem.h"
#include "bios.h"
#include "dos_inc.h"
#include "support.h"
#include "drives.h"
#include "dev_con.h"

namespace dosbox {

DOS_Device * Devices[DOS_DEVICES];

class device_NUL : public DOS_Device {
public:
	device_NUL() { SetName("NUL"); };
	virtual bool Read(Bit8u * data,Bit16u * size) {
		for(Bitu i = 0; i < *size;i++) 
			data[i]=0; 
		LOG(LOG_IOCTL,LOG_NORMAL)("%s:READ",GetName());
		return true;
	}
	virtual bool Write(Bit8u * data,Bit16u * size) {
		LOG(LOG_IOCTL,LOG_NORMAL)("%s:WRITE",GetName());
		return true;
	}
	virtual bool Seek(Bit32u * pos,Bit32u type) {
		LOG(LOG_IOCTL,LOG_NORMAL)("%s:SEEK",GetName());
		return true;
	}
	virtual bool Close() { return true; }
	virtual Bit16u GetInformation(void) { return 0x8084; }
	virtual bool ReadFromControlChannel(PhysPt bufptr,Bit16u size,Bit16u * retcode){return false;}
	virtual bool WriteToControlChannel(PhysPt bufptr,Bit16u size,Bit16u * retcode){return false;}
};

class device_LPT1 : public device_NUL {
public:
	device_LPT1() { SetName("LPT1");}
	Bit16u GetInformation(void) { return 0x80A0; }
};

bool DOS_Device::Read(Bit8u * data,Bit16u * size) {
	return Devices[devnum]->Read(data,size);
}

bool DOS_Device::Write(Bit8u * data,Bit16u * size) {
	return Devices[devnum]->Write(data,size);
}

bool DOS_Device::Seek(Bit32u * pos,Bit32u type) {
	return Devices[devnum]->Seek(pos,type);
}

bool DOS_Device::Close() {
	return Devices[devnum]->Close();
}

Bit16u DOS_Device::GetInformation(void) { 
	return Devices[devnum]->GetInformation();
}

bool DOS_Device::ReadFromControlChannel(PhysPt bufptr,Bit16u size,Bit16u * retcode) { 
	return Devices[devnum]->ReadFromControlChannel(bufptr,size,retcode);
}

bool DOS_Device::WriteToControlChannel(PhysPt bufptr,Bit16u size,Bit16u * retcode) { 
	return Devices[devnum]->WriteToControlChannel(bufptr,size,retcode);
}

DOS_File::DOS_File(const DOS_File& orig) {
	flags=orig.flags;
	time=orig.time;
	date=orig.date;
	attr=orig.attr;
	refCtr=orig.refCtr;
	open=orig.open;
	hdrive=orig.hdrive;
	name=0;
	if(orig.name) {
		name=new char [strlen(orig.name) + 1];strcpy(name,orig.name);
	}
}

DOS_File & DOS_File::operator= (const DOS_File & orig) {
	flags=orig.flags;
	time=orig.time;
	date=orig.date;
	attr=orig.attr;
	refCtr=orig.refCtr;
	open=orig.open;
	hdrive=orig.hdrive;
	if(name) {
		delete [] name; name=0;
	}
	if(orig.name) {
		name=new char [strlen(orig.name) + 1];strcpy(name,orig.name);
	}
	return *this;
}

bool device_CON::Read(Bit8u * data,Bit16u * size) {
	Bit16u oldax=reg_ax;
	Bit16u count=0;
	if ((readcache) && (*size)) {
		data[count++]=readcache;
		if(dos.echo) INT10_TeletypeOutput(readcache,7);
		readcache=0;
	}
	while (*size>count) {
		reg_ah = 0x10;
		CALLBACK_RunRealInt(0x16);
		switch(reg_al) {
		case 13:
			data[count++]=0x0D;
			if (*size>count) data[count++]=0x0A;    // it's only expanded if there is room for it. (NO cache)
			*size=count;
			reg_ax=oldax;
			if(dos.echo) {
				INT10_TeletypeOutput(13,7); //maybe don't do this ( no need for it actually ) (but it's compatible)
				INT10_TeletypeOutput(10,7);
			}
			return true;
			break;
		case 8:
			if(*size==1) data[count++]=reg_al;  //one char at the time so give back that BS
			else if(count) {                    //Remove data if it exists (extended keys don't go right)
				data[count--]=0;
				INT10_TeletypeOutput(8,7);
				INT10_TeletypeOutput(' ',7);
			} else {
				continue;                       //no data read yet so restart whileloop.
			}
			break;
		case 0xe0: /* Extended keys in the  int 16 0x10 case */
			if(!reg_ah) { /*extended key if reg_ah isn't 0 */
				data[count++] = reg_al;
			} else {
				data[count++] = 0;
				if (*size>count) data[count++] = reg_ah;
				else readcache = reg_ah;
			}
			break;
		case 0: /* Extended keys in the int 16 0x0 case */
			data[count++]=reg_al;
			if (*size>count) data[count++]=reg_ah;
			else readcache=reg_ah;
			break;
		default:
			data[count++]=reg_al;
			break;
		}
		if(dos.echo) { //what to do if *size==1 and character is BS ?????
			INT10_TeletypeOutput(reg_al,7);
		}
	}
	*size=count;
	reg_ax=oldax;
	return true;
}


bool device_CON::Write(Bit8u * data,Bit16u * size) {
	Bit16u count=0;
	Bitu i;
	Bit8u col,row;
	Bit8u tempdata;
	while (*size>count) {
		if (!ansi.esc){
			if(data[count]=='\033') {
				/*clear the datastructure */
				ClearAnsi();
				/* start the sequence */
				ansi.esc=true;
				count++;
				continue;
			} else {
				/* Some sort of "hack" now that '\n' doesn't set col to 0 (int10_char.cpp old chessgame) */
				if((data[count] == '\n') && (lastwrite != '\r')) INT10_TeletypeOutputAttr('\r',ansi.attr,ansi.enabled);
				/* pass attribute only if ansi is enabled */
				INT10_TeletypeOutputAttr(data[count],ansi.attr,ansi.enabled);
				lastwrite = data[count++];
				continue;
			}
		}

		if(!ansi.sci){

			switch(data[count]){
			case '[':
				ansi.sci=true;
				break;
			case '7': /* save cursor pos + attr */
			case '8': /* restore this  (Wonder if this is actually used) */
			case 'D':/* scrolling DOWN*/
			case 'M':/* scrolling UP*/
			default:
				LOG(LOG_IOCTL,LOG_NORMAL)("ANSI: unknown char %c after a esc",data[count]); /*prob () */
				ClearAnsi();
				break;
			}
			count++;
			continue;
		}
		/*ansi.esc and ansi.sci are true */
		Bit8u page = real_readb(BIOSMEM_SEG,BIOSMEM_CURRENT_PAGE);
		switch(data[count]){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			ansi.data[ansi.numberofarg]=10*ansi.data[ansi.numberofarg]+(data[count]-'0');
			break;
		case ';': /* till a max of NUMBER_ANSI_DATA */
			ansi.numberofarg++;
			break;
		case 'm':               /* SGR */
			for(i=0;i<=ansi.numberofarg;i++){
				ansi.enabled=true;
				switch(ansi.data[i]){
				case 0: /* normal */
					ansi.attr=0x07;//Real ansi does this as well. (should do current defaults)
					ansi.enabled=false;
					break;
				case 1: /* bold mode on*/
					ansi.attr|=0x08;
					break;
				case 4: /* underline */
					LOG(LOG_IOCTL,LOG_NORMAL)("ANSI:no support for underline yet");
					break;
				case 5: /* blinking */
					ansi.attr|=0x80;
					break;
				case 7: /* reverse */
					ansi.attr=0x70;//Just like real ansi. (should do use current colors reversed)
					break;
				case 30: /* fg color black */
					ansi.attr&=0xf8;
					ansi.attr|=0x0;
					break;
				case 31:  /* fg color red */
					ansi.attr&=0xf8;
					ansi.attr|=0x4;
					break;
				case 32:  /* fg color green */
					ansi.attr&=0xf8;
					ansi.attr|=0x2;
					break;
				case 33: /* fg color yellow */
					ansi.attr&=0xf8;
					ansi.attr|=0x6;
					break;
				case 34: /* fg color blue */
					ansi.attr&=0xf8;
					ansi.attr|=0x1;
					break;
				case 35: /* fg color magenta */
					ansi.attr&=0xf8;
					ansi.attr|=0x5;
					break;
				case 36: /* fg color cyan */
					ansi.attr&=0xf8;
					ansi.attr|=0x3;
					break;
				case 37: /* fg color white */
					ansi.attr&=0xf8;
					ansi.attr|=0x7;
					break;
				case 40:
					ansi.attr&=0x8f;
					ansi.attr|=0x0;
					break;
				case 41:
					ansi.attr&=0x8f;
					ansi.attr|=0x40;
					break;
				case 42:
					ansi.attr&=0x8f;
					ansi.attr|=0x20;
					break;
				case 43:
					ansi.attr&=0x8f;
					ansi.attr|=0x60;
					break;
				case 44:
					ansi.attr&=0x8f;
					ansi.attr|=0x10;
					break;
				case 45:
					ansi.attr&=0x8f;
					ansi.attr|=0x50;
					break;
				case 46:
					ansi.attr&=0x8f;
					ansi.attr|=0x30;
					break;
				case 47:
					ansi.attr&=0x8f;
					ansi.attr|=0x70;
					break;
				default:
					break;
				}
			}
			ClearAnsi();
			break;
		case 'f':
		case 'H':/* Cursor Pos*/
			if(!ansi.warned) { //Inform the debugger that ansi is used.
				ansi.warned = true;
				LOG(LOG_IOCTL,LOG_WARN)("ANSI SEQUENCES USED");
			}
			/* Turn them into positions that are on the screen */
			if(ansi.data[0] == 0) ansi.data[0] = 1;
			if(ansi.data[1] == 0) ansi.data[1] = 1;
			if(ansi.data[0] > ansi.nrows) ansi.data[0] = (Bit8u)ansi.nrows;
			if(ansi.data[1] > ansi.ncols) ansi.data[1] = (Bit8u)ansi.ncols;
			INT10_SetCursorPos(--(ansi.data[0]),--(ansi.data[1]),page); /*ansi=1 based, int10 is 0 based */
			ClearAnsi();
			break;
			/* cursor up down and forward and backward only change the row or the col not both */
		case 'A': /* cursor up*/
			col=CURSOR_POS_COL(page) ;
			row=CURSOR_POS_ROW(page) ;
			tempdata = (ansi.data[0]? ansi.data[0] : 1);
			if(tempdata > row) { row=0; }
			else { row-=tempdata;}
			INT10_SetCursorPos(row,col,page);
			ClearAnsi();
			break;
		case 'B': /*cursor Down */
			col=CURSOR_POS_COL(page) ;
			row=CURSOR_POS_ROW(page) ;
			tempdata = (ansi.data[0]? ansi.data[0] : 1);
			if(tempdata + static_cast<Bitu>(row) >= ansi.nrows)
			{ row = ansi.nrows - 1;}
			else	{ row += tempdata; }
			INT10_SetCursorPos(row,col,page);
			ClearAnsi();
			break;
		case 'C': /*cursor forward */
			col=CURSOR_POS_COL(page);
			row=CURSOR_POS_ROW(page);
			tempdata=(ansi.data[0]? ansi.data[0] : 1);
			if(tempdata + static_cast<Bitu>(col) >= ansi.ncols)
			{ col = ansi.ncols - 1;}
			else	{ col += tempdata;}
			INT10_SetCursorPos(row,col,page);
			ClearAnsi();
			break;
		case 'D': /*Cursor Backward  */
			col=CURSOR_POS_COL(page);
			row=CURSOR_POS_ROW(page);
			tempdata=(ansi.data[0]? ansi.data[0] : 1);
			if(tempdata > col) {col = 0;}
			else { col -= tempdata;}
			INT10_SetCursorPos(row,col,page);
			ClearAnsi();
			break;
		case 'J': /*erase screen and move cursor home*/
			if(ansi.data[0]==0) ansi.data[0]=2;
			if(ansi.data[0]!=2) {/* every version behaves like type 2 */
				LOG(LOG_IOCTL,LOG_NORMAL)("ANSI: esc[%dJ called : not supported handling as 2",ansi.data[0]);
			}
			INT10_ScrollWindow(0,0,255,255,0,ansi.attr,page);
			ClearAnsi();
			INT10_SetCursorPos(0,0,page);
			break;
		case 'h': /* SET   MODE (if code =7 enable linewrap) */
		case 'I': /* RESET MODE */
			LOG(LOG_IOCTL,LOG_NORMAL)("ANSI: set/reset mode called(not supported)");
			ClearAnsi();
			break;
		case 'u': /* Restore Cursor Pos */
			INT10_SetCursorPos(ansi.saverow,ansi.savecol,page);
			ClearAnsi();
			break;
		case 's': /* SAVE CURSOR POS */
			ansi.savecol=CURSOR_POS_COL(page);
			ansi.saverow=CURSOR_POS_ROW(page);
			ClearAnsi();
			break;
		case 'K': /* erase till end of line (don't touch cursor) */
			col = CURSOR_POS_COL(page);
			row = CURSOR_POS_ROW(page);
			INT10_WriteChar(' ',ansi.attr,page,ansi.ncols-col,true); //Use this one to prevent scrolling when end of screen is reached
			//for(i = col;i<(Bitu) ansi.ncols; i++) INT10_TeletypeOutputAttr(' ',ansi.attr,true);
			INT10_SetCursorPos(row,col,page);
			ClearAnsi();
			break;
		case 'M': /* delete line (NANSI) */
			col = CURSOR_POS_COL(page);
			row = CURSOR_POS_ROW(page);
			INT10_ScrollWindow(row,0,ansi.nrows-1,ansi.ncols-1,ansi.data[0]? -ansi.data[0] : -1,ansi.attr,0xFF);
			ClearAnsi();
			break;
		case 'l':/* (if code =7) disable linewrap */
		case 'p':/* reassign keys (needs strings) */
		case 'i':/* printer stuff */
		default:
			LOG(LOG_IOCTL,LOG_NORMAL)("ANSI: unhandled char %c in esc[",data[count]);
			ClearAnsi();
			break;
		}
		count++;
	}
	*size=count;
	return true;
}

bool device_CON::Seek(Bit32u * pos,Bit32u type) {
	// seek is valid
	*pos = 0;
	return true;
}

bool device_CON::Close() {
	return true;
}

Bit16u device_CON::GetInformation(void) {
	Bit16u head=mem_readw(BIOS_KEYBOARD_BUFFER_HEAD);
	Bit16u tail=mem_readw(BIOS_KEYBOARD_BUFFER_TAIL);

	if ((head==tail) && !readcache) return 0x80D3;	/* No Key Available */
	if (readcache || real_readw(0x40,head)) return 0x8093;		/* Key Available */

	/* remove the zero from keyboard buffer */
	Bit16u start=mem_readw(BIOS_KEYBOARD_BUFFER_START);
	Bit16u end	=mem_readw(BIOS_KEYBOARD_BUFFER_END);
	head+=2;
	if (head>=end) head=start;
	mem_writew(BIOS_KEYBOARD_BUFFER_HEAD,head);
	return 0x80D3; /* No Key Available */
}

device_CON::device_CON() {
	SetName("CON");
	readcache=0;
	lastwrite=0;
	ansi.enabled=false;
	ansi.attr=0x7;
	ansi.ncols=real_readw(BIOSMEM_SEG,BIOSMEM_NB_COLS); //should be updated once set/reset mode is implemented
	ansi.nrows=real_readb(BIOSMEM_SEG,BIOSMEM_NB_ROWS) + 1;
	ansi.saverow=0;
	ansi.savecol=0;
	ansi.warned=false;
	ClearAnsi();
}

void device_CON::ClearAnsi(void){
	for(Bit8u i=0; i<NUMBER_ANSI_DATA;i++) ansi.data[i]=0;
	ansi.esc=false;
	ansi.sci=false;
	ansi.numberofarg=0;
}

Bit8u DOS_FindDevice(char const * name) {
	/* should only check for the names before the dot and spacepadded */
	char fullname[DOS_PATHLENGTH];Bit8u drive;
	//	if(!name || !(*name)) return DOS_DEVICES; //important, but makename does it
	if (!DOS_MakeName(name,fullname,&drive)) return DOS_DEVICES;

	char* name_part = strrchr(fullname,'\\');
	if(name_part) {
		*name_part++ = 0;
		//Check validity of leading directory.
		if(!Drives[drive]->TestDir(fullname)) return DOS_DEVICES;
	} else name_part = fullname;

	char* dot = strrchr(name_part,'.');
	if(dot) *dot = 0; //no ext checking

	static char com[5] = { 'C','O','M','1',0 };
	static char lpt[5] = { 'L','P','T','1',0 };
	// AUX is alias for COM1 and PRN for LPT1
	// A bit of a hack. (but less then before).
	// no need for casecmp as makename returns uppercase
	if (strcmp(name_part, "AUX") == 0) name_part = com;
	if (strcmp(name_part, "PRN") == 0) name_part = lpt;

	/* loop through devices */
	for(Bit8u index = 0;index < DOS_DEVICES;index++) {
		if (Devices[index]) {
			if (WildFileCmp(name_part,Devices[index]->name)) return index;
		}
	}
	return DOS_DEVICES;
}


void DOS_AddDevice(DOS_Device * adddev) {
	//Caller creates the device. We store a pointer to it
	//TODO Give the Device a real handler in low memory that responds to calls
	for(Bitu i = 0; i < DOS_DEVICES;i++) {
		if(!Devices[i]){
			Devices[i] = adddev;
			Devices[i]->SetDeviceNumber(i);
			return;
		}
	}
	E_Exit("DOS:Too many devices added");
}

void DOS_DelDevice(DOS_Device * dev) {
	// We will destroy the device if we find it in our list.
	// TODO:The file table is not checked to see the device is opened somewhere!
	for (Bitu i = 0; i <DOS_DEVICES;i++) {
		if(Devices[i] && !strcasecmp(Devices[i]->name,dev->name)){
			delete Devices[i];
			Devices[i] = 0;
			return;
		}
	}
}

void DOS_SetupDevices(void) {
	DOS_Device * newdev;
	newdev=new device_CON();
	DOS_AddDevice(newdev);
	DOS_Device * newdev2;
	newdev2=new device_NUL();
	DOS_AddDevice(newdev2);
	DOS_Device * newdev3;
	newdev3=new device_LPT1();
	DOS_AddDevice(newdev3);
}

}
