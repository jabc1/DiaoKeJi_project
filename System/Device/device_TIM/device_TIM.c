/*******************************************************************************
* 文件名		: timer.c
* 版本号    : V1.1.0
* 日期      : 2014/10/09
* 说明      : 定时器定时初始化，T1和T8需单独定义，T2-T5可以共用	TIMx_Init								  
* 用法			：

void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)   TIMx：定时器编号；arr：自动重装载值；psc；预分频数
void TIM2_Config(u16 arr,u16 psc)						arr：自动重装载值；psc；预分频数
									  
*******************************************************************************/

#include "device_TIM.h"





void TIM2_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//时钟配置

 	TIM_TimeBaseStructure.TIM_Period = arr;        //装载值36M/3600=10khz
	TIM_TimeBaseStructure.TIM_Prescaler = psc;       //预分频(时钟分频)72M/(2)=36M
 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数
 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
 	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); //初始化定时器的值

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //选择TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//使能

	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM2,TIM_FLAG_Update); //清除定时器中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //使能中断

	TIM_Cmd(TIM2,ENABLE);  //打开TIM2

}



//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数

void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	if(TIMx == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断			
	}
	else if(TIMx == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断	
	}
	else if(TIMx == TIM4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断		
	}	
	#ifdef STM32F10X_HD
	else if(TIMx == TIM5){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断	
	}	
	#endif

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_ClearFlag(TIMx,TIM_FLAG_Update); //清除定时器中断标志 
	/* TIM IT enable */
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIMx, //TIM
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);



	TIM_Cmd(TIMx, ENABLE);  //使能TIMx外设
							 
}



















