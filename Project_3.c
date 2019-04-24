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
* @version 1.2
*
*/


#include "dma.h"
#include "adc.h"

#define DOUBLE_BUFFER 0

/******************************************************************************
 * 			Variables used by DMA driver  fsl_dma driver ver 2.0.1
 ******************************************************************************/

dma_handle_t DMA_Handle;						// Structure variable includes DMA Address, call back function and parameter.
uint32_t ADC_Result = (uint32_t) (&ADC0->R[0]);	// Variable holding address of ADC result register "ADC0->R[0]"


/*******************************************************************************
 * 									main() - Start
 *******************************************************************************/

int main(void)
{

	BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    ADC0_init();
    gpio_config();

	dma_transfer_config_t transferConfig;
	uint16_t mVolts;
	uint32_t destAddr[DESTINATION_BUFF_LENGTH];
    uint32_t i = 0;

    DMA_Config(transferConfig, destAddr, &DMA_Handle);

    /******************************************************************************
     ************** Prepare DMA for transferring data from ADC to memory **********
     ************** 			using DMA driver functions				 **********
     ******************************************************************************/

    DMA_PrepareTransfer(&transferConfig, (void *)ADC_Result, sizeof(uint32_t),
    		(void *)destAddr, sizeof(uint32_t), sizeof(destAddr),
                            kDMA_PeripheralToMemory);


    /****************************************************
     * This loop is just for initial testing the outcome.
     * Will be modified for the rest of the project.
     * **************************************************/

    while(1)
    {
		DMA_SubmitTransfer(&DMA_Handle, &transferConfig, kDMA_NoOptions/*kDMA_EnableInterrupt*/);
		DMA_StartTransfer(&DMA_Handle);

		/******************************************************************************/


    	PRINTF("\r\n\n\r\nDMA ADC to memory transfer.\r\n\n");  /* Print destination buffer */

    	for (i = 0; i < DESTINATION_BUFF_LENGTH; i++)		/* Loop for DMA to transfer ADC result to destination buffer */
		{
			ADC_Read();
    		mVolts = ((destAddr[i]*3.3*1000)/RESOLUTION);	/* Analog Input in mV */
			printf(" %d - Voltage: %-4d mV\r\n", i, mVolts);
			delay(250);
		}

    }
}

/*******************************************************************************
 * 									main() - End
 *******************************************************************************/

