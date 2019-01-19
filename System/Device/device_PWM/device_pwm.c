/*��������������������������������������������������������������������������������������������������������������������������������
* �ļ���		: PWM.c
* �汾��    : V1.1.0
* ����      : 2015/6/28
* ˵��      : PWM�Ķ���											  
* �÷�			��

V1.1.0 
������TIM5��PWM����


������������������������������������������������������������������������������������������������������������������������������������*/
#include "device_pwm.h"
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef   TIM_OCInitStructure;
	TIM_BDTRInitTypeDef   TIM_BDTRInitStructure;
	//��һ��������ʱ��        
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);	
	//�ڶ���������goio��
	/********TIM1 ��������*********/    	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,  ENABLE);   // T1�����ض���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14; 
															//TIM1_CH1N   TIM1_CH1    TIM1_CH2N     TIM1_CH2    TIM1_CH3N     TIM1_CH3      TIM1_CH4	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
		//����������ʱ����������        
	TIM_TimeBaseStructure.TIM_Period=arr;             // �Զ���װ�ؼĴ�����ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;            // ʱ��Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//�ظ��Ĵ����������Զ�����pwmռ�ձ�    
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //���Ĳ�pwm�������
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;                    //����Ϊpwm2���ģʽ
	TIM_OCInitStructure.TIM_Pulse=0;                                 //����ռ�ձ�ʱ��
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;             //�����������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; 		
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //ʹ�ܸ�ͨ�����
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;			//
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;         //�������״̬Ϊ1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   //�������״̬Ϊ0 			
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 		
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure); 		
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
	

	//���岽��������ɲ���������ã��߼���ʱ�����еģ�ͨ�ö�ʱ����������
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;				//����ģʽ�����ѡ�� 
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			//����ģʽ�����ѡ�� 
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;      	 	  //��������
	TIM_BDTRInitStructure.TIM_DeadTime = 0x00;                  			 //����ʱ������
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 	//ɲ������ʹ��
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;		//ɲ�����뼫��
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ�� 
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);   

	//��������ʹ�ܶ˵Ĵ�
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1,ENABLE);                                                           
	//��������Ǹ߼���ʱ�����еģ����pwm�����
	TIM_CtrlPWMOutputs(TIM1, ENABLE);                 //pwm���ʹ�ܣ�һ��Ҫ�ǵô�

}
void TIM8_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
// NVIC_InitTypeDef NVIC_InitStructure;			 //�����ж����ݽṹ

	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef   TIM_OCInitStructure;
    TIM_BDTRInitTypeDef   TIM_BDTRInitStructure;
	
	 //��һ��������ʱ��        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	
	//�ڶ���������goio��
	/********TIM8_CH1 ��������    
TIM8_CH1N->PA7
TIM8_CH2N->PB0
TIM8_CH3N->PB1
TIM8_CH1->PC6
TIM8_CH2->PC7
TIM8_CH3->PC8
TIM8_CH4->PC9
*********/    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
															//TIM8_CH1N 	
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 ; 
															//TIM8_CH2N   TIM8_CH3N 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9; 
															//TIM8_CH1     TIM8_CH2   TIM8_CH3    TIM8_CH4	
  GPIO_Init(GPIOC, &GPIO_InitStructure);	

		//����������ʱ����������        
	TIM_TimeBaseStructure.TIM_Period=arr;             // �Զ���װ�ؼĴ�����ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;            // ʱ��Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//�ظ��Ĵ����������Զ�����pwmռ�ձ�    
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    //���Ĳ�pwm�������
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;                    //����Ϊpwm2���ģʽ
	TIM_OCInitStructure.TIM_Pulse=20;                                 //����ռ�ձ�ʱ��
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;             //�����������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; 		
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //ʹ�ܸ�ͨ�����
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;			//�رջ������------------
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;         //�������״̬Ϊ1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   //�������״̬Ϊ0 			
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 		
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); 		
//	TIM_OC4Init(TIM8, &TIM_OCInitStructure); 
	

	//���岽��������ɲ���������ã��߼���ʱ�����еģ�ͨ�ö�ʱ����������
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;				//����ģʽ�����ѡ�� 
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			//����ģʽ�����ѡ�� 
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;      	 	  //��������
    TIM_BDTRInitStructure.TIM_DeadTime = 0x00;                  			 //����ʱ������
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 	//ɲ������ʹ��
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;		//ɲ�����뼫��
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ�� 
    TIM_BDTRConfig(TIM8,&TIM_BDTRInitStructure);   

	//��������ʹ�ܶ˵Ĵ�
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���	
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���	
	TIM_ARRPreloadConfig(TIM8, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM8,ENABLE);                                                           
	//��������Ǹ߼���ʱ�����еģ����pwm�����
	TIM_CtrlPWMOutputs(TIM8, ENABLE);                 //pwm���ʹ�ܣ�һ��Ҫ�ǵô�

}


