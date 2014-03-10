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

#ifndef DOSBOX_LIBDOSBOX_H_
#define DOSBOX_LIBDOSBOX_H_

#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "keyboard.h"

//LDB_EMBEDDED should be defined when compiling for a real chip target
//#define LDB_EMBEDDED
#define LDB_SUPPORT_DIRS
//#define LDB_ALLOW_TIMER_DELAY

#ifndef LDB_EMBEDDED
#include <stdio.h>
#else
#undef LDB_SUPPORT_DIRS
#endif

namespace dosbox {

/* ****************** LDB LCD Display Pipe Macros ****************** */

#define DISPLAY_INIT_SIGNATURE 0xFFABCD00
#define DISPLAY_NFRM_SIGNATURE 0xAABBCCDD
#define DISPLAY_ABOR_SIGNATURE 0xABCDABCD

#define DISPLAY_RET_BUSY 10

/* ****************** LDB Basic Data ****************** */

enum LDB_CallbackType {
	DBCB_GetTicks = 0,
	DBCB_PushScreen = 1,
	DBCB_PushSound = 2,
	DBCB_PullUIEvents = 3,
	DBCB_PushMessage = 4,
	DBCB_FileIOReq = 5,
	DBCB_LogSTDOUT = 6,
	DBCB_PullTTYInput = 7
};
#define LDB_CALLBACKSQ 8

typedef int (*LDB_CallbackFunc)(void*,size_t);

#define LDB_CALLBACK_RET_NOT_FOUND -21

/* ****************** LDB Basic Functions Prototypes ****************** */

//int Dosbox_RegisterCallback(LDB_CallbackType t, LDB_CallbackFunc f);
//int Dosbox_Run(void*);

/* ****************** Events Pipe Data Structures ****************** */

enum LDB_UIEventE {
	LDB_UIE_NONE = 0,
	LDB_UIE_QUIT = 1,
	LDB_UIE_KBD = 2,
	LDB_UIE_MOUSE = 3
};

typedef struct {
	float x,y;
} LDBFloat2D;

typedef struct {
	LDBFloat2D rel,abs;
	uint8_t button;
} LDB_MOUSEINF;

typedef struct {
	LDB_UIEventE t; 	//event type
	KBD_KEYS key;		//keyboard key
	bool pressed;		//is pressed?
	LDB_MOUSEINF m;		//mouse info
} LDB_UIEvent;

typedef struct {
	uint8_t width,channels;
	uint32_t freq,blocksize;
	bool sign,silent;
} LDB_SoundInfo;

/* ****************** FIO Wrapper Data ****************** */

typedef struct {
	char* name;
#ifdef LDB_EMBEDDED
	uint8_t hand;
	uint8_t op;
#else
	char* op;
	FILE* rf;
	DIR* df;
#endif
	int16_t todo;
	uint32_t p_x,p_y;
	void* buf;
} DBFILE;

#define LDB_FOP_NEW 0x80
#define LDB_FOP_READ 0x01
#define LDB_FOP_WRITE 0x02
#define LDB_FOP_BEGIN 0x04
#define LDB_FOP_WRAPP 0x08
#define LDB_FOP_TRUNC 0x10

/* ****************** FIO Wrapper Functions Prototypes ****************** */

DBFILE* dbfopen(const char* p, const char* m);
void dbfclose(DBFILE* f);
uint32_t dbfread(void* p, uint32_t sz, uint32_t q, DBFILE* f);
uint32_t dbfwrite(const void* p, uint32_t sz, uint32_t q, DBFILE* f);
int32_t dbfseek(DBFILE* f, uint64_t off, int32_t wh);
uint64_t dbftell(DBFILE* f);
int32_t dbfeof(DBFILE* f);
int32_t dbftruncate(DBFILE* f, int64_t len);
int32_t dbfprintf(DBFILE* f, const char *fmt, ...);
int32_t dbfngetl(char* buf, int32_t n, DBFILE* f);
bool dbisfilex(const char* name);
bool dbisdirex(const char* path);
bool dbisitexist(const char* path);
int32_t dbgetfilesize(const char* path);
int32_t dbrename(const char* oldn, const char* newn);

DBFILE* dbdiropen(const char* p);
bool dbdirread(DBFILE* d, char* entry, bool& is_dir);
void dbdirclose(DBFILE* d);
int32_t dbdirmake(const char* n);
int32_t dbdirdel(const char* n);

/* ****************** Initial Date and Time ****************** */

#define LDB_DEFTIME_HR 16
#define LDB_DEFTIME_MN 0
#define LDB_DEFTIME_SC 1
#define LDB_DEFDATE_YR 1999
#define LDB_DEFDATE_MN 1
#define LDB_DEFDATE_DY 1

} // namespace dosbox

#endif
