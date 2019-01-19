/*————————————————————————————————————————————————————————————————
* 文件名		: fsmc.c
* 版本号    : V1.0.0
* 日期      : 2014/10/08
* 说明      : 开发板的LED驱动，具体分I和III，需在use_config中定义													  
* 用法			：
使用SPI的话，必须先初始化：
SPI1_Init();
SPI2_Init();
SPI1_SetSpeed(SpeedSet)；设置SPI1的速度
SPI2_SetSpeed(SpeedSet)；设置SPI2的速度
RxData = SPI1_ReadWriteByte(TxData); 对SPI1进行数据读写
RxData = SPI2_ReadWriteByte(TxData); 对SPI2进行数据读写
——————————————————————————————————————————————————————————————————*/

/* Includes ------------------------------------------------------------------*/
#include "fsmc.h"


void FSMC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*FSMC总线使用的GPIO组时钟使能*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |		 RCC_APB2Periph_GPIOA |
	                       RCC_APB2Periph_GPIOF, ENABLE);
	
	/*FSMC数据线FSMC_D[0:15]初始化，推挽复用输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
	                              GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                              GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                              GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*FSMC地址线FSMC_A[0:17]初始化，推挽复用输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                              GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
	                              GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                              GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*FSMC NOE和NWE初试化，推挽复用输出*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	/*FSMC NE3初始化，推挽复用输出*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/*FSMC NE2初始化，推挽复用输出*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	/*FSMC NBL0和NBL1初始化，推挽复用输出*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}




