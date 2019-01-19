#ifndef __DEVICE_KEY_SYS_H_
#define __DEVICE_KEY_SYS_H_

#include "stm32f10x.h" 
#include "device_conf.h"

#define KEYON 				0x01
#define KEYLONG 			0x02
#define KEYOFF				0x04

	/*键盘控制的时间间隔*/
	#define KEY_TIME1		40	//按键第一次时延时时间  =KEY_TIME1*每次扫描的周期  最大值为255
	#define KEY_TIME2		10	//按键第二次时延时时间  =KEY_TIME2*每次扫描的周期  最大值为255
	#define DoubleHitInterval  10		//防连续双击误动间隔	
	
/***********数据结构定义*******************************************/
typedef struct 	   //按键数据类型定义
{
	u8 Cont;
	u8 State;	
} KEY_TypeDef ;	




#if (USE_MSOS == 1)
	#define PostSeparateKey(Key) 		PostKeyMessageQueue(Key) 
	#define PostKeyboard(Key) 		PostKeyMessageQueue(Key) 
#else
	#define PostSeparateKey(Key)  		(void)0 				//不用MSOS的时候改为对应的调用函数
	#define PostKeyboard(Key)  		(void)0 				//不用MSOS的时候改为对应的调用函数
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

