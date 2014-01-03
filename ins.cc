/*
 * instrument.cpp
 *
 *  Created on: Jan 3, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef unsigned long Bit64u;
typedef   signed long Bit64s;
typedef Bit64u Bitu;
typedef Bit64s Bits;
  

//#define PROFILE_SIMPLE_FILE_CALL_TRACE 1
#define PROFILE_UNIQUE_CALL_LIST 1
//#define PROFILE_TREE_CALL_TRACE 1

#define PROFILE_OUT_FILENAME "/tmp/profile"
#define PROFILE_LIST_GROW_BY 16

static FILE* fd = NULL;
static struct {
	void* fun;
	void* child;
} * tree = NULL;
static Bitu* list = NULL;
static Bit64u* weight = NULL;
static Bit64u index = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void __attribute__ ((constructor)) profile_create()
{
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
#endif
	printf("[PROFILE]: Started\n");
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
		printf("%p\n",ptr);
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

