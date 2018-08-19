/*
 * sdram.c
 *
 *  Created on: Aug 19, 2018
 *      Author: MatrixS_Master
 */

#include "main.h"
#include "sdram.h"

int SDRAM_InitSequence(SDRAM_HandleTypeDef *hsdram)
{
	FMC_SDRAM_CommandTypeDef cmd;
	__IO uint32_t tmpmrd = 0;

#if SDRAM_INIT_METHOD == 1

	/* Step 3:  Configure a clock configuration enable command */
	cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

	/* Step 4: Insert 100 us minimum delay */
	/* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	HAL_Delay(1);

	/* Step 5: Configure a PALL (precharge all) command */
	cmd.CommandMode = FMC_SDRAM_CMD_PALL;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

	/* Step 6 : Configure a Auto-Refresh command */
	cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 8;
	cmd.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

	/* Step 7: Program the external memory mode register */
	tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 |
			SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
			SDRAM_MODEREG_CAS_LATENCY_2           |
			SDRAM_MODEREG_OPERATING_MODE_STANDARD |
			SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = tmpmrd;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

	/* Step 8 --------------------------------------------------------------------*/
	/* Set the refresh rate counter */
	HAL_SDRAM_ProgramRefreshRate(hsdram,824); //7.81us * 108MHz - 20
	//	hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1));

#elif SDRAM_INIT_METHOD == 2

	cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;
	if (HAL_SDRAM_SendCommand(hsdram,&cmd,SDRAM_TIMEOUT) != HAL_OK) return 1;
	HAL_Delay(1);

	cmd.CommandMode = FMC_SDRAM_CMD_PALL;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;
	if (HAL_SDRAM_SendCommand(hsdram,&cmd,SDRAM_TIMEOUT) != HAL_OK) return 2;
	// HAL_Delay(1);

	cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 8;
	cmd.ModeRegisterDefinition = 0;
	if (HAL_SDRAM_SendCommand(hsdram,&cmd,SDRAM_TIMEOUT) != HAL_OK) return 3;
	// HAL_Delay(1);

	tmpmrd=(uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_2 | SDRAM_MODEREG_OPERATING_MODE_STANDARD |
			SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
	cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = tmpmrd;
	if (HAL_SDRAM_SendCommand(hsdram,&cmd,SDRAM_TIMEOUT) != HAL_OK) return 4;
	// HAL_Delay(1);

	hsdram->Instance->SDRTR|=((uint32_t)((1292)<<1));

#else
#error "Invalid SDRAM init method ID"
#endif /* SDRAM_INIT_METHOD */

	return 0;
}

int SDRAM_Check()
{
	volatile uint8_t* ptr = (volatile uint8_t*)SDRAM_BANK_ADDR;
	for (size_t i = 0; i < 8*1024*1024; i++,ptr++) *ptr = i;
	HAL_Delay(100);
	ptr = (volatile uint8_t*)SDRAM_BANK_ADDR;
	for (size_t i = 0; i < 8*1024*1024; i++,ptr++) {
		if (*ptr != (uint8_t)i) return 1;
	}

	return 0;
}
