

#include "user.h"



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
* Function Name  : ChangeLED
* Description    : change led state
*********************************************/
void ChangeLED(void)
{
	if ( GPIO_ReadInputDataBit_BOOL(LED_GPIO, LED1) == 0)
		{
				GPIO_SetBits(LED_GPIO,LED1);
		}
		else
		{
				GPIO_ResetBits(LED_GPIO,LED1);
		}
}		


/*********************************************
* Function Name  : ChangeLED
* Description    : change led state
*********************************************/
void TIM_INIT(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	  // Time base configuration 	
  TIM_TimeBaseStructure.TIM_Period = 500;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;      
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM_MOTOR, &TIM_TimeBaseStructure);
  // Prescaler configuration 
  TIM_PrescalerConfig(TIM_MOTOR, 35, TIM_PSCReloadMode_Immediate);
  // TIM enable counter 
  TIM_Cmd(TIM_MOTOR, ENABLE);
  /* TIM IT enable */
  TIM_ITConfig(TIM_MOTOR, TIM_IT_Update, ENABLE);  
	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
}
