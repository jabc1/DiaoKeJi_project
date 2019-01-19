/*********************************************************************************
* 文件名		: GUI_OS.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明   	: GUI针对OS操作系统进行系统刷新							  
* 用法 		：
	
**********************************************************************************/

#include "GUI_conf.h" 
#if GUI_USEMSOS
#include "os.h"

#if GUI_USEKEY				//如果使用按键			
	#include "GUI_Key.h" 
#endif
#if GUI_USETOUCH				//如果使用触摸屏的话
	#include "GUI_Touch.h" 		
#endif


u8 GUITaskPriority ;		/*GUI解析任务*/


void GUITask (void);

/*******************************************************************************
* 函数名	: GUITask
* 描述	    : GUITask任务，必须为最低优先级任务，解析窗体Form内的各个控件用于显示
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void GUITask (void)
{
	u32 GUI_message;
	
  while(1)
	{    		 
		GUI_message = (u32)OS.PendMessageQueue(GUISHOWTIME);		
//			
		GUI.B_Sate_P->GUI_OS_Run = 1;
		switch(GetMessageType(GUI_message))
		{
			#if GUI_USEKEY				//如果使用按键						
				case KeyMessageType:		
					GUI.KeyState  = ((u8 *)(&(GUI_message)))[1];
					GUI.KeyNum = ((u8 *)(&(GUI_message)))[0];
					switch(GUI.KeyState)		//0xFF
					{											
						case GUI_KT_DOWN:		//01
							GUI_KeyDown();
							break;
						case GUI_KT_UP:		//04
							GUI_KeyUp();
							break;
						case GUI_KT_LONG:		//02
							GUI_KeyLong();
							break;	

							}
				break;
			#endif		
			#if GUI_USETOUCH				//如果使用按键								
				case TouchMessageType:	//0xFE
					GUI.TouchState  = (((u8 *)(&(GUI_message)))[2])>>4;
					GUI.Touch_X = (GUI_message>>10)&0x3FF;					
					GUI.Touch_Y = GUI_message&0x3FF;					
					switch(GUI.TouchState)
					{									
						case GUI_KT_DOWN:					//第一次确认触摸屏按下
							TouchDownTime();
							break;
						case GUI_KT_LONG:			//确认触摸屏重复按下
							TouchLongTime();
							break;
						case GUI_KT_UP:			//确认触摸屏放开
							TouchUpTime();
							break;	
					}
					break;
			#endif			
			case FuntionMessageType:                    //函数指针消息，直接以0x20或0x08来判断
				GUI_Func(GUI_message);
				break;	

			default:                                
				break;
		}
		
		if(GUI.CurrentForm->ID == GUIFormID)
		{
			GUI_FormShowNow(GUI.CurrentForm);	//刷新控件
		}	 		
		GUI.B_Sate_P->GUI_OS_Run = 0;
	}
}

/*******************************************************************************
* 函数名	: GUICreateTask
* 描述	    : 创建GUITask任务，必须为最低优先级任务，解析窗体Form内的各个控件用于显示
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void GUICreateTask(void)
{	
	GUITaskPriority = OS.CreateTask(GUITask);
}
void GUI_RunFunction(u8 Priority,GUI_func RegisterFunction)
{
	OS.PostMessageQueue(Priority, RegisterFunction);
}

void GUI_FL_OpenFile(u8 Priority,u32 *FileList_P)
{
	u32 FileListMessage;
	FileListMessage = (((u32)GUI_FL_FuncMessageType<<24) | (((u32)FileList_P) & 0xFFFFFF));			
	OS.PostMessageQueue(Priority, (void *)(FileListMessage));		//发送长时间按下功能键的键值
}
void GUI_FL_Close(u8 Priority,u32 *FileList_P)
{
	u32 FileListMessage;
	FileListMessage = (((u32)GUI_FL_CloseMessageType<<24) | (((u32)FileList_P) & 0xFFFFFF));			
	OS.PostMessageQueue(Priority, (void *)(FileListMessage));		//发送长时间按下功能键的键值
}
#endif
u8 GUI_FormReset(GUI_FormStruct *GUI_Form_P)	
{
	if(GUI_Form_P->ID != GUIFormID) return 1;	
	while(GUI.B_Sate_P->GUI_OS_Run == 1)
	{
		OS.DelayTimeTick(1);
	}
	GUI_FormDeleteAllChild(GUI_Form_P);
	GUI_Clear(GUI.B_Color);
	return 0;
}


