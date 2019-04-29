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
* @file dma.h
* @brief This header file provides the dma prototypes and include files.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 25 2019
* @version 1.2
*
*/

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_dma.h"
#include "fsl_dmamux.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include <stdlib.h>

/*******************************************************************************
 * 									Definitions
 ******************************************************************************/
#define DOUBLE_BUFFER 1
#define FREE_RUNNING 0						/* Continuous running for demo and report */


#if !DOUBLE_BUFFER
/* Max value of 3477 */
#define DESTINATION_BUFF_LENGTH 256

#else
/* Max value of 1738 */
#define DESTINATION_BUFF_LENGTH 128

#endif

#define SOURCE_BUFF_LENGTH 1
#define DMA_CHANNEL 0
#define DMA_SOURCE kDmaRequestMux0ADC0		// Source of data to DMA (ADC)

/*******************************************************************************
 * 						User Defined Function Prototypes
 ******************************************************************************/

  void DMA_Config (dma_handle_t*, dma_transfer_config_t);

  /****************************************************************************/
