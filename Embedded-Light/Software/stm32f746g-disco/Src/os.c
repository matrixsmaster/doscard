#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "VM86Wrapper.h"
#include "ff.h"
#include "os.h"
#include "usart.h"
#include "dma2d.h"
#include "usb_host.h"
#include "usbh_hid_keybd.h"

uint8_t* OS_Font_Array = NULL;
uint32_t OS_Font_Array_Size = 0, OS_Last_Address = 0;
static uint32_t clut[2];
extern USBH_HandleTypeDef hUsbHostFS;

int OS_InitFont()
{
	FIL fp;
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

	OS_Font_Array_Size = sym * 24;
	OS_Last_Address = (uint32_t)OS_Font_Array + OS_Font_Array_Size;

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

	return 0;
}

uint8_t* OS_InitDisk(const char* name, uint32_t* len)
{
	FIL fp;
	UINT rb;
	uint8_t* to = (uint8_t*)OS_Last_Address;
	uint8_t* ret = to;
	memset(&fp,0,sizeof(fp));

	if (f_open(&fp,name,FA_READ) != FR_OK) return NULL;

	*len = 0;
	while (f_read(&fp,to,16,&rb) == FR_OK && rb > 0) {
		to += rb;
		*len += rb;
	}
	f_close(&fp);

	char dbg[64];
	snprintf(dbg,sizeof(dbg),"%u bytes read\r\n",*len);
	HAL_UART_Transmit(&huart1,(uint8_t*)dbg,strlen(dbg),100);

	OS_Last_Address += *len;

	return ret;
}

void OS_DrawChar(int col, int row, char x)
{
	if (x < 33 || x > 126) return;

	int cx = col * 6;
	int cy = row * 9;

#if 1
	if (HAL_DMA2D_PollForTransfer(&hdma2d,100) == HAL_OK)
		HAL_GPIO_WritePin(ARDUINO_D12_GPIO_Port,ARDUINO_D12_Pin,1);
	else
		HAL_GPIO_WritePin(ARDUINO_D12_GPIO_Port,ARDUINO_D12_Pin,0);

	if (HAL_DMA2D_Start(&hdma2d,(uint32_t)OS_Font_Array+((x-33)*24),(uint32_t)SDRAM_BANK_ADDR+(cy*LCD_LINE_SIZE+cx)*3,6,8) == HAL_OK)
		HAL_GPIO_WritePin(ARDUINO_D11_GPIO_Port,ARDUINO_D11_Pin,1);
	else
		HAL_GPIO_WritePin(ARDUINO_D11_GPIO_Port,ARDUINO_D11_Pin,0);
#else
	uint8_t* to = (uint8_t*)SDRAM_BANK_ADDR;
	uint8_t* fr = OS_Font_Array+((x-33)*24);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			to[(cy*LCD_LINE_SIZE+cx)*3] = 0;
			to[(cy*LCD_LINE_SIZE+cx)*3+1] = ((fr[i*3+j/2] >> ((j & 1)*4)) & 1)? 0xFF:0;
			to[(cy*LCD_LINE_SIZE+cx)*3+2] = 0;
			cx++;
		}
		cx = col * 9;
		cy++;
	}
#endif
}

void OS_PrintString(char* str)
{
	int col = 0, row = 1;
	for (unsigned i = 0; i < strlen(str); i++) {
		OS_DrawChar(col,row,str[i]);
		if (++col >= 80) {
			col = 0;
			row++;
		}
	}
}

static void OS_KbdInput()
{
	MX_USB_HOST_Process();

	if (USBH_HID_GetDeviceType(&hUsbHostFS) == HID_KEYBOARD) {
		HID_KEYBD_Info_TypeDef *k_pinfo;
		k_pinfo = USBH_HID_GetKeybdInfo(&hUsbHostFS);
		if (!k_pinfo) return;

		char c = USBH_HID_GetASCIICode(k_pinfo);
		if (c) VM86_InsertKey(c);
	}
}

void OS()
{
	memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
	VM86_Start(OS_Last_Address);
	OS_PrintString("VM Init complete");

	for (int l = 1, p = 0, r = 0;;) {
		r = VM86_FullStep();
		if (r < 0) break;

		if (r > 0x01000000) {
			l = (r & 0x00FF0000) >> 16;
			p = (r & 0x0000FF00) >> 8;
			char bp = r & 0xFF;
			if (bp) OS_DrawChar(p,l,bp);

		} else if (r == 1) {
			memset(SDRAM_PTR,0,LCD_SCREEN_SIZE);
			char* bp = VM86_GetShadowBuf();
			for (l = 0; l < VM86W_GEOMH; l++)
				for (p = 0; p < VM86W_GEOMW; p++,bp++) {
					if (*bp) OS_DrawChar(p,l,*bp);
				}
		}

		OS_KbdInput();
	}

	VM86_Stop();
}
