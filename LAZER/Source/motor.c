
#include "motor.h"

MOTOR_STATE motor[MOUNT_DRIVERS];

bool I2CWaitEvent( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
bool motorSendI2C(uint8 address, uint8 cmd ,uint8 data );

bool motorRecvI2C(uint8 address, uint8 cmd ,uint8 *data );
bool motorPower(int motor_number, bool power);

void init_motor(void)
{	
	int i;
	for(i = 0 ; i < MOUNT_DRIVERS ; i++)
	{
			if ((i==0) || (i==1)) 
			{
				motor[i].address_i2c=ADDRES_DRIVER_0;
				motor[i].port=0;
			}
			else 
			{
				motor[i].address_i2c=ADDRES_DRIVER_1;
				motor[i].port=1;
			}
		
			if ((i==0) || (i==3))	
			{
				motor[i].mask_enable=0xC0;
				motor[i].mask_senser=0x03;
			}
			else 
			{
				motor[i].mask_enable=0x03;
				motor[i].mask_senser=0xC0;
				
				
			}
			motorSendI2C(motor[i].address_i2c,CONFIGURATIONPORT+motor[i].port,motor[i].mask_senser);
	
	}


	
}

int motorTurn(int number,int direction, int corner )
{
	if (number>(MOUNT_DRIVERS-1)) return -1;
	motorPower(number, ENABLE);

	
	motorPower(number, DISABLE);
	return 1;
}

void motorTest(void)
{
	motorSendI2C(ADDRES_DRIVER_0,CONFIGURATIONPORT,0);
	motorSendI2C(ADDRES_DRIVER_0,CONFIGURATIONPORT+1,0);
	
	
	while(1)
	{
		motorSendI2C(ADDRES_DRIVER_0,OUTPUTPORT,255);
		motorSendI2C(ADDRES_DRIVER_0,OUTPUTPORT+1,255);
	}
}


bool motorSendI2C(uint8 address, uint8 cmd ,uint8 data )
{
  I2C_GenerateSTART(I2C, ENABLE);
	if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR ) return EE_ERROR;
 	
	I2C_Send7bitAddress(I2C, address, I2C_Direction_Transmitter);
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR ) return EE_ERROR;
  
	I2C_SendData(I2C,cmd); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
	I2C_SendData(I2C,data); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
  I2C_GenerateSTOP(I2C, ENABLE);
	
	return 1;
}

 c
{
	uint8 *read_data=data;
	
	I2C_GenerateSTART(I2C, ENABLE);
	if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR ) return EE_ERROR;
 	
	I2C_Send7bitAddress(I2C, address, I2C_Direction_Transmitter);
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR ) return EE_ERROR;
  
	I2C_SendData(I2C,cmd); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
	*read_data=I2C_ReceiveData(I2C); 
  if(I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR ) return EE_ERROR;
  
  I2C_GenerateSTOP(I2C, ENABLE);
	
	return 1;
}



bool motorPower(int motor_number, bool power)
{
	uint16  send_number=0;
	if (motor_number%2==0)
	{
		if (power==ENABLE)	send_number|=MASK_ENABLE_0 ;
	}
	else
	{
		if (power==ENABLE)	send_number|=MASK_ENABLE_1 ;
	}
//	return motorSendI2C(send_number , motor[motor_number].address_i2c);
	return TRUE;
}



