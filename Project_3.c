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

#define RESOLUTION 50	// In uVolts

int main(void)
{

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

	uint16_t ADC_in, mVolts;

	ADC0_init(); /* Configure ADC0 */
	while (1)
	{
		ADC0->SC1[0] = 0; 								/* start conversion on channel 0 */
		while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)) { }  /* wait for conversion complete */
		ADC_in = ADC0->R[0]; 							/* read conversion result and clear COCO flag */

		mVolts = ADC_in * RESOLUTION / 1000 ; 			/* Digital output in mV */
		delay(250);
		printf("Voltage: %d\r", mVolts);

	}
}

    void ADC0_init(void)
    {
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 	/* clock to PORTB */
		PORTB->PCR[20] = 0;						/* PTE20 = analog input J10-1 */
		SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK ; 	/* clock to ADC0 */
		ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK; 		/* software trigger */

		/* clock divide by 4, long sample time, single ended 16 bit, bus clock */
		ADC0->CFG1 = ADC_CFG1_ADIV(4) | ADC_CFG1_ADLSMP_MASK  | ADC_CFG1_MODE_MASK | ~ADC_CFG1_ADICLK_MASK;
		ADC0->CFG2 = 0 ; 						/* Set default longest sample time */
    }

    /******************* delay () - Start *******************/

    void delay (uint16_t num)	/* Delay n milliseconds @ 20.97 MHZ clock*/
     {
     	for(uint16_t i =0; i < num ; i++)
     	{
     		for (uint16_t j = 0; j< 2000; j++)
     		{
     		}
     	}
     }
    /******************* delay () - End *********************/
