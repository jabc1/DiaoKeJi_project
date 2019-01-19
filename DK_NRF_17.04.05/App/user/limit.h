#ifndef __LIMIT_H
#define __LIMIT_H	 
#include "stm32f10x.h"		    
#include "device_conf.h"	

/*											
								|YR
					XL		|				XR
					------+-------
								|
					+			|YL
			坐标0,0 		

*/
	//X轴左限位
	#define LIMIT_XL_PIN      GPIO_Pin_6
	#define LIMIT_XL_GPIO  		GPIOB
	#define LIMIT_XL_RCC  		RCC_APB2Periph_GPIOB
	#define LIMIT_XL      		(PBIn(6))
	//X轴右限位
	#define LIMIT_XR_PIN      GPIO_Pin_3
	#define LIMIT_XR_GPIO  		GPIOB
	#define LIMIT_XR_RCC  		RCC_APB2Periph_GPIOB
	#define LIMIT_XR      		(PBIn(3))	
	//Y轴左限位
	#define LIMIT_YL_PIN      GPIO_Pin_5
	#define LIMIT_YL_GPIO  		GPIOB
	#define LIMIT_YL_RCC  		RCC_APB2Periph_GPIOB
	#define LIMIT_YL      		(PBIn(5))
	//Y轴右限位
	#define LIMIT_YR_PIN      GPIO_Pin_4
	#define LIMIT_YR_GPIO  		GPIOB
	#define LIMIT_YR_RCC  		RCC_APB2Periph_GPIOB
	#define LIMIT_YR      		(PBIn(4))		
	
void Limit(void);
void Moto_Limit_Init(void);//IO初始化
		 					    
#endif

