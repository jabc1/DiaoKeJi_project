#ifndef __DEVICE_KEY_H_
#define __DEVICE_KEY_H_

#include "stm32f10x.h" 

#define ISUSEKEYBOARD					0					//是否使用矩阵键盘
#define ISUSESEPARATEKEY			1					//是否使用独立按键

#define KEY_TIME1		40	//按键第一次时延时时间  =KEY_TIME1*每次扫描的周期  最大值为255
#define KEY_TIME2		10	//按键第二次时延时时间  =KEY_TIME2*每次扫描的周期  最大值为255
#define DoubleHitInterval  10		//防连续双击误动间隔



void PostKeyMessageQueue(u32 key);
void InitializeKey(void);
void SetKeyMessagePointer(void);
#endif

