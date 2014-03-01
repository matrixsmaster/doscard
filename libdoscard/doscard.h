/*
 *  Copyright (C) 2014  Soloviov Dmitry
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

#ifndef DOSCARD_H_
#define DOSCARD_H_

#include "ldbwrap.h"

namespace doscard {

enum EDOSCRDState {
	DOSCRD_NOT_READY = 0,
	DOSCRD_LOADED,
	DOSCRD_INITED,
	DOSCRD_RUNNING,
	DOSCRD_SHUTDOWN,
	DOSCRD_LOADFAIL
};

class CDosCard {
public:
	CDosCard(bool autoload);
	~CDosCard();
	bool TryLoad(const char* filename);
	EDOSCRDState GetCurrentState();
	dosbox::LDB_Settings* GetSettings();
	bool ApplySettings(dosbox::LDB_Settings* pset);
	bool Prepare();
	int Run();
private:
	dosbox::LDB_Settings* settings;
};

} //namespace doscard

#endif /* DOSCARD_H_ */
