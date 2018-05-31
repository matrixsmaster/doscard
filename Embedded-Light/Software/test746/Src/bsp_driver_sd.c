/**
 ******************************************************************************
  * @file    bsp_driver_sd.c for F7 (based on stm32756g_eval_sd.c)
  * @brief   This file includes a generic uSD card driver.
  ******************************************************************************
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

#define BUS_4BITS 1
/* USER CODE BEGIN 0 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_driver_sd.h"

/* Extern variables ---------------------------------------------------------*/ 
  
extern SD_HandleTypeDef _HSD;
extern HAL_SD_CardInfoTypedef _SD_CARD_INFO; 

/**
  * @brief  Initializes the SD card device.
  * @retval SD status
  */
uint8_t BSP_SD_Init(void)
{
  uint8_t sd_state = MSD_OK;
  /* Check if the SD card is plugged in the slot */
  if (BSP_SD_IsDetected() != SD_PRESENT)
  {
    return MSD_ERROR_SD_NOT_PRESENT;
  }
  /* HAL SD initialization */
  sd_state = HAL_SD_Init(&_HSD, &_SD_CARD_INFO);
#ifdef BUS_4BITS
  /* Configure SD Bus width */
  if (sd_state == MSD_OK)
  {
    /* Enable wide operation */
    if (HAL_SD_WideBusOperation_Config(&_HSD, SDMMC_BUS_WIDE_4B) != SD_OK)
    {
      sd_state = MSD_ERROR;
    }
    else
    {
      sd_state = MSD_OK;
    }
  }
#endif
  return sd_state;
}

/**
  * @brief  Configures Interrupt mode for SD detection pin.
  * @retval Returns 0 in success otherwise 1. 
  */
uint8_t BSP_SD_ITConfig(void)
{  
  /* TBI: add user code here depending on the hardware configuration used */
  
  return (uint8_t)0;
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t sd_state;
  if(HAL_SD_ReadBlocks(&_HSD, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)  
  {
    sd_state = MSD_ERROR;
  }
  else
  {
    sd_state = MSD_OK;
  }
  return sd_state;  
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t sd_state;
  if(HAL_SD_WriteBlocks(&_HSD, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)  
  {
    sd_state = MSD_ERROR;
  }
  else
  {
    sd_state = MSD_OK;
  }
  return sd_state;  
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t sd_state = MSD_OK;
  
  /* Read block(s) in DMA transfer mode */
  if(HAL_SD_ReadBlocks_DMA(&_HSD, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)  
  {
    sd_state = MSD_ERROR;
  }
  
  /* Wait until transfer is complete */
  if(sd_state == MSD_OK)
  {
    if(HAL_SD_CheckReadOperation(&_HSD, (uint32_t)SD_DATATIMEOUT) != SD_OK)  
    {
      sd_state = MSD_ERROR;
    }
    else
    {
      sd_state = MSD_OK;
    }
  }
  
  return sd_state; 
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.  
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write 
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t sd_state = MSD_OK;
  
  /* Write block(s) in DMA transfer mode */
  if(HAL_SD_WriteBlocks_DMA(&_HSD, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)  
  {
    sd_state = MSD_ERROR;
  }
  
  /* Wait until transfer is complete */
  if(sd_state == MSD_OK)
  {
    if(HAL_SD_CheckWriteOperation(&_HSD, (uint32_t)SD_DATATIMEOUT) != SD_OK)  
    {
      sd_state = MSD_ERROR;
    }
    else
    {
      sd_state = MSD_OK;
    }
  }
  
  return sd_state; 
}

/**
  * @brief  Erases the specified memory area of the given SD card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint64_t StartAddr, uint64_t EndAddr)
{
  uint8_t sd_state;
  if(HAL_SD_Erase(&_HSD, StartAddr, EndAddr) != SD_OK)  
  {
    sd_state = MSD_ERROR;
  }
  else
  {
    sd_state = MSD_OK;
  }
  return sd_state; 
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  *            @arg  SD_TRANSFER_ERROR: Data transfer error 
  */
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void)
{
  return(HAL_SD_GetStatus(&_HSD));
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef* CardInfo)
{
  /* Get SD card Information */
  HAL_SD_Get_CardInfo(&_HSD, CardInfo);
}
/* USER CODE END 0 */

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @param  None
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_SD_IsDetected(void)
{
  __IO uint8_t status = SD_PRESENT;

  /* USER CODE BEGIN 1 */
  /* user code can be inserted here */
  /* USER CODE END 1 */    	

  return status;
}

/* USER CODE BEGIN AdditionalCode */
/* user code can be inserted here */
/* USER CODE END AdditionalCode */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
