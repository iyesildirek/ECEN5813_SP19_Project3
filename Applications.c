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
* @file Applications.c
* @brief This header file provides the peak detector prototypes and include files.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 27 2019
* @version 1.0
*
*/

/*=======================================================================*/

#include "Application.h"

/*=======================================================================*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ PeakLevel () - Start @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

uint32_t PeakLevel (uint32_t* buffer, uint16_t size)

{
	uint32_t current, peak, temp;
	peak = *buffer;

	for (uint16_t i=0 ; i <size-1 ; i++)
	{
		current = *(buffer + i);
		temp = *(buffer + i + 1);

		if (current <= temp)
			peak = temp;
		else
			peak = decay(peak);
	}
	return peak;
}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ PeakLevel () - End @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*********************************** deacy () - Start *******************************/

uint32_t decay(uint32_t num)
{

	uint32_t result = num  >> COEF;
	return result;
}

/*********************************** deacy () - End *******************************/

