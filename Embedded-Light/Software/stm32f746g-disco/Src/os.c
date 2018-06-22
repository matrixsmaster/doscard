#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "VM86Wrapper.h"
//#include "pfont.h"
#if 0
void OS_InitFont()
{}

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
