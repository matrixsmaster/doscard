#ifndef XSHELL_H_
#define XSHELL_H_

#include <string.h>
#include <inttypes.h>
#include "ldb.h"


//LDB_CallbackFunc XS_UpdateScreenBuffer;
//LDB_CallbackFunc XS_UpdateSoundBuffer;
//LDB_CallbackFunc XS_QueryUIEvents;
//LDB_CallbackFunc XS_GetTicks;

int XS_UpdateScreenBuffer(void* buf, size_t len);
int XS_UpdateSoundBuffer(void* buf, size_t len);
int XS_QueryUIEvents(void* buf, size_t len);
int XS_GetTicks(void* buf, size_t len);

#endif /* XSHELL_H_ */
