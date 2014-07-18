
#include "motor.h"

MOTOR_STATE motor[MOUNT_DRIVERS];

bool I2CWaitEvent( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);

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

bool motorSendI2C(uint16 number, uint8 address)
{
 /* Send STRAT condition */
  I2C_GenerateSTART(I2C, ENABLE);

  /* Test on EV5 and clear it */
	if (I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR) return EE_ERROR;
	
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
	if (I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR) return EE_ERROR;
	
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C, address);
  
  /* Test on EV8 and clear it */
  if (I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR) return EE_ERROR;
	
  /* Send the byte to be written */
  I2C_SendData(I2C,number); 
   
  /* Test on EV8 and clear it */
	
  if (I2CWaitEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR) return EE_ERROR;
  
	
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C, ENABLE);
	
	return NO_ERROR;
	return TRUE;
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
	return motorSendI2C(send_number , motor[motor_number].address_i2c);
}

/*******************************************************************************
* Function Name  : I2C_WaitEvent
* Description    : wait event from i2c
*******************************************************************************/
bool I2CWaitEvent( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	int eetime=WAYT_REQUEST_EEPROM;
	
  while(!I2C_CheckEvent(I2C, I2C_EVENT))
   {
    if(eetime==0) return EE_ERROR;
		else (eetime--);
  }
	 
	return NO_ERROR;
	
}

