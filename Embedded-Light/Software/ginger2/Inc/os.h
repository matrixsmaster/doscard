/*
 * This file is a part of the DOSCard project.
 *
 * (C) Copyright Dmitry 'MatrixS_Master' Soloviov, 2018
 */
#ifndef INC_OS_H_
#define INC_OS_H_

#define OS_FLOPPY_FILE "fd.img"
#define OS_FONT_COLOR 0x07E0
#define OS_OSD_COLOR 0xF81F
#define OS_FONT_MINCODE 33
#define OS_FONT_MAXCODE 126
#define OS_VK_TIMEOUT 800

typedef int (*os_callback_t)(void);

extern uint32_t OS_Last_Address;
extern uint8_t OS_VK_CurSym;

void OS_UpdateInput(int key);

void OS_PrintString(char* str);

uint8_t* OS_InitDisk(const char* name, uint32_t* len);

void OS(os_callback_t cb);

#endif /* INC_OS_H_ */
