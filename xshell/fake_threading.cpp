/*
 * fake_threading.cpp
 *
 *  Created on: Jan 26, 2014
 */
#include "fake_threading.h"

XSF_Mutex* XSF_MutexCreate()
{
	return 0;
}

void XSF_MutexDestroy(XSF_Mutex* m)
{
	//
}

int XSF_MutexL(XSF_Mutex* m)
{
	return 0;
}

int XSF_MutexU(XSF_Mutex* m)
{
	return 0;
}
