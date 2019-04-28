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
* @file adc.h
* @brief This header file provides the adc prototypes and include files.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 27 2019
* @version 1.2
*
*/

/*==================================================================*/

#include "dma.h"

/*==================================================================*/

/* Number of steps in 16 bits */
#define RESOLUTION 65536


/*******Function Prototypes*******/
  void ADC0_init(void);
  void delay (uint16_t);
  void gpio_config(void);
  uint32_t ADC_Read(void);
  void led(void);
  uint32_t PeakLevel (uint32_t*, uint16_t);
  uint32_t decay(uint32_t );
