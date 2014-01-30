#ifndef XSHELL_H_
#define XSHELL_H_

#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "SDL2/include/SDL.h"
#include "ldb.h"

#ifdef XSHELL_VERBOSE
void xvrb(char const* format,...);
#endif

int XS_UpdateScreenBuffer(void* buf, size_t len);
int XS_UpdateSoundBuffer(void* buf, size_t len);
int XS_QueryUIEvents(void* buf, size_t len);
int XS_GetTicks(void* buf, size_t len);

#endif /* XSHELL_H_ */
