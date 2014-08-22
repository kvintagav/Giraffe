
#include "motor.h"

MOTOR_STATE motor[MOUNT_DRIVERS];

bool I2CWaitEvent( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
bool motorSendI2C(uint8 address, uint8 cmd ,uint8 port0 ,uint8 port1);
bool motorPower(int motor_number, bool power);

void init_motor(void)
{	
	int i;
	for(i = 0 ; i < MOUNT_DRIVERS ; i++)
	{
			if ((i==0) || (i==1))
			{
				motor[i].address_i2c=ADDRES_DRIVER_0;
			}
			else if ((i==2) || (i==3))
			{
				motor[i].address_i2c=ADDRES_DRIVER_1;
			}
	}
}
void motorTest(void)
{
	motorSendI2C(ADDRES_DRIVER_0,OUTPUTPORT,0xA5,0xA5);
	
}

bool motorSendI2C(uint8 address, uint8 cmd ,uint8 port0 ,uint8 port1)
{
	
	while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY)){};
	
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
	
  while(!I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
  
	 /* Send the byte to be written */
  I2C_SendData(I2C,port0); 
   
  /* Test on EV8 and clear it */
	
  while(!I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
 /* Send the byte to be written */
  I2C_SendData(I2C,port1); 
   
  /* Test on EV8 and clear it */
	
  while(!I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
 
  /* Send STOP condition */
	I2C_GenerateSTOP(I2C, ENABLE);
	
	return NO_ERROR;
	
}

bool motorRecvI2C(uint16 number, uint8 address)
{

	return TRUE;
}

int motorTurn(int number,int direction, int corner )
{
	if (number>(MOUNT_DRIVERS-1)) return ERROR;
	motorPower(number, ENABLE);

	motorPower(number, DISABLE);
	return OK;
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



