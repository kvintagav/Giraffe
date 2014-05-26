#include "stm32f4xx.h"
#include "config.h"
#include "socket.h"
#include "w5200.h"
#include "util.h"
#include "main.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "spi2.h"	

void WIZ_SPI_Init(void)
	{
		  SPI_InitTypeDef   SPI_InitStructure;
		  SPI_I2S_DeInit(WIZ_SPI);

		  /* SPI Config -------------------------------------------------------------*/
		  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		  SPI_InitStructure.SPI_CRCPolynomial = 7;
		  SPI_Init(WIZ_SPI, &SPI_InitStructure);
		  /* Enable SPI1 */
		  SPI_Cmd(WIZ_SPI, ENABLE);
		  SPI_NSSInternalSoftwareConfig(WIZ_SPI, SPI_NSSInternalSoft_Set);
	}


void WIZ_CS(uint8_t val)
{
	if (val == LOW) {
   		GPIO_ResetBits(WIZ_SPI_PORT, WIZ_SCS); 
	}else if (val == HIGH){
   		GPIO_SetBits(WIZ_SPI_PORT, WIZ_SCS); 
	}
}


void SPI_WIZ_WriteByte_Only(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(WIZ_SPI, SPI_I2S_FLAG_TXE) == RESET);        
	SPI_I2S_SendData(WIZ_SPI, byte);
}

uint8_t SPI_WIZ_ReadByte_Only( void)
{
	  while (SPI_I2S_GetFlagStatus(WIZ_SPI, SPI_I2S_FLAG_RXNE) == RESET);         
	  return SPI_I2S_ReceiveData(WIZ_SPI);
}

void wait_translate(void)
{
  while (SPI_I2S_GetFlagStatus(WIZ_SPI, SPI_I2S_FLAG_TXE) == RESET);         
}	

uint8_t SPI_WIZ_SendByte(uint8_t byte)
{
	  while (SPI_I2S_GetFlagStatus(WIZ_SPI, SPI_I2S_FLAG_TXE) == RESET);        
	  SPI_I2S_SendData(WIZ_SPI, byte);        
	  while (SPI_I2S_GetFlagStatus(WIZ_SPI, SPI_I2S_FLAG_RXNE) == RESET);         
	  return SPI_I2S_ReceiveData(WIZ_SPI);
}

