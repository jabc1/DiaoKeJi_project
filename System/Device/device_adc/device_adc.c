/*********************************************************************************
* �ļ���			: device_ADC.c
* �汾��    	: V2.1.1
* ����			: ZZ
* ����      : 2015/09/29
* ˵��      : �������ADC����; RCC_A , GPIO_A , GPIO_Pin_0��drive.h�ж���  
* �÷�			��
		u8 ADCChannel[]={13,2,5,4};
		InitializeADC1Channel(ADCChannel,4,ADC_SampleTime_55Cycles5,(u32*)&App.Data.ADCValue[0]);	

	V2.1.1	
		���MSOS��ԭ����ADC.Hͷ�ļ�ȡ���ˡ�
		1.ʹ�ù�����drive.h�ﶨ��IO�����궨��
		2.ͨ��system.h�ﶨ���System������ʵ�ֺ����Ĵ���
	V2.1.0
		ʵ����ͨ������������ADCͨ����������IO
	V1.0.0
		ʵ����DMA��������
**********************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "system.h"
#include "device_ADC.h"

#define DR_ADDRESS                  ((uint32_t)0x4001244C)
__IO uint16_t ADCConvertedValue;     // ADCΪ12λģ��ת������ֻ��ADCConvertedValue�ĵ�12λ��Ч
typedef struct 	   //AD�˿��������Ͷ��� ADת��˳��Ϊ�����˳��
{
	u32 Rcc;						//ADͨ����RCC �� RCC_A
	u32 Port;						//ADͨ���Ķ˿��� GPIO_A , 
	u16 Pin;						//ADͨ���Ķ˿��� GPIO_Pin_0
} ADCIO_TypeDef ;
const ADCIO_TypeDef ADCIO[]={								//����ADCͨ����IO�˿�	
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
������InitializeADC1Channel
���ܣ���ʼ��ADC1��ת��ͨ��
������ADCChannel��ADCͨ�������飻Num��ADC��������ADC_SampleTime��ADCת�����ٶ�;ADCValue:���ݴ洢��ַָ��
���أ���
˵��: 
********************************************/
void InitializeADC1Channel(u8 *ADCChannel,u8 Num,u8 ADC_SampleTime,u32 *ADCValue)		//��ʱ�趨Ϊ2��ͨ��
{	
	u8 i;
	ADC_InitTypeDef ADC_InitStructure;        //ADC��ʼ���ṹ������
	GPIO_InitTypeDef GPIO_InitStructure;	
	DMA_InitTypeDef DMA_InitStructure;        //DMA��ʼ���ṹ������	
	
/*ADC1 ��Ӧ��DMA ��ʼ������*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //ʹ��DMAʱ��
/* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);		  																							//����DMA1�ĵ�һͨ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;		  							//DMA��Ӧ���������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCValue;   									//�ڴ�洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;														//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
  DMA_InitStructure.DMA_BufferSize = Num;		   																	//DMA�����С��1��	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;							//����һ�����ݺ��豸��ַ��ֹ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;												//����һ�����ݺ�Ŀ���ڴ��ַ����	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  	//�����������ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  					//DMA�������ݳߴ磬HalfWord����Ϊ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   														//ת��ģʽ��ѭ������ģʽ��
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;														//DMA���ȼ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  														//M2Mģʽ����
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);	
/*ADC1 ��Ӧ��GPIO ��ʼ������*/		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	for (i=0;i<Num;i++){																									//����GPIO��
		RCC_APB2PeriphClockCmd(ADCIO[ADCChannel[i]].Rcc, ENABLE);
	  GPIO_InitStructure.GPIO_Pin = ADCIO[ADCChannel[i]].Pin;
	  GPIO_Init(((GPIO_TypeDef *)ADCIO[ADCChannel[i]].Port), &GPIO_InitStructure);
	}

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);	  											//ʹ��ADCʱ��	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   																					//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	ADC_DeInit(ADC1);  																														//��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;														//������ת��ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  														//����ɨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   													//��������ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;						//ADC�ⲿ���أ��ر�״̬
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   											//���뷽ʽ,ADCΪ12λ�У��Ҷ��뷽ʽ
  ADC_InitStructure.ADC_NbrOfChannel = Num;	 																		//����ͨ����N��
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channel13 configuration */ 
	for (i=0;i<Num;i++)																														//����ADCͨ��˳��
	{
		ADC_RegularChannelConfig(ADC1, ADCChannel[i], i+1, ADC_SampleTime);
	}
  ADC_DMACmd(ADC1, ENABLE);	  																									//ADC���ʹ��
  ADC_Cmd(ADC1, ENABLE); 	 																											//����ADC1
  ADC_ResetCalibration(ADC1);	  																								//����У׼
  while(ADC_GetResetCalibrationStatus(ADC1));  																	//�ȴ�����У׼���
  ADC_StartCalibration(ADC1);																										//��ʼУ׼
  while(ADC_GetCalibrationStatus(ADC1));	   																		//�ȴ�У׼���
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);																				//����ת����ʼ��ADCͨ��DMA��ʽ���ϵĸ���RAM����
}



