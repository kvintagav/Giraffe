/*
*
@file		util.h
@brief	
*/

#include "Types.h"

#ifndef _UTIL_H
#define _UTIL_H


void Reset_W5200(void);


void Delay_us(uint8 time_us);
void Delay_ms(uint16 time_ms);

//int putchar(int ch);
//int getchar(void);

uint32 time_return(void);

#endif
