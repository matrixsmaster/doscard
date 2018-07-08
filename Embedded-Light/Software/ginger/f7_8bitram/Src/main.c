
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "dma2d.h"
#include "fatfs.h"
#include "rng.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile uint32_t g_seg_mask = 0;
volatile uint8_t g_seg_cnt = 0;
const struct SGPIOPair g_seg_leds[] = {
		{ Seg0_GPIO_Port, Seg0_Pin },
		{ Seg1_GPIO_Port, Seg1_Pin },
		{ Seg2_GPIO_Port, Seg2_Pin },
		{ Seg3_GPIO_Port, Seg3_Pin },
		{ Seg4_GPIO_Port, Seg4_Pin },
		{ Seg5_GPIO_Port, Seg5_Pin },
		{ Seg6_GPIO_Port, Seg6_Pin },
		{ Seg7_GPIO_Port, Seg7_Pin },
		{ Seg8_GPIO_Port, Seg8_Pin },
		{ Seg9_GPIO_Port, Seg9_Pin },
		{ Seg10_GPIO_Port, Seg10_Pin },
		{ Seg11_GPIO_Port, Seg11_Pin },
		{ Seg12_GPIO_Port, Seg12_Pin },
		{ Seg13_GPIO_Port, Seg13_Pin },
		{ Seg14_GPIO_Port, Seg14_Pin },
		{ Seg15_GPIO_Port, Seg15_Pin },
		{ Seg16_GPIO_Port, Seg16_Pin }
};
const uint32_t g_seg_font[] = {
		0b00000000011111111, //0
		0b00000000000001100, //1
		0b01000100001110111, //2
		0b01000100000111111, //3
		0b01000100010001100, //4
		0b01000100010111011, //5
		0b01000100011111011, //6
		0b00000000000001111, //7
		0b01000100011111111, //8
		0b01000100010111111, //9
		0 //terminator
};
volatile uint8_t* pixbuf;
volatile uint8_t pixlock = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void send(const char* s)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)s,strlen(s),10);
}

int SDRAM_InitSequence(SDRAM_HandleTypeDef *hsdram)
{
	FMC_SDRAM_CommandTypeDef cmd;
	uint32_t tmpr = 0;

	/* Step 3 --------------------------------------------------------------------*/
	/* Configure a clock configuration enable command */
	cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;
	while(HAL_IS_BIT_SET(hsdram->Instance->SDSR, FMC_SDSR_BUSY)) ;
	if (HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT) == HAL_OK) g_seg_mask = g_seg_font[3];
	else return 1;

	/* Step 4 --------------------------------------------------------------------*/
	/* Insert 100 ms delay */
	HAL_Delay(100);

	/* Step 5 --------------------------------------------------------------------*/
	/* Configure a PALL (precharge all) command */
	cmd.CommandMode = FMC_SDRAM_CMD_PALL;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;
	if (HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT) == HAL_OK) g_seg_mask = g_seg_font[5];
	else return 1;

	/* Step 6 --------------------------------------------------------------------*/
	/* Configure a Auto-Refresh command */
	cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 4;
	cmd.ModeRegisterDefinition = 0;
	if (HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT) == HAL_OK) g_seg_mask = g_seg_font[6];
	else return 1;
	if (HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT) == HAL_OK) g_seg_mask = g_seg_font[7];
	else return 1;

	/* Step 7 --------------------------------------------------------------------*/
	/* Program the external memory mode register */
	tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
			SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
			SDRAM_MODEREG_CAS_LATENCY_3           |
			SDRAM_MODEREG_OPERATING_MODE_STANDARD |
			SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* Configure a load Mode register command*/
	cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = tmpr;
	if (HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT) == HAL_OK) g_seg_mask = g_seg_font[8];
	else return 1;

	/* Step 8 --------------------------------------------------------------------*/
	/* Set the refresh rate counter */
	HAL_SDRAM_ProgramRefreshRate(hsdram,824); //7.81us * 108MHz - 20
	return 0;
}

static void TFT_Snd(uint16_t x)
{
	uint16_t data = x & 0xFF00; //High byte
	HAL_SRAM_Write_16b(&hsram1,(uint32_t*)TFT_LCD_ADDR,&data,1);
	data = x << 8; //Low byte
	HAL_SRAM_Write_16b(&hsram1,(uint32_t*)TFT_LCD_ADDR,&data,1);
}

static void TFT_Cmd(uint16_t cmd)
{
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port,TFT_RS_Pin,0);
	TFT_Snd(cmd);
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port,TFT_RS_Pin,1);
}

