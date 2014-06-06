

#include "user_tasks.h"
#include "fsmc_fpga.h"
#include "main.h"
#include "console.h"
extern SemaphoreHandle_t xMutexFSMC;

extern SemaphoreHandle_t xSemaphoreEXTI;
extern SemaphoreHandle_t xSemaphoreFSMCDMA;
extern SemaphoreHandle_t xSemaphoreSPIDMA;
extern SemaphoreHandle_t xSemaphoreCONSOLE;
extern char bufer_console[SIZE_CONS_IN]; 
//char bufer_cons_out[SIZE_CONS_OUT];

extern bit_bus input_bufer[BUFFER_SIZE];

/******************************************************************************
* Function Name  : ConsoleExchange
* Description    : Processing command from debug (USART/TELNET?USB) console
*******************************************************************************/	
/*
void ConsoleExchange (void *pvParameters)
{
		xSemaphoreTake( xSemaphoreCONSOLE,mainDONT_BLOCK);
  	PrintVersion(bufer_cons_out);
		console_send(bufer_cons_out);
		console_send("\n>");	
		while(1)
    {
				xSemaphoreTake( xSemaphoreCONSOLE,portMAX_DELAY);
				CommandProcessing( bufer_console,  bufer_cons_out);
	
				console_send(bufer_cons_out);
				console_send("\n>");
		
		}
    vTaskDelete(NULL);	
}
*/
/******************************************************************************
* Function Name  : vLedTask
* Description    : Change state led with period 1 c
*******************************************************************************/	
void vLedTask (void *pvParameters)
{
    while(1)
    {
			ChangeLED();
			
			vTaskDelay(PERIOD_LED_TASK);
		}
    vTaskDelete(NULL);	
}
/******************************************************************************
* Function Name  : StartGetBuferFPGA
* Description    : Get bufer 2KB from FPGA to memory with DMA
*******************************************************************************/

void ProcessingIntFPGA(void *pvParameters)
{
//	int i=1000;
	
	/* Take the semaphore before started to ensure it is in the correct	state. */
	xSemaphoreTake( xSemaphoreEXTI, mainDONT_BLOCK );
	xSemaphoreTake( xSemaphoreFSMCDMA,mainDONT_BLOCK);

	while(1)
  {
			/*Take Semaphore EXT Interrupt NWAIT from FPGA */
			xSemaphoreTake( xSemaphoreEXTI,portMAX_DELAY);
			
			
			/*Take mutex for use FSMC*/
			xSemaphoreTake(xMutexFSMC , portMAX_DELAY);
			if((FSMC_FPGA_GetStatus() & FPGA_READY_BUFER)==FPGA_READY_BUFER)
			{

				FSMC_FPGA_GetBufer();
				
				/*Enable DMA receive bufer from FPGA */
				
				DMA_Cmd(DMA_FSMC_STREAM, ENABLE);
				
				xSemaphoreTake( xSemaphoreFSMCDMA,portMAX_DELAY);
			}
			xSemaphoreGive(xMutexFSMC );
		}
    vTaskDelete(NULL);	
}



/******************************************************************************
* Function Name  : TCP_IPConnect
* Description    :	Installation and maintenance of connection to the server
*******************************************************************************/
void TCP_IPConnect(void *pvParameters)
{
	while(1)
	{
	//	check_socket(SOCKET_RESEACH,);
		
		vTaskDelay(TCP_IPConnect_PERIOD);
	}
	vTaskDelete(NULL);
}

/******************************************************************************
* Function Name  : StartCalcBuferFPGA
* Description    : Start generation bufer in fpga with period 1s
*******************************************************************************/

void StartCalcBuferFPGA(void *pvParameters)
{
	xSemaphoreTake(xMutexFSMC , portMAX_DELAY);
	FSMC_FPGA_Reset();
	xSemaphoreGive(xMutexFSMC );
	while(1)
    {
			xSemaphoreTake(xMutexFSMC , portMAX_DELAY);
			FSMC_START_CALC();
			xSemaphoreGive(xMutexFSMC );
			vTaskDelay(START_CAL_PERIOD);
		}
    vTaskDelete(NULL);	
}
