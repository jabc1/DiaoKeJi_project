#ifndef __DEVICE_KEY_H_
#define __DEVICE_KEY_H_

#include "stm32f10x.h" 

#define ISUSEKEYBOARD					0					//�Ƿ�ʹ�þ������
#define ISUSESEPARATEKEY			1					//�Ƿ�ʹ�ö�������

#define KEY_TIME1		40	//������һ��ʱ��ʱʱ��  =KEY_TIME1*ÿ��ɨ�������  ���ֵΪ255
#define KEY_TIME2		10	//�����ڶ���ʱ��ʱʱ��  =KEY_TIME2*ÿ��ɨ�������  ���ֵΪ255
#define DoubleHitInterval  10		//������˫���󶯼��



void PostKeyMessageQueue(u32 key);
void InitializeKey(void);
void SetKeyMessagePointer(void);
#endif

