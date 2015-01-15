
#include "motor.h"

MOTOR_STATE motor[NUMBERS_MOTOR];


int openHole(int motor_number);
int closeHole(int motor_number);
int turnOutMotorFromSenser(int motor_number);
bool flag_new_tick ; //true - avaliable new tick, false - wait interrupt from timer 
uint8 data_test = 0;
void motorOneTick(int number, bool direction);
void sendFaza(uint8 faza);
void enableMotor(int number_motor);
void disableMotor(int number_motor);

void motorInitGpio(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(MOTOR_GPIO_RCC_D, ENABLE);
	RCC_AHB1PeriphClockCmd(MOTOR_GPIO_RCC_E, ENABLE);

  GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = MOTOR_SENSE_IN_D;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOTOR_SENSE_IN_E;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = MOTOR_OBM_OUT;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(MOT_OBM_1);
  GPIO_ResetBits(MOT_OBM_2);
  GPIO_ResetBits(MOT_OBM_3);
  GPIO_ResetBits(MOT_OBM_4);
	
	GPIO_InitStructure.GPIO_Pin = MOTOR_EN_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_ResetBits(MOT_EN_1);
  GPIO_ResetBits(MOT_EN_2);
  GPIO_ResetBits(MOT_EN_3);
  GPIO_ResetBits(MOT_EN_4);


}

void motorInitTimer(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_TIM_MOTOR ,ENABLE);

	  // Time base configuration 	
			
  TIM_TimeBaseStructure.TIM_Period = 20000;          
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

void MOTOR_HANDLER(void)
{
	flag_new_tick = true;
}

/*********************************************
* Function Name  : checkSenser 
* Description    : check sensers open and close
*********************************************/
int checkSensers(int number_motor)
{
		switch(number_motor)
		{
			case 0: 
				motor[number_motor].open = GPIO_ReadInputDataBit_BOOL(SENSE_1_OPEN);
				motor[number_motor].close = GPIO_ReadInputDataBit_BOOL(SENSE_1_CLOSE);
				 
			break;
			case 1: 
				motor[number_motor].open = GPIO_ReadInputDataBit_BOOL(SENSE_2_OPEN);
				motor[number_motor].close = GPIO_ReadInputDataBit_BOOL(SENSE_2_CLOSE);
			break;
			case 2: 
				motor[number_motor].open = GPIO_ReadInputDataBit_BOOL(SENSE_3_OPEN);
				motor[number_motor].close = GPIO_ReadInputDataBit_BOOL(SENSE_3_CLOSE);
			break;
			case 3: 
				motor[number_motor].open = GPIO_ReadInputDataBit_BOOL(SENSE_4_OPEN);
				motor[number_motor].close = GPIO_ReadInputDataBit_BOOL(SENSE_4_CLOSE);
			break;
		}	
		return ( motor[number_motor].open+motor[number_motor].close*2);
		
}

/*********************************************
* Function Name  : checkSenser 
* Description    : check sensers open and close
*********************************************/
void enableMotor(int number_motor)
{
	switch(number_motor)
	{
		case 0: 
			GPIO_SetBits(MOT_EN_1);
		break;
		case 1: 
			GPIO_SetBits(MOT_EN_2);
		break;
		case 2: 
			GPIO_SetBits(MOT_EN_3);
		break;
		case 3: 
			GPIO_SetBits(MOT_EN_4);
		break;
	}
}

/*********************************************
* Function Name  : checkSenser 
* Description    : check sensers open and close
*********************************************/
void disableMotor(int number_motor)
{
	switch(number_motor)
	{
		case 0: 
			GPIO_ResetBits(MOT_EN_1);
		break;
		case 1: 
			GPIO_ResetBits(MOT_EN_2);
		break;
		case 2: 
			GPIO_ResetBits(MOT_EN_3);
		break;
		case 3: 
			GPIO_ResetBits(MOT_EN_4);
		break;
	}
}

/*********************************************
* Function Name  : sendFaza 
* Description    : set faza on output mk
*********************************************/
void sendFaza(uint8 faza)
{
	if ((faza&0x01)>0) GPIO_SetBits(MOT_OBM_1);
	else GPIO_ResetBits(MOT_OBM_1);
	
	if ((faza&0x02)>0) GPIO_SetBits(MOT_OBM_2);
	else GPIO_ResetBits(MOT_OBM_2);
	
	if ((faza&0x04)>0) GPIO_SetBits(MOT_OBM_3);
	else GPIO_ResetBits(MOT_OBM_3);
	
	if ((faza&0x08)>0) GPIO_SetBits(MOT_OBM_4);
	else GPIO_ResetBits(MOT_OBM_4);
}
	
/*********************************************
* Function Name  : motorInit 
* Description    : settings firt set of motor
*********************************************/
void motorInit(void)
{	
	int i;
	bool rezult;
	for(i = 0 ; i < NUMBERS_MOTOR ; i++)
	{
			motor[i].work = true;
			motor[i].current_faza = 0;
			motor[i].current_tick = 0;
			motor[i].current_percent = 0;
			motor[i].max_count_tick = 0;
	}
}

