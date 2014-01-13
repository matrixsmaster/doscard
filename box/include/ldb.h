#include <inttypes.h>
#include <string.h>

#ifndef DOSBOX_LIBDOSBOX_H_
#define DOSBOX_LIBDOSBOX_H_

enum LDB_CallbackType {
	DBCB_GetTicks = 0,
	DBCB_PushScreen = 1,
	DBCB_PushSound = 2,
	DBCB_PullUIEvents = 3
};

typedef int (*LDB_CallbackFunc)(void*,size_t);

int Dosbox_RegisterCallback(LDB_CallbackType t, LDB_CallbackFunc f);

#endif
