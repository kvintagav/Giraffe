
#include "flash.h"
#include "main.h"

int FlashWrite( u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	/* Unlock the Flash *********************************************************/
	uint32_t t_addr=FLASH_USER_START_ADDR+WriteAddr;
	int i;
	uint8 byte;
	FLASH_Unlock();

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
	FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	
	if (t_addr==FLASH_USER_START_ADDR)
  { 
    if (FLASH_EraseSector(FLASH_Sector_11,VoltageRange_3)!=FLASH_COMPLETE)
    {
      FLASH_Lock();
      return  -1;
    }
  }

	 for (i=0;i<NumByteToWrite;i++)
  {  
    if (FLASH_ProgramByte(t_addr,pBuffer[i])!=FLASH_COMPLETE)
    {
      FLASH_Lock();
      return -1;
    }
    t_addr++;
  }  
    
  FLASH_Lock();

  t_addr=FLASH_USER_START_ADDR+WriteAddr;
  for (i=0;i<NumByteToWrite;i++)
  {
    byte=(*(__IO uint8_t*)t_addr); t_addr++; 
    if (pBuffer[i]!=byte) return -1;
  }  
    
  return NumByteToWrite;
}