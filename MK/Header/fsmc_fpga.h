
#include "main.h"



#define CMD_AREA                   	(uint32_t)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  	(uint32_t)(1<<17)  /* A17 = ALE high */

#define DATA_AREA                  ((uint32_t)0x00000000) 

#define FPGA_CMD_RESET							((uint8_t)0xFF)   
#define FPGA_CMD_STATUS							((uint8_t)0x70)   
#define FPGA_CMD_START							((uint8_t)0x03)


#define FPGA_CMD_READ_BUFER					((uint8_t)0x01)
#define FPGA_CMD_WRITE_BUFER				((uint8_t)0x80)
#define FPGA_CMD_ERASE_BUFER				((uint8_t)0x60)
#define CMD_READ_CONTROL_REG					((uint8_t)0x02)
#define CMD_WRITE_CONTROL_REG					((uint8_t)0x81)



#define FPGA_PAGE_SIZE 							((uint16_t)0x0800) /* 2 * 1024 bytes in memory area */
#define FPGA_CNTR_SIZE            ((uint16_t)0x0008)   /* 8 bytes in control area */

/* FSMC FPGA memory address computation */
#define ADDR_1st_CYCLE(ADDR)       (uint8_t)((ADDR)& 0xFF)               /* 1st addressing cycle */
#define ADDR_2nd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF00) >> 8)      /* 2nd addressing cycle */

/* FPGA memory status */
#define FPGA_READY_BUFER         		((uint32_t)0x00000001)
#define FPGA_INVALID_ADDRESS       	((uint32_t)0x00000200)
#define FPGA_TIMEOUT_ERROR         	((uint32_t)0x00000400)
#define FPGA_BUSY                  	((uint32_t)0x00000000)
#define FPGA_ERROR                 	((uint32_t)0x00000001)
#define FPGA_READY                 	((uint32_t)0x00000040)




void START_DMA_FPGA(void);
void FSMC_FPGA_Init(void);
void FSMC_FPGA_Reset(void);
uint32_t FSMC_FPGA_GetStatus(void);
void FSMC_FPGA_GetBufer(void);
uint32_t FSMC_START_CALC(void);
uint32_t FSMC_NAND_ReadBufer(bit_data *pBuffer,uint32_t NumByteToRead);
uint32_t FSMC_NAND_WriteBufer(bit_data *pBuffer, uint32_t NumByteToWrite);
bit_bus FSMC_NAND_ReadByte( bit_data ADDR);
uint32_t FSMC_NAND_WriteByte(bit_data BYTE, bit_data ADDR);
