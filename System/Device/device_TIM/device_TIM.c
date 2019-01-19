/*******************************************************************************
* �ļ���		: timer.c
* �汾��    : V1.1.0
* ����      : 2014/10/09
* ˵��      : ��ʱ����ʱ��ʼ����T1��T8�赥�����壬T2-T5���Թ���	TIMx_Init								  
* �÷�			��

void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)   TIMx����ʱ����ţ�arr���Զ���װ��ֵ��psc��Ԥ��Ƶ��
void TIM2_Config(u16 arr,u16 psc)						arr���Զ���װ��ֵ��psc��Ԥ��Ƶ��
									  
*******************************************************************************/

#include "device_TIM.h"





void TIM2_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//ʱ������

 	TIM_TimeBaseStructure.TIM_Period = arr;        //װ��ֵ36M/3600=10khz
	TIM_TimeBaseStructure.TIM_Prescaler = psc;       //Ԥ��Ƶ(ʱ�ӷ�Ƶ)72M/(2)=36M
 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���
 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
 	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); //��ʼ����ʱ����ֵ

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //ѡ��TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ��

	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM2,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ʹ���ж�

	TIM_Cmd(TIM2,ENABLE);  //��TIM2

}



//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��

void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	if(TIMx == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�			
	}
	else if(TIMx == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�	
	}
	else if(TIMx == TIM4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�		
	}	
	#ifdef STM32F10X_HD
	else if(TIMx == TIM5){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�	
	}	
	#endif

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ClearFlag(TIMx,TIM_FLAG_Update); //�����ʱ���жϱ�־ 
	/* TIM IT enable */
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIMx, //TIM
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);



	TIM_Cmd(TIMx, ENABLE);  //ʹ��TIMx����
							 
}



















