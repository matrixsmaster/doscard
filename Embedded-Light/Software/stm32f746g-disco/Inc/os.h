#ifndef INC_OS_H_
#define INC_OS_H_

#include "sdram_conf.h"

#define OS_FONT_FILE "font.txt"

extern uint8_t* OS_Font_Array;

int OS_InitFont();
void OS_PrintString(char* str);
void OS();

#endif /* INC_OS_H_ */
