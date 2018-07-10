/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stm32f746xx.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define TFT_Reset_Pin GPIO_PIN_2
#define TFT_Reset_GPIO_Port GPIOE
#define Seg0_Pin GPIO_PIN_0
#define Seg0_GPIO_Port GPIOA
#define Seg1_Pin GPIO_PIN_1
#define Seg1_GPIO_Port GPIOA
#define Seg2_Pin GPIO_PIN_2
#define Seg2_GPIO_Port GPIOA
#define Seg13_Pin GPIO_PIN_2
#define Seg13_GPIO_Port GPIOH
#define Seg14_Pin GPIO_PIN_3
#define Seg14_GPIO_Port GPIOH
#define Seg15_Pin GPIO_PIN_4
#define Seg15_GPIO_Port GPIOH
#define Seg16_Pin GPIO_PIN_5
#define Seg16_GPIO_Port GPIOH
#define Seg3_Pin GPIO_PIN_3
#define Seg3_GPIO_Port GPIOA
#define Seg4_Pin GPIO_PIN_4
#define Seg4_GPIO_Port GPIOA
#define Seg5_Pin GPIO_PIN_5
#define Seg5_GPIO_Port GPIOA
#define Seg6_Pin GPIO_PIN_6
#define Seg6_GPIO_Port GPIOA
#define Seg7_Pin GPIO_PIN_7
#define Seg7_GPIO_Port GPIOA
#define Seg11_Pin GPIO_PIN_4
#define Seg11_GPIO_Port GPIOC
#define Seg12_Pin GPIO_PIN_5
#define Seg12_GPIO_Port GPIOC
#define Seg8_Pin GPIO_PIN_0
#define Seg8_GPIO_Port GPIOB
#define Seg9_Pin GPIO_PIN_1
#define Seg9_GPIO_Port GPIOB
#define Seg10_Pin GPIO_PIN_2
#define Seg10_GPIO_Port GPIOB
#define TFT_RS_Pin GPIO_PIN_3
#define TFT_RS_GPIO_Port GPIOG
#define SDRAM_DQM_Pin GPIO_PIN_7
#define SDRAM_DQM_GPIO_Port GPIOG
#define KbdR0_Pin GPIO_PIN_10
#define KbdR0_GPIO_Port GPIOG
#define KbdR1_Pin GPIO_PIN_11
#define KbdR1_GPIO_Port GPIOG
#define KbdR2_Pin GPIO_PIN_12
#define KbdR2_GPIO_Port GPIOG
#define KbdR3_Pin GPIO_PIN_13
#define KbdR3_GPIO_Port GPIOG
#define KbdR4_Pin GPIO_PIN_14
#define KbdR4_GPIO_Port GPIOG
#define KbdT0_Pin GPIO_PIN_3
#define KbdT0_GPIO_Port GPIOK
#define KbdT1_Pin GPIO_PIN_4
#define KbdT1_GPIO_Port GPIOK
#define KbdT2_Pin GPIO_PIN_5
#define KbdT2_GPIO_Port GPIOK
#define KbdT3_Pin GPIO_PIN_6
#define KbdT3_GPIO_Port GPIOK

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#define SDRAM_BANK_ADDR                 		 ((uint32_t)0xC0000000)
#define SDRAM_TIMEOUT							 ((uint32_t)100)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

#define TFT_LCD_ADDR						     ((uint32_t)0x60000000)
#define TFT_LCD_WIDTH							 240
#define TFT_LCD_HEIGHT							 320
#define TFT_TOTAL_BYTES							 (TFT_LCD_WIDTH*TFT_LCD_HEIGHT*4)

#define BTN_LM_1	0x80000
#define BTN_LM_2	0x4000
#define BTN_LM_3	0x200
#define BTN_RM_1	0x8000
#define BTN_RM_2	0x400
#define BTN_RM_3	0x20
#define BTN_LJ_N	0x10
#define BTN_LJ_E	0x2000
#define BTN_LJ_S	0x100
#define BTN_LJ_W	0x40000
#define BTN_LJ_C	0x1
#define BTN_RJ_N	0x4
#define BTN_RJ_E	0x800
#define BTN_RJ_S	0x40
#define BTN_RJ_W	0x10000
#define BTN_RJ_C	0x2
#define BTN_Q_NW	0x8
#define BTN_Q_NE	0x1000
#define BTN_Q_SW	0x20000
#define BTN_Q_SE	0x80

struct SGPIOPair {
	  GPIO_TypeDef* port;
	  uint16_t pin;
};

struct SKeyboard {
	struct SGPIOPair trans[4];
	struct SGPIOPair recv[5];
};

void send(const char* s);

extern volatile uint32_t g_seg_mask;
extern volatile uint8_t g_seg_cnt;
extern const struct SGPIOPair g_seg_leds[];
extern volatile uint8_t* g_frames;
extern volatile uint8_t g_frame_cnt;
extern volatile uint32_t g_btn_mask;
extern volatile uint8_t g_btn_cnt;
extern const struct SKeyboard g_kbd_btns;
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
