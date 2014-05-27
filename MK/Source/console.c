

#include "console.h"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


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
void CommandProcessing(char *bufer_in, char *bufer_out)
{
	char **cmdp;
  char *cp;
  char *help = {"help: Show all available commands\
    \r\n print:\
    \r\n set:\
    \r\n save:\
		\r\n reboot:\r\n"}; /* command HELP : Message */ 
	
		
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
				
				ResetStart();
      default :
        break;
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
	while(string[i])
	{
		send_byte(string[i]);
			i++;
	}
}
/******************************************************************************
* Function Name  : 
* Description    : 
*******************************************************************************/