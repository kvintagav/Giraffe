#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"

#define FLASH_USER_START_ADDR   ((uint32_t)0x080E0000)   
#define FLASH_USER_END_ADDR     ((uint32_t)0x080F0000)  

#define FLASH_WRITE_SUCCESS               0
#define FLASH_ERR_OUT_OF_RANGE           -1
#define FLASH_ERR_WRITE_FAILED           -2
#define FLASH_ERR_ERASE_FAILED           -3
#define FLASH_ERR_DATA_VERIFY_FAILED     -4

int FlashWrite( u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
int FlashRead( u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

#endif

