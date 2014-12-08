/*
 *  Copyright (C) 2014  Dmitry Soloviov
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

#ifndef SYNCS_H_
#define SYNCS_H_

namespace doscard {

#define MUTEX_DEBUG

typedef volatile int32_t LDBI_Mutex;

#define MUTEX_GLOBAL_VAR LDBI_Mutex mutex_screen,mutex_sound,mutex_events,mutex_runtime,mutex_string,mutex_extend

extern MUTEX_GLOBAL_VAR;

#define MUTEX_GLOBAL_INIT {		mutex_screen  = 0; \
								mutex_sound   = 0; \
								mutex_events  = 0; \
								mutex_runtime = 0; \
								mutex_string  = 0; \
								mutex_extend  = 0; }

#define MUTEX_PRINT_INFO {		printf("mutex_screen:  %x\n",&mutex_screen);  \
								printf("mutex_sound:   %x\n",&mutex_sound);   \
								printf("mutex_events:  %x\n",&mutex_events);  \
								printf("mutex_runtime: %x\n",&mutex_runtime); \
								printf("mutex_string:  %x\n",&mutex_string);  \
								printf("mutex_extend:  %x\n",&mutex_extend);  }

#define MUTEX_IDLE usleep(1)
#define MUTEX_TEST(X) (X > 0)

#ifdef MUTEX_DEBUG
	#define MUTEX_LOCK(X) do {while (X) MUTEX_IDLE; X = 1; printf("LOCKED %x\n",&X);} while(0)
	#define MUTEX_UNLOCK(X) {X = 0; printf("UNLOCKED %x\n",&X);}
#else
	#define MUTEX_LOCK(X) do {while (X) MUTEX_IDLE; X = 1;} while(0)
	#define MUTEX_UNLOCK(X) X = 0
	#undef  MUTEX_PRINT_INFO
#endif

#define MUTEX_WAIT_FOR_EVENT(X,Y) do {MUTEX_LOCK(X); if (Y) break; MUTEX_UNLOCK(X); MUTEX_IDLE;} while(1)

#define LOCK_SCREEN		MUTEX_LOCK(mutex_screen)
#define LOCK_SOUND		MUTEX_LOCK(mutex_sound)
#define LOCK_EVENTS		MUTEX_LOCK(mutex_events)
#define LOCK_RUNTIME	MUTEX_LOCK(mutex_runtime)
#define LOCK_STRING		MUTEX_LOCK(mutex_string)
#define LOCK_EXTEND		MUTEX_LOCK(mutex_extend)

#define UNLOCK_SCREEN	MUTEX_UNLOCK(mutex_screen)
#define UNLOCK_SOUND	MUTEX_UNLOCK(mutex_sound)
#define UNLOCK_EVENTS	MUTEX_UNLOCK(mutex_events)
#define UNLOCK_RUNTIME	MUTEX_UNLOCK(mutex_runtime)
#define UNLOCK_STRING	MUTEX_UNLOCK(mutex_string)
#define UNLOCK_EXTEND	MUTEX_UNLOCK(mutex_extend)

}

#endif /* SYNCS_H_ */
