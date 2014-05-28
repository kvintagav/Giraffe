/******************** (C) COPYRIGHT 2014 Kazakov Andrey ********************
* File Name          : console.c
* Author             : Kazakov Andrey
* Version            : V1.0
* Date               : 28/05/2014
* Description        : This file provides a set of functions needed to manage the
*                      communication between user and device via console 
*******************************************************************************/

#include "console.h"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


extern SemaphoreHandle_t xMutexUSART_CONSOLE;

void send_byte(char data);

enum tel_cmd {
  HELP_CMD,
	PRINT,
	SETTINGS,
	SAVE,
	REBOOT,
    
};

// Command table
char *commands[] = {
  "help",
	"print",
	"set",
	"save",
	"reboot",
  NULL
};
/*********************************************
* Function Name  : LED_INIT
* Description    : init led for indication work of different process and freertos
*********************************************/
void LED_INIT(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(LED_GPIO_RCC, ENABLE);
 
    GPIO_StructInit(&GPIO_InitStructure);
    
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = LED1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
	  GPIO_ResetBits(LED_GPIO,LED1);
}

/*********************************************
* Function Name  : DEBUG_USART_INIT
* Description    : init console for control and debug devices
*********************************************/

void CONSOLE_USART_INIT(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
/* Enable GPIOx and AFIO clocks */

	RCC_APB2PeriphClockCmd(USART_CLK , ENABLE);

	RCC_AHB1PeriphClockCmd(USART_PORT , ENABLE);

  /* Configure    USART2 Tx as alternate function push-pull */

  GPIO_InitStructure.GPIO_Pin = USART_GPIO_TX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(USART_GPIO, &GPIO_InitStructure);

  /* Configure    USART2 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin =  USART_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  
  GPIO_Init(USART_GPIO, &GPIO_InitStructure);
	
	GPIO_PinAFConfig( USART_GPIO, USART_TX_SOURCE, USART_AF );
	GPIO_PinAFConfig( USART_GPIO, USART_RX_SOURCE, USART_AF );

  /* Enable USART2 clocks */

	USART_InitStructure.USART_BaudRate = 921600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART, &USART_InitStructure);
	//USART_Cmd(USART,ENABLE);
	
	__enable_irq(); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	

	NVIC_EnableIRQ(USART1_IRQn); 
	NVIC_SetPriority(USART1_IRQn,configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1); 
	USART->CR1 |= USART_CR1_RXNEIE; 
	USART_Cmd(USART,ENABLE);
}

/******************************************************************************
* Function Name  : ResetStart
* Description    : Reboot system
*******************************************************************************/
void ResetStart(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(TIME_RESET);
	IWDG_ReloadCounter();
	IWDG_Enable();
}


/******************************************************************************
* Function Name  : CommandProcessing
* Description    : Processing input command 
*******************************************************************************/
void CommandProcessing( char *bufer_in, char *bufer_out)
{
	
	char **cmdp;
  char *cp;
  char *help = {"\nhelp: Show all available commands\
    \r\n print: show all settings parameter\
    \r\n set: setting need parameter\
		\r\n save: change settings to save \
		\r\n reboot: reboot system, start with new settings\r"}; /* command HELP : Message */ 

			
			
		if(*bufer_in != '\0') {
    /* Find the input command in table; if it isn't there, return Syntax Error */
    for(cmdp = commands; *cmdp != NULL; cmdp++) {      
      if(strncmp(*cmdp, bufer_in, strlen(*cmdp)) == 0) break;      
    }
		
		 if(*cmdp == NULL) {
      sprintf(bufer_out,"\nBAD command\r");
      return;
    }
		 switch(cmdp - commands) {
					 
				case HELP_CMD :     /* Process HELP command */
							sprintf(bufer_out, help);
					
					break;
				
				case PRINT:      /* Process GET LED command */
					break;
					
				case SETTINGS :      /* Process LED3 ON command */      
					break;
					
				case SAVE :      /* Process LED4 ON command */
					break;
					
				case REBOOT :     /* Process LED3 OFF command */
					sprintf(bufer_out, "\ndevice rebooot\r");
					ResetStart();
				default :
					break;
			}
	}
}

/******************************************************************************
* Function Name  : send_byte
* Description    : Send byte to usart
*******************************************************************************/	
void send_byte(char data)
{
	while(!(USART->SR & USART_SR_TC));
	USART_SendData(USART,data);
}
/******************************************************************************
* Function Name  : usart_send
* Description    : send string to out 
*******************************************************************************/	
void console_send( char* string)
{
	int i=0;
	
	xSemaphoreTake(xMutexUSART_CONSOLE , portMAX_DELAY);

	
	while(string[i])
	{
		send_byte(string[i]);
			i++;
	}
	xSemaphoreGive(xMutexUSART_CONSOLE );

}
/******************************************************************************
* Function Name  : 
* Description    : 
*******************************************************************************/
