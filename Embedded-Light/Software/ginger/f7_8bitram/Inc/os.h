#ifndef INC_OS_H_
#define INC_OS_H_

#define OS_FLOPPY_IMAGE "fd.img"

extern uint8_t* fd_img;
extern uint32_t fd_len;

void OS_DrawChar(int col, int row, char x);
int OS_InitFloppy();
void OS();

#endif /* INC_OS_H_ */
