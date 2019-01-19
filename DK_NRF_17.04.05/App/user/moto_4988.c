#include "os.h"
#include "moto_4988.h"
#include "dk_conf.h"
#include "device_conf.h"

#define SPEED_PUSH_NUM 7   				//���μӼ���ʹ�õ���������
MOTO_Struct Moto[3];								//3����������Ļ�����
u16 MotoSpeed_XY[SPEED_PUSH_NUM];		//XY��������ٶ�
u16 MotoSpeed_Z[SPEED_PUSH_NUM];		//Z��������ٶ�
u8 const MotoSpeed_C[] = {4,5,7,11,17,25,35};  //���μӼ����õ�ÿ������ʱ�䣬��һ������������ʱÿ������ʱ�䣬�����ǿ�ʼ�ͽ���ʱ����ʱ��
//u8 const MotoSpeed_C[] = {20,28,42,60,80,100}; 
/********************************************
������Set_Speed_Z
���ܣ�����Z��������ٶ�
������Speed �ٶ�
���أ���
********************************************/
void Set_Speed_Z(u16 Speed)
{
	u16 i;
	if(Speed<10) Speed = 10;
	for(i=0;i<SPEED_PUSH_NUM;i++)
	{
		MotoSpeed_Z[i] = MotoSpeed_C[SPEED_PUSH_NUM - i-1] * Speed;
	}
}
void Set_Speed_XY(u16 Speed)
{
	u16 i;
	if(Speed<10) Speed = 10;
	for(i=0;i<SPEED_PUSH_NUM;i++)
	{
		MotoSpeed_XY[i] = MotoSpeed_C[SPEED_PUSH_NUM - i-1] * Speed;
	}
}
/********************************************
������MOTO_TIM_Init
���ܣ�3����ʱ���ĳ�ʼ��
������arr���Զ���װֵ��psc��ʱ��Ԥ��Ƶ��
���أ���
********************************************/
void MOTO_TIM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 	
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//����TIM2��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//����TIM3��ʱ��	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//����TIM4��ʱ��	
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�	
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�	
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�	
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���			

	//TIM2ͨ��3 IO��ʼ��
	IO_AF_PP_HIGH(MOTO1_STEP_GPIO,MOTO1_STEP_PIN,MOTO1_STEP_RCC);
	GPIO_ResetBits(MOTO1_STEP_GPIO,MOTO1_STEP_PIN);	
	//TIM3ͨ��2
	IO_AF_PP_HIGH(MOTO2_STEP_GPIO,MOTO2_STEP_PIN,MOTO2_STEP_RCC);
	GPIO_ResetBits(MOTO2_STEP_GPIO,MOTO2_STEP_PIN);	
	//TIM4ͨ��2
	IO_AF_PP_HIGH(MOTO3_STEP_GPIO,MOTO3_STEP_PIN,MOTO3_STEP_RCC);
	GPIO_ResetBits(MOTO3_STEP_GPIO,MOTO3_STEP_PIN);	

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	
// TIM2_PWM_CH2					//TIM2ͨ��3
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 	
	TIM_ITConfig(TIM2, TIM_IT_CC2,ENABLE);//ʹ�ܻ���ʧ��ָ����TIM�ж�	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
