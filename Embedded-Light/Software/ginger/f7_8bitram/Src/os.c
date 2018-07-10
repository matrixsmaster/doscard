#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "os.h"
#include "main.h"
#include "VM86Wrapper.h"
#include "pfont.h"
#include "VM86conf.h"
#include "fatfs.h"

void OS_DrawChar(int col, int row, char x)
{
	if (x < 33 || x > 126) return;
	send("char\r\n");
	__IO uint16_t* buf = (__IO uint16_t*)g_frames;
	int cx = (25-row)*8;
	int cy = col*4;
	for (int a,j,i = 0; i < 4; i++) {
		uint8_t sym = font8x4_rotated[(x-33)*4+i];
		for (j = 0; j < 8; j++) {
			if (sym & 0x80) {
				a = TFT_LCD_WIDTH*2*cy + cx*2;
				buf[a] = 0x0700;
				buf[++a] = 0xE000;
			}
			sym <<= 1;
			cx++;
		}
		cx = (25-row)*8;
		cy++;
	}
}

int OS_InitFloppy()
{
	FIL fp;
	UINT br;

	memset(&fp,0,sizeof(fp));
	if (f_open(&fp,OS_FLOPPY_IMAGE,FA_READ) != FR_OK) {
		send("Can't open floppy image file\r\n");
		return 0;
	}

	fd_img = (uint8_t*)(SDRAM_BANK_ADDR + TFT_TOTAL_BYTES + RAM_SIZE + IO_PORT_COUNT + VIDEO_RAM_SIZE);
	uint8_t* ptr = fd_img;
	while (f_read(&fp,ptr,16,&br) == FR_OK && br > 0) {
		ptr += br;
		fd_len += br;
	}
	f_close(&fp);

	char b[64];
	snprintf(b,sizeof(b),"%lu bytes loaded @ %p.\r\n",fd_len,fd_img);
	send(b);
	send("Floppy image loaded successfully.\r\n");
	return 1;
}

void OS()
{
	memset(g_frames,0,TFT_TOTAL_BYTES);

	char buf[160];
	buf[0] = 0;
	for (int i = 0; i < 16; i++)
		snprintf(buf,sizeof(buf),"%s 0x%02X",buf,fd_img[i]);
	strcat(buf,"\r\n");
	send(buf);

	VM86_Start(SDRAM_BANK_ADDR);
	send("VM Started\r\n");

	buf[0] = 0;
	for (int i = 0; i < 16; i++)
		snprintf(buf,sizeof(buf),"%s 0x%02X",buf,fd_img[i]);
	strcat(buf,"\r\n");
	send(buf);

	uint32_t cyc = 0;
	for (int l = 1, p = 0, r = 0;;) {
		r = VM86_FullStep();
		if (r < 0) break;

		if (cyc++ >= 65645) {
		while (!(g_btn_mask & BTN_LM_1)) ;
		while (g_btn_mask & BTN_LM_1) ;

		buf[0] = 0;
		for (int i = 0; i < 16; i++)
			snprintf(buf,sizeof(buf),"%s 0x%02X",buf,fd_img[i]);
		strcat(buf,"\r\n");
		send(buf);
		}

		if (r > 0x01000000) {
			l = (r & 0x00FF0000) >> 16;
			p = (r & 0x0000FF00) >> 8;
			char bp = r & 0xFF;
			//LCD_DrawFullRect((25-l)*8,p*4,8,4);
			if (bp) OS_DrawChar(p,l,bp);

		} else if (r == 1) {
			memset(g_frames,0,TFT_TOTAL_BYTES); //clear

			char* bp = VM86_GetShadowBuf();
			for (l = 0; l < VM86W_GEOMH; l++)
				for (p = 0; p < VM86W_GEOMW; p++,bp++) {
					if (*bp) OS_DrawChar(p,l,*bp);
				}
		}
	}

	VM86_Stop();
	send("VM stopped\r\n");
}
