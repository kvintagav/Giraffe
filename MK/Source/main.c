/*
    
*/

/******************************************************************************
 
 */


#include "main.h"
#include "fsmc_fpga.h"
#include "user_tasks.h"
#include "periphery_init.h"
#include "console.h"

/*Mutex*/
SemaphoreHandle_t xMutexFSMC= 				NULL;
SemaphoreHandle_t xMutexUSART_DEBUG=	NULL;
SemaphoreHandle_t xMutexSPI_WIZ=			NULL;

/*Semaphore*/
SemaphoreHandle_t xSemaphoreEXTI= 		NULL;
SemaphoreHandle_t xSemaphoreFSMCDMA= 	NULL;
SemaphoreHandle_t xSemaphoreSPIDMA= 	NULL;
SemaphoreHandle_t xSemaphoreCONSOLE= 	NULL;

/*Queue*/

#ifdef __GNUC__
  
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/***********************************************/
void vApplicationTickHook( void )
{
	
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */

	for( ;; );
}
/*-----------------------------------------------------------*/

/*******************************************************************/
void vFreeRTOSInitAll()
{
		LED_INIT();
	
		FSMC_FPGA_Init();

		CONSOLE_USART_INIT();
	
		#ifdef WIZNET
			WIZ_GPIO_Install();
			WIZ_Config();
		#endif
	
	
}
 
 
 

 
/*******************************************************************/
int main(void)
{
	xMutexFSMC = xSemaphoreCreateMutex();
	xMutexUSART_DEBUG = xSemaphoreCreateMutex();

	vSemaphoreCreateBinary(xSemaphoreFSMCDMA);
	vSemaphoreCreateBinary(xSemaphoreEXTI);
	vSemaphoreCreateBinary(xSemaphoreCONSOLE);
	
	#ifdef WIZNET
		xMutexSPI_WIZ =  xSemaphoreCreateMutex();
		vSemaphoreCreateBinary(xSemaphoreSPIDMA);
	#endif
		
  vFreeRTOSInitAll();
	
//	usart_send("\r\ndevice_start\r");

	
	if ((xMutexFSMC != NULL)&&(xSemaphoreEXTI !=NULL)&&(xSemaphoreFSMCDMA!=NULL)&&(xSemaphoreCONSOLE!=NULL))
	{
		xTaskCreate(GetBuferFPGA,(signed char*)"GetBuferFPGA", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+4 , NULL);
		xTaskCreate(StartCalcBuferFPGA,(signed char*)"StartCalcBufer", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3 , NULL);
		xTaskCreate(vLedTask,(signed char *)"LedTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2 , NULL);
	  xTaskCreate(ConsoleExchange,(signed char *)"ConsoleExchange", configMINIMAL_STACK_SIZE*3, NULL, tskIDLE_PRIORITY+2 , NULL);

		
		vTaskStartScheduler();
	}

	for( ;; );
}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART, USART_FLAG_TC) == RESET)
  {}

  return ch;
}