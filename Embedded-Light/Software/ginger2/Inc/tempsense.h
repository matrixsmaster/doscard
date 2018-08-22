/*
 * tempsense.h
 *
 *  Created on: Aug 22, 2018
 *      Author: MatrixS_Master
 */

#ifndef INC_TEMPSENSE_H_
#define INC_TEMPSENSE_H_

#include <inttypes.h>

#define CPUTEMP_V25			0.76f
#define CPUTEMP_ASLOPE		(2.5f / 1e3)
#define ADC1_RESOLUTION		(3.3f / 4096.f)

typedef struct {
	int threshold;
	void* gpio_port;
	uint16_t gpio_pin;
	int blink_time;
} cputemp_rec_t;

int GetCPUTemp();
void ShowCPUTemp();

#endif /* INC_TEMPSENSE_H_ */
