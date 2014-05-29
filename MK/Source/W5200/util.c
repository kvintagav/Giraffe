
#include "stm32f4xx.h"
//#include "config.h"
#include "util.h"
#include "w5200.h"
#include "main.h"
#include "console.h"
#include <stdio.h>
#include <stdarg.h>




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



