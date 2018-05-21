/*
 * os.c
 *
 *  Created on: May 19, 2018
 *      Author: lisa
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "VM86.h"

#if 0

#define IO_PORT_COUNT 0x10000
#define RAM_SIZE 0x10FFF0
#define RAM_SHIFT 0x200000

void OS()
{
	//__Delay(50);

    LCD_Clear(LCD_COLOR_GREEN);
    LCD_SetFont(&Font8x8);
    LCD_DisplayStringLine(LCD_LINE_0,(uint8_t*)"Testing this RAM module with a long line of text");

    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET) ;
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET) ;

    /*uint8_t* arr = (uint8_t*)malloc(1024*1024); //1MB
    if (arr) LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"Allocated 1MB...");
    else LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"Unable to allocate");*/

    __IO uint8_t* mem = (__IO uint8_t*)SDRAM_BANK_ADDR;
    mem += RAM_SHIFT;
    __IO uint8_t* io_ports = mem + RAM_SIZE;
    __IO uint8_t* other = io_ports + IO_PORT_COUNT;
    char buf[32];
    uint32_t result[3] = {0,0,0};

    //LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"Writing RAM...");

    for (uint32_t i = 0; i < RAM_SIZE; i++) mem[i] = (uint8_t)0xDE;

    //LCD_DisplayStringLine(LCD_LINE_2,(uint8_t*)"Writing IOP...");

    for (uint32_t i = 0; i < IO_PORT_COUNT; i++) io_ports[i] = (uint8_t)0x05;

    //LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"Writing ETC...");

    for (uint32_t i = 0; i < 100; i++) other[i] = (uint8_t)0x77;

    //LCD_DisplayStringLine(LCD_LINE_4,(uint8_t*)"Checking RAM...");

    for (uint32_t i = 0; i < RAM_SIZE; i++)
    	if (mem[i] != 0xDE) {
    		result[0] = i; break;
    		//snprintf(buf,sizeof(buf),"0x%04lX: %02X",i,mem[i]);
    		//LCD_DisplayStringLine(LCD_LINE_4,(uint8_t*)buf);
    	}

    //LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"Checking IOP...");

    for (uint32_t i = 0; i < IO_PORT_COUNT; i++)
    	if (io_ports[i] != 0x05) {
    		result[1] = i; break;
    		//snprintf(buf,sizeof(buf),"0x%04lX: %02X",i,io_ports[i]);
    		//LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)buf);
    	}

    //LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"Checking ETC...");

    for (uint32_t i = 0; i < 100; i++)
    	if (other[i] != 0x77) {
    		result[2] = i; break;
    		//snprintf(buf,sizeof(buf),"0x%04lX: %02X",i,other[i]);
    		//LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)buf);
    	}

    //LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"Check complete!");

    LCD_Clear(LCD_COLOR_GREEN);

    for (int i = 0; i < 3; i++) {
    	switch (i) {
    	case 0: snprintf(buf,sizeof(buf),"0x%04lX: %02X",result[i],mem[result[i]]); break;
    	case 1: snprintf(buf,sizeof(buf),"0x%04lX: %02X",result[i],io_ports[result[i]]); break;
    	case 2: snprintf(buf,sizeof(buf),"0x%04lX: %02X",result[i],other[result[i]]); break;
    	}
    	LCD_DisplayStringLine(LINE(i),(uint8_t*)buf);
    }

    int n = 0;
    while (1) {
    	//while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET) ;
    	//while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET) ;

    	for (uint32_t i = 0; i < RAM_SIZE; i++)
    		if (mem[i] != 0xDE) {
    			result[0] = i; break;
    		}

    	for (uint32_t i = 0; i < IO_PORT_COUNT; i++)
    		if (io_ports[i] != 0x05) {
    			result[1] = i; break;
    		}

    	for (uint32_t i = 0; i < 100; i++)
    		if (other[i] != 0x77) {
    			result[2] = i; break;
    		}

    	LCD_Clear(LCD_COLOR_GREEN);
    	snprintf(buf,sizeof(buf),"%i",n++);
    	LCD_DisplayStringLine(LCD_LINE_0,(uint8_t*)buf);

    	for (int i = 0; i < 3; i++) {
    		switch (i) {
    		case 0: snprintf(buf,sizeof(buf),"0x%04lX: %02X",result[i],mem[result[i]]); break;
    		case 1: snprintf(buf,sizeof(buf),"0x%04lX: %02X",result[i],io_ports[result[i]]); break;
    		case 2: snprintf(buf,sizeof(buf),"0x%04lX: %02X",result[i],other[result[i]]); break;
    		}
    		LCD_DisplayStringLine(LINE(i+1),(uint8_t*)buf);
    	}

    	//for (int i = 0; i < 240*2; i++) mem[n++] = 0;
    }
}

#else

void OS()
{
	LCD_Clear(LCD_COLOR_GREEN);
	LCD_SetFont(&Font8x8);
	LCD_DisplayStringLine(LCD_LINE_0,(uint8_t*)"Starting...");

	VM86_Start(SDRAM_BANK_ADDR);

	char* str;
	for (int l = 1, p = 0;;) {
		STM_EVAL_LEDToggle(LED3);

		str = VM86_FullStep();
		if (!str) continue;

		LCD_DisplayChar(l*8,p*8,str[0]);
		if (++p >= 30) {
			p = 0;
			if (++l >= 40) l = 0;
		}
		free(str);
	}
}

#endif
