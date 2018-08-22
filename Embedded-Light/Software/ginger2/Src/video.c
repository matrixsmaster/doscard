/*
 * video.c
 *
 *  Created on: Aug 19, 2018
 *      Author: MatrixS_Master
 */

#include <string.h>
#include "tim.h"
#include "fmc.h"
#include "main.h"
#include "video.h"
#include "robot.h"

volatile uint16_t* g_frames;
volatile uint8_t g_max_frames = 0;
volatile uint8_t g_frame_cnt = 0;

static void TFT_Snd(uint16_t x)
{
	HAL_Delay(5);
#if 0
	uint16_t data = x & 0xFF00; //High byte
	HAL_SRAM_Write_16b(&hsram1,(uint32_t*)TFT_LCD_ADDR,&data,1);
	data = x << 8; //Low byte
	HAL_Delay(5);
	HAL_SRAM_Write_16b(&hsram1,(uint32_t*)TFT_LCD_ADDR,&data,1);
#else
	HAL_SRAM_Write_16b(&hsram1,(uint32_t*)TFT_LCD_ADDR,&x,1);
#endif
}

static void TFT_Cmd(uint16_t cmd)
{
	HAL_Delay(1);
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port,TFT_RS_Pin,0);
	TFT_Snd(cmd);
	HAL_Delay(1);
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port,TFT_RS_Pin,1);
}

#define LCD_Write_COM(H,L) TFT_Cmd(((uint16_t)(H)<<8)|(uint16_t)(L))
#define LCD_Write_DATA(H,L) TFT_Snd(((uint16_t)(H)<<8)|(uint16_t)(L))
#define main_W_com_data(C,D) TFT_Cmd(C); TFT_Snd(D)
#define LCD_WriteCommand(C) TFT_Cmd(C)
#define LCD_WriteData(D) TFT_Snd(D)

static void TFT_Init()
{
	HAL_GPIO_WritePin(LCD1_RESET_GPIO_Port,LCD1_RESET_Pin,1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD1_RESET_GPIO_Port,LCD1_RESET_Pin,0);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD1_RESET_GPIO_Port,LCD1_RESET_Pin,1);
	HAL_Delay(120);
	//************* Start Initial Sequence **********//
	LCD_WriteCommand(0x11);    //Exit Sleep
	HAL_Delay(120);
	//************* Start Initial Sequence **********//
	LCD_WriteCommand(0xCF);
	LCD_WriteData(0x00);
	LCD_WriteData(0x83);
	LCD_WriteData(0X30);

	LCD_WriteCommand(0xED);
	LCD_WriteData(0x64);
	LCD_WriteData(0x03);
	LCD_WriteData(0X12);
	LCD_WriteData(0X81);

	LCD_WriteCommand(0xE8);
	LCD_WriteData(0x85);
	LCD_WriteData(0x00);
	LCD_WriteData(0x78);

	LCD_WriteCommand(0xCB);
	LCD_WriteData(0x39);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x00);
	LCD_WriteData(0x34);
	LCD_WriteData(0x02);

	LCD_WriteCommand(0xF7);
	LCD_WriteData(0x20);

	LCD_WriteCommand(0xEA);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	LCD_WriteCommand(0xC0);    //Power control
	LCD_WriteData(0x19);   //VRH[5:0]

	LCD_WriteCommand(0xC1);    //Power control
	LCD_WriteData(0x11);   //SAP[2:0];BT[3:0]

	LCD_WriteCommand(0xC5);    //VCM control
	LCD_WriteData(0x3C);
	LCD_WriteData(0x3F);

	LCD_WriteCommand(0xC7);    //VCM control2
	LCD_WriteData(0X90);

	LCD_WriteCommand(0x36);    // Memory Access Control
	LCD_WriteData(0x28);

	LCD_WriteCommand(0x3A);
	LCD_WriteData(0x55);   // 16Bit

	LCD_WriteCommand(0xB1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x17);

	LCD_WriteCommand(0xB6);    // Display Function Control
	LCD_WriteData(0x0A);
	LCD_WriteData(0xA2);

	LCD_WriteCommand(0xF6);
	LCD_WriteData(0x01);
	LCD_WriteData(0x30);

	LCD_WriteCommand(0xF2);    // 3Gamma Function Disable
	LCD_WriteData(0x00);

	LCD_WriteCommand(0x26);    //Gamma curve selected
	LCD_WriteData(0x01);

	LCD_WriteCommand(0xE0);    //Set Gamma
	LCD_WriteData(0x0F);
	LCD_WriteData(0x26);
	LCD_WriteData(0x22);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x10);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x4C);
	LCD_WriteData(0XCA);
	LCD_WriteData(0x36);
	LCD_WriteData(0x00);
	LCD_WriteData(0x15);
	LCD_WriteData(0x00);
	LCD_WriteData(0x10);
	LCD_WriteData(0x10);
	LCD_WriteData(0x00);

	LCD_WriteCommand(0XE1);    //Set Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x19);
	LCD_WriteData(0x1B);
	LCD_WriteData(0x05);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x05);
	LCD_WriteData(0x33);
	LCD_WriteData(0x35);
	LCD_WriteData(0x49);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x1F);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x3F);
	LCD_WriteData(0x3F);
	LCD_WriteData(0x0F);

	LCD_WriteCommand(0x11);    //Exit Sleep
	HAL_Delay(120);

	LCD_WriteCommand(0x29);    //Display on
}

