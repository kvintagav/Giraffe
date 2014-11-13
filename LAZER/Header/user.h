#ifndef __USER_H
#define __USER_H



#include "main.h"

#define OPEN 1
#define CLOSE 0

void LED_INIT(void);
void ChangeLED(void);
void TIM_INIT(void);
void TIM_USER_HANDLER(void);
void I2C_INIT(void);
void DMOT_INIT(void);
void MOT_OBOR ();
void Delay (uint32 count);
void Set_OKO (void);
int Otkr_Zakr (int persent);

#endif