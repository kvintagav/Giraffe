/*
    
*/

/******************************************************************************
 
 */


#include "main.h"
#include "fsmc_fpga.h"
#include "user_tasks.h"
#include "console.h"




/*Queue*/

extern char bufer_cons_out[SIZE_CONS_OUT];



/*******************************************************************/
void InitAll()
{
		CONSOLE_USART_INIT();
		console_send("\r\n\r\nDevice_start\r");

		LED_INIT();
		
		
		#ifdef EEPROM
			I2C_EE_INIT();
			if (ReadConfig()==TRUE)	console_send("\nEEPROM start\r");
			else console_send("\nEEPROM is not connect \r\n parameters is enabled by default\r");
		#else
				SettingsDefault();
				CheckAndWriteVersion();
			  PrintVersion(bufer_cons_out);
				console_send(bufer_cons_out);
				
			
		#endif

			WIZ_GPIO_Install();
			WIZ_Config();
			console_send("\nWIZNET start\r");
	
	
}
 
 
 

 
/*******************************************************************/
int main(void)

{
  InitAll();
		
	while(1)
	{
	}
}

