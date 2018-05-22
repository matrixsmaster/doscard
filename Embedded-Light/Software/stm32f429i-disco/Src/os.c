#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "VM86Wrapper.h"

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
			LCD_DrawFullRect((25-l)*8,p*4,8,4);

		} else if (r == 1) {
			LCD_Clear(LCD_COLOR_GREEN);
			char* bp = VM86_GetShadowBuf();
			for (l = 0; l < VM86W_GEOMH; l++)
				for (p = 0; p < VM86W_GEOMW; p++,bp++) {
					if (*bp && *bp != ' ') LCD_DrawFullRect((25-l)*8,p*4,8,4);
				}
		}
	}

	VM86_Stop();
	LCD_Clear(LCD_COLOR_RED);
}
