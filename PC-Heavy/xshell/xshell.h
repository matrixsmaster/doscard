/*
 *  Copyright (C) 2013-2014  Dmitry Soloviov
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

#ifndef XSHELL_H_
#define XSHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include "ldb.h"

#ifdef LDB_EMBEDDED
#error LDB_EMBEDDED is defined
#endif

#define XSHELL_CAPTION "DOSCARD XShell"
#define XSHELL_DEF_WND_W 640
#define XSHELL_DEF_WND_H 480

int32_t XS_UpdateScreenBuffer(void* buf, size_t len);
int32_t XS_UpdateSoundBuffer(void* buf, size_t len);
int32_t XS_QueryUIEvents(void* buf, size_t len);
int32_t XS_GetTicks(void* buf, size_t len);
int32_t XS_Message(void* buf, size_t len);
int32_t XS_FIO(void* buf, size_t len);

void XS_AudioCallback(void* userdata, uint8_t* stream, int len);

#endif /* XSHELL_H_ */
