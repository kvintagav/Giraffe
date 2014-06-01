
#ifndef __MAIN_H
#define __MAIN_H

/* Kernel includes. */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"
#include "semphr.h"
#include "Types.h"
/* Hardware and starter kit includes. */

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "system_stm32f4xx.h"

/* User Libs */
#include "stdbool.h"
#include "stdio.h"
/*WIZNET LIB*/

#define FSMC

#ifdef WIZNET
	#include "util.h"
	#include "config.h"
	#include "w5200.h"
	#include "socket.h"
	#include "spi2.h"
	#include "loopback.h"
#endif

#ifdef EEPROM
	#include "eeprom.h"
#endif

/*Config Gpio for platform */
#ifdef DISCOVERY
 #include "PlatformConfigDiscovery.h"
#endif

#include "ProgrammConfigDefault.h"

#define TRUE 	1
#define FALSE 0



#endif /* __MAIN_H */
