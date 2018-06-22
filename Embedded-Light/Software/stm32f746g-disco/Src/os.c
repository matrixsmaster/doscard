#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "VM86Wrapper.h"
#include "ff.h"
#include "os.h"
#include "usart.h"
#include "dma2d.h"

uint8_t* OS_Font_Array = NULL;
static uint32_t clut[2];

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

		for (int i = 0; i < 6; i++) {
			if (i % 2)
				OS_Font_Array[sym*24+fsm*3+i/2] |= (buf[i] == '1')? 0x10 : 0;
			else
				OS_Font_Array[sym*24+fsm*3+i/2]  = (buf[i] == '1')? 1 : 0;
		}

		if (++fsm >= 8) {
			fsm = 0;
			sym++;
		}
	}
	f_close(&fp);

	char dbg[64];
	snprintf(dbg,sizeof(dbg),"%d symbols read, FSM = %d\r\n",sym,fsm);
	HAL_UART_Transmit(&huart1,(uint8_t*)dbg,strlen(dbg),100);

	clut[0] = 0x00000000; //black
	clut[1] = 0x000000FF; //green

	DMA2D_CLUTCfgTypeDef cfg;
	cfg.CLUTColorMode = DMA2D_CCM_RGB888;
	cfg.Size = 2;
	cfg.pCLUT = clut;

	if (HAL_DMA2D_CLUTLoad(&hdma2d,cfg,1) != HAL_OK) {
		const char _s[] = "CLUT config failed\r\n";
		HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),100);
		return 3;

	} else {
		const char _s[] = "CLUT config passed\r\n";
		HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),100);
	}

	if (HAL_DMA2D_PollForTransfer(&hdma2d,10) == HAL_OK) {
		const char _s[] = "CLUT Load Poll finished\r\n";
		HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),100);
	}

	memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
#if 0
	int cx = 0, cy = 0;
	for (int i = 0; i < sym; i++) {
		if (HAL_DMA2D_Start(&hdma2d,
				(uint32_t)OS_Font_Array+(i*24),
				(uint32_t)SDRAM_BANK_ADDR+(cy*LCD_LINE_SIZE+cx)*3,
				6,8) == HAL_OK) HAL_GPIO_WritePin(ARDUINO_D11_GPIO_Port,ARDUINO_D11_Pin,1);
		else
			HAL_GPIO_WritePin(ARDUINO_D11_GPIO_Port,ARDUINO_D11_Pin,0);
		if (HAL_DMA2D_PollForTransfer(&hdma2d,1) == HAL_OK)
			HAL_GPIO_WritePin(ARDUINO_D12_GPIO_Port,ARDUINO_D12_Pin,1);
		else
			HAL_GPIO_WritePin(ARDUINO_D12_GPIO_Port,ARDUINO_D12_Pin,0);

		cx += 6;
		if (cx >= 480) {
			cx = 0;
			cy += 9;
		}
	}
#endif
//	HAL_GPIO_WritePin(ARDUINO_D13_GPIO_Port,ARDUINO_D13_Pin,1);

	return 0;
}

#if 1
void OS_DrawChar(int col, int row, char x)
{
	if (x < 33 || x > 126) return;
//	__IO uint16_t* buf = LCD_GetCurrentBuffer();
	int cx = col * 9;
	int cy = row * 6;
//	for (int i = 0; i < 4; i++) {
//		uint8_t sym = font8x4_rotated[(x-33)*4+i];
//		for (int j = 0; j < 8; j++) {
//			if (sym & 0x80)
//				*(__IO uint16_t*)(buf+(LCD_PIXEL_WIDTH*cy+cx)) = 0; //FIXME: black color
//			sym <<= 1;
//			cx++;
//		}

	if (HAL_DMA2D_PollForTransfer(&hdma2d,1) == HAL_OK)
		HAL_GPIO_WritePin(ARDUINO_D12_GPIO_Port,ARDUINO_D12_Pin,1);
	else
		HAL_GPIO_WritePin(ARDUINO_D12_GPIO_Port,ARDUINO_D12_Pin,0);

		if (HAL_DMA2D_Start(&hdma2d,(uint32_t)OS_Font_Array+((x-33)*24),(uint32_t)SDRAM_BANK_ADDR+(cy*LCD_LINE_SIZE+cx)*3,6,8) == HAL_OK)
			HAL_GPIO_WritePin(ARDUINO_D11_GPIO_Port,ARDUINO_D11_Pin,1);
		else
			HAL_GPIO_WritePin(ARDUINO_D11_GPIO_Port,ARDUINO_D11_Pin,0);


//		cx = row*9;
//		cy++;
//	}
}

void OS_PrintString(char* str)
{
	int col = 0, row = 0;
	for (unsigned i = 0; i < strlen(str); i++) {
		OS_DrawChar(col,row,str[i]);
		if (++col >= 80) {
			col = 0;
			row++;
		}
	}
}

void OS()
{
	memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
//	LCD_SetFont(&Font8x8);
//	LCD_DisplayStringLine(LCD_LINE_0,(uint8_t*)"Starting...");

	VM86_Start(SDRAM_BANK_ADDR);

	for (int l = 1, p = 0, r = 0;;) {
		HAL_GPIO_TogglePin(ARDUINO_D13_GPIO_Port,ARDUINO_D13_Pin);

		r = VM86_FullStep();
		if (r < 0) break;

		if (r > 0x01000000) {
			l = (r & 0x00FF0000) >> 16;
			p = (r & 0x0000FF00) >> 8;
			char bp = r & 0xFF;
			//LCD_DrawFullRect((25-l)*8,p*4,8,4);
			if (bp) OS_DrawChar(p,l,bp);

		} else if (r == 1) {
			memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
			char* bp = VM86_GetShadowBuf();
			for (l = 0; l < VM86W_GEOMH; l++)
				for (p = 0; p < VM86W_GEOMW; p++,bp++) {
					//if (*bp && *bp != ' ') LCD_DrawFullRect((25-l)*8,p*4,8,4);
					if (*bp) OS_DrawChar(p,l,*bp);
				}
		}
	}

	VM86_Stop();
	memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
}
#endif
