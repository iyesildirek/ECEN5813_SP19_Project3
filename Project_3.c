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
* @date April 27 2019
* @version 1.5
*
*/

#include "dma.h"
#include "adc.h"
#include "mem_display.h"

/* Select Project to Compile*/
#define PART2 1

#if !PART2
/******************************************************************************
 * 			Variables used by DMA driver  fsl_dma driver ver 2.0.1
 ******************************************************************************/

dma_handle_t DMA_Handle;						// Structure variable includes DMA Address, call back function and parameter.
uint32_t ADC_Result = (uint32_t) (&ADC0->R[0]);	// Variable holding address of ADC result register "ADC0->R[0]"
dma_transfer_config_t transferConfig;

uint32_t *p_destAddress = NULL;					// Pointer used to swap buffers within DMA ISR.

uint32_t Buffer_1[DESTINATION_BUFF_LENGTH];

#if DOUBLE_BUFFER

uint32_t Buffer_2[DESTINATION_BUFF_LENGTH];

#endif

bool Swap_Buffers = true;

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
    uint32_t i = 0;
    uint32_t numOfBytes = 4 * DESTINATION_BUFF_LENGTH;		// 4 bytes per transfer (x32 bits)

    DMA_Config(&DMA_Handle, transferConfig);

    /******************************************************************************
     ************** Prepare DMA for transferring data from ADC to memory **********
     ************** 			using DMA driver functions				 **********
     ******************************************************************************/

    p_destAddress = Buffer_1;
    DMA_SetTransferConfig(DMA0, DMA_CHANNEL, &transferConfig);

#if DOUBLE_BUFFER

    DMA_EnableInterrupts(DMA0, DMA_CHANNEL);

#endif

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

    	if(p_destAddress == Buffer_1)
    	{
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

				delay(10);
				mVolts = (*(Buffer_1+i)*3.3*1000)/RESOLUTION;	/* Analog Input in mV */
				mem_display((uint32_t*)Buffer_1,(uint32_t)DESTINATION_BUFF_LENGTH, mVolts, i);
			}
    	}

#if DOUBLE_BUFFER

    	else
    	{
			printf("Buffer 2:\n\r");
			printf("========\n\r");
			printf("\n Index 	  Address         Value(h)        Value(d)     voltage (mV)\n");
			printf("\r -----	------------    -------------    ----------   -------------- \n");

			for (i = 0; i < DESTINATION_BUFF_LENGTH; i++)		/* Loop for DMA transferring ADC
																 * results to the destination buffer */
			{
				ADC_Read();

				delay(10);
				mVolts = (*(Buffer_2+i)*3.3*1000)/RESOLUTION;	/* Analog Input in mV */
				mem_display((uint32_t*)Buffer_2,(uint32_t)DESTINATION_BUFF_LENGTH, mVolts, i);
			}
    	}

    	printf("\n\rPress 'x' to exit, or any other key to continue...\n\n\r");

    	if(getchar() == 'x')
    	{
			printf("\n\rGood Bye!\n\n\r");
			break;
		}
    }

    /* Free up memory allocations before closing */

    free(Buffer_2);
    free(Buffer_1);

#else

		printf("\n\rPress 'x' to exit, or any other key to continue...\n\n\r");

		if(getchar() == 'x')
		{
			printf("\n\rGood Bye!\n\n\r");
			break;
		}
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

void DMA0_IRQHandler(void)
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

/* Part 2 Demo */
#else
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

while (1)
	{
		/************************************
		 * Set AIEN to 1 to enable interrupt
		 * Set DIFF to 0b0 for single
		 * Set ADCH to 0b00000 for ADC_SE0
		 * Set register to 0x40
		 **********************************/
	ADC0->SC1[0] = 0x40;
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
	}
}
#endif
