/*
 * tempsense.c
 *
 *  Created on: Aug 22, 2018
 *      Author: MatrixS_Master
 */

#include <math.h>
#include "gpio.h"
#include "adc.h"
#include "tempsense.h"
#include "main.h"

static const cputemp_rec_t cputemp_table[] = {
		{  0, LD1_GPIO_Port, LD1_Pin, -1 },
		{  0, LD2_GPIO_Port, LD2_Pin, -1 },
		{ 40, LD2_GPIO_Port, LD2_Pin, 300 },
		{ 50, LD2_GPIO_Port, LD2_Pin, 100 },
		{ 55, LD2_GPIO_Port, LD2_Pin, 0 },
		{ 60, LD1_GPIO_Port, LD1_Pin, 300 },
		{ 65, LD1_GPIO_Port, LD1_Pin, 100 },
		{ 70, LD1_GPIO_Port, LD1_Pin, 0 },
		{ -1, LD1_GPIO_Port, LD1_Pin, 0 } /* terminator */
};

int GetCPUTemp()
{
	int t = 0;
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1,1) == HAL_OK) {
		t = HAL_ADC_GetValue(&hadc1);
		t = round( ((ADC1_RESOLUTION*(float)t) - CPUTEMP_V25) / CPUTEMP_ASLOPE ) + 25;
	}
	HAL_ADC_Stop(&hadc1);

#if USB_DEBUG_EN
	char str[128];
	snprintf(str,sizeof(str),"CPU: %d C\r\n",t);
	send(str);
#endif

	return t;
}

void ShowCPUTemp()
{
	int temp = GetCPUTemp();
	uint32_t time = HAL_GetTick();

	for (int i = 0; cputemp_table[i].threshold >= 0; i++) {
		if (temp >= cputemp_table[i].threshold) {
			if (cputemp_table[i].blink_time < 0)
				HAL_GPIO_WritePin(cputemp_table[i].gpio_port,cputemp_table[i].gpio_pin,0);
			else if (cputemp_table[i].blink_time == 0)
				HAL_GPIO_WritePin(cputemp_table[i].gpio_port,cputemp_table[i].gpio_pin,1);
			else if (time % cputemp_table[i].blink_time == 0)
				HAL_GPIO_TogglePin(cputemp_table[i].gpio_port,cputemp_table[i].gpio_pin);
		}
	}
}
