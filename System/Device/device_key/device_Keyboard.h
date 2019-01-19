#ifndef __DEVICE_KEYBOARD_H_
#define __DEVICE_KEYBOARD_H_

#include "stm32f10x.h" 

#define ISUSEKEYBOARD					0					//是否使用矩阵键盘
#define ISUSESEPARATEKEY			1					//是否使用独立按键





void PostKeyMessageQueue(u32 key);
void InitializeKey(void);
void SetKeyMessagePointer(void);
#endif

