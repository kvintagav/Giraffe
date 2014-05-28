/******************** (C) COPYRIGHT 2014 Kazakov Andrey ********************
* File Name          : eeprom.h
* Author             : Kazakov Andrey
* Version            : V1.0
* Date               : 28/05/2014
* Description        : 
*
*******************************************************************************/

#include "main.h"

#define I2C_TIMEOUT_MAX 		0xffff
#define I2C_PageSize        16
void I2C_EE_INIT(void);
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);

