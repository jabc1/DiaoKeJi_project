#ifndef __DEVICE_KEYBOARD_H_
#define __DEVICE_KEYBOARD_H_

#include "stm32f10x.h" 

#define ISUSEKEYBOARD					0					//�Ƿ�ʹ�þ������
#define ISUSESEPARATEKEY			1					//�Ƿ�ʹ�ö�������





void PostKeyMessageQueue(u32 key);
void InitializeKey(void);
void SetKeyMessagePointer(void);
#endif

