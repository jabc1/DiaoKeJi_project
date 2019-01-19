#include "os.h"
#include "moto_4988.h"
#include "dk_conf.h"
#include "device_conf.h"

#define SPEED_PUSH_NUM 7   				//梯形加减速使用的脉冲数量
MOTO_Struct Moto[3];								//3个步进电机的机构体
u16 MotoSpeed_XY[SPEED_PUSH_NUM];		//XY轴的运行速度
u16 MotoSpeed_Z[SPEED_PUSH_NUM];		//Z轴的运行速度
u8 const MotoSpeed_C[] = {4,5,7,11,17,25,35};  //梯形加减速用的每个脉冲时间，第一个是正常运行时每个脉冲时间，最后个是开始和结束时脉冲时间
//u8 const MotoSpeed_C[] = {20,28,42,60,80,100}; 
/********************************************
函数：Set_Speed_Z
功能：计算Z轴的运行速度
参数：Speed 速度
返回：无
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
函数：MOTO_TIM_Init
功能：3个定时器的初始化
参数：arr：自动重装值；psc：时钟预分频数
返回：无
********************************************/
void MOTO_TIM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 	
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//启动TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//启动TIM3的时钟	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//启动TIM4的时钟	
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断	
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断	
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断	
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器			

	//TIM2通道3 IO初始化
	IO_AF_PP_HIGH(MOTO1_STEP_GPIO,MOTO1_STEP_PIN,MOTO1_STEP_RCC);
	GPIO_ResetBits(MOTO1_STEP_GPIO,MOTO1_STEP_PIN);	
	//TIM3通道2
	IO_AF_PP_HIGH(MOTO2_STEP_GPIO,MOTO2_STEP_PIN,MOTO2_STEP_RCC);
	GPIO_ResetBits(MOTO2_STEP_GPIO,MOTO2_STEP_PIN);	
	//TIM4通道2
	IO_AF_PP_HIGH(MOTO3_STEP_GPIO,MOTO3_STEP_PIN,MOTO3_STEP_RCC);
	GPIO_ResetBits(MOTO3_STEP_GPIO,MOTO3_STEP_PIN);	

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	
// TIM2_PWM_CH2					//TIM2通道3
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器 	
	TIM_ITConfig(TIM2, TIM_IT_CC2,ENABLE);//使能或者失能指定的TIM中断	
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
// TIM3_PWM_CH4					//TIM3通道2
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR4上的预装载寄存器	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器 	
	TIM_ITConfig(TIM3, TIM_IT_CC4,ENABLE);//使能或者失能指定的TIM中断	
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设	
// TIM4_PWM_CH2					//TIM3通道2
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器 	
	TIM_ITConfig(TIM4, TIM_IT_CC2,ENABLE);//使能或者失能指定的TIM中断	
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
}
/********************************************
函数：Run_MainMoto
功能：启动主轴
参数：无
返回：无
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
函数：Stop_MainMoto
功能：关闭主轴
参数：无
返回：无
********************************************/
void Stop_MainMoto(void)
{
	MOTO_MAIN_OFF;
	DK.State_B_P->Spindle_On = 0;	
}
/********************************************
函数：Initialize_Moto
功能：Moto端口初始化
参数：无
返回：无
********************************************/
void Initialize_Moto(void)
{		
	u8 i;
	IO_OUT_PP_HIGH(MOTO1_DIR_GPIO,MOTO1_DIR_PIN,MOTO1_DIR_RCC);
	IO_OUT_PP_HIGH(MOTO2_DIR_GPIO,MOTO2_DIR_PIN,MOTO2_DIR_RCC);
	IO_OUT_PP_HIGH(MOTO3_DIR_GPIO,MOTO3_DIR_PIN,MOTO3_DIR_RCC);
	IO_OUT_PP_HIGH(MOTO_MAIN_GPIO,MOTO_MAIN_PIN,MOTO_MAIN_RCC);
	IO_OUT_PP_HIGH(MOTO_EN_GPIO,MOTO_EN_PIN,MOTO_EN_RCC);	
	MOTO_OFF;						//禁用步进电机
	MOTO_MAIN_OFF;			//关闭主轴
	GPIO_SetBits(MOTO_MAIN_GPIO,MOTO_MAIN_PIN);		
	for(i=0;i<3;i++)
	{
		Moto[i].Flag 		= false;				//moto运行标志
		Moto[i].Current = 0;				//moto的当前位置	
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
函数：Stop_Moto
功能：Moto端口停止
参数：Axis 对应的马达编号
返回：无
********************************************/
void Stop_Moto(u8 Axis)
{
	*Moto[Axis].TIM_CCR = 0;
	Moto[Axis].Target = Moto[Axis].Current;	
	Moto[Axis].Flag = 0;
}
void Moto_Wait_XY(void)
{
	while((Moto[AXIS_X].Flag)||(Moto[AXIS_Y].Flag))   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行
	{
		OS.DelayTimeTick(1);
	}	
	InitPlanQueue();		
}
/********************************************
函数：Move_X
功能：X轴马达运行
参数：Step 需要运行的脉冲数量
返回：无
********************************************/
void Moto_Move(u8 AXIS,s32 Step)//正向运行 step为需要运行的脉冲数量，不管1个周期需要多少脉冲
{
	if(Step == 0) return;		//脉冲数量为0，不需要运行	
	Moto[AXIS].Target = Moto[AXIS].Target + Step;
	if(Moto[AXIS].Flag == 0)		//原来在不在运行
	{	
		if(Step>0)
		{
			*(Moto[AXIS].DIR)	 = POSITIVE;
		}
		else
		{
			*(Moto[AXIS].DIR)	 = REVERSE;
		}
		
		//		OS.DelayTimeTick(1);//延时1ms等待4988 DIR建立稳定
		Moto[AXIS].Flag = 1;	//启动脉冲
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
	while(Moto[AXIS_Z].Flag)  //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行
	{
		OS.DelayTimeTick(1);
	}	
	Moto_Move(AXIS_X, -Moto[AXIS_X].Current);
	Moto_Move(AXIS_Y, -Moto[AXIS_Y].Current);
	Moto_Wait_XY();
	MOTO_OFF;
}
/********************************************
函数：TIM_Moto
功能：Moto的脉冲输出控制
参数：AXIS:电机编号
返回：无
说明：Moto_1的PWM控制输出
********************************************/
void TIM_Moto(u8 AXIS)
{
		if(Moto[AXIS].Flag)		//运行标志
		{
			/**速度计算**/
			if(DK.Ctrl == CTRL_PAUSE)
			{
				Moto[AXIS].Flag = 1;  	//
				*(Moto[AXIS].TIM_CCR) = 0;	
				return;
			}
			*(Moto[AXIS].TIM_CCR) = 36;

			if(Moto[AXIS].Flag<=SPEED_PUSH_NUM)		//moto速度
			{				
				*(Moto[AXIS].TIM_PSC) = Moto[AXIS].Speed[Moto[AXIS].Flag - 1];
				Moto[AXIS].Flag ++;
			}
			else
			{
				*(Moto[AXIS].TIM_PSC) = Moto[AXIS].Speed[SPEED_PUSH_NUM - 1];				
			}
			if(*(Moto[AXIS].DIR) == REVERSE)	//电机是反转的
			{				
				if(DK.Alarm_B_P->L[AXIS]	|| (Moto[AXIS].Target >= Moto[AXIS].Current))		//限位判断 和目标值判断
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
			else	//电机是正转的
			{
				if(DK.Alarm_B_P->R[AXIS]	|| (Moto[AXIS].Target <= Moto[AXIS].Current))		//限位判断 和目标值判断
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
函数：TIM2_IRQHandler
功能：TIM2的中断入口
参数：无
返回：无
说明：Moto_1的PWM控制输出
********************************************/
void TIM2_IRQHandler(void)//定时器中断函数
{

	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) //检查TIM2更新中断发生与否
	{	
		TIM_ClearFlag(TIM2,TIM_IT_CC2);//		
		TIM_Moto(0);
	}
}
void TIM3_IRQHandler(void)//定时器中断函数
{

	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) //检查TIM3更新中断发生与否
	{	
		TIM_ClearFlag(TIM3,TIM_IT_CC4);//		
		TIM_Moto(1);
	}
}
void TIM4_IRQHandler(void)//定时器中断函数
{

	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) //检查TIM4更新中断发生与否
	{	
		TIM_ClearFlag(TIM4,TIM_IT_CC2);//		
		TIM_Moto(2);
	}
}






//void Move_X(s32 Step)//正向运行 step为需要运行的脉冲数量，不管1个周期需要多少脉冲
//{
//	if(Step == 0) return;		//脉冲数量为0，不需要运行
//	Moto[AXIS_X].Target = Moto[AXIS_X].Target + Step;
//	if(Moto[AXIS_X].Flag == 0)		//原来在不在运行
//	{	
//		if(Step>0)
//		{
//		M_X_DIR = POSITIVE;
//		}
//		else
//		{
//		M_X_DIR = REVERSE;
//		}
//		OS.DelayTimeTick(1);//延时1ms等待4988 DIR建立稳定
//		Moto[AXIS_X].Flag = 1;	//启动脉冲
//	}		
//}
//void Move_Y(s32 Step)//正向运行 step为需要运行的脉冲数量，不管1个周期需要多少脉冲
//{
//	if(Step == 0) return;		//脉冲数量为0，不需要运行
//	Moto[AXIS_Y].Target = Moto[AXIS_Y].Target + Step;
//	if(Moto[AXIS_Y].Flag == 0)		//原来在不在运行
//	{	
//		if(Step>0)
//		{
//		M_Y_DIR = POSITIVE;
//		}
//		else
//		{
//		M_Y_DIR = REVERSE;
//		}
//		OS.DelayTimeTick(1);//延时1ms等待4988 DIR建立稳定
//		Moto[AXIS_Y].Flag = 1;	//启动脉冲
//	}		
//}


//void Move_Z(s32 Step)//正向运行 step为需要运行的脉冲数量，不管1个周期需要多少脉冲
//{
//	if(Step == 0) return;		//脉冲数量为0，不需要运行
//	Moto[AXIS_Z].Target = Moto[AXIS_Z].Target + Step;
//	if(Moto[AXIS_Z].Flag == 0)		//原来在不在运行
//	{	
//		if(Step>0)
//		{
//		M_Z_DIR = POSITIVE;
//		}
//		else
//		{
//		M_Z_DIR = REVERSE;
//		}
//		OS.DelayTimeTick(1);//延时1ms等待4988 DIR建立稳定
//		Moto[AXIS_Z].Flag = 1;	//启动脉冲
//	}		
//	while(Moto[AXIS_Z].Flag)   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行
//	{
//		OS.DelayTimeTick(1);
//	}		
//}




