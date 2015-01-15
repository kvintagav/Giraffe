#ifndef __USER_H
#define __USER_H



#include "main.h"



void LED_INIT(void);
void ChangeLED(void);
void TIM_INIT(void);
void TIM_USER_HANDLER(void);
void I2C_INIT(void);

void Delay (uint32 count);

#endif
