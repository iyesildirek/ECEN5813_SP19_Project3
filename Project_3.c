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
* @file Project_3.c
* @brief This source file contains a c program perform ADC readings and utilizes 
* the board DMA to store the readings into a buffer.
*
* @authors: Ismail Yesildirek, Bijan Kianian
* @date April 21 2019
* @version 1.1
*
*/

#include "adc.h"
#include "dma.h"

/* Select Project to Compile*/
#define PART2 0
#define PART3 1
#define PART4 1
#define PART5 1

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

	/* Configure D15*/
	gpio_config();

#if PART3
	DMA0_init();
#else

#endif

	while (1)
	{
		/************************************
		 * Set AIEN to 1 to enable interrupt
		 * Set DIFF to 0b0 for single
		 * Set ADCH to 0b00000 for ADC_SE0
		 * Set register to 0x40
		 **********************************/
		ADC0->SC1[0] = 0x40;
#if PART2
		while(!(ADC0->SC1[0] & 0x80))
		{
			/* Toggle PTE1 LED */
	    	PTE->PTOR |= 0x02;
		}
		/* Write to register */
		ADC_in = ADC0->R[0];

		/* Analog Input in V */
		mVolts = ((ADC_in*3.3*1000)/RESOLUTION);

		delay(250);
		printf("Voltage: %d mV\r", mVolts);

#else
		DMAMUX0->CHCFG[0] &= 0X80; //disable DMA
		/*Start DMA */
		DMA0->DMA[0].DCR |= (1<<16);
		DMAMUX0->CHCFG[0] |= 0x80; //enable DMA

		while(!(DMA0->DMA[0].DSR_BCR & 0x1000000))
		{
		}

		/* Analog Input in V */
		mVolts = ((ADC0->R[0]*3.3*1000)/RESOLUTION);

		delay(250);
		printf("Voltage: %d mV\r", mVolts);
		DMAMUX0->CHCFG[0] &= 0X80; //disable DMA
		DMA0->DMA[0].DSR_BCR &= (1<<16);
		DMAMUX0->CHCFG[0] |= 0x80; //enable DMA
#endif
		}

}

    void ADC0_init(void)
    {
		/* clock to PORTE */
		SIM->SCGC5 |= 0x2000;

		/*******************************************
		 * PTE20 = analog input J10-1
		 * Use default Pin MUX set pins 10-8 to 0
		 * set pins 19 - 16 to 0b0011 for DMA enable
		 * set pin 24 to 1 to enable ISR/DMA flag
		 * Set to 0x1030000 for DMA settings
		 * or set to 0x0 for non-DMA
		 *******************************************/
#if PART2
		PORTE->PCR[20] = 0x0;
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
		 * 1.5 MHz ADC clock Settings
		 * Bus clock is 24MHz per manual table 5.1
		 * Set bit 7 to 0 for normal power
		 * Set bit 6-5 to 0b11 to divide clock by 8
		 * Set bit 4 to 0 for short sample
		 * Set bits 3-2 to 0b11 for 16bit resolution
		 * Set bits 1-0 to 0b01 to divide 24MHz clock by 2
		 * Select 0x6D for the settings above
		 *******************************************/
		ADC0->CFG1 = 0x6D;

		/* Set default longest sample time */
		ADC0->CFG2 = 0 ;
    }

    /******************* delay () - Start *******************/

    void delay (uint16_t num)	/* Delay n times @ 24 MHZ clock*/
     {
     	for(uint16_t i =0; i < num ; i++)
     	{
     		for (uint16_t j = 0; j< 2000; j++)
     		{
     		}
     	}
     }
    /******************* delay () - End *********************/

    void gpio_config(void)
    {
    	/****************************************************
    	 * D15 Settings for LED output
    	 * Enable Port E clock set bit 13 to 1
    	 * Enable PTE1, set bits 10-8 to 0b001 for alt 1
    	 * Set pin as Digital Output 0b01
    	 * Set pin to inverse (turn on LED)
    	 ***************************************************/
    	SIM->SCGC5 |= 0x2000;
    	PORTE->PCR[1] = 0x100;
    	PTE->PDDR |= 0x02;
    }
