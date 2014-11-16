

#include "stm32f4xx.h"

#include "Types.h"

typedef  uint8_t fpga_size_byte;
typedef vu8 bit_bus ;
typedef uint8_t bit_data;


/*USER TIM*/

#define TIM_MOTOR TIM3
#define RCC_TIM_MOTOR RCC_APB1Periph_TIM3

/* WIZNET SPI Settings*/
#define WIZ_SPI_PORT_RCC			RCC_APB1Periph_SPI2
#define WIZ_PORT_SETTINGS			GPIOE
#define WIZ_PWDN		        	GPIO_Pin_1	// out
#define WIZ_RESET		        	GPIO_Pin_2	// out


#define WIZ_PORT_INT					GPIOE
#define WIZ_INT			        	GPIO_Pin_3  // in

#define WIZ_SPI 							SPI2
#define WIZ_SPI_PORT 					GPIOB
#define WIZ_SCS			  				GPIO_Pin_12	// out //nss
#define WIZ_SCLK							GPIO_Pin_13	// out
#define WIZ_MISO							GPIO_Pin_14	// in
#define WIZ_MOSI							GPIO_Pin_15	// out

#define GPIO_AF_SPI 					GPIO_AF_SPI2
#define WIZ_SCLK_AF 					GPIO_PinSource13
#define WIZ_MISO_AF 					GPIO_PinSource14
#define WIZ_MOSI_AF 					GPIO_PinSource15

/*USART settings*/	

#define USART									USART1
#define USART_CLK             RCC_APB2Periph_USART1
#define USART_PORT 						RCC_AHB1Periph_GPIOA 
#define USART_GPIO						GPIOA
#define USART_GPIO_RX					GPIO_Pin_10
#define USART_GPIO_TX					GPIO_Pin_9	
#define USART_AF           		GPIO_AF_USART1
#define USART_TX_SOURCE       GPIO_PinSource9
#define USART_RX_SOURCE       GPIO_PinSource10
#define USART_IRQ							USART1_IRQn

/*LED GPIO Settings*/

#define LED_GPIO_RCC 					RCC_AHB1Periph_GPIOD
#define LED_GPIO 							GPIOD
#define LED1									GPIO_Pin_2	// out

/*Interrut from FPGA*/

#define EXT_INT_FPGA_Channel 	EXTI9_5_IRQn
#define EXT_LINE_INT_FPGA 		EXTI_Line6

/*FSMC width bus*/

#define FSMC_8bit// FSMC_16bit

/*Settings for DMA FSMC*/

#define DMA_FSMC_STREAM               DMA2_Stream0
#define DMA_FSMC_CHANNEL              DMA_Channel_0
#define DMA_FSMC_STREAM_CLOCK         RCC_AHB1Periph_DMA2   
#define DMA_FSMC_STREAM_IRQ           DMA2_Stream0_IRQn
#define DMA_FSMC_IT_TC	              DMA_IT_TC
#define DMA_FSMC_IT_TCIF_CHECK 				DMA_IT_TCIF0

/*Settings for I2C*/


#define I2C_EE                       	I2C1
#define I2C_EE_RCC 				 						RCC_APB1Periph_I2C1
#define I2C_EE_RCC_PORT 							RCC_AHB1Periph_GPIOB
#define GPIO_EE_Pin_SCL 							GPIO_Pin_6
#define GPIO_EE_Pin_SDA 							GPIO_Pin_7
#define I2C_EE_PORT										GPIOB
#define I2C_EE_SCL_SOURCE							GPIO_PinSource6
#define I2C_EE_SDA_SOURCE							GPIO_PinSource7
#define I2C_EE_GPIO_AF								GPIO_AF_I2C1 




#define I2C                          	I2C2
#define I2C_RCC 											RCC_APB1Periph_I2C2
#define I2C_RCC_PORT 									RCC_AHB1Periph_GPIOB
#define GPIO_Pin_SCL 									GPIO_Pin_10
#define GPIO_Pin_SDA 									GPIO_Pin_11
#define I2C_PORT											GPIOB
#define I2C_SCL_SOURCE								GPIO_PinSource10
#define I2C_SDA_SOURCE								GPIO_PinSource11
#define I2C_GPIO_AF										GPIO_AF_I2C2 


/*Settings Interrupt for PCA9539*/

#define INT_PCA9539_1_GPIO						GPIO_Pin_2
#define INT_PCA9539_1_PORT						GPIOC
#define INT_PCA9539_1_RCC 						RCC_AHB1Periph_GPIOC
#define INT_PCA9539_1_IRQChannel			EXTI2_IRQn
#define INT_PCA9539_1_IRQLine 				EXTI_Line2
#define INT_PCA9539_1_PortSource      EXTI_PortSourceGPIOC
#define INT_PCA9539_1_PinSource 			EXTI_PinSource2

#define INT_PCA9539_2_GPIO						GPIO_Pin_3
#define INT_PCA9539_2_PORT						GPIOC
#define INT_PCA9539_2_RCC 						RCC_AHB1Periph_GPIOC
#define INT_PCA9539_2_IRQChannel			EXTI3_IRQn
#define INT_PCA9539_2_IRQLine 				EXTI_Line3
#define INT_PCA9539_2_PortSource      EXTI_PortSourceGPIOC
#define INT_PCA9539_2_PinSource 			EXTI_PinSource3


/*DMOT GPIO Settings*/

#define DMOT_GPIO_RCC 				RCC_AHB1Periph_GPIOC
#define DMOT_GPIO 						GPIOC
#define DMOT0									GPIO_Pin_0	// out
#define DMOT1									GPIO_Pin_1	// out
#define DMOT2									GPIO_Pin_2	// out
#define DMOT3									GPIO_Pin_3	// out

/*DCON GPIO Settings*/
#define DCON_GPIO_RCC 				RCC_AHB1Periph_GPIOC
#define DCON_GPIO 						GPIOC
#define DCON1									GPIO_Pin_6	// in
#define DCON2									GPIO_Pin_7	// in


/*Settings Reset for PCA9539*/
