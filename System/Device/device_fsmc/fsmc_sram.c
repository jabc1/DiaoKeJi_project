/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_sram.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file provides a set of functions needed to drive the 
*                      IS61WV51216BLL SRAM memory mounted on STM3210E-EVAL board.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "fsmc_sram.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Bank1_SRAM3_ADDR    ((u32)0x68000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : FSMC_SRAM_Init
* Description    : Configures the FSMC and GPIOs to interface with the SRAM memory.
*                  This function must be called before any write/read operation
*                  on the SRAM.
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  p;
  	GPIO_InitTypeDef GPIO_InitStructure; 
  	/*FSMC NE3���Ի������츴�����*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
/*--------------FSMC ���� �洢����������------------------------------*/
  	p.FSMC_AddressSetupTime = 0;           //��ַ����ʱ��    
  	p.FSMC_AddressHoldTime = 0;            //��ַ����ʱ��  
  	p.FSMC_DataSetupTime = 2;              //���ݽ���ʱ��
  	p.FSMC_BusTurnAroundDuration = 0;      //���߻ָ�ʱ��
  	p.FSMC_CLKDivision = 0;                // ʱ�ӷ�Ƶ���� 
  	p.FSMC_DataLatency = 0;          	   //���ݲ���ʱ��
  	p.FSMC_AccessMode =  FSMC_AccessMode_A; //FSMC NOR������ʱ��
  	
/*--------------FSMC ���� ��������------------------------------*/
  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;                  //ʹ����FSMC��BANK1���Ӱ��3             
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//��ֹ��ַ�����߸���
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;           //�洢������ΪSRAM
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;  //�洢�����ݿ��Ϊ16λ
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; //�ر�ͻ��ģʽ����
    //�ȴ��ź����ȼ���ֻ����ʹ��ͻ������ģʽ����Ч
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low; 
    //�ر�Wrapped burst access mode��ֻ����ʹ��ͻ������ģʽ����Ч
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;         
    //�ȴ��ź����ã�ֻ����ʹ��ͻ������ģʽ����Ч
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;     //ʹ�����BANK��д����
    //ʹ��/�رյȴ���Ϣ���ã�ֻ��ʹ��ͻ������ģʽ����Ч
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;     
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; //�ر�Extend Mode
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;     //�ر�Write Burst Mode   
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;               //������ʱ�����
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                   //д����ʱ�����

  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

/*--------------ʹ��BANK1���Ӱ��3------------------------------*/
  	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_WriteBuffer
* Description    : Writes a Half-word buffer to the FSMC SRAM memory. 
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr : SRAM memory internal address from which the data 
*                    will be written.
*                  - NumHalfwordToWrite : number of half-words to write. 
*                    
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(u16 *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr += 2;
  }   
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_ReadBuffer
* Description    : Reads a block of data from the FSMC SRAM memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the SRAM memory.
*                  - ReadAddr : SRAM memory internal address to read from.
*                  - NumHalfwordToRead : number of half-words to read.
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(vu16*) (Bank1_SRAM3_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr += 2;
  }  
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
