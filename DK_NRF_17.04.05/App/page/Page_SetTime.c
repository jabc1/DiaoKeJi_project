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
		
	u8 Set_Hour;
	u8 Set_Min;
	u8 Set_Sec;	

void SetTimeReturn(void)
{
	DK.State = 0;
	Init_MainPage();
}
void HourUp(void)
{
	if(Set_Hour>=24)
	{
		Set_Hour=24;		
	}
	else
	{
		Set_Hour++;		
	}
	GUI_DispDecAt(Set_Hour,45,100,2);	
	
}
void HourDn(void)
{
	if(Set_Hour<1)
	{
		Set_Hour=1;		
	}
	else
	{
		Set_Hour--;		
	}
	GUI_DispDecAt(Set_Hour,45,100,2);		
}
void MinUp(void)
{
	if(Set_Min>=60)
	{
		Set_Min=60;		
	}
	else
	{
		Set_Min++;		
	}
	GUI_DispDecAt(Set_Min,115,100,2);		
}
void MinDn(void)
{
	if(Set_Min<1)
	{
		Set_Min=1;		
	}
	else
	{
		Set_Min--;		
	}
	GUI_DispDecAt(Set_Min,115,100,2);	
}
void SecUp(void)
{
	if(Set_Sec>60)
	{
		Set_Sec=1;		
	}
	else
	{
		Set_Sec++;		
	}
	GUI_DispDecAt(Set_Sec,185,100,2);		
}
void SecDn(void)
{
	if(Set_Sec<1)
	{
		Set_Sec=1;		
	}
	else
	{
		Set_Sec--;		
	}
	GUI_DispDecAt(Set_Sec,185,100,2);		
}
void TimeOK(void)
{

	RTC_Calendar.Hour = Set_Hour;
	RTC_Calendar.Min = Set_Min;
	RTC_Calendar.Sec = Set_Sec;
	
	RTC_Set(RTC_Calendar.Year,RTC_Calendar.Month,RTC_Calendar.Date,RTC_Calendar.Hour,RTC_Calendar.Min,RTC_Calendar.Sec);
	SetTimeReturn();
}

void Page_SetTime(void)
{
	Page_Time_Struct *GUI_P;
	GUI_P=(Page_Time_Struct *)GUIKJ;
	Page = PAGE_SETTIME;
	Set_Hour = RTC_Calendar.Hour;
	Set_Min =  RTC_Calendar.Min;
	Set_Sec = RTC_Calendar.Sec;
	
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);
	GUI_P->B_H_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,30,70,50,20);
	GUI_P->B_H_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,30,120,50,20);
	GUI_P->B_M_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,100,70,50,20);		
	GUI_P->B_M_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,100,120,50,20);	
	GUI_P->B_S_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,170,70,50,20);		
	GUI_P->B_S_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,170,120,50,20);	
	GUI_P->B_OK = GUI_ButtonCreateAsChild(GUI.CurrentForm,50,180,50,20);		
	GUI_P->B_Return = GUI_ButtonCreateAsChild(GUI.CurrentForm,150,180,50,20);		

	GUI_PutStringAt("日期修改",70,20);	
	GUI_PutStringAt("时",45,45);	
	GUI_DispDecAt(RTC_Calendar.Hour,45,100,2);
	GUI_PutStringAt("分",115,45);	
	GUI_DispDecAt(RTC_Calendar.Min,115,100,2);	
	GUI_PutStringAt("秒",185,45);	
	GUI_DispDecAt(RTC_Calendar.Sec,185,100,2);	
	
	
	GUI_ButtonSetString(GUI_P->B_H_U,"+","+");				
	GUI_ButtonSetString(GUI_P->B_H_D,"-","-");	
	GUI_ButtonSetString(GUI_P->B_M_U,"+","+");	
	GUI_ButtonSetString(GUI_P->B_M_D,"-","-");		
	GUI_ButtonSetString(GUI_P->B_S_U,"+","+");	
	GUI_ButtonSetString(GUI_P->B_S_D,"-","-");		
	GUI_ButtonSetString(GUI_P->B_OK,"确定","确定");			
	GUI_ButtonSetString(GUI_P->B_Return,"取消","取消");		

	GUI_P->B_H_U->Func_Down = HourUp;
	GUI_P->B_H_D->Func_Down = HourDn;	
	GUI_P->B_M_U->Func_Down = MinUp;
	GUI_P->B_M_D->Func_Down = MinDn;	
	GUI_P->B_S_U->Func_Down = SecUp;
	GUI_P->B_S_D->Func_Down = SecDn;	
	
	GUI_P->B_H_U->Func_Long = HourUp;
	GUI_P->B_H_D->Func_Long = HourDn;	
	GUI_P->B_M_U->Func_Long = MinUp;
	GUI_P->B_M_D->Func_Long = MinDn;	
	GUI_P->B_S_U->Func_Long = SecUp;
	GUI_P->B_S_D->Func_Long = SecDn;	
	GUI_P->B_OK->Func_Down = TimeOK;		
	GUI_P->B_Return->Func_Down = SetTimeReturn;	
	
	GUI_P->B_H_U->OsPriority = ShowTaskPriority;
	GUI_P->B_H_D->OsPriority = ShowTaskPriority;
	GUI_P->B_M_U->OsPriority = ShowTaskPriority;
	GUI_P->B_M_D->OsPriority = ShowTaskPriority;	
	GUI_P->B_S_U->OsPriority = ShowTaskPriority;
	GUI_P->B_S_D->OsPriority = ShowTaskPriority;
	GUI_P->B_OK->OsPriority = ShowTaskPriority;
	GUI_P->B_Return->OsPriority = ShowTaskPriority;	
	
	
	
	
	
}









