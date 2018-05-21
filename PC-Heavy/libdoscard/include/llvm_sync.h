/*
 *  Copyright (C) 2014-2015  Dmitry Soloviov
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

//IMPORTANT: Reverted to more stable, but less functional 'kludge' version

#ifndef LLVM_SYNC_H_
#define LLVM_SYNC_H_

#define USE_WAITING_MUTEXES 3

//typedef atomic_int LDBI_Mutex;
//TODO: play around __sync_bool_compare_and_swap();
typedef volatile unsigned int LDBI_Mutex;

/* ****************** Weak Sync Macros ****************** */

#define MUTEX_UNLOCK mutex = 0

#ifdef USE_WAITING_MUTEXES
#define MUTEX_LOCK do {while (mutex) usleep(USE_WAITING_MUTEXES); mutex = 1;} while(0)
#define MUTEX_WAIT_FOR_EVENT(X) do {MUTEX_LOCK; if (X) break; MUTEX_UNLOCK; usleep(USE_WAITING_MUTEXES*2);} while(1)
#else
#define MUTEX_LOCK do {while (mutex); mutex = 1;} while(0)
#define MUTEX_WAIT_FOR_EVENT(X) do {MUTEX_LOCK; if (X) break; MUTEX_UNLOCK; } while(1)
#endif


#endif /* LLVM_SYNC_H_ */
