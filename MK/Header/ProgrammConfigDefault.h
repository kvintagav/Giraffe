
#ifndef PROGRAMM_CONFIG_DEFAULT_H
#define PROGRAMM_CONFIG_DEFAULT_H

#include "stm32f4xx.h"

#include "Types.h"

#define SIZE_CONS_OUT		100
#define SIZE_CONS_IN		20

#define BUFFER_SIZE     200  /*Maximum size bufer receive from FPGA*/
#define TIME_RESET			20   /* 20*256 clk before reset*/



/*Priority interrupt */



/*Priority tasks*/

/*Period task*/
#define PERIOD_LED_TASK 10000


#endif

