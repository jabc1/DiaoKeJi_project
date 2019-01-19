#ifndef __DEVICE_SPI_H_
#define __DEVICE_SPI_H_

#include "stm32f10x.h" 

/**************************SPI的宏定义****************************************************/
	#define SPI1_PORT   GPIOA							// SPI_1的端口宏定义
	#define SPI1_SCK    GPIO_Pin_5
	#define SPI1_MISO   GPIO_Pin_6  
	#define SPI1_MOSI   GPIO_Pin_7

	#define GPIO_SPI2     GPIOB							// SPI_1的端口宏定义
	#define RCC_SPI2      RCC_APB2Periph_GPIOB
	#define SPI2_SCK      GPIO_Pin_13
	#define SPI2_MISO      GPIO_Pin_14
	#define SPI2_MOSI      GPIO_Pin_15

	#define GPIO_SPI3     GPIOB							// SPI_1的端口宏定义
	#define RCC_SPI3      RCC_APB2Periph_GPIOB
	#define SPI3_SCK      GPIO_Pin_3
	#define SPI3_MISO      GPIO_Pin_4
	#define SPI3_MOSI      GPIO_Pin_5

// SPI总线速度设置 
#define SPI_SPEED_2   0
#define SPI_SPEED_4   1
#define SPI_SPEED_8   2
#define SPI_SPEED_16  3
#define SPI_SPEED_32  4
#define SPI_SPEED_64  5
#define SPI_SPEED_128 6
#define SPI_SPEED_256 7




void SPIx_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);
void SPIx_Init(SPI_TypeDef* SPIx,u16 DataSize);
u8 SPIx_ReadWriteByte(SPI_TypeDef* SPIx,u16 TxData);














#endif



