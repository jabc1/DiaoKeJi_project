/*********************************************************************************
* �ļ���			: device_key.c
* �汾��    	: V2.0.0
* ����      : 2015/07/5
* ˵��      : 4*4���̵���������,������ʱ��չ��m*n									  
* �÷�			��
�� IN_Keyboard �� OUT_Keyboard �ж���m*n�������õĶ˿�

#define KEY_TIME1		30	//������һ��ʱ��ʱʱ��  =KEY_TIME1*ÿ��ɨ�������20ms
#define KEY_TIME2		20	//�����ڶ���ʱ��ʱʱ��  =KEY_TIME2*ÿ��ɨ�������20ms

**********************************************************************************/



#include "device_key.h"
#include "device.h"
#include "Device_conf.h" 
#if (USE_MSOS == 1)
	#include "OS.h"
	u8 KeyMessageQueuePointer = 0;			//����KEY��Ϣ�����Ǹ��ź�ָ��		

	#if (USE_GUI == 1)
	extern 	u8 GUITaskPriority ;		/*GUI��������*/	
	#define KeyMessageToTaskPriority	GUITaskPriority
	#else
	#define KeyMessageToTaskPriority KeyMessageQueuePointer

	#endif

#endif



#if(ISUSEKEYBOARD)	
	#include "device_Keyboard.c"
#endif		
#if(ISUSESEPARATEKEY)			
	#include "device_Separatekey.c"	
#endif		

/**************************�ֲ���������****************************************************/



	
	

#if (USE_MSOS == 1)
/*******************************************************************************
* ������	: SetKeyMessagePointer
* ����	    : ���õ�ǰ����ΪKEY���͵�����ָ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SetKeyMessagePointer(u8 MessagePointer)
{
	KeyMessageQueuePointer = MessagePointer;	
}
/*******************************************************************************
* ������	: PostKeyMessageQueue
* ����	    : ����KEY��Ϣ
* �������  : messageType: ��Ϣ����
*             data:��Ϣֵ��MessageTimer����Ϊ32bit��ַ,���������¶���24bit����
* ���ز���  : ��
*******************************************************************************/
void PostKeyMessageQueue(u32 key)
{
	u32 Message;
	if (KeyMessageToTaskPriority >=TaskSum) return;		
	Message = KeyMessageType;
	OS.PostMessageQueue(KeyMessageToTaskPriority, (void *)((Message<<24) | key));		
}
#endif
void KeySystick50Routine(void)
{
	#if(ISUSEKEYBOARD)	
		ReadKeyboard();								//������̰����ж�
	#endif		
	
	#if(ISUSESEPARATEKEY)				
		ReadSeparateKey();																	//�������̰����ж�
	#endif		
} 


void InitializeKey(void)
{	

	#if(ISUSEKEYBOARD)
		Initialize_KeyboardIO();					//������̳�ʼ��
	#endif
	#if(ISUSESEPARATEKEY)
		Initialize_SeparateKeyIO();				//�������̳�ʼ��
	#endif	
	#if (USE_MSOS == 1)
		OS.Systick.Register(Systick50, KeySystick50Routine);
	#endif	
}
  




