/*********************************************************************************
* 文件名			: device_key.c
* 版本号    	: V2.0.0
* 日期      : 2015/07/5
* 说明      : 4*4键盘的驱动程序,可以随时扩展成m*n									  
* 用法			：
在 IN_Keyboard 和 OUT_Keyboard 中定义m*n键盘所用的端口

#define KEY_TIME1		30	//按键第一次时延时时间  =KEY_TIME1*每次扫描的周期20ms
#define KEY_TIME2		20	//按键第二次时延时时间  =KEY_TIME2*每次扫描的周期20ms

**********************************************************************************/



#include "device_key.h"
#include "device.h"
#include "Device_conf.h" 
#if (USE_MSOS == 1)
	#include "OS.h"
	u8 KeyMessageQueuePointer = 0;			//定义KEY信息发给那个信号指针		

	#if (USE_GUI == 1)
	extern 	u8 GUITaskPriority ;		/*GUI解析任务*/	
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

/**************************局部函数定义****************************************************/



	
	

#if (USE_MSOS == 1)
/*******************************************************************************
* 函数名	: SetKeyMessagePointer
* 描述	    : 设置当前任务为KEY发送的任务指针
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void SetKeyMessagePointer(u8 MessagePointer)
{
	KeyMessageQueuePointer = MessagePointer;	
}
/*******************************************************************************
* 函数名	: PostKeyMessageQueue
* 描述	    : 发送KEY消息
* 输入参数  : messageType: 消息类型
*             data:消息值，MessageTimer类型为32bit地址,其他类型下都是24bit数据
* 返回参数  : 无
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
		ReadKeyboard();								//矩阵键盘按键判断
	#endif		
	
	#if(ISUSESEPARATEKEY)				
		ReadSeparateKey();																	//独立键盘按键判断
	#endif		
} 


void InitializeKey(void)
{	

	#if(ISUSEKEYBOARD)
		Initialize_KeyboardIO();					//矩阵键盘初始化
	#endif
	#if(ISUSESEPARATEKEY)
		Initialize_SeparateKeyIO();				//独立键盘初始化
	#endif	
	#if (USE_MSOS == 1)
		OS.Systick.Register(Systick50, KeySystick50Routine);
	#endif	
}
  




