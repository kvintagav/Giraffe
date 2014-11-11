/******************** (C) COPYRIGHT 2014 Kazakov Andrey ********************
* File Name          : eeprom.c
* Author             : Kazakov Andrey
* Version            : V1.0
* Date               : 28/05/2014
* Description        : 
*******************************************************************************/

#include "eeprom.h"

/* Privat function */
bool I2CWaitEvent( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);

/*********************************************
* Function Name  : I2C_EE_INIT
* Description    : init module I2C EEPROM
*********************************************/
void I2C_EE_INIT(void){
 
	GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as I2C_SDA and I2C_SCL
	I2C_InitTypeDef I2C_InitStruct; // this is for the I2C initilization
 

	/* enable APB1 peripheral clock for I2C*/
	RCC_APB1PeriphClockCmd(I2C_EE_RCC , ENABLE);
  RCC_AHB1PeriphClockCmd(I2C_EE_RCC_PORT , ENABLE); 
 
	GPIO_PinAFConfig(I2C_EE_PORT, I2C_EE_SDA_SOURCE, I2C_EE_GPIO_AF); //
	GPIO_PinAFConfig(I2C_EE_PORT, I2C_EE_SCL_SOURCE, I2C_EE_GPIO_AF);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_EE_Pin_SCL | GPIO_EE_Pin_SDA ; // Pins (I2C_SCL) and (I2C_SDA)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;// this defines the output type as open drain
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;// this activates the pullup resistors on the IO pins
	GPIO_Init(I2C_EE_PORT, &GPIO_InitStruct);// now all the values are passed to the GPIO_Init() 
 
	
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
	I2C_Init(I2C_EE,&I2C_InitStruct);
	I2C_Cmd(I2C_EE, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : Writes buffer of data to the I2C EEPROM.
*******************************************************************************/
bool I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      if (I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle)==EE_ERROR) return EE_ERROR;
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        if (I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize)==EE_ERROR) return EE_ERROR; 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        if (I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle)==EE_ERROR) return EE_ERROR;
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      if (I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle)==EE_ERROR) return EE_ERROR;
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        if (I2C_EE_PageWrite(pBuffer, WriteAddr, count)==EE_ERROR) return EE_ERROR;
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        if (I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize)==EE_ERROR) return EE_ERROR;
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        if (I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle)==EE_ERROR) return EE_ERROR;
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
	
	return NO_ERROR;
}

/*******************************************************************************
* Function Name  : I2C_EE_PageWrite
* Description    : Writes more than one byte to the EEPROM with a single WRITE
*                  cycle. The number of byte can't exceed the EEPROM page size.
*******************************************************************************/
bool I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
	
  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
 	if (I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR) return EE_ERROR;
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
	if (I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR) return EE_ERROR;
	
  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(I2C_EE, WriteAddr);  

  /* Test on EV8 and clear it */
	while(! I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR) return EE_ERROR;
	
  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C_EE, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
		while (!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR) return EE_ERROR;
	
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);
	
	return NO_ERROR;
}
/*******************************************************************************
* Function Name  : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
*******************************************************************************/
bool I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
{

  /* Send STRAT condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
	if (I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR) return EE_ERROR;
	
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
	if (I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR) return EE_ERROR;
	
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C_EE, WriteAddr);
  
  /* Test on EV8 and clear it */
  if (I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR) return EE_ERROR;
	
  /* Send the byte to be written */
  I2C_SendData(I2C_EE, *pBuffer); 
   
  /* Test on EV8 and clear it */
	
  if (I2CWaitEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR) return EE_ERROR;
  
	
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);
	
	return NO_ERROR;
}
/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
*******************************************************************************/
bool I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
	
  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
	if (I2CWaitEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR) return EE_ERROR;
   
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  if(I2CWaitEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==EE_ERROR) return EE_ERROR;
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C_EE, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C_EE, ReadAddr);  

  /* Test on EV8 and clear it */
	if (I2CWaitEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==EE_ERROR)return EE_ERROR;
     
	/* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
	if(I2CWaitEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT)==EE_ERROR ) return EE_ERROR;
  	 
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
	if(I2CWaitEvent(I2C_EE, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==EE_ERROR) return EE_ERROR;
   
	 
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C_EE, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C_EE, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C_EE);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C_EE, ENABLE);
	
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_EE_WaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
*******************************************************************************/
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C_EE, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C_EE, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C_EE, I2C_FLAG_AF);
}

/*******************************************************************************
* Function Name  : I2C_WaitEvent
* Description    : wait event from i2c
*******************************************************************************/
bool I2CWaitEvent( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	int eetime=WAYT_REQUEST_EEPROM;
	
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT))
   {
    if(eetime==0) return EE_ERROR;
		else (eetime--);
  }
	 
	return NO_ERROR;
	
}
