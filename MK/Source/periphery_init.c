
#include "periphery_init.h"


/*********************************************
* Function Name  : DEBUG_USART_INIT
* Description    : init console for control and debug devices
*********************************************/

void CONSOLE_USART_INIT(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
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

	USART_InitStructure.USART_BaudRate = 115200;
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
