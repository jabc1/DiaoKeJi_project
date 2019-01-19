#ifndef __DEVICE_KEY_SYS_H_
#define __DEVICE_KEY_SYS_H_

#include "stm32f10x.h" 
#include "device_conf.h"

#define KEYON 				0x01
#define KEYLONG 			0x02
#define KEYOFF				0x04

	/*���̿��Ƶ�ʱ����*/
	#define KEY_TIME1		40	//������һ��ʱ��ʱʱ��  =KEY_TIME1*ÿ��ɨ�������  ���ֵΪ255
	#define KEY_TIME2		10	//�����ڶ���ʱ��ʱʱ��  =KEY_TIME2*ÿ��ɨ�������  ���ֵΪ255
	#define DoubleHitInterval  10		//������˫���󶯼��	
	
/***********���ݽṹ����*******************************************/
typedef struct 	   //�����������Ͷ���
{
	u8 Cont;
	u8 State;	
} KEY_TypeDef ;	




#if (USE_MSOS == 1)
	#define PostSeparateKey(Key) 		PostKeyMessageQueue(Key) 
	#define PostKeyboard(Key) 		PostKeyMessageQueue(Key) 
#else
	#define PostSeparateKey(Key)  		(void)0 				//����MSOS��ʱ���Ϊ��Ӧ�ĵ��ú���
	#define PostKeyboard(Key)  		(void)0 				//����MSOS��ʱ���Ϊ��Ӧ�ĵ��ú���
#endif









void PostKeyMessageQueue(u32 key);
void InitializeKey(void);
void SetKeyMessagePointer(u8 MessagePointer);

#if(ISUSESEPARATEKEY == 1)		
	void Initialize_SeparateKeyIO(void);
	void ReadSeparateKey(void);

#endif

#if(ISUSEKEYBOARD == 1)		
void ReadKeyboard(void);
void Initialize_KeyboardIO(void);
#endif

#endif

