#ifndef INC_OS_H_
#define INC_OS_H_

#include "sdram_conf.h"

#define OS_FONT_FILE "font.txt"
#define OS_BIOS_FILE "bios.img"
#define OS_FLOPPY_FILE "fd.img"

extern uint8_t* OS_Font_Array;
extern uint32_t OS_Font_Array_Size, OS_Last_Address;

int OS_InitFont();
void OS_PrintString(char* str);
uint8_t* OS_InitDisk(const char* name, uint32_t* len);
void OS();

#endif /* INC_OS_H_ */
