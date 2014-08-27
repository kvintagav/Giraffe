/******************** (C) COPYRIGHT 2014 Kazakov Andrey ********************
* File Name          : eeprom.h
* Author             : Kazakov Andrey
* Version            : V1.0
* Date               : 28/05/2014
* Description        : 
*
*******************************************************************************/


#ifndef __EEPROM_H
#define __EEPROM_H

#include "main.h"

//#define I2C_TIMEOUT_MAX 		0xffff

#define EE_ERROR 0
#define NO_ERROR 1

void I2C_EE_INIT(void);
bool I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
bool I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
bool I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
bool I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);
bool I2CWaitEvent( I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);

#endif
