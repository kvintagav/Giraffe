

/* Includes ------------------------------------------------------------------*/
#include "fsmc_fpga.h"

/* Private define ------------------------------------------------------------*/
#define FPGA_FLASH_START_ADDR     ((uint32_t)0x70000000) 
bit_bus control_reg;
bit_bus input_bufer[BUFFER_SIZE];
DMA_InitTypeDef    DMA_FSMC_InitStructure;


extern SemaphoreHandle_t xMutexFSMC;
extern SemaphoreHandle_t xSemaphoreEXTI;
extern SemaphoreHandle_t xSemaphoreFSMCDMA;
extern SemaphoreHandle_t xSemaphoreSPIDMA;

/*******************************************************************************
* Function Name  : FSMC_NAND_Init
* Description    : Configures the FSMC and GPIOs to interface with the NAND memory.
*                  This function must be called before any write/read operation
*                  on the NAND.
******************************************************************************/
void FSMC_FPGA_Init(void)
{
	
  GPIO_InitTypeDef GPIO_InitStructure; 
  FSMC_NANDInitTypeDef FSMC_FPGAInitStructure;
  FSMC_NAND_PCCARDTimingInitTypeDef  p;
	EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
  RCC_AHB1PeriphClockCmd(DMA_FSMC_STREAM_CLOCK, ENABLE);
	
   /*-- GPIO Configuration ------------------------------------------------------*/
  /* CLE, ALE, D0->D3, NOE, NWE and NCE2  NAND pin configuration  */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);	
	
  /* D0->D3,*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1 , GPIO_AF_FSMC);

   /* D4->D7 NAND pin configuration  */ 
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);



  /*-- GPIO NAND pin configuration  ------------------------------------------------------*/
  /*  D0->D3 NAND pin configuration  */
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* D4->D7 NAND pin configuration  */  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /*ALE and CLE */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12 , GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
 
  /*NOE and NWE*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5 , GPIO_AF_FSMC); 

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
	

  /* CS */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7 , GPIO_AF_FSMC);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_SetupTime = 0x01;
  p.FSMC_WaitSetupTime = 0x03;
  p.FSMC_HoldSetupTime = 0x02;
  p.FSMC_HiZSetupTime = 0x01;

  FSMC_FPGAInitStructure.FSMC_Bank = FSMC_Bank2_NAND;
  FSMC_FPGAInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Disable;
  FSMC_FPGAInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_FPGAInitStructure.FSMC_ECC = FSMC_ECC_Enable;
  FSMC_FPGAInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes;
  FSMC_FPGAInitStructure.FSMC_TCLRSetupTime = 0x15;
  FSMC_FPGAInitStructure.FSMC_TARSetupTime = 0x15;
  FSMC_FPGAInitStructure.FSMC_CommonSpaceTimingStruct = &p;
  FSMC_FPGAInitStructure.FSMC_AttributeSpaceTimingStruct = &p;

  FSMC_NANDInit(&FSMC_FPGAInitStructure);

  /* FSMC NAND Bank Cmd Test */
  FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
	
		/*NWAIT*/
 // GPIO_PinAFConfig(GPIOD, GPIO_PinSource6 , GPIO_AF_FSMC);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  
  /* Connect EXTI Line6 to PD6 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);

  /* Configure EXTI Line6 */
  EXTI_InitStructure.EXTI_Line = EXT_LINE_INT_FPGA ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Enable and set EXTI Line6 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXT_INT_FPGA_Channel ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA_FSMC_STREAM);
	
	

	/* Settings DMA for read bufer */
	DMA_FSMC_InitStructure.DMA_Channel = DMA_FSMC_CHANNEL;  
  DMA_FSMC_InitStructure.DMA_PeripheralBaseAddr = FPGA_FLASH_START_ADDR;
  DMA_FSMC_InitStructure.DMA_Memory0BaseAddr = (uint32_t)input_bufer;
  DMA_FSMC_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
  DMA_FSMC_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
  DMA_FSMC_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_FSMC_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_FSMC_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_FSMC_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
  DMA_FSMC_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_FSMC_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_FSMC_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_FSMC_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_FSMC_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_FSMC_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_FSMC_STREAM, &DMA_FSMC_InitStructure);
	
	 DMA_ITConfig(DMA_FSMC_STREAM, DMA_FSMC_IT_TC, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = DMA_FSMC_STREAM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}



/******************************************************************************
* Function Name  : FSMC_NAND_Detect
* Description    : Detect FPGA 
*******************************************************************************/
bool FSMC_FPGA_Detect(void)
{
	uint16_t  status = 0x00;

  *(bit_bus *)(FPGA_FLASH_START_ADDR | CMD_AREA) = FPGA_CMD_DETECT;
	status = *(bit_bus *)(FPGA_FLASH_START_ADDR | DATA_AREA);
	if (status==FPGA_DETECT) return(TRUE);
	else  return (FALSE);
	
	return(FALSE);
}
/******************************************************************************
* Function Name  : FSMC_NAND_Reset
* Description    : This routine reset the NAND FLASH
*******************************************************************************/
void FSMC_FPGA_Reset(void)
{
  *(bit_bus *)(FPGA_FLASH_START_ADDR | CMD_AREA) = FPGA_CMD_RESET;

 
}
/******************************************************************************
* Function Name  : FSMC_FPGA_GetBufer
* Description    : Command from Get bufer from fpga 
*******************************************************************************/

void FSMC_FPGA_GetBufer(void)
{
  *(bit_bus *)(FPGA_FLASH_START_ADDR | CMD_AREA) = FPGA_CMD_READ_BUFER;
}

/******************************************************************************
* Function Name  : FSMC_START_CALC
* Description    : start calculation bufer 
*******************************************************************************/

bit_bus FSMC_START_CALC (void)
{
	
	*(bit_bus *)(FPGA_FLASH_START_ADDR | CMD_AREA) = CMD_WRITE_CONTROL_REG;
	*(bit_bus *)(FPGA_FLASH_START_ADDR | DATA_AREA) = control_reg;
	*(bit_bus *)(FPGA_FLASH_START_ADDR | CMD_AREA) = FPGA_CMD_START;
	control_reg++;
	
	return(0);
}
/******************************************************************************
* Function Name  : FSMC_NAND_GetStatus
* Description    : Get the NAND operation status
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation  
*******************************************************************************/
bit_bus FSMC_FPGA_GetStatus(void)
{
  bit_bus  status = 0;
	
	*(bit_bus *)(FPGA_FLASH_START_ADDR | CMD_AREA) = FPGA_CMD_STATUS;
	status = *(bit_bus *)(FPGA_FLASH_START_ADDR | DATA_AREA);
	
  return (status);      
}
