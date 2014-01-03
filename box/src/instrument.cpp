/*
 * instrument.cpp
 *
 *  Created on: Jan 3, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/config.h"

//#define PROFILE_SIMPLE_FILE_CALL_TRACE 1
#define PROFILE_UNIQUE_CALL_LIST 1
//#define PROFILE_TREE_CALL_TRACE 1

#define PROFILE_OUT_FILENAME "/tmp/boxprofile"
#define PROFILE_LIST_GROW_BY 4096

static FILE* fd = NULL;
static struct {
	void* fun;
	void* child;
} * tree = NULL;
static Bitu* list = NULL;
static unsigned long long index = 0;
volatile int mutex = 0;

void __attribute__ ((constructor)) profile_create()
{
#if (PROFILE_SIMPLE_FILE_CALL_TRACE)
	fd = fopen(PROFILE_OUT_FILENAME,"w");
#elif (PROFILE_UNIQUE_CALL_LIST)
	list = reinterpret_cast<Bitu*> (malloc(PROFILE_LIST_GROW_BY*sizeof(Bitu)));
//	index = PROFILE_LIST_GROW_BY;
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
		fprintf(stderr,"[PROFILE]: Unable to open profile output file!\n");
		return;
	}
	void* ptr;
	for (unsigned long long i=0; i<index; i++) {
		ptr = reinterpret_cast<void*> (list[i]);
		fprintf(fd,"%p\n",ptr);
	}
	free(list);
	fclose(fd);
#elif (PROFILE_TREE_CALL_TRACE)
#endif
	printf("[PROFILE]: Exit\n");
}

extern "C" void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
#if (PROFILE_SIMPLE_FILE_CALL_TRACE)
	if (fd) fprintf(fd,"> %p %p\n",this_fn,call_site);
#elif (PROFILE_UNIQUE_CALL_LIST)
	while (mutex) ;
	mutex = 1;
	for (unsigned long long i=0; i<index; i++)
		if (list[i] == reinterpret_cast<Bitu> (this_fn)) return;
	if ((++index) % PROFILE_LIST_GROW_BY == 0) {
		list = reinterpret_cast<Bitu*> (realloc(list,(index+PROFILE_LIST_GROW_BY)*sizeof(Bitu)));
		if (list == NULL) {
			fprintf(stderr,"\n[PROFILE]: OUT OF MEMORY!\n");
			abort();
		}
	}
	list[index] = reinterpret_cast<Bitu> (this_fn);
	mutex = 0;
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

