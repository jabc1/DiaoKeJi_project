/*********************************************************************************
* �ļ���		: GUI_OS.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��   	: GUI���OS����ϵͳ����ϵͳˢ��							  
* �÷� 		��
	
**********************************************************************************/

#include "GUI_conf.h" 
#if GUI_USEMSOS
#include "os.h"

#if GUI_USEKEY				//���ʹ�ð���			
	#include "GUI_Key.h" 
#endif
#if GUI_USETOUCH				//���ʹ�ô������Ļ�
	#include "GUI_Touch.h" 		
#endif


u8 GUITaskPriority ;		/*GUI��������*/


void GUITask (void);

/*******************************************************************************
* ������	: GUITask
* ����	    : GUITask���񣬱���Ϊ������ȼ����񣬽�������Form�ڵĸ����ؼ�������ʾ
* �������  : ��
* ���ز���  : ��
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
			#if GUI_USEKEY				//���ʹ�ð���						
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
			#if GUI_USETOUCH				//���ʹ�ð���								
				case TouchMessageType:	//0xFE
					GUI.TouchState  = (((u8 *)(&(GUI_message)))[2])>>4;
					GUI.Touch_X = (GUI_message>>10)&0x3FF;					
					GUI.Touch_Y = GUI_message&0x3FF;					
					switch(GUI.TouchState)
					{									
						case GUI_KT_DOWN:					//��һ��ȷ�ϴ���������
							TouchDownTime();
							break;
						case GUI_KT_LONG:			//ȷ�ϴ������ظ�����
							TouchLongTime();
							break;
						case GUI_KT_UP:			//ȷ�ϴ������ſ�
							TouchUpTime();
							break;	
					}
					break;
			#endif			
			case FuntionMessageType:                    //����ָ����Ϣ��ֱ����0x20��0x08���ж�
				GUI_Func(GUI_message);
				break;	

			default:                                
				break;
		}
		
		if(GUI.CurrentForm->ID == GUIFormID)
		{
			GUI_FormShowNow(GUI.CurrentForm);	//ˢ�¿ؼ�
		}	 		
		GUI.B_Sate_P->GUI_OS_Run = 0;
	}
}

/*******************************************************************************
* ������	: GUICreateTask
* ����	    : ����GUITask���񣬱���Ϊ������ȼ����񣬽�������Form�ڵĸ����ؼ�������ʾ
* �������  : ��
* ���ز���  : ��
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
	OS.PostMessageQueue(Priority, (void *)(FileListMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
}
void GUI_FL_Close(u8 Priority,u32 *FileList_P)
{
	u32 FileListMessage;
	FileListMessage = (((u32)GUI_FL_CloseMessageType<<24) | (((u32)FileList_P) & 0xFFFFFF));			
	OS.PostMessageQueue(Priority, (void *)(FileListMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
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


