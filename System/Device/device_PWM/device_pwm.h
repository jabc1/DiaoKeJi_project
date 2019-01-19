#ifndef __DEVICE_PWM_H
#define __DEVICE_PWM_H
#include "stm32f10x.h"


/*TIM使用PWM功能，并且输出口使用数量和定义*/
//----------------------------------T1 PWM 端口定义——————————————————————————————//
#define TIM1_PartialRemap 		0
#define TIM1_FullRemap				1
#define TIM1_USE_CH1
#define TIM1_USE_CH2
#define TIM1_USE_CH3
#define TIM1_USE_CH4
#define TIM1_USE_CH1N
#define TIM1_USE_CH2N
#define TIM1_USE_CH3N
//----------------------------------T2 PWM 端口定义——————————————————————————————//
#define TIM2_NORemap						0
#define TIM2_PartialRemap1			1
#define TIM2_PartialRemap2			0
#define TIM2_FullRemap					0
	#define TIM2_USE_CH1
	#define TIM2_USE_CH2
	#define TIM2_USE_CH3
	#define TIM2_USE_CH4
//----------------------------------T3 PWM 端口定义——————————————————————————————//
#define TIM3_NORemap						0
#define TIM3_PartialRemap				0
#define TIM3_FullRemap					1
	#define TIM3_USE_CH1
	#define TIM3_USE_CH2
	#define TIM3_USE_CH3
	#define TIM3_USE_CH4
//----------------------------------T4 PWM 端口定义——————————————————————————————//	
#define TIM4_NORemap						1
#define TIM4_Remap 							0
	#define TIM4_USE_CH1
	#define TIM4_USE_CH2
	#define TIM4_USE_CH3
	#define TIM4_USE_CH4
	
//----------------------------------T5 PWM 端口定义——————————————————————————————//		    
	#define TIM5_USE_CH1
	#define TIM5_USE_CH2
	#define TIM5_USE_CH3
	#define TIM5_USE_CH4
//-------------------------------------------------------------------------------//



#define GPIO_T1CH1      GPIO_Pin_8
#define PORT_T1CH1     GPIOA
#define RCC_T1CH1      RCC_APB2Periph_GPIOA

#define GPIO_T1CH2      GPIO_Pin_9
#define PORT_T1CH2     GPIOA
#define RCC_T1CH2      RCC_APB2Periph_GPIOA

#define GPIO_T1CH3      GPIO_Pin_10
#define PORT_T1CH3     GPIOA
#define RCC_T1CH3      RCC_APB2Periph_GPIOA

#define GPIO_T1CH4      GPIO_Pin_11
#define PORT_T1CH4     GPIOA
#define RCC_T1CH4      RCC_APB2Periph_GPIOA
#define PORT_T1CH     			GPIOA

#define GPIO_T1CH1N      GPIO_Pin_13
#define PORT_T1CH1N     GPIOB
#define RCC_T1CH1N     RCC_APB2Periph_GPIOB

#define GPIO_T1CH2N      GPIO_Pin_14
#define PORT_T1CH2N     GPIOB
#define RCC_T1CH2N      RCC_APB2Periph_GPIOB

#define GPIO_T1CH3N      GPIO_Pin_15
#define PORT_T1CH3N     GPIOB
#define RCC_T1CH3N      RCC_APB2Periph_GPIOB

#define PORT_T1CHN     			GPIOB










#if 	TIM2_NORemap > 0			//TIM2没有使用端口重定义
	#define GPIO_T2CH1      		GPIO_Pin_0
	#define PORT_T2CH1     		 	GPIOA
	#define RCC_T2CH1       		RCC_APB2Periph_GPIOA

	#define GPIO_T2CH2      		GPIO_Pin_1
	#define PORT_T2CH2     		 	GPIOA
	#define RCC_T2CH2      			RCC_APB2Periph_GPIOA

	#define GPIO_T2CH3      		GPIO_Pin_2
	#define PORT_T2CH3     		 	GPIOA
	#define RCC_T2CH3       		RCC_APB2Periph_GPIOA

	#define GPIO_T2CH4      		GPIO_Pin_3
	#define PORT_T2CH4     		 	GPIOA
	#define RCC_T2CH4       		RCC_APB2Periph_GPIOA
