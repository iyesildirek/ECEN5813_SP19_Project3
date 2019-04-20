/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/*****************************************************************************
* Copyright (C) 2019 by Ismail Yesildirek & Bijan Kianian
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Ismail Yesildirek, Bijan Kianian, and the University of Colorado are not
* liable for any misuse of this material.
*
*****************************************************************************/
/**
* @file Project_2.c
* @brief This source file contains a c program perform ADC readings and utilizes 
* the board DMA to store the readings into a buffer.
*
* @authors: Ismail Yesildirek, Bijan Kianian
* @date April 20 2019
* @version 1.0
*
*/

#include "adc.h"

/* Number of steps in 16 bits */
#define RESOLUTION 65536

/* Select Project to Compile*/
#define PART2 1

int main(void)
{

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

	/* Local Variables */
    uint16_t ADC_in, mVolts;

	/* Configure ADC0 */
	ADC0_init();

	while (1)
	{
		/************************************
		 * Set AIEN to 1 to enable interrupt
		 * Set DIFF to 0b0 for single
		 * Set ADCH to 0b00000 for ADC_SE0
		 * Set register to 0x40
		 **********************************/
		ADC0->SC1[0] = 0x40;
		while(!(ADC0->SC1[0] & 0x80)) { }
		ADC_in = ADC0->R[0];

		/* Analog Input in V */
		mVolts = ((ADC_in*3.3*1000)/RESOLUTION);

		delay(250);
		printf("Voltage: %d mV\r", mVolts);
	}
}

    void ADC0_init(void)
    {
		/* clock to PORTE */
		SIM->SCGC5 |= 0x2000;

		/*******************************************
		 * PTE20 = analog input J10-1
		 * ENABLE PULL DOWN BY SETTING 0b10
		 * Use default Pin MUX set pins 10-8 to 0
		 * set pins 19 - 16 to 0b0011 for DMA enable
		 * set pin 24 to 1 to enable ISR/DMA flag
		 * Set to 0x1030000 for DMA settings
		 * or set to 0x3 for non-DMA
		 *******************************************/
#if PART2
		PORTE->PCR[20] = 0x3;

#else
		PORTE->PCR[20] = 0x1030000;
#endif
		/* Enable ADC0 clock*/
		SIM->SCGC6 |= 0x8000000;

		/***********************************************
		 * Set register bit 6 to 0 for software trigger
		 ***********************************************/
		ADC0->SC2 &= ~0x40;

		/*******************************************
		 * Set to 12 MHz ADC clock
		 * Set bit 7 to 0 for normal power
		 * Set bit 6-5 to 0b01 for 12MHz clock
		 * Set bit 4 to 0 for short sample
		 * Set bits 3-2 to 0b11 for 16bit resolution
		 * Set bits 1-0 to 0b01 for 24MHz clock
		 * Select 0x2D for the settings above
		 *******************************************/
		ADC0->CFG1 = 0x2D;

		/* Set default longest sample time */
		ADC0->CFG2 = 0 ;
    }

    /******************* delay () - Start *******************/

    void delay (uint16_t num)	/* Delay n times @ 12 MHZ clock*/
     {
     	for(uint16_t i =0; i < num ; i++)
     	{
     		for (uint16_t j = 0; j< 2000; j++)
     		{
     		}
     	}
     }
    /******************* delay () - End *********************/
