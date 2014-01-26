/*
 * fake_threading.h
 *
 *  Created on: Jan 4, 2014
 */

#ifndef FAKE_THREADING_H_
#define FAKE_THREADING_H_

static int XSF_MutexCount = 0;

//XShell Fake Mutex
typedef volatile int XSF_Mutex;

XSF_Mutex* XSF_MutexCreate();
void XSF_MutexDestroy(XSF_Mutex* m);
int XSF_MutexL(XSF_Mutex* m);
int XSF_MutexU(XSF_Mutex* m);

#endif /* FAKE_THREADING_H_ */
