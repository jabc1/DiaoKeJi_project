/*��������������������������������������������������������������������������������������������������������������������������������
* �ļ���		: fsmc.c
* �汾��    : V1.0.0
* ����      : 2014/10/08
* ˵��      : �������LED�����������I��III������use_config�ж���													  
* �÷�			��
ʹ��SPI�Ļ��������ȳ�ʼ����
SPI1_Init();
SPI2_Init();
SPI1_SetSpeed(SpeedSet)������SPI1���ٶ�
SPI2_SetSpeed(SpeedSet)������SPI2���ٶ�
RxData = SPI1_ReadWriteByte(TxData); ��SPI1�������ݶ�д
RxData = SPI2_ReadWriteByte(TxData); ��SPI2�������ݶ�д
������������������������������������������������������������������������������������������������������������������������������������*/

/* Includes ------------------------------------------------------------------*/
#include "fsmc.h"


void FSMC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*FSMC����ʹ�õ�GPIO��ʱ��ʹ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |		 RCC_APB2Periph_GPIOA |
	                       RCC_APB2Periph_GPIOF, ENABLE);
	
	/*FSMC������FSMC_D[0:15]��ʼ�������츴�����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
	                              GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                              GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                              GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*FSMC��ַ��FSMC_A[0:17]��ʼ�������츴�����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                              GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
	                              GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                              GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*FSMC NOE��NWE���Ի������츴�����*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	/*FSMC NE3��ʼ�������츴�����*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/*FSMC NE2��ʼ�������츴�����*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	/*FSMC NBL0��NBL1��ʼ�������츴�����*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}




