
#ifndef __MAIN_H
#define __MAIN_H


/* Hardware and starter kit includes. */

#include "Types.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "system_stm32f4xx.h"

/* User Libs */
#include "stdbool.h"
#include "stdio.h"
/*WIZNET LIB*/

//#define FSMC


	#include "util.h"
	#include "config.h"
	#include "w5200.h"
	#include "socket.h"
	#include "spi2.h"
	#include "loopback.h"


#ifdef EEPROM
	#include "eeprom.h"
#endif

/*Config Gpio for platform */
#ifdef DISCOVERY
	#include "PlatformConfigDiscovery.h"
#else 
	#include "PlatformConfig.h"
#endif

#include "ProgrammConfigDefault.h"

#define TRUE 	1
#define FALSE 0



#endif /* __MAIN_H */
