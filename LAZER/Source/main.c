/*
    
*/

/******************************************************************************
 
 */


#include "main.h"
#include "fsmc_fpga.h"
#include "user.h"
#include "console.h"
#include "motor.h"
#include "flash.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"




/*Queue*/

extern char bufer_cons_out[SIZE_CONS_OUT];
extern CONFIG_MSG Config_Msg;

extern uint8 DATA_BUFF_A[TX_RX_MAX_BUF_SIZE]; 
extern MOTOR_STATE motor[NUMBERS_MOTOR];



/*******************************************************************/
void InitAll()
{
//	int i;
//	uint8 buffer_in[16]={0};
	//uint8 buffer_out[16]={0};
	
	CONSOLE_USART_INIT();

	console_send("\r\nFour motors device start\r");

	
	
	
	LED_INIT();
	I2C_INIT();


	ReadConfig();
	
	/*	
		#ifdef EEPROM
		
	if (ReadConfig()==TRUE)	console_send("\nEEPROM start\r");
			else 
			{
				console_send("\nEEPROM is not connect \r\n parameters is enabled by default\r");
				SettingsDefault();
			}
		#else
			SettingsDefault();
			CheckAndWriteVersion();
			PrintVersion(bufer_cons_out);
			console_send(bufer_cons_out);
		#endif
	*/
		(SysTick_Config(SystemCoreClock / 16));
		

		motorInitTimer();
		motorInitGpio();
		motorInit();	
		motorTest();
		motorSettings();
	
		
		WIZ_GPIO_Install();
		WIZ_Config();
		console_send("\nWIZNET start\r");
}
 
 
 

 
/*******************************************************************/
int main(void)

{
//	int i;
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
 
    GPIO_StructInit(&GPIO_InitStructure);
    
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
	  GPIO_SetBits(GPIOD,GPIO_Pin_5);
		GPIO_ResetBits(GPIOD,GPIO_Pin_5);
	InitAll();

	socket(0, Sn_MR_TCP, Config_Msg.port_science, 0);
	
	
	while(1)
	{
		 motor_tcps(0, Config_Msg.port_science);
		
	}
}

