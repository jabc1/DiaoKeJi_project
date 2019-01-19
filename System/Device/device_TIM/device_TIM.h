#ifndef __DEVICE_TIM_H
#define __DEVICE_TIM_H
#include "stm32f10x.h"

void TIM2_Config(u16 arr,u16 psc);

void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc); 
 
#endif