// TIM3_PWM_CH4					//TIM3ͨ��2
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR4�ϵ�Ԥװ�ؼĴ���	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 	
	TIM_ITConfig(TIM3, TIM_IT_CC4,ENABLE);//ʹ�ܻ���ʧ��ָ����TIM�ж�	
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����	
// TIM4_PWM_CH2					//TIM3ͨ��2
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 	
	TIM_ITConfig(TIM4, TIM_IT_CC2,ENABLE);//ʹ�ܻ���ʧ��ָ����TIM�ж�	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
}
/********************************************
������Run_MainMoto
���ܣ���������
��������
���أ���
********************************************/
void Run_MainMoto(void)
{
	u8 i,k,j;
	DK.State_B_P->Spindle_On = 1;	
	for(i=4;i<16;i++)
	{
		for(j=0;j<10;j++)
		{
			for(k=0;k<16;k++)
			{		
				if(k<=i)
				{
					MOTO_MAIN_ON;
				}
				else
				{
					MOTO_MAIN_OFF;
				}
				if(DK.State_B_P->Spindle_On == 0 )
				{
					MOTO_MAIN_OFF;
					return;
				}
				OS.DelayTimeTick(1);
			}
		}
	}
	MOTO_MAIN_ON;
}
/********************************************
������Stop_MainMoto
���ܣ��ر�����
��������
���أ���
********************************************/
void Stop_MainMoto(void)
{
	MOTO_MAIN_OFF;
	DK.State_B_P->Spindle_On = 0;	
}
/********************************************
������Initialize_Moto
���ܣ�Moto�˿ڳ�ʼ��
��������
���أ���
********************************************/
void Initialize_Moto(void)
{		
	u8 i;
	IO_OUT_PP_HIGH(MOTO1_DIR_GPIO,MOTO1_DIR_PIN,MOTO1_DIR_RCC);
	IO_OUT_PP_HIGH(MOTO2_DIR_GPIO,MOTO2_DIR_PIN,MOTO2_DIR_RCC);
	IO_OUT_PP_HIGH(MOTO3_DIR_GPIO,MOTO3_DIR_PIN,MOTO3_DIR_RCC);
	IO_OUT_PP_HIGH(MOTO_MAIN_GPIO,MOTO_MAIN_PIN,MOTO_MAIN_RCC);
	IO_OUT_PP_HIGH(MOTO_EN_GPIO,MOTO_EN_PIN,MOTO_EN_RCC);	
	MOTO_OFF;						//���ò������
	MOTO_MAIN_OFF;			//�ر�����
	GPIO_SetBits(MOTO_MAIN_GPIO,MOTO_MAIN_PIN);		
	for(i=0;i<3;i++)
	{
		Moto[i].Flag 		= false;				//moto���б�־
		Moto[i].Current = 0;				//moto�ĵ�ǰλ��	
	}
	Set_Speed_XY(DK.SYS.Speed_XY);	
	Set_Speed_Z(DK.SYS.Speed_Z);	
	Moto[0].DIR = (vu32*)&MOTO1_DIR_O;
	Moto[1].DIR = (vu32*)&MOTO2_DIR_O;	
	Moto[2].DIR = (vu32*)&MOTO3_DIR_O;	
	Moto[0].TIM_CCR = (vu16*)&(TIM2->CCR2);
	Moto[1].TIM_CCR = (vu16*)&(TIM3->CCR4);
	Moto[2].TIM_CCR = (vu16*)&(TIM4->CCR2);
	Moto[0].TIM_PSC = (vu16*)&(TIM2->PSC);
	Moto[1].TIM_PSC = (vu16*)&(TIM3->PSC);
	Moto[2].TIM_PSC = (vu16*)&(TIM4->PSC);
	
	MOTO_TIM_Init(72,40);	
	for(i=0;i<3;i++)
	{
		if(i == AXIS_Z)
		{
			Moto[i].Speed = MotoSpeed_Z;			
		}
		else
		{
			Moto[i].Speed = MotoSpeed_XY;
		}
	}	
}
/********************************************
������Stop_Moto
���ܣ�Moto�˿�ֹͣ
������Axis ��Ӧ�������
���أ���
********************************************/
void Stop_Moto(u8 Axis)
{
	*Moto[Axis].TIM_CCR = 0;
	Moto[Axis].Target = Moto[Axis].Current;	
	Moto[Axis].Flag = 0;
}
void Moto_Wait_XY(void)
{
	while((Moto[AXIS_X].Flag)||(Moto[AXIS_Y].Flag))   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
	{
		OS.DelayTimeTick(1);
	}	
	InitPlanQueue();		
}
/********************************************
������Move_X
���ܣ�X���������
������Step ��Ҫ���е���������
���أ���
********************************************/
void Moto_Move(u8 AXIS,s32 Step)//�������� stepΪ��Ҫ���е���������������1��������Ҫ��������
{
	if(Step == 0) return;		//��������Ϊ0������Ҫ����	
	Moto[AXIS].Target = Moto[AXIS].Target + Step;
	if(Moto[AXIS].Flag == 0)		//ԭ���ڲ�������
	{	
		if(Step>0)
		{
			*(Moto[AXIS].DIR)	 = POSITIVE;
		}
		else
		{
			*(Moto[AXIS].DIR)	 = REVERSE;
		}
		
		//		OS.DelayTimeTick(1);//��ʱ1ms�ȴ�4988 DIR�����ȶ�
		Moto[AXIS].Flag = 1;	//��������
	}		
}


