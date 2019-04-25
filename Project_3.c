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
* @date April 25 2019
* @version 1.3
*
*/

#include "dma.h"
#include "adc.h"
#include "mem_display.h"

/******************************************************************************
 * 			Variables used by DMA driver  fsl_dma driver ver 2.0.1
 ******************************************************************************/

dma_handle_t DMA_Handle;						// Structure variable includes DMA Address, call back function and parameter.
uint32_t ADC_Result = (uint32_t) (&ADC0->R[0]);	// Variable holding address of ADC result register "ADC0->R[0]"
dma_transfer_config_t transferConfig;

uint32_t *p_destAddress = NULL;					// Pointer used to swap buffers within DMA ISR.

uint32_t* Buffer_1 ;
uint32_t* Buffer_2 ;


bool Swap_Buffers = false;
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

    uint32_t mVolts;
	uint32_t numOfBytes = 4 * DESTINATION_BUFF_LENGTH;		// 4 bytes per transfer (x32 bits)
    uint32_t i = 0;

    Buffer_1 = (uint32_t*)calloc(DESTINATION_BUFF_LENGTH , sizeof(uint32_t));

#if DOUBLE_BUFFER

    Buffer_2 = (uint32_t*)calloc(DESTINATION_BUFF_LENGTH , sizeof(uint32_t));

#endif

    DMA_Config(&DMA_Handle, transferConfig);

    /******************************************************************************
     ************** Prepare DMA for transferring data from ADC to memory **********
     ************** 			using DMA driver functions				 **********
     ******************************************************************************/

#if DOUBLE_BUFFER

    DMA_SetTransferConfig(DMA0, DMA_CHANNEL, &transferConfig);
    DMA_EnableInterrupts(DMA0, DMA_CHANNEL);
    DMA_HandleIRQ(&DMA_Handle);
    DMA0_IRQHandler(transferConfig);
    NVIC_EnableIRQ(DMA0_IRQn);

#else

    p_destAddress = Buffer_1;

#endif

    /****************************************************
     * This loop is just for initial testing the outcome.
     * Will be modified for the rest of the project.
     * **************************************************/

    while(1)
    {

    	DMA_PrepareTransfer(&transferConfig, (void *)ADC_Result, sizeof(uint32_t),
    	    		p_destAddress, sizeof(uint32_t), numOfBytes,
    	                            kDMA_PeripheralToMemory);


    	DMA_SubmitTransfer(&DMA_Handle, &transferConfig, kDMA_EnableInterrupt);
		DMA_StartTransfer(&DMA_Handle);

		/************************************
		 ******* Report preparation *********
		 ************************************/

		printf("\n\r\t\t########################################\n");
    	printf("\r\t\t#    ADC to memory transfer via DMA.   #\n");  /* Print destination buffer */
    	printf("\r\t\t########################################\r\n\n");
    	printf("Buffer 1:\n\r");
    	printf("========\n\r");
    	printf("\n Index 	  Address         Value(h)        Value(d)     voltage (mV)\n");
    	printf("\r -----	------------    -------------    ----------   -------------- \n");
    	for (i = 0; i < DESTINATION_BUFF_LENGTH; i++)		/* Loop for DMA transferring ADC
    														 * results to the destination buffer */
		{
    		ADC_Read();
			delay(250);

			mVolts = (*(Buffer_1+i)*3.3*1000)/RESOLUTION;	/* Analog Input in mV */
			mem_display((uint32_t*)Buffer_1,(uint32_t)DESTINATION_BUFF_LENGTH, mVolts, i);

		}
#if DOUBLE_BUFFER

    	printf("\n\r+++++++++++++++++\n\r");
    	printf("Buffer 2:\n\r");
    	printf("========\n\r");

    	for (i = 0; i < DESTINATION_BUFF_LENGTH; i++)		/* Loop for DMA transferring ADC
    	    														 * results to the destination buffer */
		{
			ADC_Read();

			delay(250);
			mVolts = (*(Buffer_2+i)*3.3*1000)/RESOLUTION;	/* Analog Input in mV */
			mem_display((uint32_t*)Buffer_2,(uint32_t)DESTINATION_BUFF_LENGTH, mVolts, i);

		}
    }

    /* Free up memory allocations before closing */

    free(Buffer_2);
    free(Buffer_1);

#else
	}

    free(Buffer_1);

#endif

}

/*******************************************************************************
 * 									main() - End
 *******************************************************************************/
#if DOUBLE_BUFFER
/*******************************************************************************
 * 					    DMA Interrupt Service Routine - Start
 *******************************************************************************/

void DMA0_IRQHandler(dma_transfer_config_t transferConfig)
{
    DMA_ClearChannelStatusFlags(DMA0, DMA_CHANNEL, kDMA_TransactionsDoneFlag);	    /* Clear transaction done interrupt flag */

    if(Swap_Buffers == false)
    {
    	p_destAddress = Buffer_1;
    	Swap_Buffers = true;
    }

    else
	{
		p_destAddress = Buffer_2;
		Swap_Buffers = false;
	}

    led();		// Toggle on-board "Blue" LED
}

/*******************************************************************************
 * 					    DMA Interrupt Service Routine - End
 *******************************************************************************/
#endif

