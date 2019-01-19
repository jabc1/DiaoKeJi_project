/*————————————————————————————————————————————————————————————————
* 文件名		: PWM.c
* 版本号    : V1.1.0
* 日期      : 2015/6/28
* 说明      : PWM的定义											  
* 用法			：

V1.1.0 
增加了TIM5的PWM定义


——————————————————————————————————————————————————————————————————*/
#include "device_pwm.h"
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef   TIM_OCInitStructure;
	TIM_BDTRInitTypeDef   TIM_BDTRInitStructure;
	//第一步：配置时钟        
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);	
	//第二步，配置goio口
	/********TIM1 引脚配置*********/    	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,  ENABLE);   // T1引脚重定向
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14; 
															//TIM1_CH1N   TIM1_CH1    TIM1_CH2N     TIM1_CH2    TIM1_CH3N     TIM1_CH3      TIM1_CH4	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
		//第三步，定时器基本配置        
	TIM_TimeBaseStructure.TIM_Period=arr;             // 自动重装载寄存器的值	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;            // 时钟预分频数
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;    // 采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//重复寄存器，用于自动更新pwm占空比    
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //第四步pwm输出配置
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;                    //设置为pwm2输出模式
	TIM_OCInitStructure.TIM_Pulse=0;                                 //设置占空比时间
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;             //设置输出极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; 		
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //使能该通道输出
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;			//
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;         //输出空闲状态为1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   //输出空闲状态为0 			
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 		
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure); 		
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
	

	//第五步，死区和刹车功能配置，高级定时器才有的，通用定时器不用配置
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;				//运行模式下输出选择 
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			//空闲模式下输出选择 
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;      	 	  //锁定设置
	TIM_BDTRInitStructure.TIM_DeadTime = 0x00;                  			 //死区时间设置
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 	//刹车功能使能
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;		//刹车输入极性
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能 
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);   

	//第六步，使能端的打开
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);                //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM1,ENABLE);                                                           
	//下面这句是高级定时器才有的，输出pwm必须打开
	TIM_CtrlPWMOutputs(TIM1, ENABLE);                 //pwm输出使能，一定要记得打开

}
void TIM8_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
// NVIC_InitTypeDef NVIC_InitStructure;			 //设置中断数据结构

	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef   TIM_OCInitStructure;
    TIM_BDTRInitTypeDef   TIM_BDTRInitStructure;
	
	 //第一步：配置时钟        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	
	//第二步，配置goio口
	/********TIM8_CH1 引脚配置    
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

		//第三步，定时器基本配置        
	TIM_TimeBaseStructure.TIM_Period=arr;             // 自动重装载寄存器的值	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;            // 时钟预分频数
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;    // 采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//重复寄存器，用于自动更新pwm占空比    
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    //第四步pwm输出配置
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;                    //设置为pwm2输出模式
	TIM_OCInitStructure.TIM_Pulse=20;                                 //设置占空比时间
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;             //设置输出极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; 		
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //使能该通道输出
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;			//关闭互补输出------------
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;         //输出空闲状态为1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   //输出空闲状态为0 			
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 		
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); 		
//	TIM_OC4Init(TIM8, &TIM_OCInitStructure); 
	

	//第五步，死区和刹车功能配置，高级定时器才有的，通用定时器不用配置
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;				//运行模式下输出选择 
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			//空闲模式下输出选择 
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;      	 	  //锁定设置
    TIM_BDTRInitStructure.TIM_DeadTime = 0x00;                  			 //死区时间设置
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 	//刹车功能使能
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;		//刹车输入极性
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能 
    TIM_BDTRConfig(TIM8,&TIM_BDTRInitStructure);   

	//第六步，使能端的打开
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器	
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器	
	TIM_ARRPreloadConfig(TIM8, ENABLE);                //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM8,ENABLE);                                                           
	//下面这句是高级定时器才有的，输出pwm必须打开
	TIM_CtrlPWMOutputs(TIM8, ENABLE);                 //pwm输出使能，一定要记得打开

}


/***************************************************************************************
函数：TIM2_PWM_Init
功能：对TIM2 PWM功能初始化
参数：arr: 自动重装值；psc: 预分频数(即每个计数需要的脉冲数)
返回：无
说明：1. PWM是否开启输出口重定义由PWM.H中宏定义
			2. 输出需要几个通道由PWM.H中宏定义
			3. 输出的脉冲频率为：	72MH/arr/psc	如TIM2_PWM_Init(900,0);	 //PWM频率=72000/900=80Khz	
			4. 脉宽由对应的通道更改函数					TIM_SetCompare1(TIM2,X); //改变通道1PWM占空比	
																					TIM_SetCompare2(TIM2,X); //改变通道2PWM占空比	
***************************************************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	#if 	TIM2_PartialRemap1 > 0					//TIM2使用部分端口重定义1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟使能 复用时钟使能 复
		GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);		 //TIM2部分映射1	
	#endif	
	#if  	TIM2_PartialRemap2 > 0					//TIM2使用部分端口重定义1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟使能 复用时钟使能 复
		GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);		 //TIM2部分映射1	
	#endif
	#if  	TIM2_FullRemap > 0					//TIM2使用完全端口重定义
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟使能 复用时钟使能 复
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);		 //TIM2完全映射
	#endif	
	#ifdef 	TIM2_USE_CH1					//TIM2通道1
		RCC_APB2PeriphClockCmd(RCC_T2CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM2_USE_CH2					//TIM2通道2
		RCC_APB2PeriphClockCmd(RCC_T2CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM2_USE_CH3					//TIM2通道3
		RCC_APB2PeriphClockCmd(RCC_T2CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM2_USE_CH4					//TIM2通道4
		RCC_APB2PeriphClockCmd(RCC_T2CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T2CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T2CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	#ifdef 	TIM2_USE_CH1					//TIM2通道1
		TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM2_USE_CH2					//TIM2通道2
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM2_USE_CH3					//TIM2通道3
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif	
	#ifdef 	TIM2_USE_CH4					//TIM2通道4
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器 
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设

}
/***************************************************************************************
函数：TIM3_PWM_Init
功能：对TIM3 PWM功能初始化
参数：arr: 自动重装值；psc: 预分频数(即每个计数需要的脉冲数)
返回：无
说明：1. PWM是否开启输出口重定义由PWM.H中宏定义
			2. 输出需要几个通道由PWM.H中宏定义
			3. 输出的脉冲频率为：	72MH/arr/psc	如TIM3_PWM_Init(900,0);	 //PWM频率=72000/900=80Khz	
			4. 脉宽由对应的通道更改函数					TIM_SetCompare1(TIM3,X); //改变通道1PWM占空比X	
																					TIM_SetCompare2(TIM3,X); //改变通道2PWM占空比X	
***************************************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	#if  	TIM3_PartialRemap	 > 0				//TIM2使用部分端口重定义1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟使能 复用时钟使能 复
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);		 //TIM3部分映射1	
	#endif
	#if  	TIM3_FullRemap > 0					//TIM2使用完全端口重定义
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟使能 复用时钟使能 复
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);		 //TIM3完全映射
	#endif	
	#ifdef 	TIM3_USE_CH1					//TIM3通道1
		RCC_APB2PeriphClockCmd(RCC_T3CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM3_USE_CH2					//TIM3通道2
		RCC_APB2PeriphClockCmd(RCC_T3CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM3_USE_CH3					//TIM3通道3
		RCC_APB2PeriphClockCmd(RCC_T3CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM3_USE_CH4					//TIM3通道4
		RCC_APB2PeriphClockCmd(RCC_T3CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T3CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T3CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高	
	
	#ifdef 	TIM3_USE_CH1					//TIM3通道1
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM3_USE_CH2					//TIM3通道2
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM3_USE_CH3					//TIM3通道3
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif	
	#ifdef 	TIM3_USE_CH4					//TIM3通道4
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器 
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设

}
/***************************************************************************************
函数：TIM4_PWM_Init
功能：对TIM4 PWM功能初始化
参数：arr: 自动重装值；psc: 预分频数(即每个计数需要的脉冲数)
返回：无
说明：1. PWM是否开启输出口重定义由PWM.H中宏定义
			2. 输出需要几个通道由PWM.H中宏定义
			3. 输出的脉冲频率为：	72MH/arr/psc	如TIM4_PWM_Init(900,0);	 //PWM频率=72000/900=80Khz	
			4. 脉宽由对应的通道更改函数					TIM_SetCompare1(TIM4,X); //改变通道1PWM占空比	
																					TIM_SetCompare2(TIM4,X); //改变通道2PWM占空比	
***************************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	#if  	TIM4_Remap > 0					//TIM2使用部分端口重定义1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟使能 复用时钟使能 复
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);		 //TIM4映射	
	#endif

	#ifdef 	TIM4_USE_CH1					//TIM4通道1
		RCC_APB2PeriphClockCmd(RCC_T4CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM4_USE_CH2					//TIM4通道2
		RCC_APB2PeriphClockCmd(RCC_T4CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM4_USE_CH3					//TIM4通道3
		RCC_APB2PeriphClockCmd(RCC_T4CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM4_USE_CH4					//TIM4通道4
		RCC_APB2PeriphClockCmd(RCC_T4CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T4CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T4CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高	
	#ifdef 	TIM4_USE_CH1					//TIM4通道1
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM4_USE_CH2					//TIM4通道2
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM4_USE_CH3					//TIM4通道3
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif	
	#ifdef 	TIM4_USE_CH4					//TIM4通道4
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器 
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设

}

/***************************************************************************************
函数：TIM4_PWM_Init
功能：对TIM4 PWM功能初始化
参数：arr: 自动重装值；psc: 预分频数(即每个计数需要的脉冲数)
返回：无
说明：1. PWM是否开启输出口重定义由PWM.H中宏定义
			2. 输出需要几个通道由PWM.H中宏定义
			3. 输出的脉冲频率为：	72MH/arr/psc	如TIM4_PWM_Init(900,0);	 //PWM频率=72000/900=80Khz	
			4. 脉宽由对应的通道更改函数					TIM_SetCompare1(TIM4,X); //改变通道1PWM占空比	
																					TIM_SetCompare2(TIM4,X); //改变通道2PWM占空比	
***************************************************************************************/
void TIM5_PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	#ifdef 	TIM5_USE_CH1					//TIM5通道1
		RCC_APB2PeriphClockCmd(RCC_T5CH1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH1, &GPIO_InitStructure);	
	#endif
	#ifdef 	TIM5_USE_CH2					//TIM5通道2
		RCC_APB2PeriphClockCmd(RCC_T5CH2, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH2, &GPIO_InitStructure);	
	#endif	
	#ifdef 	TIM5_USE_CH3					//TIM5通道3
		RCC_APB2PeriphClockCmd(RCC_T5CH3, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH3, &GPIO_InitStructure);	
	#endif		
	#ifdef 	TIM5_USE_CH4					//TIM5通道4
		RCC_APB2PeriphClockCmd(RCC_T5CH4, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_T5CH4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_T5CH4, &GPIO_InitStructure);	
	#endif															

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	#ifdef 	TIM5_USE_CH1					//TIM5通道1
		TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM5_USE_CH2					//TIM5通道2
		TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	#ifdef 	TIM5_USE_CH3					//TIM5通道3
		TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif	
	#ifdef 	TIM5_USE_CH4					//TIM5通道4
		TIM_OC4Init(TIM5, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器	
	#endif
	
	TIM_ARRPreloadConfig(TIM5, ENABLE); //使能TIMx在ARR上的预装载寄存器 
	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设

}