/*********************************************
* Function Name  : 
* Description    : 
*********************************************/
int motorTurnOnPercent(int motor_number , int percent)
{
	int numbers_tick;
	if ((motor_number>(NUMBERS_MOTOR-1))||(motor_number<0)||(percent>100)|| (percent<0)||(motor[motor_number].work==false)) return -1;
	
	if (motor[motor_number].max_count_tick!=0)
	{
		if (percent == 0) 
		{
			openHole(motor_number);
			turnOutMotorFromSenser(motor_number);
		}
		else if (percent == 100) 
		{
			closeHole(motor_number);
			turnOutMotorFromSenser(motor_number);
		}
		else 
		{
			if (percent > motor[motor_number].current_percent)
			{
				numbers_tick = (int)((motor[motor_number].max_count_tick*(percent-motor[motor_number].current_percent))/100);
				motorTurn(motor_number,CLOSE,numbers_tick,false);
			}
			else
			{
				numbers_tick = (int)((motor[motor_number].max_count_tick*(motor[motor_number].current_percent-percent))/100);
				motorTurn(motor_number,OPEN,numbers_tick,false);
			}
		}
		motor[motor_number].current_percent=percent;
	}
	else return -1;
	return numbers_tick; 
}

/*********************************************
* Function Name  : 
* Description    : 
*********************************************/
void motorOneTick(int number, bool direction_napr)
{
	uint8 value = 0;
	bool direction =!direction_napr;
	while (!flag_new_tick){}
			flag_new_tick= false;
				
			switch (motor[number].current_faza)
			{
				case 0: 
					motor[number].current_faza=(direction) ?1 : 7; 
					value=(direction) ? FAZA1 : FAZA7;
				break;
				case 1: 
					motor[number].current_faza=(direction) ?2 : 0; 
					value=(direction) ? FAZA2 : FAZA0;
				break;
				case 2: 
					motor[number].current_faza=(direction) ?3 : 1; 
					value=(direction) ? FAZA3 : FAZA1;
				break;
				case 3: 
					motor[number].current_faza=(direction) ?4 : 2; 
					value=(direction) ? FAZA4 : FAZA2;
				break;
				case 4: 
					motor[number].current_faza=(direction) ?5 : 3; 
					value=(direction) ? FAZA5 : FAZA3;
				break;
				case 5: 
					motor[number].current_faza=(direction) ?6 : 4; 
					value=(direction) ? FAZA6 : FAZA4;
				break;
				case 6: 
					motor[number].current_faza=(direction) ?7 : 5; 
					value=(direction) ? FAZA7 : FAZA5;
				break;
				case 7: 
					motor[number].current_faza=(direction) ?0 : 6; 
					value=(direction) ? FAZA0 : FAZA6;
				break;
				default:
					motor[number].current_faza=0; 
					value= FAZA0;
				break;
			}
			sendFaza(value);
			
}


/*********************************************
* Function Name  : 
* Description    : 
*********************************************/
int motorTurn(int number, bool direction,int tick ,bool turn_to_senser)
{
	int tick_numb = 0;
	bool work_motor = true;
	bool out_from_senser =  false; //if senser enable nado otodvinut ot nego
	uint8 rezult_senser = 0;

	enableMotor(number);
	while (work_motor)
	{
		if (!turn_to_senser)
		{
				motorOneTick(number, direction);
				tick_numb++;
				if (tick_numb>=tick) work_motor = false;
		}
		else
		{
			if (out_from_senser==true)
			{
					if (checkSensers(number)>0)
					{
						motorOneTick(number, !direction);
						tick_numb--;
						if (tick_numb==0) work_motor = false;
					}
					else  work_motor=false;
			}
			else
			{
				motorOneTick(number, direction);
				tick_numb++;
				if (checkSensers(number)>0)	out_from_senser=true;
				if (tick_numb>=MAX_NUMBERS_TICKS)	work_motor = false;
				
			}
		}
	}
	disableMotor(number);
	return tick_numb;
	
}


/*********************************************
* Function Name  : 
* Description    : 
*********************************************/
int turnOutMotorFromSenser(int motor_number)
{
	int mount;
	bool turn_motor= true;
	while (turn_motor)
	{
		checkSensers(motor_number);
		if (motor[motor_number].open == true)
		{
			motorTurn(motor_number,CLOSE,TURN_ONE_PERIOD,false);
			mount+=TURN_ONE_PERIOD;
		}
		else if (motor[motor_number].close == true)
		{
			motorTurn(motor_number,OPEN,TURN_ONE_PERIOD,false);
			mount+=TURN_ONE_PERIOD;
		}
		else  turn_motor=false;
		if (mount >=MAX_NUMBERS_TICKS) turn_motor = false;
	}
	return mount;
}
int openHole(int motor_number)
{
	return motorTurn(motor_number,OPEN,0,true);
}

int closeHole(int motor_number)
{
	return motorTurn(motor_number,CLOSE,0,true);
}


/*********************************************
* Function Name  : 
* Description    : 
*********************************************/
void motorSettings(void)
{
	int motor_number;
	int mount_tick;
	int rezult;

	for (motor_number = 3 ; motor_number<NUMBERS_MOTOR; motor_number++)
	{
		if (motor[motor_number].work==true)
		{
			rezult = openHole(motor_number);
			if (rezult < MAX_NUMBERS_TICKS)
			{	
				motor[motor_number].work=true;

				mount_tick = closeHole(motor_number);
					
				mount_tick += openHole(motor_number);
				
				motor[motor_number].max_count_tick=(int)(mount_tick>>1);
				motor[motor_number].current_percent=0;

				motorTurnOnPercent( motor_number ,  25);
			}	
			else 
			{
				motor[motor_number].work=false;

				motor[motor_number].max_count_tick=0;
				motor[motor_number].current_percent=0;
			}
		}
	}
}

/****************************************
* Name: motorTest
* Desc: Motor testing
*****************************************/
void motorTest(void)
{
	int index,mount_tick;
	bool rezult;
	uint8_t read_data;
	for (index = 0 ; index<NUMBERS_MOTOR ; index++)
	{
			turnOutMotorFromSenser(index);
	}
}


