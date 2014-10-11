

#include "user.h"


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


void I2C_INIT(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as I2C_SDA and I2C_SCL
	I2C_InitTypeDef I2C_InitStruct; // this is for the I2C initilization
 
	/* enable APB1 peripheral clock for I2C*/
	RCC_APB1PeriphClockCmd(I2C_RCC , ENABLE);
  RCC_AHB1PeriphClockCmd(I2C_RCC_PORT , ENABLE); 
 
	GPIO_PinAFConfig(I2C_PORT, I2C_SDA_SOURCE, I2C_GPIO_AF); //
	GPIO_PinAFConfig(I2C_PORT, I2C_SCL_SOURCE, I2C_GPIO_AF);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SCL | GPIO_Pin_SDA ; // Pins (I2C_SCL) and (I2C_SDA)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;// this defines the output type as open drain
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;// this activates the pullup resistors on the IO pins
	GPIO_Init(I2C_PORT, &GPIO_InitStruct);// now all the values are passed to the GPIO_Init() 
 
	
	 /* Configure I2C */
	//I2C_DeInit(I2C);

	/* Enable the I2C peripheral */
	
	/* Set the I2C structure parameters */
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 400000;

	/* Initialize the I2C peripheral w/ selected parameters */
	I2C_Init(I2C,&I2C_InitStruct);
	I2C_Cmd(I2C, ENABLE);

	
}
/*********************************************
* Function Name  : TIM_INIT
* Description    : init user timer
*********************************************/
void TIM_INIT(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_TIM_MOTOR ,ENABLE);

	  // Time base configuration 	
  TIM_TimeBaseStructure.TIM_Period = 250;          
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
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
}
/*********************************************
* Function Name  : TIM_USER_HANDLER
* Description    : user function for tim
*********************************************/
void TIM_USER_HANDLER(void){
	
	
}
