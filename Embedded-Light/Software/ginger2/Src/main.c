
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
#include "adc.h"
#include "crc.h"
#include "dma2d.h"
#include "fatfs.h"
#include "jpeg.h"
#include "ltdc.h"
#include "rng.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include <math.h>
#include "usbd_cdc.h"
#include "os.h"
#include "VM86conf.h"
#include "tempsense.h"
#include "splash_screen.h"
#include "jpeg_utils.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
static uint32_t os_callback_cnt = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void send(const char* s)
{
#if USB_DEBUG_EN
	USBD_CDC_SetTxBuffer(&hUsbDeviceFS,(uint8_t*)s,strlen(s));
	while (USBD_CDC_TransmitPacket(&hUsbDeviceFS) != USBD_OK) HAL_Delay(100);
#else
	HAL_UART_Transmit(&huart1,(uint8_t*)s,strlen(s),10);
	HAL_Delay(30);
#endif
}

int os_callback_fun()
{
	if (os_callback_cnt++ % 700 == 0)
		ShowCPUTemp();
	return 0;
}

static uint8_t jpeg_test_in[4096];
static uint8_t jpeg_test_buf[768];
static uint32_t jpeg_in_feed = 0;//4096;
static uint32_t jpeg_last = 0;
static JPEG_ConfTypeDef jpeg_inf;
static JPEG_YCbCrToRGB_Convert_Function fun = NULL;
static uint8_t* jpeg_out;
//static int jpg_cx = 0, jpg_cy = 0;
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg,JPEG_ConfTypeDef *pInfo)
{
	char buf[80];
	snprintf(buf,sizeof(buf),"INFO : %hu %lu %lu\r\n",pInfo->ImageQuality,pInfo->ImageWidth,pInfo->ImageHeight);
	send(buf);
	jpeg_inf = *pInfo;
	uint32_t total = 0;
	JPEG_GetDecodeColorConvertFunc(pInfo,&fun,&total);
	snprintf(buf,sizeof(buf),"Total %lu blocks, function %p provided\r\n",total,fun);
	send(buf);
}
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
//	send("get data\r\n");
	char buf[80];
	snprintf(buf,sizeof(buf),"get data: %lu\r\n",NbDecodedData);
	send(buf);

	HAL_JPEG_Pause(hjpeg,JPEG_PAUSE_RESUME_INPUT);
	jpeg_in_feed += NbDecodedData; //this will finally 'decode' the image
	if (jpeg_in_feed >= new_logo_jpg_len) {
		HAL_JPEG_ConfigInputBuffer(hjpeg,jpeg_test_in,0);
		jpeg_last = 1;
		send("\r\nlast one\r\n");
	} else {
		uint32_t rem = new_logo_jpg_len - jpeg_in_feed;
		uint32_t sz = 4096;
		if (rem < sz) sz = rem;
		memcpy(jpeg_test_in,new_logo_jpg+jpeg_in_feed,sz);
		HAL_JPEG_ConfigInputBuffer(hjpeg,jpeg_test_in,sz);


		snprintf(buf,sizeof(buf),"feed = %lu\trem = %lu, sz = %lu\r\n",jpeg_in_feed,rem,sz);
//		jpeg_in_feed += sz;
		send(buf);
	}
	HAL_JPEG_Resume(hjpeg,JPEG_PAUSE_RESUME_INPUT);
}
static int jpeg_cnt = 0;
void HAL_JPEG_DataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
	char buf[80];
	snprintf(buf,sizeof(buf),"data ready: %lu\r\n",OutDataLength);
	send(buf);
//	for (int i = 0; i < 16; i++) {
//		memset(buf,0,sizeof(buf));
//		for (int j = 0; j < 16; j++) {
//			if (((uint32_t*)jpeg_test_buf)[i*16+j] != jpeg_last)
//				snprintf(buf,sizeof(buf),"%s 0x%08lX",buf,((uint32_t*)jpeg_test_buf)[i*16+j]);
//			jpeg_last = ((uint32_t*)jpeg_test_buf)[i*16+j];
//		}
//		if (buf[0]) send(buf);
//	}

	uint32_t fuck = 0;
	uint32_t r = fun(pDataOut,jpeg_out,jpeg_cnt,OutDataLength,&fuck); //I hate this fucking undocumented shit from STM!

	snprintf(buf,sizeof(buf),"conv: %lu, cnt = %i\r\n",fuck,jpeg_cnt);
	send(buf);
	jpeg_cnt += r;