/***************************************************************************************
������TIM2_PWM_Init
���ܣ���TIM2 PWM���ܳ�ʼ��
������arr: �Զ���װֵ��psc: Ԥ��Ƶ��(��ÿ��������Ҫ��������)
���أ���
˵����1. PWM�Ƿ���������ض�����PWM.H�к궨��
			2. �����Ҫ����ͨ����PWM.H�к궨��
			3. ���������Ƶ��Ϊ��	72MH/arr/psc	��TIM2_PWM_Init(900,0);	 //PWMƵ��=72000/900=80Khz	
			4. �����ɶ�Ӧ��ͨ�����ĺ���					TIM_SetCompare1(TIM2,X); //�ı�ͨ��1PWMռ�ձ�	
																					TIM_SetCompare2(TIM2,X); //�ı�ͨ��2PWMռ�ձ�	
***************************************************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	#if 	TIM2_PartialRemap1 > 0					//TIM2ʹ�ò��ֶ˿��ض���1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��ʹ�� ����ʱ��ʹ�� ��
		GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);		 //TIM2����ӳ��1	
	#endif	
	#if  	TIM2_PartialRemap2 > 0					//TIM2ʹ�ò��ֶ˿��ض���1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��ʹ�� ����ʱ��ʹ�� ��
		GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);		 //TIM2����ӳ��1	
	#endif
	#if  	TIM2_FullRemap > 0					//TIM2ʹ����ȫ�˿��ض���
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��ʹ�� ����ʱ��ʹ�� ��
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);		 //TIM2��ȫӳ��
	#endif	
	#ifdef 	TIM2_USE_CH1					//TIM2ͨ��1
		RCC_APB2PeriphClockCmd(RCC_T2CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM2_USE_CH2					//TIM2ͨ��2
		RCC_APB2PeriphClockCmd(RCC_T2CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM2_USE_CH3					//TIM2ͨ��3
		RCC_APB2PeriphClockCmd(RCC_T2CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM2_USE_CH4					//TIM2ͨ��4
		RCC_APB2PeriphClockCmd(RCC_T2CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	#ifdef 	TIM2_USE_CH1					//TIM2ͨ��1
		TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM2_USE_CH2					//TIM2ͨ��2
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM2_USE_CH3					//TIM2ͨ��3
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif	
	#ifdef 	TIM2_USE_CH4					//TIM2ͨ��4
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����

}
/***************************************************************************************
������TIM3_PWM_Init
���ܣ���TIM3 PWM���ܳ�ʼ��
������arr: �Զ���װֵ��psc: Ԥ��Ƶ��(��ÿ��������Ҫ��������)
���أ���
˵����1. PWM�Ƿ���������ض�����PWM.H�к궨��
			2. �����Ҫ����ͨ����PWM.H�к궨��
			3. ���������Ƶ��Ϊ��	72MH/arr/psc	��TIM3_PWM_Init(900,0);	 //PWMƵ��=72000/900=80Khz	
			4. �����ɶ�Ӧ��ͨ�����ĺ���					TIM_SetCompare1(TIM3,X); //�ı�ͨ��1PWMռ�ձ�X	
																					TIM_SetCompare2(TIM3,X); //�ı�ͨ��2PWMռ�ձ�X	
***************************************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	#if  	TIM3_PartialRemap	 > 0				//TIM2ʹ�ò��ֶ˿��ض���1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��ʹ�� ����ʱ��ʹ�� ��
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);		 //TIM3����ӳ��1	
	#endif
	#if  	TIM3_FullRemap > 0					//TIM2ʹ����ȫ�˿��ض���
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��ʹ�� ����ʱ��ʹ�� ��
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);		 //TIM3��ȫӳ��
	#endif	
	#ifdef 	TIM3_USE_CH1					//TIM3ͨ��1
		RCC_APB2PeriphClockCmd(RCC_T3CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM3_USE_CH2					//TIM3ͨ��2
		RCC_APB2PeriphClockCmd(RCC_T3CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM3_USE_CH3					//TIM3ͨ��3
		RCC_APB2PeriphClockCmd(RCC_T3CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM3_USE_CH4					//TIM3ͨ��4
		RCC_APB2PeriphClockCmd(RCC_T3CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�	
	
	#ifdef 	TIM3_USE_CH1					//TIM3ͨ��1
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM3_USE_CH2					//TIM3ͨ��2
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM3_USE_CH3					//TIM3ͨ��3
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif	
	#ifdef 	TIM3_USE_CH4					//TIM3ͨ��4
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����

}
/***************************************************************************************
������TIM4_PWM_Init
���ܣ���TIM4 PWM���ܳ�ʼ��
������arr: �Զ���װֵ��psc: Ԥ��Ƶ��(��ÿ��������Ҫ��������)
���أ���
˵����1. PWM�Ƿ���������ض�����PWM.H�к궨��
			2. �����Ҫ����ͨ����PWM.H�к궨��
			3. ���������Ƶ��Ϊ��	72MH/arr/psc	��TIM4_PWM_Init(900,0);	 //PWMƵ��=72000/900=80Khz	
			4. �����ɶ�Ӧ��ͨ�����ĺ���					TIM_SetCompare1(TIM4,X); //�ı�ͨ��1PWMռ�ձ�	
																					TIM_SetCompare2(TIM4,X); //�ı�ͨ��2PWMռ�ձ�	
***************************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	#if  	TIM4_Remap > 0					//TIM2ʹ�ò��ֶ˿��ض���1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��ʹ�� ����ʱ��ʹ�� ��
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);		 //TIM4ӳ��	
	#endif

	#ifdef 	TIM4_USE_CH1					//TIM4ͨ��1
		RCC_APB2PeriphClockCmd(RCC_T4CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM4_USE_CH2					//TIM4ͨ��2
		RCC_APB2PeriphClockCmd(RCC_T4CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM4_USE_CH3					//TIM4ͨ��3
		RCC_APB2PeriphClockCmd(RCC_T4CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM4_USE_CH4					//TIM4ͨ��4
		RCC_APB2PeriphClockCmd(RCC_T4CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�	
	#ifdef 	TIM4_USE_CH1					//TIM4ͨ��1
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM4_USE_CH2					//TIM4ͨ��2
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM4_USE_CH3					//TIM4ͨ��3
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif	
	#ifdef 	TIM4_USE_CH4					//TIM4ͨ��4
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����

}

/***************************************************************************************
������TIM4_PWM_Init
���ܣ���TIM4 PWM���ܳ�ʼ��
������arr: �Զ���װֵ��psc: Ԥ��Ƶ��(��ÿ��������Ҫ��������)
���أ���
˵����1. PWM�Ƿ���������ض�����PWM.H�к궨��
			2. �����Ҫ����ͨ����PWM.H�к궨��
			3. ���������Ƶ��Ϊ��	72MH/arr/psc	��TIM4_PWM_Init(900,0);	 //PWMƵ��=72000/900=80Khz	
			4. �����ɶ�Ӧ��ͨ�����ĺ���					TIM_SetCompare1(TIM4,X); //�ı�ͨ��1PWMռ�ձ�	
																					TIM_SetCompare2(TIM4,X); //�ı�ͨ��2PWMռ�ձ�	
***************************************************************************************/
void TIM5_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	#ifdef 	TIM5_USE_CH1					//TIM5ͨ��1
		RCC_APB2PeriphClockCmd(RCC_T5CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM5_USE_CH2					//TIM5ͨ��2
		RCC_APB2PeriphClockCmd(RCC_T5CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM5_USE_CH3					//TIM5ͨ��3
		RCC_APB2PeriphClockCmd(RCC_T5CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM5_USE_CH4					//TIM5ͨ��4
		RCC_APB2PeriphClockCmd(RCC_T5CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	#ifdef 	TIM5_USE_CH1					//TIM5ͨ��1
		TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM5_USE_CH2					//TIM5ͨ��2
		TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	#ifdef 	TIM5_USE_CH3					//TIM5ͨ��3
		TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif	
	#ifdef 	TIM5_USE_CH4					//TIM5ͨ��4
		TIM_OC4Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
		TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	#endif
	
	TIM_ARRPreloadConfig(TIM5, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����

}




/***************************************************************************************
������TIMx_PWM_Init
���ܣ���TIM2~TIM5 PWM���ܳ�ʼ��
������TIMx: ��ʱ���ţ�arr: �Զ���װֵ��psc: Ԥ��Ƶ��(��ÿ��������Ҫ��������)
���أ���
˵����1. PWM��IO����Ҫ���ⶨ�壬����Ҫ�˿��ض���Ļ�Ҳ����IO�ڶ���ʱһ����
			2. �����Ҫ����ͨ���̶�
			3. ���������Ƶ��Ϊ��	72MH/arr/psc	��TIM2_PWM_Init(900,0);	 //PWMƵ��=72000/900=80Khz	
			4. �����ɶ�Ӧ��ͨ�����ĺ���					TIM_SetCompare1(TIM2,X); //�ı�ͨ��1PWMռ�ձ�	
																					TIM_SetCompare2(TIM2,X); //�ı�ͨ��2PWMռ�ձ�	
***************************************************************************************/
void TIMx_PWM_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	if(TIMx == TIM2){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	}
	else if(TIMx == TIM3){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		
	}
	else if(TIMx == TIM4){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		
	}	
	else if(TIMx == TIM5){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		
	}	

   
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIMx, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIMx, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_ARRPreloadConfig(TIMx, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIMx, ENABLE);  //ʹ��TIMx����
}


//���ݵ���ǰPWM����
//void TIM3_PWM_Init(u16 arr,u16 psc)
//{ 
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure; 
//	GPIO_InitTypeDef GPIO_InitStructure;	
// NVIC_InitTypeDef NVIC_InitStructure;			 //�����ж����ݽṹ
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
////	GPIO_InitStructure.GPIO_Pin = GPIO_T3CH1 | GPIO_T3CH2 | GPIO_T3CH3 | GPIO_T3CH4 ;
//	GPIO_InitStructure.GPIO_Pin =  GPIO_T3CH3 | GPIO_T3CH4 ;	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(PORT_T3CH, &GPIO_InitStructure);
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��ʹ�� ����ʱ��ʹ�� ��
//	
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);		 //TIM3��ȫӳ��
//   
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //ͨ��TIM3
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);


//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
//	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

//	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
// 
//	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
//}










