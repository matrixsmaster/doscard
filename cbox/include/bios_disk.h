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

#ifndef DOSBOX_BIOS_DISK_H
#define DOSBOX_BIOS_DISK_H

#include "mem.h"
#include "dos_inc.h"
#include "bios.h"

/* The Section handling Bios Disk Access */
#define BIOS_MAX_DISK 10

#define MAX_SWAPPABLE_DISKS 20
struct diskGeo {
	Bit32u ksize;  /* Size in kilobytes */
	Bit16u secttrack; /* Sectors per track */
	Bit16u headscyl;  /* Heads per cylinder */
	Bit16u cylcount;  /* Cylinders per side */
	Bit16u biosval;   /* Type to return from BIOS */
};

class imageDisk  {
public:
	Bit8u Read_Sector(Bit32u head,Bit32u cylinder,Bit32u sector,void * data);
	Bit8u Write_Sector(Bit32u head,Bit32u cylinder,Bit32u sector,void * data);
	Bit8u Read_AbsoluteSector(Bit32u sectnum, void * data);
	Bit8u Write_AbsoluteSector(Bit32u sectnum, void * data);

	void Set_Geometry(Bit32u setHeads, Bit32u setCyl, Bit32u setSect, Bit32u setSectSize);
	void Get_Geometry(Bit32u * getHeads, Bit32u *getCyl, Bit32u *getSect, Bit32u *getSectSize);
	Bit8u GetBiosType(void);
	Bit32u getSectSize(void);
	imageDisk(DBFILE* imgFile, Bit8u *imgName, Bit32u imgSizeK, bool isHardDisk);
	~imageDisk() { if(diskimg != NULL) { dbfclose(diskimg); }	};

	bool hardDrive;
	bool active;
	DBFILE* diskimg;
	Bit8u diskname[512];
	Bit8u floppytype;

	Bit32u sector_size;
	Bit32u heads,cylinders,sectors;
	Bit32u current_fpos;
};

void updateDPT(void);
void incrementFDD(void);

#define MAX_HDD_IMAGES 2

void swapInDisks(void);
void swapInNextDisk(void);
bool getSwapRequest(void);

#endif
