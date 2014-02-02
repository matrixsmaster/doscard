/*
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

#include <inttypes.h>
#include <string.h>

#ifndef DOSBOX_LIBDOSBOX_H_
#define DOSBOX_LIBDOSBOX_H_

enum LDB_CallbackType {
	DBCB_GetTicks = 0,
	DBCB_PushScreen = 1,
	DBCB_PushSound = 2,
	DBCB_PullUIEvents = 3,
	DBCB_PushMessage = 4
};
#define LDB_CALLBACKSQ 5

typedef int (*LDB_CallbackFunc)(void*,size_t);

int Dosbox_RegisterCallback(LDB_CallbackType t, LDB_CallbackFunc f);
void* Dosbox_Run(void*);

#endif
