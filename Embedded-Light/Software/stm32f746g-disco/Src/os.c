#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "VM86Wrapper.h"
#include "ff.h"
#include "os.h"
#include "usart.h"

uint8_t* OS_Font_Array = NULL;

int OS_InitFont()
{
	FIL fp;
	UINT rb;
	char buf[16];
	int fsm = 0, sym = 0;

	if (!OS_Font_Array) return 1;

	memset(&fp,0,sizeof(fp));

	if (f_open(&fp,OS_FONT_FILE,FA_READ) != FR_OK) {
		const char _s[] = "Can't open font file\r\n";
		HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),100);
		return 2;

	} else {
		const char _s[] = "Font file opened\r\n";
		HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),100);
	}

	while (f_gets(buf,sizeof(buf),&fp)) {
		if (buf[0] != '0' && buf[0] != '1') continue;

		uint8_t x = 0;
		for (int i = 0; i < 5; i++) {
			if (buf[i] == '1') x |= 1 << (7-i);
		}

		OS_Font_Array[sym*8+fsm] = x;
		if (++fsm >= 8) {
			fsm = 0;
			sym++;
		}
	}
	f_close(&fp);

	char dbg[64];
	snprintf(dbg,sizeof(dbg),"%d symbols read, FSM = %d\r\n",sym,fsm);
	HAL_UART_Transmit(&huart1,(uint8_t*)dbg,strlen(dbg),100);

	//FIXME: debug only
	memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
	int cx = 0, cy = 0;
	for (int i = 0; i < sym; i++) {
		for (int j = 0; j < 8; j++) {
			uint8_t x = OS_Font_Array[i*8+j];
//			snprintf(dbg,sizeof(dbg),"x=%02hX\r\n",x);
//			HAL_UART_Transmit(&huart1,(uint8_t*)dbg,strlen(dbg),100);
			for (int k = 0; k < 6; k++) {
				if (x & 0x80) {
					memset(SDRAM_PTR+(((cy+j)*LCD_LINE_SIZE+cx+k)*3),0xFF,3); //white pixel
//					snprintf(dbg,sizeof(dbg),"set at %d\r\n",(((cy+j)*LCD_LINE_SIZE+cx+k)*3));
//					HAL_UART_Transmit(&huart1,(uint8_t*)dbg,strlen(dbg),100);
				}
				x <<= 1;
			}
		}
		cx += 6;
		if (cx >= 480) {
			cx = 0;
			cy += 9;
		}
	}
	for(;;);

	return 0;
}

#if 0
void OS_DrawChar(int col, int row, char x)
{
	if (x < 33 || x > 126) return;
	__IO uint16_t* buf = LCD_GetCurrentBuffer();
	int cx = (25-row)*8;
	int cy = col*4;
	for (int i = 0; i < 4; i++) {
		uint8_t sym = font8x4_rotated[(x-33)*4+i];
		for (int j = 0; j < 8; j++) {
			if (sym & 0x80)
				*(__IO uint16_t*)(buf+(LCD_PIXEL_WIDTH*cy+cx)) = 0; //FIXME: black color
			sym <<= 1;
			cx++;
		}
		cx = (25-row)*8;
		cy++;
	}
}

void OS()
{
	LCD_Clear(LCD_COLOR_GREEN);
//	LCD_SetFont(&Font8x8);
//	LCD_DisplayStringLine(LCD_LINE_0,(uint8_t*)"Starting...");

	VM86_Start(SDRAM_BANK_ADDR);

	for (int l = 1, p = 0, r = 0;;) {
		STM_EVAL_LEDToggle(LED3);

		r = VM86_FullStep();
		if (r < 0) break;

		if (r > 0x01000000) {
			l = (r & 0x00FF0000) >> 16;
			p = (r & 0x0000FF00) >> 8;
			char bp = r & 0xFF;
			//LCD_DrawFullRect((25-l)*8,p*4,8,4);
			if (bp) OS_DrawChar(p,l,bp);

		} else if (r == 1) {
			LCD_Clear(LCD_COLOR_GREEN);
			char* bp = VM86_GetShadowBuf();
			for (l = 0; l < VM86W_GEOMH; l++)
				for (p = 0; p < VM86W_GEOMW; p++,bp++) {
					//if (*bp && *bp != ' ') LCD_DrawFullRect((25-l)*8,p*4,8,4);
					if (*bp) OS_DrawChar(p,l,*bp);
				}
		}
	}

	VM86_Stop();
	LCD_Clear(LCD_COLOR_RED);
}
#endif
