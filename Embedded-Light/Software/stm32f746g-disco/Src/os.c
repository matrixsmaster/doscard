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

		/*uint8_t x = 0;
		for (int i = 0; i < 5; i++) {
			if (buf[i] == '1') x |= 1 << (7-i);
		}*/

		for (int i = 0; i < 6; i++) {
			if (i % 2) OS_Font_Array[sym*24+fsm*3+i/2] |= (buf[i] == '1')? 0x10 : 0;
			else OS_Font_Array[sym*24+fsm*3+i/2] = (buf[i] == '1')? 1 : 0;
		}

//		OS_Font_Array[sym*8+fsm] = x;
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

//	memset(clut,0,sizeof(clut));
//	for (uint32_t i = 1; i < 16; i++) clut[i] = 0xFF;
	clut[0] = 0x00000000;
	clut[1] = 0x000000FF;
//	clut[2] = 0xFFFFFF;

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

//	if (HAL_DMA2D_EnableCLUT(&hdma2d,1) != HAL_OK) {
//		const char _s[] = "CLUT enabling failed\r\n";
//		HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),100);
//		return 4;
//	}


	memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
	int cx = 0, cy = 0;
	for (int i = 0; i < sym; i++) {
#if 1
//		uint32_t* aa = OS_Font_Array+(i*24);
//		uint32_t* bb = SDRAM_PTR+(cy*LCD_LINE_SIZE+cx)*3;
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
#else
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 5; k++) {
				uint8_t x = 0;
				if (k % 2 == 0) x = OS_Font_Array[i*24+j*3+k/2] >> 4;
				else x = OS_Font_Array[i*24+j*3+k/2] & 0x0F;
//				snprintf(dbg,sizeof(dbg),"%d:%d + %d:%d  x = %02hX\r\n",cx,cy,k,j,x);
//				HAL_UART_Transmit(&huart1,(uint8_t*)dbg,strlen(dbg),100);
				SDRAM_PTR[((cy+j)*LCD_LINE_SIZE+cx+k)*3] = x? 0xFF : 0;
			}
		}
#endif
		cx += 6;
		if (cx >= 480) {
			cx = 0;
			cy += 9;
		}
	}
	HAL_GPIO_WritePin(ARDUINO_D13_GPIO_Port,ARDUINO_D13_Pin,1);
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
