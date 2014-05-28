
#ifndef PROGRAMM_CONFIG_DEFAULT_H
#define PROGRAMM_CONFIG_DEFAULT_H

#include "stm32f4xx.h"

#include "Types.h"

#define SIZE_CONS_OUT		100
#define SIZE_CONS_IN		20

#define BUFFER_SIZE     200  /*Maximum size bufer receive from FPGA*/
#define TIME_RESET			200   /* 20*256 clk before reset*/



/*Priority interrupt */



/*Priority tasks*/
/*Period task*/
#define START_CAL_PERIOD			10000
#define TCP_IPConnect_PERIOD 	10000
#define PERIOD_LED_TASK 			10000

/*Network setting default*/
#define MAC_1			0x00
#define MAC_2			0x08
#define MAC_3			0xDC
#define MAC_4			0x01
#define MAC_5			0x02
#define MAC_6			0x03//MAC Address

#define IP_1 	 		192
#define IP_2			168
#define IP_3			0
#define IP_4			51//IP Address

#define GateWay_1	192
#define GateWay_2 168
#define GateWay_3 0
#define GateWay_4 1//Gateway Address

#define SubNet_1 	255
#define SubNet_2 	255
#define SubNet_3	255
#define SubNet_4	0//SubnetMask Address

#define PortScience 5000

/*Network settings server default*/
#define Dest_IP_1 	192
#define Dest_IP_2 	168
#define Dest_IP_3 	0
#define Dest_IP_4 	36 //DST_IP Address server

#define Dest_PORT 	5000 //DST_IP port server

#endif

