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
* @date April 25 2019
* @version 1.2
*
*/

/*==================================================================*/

#include "dma.h"

/*==================================================================*/

 void DMA_Config(dma_handle_t* DMA_Handle, dma_transfer_config_t transferConfig)
 {
	    /* Configure DMAMUX */

	    DMAMUX_Init(DMAMUX0);
	    DMAMUX_SetSource(DMAMUX0, DMA_CHANNEL, DMA_SOURCE);
	    DMAMUX_EnableChannel(DMAMUX0, DMA_CHANNEL);

	    /* Configure DMA */

	    DMA_Init(DMA0);
	    DMA_CreateHandle(DMA_Handle, DMA0, DMA_CHANNEL);
 }
