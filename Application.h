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
* @file Application.h
* @brief This header file provides the adc prototypes and include files.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 28 2019
* @version 1.0
*
*/

#include "dma.h"
#define COEF 122
#define SHIFT 7
#define LOOKUP_SIZE 6


typedef struct
{
	int32_t db_Value;
	uint32_t sample;
}table;

/*******Function Prototypes*******/
  uint32_t PeakLevel (uint32_t *, uint16_t );
  uint32_t decay(uint32_t );
  int32_t deciBel(uint32_t );
