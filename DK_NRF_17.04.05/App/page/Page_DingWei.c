/*********************************************************************************
* 文件名		: page.C
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 处理与菜单有关的函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/
#include "app.h"
#include "page.h"
#include "DK_conf.h"
#include "NRF_CTRL.h"



u8 DW_Speed ;
u8 DW_XY_Key;
u8 DW_Z_Key;


void DW_Up(void)
{	
	DW_Z_Key = 0;
	DW_XY_Key = 0;
	DW_Speed =0;
	DW_Speed = 0;
}
void DW_ZS_Down(void)
{	
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_ZS;			
}


void DW_S_Down(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_S;			
}

void DW_YS_Down(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_YS;			
}
void DW_Z_Down(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_Z;			
}

void DW_Y_Down(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_Y;			
}

void DW_ZX_Down(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_ZX;			
}

void DW_X_Down(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_X;			
}

void DW_YX_Down(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_YX;			
}

void DW_XY_0(void)
{
	DW_Z_Key = 0;
	DW_XY_Key = DW_NUM_XY_0;			
}
void DW_Z_Dn_Down(void)
{
	DW_Z_Key = DW_NUM_Z_DN;
	DW_XY_Key = DW_NUM_NULL;	
}

void DW_Z_Up_Down(void)
{
	DW_Z_Key = DW_NUM_Z_UP;
	DW_XY_Key = DW_NUM_NULL;
}
void DW_Z_0(void)
{
	DW_Z_Key = DW_NUM_Z_0;
	DW_XY_Key = 0;
}
void DW_MainMotoOn(void)
{
	DK.State_B_P->Spindle_On = 1;
}
void DW_MainMotoOff(void)
{
	DK.State_B_P->Spindle_On = 0;
}
void Page_DW_Return(void)
{
	while(NRF.Send == 1)	//等待上次发送完成
	{
		OS.DelayTimeTick(1);
	}			
	Send_DK_Action(NRF_KZ_MOTO_OFF);		
	Init_MainPage();
}
void Page_DingWei(void)
{
	
	Page_DingWei_Struct *GUI_P;

	DW_Speed = 0;//定位时移动的倍率
	DW_XY_Key = 0;//定位时移动的倍率	
	DW_Z_Key = 0;//定位时移动的倍率	
  DK.State_B_P->Spindle_On = 0;

	while(NRF.Send == 1)	//等待上次发送完成
	{
		OS.DelayTimeTick(1);
	}			
	Send_DK_Action(NRF_KZ_MOTO_ON);	
	
	GUI_P=(Page_DingWei_Struct *)GUIKJ;
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);
	Page = PAGE_DINGWEI;
	
	GUI_P->B_ZS = GUI_ButtonCreateAsChild(GUI.CurrentForm,20,60,40,20);
	GUI_ButtonSetString(GUI_P->B_ZS,"左上","左上");	
	GUI_P->B_ZS->Func_Down = DW_ZS_Down;	
	GUI_P->B_ZS->Func_Up = DW_Up;		
	GUI_P->B_ZS->OsPriority = 2;

	GUI_P->B_S = GUI_ButtonCreateAsChild(GUI.CurrentForm,70,60,40,20);
	GUI_ButtonSetString(GUI_P->B_S,"上","上");	
	GUI_P->B_S->Func_Down = DW_S_Down;	
	GUI_P->B_S->Func_Up = DW_Up;		
	GUI_P->B_S->OsPriority = 2;	
	
	GUI_P->B_YS = GUI_ButtonCreateAsChild(GUI.CurrentForm,120,60,40,20);
	GUI_ButtonSetString(GUI_P->B_YS,"右上","右上");	
	GUI_P->B_YS->Func_Down = DW_YS_Down;	
	GUI_P->B_YS->Func_Up = DW_Up;		
	GUI_P->B_YS->OsPriority = 2;	
	
	GUI_P->B_Z = GUI_ButtonCreateAsChild(GUI.CurrentForm,20,90,40,20);
	GUI_ButtonSetString(GUI_P->B_Z,"左","左");	
	GUI_P->B_Z->Func_Down = DW_Z_Down;	
	GUI_P->B_Z->Func_Up = DW_Up;		
	GUI_P->B_Z->OsPriority = 2;	
	
	GUI_P->B_XY_0 = GUI_ButtonCreateAsChild(GUI.CurrentForm,70,90,40,20);
	GUI_ButtonSetString(GUI_P->B_XY_0,"原点","原点");	
	GUI_P->B_XY_0->Func_Down = DW_XY_0;	
	GUI_P->B_XY_0->Func_Up = DW_Up;	
	GUI_P->B_XY_0->OsPriority = 2;		
	
	GUI_P->B_Y = GUI_ButtonCreateAsChild(GUI.CurrentForm,120,90,40,20);
	GUI_ButtonSetString(GUI_P->B_Y,"右","右");	
	GUI_P->B_Y->Func_Down = DW_Y_Down;	
	GUI_P->B_Y->Func_Up = DW_Up;		
	GUI_P->B_Y->OsPriority = 2;	
	
	GUI_P->B_ZX = GUI_ButtonCreateAsChild(GUI.CurrentForm,20,120,40,20);
	GUI_ButtonSetString(GUI_P->B_ZX,"左下","左下");	
	GUI_P->B_ZX->Func_Down = DW_ZX_Down;	
	GUI_P->B_ZX->Func_Up = DW_Up;		
	GUI_P->B_ZX->OsPriority = 2;	
	
	GUI_P->B_X = GUI_ButtonCreateAsChild(GUI.CurrentForm,70,120,40,20);
	GUI_ButtonSetString(GUI_P->B_X,"下","下");	
	GUI_P->B_X->Func_Down = DW_X_Down;	
	GUI_P->B_X->Func_Up = DW_Up;		
	GUI_P->B_X->OsPriority = 2;	
	
	GUI_P->B_YX = GUI_ButtonCreateAsChild(GUI.CurrentForm,120,120,40,20);
	GUI_ButtonSetString(GUI_P->B_YX,"右下","右下");	
	GUI_P->B_YX->Func_Down = DW_YX_Down;	
	GUI_P->B_YX->Func_Up = DW_Up;		
	GUI_P->B_YX->OsPriority = 2;


	GUI_P->B_Z_Up = GUI_ButtonCreateAsChild(GUI.CurrentForm,190,60,40,20);
	GUI_ButtonSetString(GUI_P->B_Z_Up,"Z+","Z+");	
	GUI_P->B_Z_Up->Func_Down = DW_Z_Up_Down;	
	GUI_P->B_Z_Up->Func_Up = DW_Up;		
	GUI_P->B_Z_Up->OsPriority = 2;

	GUI_P->B_Z_Dn = GUI_ButtonCreateAsChild(GUI.CurrentForm,190,120,40,20);
	GUI_ButtonSetString(GUI_P->B_Z_Dn,"Z-","Z-");	
	GUI_P->B_Z_Dn->Func_Down = DW_Z_Dn_Down;	
	GUI_P->B_Z_Dn->Func_Up = DW_Up;		
	GUI_P->B_Z_Dn->OsPriority = 2;

	GUI_P->B_Z_0 = GUI_ButtonCreateAsChild(GUI.CurrentForm,190,90,40,20);
	GUI_ButtonSetString(GUI_P->B_Z_0,"原点","原点");
	GUI_P->B_Z_0->OsPriority = 2;			
	GUI_P->B_Z_0->Func_Down = DW_Z_0;	
	GUI_P->B_Z_0->Func_Up = DW_Up;		
	
	GUI_P->B_Return = GUI_ButtonCreateAsChild(GUI.CurrentForm,250,120,60,20);		
	GUI_ButtonSetString(GUI_P->B_Return,"返回","返回");		
	GUI_P->B_Return->Func_Down = Page_DW_Return;		

	GUI_P->B_Moto_On = GUI_ButtonCreateAsChild(GUI.CurrentForm,250,60,60,20);
	GUI_ButtonSetString(GUI_P->B_Moto_On,"主轴开","主轴开");
	GUI_P->B_Moto_On->OsPriority = 2;			
	GUI_P->B_Moto_On->Func_Down = DW_MainMotoOn;	
	
	GUI_P->B_Moto_Off = GUI_ButtonCreateAsChild(GUI.CurrentForm,250,90,60,20);
	GUI_ButtonSetString(GUI_P->B_Moto_Off,"主轴关","主轴关");
	GUI_P->B_Moto_Off->OsPriority = 2;			
	GUI_P->B_Moto_Off->Func_Down = DW_MainMotoOff;	

	GUI.Font = &Font8x16;		
	GUI_PutStringAt("雕刻机定位操作",70,30);	
		DK.Ctrl = CTRL_SET_DW;	
}	



void Page_DingWei_Routine(void)
{
	if(DK.Ctrl == CTRL_SET_DW)
	{
		if(DW_XY_Key || DW_Z_Key) DW_Speed ++;		
		if(DW_Speed>=200) DW_Speed = 200;
		if(DK.Ctrl == CTRL_STOP)		return;		//停止
		if(DK.Alarm)	return;		//有故障退出自动
		if(NRF.Send == 0)	//等待上次发送完成
		{		
				Send_DW(DW_Speed,DW_XY_Key,DW_Z_Key);			
		}
	}
}