//	jpeg_out += r*4;

	HAL_JPEG_Pause(hjpeg,JPEG_PAUSE_RESUME_OUTPUT);
	HAL_JPEG_ConfigOutputBuffer(hjpeg,jpeg_test_buf,sizeof(jpeg_test_buf));
	HAL_JPEG_Resume(hjpeg,JPEG_PAUSE_RESUME_OUTPUT);
}
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
	send("JPEG DECODE CPLT CALLBACK\r\n");
}
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	send("JPEG ERROR CALLBACK\r\n");
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

  /* MPU Configuration----------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache-------------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache-------------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_JPEG_Init();
  MX_LTDC_Init();
  MX_RNG_Init();
  MX_SDMMC1_SD_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  //First of all, let's wait for a bit longer than half a second to settle the board
  HAL_Delay(600);

#if USB_DEBUG_EN
//  while (HAL_GPIO_ReadPin(B4_GPIO_Port,B4_Pin) == GPIO_PIN_SET) ;
  send("Alive!\r\n");
#endif

  //The second thing is to init our SDRAM chip and check it for errors
  if (SDRAM_InitSequence(&hsdram1)) Error_Handler();
  HAL_Delay(500);
  if (SDRAM_Check()) Error_Handler();
  send("SDRAM OK\r\n");

  //Now we will mount the SD card
//  HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,1);
//  memset(&SDFatFS,0,sizeof(SDFatFS));
//  FRESULT r = f_mount(&SDFatFS,SDPath,1);
//  if (r != FR_OK) Error_Handler();
//  send("SD mounted\r\n");

  //Now enable the display unit
  HAL_GPIO_WritePin(LD1_GPIO_Port,LD1_Pin,1);
  video_init();
  send("TFT OK\r\n");

  //We have enabled the display unit, now we should set the RAM boundaries
  //for other subsystems (like VM and VK)
//  g_max_frames = 2; //the second "frame" is for OSD
  g_max_frames = 1; //FIXME: JPEG debug only
//  OS_Last_Address = SDRAM_BANK_ADDR + TFT_TOTAL_BYTES * g_max_frames;

  //FIXME: debug only
  if (HAL_JPEG_EnableHeaderParsing(&hjpeg) != HAL_OK) Error_Handler();
  memcpy(jpeg_test_in,new_logo_jpg,4096);
  jpeg_out = g_frames;
  if (HAL_JPEG_Decode(&hjpeg,jpeg_test_in,4096,jpeg_test_buf,sizeof(jpeg_test_buf),100000) != HAL_OK) {
	  char buf[123];
	  snprintf(buf,sizeof(buf),"JPEG Err %lu\r\n",HAL_JPEG_GetError(&hjpeg));
	  send(buf);
	  Error_Handler();
  }
  send("DECODED\r\n");
  for (;;);

  //Now let's initialize the virtual floppy image
  fd_img = OS_InitDisk(OS_FLOPPY_FILE,&fd_len);
  if (!fd_img) Error_Handler();

  //After small delay we will be ready to run the VM
  HAL_Delay(600);
  HAL_GPIO_WritePin(LD1_GPIO_Port,LD1_Pin,0);
  HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,0);

  //This will enable our buttons and VK
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  //And now we will run the VM
  OS(os_callback_fun);

  //If we're reached there, the VM has stopped.
  send("VM shutdown\r\n");
  memset((void*)g_frames,0,TFT_TOTAL_BYTES);
  OS_PrintString("Shutdown.");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
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

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_5);

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

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disables the MPU */
  HAL_MPU_Disable();
    /**Initializes and configures the Region and the memory to be protected 
    */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /**Initializes and configures the Region and the memory to be protected 
    */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x60000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	char buf[80];
	snprintf(buf,sizeof(buf),"Error at %d in %s\r\n",line,file);
	send(buf);
  while(1)
  {
	  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  HAL_Delay(200);
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
