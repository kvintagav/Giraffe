/*
    
*/

/******************************************************************************
 
 */


#include "main.h"
#include "fsmc_fpga.h"
#include "user.h"
#include "console.h"

#include "motor.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdio.h"


/*Queue*/

extern char bufer_cons_out[SIZE_CONS_OUT];
extern CONFIG_MSG Config_Msg;

extern uint8 DATA_BUFF_A[TX_RX_MAX_BUF_SIZE]; 
extern MOTOR_STATE motor[MOUNT_DRIVERS];



/*******************************************************************/
void InitAll()
{
		CONSOLE_USART_INIT();
		console_send("\r\n\r\nDevice_start\r");

		LED_INIT();
		
		I2C_INIT();
	
		#ifdef EEPROM
			
			if (ReadConfig()==TRUE)	console_send("\nEEPROM start\r");
			else console_send("\nEEPROM is not connect \r\n parameters is enabled by default\r");
		#else
		SettingsDefault();
		CheckAndWriteVersion();
		PrintVersion(bufer_cons_out);
		console_send(bufer_cons_out);
		
			
		#endif
	
		init_gpio_motor();
		init_motor();	
		motorTest();
		(SysTick_Config(SystemCoreClock / 16));
		WIZ_GPIO_Install();

		WIZ_Config();

		console_send("\nWIZNET start\r");
	

		
}
 
 
 

 
/*******************************************************************/
int main(void)

{
  InitAll();
		
	socket(0, Sn_MR_TCP, Config_Msg.port_science, 0);
	
	//socket(1, Sn_MR_TCP, Config_Msg.port_science+1, 0);

	while(1)
	{
		 motor_tcps(0, Config_Msg.port_science);
		
	}
}

