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
* @file dma.c
* @brief This source file contains a c program to implement a DMA functions.
*
* @authors: Ismail Yesildirek, Bijan Kianian
* @date April 21 2019
* @version 1.0
*
*/

/***********************************
 * https://www.somacon.com/p125.php
 * For Masking logic
 **********************************/
#include "adc.h"
#include "dma.h"

/*******DMA Variables*******/
uint32_t buffer_part_3[BUFFER_SIZE_PART_3];
uint32_t buff_1_part_4[BUFFER_SIZE_PART_4];
uint32_t buff_2_part_4[BUFFER_SIZE_PART_4];

void DMA0_init(void)
{
	/************************************************************
	 * Enable DMA clock
	 * Disable DMA prior to modifying trigger or source settings
	 * Set source & destination address in 32 bits only
	 * Set the number of Bytes to transfer in BCR register to 256
	 * for part 3 or 128 for part 4
	 * Set to 1 = EINT, ERQ, AA, DINC
	 * Set to 0 = CS, SINC
	 * Set SS and DSIZE to 32 bit = 00
	 * Set START to 0?
	 * Set DCR register to 0xD0080000
	 * Set trigger bit and source bit
	 * and then enable DMA Channel 0
	 ***********************************************************/
	SIM->SCGC6 |= 0x02;
	SIM->SCGC7 |= 0x08;
	DMAMUX0->CHCFG[0] &= 0X0;

	DMA0->DMA[0].SAR = (uint32_t)&(ADC0->R[0]);
	DMA0->DMA[0].DAR = (uint32_t)&buffer_part_3;

	DMA0->DMA[0].DSR_BCR = 0x100;

	DMA0->DMA[0].DCR |= 0xD0080000;

	DMAMUX0->CHCFG[0] |= 0x40;
	DMAMUX0->CHCFG[0] |= 0x80;
}


