/*
 * sdram.h
 *
 *  Created on: Aug 19, 2018
 *      Author: MatrixS_Master
 */

#ifndef INC_SDRAM_H_
#define INC_SDRAM_H_

#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_sdram.h"

#define SDRAM_INIT_METHOD						 1

#define SDRAM_BANK_ADDR                 		 ((uint32_t)0xC0000000)
#define SDRAM_TIMEOUT							 ((uint32_t)0xFFFF)
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

int SDRAM_InitSequence(SDRAM_HandleTypeDef *hsdram);
int SDRAM_Check();

#endif /* INC_SDRAM_H_ */
