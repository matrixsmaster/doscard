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

//typedef atomic_int LDBI_Mutex;
//TODO: __sync_bool_compare_and_swap();

typedef volatile unsigned int LDBI_Mutex;
#define MUTEX_LOCK do {while (mutex) usleep(1); mutex = 1;} while(0)
#define MUTEX_UNLOCK mutex = 0
#define MUTEX_WAIT_FOR_EVENT(X) do {MUTEX_LOCK; if (X) break; MUTEX_UNLOCK; usleep(2);} while(1)


#endif /* LLVM_SYNC_H_ */