#endif

#if 	TIM2_PartialRemap1 >0					//TIM2使用部分端口重定义1
	#define GPIO_T2CH1      		GPIO_Pin_15
	#define PORT_T2CH1     		 	GPIOA
	#define RCC_T2CH1       		RCC_APB2Periph_GPIOA

	#define GPIO_T2CH2      		GPIO_Pin_3
	#define PORT_T2CH2     		 	GPIOB
	#define RCC_T2CH2      			RCC_APB2Periph_GPIOB

	#define GPIO_T2CH3      		GPIO_Pin_2
	#define PORT_T2CH3     		 	GPIOA
	#define RCC_T2CH3       		RCC_APB2Periph_GPIOA

	#define GPIO_T2CH4      		GPIO_Pin_3
	#define PORT_T2CH4     		 	GPIOA
	#define RCC_T2CH4       		RCC_APB2Periph_GPIOA
#endif

#if 	TIM2_PartialRemap2 > 0					//TIM2使用部分端口重定义2
	#define GPIO_T2CH1      		GPIO_Pin_0
	#define PORT_T2CH1     		 	GPIOA
	#define RCC_T2CH1       		RCC_APB2Periph_GPIOA

	#define GPIO_T2CH2      		GPIO_Pin_1
	#define PORT_T2CH2    		 	GPIOA
	#define RCC_T2CH2      			RCC_APB2Periph_GPIOA

	#define GPIO_T2CH3      		GPIO_Pin_10
	#define PORT_T2CH3     		 	GPIOB
	#define RCC_T2CH3       		RCC_APB2Periph_GPIOB

	#define GPIO_T2CH4      		GPIO_Pin_11
	#define PORT_T2CH4     		 	GPIOB
	#define RCC_T2CH4       		RCC_APB2Periph_GPIOB
#endif

#if 	TIM2_FullRemap >0					//TIM2使用完全端口重定义
	#define GPIO_T2CH1      		GPIO_Pin_15
	#define PORT_T2CH1     		 	GPIOA
	#define RCC_T2CH1       		RCC_APB2Periph_GPIOA

	#define GPIO_T2CH2      		GPIO_Pin_3
	#define PORT_T2CH2     		 	GPIOB
	#define RCC_T2CH2      			RCC_APB2Periph_GPIOB

	#define GPIO_T2CH3      		GPIO_Pin_10
	#define PORT_T2CH3     		 	GPIOB
	#define RCC_T2CH3       		RCC_APB2Periph_GPIOB

	#define GPIO_T2CH4      		GPIO_Pin_11
	#define PORT_T2CH4     		 	GPIOB
	#define RCC_T2CH4       		RCC_APB2Periph_GPIOB
#endif

#if 	TIM3_NORemap > 0				//TIM3没有使用端口重定义
	#define GPIO_T3CH1      		GPIO_Pin_6
	#define PORT_T3CH1     		 	GPIOA
	#define RCC_T3CH1       		RCC_APB2Periph_GPIOA

	#define GPIO_T3CH2      		GPIO_Pin_7
	#define PORT_T3CH2    		 	GPIOA
	#define RCC_T3CH2      			RCC_APB2Periph_GPIOA

	#define GPIO_T3CH3      		GPIO_Pin_0
	#define PORT_T3CH3     			GPIOB
	#define RCC_T3CH3      			RCC_APB2Periph_GPIOB

	#define GPIO_T3CH4      		GPIO_Pin_1
	#define PORT_T3CH4     			GPIOB
	#define RCC_T3CH4      			RCC_APB2Periph_GPIOB
#endif

