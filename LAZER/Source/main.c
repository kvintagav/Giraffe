/*
    
*/

/******************************************************************************
 
 */


#include "main.h"
#include "fsmc_fpga.h"
#include "user.h"
#include "console.h"
#include "eeprom.h"
#include "motor.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdio.h"

#define  NUMB_PARAM 10
int InParMassiv[NUMB_PARAM];
char command[10];
/*Queue*/

extern char bufer_cons_out[SIZE_CONS_OUT];
extern CONFIG_MSG Config_Msg;

extern uint8 DATA_BUFF_A[TX_RX_MAX_BUF_SIZE]; 
extern MOTOR_STATE motor[MOUNT_DRIVERS];



/*******************************************************************/
void InitAll()
{
	 GPIO_InitTypeDef GPIO_InitStructure;

		CONSOLE_USART_INIT();
		console_send("\r\n\r\nDevice_start\r");

		LED_INIT();
		
/*	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 
    GPIO_StructInit(&GPIO_InitStructure);
    
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	*/
	
	
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
	
	//	TIM_INIT(); //init user timer 9600 khz
	//	init_motor();	
	
		WIZ_GPIO_Install();

		WIZ_Config();
		console_send("\nWIZNET start\r\n\r>");
	
	(SysTick_Config(SystemCoreClock / 16));
		
}
 
 
 

 
/*******************************************************************/
int main(void)

{
//	int i;
  InitAll();
		
	/* Функции для работы с I2C 	
		bool motorSendI2C(uint8 address, uint8 cmd ,uint8 data )
	пример	motorSendI2C(ADDRES_DRIVER_0,OUTPORT,255);
		

		bool motorRecvI2C(uint8 address, uint8 cmd ,uint8 *data )
	пример	motorRecvI2C(ADDRES_DRIVER_0, INPORT , &koncevic1);
	в motorRecv значёк & ОБЯЗАТЕЛЕН - это указатель, без него работать не будет
	*/
	socket(0, Sn_MR_TCP, Config_Msg.port_science, 0);
		
	
	while(1)
	{
	//	motorTest();
			
		/// loopback_tcps(0, Config_Msg.port_science);
		
		
		 if (motor_tcps( 0,Config_Msg.port_science)==TRUE) 
		 {
			 ParsingParameter(DATA_BUFF_A, InParMassiv, command);
			 if   (strstr( command,"DRIVE")!=NULL)
			 {
				 if (motorTurn(InParMassiv[1]-1,InParMassiv[0], InParMassiv[2] )==ERROR)
				 {	
					 // send corner turn motor 
				 }
			 }
		 }
		
	}
}

