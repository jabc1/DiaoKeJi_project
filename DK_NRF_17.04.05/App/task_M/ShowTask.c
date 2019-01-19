/*********************************************************************************
* �ļ���		: show.C
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ������˵��йصĺ���					  
* �÷�			��
* �汾˵��	��
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
			case KeyMessageType:    							//������Ϣ  ֻ�а�������Ϣ������������ʱ��Ż���յ���Ϣ
				break;
			case FuntionMessageType:                    //����ָ����Ϣ��ֱ����0x20��0x08���ж�
			Function(message);
				break;	
			case FileListCloseMessageType:                    //�ļ��б��ֶ��ر���Ϣ
				Init_MainPage();
				break;
			case UseMessage_ShowDKState:                    //�ļ��б��ֶ��ر���Ϣ
				ShowDKState(20,150);

				break;			
			
			default:                                //�����ʱ����Ϣ

				break;
		}
				switch(Page)
				{
					case PAGE_MAIN:    							//��ǰҳ����ϵͳҳ��	
						Page_Routine();
									ShowDKState(170,150);
						break;
					case PAGE_DINGWEI:    							//��ǰҳ����ϵͳ����ҳ��			

						Page_DingWei_Routine();
									ShowDKState(20,150);
						break;		
					case PAGE_SHOWCN:    							//��ǰҳ������ʾ����ҳ��			
				ShowDKState(20,10);
//						Page_ShowCN_Routine();
						break;		


					default:  										
						break;										

				}
	}
}

