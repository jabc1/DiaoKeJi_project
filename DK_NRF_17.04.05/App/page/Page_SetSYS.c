/*********************************************************************************
* 文件名		: page2.C
* 版本号    : V1.0.0
* 日期      : 2016/02/22
* 说明      : 图像显示函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/
#include "app.h"
#include "page.h"
#include "dk_conf.h"
#include "NRF_CTRL.h"

void Page_Speed_Fast_U(void)
{
	DK.SYS.Speed_Fast++;
	if(DK.SYS.Speed_Fast>50)
	{
		DK.SYS.Speed_Fast = 50;
	}
	GUI_DispDecAt(DK.SYS.Speed_Fast,55,95,3);	
	
}
void Page_Speed_Fast_D(void)
{
	if(DK.SYS.Speed_Fast<=10)
	{
		DK.SYS.Speed_Fast=10;		
	}
	else
	{
		DK.SYS.Speed_Fast--;		
	}
	GUI_DispDecAt(DK.SYS.Speed_Fast,55,95,3);		
}
void Page_Speed_XY_U(void)
{
	DK.SYS.Speed_XY++;
	if(DK.SYS.Speed_XY>50)
	{
		DK.SYS.Speed_XY = 50;
	}
	GUI_DispDecAt(DK.SYS.Speed_XY,120,95,3);	
	
}
void Page_Speed_XY_D(void)
{
	if(DK.SYS.Speed_XY<=10)
	{
		DK.SYS.Speed_XY=10;		
	}
	else
	{
		DK.SYS.Speed_XY--;		
	}
	GUI_DispDecAt(DK.SYS.Speed_XY,120,95,3);		
}

void Page_Speed_Z_U(void)
{
	DK.SYS.Speed_Z++;
	if(DK.SYS.Speed_Z>50)
	{
		DK.SYS.Speed_Z = 50;
	}
	GUI_DispDecAt(DK.SYS.Speed_Z,185,95,3);	
	
}
void Page_Speed_Z_D(void)
{
	if(DK.SYS.Speed_Z<=15)
	{
		DK.SYS.Speed_Z=15;		
	}
	else
	{
		DK.SYS.Speed_Z--;		
	}
	GUI_DispDecAt(DK.SYS.Speed_Z,185,95,3);		
}

void TFTShowSizeUp(void)
{
	DK.SYS.TFTShowSize++;
	if(DK.SYS.TFTShowSize>20)
	{
		DK.SYS.TFTShowSize = 20;
	}
	GUI_DispDecAt(DK.SYS.TFTShowSize,250,95,3);	
	
}
void TFTShowSizeDn(void)
{
	if(DK.SYS.TFTShowSize<=1)
	{
		DK.SYS.TFTShowSize=1;		
	}
	else
	{
		DK.SYS.TFTShowSize--;		
	}
	GUI_DispDecAt(DK.SYS.TFTShowSize,250,95,3);		
}




void Page_SpeedSetOK(void)
{
	DK.Ctrl = CTRL_STOP;
	Send_SZ_SYS();	
	while(NRF.Send == 1)	//等待上次发送完成
	{
		OS.DelayTimeTick(1);
	}	
	Init_MainPage();	
}


void Page_SetSYS(void)
{
	Page_Speed_Struct *GUI_P;
	GUI_P=(Page_Speed_Struct *)GUIKJ;
	
	Page = PAGE_SPEED;
	
	GUI_FormDeleteAllChild(GUI.CurrentForm);
	GUI_Clear(GUI.B_Color);

	GUI.Font = &Font16x24;
	GUI_PutStringAt("电机系统参数修改",60,20);
	GUI.Font = &Font8x16;	
	GUI_PutStringAt("快速",50,150);	
	GUI_PutStringAt("XY轴",115,150);	
	GUI_PutStringAt("Z轴",180,150);		
	GUI_PutStringAt("TFT",245,150);			
	GUI_P->B_FAST_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,45,70,40,20);
	GUI_ButtonSetString(GUI_P->B_FAST_U,"+","+");	
	GUI_P->B_FAST_U->Func_Down = Page_Speed_Fast_U;	
	GUI_P->B_FAST_U->Func_Long = Page_Speed_Fast_U;		
	GUI_P->B_FAST_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,45,120,40,20);
	GUI_ButtonSetString(GUI_P->B_FAST_D,"-","-");	
	GUI_P->B_FAST_D->Func_Down = Page_Speed_Fast_D;		
	GUI_P->B_FAST_D->Func_Long = Page_Speed_Fast_D;			
	GUI_P->B_XY_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,110,70,40,20);
	GUI_ButtonSetString(GUI_P->B_XY_U,"+","+");	
	GUI_P->B_XY_U->Func_Down = Page_Speed_XY_U;	
	GUI_P->B_XY_U->Func_Long = Page_Speed_XY_U;		
	GUI_P->B_XY_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,110,120,40,20);
	GUI_ButtonSetString(GUI_P->B_XY_D,"-","-");	
	GUI_P->B_XY_D->Func_Down = Page_Speed_XY_D;	
	GUI_P->B_XY_D->Func_Long = Page_Speed_XY_D;		
	GUI_P->B_Z_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,175,70,40,20);
	GUI_ButtonSetString(GUI_P->B_Z_U,"+","+");			
	GUI_P->B_Z_U->Func_Down = Page_Speed_Z_U;
	GUI_P->B_Z_U->Func_Long = Page_Speed_Z_U;	
	GUI_P->B_Z_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,175,120,40,20);	
	GUI_ButtonSetString(GUI_P->B_Z_D,"-","-");	
	GUI_P->B_Z_D->Func_Down = Page_Speed_Z_D;		
	GUI_P->B_Z_D->Func_Long = Page_Speed_Z_D;		
	GUI_P->B_T_UP = GUI_ButtonCreateAsChild(GUI.CurrentForm,240,70,50,20);
	GUI_ButtonSetString(GUI_P->B_T_UP,"+","+");				
	GUI_P->B_T_UP->Func_Down = TFTShowSizeUp;					
	GUI_P->B_T_UP->Func_Long = TFTShowSizeUp;
	GUI_P->B_T_DN = GUI_ButtonCreateAsChild(GUI.CurrentForm,240,120,50,20);
	GUI_ButtonSetString(GUI_P->B_T_DN,"-","-");	
	GUI_P->B_T_DN->Func_Down = TFTShowSizeDn;	
	GUI_P->B_T_DN->Func_Long = TFTShowSizeDn;		
	
	
	
	
	GUI_P->B_OK = GUI_ButtonCreateAsChild(GUI.CurrentForm,100,180,50,20);	
	GUI_ButtonSetString(GUI_P->B_OK,"确定","确定");			
	GUI_P->B_OK->Func_Down = Page_SpeedSetOK;
	
	GUI_P->B_Return = GUI_ButtonCreateAsChild(GUI.CurrentForm,200,180,50,20);	
	GUI_ButtonSetString(GUI_P->B_Return,"取消","取消");		
	GUI_P->B_Return->Func_Down = Init_MainPage;	

	DK.Ctrl = CTRL_STOP;
	Send_DK_Action(NRF_CX_SYS);			//查询DK系统状态
	while(NRF.Send == 1)	//等待上次发送完成
	{
		OS.DelayTimeTick(1);
	}	

	GUI_DispDecAt(DK.SYS.Speed_Fast,55,95,3);
	GUI_DispDecAt(DK.SYS.Speed_XY,120,95,3);
	GUI_DispDecAt(DK.SYS.Speed_Z,185,95,3);
	GUI_DispDecAt(DK.SYS.TFTShowSize,250,95,3);	
}






