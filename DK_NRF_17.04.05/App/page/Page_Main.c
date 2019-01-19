/*********************************************************************************
* �ļ���		: Page_main.C
* ˵��      : ��ҳ��ʾ����					  
* �÷�			��
* �汾˵��	��
**********************************************************************************/

#include <stdlib.h>
#include "page.h"
#include "dk_conf.h"
#include "NRF_Ctrl.h"
#include "app.h"
extern void Show_Font(u16 x,u16 y,u8 *font,u8 mode);


GUIMenu MainMenu = { //�������˵�
 6,                  //Ĭ����ʾ0-3��ܹ�4���ǰѡ���0��
  {
		"ϵͳ����    >",				
		"��λ",		
		"ѡ��SD���ļ�",
		"����ͨ�ŵ��",
		"��������ԭ��",
		"�ر�����",
  }
};	
GUIMenu SysMenu = {//ϵͳ���ò˵�
 5,
 {
  "�������",	 
  "ʱ������",
  "��������",	 
  "��������",	 
	 "USB����"	 
	}
};	


void MotoGotoZero(void)
{
	while(NRF.Send == 1)	//�ȴ��ϴη������
	{
		OS.DelayTimeTick(1);
	}		
	Send_DK_Action(NRF_KZ_GOTOZERO);	
}
void MainMotoStop(void)
{
	while(NRF.Send == 1)	//�ȴ��ϴη������
	{
		OS.DelayTimeTick(1);
	}		
	Send_DK_Action(NRF_KZ_MAINMOTO_OFF);	
}
void InitMenu(void)
{
	MainMenu.parent = NULL;		//û���ϼ��˵�����ʾ�Ƕ����˵�
	MainMenu.subMenus = malloc(4*MainMenu.itemCount);
	MainMenu.subMenus[0] = &SysMenu;//��1��3��û���Ӳ˵���null,ѡ����������func����Ӧ���еĺ���
	MainMenu.subMenus[1] = NULL;
	MainMenu.subMenus[2] = NULL;
	MainMenu.subMenus[3] = NULL;
	MainMenu.subMenus[4] = NULL;
	MainMenu.subMenus[5] = NULL;	
	MainMenu.func = malloc(4*MainMenu.itemCount);
	MainMenu.func[0] = NULL;
	MainMenu.func[1] = &Page_DingWei;//��ѡ���˲�����ȷ������ִ��func����	
	MainMenu.func[2] = &FileSelectShow;	
	MainMenu.func[3] = NULL;//&Page_DK_Comm;
	MainMenu.func[4] = &MotoGotoZero;	
	MainMenu.func[5] = &MainMotoStop;


	SysMenu.subMenus = malloc(SysMenu.itemCount*sizeof(&SysMenu));
	SysMenu.subMenus[0] = NULL;//��1��3��û���Ӳ˵���null,ѡ����������func����Ӧ���еĺ���
	SysMenu.subMenus[1] = NULL;
	SysMenu.subMenus[2] = NULL;
	SysMenu.subMenus[3] = NULL;
	SysMenu.subMenus[4] = NULL;

	
	SysMenu.parent = &MainMenu;//�ϼ��˵�ָ��	
	SysMenu.func = malloc(SysMenu.itemCount*4);
	SysMenu.func[0] = &Page_SetSYS;
	SysMenu.func[1] = &Page_SetTime;
	SysMenu.func[2] = &Page_SetDate;
	SysMenu.func[3] = NULL;//&Page_SetCommSet;//��ѡ���˲�����ȷ������ִ��func����
	SysMenu.func[4] = NULL;//&Page_SetUSB;//��ѡ���˲�����ȷ������ִ��func����

}


void Init_MainPage(void)
{
	Page_Main_Struct *GUI_P;
	GUI_P = (Page_Main_Struct *)GUIKJ;
	
	
	Page = PAGE_MAIN;
//	GUI_Clear(GUI.B_Color);
	GUI_FormReset(GUI.CurrentForm);	
	GUI.Font = &Font16x24;
	GUI_PutStringAt("��ӭʹ�õ�̻�",70,40);	
	GUI.Font = &Font8x16;		
	GUI_P->L_Date = GUI_LableCreateAsChild(GUI.CurrentForm,80,10,8);	
	GUI_P->L_Time = GUI_LableCreateAsChild(GUI.CurrentForm,150,10,8);	
	GUI_P->L_Date->Font = &Font8x16;
	GUI_P->L_Time->Font = &Font8x16;	
	GUI_LableSetDate(GUI_P->L_Date,(u8*)(&RTC_Calendar));	
	GUI_LableSetDate(GUI_P->L_Time,(u8*)(&RTC_Calendar.Hour));		
	GUI_P->Menu_P = GUI_MenuCreateAsChild(GUI.CurrentForm,30,70,160,140);
	GUI_SetCurrentMenus(GUI_P->Menu_P,&MainMenu);
	DK.Ctrl = CTRL_STOP;			//ֹͣ
	while(NRF.Send == 1)	//�ȴ��ϴη������
	{
		OS.DelayTimeTick(1);
	}	
	Send_DK_Action(NRF_CX_STATE);			//��ѯDK״̬;
//	Menu_P->BColor = GUI.B_Color;	
}



void Page_Routine(void)
{
	Page_Main_Struct *GUI_P;
	GUI_P = (Page_Main_Struct *)GUIKJ;
	GUI_LableSetDate(GUI_P->L_Date,(u8*)(&RTC_Calendar));	
	GUI_LableSetDate(GUI_P->L_Time,(u8*)(&RTC_Calendar.Hour));	

	if(DK.Ctrl == CTRL_STOP)
	{
		if(NRF.Send == 0)	//�ȴ��ϴη������
		{		
				Send_DK_Action(NRF_CX_STATE);			//��ѯDK״̬;		
		}
	}
	
}







