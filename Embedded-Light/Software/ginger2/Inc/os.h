#ifndef INC_OS_H_
#define INC_OS_H_

#define OS_FLOPPY_FILE "fd.img"

extern uint32_t OS_Last_Address;

void OS_PrintString(char* str);
uint8_t* OS_InitDisk(const char* name, uint32_t* len);
void OS();

#endif /* INC_OS_H_ */
