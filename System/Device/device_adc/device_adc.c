/*********************************************************************************
* 文件名			: device_ADC.c
* 版本号    	: V2.1.1
* 作者			: ZZ
* 日期      : 2015/09/29
* 说明      : 开发板的ADC驱动; RCC_A , GPIO_A , GPIO_Pin_0在drive.h中定义  
* 用法			：
		u8 ADCChannel[]={13,2,5,4};
		InitializeADC1Channel(ADCChannel,4,ADC_SampleTime_55Cycles5,(u32*)&App.Data.ADCValue[0]);	

	V2.1.1	
		针对MSOS将原来的ADC.H头文件取消了。
		1.使用公共的drive.h里定义IO公共宏定义
		2.通过system.h里定义的System变量来实现函数的传递
	V2.1.0
		实现了通过数组来定义ADC通道的数量和IO
	V1.0.0
		实现了DMA传送数据
**********************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "system.h"
#include "device_ADC.h"

#define DR_ADDRESS                  ((uint32_t)0x4001244C)
__IO uint16_t ADCConvertedValue;     // ADC为12位模数转换器，只有ADCConvertedValue的低12位有效
typedef struct 	   //AD端口数据类型定义 AD转换顺序为数组的顺序，
{
	u32 Rcc;						//AD通道的RCC 如 RCC_A
	u32 Port;						//AD通道的端口如 GPIO_A , 
	u16 Pin;						//AD通道的端口如 GPIO_Pin_0
} ADCIO_TypeDef ;
const ADCIO_TypeDef ADCIO[]={								//定义ADC通道的IO端口	
	{RCC_A , GPIO_A , GPIO_Pin_0,},						/*ADChanne0*/
	{RCC_A , GPIO_A , GPIO_Pin_1,},
	{RCC_A , GPIO_A , GPIO_Pin_2,},
	{RCC_A , GPIO_A , GPIO_Pin_3,},
	{RCC_A , GPIO_A , GPIO_Pin_4,},
	{RCC_A , GPIO_A , GPIO_Pin_5,},
	{RCC_A , GPIO_A , GPIO_Pin_6,},
	{RCC_A , GPIO_A , GPIO_Pin_7,},
	{RCC_B , GPIO_B , GPIO_Pin_0,},
	{RCC_B , GPIO_B , GPIO_Pin_1,},						/*ADChanne9*/
	{RCC_C , GPIO_C , GPIO_Pin_0,},						/*ADChanne10*/
	{RCC_C , GPIO_C , GPIO_Pin_1,},
	{RCC_C , GPIO_C , GPIO_Pin_2,},
	{RCC_C , GPIO_C , GPIO_Pin_3,},
	{RCC_C , GPIO_C , GPIO_Pin_4,},
	{RCC_C , GPIO_C , GPIO_Pin_5,},						/*ADChanne15*/
};

/********************************************
函数：InitializeADC1Channel
功能：初始化ADC1的转换通道
参数：ADCChannel：ADC通道的数组；Num：ADC的数量；ADC_SampleTime：ADC转换的速度;ADCValue:数据存储地址指针
返回：无
说明: 
********************************************/
void InitializeADC1Channel(u8 *ADCChannel,u8 Num,u8 ADC_SampleTime,u32 *ADCValue)		//暂时设定为2个通道
{	
	u8 i;
	ADC_InitTypeDef ADC_InitStructure;        //ADC初始化结构体声明
	GPIO_InitTypeDef GPIO_InitStructure;	
	DMA_InitTypeDef DMA_InitStructure;        //DMA初始化结构体声明	
	
/*ADC1 对应的DMA 初始化程序*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //使能DMA时钟
/* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);		  																							//开启DMA1的第一通道
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;		  							//DMA对应的外设基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCValue;   									//内存存储基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;														//DMA的转换模式为SRC模式，由外设搬移到内存
  DMA_InitStructure.DMA_BufferSize = Num;		   																	//DMA缓存大小，1个	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;							//接收一次数据后，设备地址禁止后移
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;												//接收一次数据后，目标内存地址后移	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  	//定义外设数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  					//DMA搬移数据尺寸，HalfWord就是为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   														//转换模式，循环缓存模式。
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;														//DMA优先级高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  														//M2M模式禁用
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);	
/*ADC1 对应的GPIO 初始化程序*/		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	for (i=0;i<Num;i++){																									//设置GPIO口
		RCC_APB2PeriphClockCmd(ADCIO[ADCChannel[i]].Rcc, ENABLE);
	  GPIO_InitStructure.GPIO_Pin = ADCIO[ADCChannel[i]].Pin;
	  GPIO_Init(((GPIO_TypeDef *)ADCIO[ADCChannel[i]].Port), &GPIO_InitStructure);
	}

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);	  											//使能ADC时钟	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   																					//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	ADC_DeInit(ADC1);  																														//复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;														//独立的转换模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  														//开启扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   													//开启连续转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;						//ADC外部开关，关闭状态
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   											//对齐方式,ADC为12位中，右对齐方式
  ADC_InitStructure.ADC_NbrOfChannel = Num;	 																		//开启通道数N个
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channel13 configuration */ 
	for (i=0;i<Num;i++)																														//设置ADC通道顺序
	{
		ADC_RegularChannelConfig(ADC1, ADCChannel[i], i+1, ADC_SampleTime);
	}
  ADC_DMACmd(ADC1, ENABLE);	  																									//ADC命令，使能
  ADC_Cmd(ADC1, ENABLE); 	 																											//开启ADC1
  ADC_ResetCalibration(ADC1);	  																								//重新校准
  while(ADC_GetResetCalibrationStatus(ADC1));  																	//等待重新校准完成
  ADC_StartCalibration(ADC1);																										//开始校准
  while(ADC_GetCalibrationStatus(ADC1));	   																		//等待校准完成
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);																				//连续转换开始，ADC通过DMA方式不断的更新RAM区。
}



