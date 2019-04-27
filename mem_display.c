/***************************************************************************
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
* @file mem_display.c
* @brief This source file contains code that provides the display() function definition
* and displays content of memory in terms of 32 bit words in hexadecimal format.
*
* @author Ismail Yesildirek & Bijan Kianian
* @date April 27 2019
* @version 1.1
*
*/


#include "mem_display.h"
#include "dma.h"

void mem_display(uint32_t* address, uint32_t offset, uint16_t Voltage, uint16_t index)
{
    printf("\r   %d	 %p       0x%08lx        %d           %d\n" , index, address + index, *(address + index), *(address + index), Voltage);
    return;
}
