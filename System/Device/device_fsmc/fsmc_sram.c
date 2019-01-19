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
  	/*FSMC NE3初试化，推挽复用输出*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
/*--------------FSMC 总线 存储器参数配置------------------------------*/
  	p.FSMC_AddressSetupTime = 0;           //地址建立时间    
  	p.FSMC_AddressHoldTime = 0;            //地址保持时间  
  	p.FSMC_DataSetupTime = 2;              //数据建立时间
  	p.FSMC_BusTurnAroundDuration = 0;      //总线恢复时间
  	p.FSMC_CLKDivision = 0;                // 时钟分频因子 
  	p.FSMC_DataLatency = 0;          	   //数据产生时间
  	p.FSMC_AccessMode =  FSMC_AccessMode_A; //FSMC NOR控制器时序
  	
/*--------------FSMC 总线 参数配置------------------------------*/
  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;                  //使用了FSMC的BANK1的子板块3             
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//禁止地址数据线复用
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;           //存储器类型为SRAM
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;  //存储器数据宽度为16位
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; //关闭突发模式访问
    //等待信号优先级，只有在使能突发访问模式才有效
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low; 
    //关闭Wrapped burst access mode，只有在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;         
    //等待信号设置，只有在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;     //使能这个BANK的写操作
    //使能/关闭等待信息设置，只在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;     
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; //关闭Extend Mode
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;     //关闭Write Burst Mode   
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;               //读操作时序参数
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                   //写操作时序参数

  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

/*--------------使能BANK1的子板块3------------------------------*/
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