void MotoGotoZero(void)
{
	
	DK.Ctrl = CTRL_STOP;
	Stop_Moto(AXIS_X);
	Stop_Moto(AXIS_Y);
	Stop_Moto(AXIS_Z);
	MOTO_ON;		
	Set_Speed_XY(DK.SYS.Speed_Fast);
	Set_Speed_XY(DK.SYS.Speed_Z);	
	if(Moto[AXIS_Z].Current<200 * STEP_TO_PUSH)
	{
		Moto_Move(AXIS_Z,200 * STEP_TO_PUSH -Moto[AXIS_Z].Current ) ;
	}	
	while(Moto[AXIS_Z].Flag)  //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
	{
		OS.DelayTimeTick(1);
	}	
	Moto_Move(AXIS_X, -Moto[AXIS_X].Current);
	Moto_Move(AXIS_Y, -Moto[AXIS_Y].Current);
	Moto_Wait_XY();
	MOTO_OFF;
}
/********************************************
������TIM_Moto
���ܣ�Moto�������������
������AXIS:������
���أ���
˵����Moto_1��PWM�������
********************************************/
void TIM_Moto(u8 AXIS)
{
		if(Moto[AXIS].Flag)		//���б�־
		{
			/**�ٶȼ���**/
			if(DK.Ctrl == CTRL_PAUSE)
			{
				Moto[AXIS].Flag = 1;  	//
				*(Moto[AXIS].TIM_CCR) = 0;	
				return;
			}
			*(Moto[AXIS].TIM_CCR) = 36;

			if(Moto[AXIS].Flag<=SPEED_PUSH_NUM)		//moto�ٶ�
			{				
				*(Moto[AXIS].TIM_PSC) = Moto[AXIS].Speed[Moto[AXIS].Flag - 1];
				Moto[AXIS].Flag ++;
			}
			else
			{
				*(Moto[AXIS].TIM_PSC) = Moto[AXIS].Speed[SPEED_PUSH_NUM - 1];				
			}
			if(*(Moto[AXIS].DIR) == REVERSE)	//����Ƿ�ת��
			{				
				if(DK.Alarm_B_P->L[AXIS]	|| (Moto[AXIS].Target >= Moto[AXIS].Current))		//��λ�ж� ��Ŀ��ֵ�ж�
				{
					Stop_Moto(AXIS);	
					return;
				}					
				else
				{
					Moto[AXIS].Current--;
					if(Moto[AXIS].Current - Moto[AXIS].Target < SPEED_PUSH_NUM)
					{				
						*(Moto[AXIS].TIM_PSC) =  Moto[AXIS].Speed[Moto[AXIS].Current - Moto[AXIS].Target];
					}							
				}			
			}
			else	//�������ת��
			{
				if(DK.Alarm_B_P->R[AXIS]	|| (Moto[AXIS].Target <= Moto[AXIS].Current))		//��λ�ж� ��Ŀ��ֵ�ж�
				{
					Stop_Moto(AXIS);	
					return;
				}					
				else
				{
					Moto[AXIS].Current++;
					
				}				
					if(Moto[AXIS].Target - Moto[AXIS].Current <SPEED_PUSH_NUM)
					{				
						*(Moto[AXIS].TIM_PSC) =  Moto[AXIS].Speed[Moto[AXIS].Target - Moto[AXIS].Current];
					}						
			}
		}	
}
/********************************************
������TIM2_IRQHandler
���ܣ�TIM2���ж����
��������
���أ���
˵����Moto_1��PWM�������
********************************************/
void TIM2_IRQHandler(void)//��ʱ���жϺ���
{

	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) //���TIM2�����жϷ������
	{	
		TIM_ClearFlag(TIM2,TIM_IT_CC2);//		
		TIM_Moto(0);
	}
}
void TIM3_IRQHandler(void)//��ʱ���жϺ���
{

	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) //���TIM3�����жϷ������
	{	
		TIM_ClearFlag(TIM3,TIM_IT_CC4);//		
		TIM_Moto(1);
	}
}
void TIM4_IRQHandler(void)//��ʱ���жϺ���
{

	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) //���TIM4�����жϷ������
	{	
		TIM_ClearFlag(TIM4,TIM_IT_CC2);//		
		TIM_Moto(2);
	}
}






//void Move_X(s32 Step)//�������� stepΪ��Ҫ���е���������������1��������Ҫ��������
//{
//	if(Step == 0) return;		//��������Ϊ0������Ҫ����
//	Moto[AXIS_X].Target = Moto[AXIS_X].Target + Step;
//	if(Moto[AXIS_X].Flag == 0)		//ԭ���ڲ�������
//	{	
//		if(Step>0)
//		{
//		M_X_DIR = POSITIVE;
//		}
//		else
//		{
//		M_X_DIR = REVERSE;
//		}
//		OS.DelayTimeTick(1);//��ʱ1ms�ȴ�4988 DIR�����ȶ�
//		Moto[AXIS_X].Flag = 1;	//��������
//	}		
//}
//void Move_Y(s32 Step)//�������� stepΪ��Ҫ���е���������������1��������Ҫ��������
//{
//	if(Step == 0) return;		//��������Ϊ0������Ҫ����
//	Moto[AXIS_Y].Target = Moto[AXIS_Y].Target + Step;
//	if(Moto[AXIS_Y].Flag == 0)		//ԭ���ڲ�������
//	{	
//		if(Step>0)
//		{
//		M_Y_DIR = POSITIVE;
//		}
//		else
//		{
//		M_Y_DIR = REVERSE;
//		}
//		OS.DelayTimeTick(1);//��ʱ1ms�ȴ�4988 DIR�����ȶ�
//		Moto[AXIS_Y].Flag = 1;	//��������
//	}		
//}


//void Move_Z(s32 Step)//�������� stepΪ��Ҫ���е���������������1��������Ҫ��������
//{
//	if(Step == 0) return;		//��������Ϊ0������Ҫ����
//	Moto[AXIS_Z].Target = Moto[AXIS_Z].Target + Step;
//	if(Moto[AXIS_Z].Flag == 0)		//ԭ���ڲ�������
//	{	
//		if(Step>0)
//		{
//		M_Z_DIR = POSITIVE;
//		}
//		else
//		{
//		M_Z_DIR = REVERSE;
//		}
//		OS.DelayTimeTick(1);//��ʱ1ms�ȴ�4988 DIR�����ȶ�
//		Moto[AXIS_Z].Flag = 1;	//��������
//	}		
//	while(Moto[AXIS_Z].Flag)   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
//	{
//		OS.DelayTimeTick(1);
//	}		
//}




