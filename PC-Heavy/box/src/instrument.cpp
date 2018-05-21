/*
 * instrument.cpp
 *
 * Instrumentation module for DosCard Project analyze and profiling
 *  (C) Dmitry "MatrixS_Master" Soloviov, 2014
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

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/config.h"


#define PROFILE_OUT_FILENAME "boxprofile"
#define PROFILE_LIST_GROW_BY 8192

static FILE* fd = NULL;
//static struct {
//	void* fun;
//	void* child;
//} * tree = NULL;
static Bitu* list = NULL;
static Bit64u* weight = NULL;
static Bit64u index = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void __attribute__ ((constructor)) profile_create()
{
	printf("[PROFILE]: Started\n");
#if (PROFILE_SIMPLE_FILE_CALL_TRACE)
	fd = fopen(PROFILE_OUT_FILENAME,"w");
#elif (PROFILE_UNIQUE_CALL_LIST)
	list = reinterpret_cast<Bitu*> (malloc(PROFILE_LIST_GROW_BY*sizeof(Bitu)));
	weight = reinterpret_cast<Bit64u*> (malloc(PROFILE_LIST_GROW_BY*sizeof(Bit64u)));
	if ((!list) || (!weight)) {
		fprintf(stderr,"[PROFILE]: FATAL: Unable to allocate list memory!\n");
		abort();
	}
	index = 0;
#elif (PROFILE_TREE_CALL_TRACE)
#else
	printf("[PROFILE]: Stub mode\n");
#endif
}

void __attribute__ ((destructor)) profile_destroy()
{
#if (PROFILE_SIMPLE_FILE_CALL_TRACE)
	if (fd) fclose(fd);
#elif (PROFILE_UNIQUE_CALL_LIST)
	fd = fopen(PROFILE_OUT_FILENAME,"w");
	if (!fd) {
		fprintf(stderr,"[PROFILE]: ERR: Unable to open profile output file!\n");
		return;
	}
	void* ptr;
	for (Bit64u i=0; i<index; i++) {
		ptr = reinterpret_cast<void*> (list[i]);
//		printf("%p\n",ptr);
		fprintf(fd,"%p\n",ptr);
	}
	fclose(fd);
	free(weight);
	free(list);
#elif (PROFILE_TREE_CALL_TRACE)
#endif
	printf("[PROFILE]: Exit\n");
}

extern "C" void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
#if (PROFILE_SIMPLE_FILE_CALL_TRACE)
	if (fd) fprintf(fd,"> %p %p\n",this_fn,call_site);
#elif (PROFILE_UNIQUE_CALL_LIST)
	if (!list) return;
	pthread_mutex_lock(&mutex);
	if (index > 1) {
		for (Bit64u i=index-1; i>0; i--)
			if (list[i] == reinterpret_cast<Bitu> (this_fn)) {
				weight[i]++; //we don't care of overflows -> it will create more dynamics :)
				if ((i < index-1) && (weight[i+1] < weight[i])) {
					//swap
					Bit64u xw = weight[i];
					weight[i] = weight[i+1];
					weight[i+1] = xw;
					Bitu xl = list[i];
					list[i] = list[i+1];
					list[i+1] = xl;
				}
				pthread_mutex_unlock(&mutex);
				return;
			}
	}
	if ((++index) % PROFILE_LIST_GROW_BY == 0) {
		list = reinterpret_cast<Bitu*> (realloc(list,(index+PROFILE_LIST_GROW_BY)*sizeof(Bitu)));
		weight = reinterpret_cast<Bit64u*> (realloc(weight,(index+PROFILE_LIST_GROW_BY)*sizeof(Bit64u)));
		if ((!list) || (!weight)) {
			fprintf(stderr,"\n[PROFILE]: FATAL: OUT OF MEMORY!\n");
			abort();
		}
	}
	list[index-1] = reinterpret_cast<Bitu> (this_fn);
	weight[index-1] = 1;
	pthread_mutex_unlock(&mutex);
#elif (PROFILE_TREE_CALL_TRACE)
#endif
}

extern "C" void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
#if (PROFILE_SIMPLE_FILE_CALL_TRACE)
	if (fd) fprintf(fd,"< %p %p\n",this_fn,call_site);
#elif (PROFILE_UNIQUE_CALL_LIST)
#elif (PROFILE_TREE_CALL_TRACE)
#endif
}
