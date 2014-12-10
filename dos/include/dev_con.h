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

#include "dos_inc.h"
#include "int10.h"
#include <string.h>

namespace dosbox {

#define NUMBER_ANSI_DATA 10

class device_CON : public DOS_Device {
public:
	device_CON();
	bool Read(Bit8u * data,Bit16u * size);
	bool Write(Bit8u * data,Bit16u * size);
	bool Seek(Bit32u * pos,Bit32u type);
	bool Close();
	void ClearAnsi(void);
	Bit16u GetInformation(void);
	bool ReadFromControlChannel(PhysPt bufptr,Bit16u size,Bit16u * retcode){return false;}
	bool WriteToControlChannel(PhysPt bufptr,Bit16u size,Bit16u * retcode){return false;}
private:
	Bit8u readcache;
	Bit8u lastwrite;
	struct ansi { /* should create a constructor, which would fill them with the appropriate values */
		bool esc;
		bool sci;
		bool enabled;
		Bit8u attr;
		Bit8u data[NUMBER_ANSI_DATA];
		Bit8u numberofarg;
		Bit16u nrows;
		Bit16u ncols;
		Bit8s savecol;
		Bit8s saverow;
		bool warned;
	} ansi;
};

} //namespace dosbox
