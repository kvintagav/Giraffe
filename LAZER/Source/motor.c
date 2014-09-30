
#include "motor.h"
#include "eeprom.h"
MOTOR_STATE motor[MOUNT_DRIVERS];

uint8 value ;
bool I2CWaitEventMotor( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void init_motor(void)
{	
	int i;
	for(i = 0 ; i < MOUNT_DRIVERS ; i++)
	{
			if ((i==0) || (i==1)) 	motor[i].address_i2c=ADDRES_DRIVER_0;
			else										motor[i].address_i2c=ADDRES_DRIVER_1;
			
			if ((i==0) || (i==2)) 	motor[i].port=0;				
			else										motor[i].port=1;
			
			if ((i==0) || (i==3))		motor[i].mask_enable=0xC0;
			else										motor[i].mask_enable=0x03;
			
			if ((i==0) || (i==3))		motor[i].mask_senser=0x03;
			else										motor[i].mask_senser=0xC0;
		
			motor[i].current_faza=0;
		
  		motorSendI2C(motor[i].address_i2c,CONFPORT+motor[i].port,0x00);
			motorSendI2C(motor[i].address_i2c,OUTPORT+motor[i].port,0x00);
		
	}
}


int motorTurn(int number,int direction, int tick )
{

	int i,j;
	uint8 address = motor[number].address_i2c;
	uint8 outport = OUTPORT+motor[number].port;
		 value = motor[number].mask_enable;
	if (number>(MOUNT_DRIVERS-1)) return -1;
	
	motorSendI2C(address,outport, value); //ENABLE
	
	for (i=0 ; i < tick  ; i++ )
	{
		switch (motor[number].current_faza)
		{
			case 0: 
				motor[number].current_faza=(direction) ?1 : 3; 
				value=(direction) ? FAZA1 : FAZA3;
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
				motor[number].current_faza=(direction) ?0 : 2; 
				value=(direction) ? FAZA0 : FAZA2;
			break;
			default:
				motor[number].current_faza=0; 
				value= FAZA0;
			break;
		}
		value|=motor[number].mask_enable;
		motorSendI2C(address , outport , value);
			
		for (j=0;j<10000000;j++){};
	}

		
	
	
	motorSendI2C(address,outport,0x00); //DISABLE
	return 1;
}

void motorTest(void)
{
/*Здесь можно писать свой код для тестирования*/
/*	uint8 koncevic1;
	uint8 koncevic2;
	
	
	motorSendI2C(ADDRES_DRIVER_0,CONFPORT,0);
	motorSendI2C(ADDRES_DRIVER_0,CONFPORT+1,0);

	while(1)
	{
		motorSendI2C(ADDRES_DRIVER_0,OUTPORT,255);
		motorSendI2C(ADDRES_DRIVER_0,OUTPORT+1,255);
		
		motorRecvI2C(ADDRES_DRI                         VER_0, INPORT , &koncevic1);
		motorRecvI2C(ADDRES_DRIVER_0, INPORT+1 , &koncevic2);
	}
	*/
	
	motorTurn(0,1,10 );
	/*
	motorSendI2C(ADDRES_DRIVER_1,CONFPORT,0);
	motorSendI2C(ADDRES_DRIVER_1,CONFPORT+1,0);
	
	motorSendI2C(ADDRES_DRIVER_0,OUTPORT,255);
	motorSendI2C(ADDRES_DRIVER_0,OUTPORT+1,255);
	*/
}

/****************************************
* Name:Send message to motor driver 
* Desc: address cmd data 
*****************************************/
bool motorSendI2C(uint8 address, uint8 cmd ,uint8 data )
{
	/*
	//Generate START
  I2C_GenerateSTART(I2C, ENABLE);
	if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR ) return EE_ERROR;
 	
	//Send Address
	I2C_Send7bitAddress(I2C, address, I2C_Direction_Transmitter); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR ) return EE_ERROR;
  
	//Send command
	I2C_SendData(I2C,cmd);
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
	//Send data 8 bit
	I2C_SendData(I2C,data); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
	//Generate Stop
  I2C_GenerateSTOP(I2C, ENABLE);
	
	return 1;
	*/
	 /* Send STRAT condition */
	 
	I2C_GenerateSTART(I2C, ENABLE);

		
  /* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)){};
	
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C, address, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){};
	
  /* Send the byte to be written */
  I2C_SendData(I2C,cmd); 
   
  /* Test on EV8 and clear it */
	
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
  
	 /* Send the byte to be written */
  I2C_SendData(I2C,data); 
   
  /* Test on EV8 and clear it */
	
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
 /* Send the byte to be written */
 // I2C_SendData(I2C,port1); 
   
  /* Test on EV8 and clear it */
	
 // while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
 
  /* Send STOP condition */
	I2C_GenerateSTOP(I2C, ENABLE);
	
	return NO_ERROR;
}

/****************************************
* Name:Recv message from motor driver 
* Desc: data - pointer on the data  
*****************************************/
bool motorRecvI2C(uint8 address, uint8 cmd ,uint8 *data )
{
	uint8 *read_data=data;
	//Generate start
	I2C_GenerateSTART(I2C, ENABLE);
	if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR ) return EE_ERROR;
 	
	//Send Address
	I2C_Send7bitAddress(I2C, address, I2C_Direction_Transmitter);
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR ) return EE_ERROR;
  
	//Send command
	I2C_SendData(I2C,cmd); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
	//Read data 
	*read_data=I2C_ReceiveData(I2C); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
	//Generate Stop
  I2C_GenerateSTOP(I2C, ENABLE);
	
	return 1;
}


/*******************************************************************************
* Function Name  : I2C_WaitEvent
* Description    : wait event from i2c
*******************************************************************************/
bool I2CWaitEventMotor( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	int eetime=WAYT_REQUEST_EEPROM;
	
  while(!I2C_CheckEvent(I2C, I2C_EVENT))
   {
    if((eetime--)==0) return EE_ERROR;
		
  }
	 
	return NO_ERROR;
	
}