/***************************************************************************************
函数：TIMx_PWM_Init
功能：对TIM2~TIM5 PWM功能初始化
参数：TIMx: 定时器号；arr: 自动重装值；psc: 预分频数(即每个计数需要的脉冲数)
返回：无
说明：1. PWM的IO口需要另外定义，如需要端口重定义的话也需在IO口定义时一起定义
			2. 输出需要几个通道固定
			3. 输出的脉冲频率为：	72MH/arr/psc	如TIM2_PWM_Init(900,0);	 //PWM频率=72000/900=80Khz	
			4. 脉宽由对应的通道更改函数					TIM_SetCompare1(TIM2,X); //改变通道1PWM占空比	
																					TIM_SetCompare2(TIM2,X); //改变通道2PWM占空比	
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

   
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	TIM_ARRPreloadConfig(TIMx, ENABLE); //使能TIMx在ARR上的预装载寄存器
 
	TIM_Cmd(TIMx, ENABLE);  //使能TIMx外设
}


//备份的以前PWM函数
//void TIM3_PWM_Init(u16 arr,u16 psc)
//{ 
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure; 
//	GPIO_InitTypeDef GPIO_InitStructure;	
// NVIC_InitTypeDef NVIC_InitStructure;			 //设置中断数据结构
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
////	GPIO_InitStructure.GPIO_Pin = GPIO_T3CH1 | GPIO_T3CH2 | GPIO_T3CH3 | GPIO_T3CH4 ;
//	GPIO_InitStructure.GPIO_Pin =  GPIO_T3CH3 | GPIO_T3CH4 ;	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(PORT_T3CH, &GPIO_InitStructure);
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟使能 复用时钟使能 复
//	
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);		 //TIM3完全映射
//   
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //通道TIM3
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //副优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);


//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
//	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
//	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
//	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

//	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
// 
//	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
//}










