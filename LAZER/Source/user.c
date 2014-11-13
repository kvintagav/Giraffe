

#include "user.h"
#include <stdbool.h>

#define OPEN 1
#define CLOSE 0

uint16  cnt;
int16  	R_cnt;					// polojenie diafragmy
int16  	R_max;
uint16  P_cnt;					// schivaem v prerivanii
bool  Napr;
uint16  S_par;					// koef delenia v prerivanii
int16   R_tmp;
void Obnulaem_Upr_Dvigatel(void);


void Obnulaem_Upr_Dvigatel(void)
{
	GPIO_ResetBits(DMOT_GPIO,DMOT0);			// obnulaem upravlenie dvigatela
	GPIO_ResetBits(DMOT_GPIO,DMOT1);
	GPIO_ResetBits(DMOT_GPIO,DMOT2);
	GPIO_ResetBits(DMOT_GPIO,DMOT3);
}
int Otkr_Zakr(int persent)	// otkpivaem-zakrivaem ha prosenty
{
	int tempbyte;
	int temp;
	int Sint;
	if (persent <= 100)
	{
			temp = persent;
			if (R_cnt < temp)
					{Napr = CLOSE;		Sint = temp - R_cnt;}					// opredelaem napravlenie
			else
					{Napr = OPEN;		Sint = R_cnt - temp;}					// vrashenia
			while (R_cnt != temp)
			{
					if (Sint < 63)															// reguliruem
					{
							tempbyte = (Sint >> 5);									// skorost
							S_par = 120 - tempbyte;
					}
					else	
							S_par = 63;															// vrashenia 
			}
			
			Obnulaem_Upr_Dvigatel();
			
			S_par = 0;														// v prerivanii nichego ne delaem
			tempbyte = 0;
	}
	else
			tempbyte = -1;	
	return tempbyte;
}	

// Kalibrovka
void Set_OKO(void)
{
		S_par = 100;
		cnt = 1;
		Napr = OPEN;						// otkrivaem diafragmu polnostu
		while (cnt != 9)		// kogda srabotaet praviy konsevik
		{										// zaskochem vnutr
				cnt = 1;
				while (cnt != 9);	
		}
		Napr = CLOSE;	cnt = 1;	// pomenaly napravlenie - teper zakrivaem
		while (cnt != 10);	
		Napr = OPEN;	cnt = 1;	// pomenaly napravlenie - teper otkrivaem
		while (cnt != 9);
		// diafragma polnostu otkrita
		Obnulaem_Upr_Dvigatel();	// obnulaem upravlenie dvigatela
		
		S_par = 0;												// v prerivanii nichego ne delaem
		Napr = CLOSE;		cnt = 1;		
}
void Delay (uint32 count)
{
uint16 i;
	do								{		
		for (i=0; i < 20000; i++) ;         // 
		count--;
	}	while (count != 0);
}	

void MOT_OBOR ()
{

	if (GPIO_ReadInputDataBit(DCON_GPIO,DCON1)==0)		//	if (DCON1 == 0)
	{
		if (Napr==OPEN)
		{
			Obnulaem_Upr_Dvigatel();	cnt = 9;
		}
		R_cnt = 0;
	}
	if (GPIO_ReadInputDataBit(DCON_GPIO,DCON2)==0)		//	if (DCON2 == 0)
	{
		if (Napr == CLOSE)
		{
			Obnulaem_Upr_Dvigatel();	cnt = 10;
		}
		R_max = R_cnt;
	}																
	if (cnt < 9)
	{
		if (Napr == CLOSE)
		{
			switch (cnt)
			{
				case 1:	{GPIO_ResetBits(DMOT_GPIO,DMOT3);
								GPIO_SetBits(DMOT_GPIO,DMOT0); 	cnt++;}	  /* 0001 */	break;
				case 2:	{GPIO_SetBits(DMOT_GPIO,DMOT1);	cnt++;}	  /* 0011 */	break;
				case 3:	{GPIO_ResetBits(DMOT_GPIO,DMOT0);	cnt++;} /* 0010 */	break;
				case 4:	{GPIO_SetBits(DMOT_GPIO,DMOT2); cnt++;}   /* 0110 */	break;
				case 5:	{GPIO_ResetBits(DMOT_GPIO,DMOT1);	cnt++;} /* 0100 */	break;
				case 6:	{GPIO_SetBits(DMOT_GPIO,DMOT3);	cnt++;}   /* 1100 */	break;
				case 7: {GPIO_ResetBits(DMOT_GPIO,DMOT2); cnt++;} /* 1000 */	break;
				case 8:	{GPIO_SetBits(DMOT_GPIO,DMOT0); cnt = 1;} /* 1001 */	break;
				default: cnt = 1; break;
			}
			if (R_tmp >= 18)
					{R_cnt++;	R_tmp = 0;}
			R_tmp++;
		}
		else
		{
			switch (cnt)
			{
				case 1:	{GPIO_ResetBits(DMOT_GPIO,DMOT0);
								GPIO_SetBits(DMOT_GPIO,DMOT3);	cnt++;}		/* 1000 */ break;
				case 2:	{GPIO_SetBits(DMOT_GPIO,DMOT2);	cnt++;} 	/* 1100 */ break;
				case 3:	{GPIO_ResetBits(DMOT_GPIO,DMOT3);	cnt++;} /* 0100 */ break;
				case 4:	{GPIO_SetBits(DMOT_GPIO,DMOT1); cnt++;} 	/* 0110 */ break;
				case 5:	{GPIO_ResetBits(DMOT_GPIO,DMOT2);	cnt++;} /* 0010 */ break;
				case 6:	{GPIO_SetBits(DMOT_GPIO,DMOT0);	cnt++;} 	/* 0011 */ break;
				case 7:	{GPIO_ResetBits(DMOT_GPIO,DMOT1); cnt++;} /* 0001 */ break;
				case 8:	{GPIO_SetBits(DMOT_GPIO,DMOT3); cnt = 1;}	/* 1001 */ break;
				default:	cnt = 1;    	break;
			}
			if ((18+R_tmp) <= 0)
					{R_cnt--;	R_tmp = 0;}
			R_tmp--;
		}
		
	}
}

/*********************************************
* Function Name  : DMOT_INIT
* Description    : init <<Diafragm-MOTOR>>
*********************************************/
void DMOT_INIT(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
 
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  /* Configure PC6,7 pin as input PULLup */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 ;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
// proverka (mojno ubrat)
		Obnulaem_Upr_Dvigatel();
		
	  GPIO_SetBits(DMOT_GPIO,DMOT0);
		GPIO_ResetBits(DMOT_GPIO,DMOT0);
		GPIO_SetBits(DMOT_GPIO,DMOT1);
		GPIO_ResetBits(DMOT_GPIO,DMOT1);
		GPIO_SetBits(DMOT_GPIO,DMOT2);
		GPIO_ResetBits(DMOT_GPIO,DMOT2);
		GPIO_SetBits(DMOT_GPIO,DMOT3);	
		GPIO_ResetBits(DMOT_GPIO,DMOT3);	
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
void TIM_USER_HANDLER(void)
{
		if (S_par != 0)				// esly to ne dvigaemsa
		{
				if (P_cnt > S_par)
				{
						MOT_OBOR ();  	P_cnt = 0;
				}
				P_cnt++;
		}
}