#if 	TIM3_PartialRemap > 0					//TIM3使用部分端口重定义
	#define GPIO_T3CH1      		GPIO_Pin_4
	#define PORT_T3CH1     		 	GPIOB
	#define RCC_T3CH1       		RCC_APB2Periph_GPIOB

	#define GPIO_T3CH2      		GPIO_Pin_5
	#define PORT_T3CH2    		 	GPIOB
	#define RCC_T3CH2      			RCC_APB2Periph_GPIOB

	#define GPIO_T3CH3      		GPIO_Pin_0
	#define PORT_T3CH3     			GPIOB
	#define RCC_T3CH3      			RCC_APB2Periph_GPIOB

	#define GPIO_T3CH4      		GPIO_Pin_1
	#define PORT_T3CH4     			GPIOB
	#define RCC_T3CH4      			RCC_APB2Periph_GPIOB
#endif

#if  	TIM3_FullRemap > 0					//TIM3使用完全端口重定义
	#define GPIO_T3CH1      		GPIO_Pin_6
	#define PORT_T3CH1     			GPIOC
	#define RCC_T3CH1      			RCC_APB2Periph_GPIOC

	#define GPIO_T3CH2      		GPIO_Pin_7
	#define PORT_T3CH2     			GPIOC
	#define RCC_T3CH2      			RCC_APB2Periph_GPIOC

	#define GPIO_T3CH3      		GPIO_Pin_8
	#define PORT_T3CH3     			GPIOC
	#define RCC_T3CH3      			RCC_APB2Periph_GPIOC

	#define GPIO_T3CH4      		GPIO_Pin_9
	#define PORT_T3CH4     			GPIOC
	#define RCC_T3CH4      			RCC_APB2Periph_GPIOC
#endif


#if 	TIM4_NORemap > 0					//TIM4没有使用端口重定义
	#define GPIO_T4CH1      		GPIO_Pin_6
	#define PORT_T4CH1     			GPIOB
	#define RCC_T4CH1      			RCC_APB2Periph_GPIOB

	#define GPIO_T4CH2      		GPIO_Pin_7
	#define PORT_T4CH2     			GPIOB
	#define RCC_T4CH2      			RCC_APB2Periph_GPIOB

	#define GPIO_T4CH3      		GPIO_Pin_8
	#define PORT_T4CH3     			GPIOB
	#define RCC_T4CH3      			RCC_APB2Periph_GPIOB

	#define GPIO_T4CH4      		GPIO_Pin_9
	#define PORT_T4CH4     			GPIOB
	#define RCC_T4CH4      			RCC_APB2Periph_GPIOB
#endif

#if  	TIM4_Remap > 0					//TIM4使用端口重定义
	#define GPIO_T4CH1      		GPIO_Pin_12
	#define PORT_T4CH1     		 	GPIOD
	#define RCC_T4CH1       		RCC_APB2Periph_GPIOD

	#define GPIO_T4CH2      		GPIO_Pin_13
	#define PORT_T4CH2    		 	GPIOD
	#define RCC_T4CH2      			RCC_APB2Periph_GPIOD

	#define GPIO_T4CH3      		GPIO_Pin_14
	#define PORT_T4CH3     			GPIOD
	#define RCC_T4CH3      			RCC_APB2Periph_GPIOD

	#define GPIO_T4CH4      		GPIO_Pin_15
	#define PORT_T4CH4     			GPIOD
	#define RCC_T4CH4      			RCC_APB2Periph_GPIOD
#endif
//TIM5
	#define GPIO_T5CH1      		GPIO_Pin_0
	#define PORT_T5CH1     		 	GPIOA
	#define RCC_T5CH1       		RCC_APB2Periph_GPIOA

	#define GPIO_T5CH2      		GPIO_Pin_1
	#define PORT_T5CH2    		 	GPIOA
	#define RCC_T5CH2      			RCC_APB2Periph_GPIOA

	#define GPIO_T5CH3      		GPIO_Pin_2
	#define PORT_T5CH3     			GPIOA
	#define RCC_T5CH3      			RCC_APB2Periph_GPIOA

	#define GPIO_T5CH4      		GPIO_Pin_3
	#define PORT_T5CH4     			GPIOA
	#define RCC_T5CH4      			RCC_APB2Periph_GPIOA




void PWM_Init(u16 arr,u16 psc);

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM5_PWM_Init(u16 arr,u16 psc);
void TIM8_PWM_Init(u16 arr,u16 psc);

void TIMx_PWM_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc);

#endif
