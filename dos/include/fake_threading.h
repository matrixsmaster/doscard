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

#ifndef FAKE_THREADING_H_
#define FAKE_THREADING_H_

static int XSF_MutexCount = 0;

//XShell Fake Mutex
typedef volatile int XSF_Mutex;

XSF_Mutex* XSF_MutexCreate();
void XSF_MutexDestroy(XSF_Mutex* m);
int XSF_MutexL(XSF_Mutex* m);
int XSF_MutexU(XSF_Mutex* m);

#endif /* FAKE_THREADING_H_ */
