/*
 * instrument.cpp
 *
 *  Created on: Jan 3, 2014
 */

#include <stdio.h>
#include <stdlib.h>

static FILE* fd = NULL;

extern "C" void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	printf("[PRF]: enter()\n");
}

extern "C" void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	printf("[PRF]: exit()\n");
}
