#ifndef __DEVICE_ADC_H_
#define __DEVICE_ADC_H_

#include "stm32f10x.h" 

void InitializeADC1Channel(u8 *ADCChannel,u8 Num,u8 ADC_SampleTime,u32 *ADCValue);
#endif

