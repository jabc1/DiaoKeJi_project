/*********************************************************************************
* 文件名		: show.C
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 处理与菜单有关的函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/

#include "OS.h"
#include "app.h"
#include <stdlib.h>
#include "page.h"
#include "gui_conf.h"
#include "NRF_CTRL.h"
extern void Show_Font(u16 x,u16 y,u8 *font,u8 mode);

GUI_FormStruct *Form1;
u8 Page = 0;
u32 GUIKJ[15];

void ShowDKState(u16 X,u16 Y)
{
	GUI_DispDec(DK.NRF.X,X,Y,7);
	GUI_DispDec(DK.NRF.Y,X,Y+16,7);	
	GUI_DispDec(DK.NRF.Z,X,Y+32,7);	
	GUI_DispDec(DK.NRF.N,X+80,Y,5);	
	GUI_DispDec(DK.NRF.NRF_N,X+80,Y+16,5);	
	GUI_DispDec(DK.NRF.queue,X+80,Y+32,5);		
	
	
}

void InitializeShow(void)
{
	Form1 = GUI_FormCreate(20);	
	GUI.CurrentForm = Form1;
}


void ShowTask(void)
{
	u32 message;
	
	InitializeShow();
	InitMenu();
	Init_MainPage();
  while(1)
	{    		 
		message = (u32)OS.PendMessageQueue(100);
		switch(GetMessageType(message))
		{
			case KeyMessageType:    							//按键消息  只有按键的消息发向这个任务的时候才会接收到消息
				break;
			case FuntionMessageType:                    //函数指针消息，直接以0x20或0x08来判断
			Function(message);
				break;	
			case FileListCloseMessageType:                    //文件列表被手动关闭消息
				Init_MainPage();
				break;
			case UseMessage_ShowDKState:                    //文件列表被手动关闭消息
				ShowDKState(20,150);

				break;			
			
			default:                                //软件定时器消息

				break;
		}
				switch(Page)
				{
					case PAGE_MAIN:    							//当前页面是系统页面	
						Page_Routine();
									ShowDKState(170,150);
						break;
					case PAGE_DINGWEI:    							//当前页面是系统设置页面			

						Page_DingWei_Routine();
									ShowDKState(20,150);
						break;		
					case PAGE_SHOWCN:    							//当前页面是显示控制页面			
				ShowDKState(20,10);
//						Page_ShowCN_Routine();
						break;		


					default:  										
						break;										

				}
	}
}

