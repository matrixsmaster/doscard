#ifndef XSHELL_H_
#define XSHELL_H_

#include <string.h>
#include <inttypes.h>

int XS_UpdateScreenBuffer(void* buf, size_t len);
int XS_UpdateSoundBuffer(void* buf, size_t len);
int XS_QueryUIEvents(void* buf, size_t len);
uint32_t XS_GetTicks(void);

#endif /* XSHELL_H_ */
