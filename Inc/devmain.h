/*
 * devmain.h
 *
 *  Created on: Jun 30, 2014
 *      Author: master
 */

#ifndef DEVMAIN_H_
#define DEVMAIN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "usb_device.h"

/* Private variables ---------------------------------------------------------*/
extern RNG_HandleTypeDef hrng;
extern UART_HandleTypeDef huart1;
extern SDRAM_HandleTypeDef hsdram2;

#ifdef __cplusplus
extern "C" {
#endif

void devmain(void);

#ifdef __cplusplus
}
#endif

#endif /* DEVMAIN_H_ */
