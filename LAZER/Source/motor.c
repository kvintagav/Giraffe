
#include "motor.h"
#include "eeprom.h"
MOTOR_STATE motor[MOUNT_DRIVERS];


bool motorSendI2C(uint8 address, uint8 cmd ,uint8 data );
bool motorRecvI2C(uint8 address, uint8 cmd ,uint8 *data );

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
			
			motorSendI2C(motor[i].address_i2c,CONFPORT+motor[i].port,motor[i].mask_senser);
	}
}

int motorTurn(int numb_motor,int direction, int corner )
{
//	uint8 address = motor[numb_motor].address_i2c;
//	uint8 port = motor[numb_motor].port;
	uint8 value = motor[numb_motor].mask_enable;
	
	if (numb_motor>(MOUNT_DRIVERS-1)) return -1;
	motorSendI2C(motor[numb_motor].address_i2c,OUTPORT+motor[numb_motor].port,value); //ENABLE
	
	
	
	motorSendI2C(motor[numb_motor].address_i2c,OUTPORT+motor[numb_motor].port,0x00); //DISABLE
	return 1;
}

void motorTest(void)
{
	motorSendI2C(ADDRES_DRIVER_0,CONFPORT,0);
	motorSendI2C(ADDRES_DRIVER_0,CONFPORT+1,0);

	while(1)
	{
		motorSendI2C(ADDRES_DRIVER_0,OUTPORT,255);
		motorSendI2C(ADDRES_DRIVER_0,OUTPORT+1,255);
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

bool motorRecvI2C(uint8 address, uint8 cmd ,uint8 *data )
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




