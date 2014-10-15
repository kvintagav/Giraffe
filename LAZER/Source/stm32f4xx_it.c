/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and peripherals interrupt
  *          service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "stdbool.h"
#include "fsmc_fpga.h"
#include "user.h"
#include "config.h"
#include "console.h"
#include "motor.h"

extern void Timer2_ISR(void);

char bufer_console[SIZE_CONS_IN]; 
char bufer_cons_out[SIZE_CONS_OUT];
vu8 index_buf_cons;
uint8 led_delay;

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


void EXTI2_IRQHandler (void)
{
	if(EXTI_GetITStatus(INT_PCA9539_1_IRQLine) != RESET)
  {
    motor_senser(); 
    EXTI_ClearITPendingBit(INT_PCA9539_1_IRQLine);
  }
}
void EXTI3_IRQHandler (void)
{
	if(EXTI_GetITStatus(INT_PCA9539_2_IRQLine) != RESET)
  {
    motor_senser(); 
    EXTI_ClearITPendingBit(INT_PCA9539_2_IRQLine);
  }
}

/*************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    :  This function handles EXT NWAIT for FPGA .
*************************************************************/
void EXTI9_5_IRQHandler(void)
{
	/*
	long xHigherPriorityTaskWoken = pdFALSE;
	EXTI_ClearITPendingBit( EXTI_Line6 );

	xSemaphoreGiveFromISR( xSemaphoreEXTI, &xHigherPriorityTaskWoken );
	if( xHigherPriorityTaskWoken == pdTRUE )
  {
     portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
	*/
}
/*********************************************************************
* Function Name  : DMA2_Stream0_IRQHandler
* Description    :  This function handles end translate data from fpga.
**********************************************************************/
void DMA2_Stream0_IRQHandler(void)
{
/*	long xHigherPriorityTaskWoken = pdFALSE;
  DMA_ClearITPendingBit(DMA_FSMC_STREAM,DMA_FSMC_IT_TCIF_CHECK);

  xSemaphoreGiveFromISR( xSemaphoreFSMCDMA, &xHigherPriorityTaskWoken );
	if( xHigherPriorityTaskWoken == pdTRUE )
  {
     portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
*/	
}

/*********************************************************************
* Function Name  : SysTick_Handler
* Description    :  This function change led.
**********************************************************************/
void SysTick_Handler(void)
{
	led_delay++;
	if (led_delay>=16)
	{
		ChangeLED();
		led_delay=0;
	}
}

/*********************************************************************
* Function Name  : USARTx_IRQHandler
* Description    : This function handles byte from host
**********************************************************************/

void USART1_IRQHandler(void)
{
	char input_data; 

	
	ChangeLED();
	USART_ClearITPendingBit(USART, USART_IT_RXNE);
	input_data= USART_ReceiveData(USART);
	USART_SendData(USART,input_data);
	if (index_buf_cons<SIZE_CONS_IN) 
	{
		if (input_data=='\r') 
		{
			if (index_buf_cons==0)
			{	
				USART_SendData(USART,'\n');
				USART_SendData(USART,'>');
			}
			else
			{
				CommandProcessing( bufer_console,  bufer_cons_out);
				console_send(bufer_cons_out);
				console_send("\n>");
				index_buf_cons=0;
			}
		}
		else 
		{
			bufer_console[index_buf_cons]=input_data;
			index_buf_cons++;
		}
	}
	else 
	{
			index_buf_cons=0;
	}

	ChangeLED();

}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		Timer2_ISR();
		
	}
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
		TIM_USER_HANDLER(); 	//	This can be code for interrupt


	}
}
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */

void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */


void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
