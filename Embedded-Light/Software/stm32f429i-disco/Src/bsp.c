/**
  ******************************************************************************
  * @file    bsp.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   bsp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TS_Pressed;
__IO uint32_t TS_Orientation = 0;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE            USB_OTG_Core __ALIGN_END;
__ALIGN_BEGIN USBH_HOST                      USB_Host __ALIGN_END;

/* Private function prototypes -----------------------------------------------*/
uint32_t BSP_TSC_Init(void);
void Alarm_Init(void);

/* Private functions ---------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
