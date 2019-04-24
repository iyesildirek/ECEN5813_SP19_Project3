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
* @file project_3.c
* @brief This header file provides the adc prototypes and include files.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 23 2019
* @version 1.2
*
*/

#include "dma.h"
#include "adc.h"
#include "mem_display.h"

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
		DMA_SubmitTransfer(&DMA_Handle, &transferConfig, kDMA_NoOptions);
		DMA_StartTransfer(&DMA_Handle);

		/******************************************************************************/

    	printf("\r\n\n\r\nDMA ADC to memory transfer.\r\n\n");  /* Print destination buffer */
    	printf("\n Index 	  Address         Value(h)        Value(d)     voltage (mV)\n");
    	printf("\r -----	------------    -------------    ----------   -------------- \n");
    	for (i = 0; i < DESTINATION_BUFF_LENGTH; i++)		/* Loop for DMA transferring ADC
    														 * results to the destination buffer */
		{
			ADC_Read();

			delay(250);
			mVolts = ((destAddr[i]*3.3*1000)/RESOLUTION);	/* Analog Input in mV */
			mem_display((uint32_t*)destAddr,(uint32_t)DESTINATION_BUFF_LENGTH, mVolts, i);
		}
    }
}

/*******************************************************************************
 * 									main() - End
 *******************************************************************************/
