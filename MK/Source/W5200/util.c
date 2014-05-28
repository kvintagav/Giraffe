
#include "stm32f4xx.h"
#include "config.h"
#include "util.h"
#include "w5200.h"
#include "main.h"
#include <stdio.h>
#include <stdarg.h>



extern CONFIG_MSG Config_Msg;

/*
extern uint8 txsize[MAX_SOCK_NUM];
extern uint8 rxsize[MAX_SOCK_NUM];
*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

extern uint8 MAC[6];
extern uint8 IP[4];
extern uint8 GateWay[4];
extern uint8 SubNet[4];
extern uint8 Enable_DHCP;
extern uint8 Dest_IP[4] ;
extern uint16 Dest_PORT ;

void Reset_W5200(void)
{
	GPIO_ResetBits(WIZ_PORT_SETTINGS, WIZ_RESET);
	Delay_us(2);
	GPIO_SetBits(WIZ_PORT_SETTINGS, WIZ_RESET);
	Delay_ms(150);
}





void Delay_us( u8 time_us )
{
  register u8 i;
  register u8 j;
  for( i=0;i<time_us;i++ )    
  {
    for( j=0;j<5;j++ )          // 25CLK
    {
            //1CLK                  
    }      
  }                              // 25CLK*0.04us=1us
}

void Delay_ms( u16 time_ms )
{
  register u16 i;
  for( i=0;i<time_ms;i++ )
  {
    Delay_us(250);
    Delay_us(250);
    Delay_us(250);
    Delay_us(250);
  }
}

uint32 time_return(void) 
{
  extern uint32_t my_time; 
  return my_time;
}


#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



