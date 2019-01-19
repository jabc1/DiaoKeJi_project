/*********************************************************************************
* 文件名		: Page_main.C
* 说明      : 主页显示内容					  
* 用法			：
* 版本说明	：
**********************************************************************************/

#include <stdlib.h>
#include "page.h"
#include "dk_conf.h"
#include "NRF_Ctrl.h"
#include "app.h"
extern void Show_Font(u16 x,u16 y,u8 *font,u8 mode);


GUIMenu MainMenu = { //定义主菜单
 6,                  //默认显示0-3项，总共4项，当前选择第0项
  {
		"系统设置    >",				
		"定位",		
		"选择SD卡文件",
		"串口通信雕刻",
		"返回坐标原点",
		"关闭主轴",
  }
};	
GUIMenu SysMenu = {//系统设置菜单
 5,
 {
  "雕刻设置",	 
  "时间设置",
  "日期设置",	 
  "串口设置",	 
	 "USB控制"	 
	}
};	


void MotoGotoZero(void)
{
	while(NRF.Send == 1)	//等待上次发送完成
	{
		OS.DelayTimeTick(1);
	}		
	Send_DK_Action(NRF_KZ_GOTOZERO);	
}
void MainMotoStop(void)
{
	while(NRF.Send == 1)	//等待上次发送完成
	{
		OS.DelayTimeTick(1);
	}		
	Send_DK_Action(NRF_KZ_MAINMOTO_OFF);	
}
void InitMenu(void)
{
	MainMenu.parent = NULL;		//没有上级菜单，表示是顶级菜单
	MainMenu.subMenus = malloc(4*MainMenu.itemCount);
	MainMenu.subMenus[0] = &SysMenu;//第1到3项没有子菜单置null,选择后程序会调用func中相应项中的函数
	MainMenu.subMenus[1] = NULL;
	MainMenu.subMenus[2] = NULL;
	MainMenu.subMenus[3] = NULL;
	MainMenu.subMenus[4] = NULL;
	MainMenu.subMenus[5] = NULL;	
	MainMenu.func = malloc(4*MainMenu.itemCount);
	MainMenu.func[0] = NULL;
	MainMenu.func[1] = &Page_DingWei;//当选择了并按了确定，会执行func函数	
	MainMenu.func[2] = &FileSelectShow;	
	MainMenu.func[3] = NULL;//&Page_DK_Comm;
	MainMenu.func[4] = &MotoGotoZero;	
	MainMenu.func[5] = &MainMotoStop;


	SysMenu.subMenus = malloc(SysMenu.itemCount*sizeof(&SysMenu));
	SysMenu.subMenus[0] = NULL;//第1到3项没有子菜单置null,选择后程序会调用func中相应项中的函数
	SysMenu.subMenus[1] = NULL;
	SysMenu.subMenus[2] = NULL;
	SysMenu.subMenus[3] = NULL;
	SysMenu.subMenus[4] = NULL;

	
	SysMenu.parent = &MainMenu;//上级菜单指针	
	SysMenu.func = malloc(SysMenu.itemCount*4);
	SysMenu.func[0] = &Page_SetSYS;
	SysMenu.func[1] = &Page_SetTime;
	SysMenu.func[2] = &Page_SetDate;
	SysMenu.func[3] = NULL;//&Page_SetCommSet;//当选择了并按了确定，会执行func函数
	SysMenu.func[4] = NULL;//&Page_SetUSB;//当选择了并按了确定，会执行func函数

}


void Init_MainPage(void)
{
	Page_Main_Struct *GUI_P;
	GUI_P = (Page_Main_Struct *)GUIKJ;
	
	
	Page = PAGE_MAIN;
//	GUI_Clear(GUI.B_Color);
	GUI_FormReset(GUI.CurrentForm);	
	GUI.Font = &Font16x24;
	GUI_PutStringAt("欢迎使用雕刻机",70,40);	
	GUI.Font = &Font8x16;		
	GUI_P->L_Date = GUI_LableCreateAsChild(GUI.CurrentForm,80,10,8);	
	GUI_P->L_Time = GUI_LableCreateAsChild(GUI.CurrentForm,150,10,8);	
	GUI_P->L_Date->Font = &Font8x16;
	GUI_P->L_Time->Font = &Font8x16;	
	GUI_LableSetDate(GUI_P->L_Date,(u8*)(&RTC_Calendar));	
	GUI_LableSetDate(GUI_P->L_Time,(u8*)(&RTC_Calendar.Hour));		
	GUI_P->Menu_P = GUI_MenuCreateAsChild(GUI.CurrentForm,30,70,160,140);
	GUI_SetCurrentMenus(GUI_P->Menu_P,&MainMenu);
	DK.Ctrl = CTRL_STOP;			//停止
	while(NRF.Send == 1)	//等待上次发送完成
	{
		OS.DelayTimeTick(1);
	}	
	Send_DK_Action(NRF_CX_STATE);			//查询DK状态;
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
		if(NRF.Send == 0)	//等待上次发送完成
		{		
				Send_DK_Action(NRF_CX_STATE);			//查询DK状态;		
		}
	}
	
}







