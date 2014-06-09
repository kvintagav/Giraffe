#ifndef __SPI2_H
#define __SPI2_H

#include "stm32f4xx.h"

void WIZ_SPI_Init(void);
void WIZ_CS(uint8_t val);

uint8_t SPI_WIZ_SendByte(uint8_t byte);
void SPI_WIZ_WriteByte_Only(uint8_t byte);
uint8_t SPI_WIZ_ReadByte_Only( void);
void wait_translate(void);
//void SPI1_TXByte(uint8_t byte);

#endif

