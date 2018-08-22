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

extern uint32_t OS_Last_Address;

void OS_DrawLargeChar(char x);

void OS_PrintString(char* str);

uint8_t* OS_InitDisk(const char* name, uint32_t* len);

void OS();

#endif /* INC_OS_H_ */
