
#include "main.h"

#define FPGA_DETECT 0xA5

#define CMD_AREA                   	(uint32_t)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  	(uint32_t)(1<<17)  /* A17 = ALE high */

#define DATA_AREA                  ((uint32_t)0x00000000) 

#define FPGA_CMD_RESET							((uint8_t)0xFF)   
#define FPGA_CMD_STATUS							((uint8_t)0x70)   
#define FPGA_CMD_START							((uint8_t)0x03)
#define FPGA_CMD_DETECT							((uint8_t)0x04)


#define FPGA_CMD_READ_BUFER					((uint8_t)0x01)
#define FPGA_CMD_WRITE_BUFER				((uint8_t)0x80)
#define FPGA_CMD_ERASE_BUFER				((uint8_t)0x60)
#define CMD_READ_CONTROL_REG					((uint8_t)0x02)
#define CMD_WRITE_CONTROL_REG					((uint8_t)0x81)


#define CMD_WRITE_CONTROL_REG					((uint8_t)0x81)



/* FPGA memory status */
#define FPGA_WORK				         		((uint8_t)0x0001)
#define FPGA_READY_BUFER         		((uint8_t)0x0002)
#define FPGA_READ_BUFER         		((uint8_t)0x0004)




void START_DMA_FPGA(void);
void FSMC_FPGA_Init(void);
void FSMC_FPGA_Reset(void);
bit_bus FSMC_FPGA_GetStatus(void);
void FSMC_FPGA_GetBufer(void);
bit_bus FSMC_START_CALC(void);
bool FSMC_FPGA_Detect(void);

