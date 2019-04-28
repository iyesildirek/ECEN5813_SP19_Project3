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

static table vlookup[] =
{
		{0, 65536},
		{-3, 46334},
		{-6,32768},
		{-10,20709},
		{-20, 6554},
		{-50, 0}
} ;

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

	uint32_t result = num * COEF >> SHIFT ;
	return result;
}

/*********************************** deacy () - End *******************************/

/*********************************** dB () - Start *******************************/
int32_t deciBel(uint32_t peak)
{
	int32_t dB, mid, mid2, mid_loop_top,mid_loop_bot;
	/* midpoint between 0 and 3dB */
	mid = ((vlookup[0].sample-vlookup[1].sample)>>1)+vlookup[1].sample;
	/* midpoint between -20 and -50dB */
	mid2 = (vlookup[4].sample-vlookup[5].sample)>>1;
	
	if (peak >= mid)
	{
		dB = vlookup[0].db_Value;
	}
	else if((peak < mid) && (peak > mid2) ) 
	{
		for (uint8_t i=1; i<LOOKUP_SIZE-2; i++)
		{
			mid_loop_top = ((vlookup[i-1].sample-vlookup[i].sample)>>1)+vlookup[i].sample;
			mid_loop_bot = ((vlookup[i].sample-vlookup[i+1].sample)>>1)+vlookup[i+1].sample;
			
		if ((peak < mid_loop_top) && (peak >= mid_loop_bot))
			{
			dB = vlookup[i].db_Value;
			}
		}
	}
	else
	{
		dB = vlookup[5].db_Value;
	}
	return dB;
}

/*********************************** dB () - End *******************************/