static void Address_set(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
	LCD_WriteCommand(0x2A);
	LCD_WriteData((unsigned char)((x0>>8)&0xff));
	LCD_WriteData((unsigned char)(x0&0xff));
	LCD_WriteData((unsigned char)((x1>>8)&0xff));
	LCD_WriteData((unsigned char)(x1&0xff));

	LCD_WriteCommand(0x2B);
	LCD_WriteData((unsigned char)((y0>>8)&0xff));
	LCD_WriteData((unsigned char)(y0&0xff));
	LCD_WriteData((unsigned char)((y1>>8)&0xff));
	LCD_WriteData((unsigned char)(y1&0xff));

	LCD_WriteCommand(0x2C);
}

#if 0
/* This function should init the secondary display connected to LTDC interface.
 * TODO: write I2C initialisation procedure for DT022CTFT
 */
static void LCD_Init()
{
	HAL_GPIO_WritePin(LCD2_RESET_GPIO_Port,LCD2_RESET_Pin,1);
	HAL_Delay(5);
	HAL_GPIO_WritePin(LCD2_RESET_GPIO_Port,LCD2_RESET_Pin,0);
	HAL_Delay(5);
	HAL_GPIO_WritePin(LCD2_RESET_GPIO_Port,LCD2_RESET_Pin,1);
	HAL_Delay(5);
}
#endif

void video_init()
{
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	TFT_Init();
	Address_set(0,0,TFT_LCD_WIDTH-1,TFT_LCD_HEIGHT-1);
	memcpy((uint32_t*)TFT_LCD_ADDR,gimp_image.pixel_data,gimp_image.width*gimp_image.height*gimp_image.bytes_per_pixel);

	HAL_Delay(500);

	g_frames = (volatile uint16_t*)SDRAM_BANK_ADDR;
	memset((void*)g_frames,0,TFT_TOTAL_BYTES);
	HAL_TIM_Base_Start_IT(&htim7);
}

#if 0
void video_load()
{
	const int max_frames = 60;
	for (int q = 0; q < max_frames; q++) {
		FIL fp;
		UINT br;
		char fn[64];
		volatile uint16_t* hold = (volatile uint16_t*)&g_frames[q*(320*240)];

		memset(&fp,0,sizeof(fp));
		snprintf(fn,sizeof(fn),"anim-%d.bmp",q);
		send(fn);

		if (f_open(&fp,fn,FA_READ) != FR_OK) {
			send("Can't open file\r\n");
			send(fn);
			continue;
		}
		send("... ");

		f_lseek(&fp,138);
		uint8_t r,g,b;
		uint16_t x;
		uint8_t val[4];
		int i = 0;
		while (f_read(&fp,val,4,&br) == FR_OK && br == 4) {
			r = val[2] >> 3;
			g = val[1] >> 2;
			b = val[0] >> 3;
			x = ((uint16_t)r) << 11;
			x |= ((uint16_t)g) << 5;
			x |= b;
			hold[(i%240)*320+(i/240)] = x;
			i++;
		}
		f_close(&fp);
		send("OK\r\n");

		HAL_GPIO_TogglePin(LD1_GPIO_Port,LD1_Pin);
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	}
}

void video_player()
{
	if (blink) HAL_GPIO_TogglePin(LD1_GPIO_Port,LD1_Pin);

	int nxt = (int)g_frame_cnt + dir;
	if (nxt >= max_frames) nxt = 0;
	if (nxt < 0) nxt = max_frames - 1;
	g_frame_cnt = nxt;

	HAL_Delay(per);

	if (HAL_GPIO_ReadPin(B1_GPIO_Port,B1_Pin) == GPIO_PIN_RESET)
		per += 10;
	else if (HAL_GPIO_ReadPin(B2_GPIO_Port,B2_Pin) == GPIO_PIN_RESET)
		per -= 10;
	else if (HAL_GPIO_ReadPin(B3_GPIO_Port,B3_Pin) == GPIO_PIN_RESET)
		blink ^= 1;
	else if (HAL_GPIO_ReadPin(B4_GPIO_Port,B4_Pin) == GPIO_PIN_RESET)
		dir = (dir > 0)? -1:1;
}
#endif
