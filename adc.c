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
* @file adc.c
* @brief This source file contains a c program to implement a DMA functions.
*
* @authors: Ismail Yesildirek, Bijan Kianian
* @date April 27 2019
* @version 1.3
*
*/

/*==================================================================*/
#include "adc.h"
/*==================================================================*/

/*########################## ADC0_init() - Start ##########################*/

void ADC0_init(void)
 {

	/* Enable ADC0 clock*/
		SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;	// 0x8000000

	/* clock to PORTE */

		SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;	//0x2000

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
		ADC0->CFG2 = 0 ;

		/*******************************************
		 * PTE20 = analog input J10-1
		 * Use default Pin MUX set pins 10-8 to 0
		 * set pins 19 - 16 to 0 for IRQ/DMA disable
		 * pins 0,1 to 1 for pull up resistor
		 *******************************************/

		PORTE->PCR[20] = 0x3;

		/*********************************************************
		 * Set register bit 6 to 0 for software trigger and DMA
		 *********************************************************/

		ADC0->SC2 	&= ~ADC_SC2_ADTRG_MASK  ;			// Software triggered [0x40]
		ADC0->SC2	|=  ADC_SC2_DMAEN_MASK  ;			// DMA enable. [0x04]
		ADC0->SC3  	&= ~ADC_SC3_ADCO_MASK   ;			// Disable continuous conversion [0x08]
		ADC0->SC1[0] |= ADC_SC1_ADCH(31);				// Disable ADC [ADCH = 11111] until it's called for conversion.
 }

/*############################ ADC0_init() - End ############################*/

/*+++++++++++++++++++++++++++ ADC_Read () - Start +++++++++++++++++++++++++*/

uint32_t ADC_Read(void)
{
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK; 				//Write to SC1 to start conversion with Interrupt enabled [0x40]
	return ADC0->R[0];
}

/*+++++++++++++++++++++++++++ ADC_Read () - End +++++++++++++++++++++++++*/
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

 /*========================= gpio_config() - Start ========================*/
 void gpio_config(void)
 {
 	/****************************************************
 	 * D15 Settings for LED output
 	 * Enable Port E clock set bit 13 to 1
 	 * Enable PTE1, set bits 10-8 to 0b001 for alt 1
 	 * Set pin as Digital Output 0b01
 	 * Set pin to inverse (turn on LED)
 	 ***************************************************/
 	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;	//0x2000
 	PORTE->PCR[1] = 0x100;
 	PTE->PDDR |= 0x02;

 	/****************************************
 	 * Configuring Blue LED on board to toggle
 	 * sync with DMA interrupt assertions after
 	 * completion of each block transfer.
 	 ****************************************/

	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;	/* enable clock to Port D [0x1000]*/
	PORTD->PCR[1] = 0x100;				/* Port D pin 1 as GPIO. */
	PTD->PDDR |= 0x02;					/* Configure pin 1 as output pin */
	PTD->PDOR |= 0x02; 					/* Starting from 'OFF' state*/
 }

/*---------------------------------- led () -Start --------------------*/

 /* Toggling Blue LED */
 void led(void)
 {
	 PTD->PTOR |= 0x02;
 }
// /*========================= gpio_config() - End ========================*/