#define LCD_Write_COM(H,L) TFT_Cmd(((uint16_t)(H)<<8)|(uint16_t)(L))
#define LCD_Write_DATA(H,L) TFT_Snd(((uint16_t)(H)<<8)|(uint16_t)(L))
#define main_W_com_data(C,D) TFT_Cmd(C); TFT_Snd(D)

void TFT_Init()
{
	HAL_GPIO_WritePin(TFT_Reset_GPIO_Port,TFT_Reset_Pin,1);
	HAL_Delay(5);
	HAL_GPIO_WritePin(TFT_Reset_GPIO_Port,TFT_Reset_Pin,0);
	HAL_Delay(5);
	HAL_GPIO_WritePin(TFT_Reset_GPIO_Port,TFT_Reset_Pin,1);
	HAL_Delay(5);
//	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,0);
	//************* Start Initial Sequence **********//
	main_W_com_data(0x0001, 0x0100); // set SS and SM bit
	main_W_com_data(0x0002, 0x0200); // set 1 line inversion
	main_W_com_data(0x0003, 0x1030); // set GRAM write direction and BGR=1.
	main_W_com_data(0x0004, 0x0000); // Resize register
	main_W_com_data(0x0008, 0x0207); // set the back porch and front porch
	main_W_com_data(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	main_W_com_data(0x000A, 0x0000); // FMARK function
	main_W_com_data(0x000C, 0x0000); // RGB interface setting
	main_W_com_data(0x000D, 0x0000); // Frame marker Position
	main_W_com_data(0x000F, 0x0000); // RGB interface polarity
	//*************Power On sequence ****************//
	main_W_com_data(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	main_W_com_data(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	main_W_com_data(0x0012, 0x0000); // VREG1OUT voltage
	main_W_com_data(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	main_W_com_data(0x0007, 0x0001);
	HAL_Delay(200); // Dis-charge capacitor power voltage
	main_W_com_data(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
	main_W_com_data(0x0011, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
	HAL_Delay(50); // Delay 50ms
	main_W_com_data(0x0012, 0x000D); // 0012
	HAL_Delay(50); // Delay 50ms
	main_W_com_data(0x0013, 0x1200); // VDV[4:0] for VCOM amplitude
	main_W_com_data(0x0029, 0x000A); // 04  VCM[5:0] for VCOMH
	main_W_com_data(0x002B, 0x000D); // Set Frame Rate
	HAL_Delay(50); // Delay 50ms
	main_W_com_data(0x0020, 0x0000); // GRAM horizontal Address
	main_W_com_data(0x0021, 0x0000); // GRAM Vertical Address
	// ----------- Adjust the Gamma Curve ----------//
	main_W_com_data(0x0030, 0x0000);
	main_W_com_data(0x0031, 0x0404);
	main_W_com_data(0x0032, 0x0003);
	main_W_com_data(0x0035, 0x0405);
	main_W_com_data(0x0036, 0x0808);
	main_W_com_data(0x0037, 0x0407);
	main_W_com_data(0x0038, 0x0303);
	main_W_com_data(0x0039, 0x0707);
	main_W_com_data(0x003C, 0x0504);
	main_W_com_data(0x003D, 0x0808);
	//------------------ Set GRAM area ---------------//
	main_W_com_data(0x0050, 0x0000); // Horizontal GRAM Start Address
	main_W_com_data(0x0051, 0x00EF); // Horizontal GRAM End Address
	main_W_com_data(0x0052, 0x0000); // Vertical GRAM Start Address
	main_W_com_data(0x0053, 0x013F); // Vertical GRAM Start Address
	main_W_com_data(0x0060, 0xA700); // Gate Scan Line
	main_W_com_data(0x0061, 0x0001); // NDL,VLE, REV

	main_W_com_data(0x006A, 0x0000); // set scrolling line
	//-------------- Partial Display Control ---------//
	main_W_com_data(0x0080, 0x0000);
	main_W_com_data(0x0081, 0x0000);
	main_W_com_data(0x0082, 0x0000);
	main_W_com_data(0x0083, 0x0000);
	main_W_com_data(0x0084, 0x0000);
	main_W_com_data(0x0085, 0x0000);
	//-------------- Panel Control -------------------//
	main_W_com_data(0x0090, 0x0010);
	main_W_com_data(0x0092, 0x0000);
	main_W_com_data(0x0007, 0x0133); // 262K color and display ON
}

static void Address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_Write_COM(0x00,0x20);
	LCD_Write_DATA(x1>>8,x1);
	LCD_Write_COM(0x00,0x21);
	LCD_Write_DATA(y1>>8,y1);
	LCD_Write_COM(0x00,0x50);
	LCD_Write_DATA(x1>>8,x1);
	LCD_Write_COM(0x00,0x52);
	LCD_Write_DATA(y1>>8,y1);
	LCD_Write_COM(0x00,0x51);
	LCD_Write_DATA(x2>>8,x2);
	LCD_Write_COM(0x00,0x53);
	LCD_Write_DATA(y2>>8,y2);
	LCD_Write_COM(0x00,0x22);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RNG_Init();
  MX_FMC_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_DMA2D_Init();
  MX_SDMMC1_SD_Init();
  MX_USB_DEVICE_Init();
  MX_FATFS_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(SDRAM_DQM_GPIO_Port,SDRAM_DQM_Pin,1);
  HAL_GPIO_WritePin(Seg0_GPIO_Port,Seg0_Pin,1);
  HAL_GPIO_WritePin(Seg1_GPIO_Port,Seg1_Pin,1);
  HAL_GPIO_WritePin(Seg2_GPIO_Port,Seg2_Pin,1);
  HAL_GPIO_WritePin(Seg3_GPIO_Port,Seg3_Pin,1);
  HAL_GPIO_WritePin(Seg4_GPIO_Port,Seg4_Pin,1);
  HAL_GPIO_WritePin(Seg5_GPIO_Port,Seg5_Pin,1);
  HAL_GPIO_WritePin(Seg6_GPIO_Port,Seg6_Pin,1);
  HAL_GPIO_WritePin(Seg7_GPIO_Port,Seg7_Pin,1);
  HAL_GPIO_WritePin(Seg8_GPIO_Port,Seg8_Pin,1);
  HAL_GPIO_WritePin(Seg9_GPIO_Port,Seg9_Pin,1);
  HAL_GPIO_WritePin(Seg10_GPIO_Port,Seg10_Pin,1);
  HAL_GPIO_WritePin(Seg11_GPIO_Port,Seg11_Pin,1);
  HAL_GPIO_WritePin(Seg12_GPIO_Port,Seg12_Pin,1);
  HAL_GPIO_WritePin(Seg13_GPIO_Port,Seg13_Pin,1);
  HAL_GPIO_WritePin(Seg14_GPIO_Port,Seg14_Pin,1);
  HAL_GPIO_WritePin(Seg15_GPIO_Port,Seg15_Pin,1);
  HAL_GPIO_WritePin(Seg16_GPIO_Port,Seg16_Pin,1);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_GPIO_WritePin(TFT_RS_GPIO_Port,TFT_RS_Pin,1);

//#if USE_SDRAM
  HAL_GPIO_WritePin(SDRAM_DQM_GPIO_Port,SDRAM_DQM_Pin,0);
  if (SDRAM_InitSequence(&hsdram1)) {
	  const char _s[] = "Unable to init SDRAM :(\r\n";
	  HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),10);
	  Error_Handler();

  } else {
	  char buf[128];
	  uint8_t* tst = (uint8_t*)SDRAM_BANK_ADDR;
	  size_t sz = 8*1024*1024; //8 MiB
	  memset(tst,0,sz);
	  send("memory nullified (bank 0)\r\n");
//	  uint8_t* tst2 = (uint8_t*)0xC0800000;
//	  memset(tst2,0,sz);
//	  send("memory nullified (bank 1)\r\n");

#if 0
	  int16_t x = HAL_RNG_GetRandomNumber(&hrng);
	  snprintf(buf,sizeof(buf),"Value generated: %hd\r\n",x);
	  send(buf);

	  *(int16_t*)&tst[4] = x; //aligned write access
	  snprintf(buf,sizeof(buf),"[ALIGNED] Wrote %hd\r\n",x);
	  send(buf);

	  x = *(int16_t*)&tst[4]; //aligned read access
	  snprintf(buf,sizeof(buf),"[ALIGNED] x = %hd\r\n",x);
	  send(buf);

//	  x = HAL_RNG_GetRandomNumber(&hrng);
//	  snprintf(buf,sizeof(buf),"Next value generated: %hd\r\n",x);
//	  send(buf);

	  /*UNALIGNED DEREFERENCE IS STILL NOT WORKING ON F7 DEVICES,
	   * ALTHOUGH IT'S WORKING PERFECTLY ON THE F4 DEVICES.
	   */

	  *(int16_t*)&tst[3] = x; //unaligned write access
	  snprintf(buf,sizeof(buf),"[UNALIGNED] Wrote %hd\r\n",x);
	  send(buf);

	  x = *(int16_t*)&tst[3]; //unaligned read access
	  snprintf(buf,sizeof(buf),"[UNALIGNED] x = %hd\r\n",x);
	  send(buf);
#endif

	  for (size_t i = 0; i < sz; i++) tst[i] = (uint8_t)i;
	  send("wrote some data\r\nchecking...  ");
	  for (size_t i = 0; i < sz; i++) {
		  if (tst[i] != (uint8_t)i) {
			  snprintf(buf,sizeof(buf),"mismatch @ %u\r\n",i);
			  send(buf);
		  } /*else {
	  			  snprintf(buf,sizeof(buf),"%u OK\r\n",i);
	  			  send(buf);
	  		  }*/
	  }
	  send("OK\r\n");
  }

//#elif 0
  TFT_Init();
  Address_set(0,0,239,319);
  send("TFT LCD Initialization complete\r\n");
#if 1
  pixbuf = (uint8_t*)SDRAM_BANK_ADDR;
  memset(pixbuf,0,320*240*4);
//  for (int i = 0; i < 320*240; i++) {
//	  pixbuf[i*4+1] = gimp_image.pixel_data[i*2+1];
//	  pixbuf[i*4+3] = gimp_image.pixel_data[i*2];
//  }
//  HAL_TIM_Base_Start_IT(&htim6);
//  if (HAL_DMA2D_Start(&hdma2d,(uint32_t)pixbuf,TFT_LCD_ADDR,480,320) == HAL_OK) send("DMA2D start OK\r\n");
//  else {
//	  send("DMA2D start FAILED\r\n");
//	  Error_Handler();
//  }
//  if (HAL_DMA2D_PollForTransfer(&hdma2d,100) == HAL_OK) send("DMA2D transfer complete\r\n");
//  else send("Error polling DMA2D transfer status\r\n");
#else
  for (int i = 0; i < 320*240; i++)
	  TFT_Snd((((uint16_t)gimp_image.pixel_data[i*2+1])<<8) | gimp_image.pixel_data[i*2]);
#endif
//#endif

  memset(&SDFatFS,0,sizeof(SDFatFS));
  FRESULT r = f_mount(&SDFatFS,SDPath,1);
  {
	  char _s[] = "r = 0\r\n";
	  _s[4] += r;
	  send(_s);
	  g_seg_mask = g_seg_font[r & 0xF];
  }
  if (r != FR_OK) Error_Handler();
  send("SD filesystem mounted\r\n");

  for (int q = 0; q < 27; q++) {
	  FIL fp;
	  UINT br;
	  char fn[64];
	  volatile uint8_t* hold = pixbuf + q*(320*240*4);

	  memset(&fp,0,sizeof(fp));
	  snprintf(fn,sizeof(fn),"anim-%d.bmp",q);
	  send(fn);

	  if (f_open(&fp,fn,FA_READ) != FR_OK) {
		  const char _s[] = "Can't open file\r\n";
		  HAL_UART_Transmit(&huart1,(uint8_t*)_s,strlen(_s),100);
		  continue;
	  }

	  f_lseek(&fp,138);
//	  f_read(&fp,hold,320*240*4,&br);
	  uint8_t r,g,b;
	  uint16_t x, i = 0;
//	  pixlock = 1;
	  while (f_read(&fp,hold,4,&br) == FR_OK && br == 4) {
		  r = hold[2] >> 3;
		  g = hold[1] >> 2;
		  b = hold[0] >> 3;
		  x = ((uint16_t)r) << 11;
		  x |= ((uint16_t)g) << 5;
		  x |= b;
		  hold[0] = 0;
		  hold[1] = x >> 8;
		  hold[2] = 0;
		  hold[3] = x;
		  i++;
		  hold += 4;
	  }
//	  pixlock = 0;
	  f_close(&fp);
  }
  HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
#if 0
	  HAL_GPIO_TogglePin(Seg16_GPIO_Port,Seg16_Pin);
	  HAL_Delay(500);
#elif 0
	  for (int j = 0; j < 8; j++) {
		  for (int i = 0; i < 8; i++) {
			  g_seg_mask = (1<<i);
			  HAL_Delay(50);
		  }
		  g_seg_mask = 0;
	  }
	  for (int i = 0; i < 3; i++) {
		  g_seg_mask = ~g_seg_mask;
		  HAL_Delay(300);
	  }
	  g_seg_mask = 0;
	  HAL_Delay(1000);
#elif 0
	  for (int i = 0; i < 10; i++) {
		  g_seg_mask = g_seg_font[i];
		  HAL_Delay(600);
	  }
#else
	  for (int i = 0; i < 10; i++) {
		  g_seg_mask = g_seg_font[i];
		  if (++pixlock > 26) pixlock = 0;
		  HAL_Delay(100);
	  }
#endif
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	char buf[128];
	snprintf(buf,sizeof(buf),"Err at line %d in %s\r\n",line,file);
	send(buf);
//	HAL_GPIO_WritePin(Seg16_GPIO_Port,Seg16_Pin,0);
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
//	  g_seg_mask = (1<<16);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/