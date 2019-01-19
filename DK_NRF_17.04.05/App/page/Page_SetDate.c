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
	u8 Set_Year;			//公历日月年周
	u8 Set_Month;
	u8 Set_Date;				
void SetDateReturn(void)
{
	DK.State = 0;
	Init_MainPage();
}
void DateUp(void)
{
	Set_Date++;
	if(mon_table[Set_Month]<Set_Date)
	{
		Set_Date = mon_table[Set_Month];
	}
	GUI_DispDecAt(Set_Date,185,100,2);	
	
}
void DateDn(void)
{
	if(Set_Date<1)
	{
		Set_Date=1;		
	}
	else
	{
		Set_Date--;		
	}
	GUI_DispDecAt(Set_Date,185,100,2);		
}
void MonthUp(void)
{
	if(Set_Month>=12)
	{
		Set_Month=12;		
	}
	else
	{
		Set_Month++;		
	}
	GUI_DispDecAt(Set_Month,115,100,2);		
}
void MonthDn(void)
{
	if(Set_Month<1)
	{
		Set_Month=1;		
	}
	else
	{
		Set_Month--;		
	}
	GUI_DispDecAt(Set_Month,115,100,2);	
}
void YearUp(void)
{
	if(Set_Year>99)
	{
		Set_Year=1;		
	}
	else
	{
		Set_Year++;		
	}
	GUI_DispDecAt(Set_Year,45,100,2);		
}
void YearDn(void)
{
	if(Set_Year<1)
	{
		Set_Year=1;		
	}
	else
	{
		Set_Year--;		
	}
	GUI_DispDecAt(Set_Year,45,100,2);		
}
void DateOK(void)
{

	RTC_Calendar.Year =Set_Year;
	RTC_Calendar.Month = Set_Month;
	RTC_Calendar.Date = Set_Date;
	
RTC_Set(RTC_Calendar.Year,RTC_Calendar.Month,RTC_Calendar.Date,RTC_Calendar.Hour,RTC_Calendar.Min,RTC_Calendar.Sec);
SetDateReturn();
}

void Page_SetDate(void)
{
	Page_Date_Struct *GUI_P;
	GUI_P=(Page_Date_Struct *)GUIKJ;
	Page = PAGE_SETDATE;
	Set_Date = RTC_Calendar.Date;
	Set_Month =  RTC_Calendar.Month;
	Set_Year = RTC_Calendar.Year;
	
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);
	GUI_PutStringAt("日期修改",70,20);	
	
	GUI_P->B_Y_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,30,70,50,20);
	GUI_ButtonSetString(GUI_P->B_Y_U,"+","+");				
	GUI_P->B_Y_U->Func_Down = YearUp;
	GUI_P->B_Y_U->Func_Long = YearUp;
	GUI_P->B_Y_U->OsPriority = ShowTaskPriority;
	GUI_P->B_Y_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,30,120,50,20);
	GUI_ButtonSetString(GUI_P->B_Y_D,"-","-");	
	GUI_P->B_Y_D->Func_Down = YearDn;		
	GUI_P->B_Y_D->Func_Long = YearDn;	
	GUI_P->B_Y_D->OsPriority = ShowTaskPriority;
	GUI_P->B_M_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,100,70,50,20);		
	GUI_ButtonSetString(GUI_P->B_M_U,"+","+");	
	GUI_P->B_M_U->Func_Down = MonthUp;
	GUI_P->B_M_U->Func_Long = MonthUp;
	GUI_P->B_M_U->OsPriority = ShowTaskPriority;
	GUI_P->B_M_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,100,120,50,20);	
	GUI_ButtonSetString(GUI_P->B_M_D,"-","-");		
	GUI_P->B_M_D->Func_Down = MonthDn;			
	GUI_P->B_M_D->Func_Long = MonthDn;	
	GUI_P->B_M_D->OsPriority = ShowTaskPriority;
	GUI_P->B_D_U = GUI_ButtonCreateAsChild(GUI.CurrentForm,170,70,50,20);		
	GUI_ButtonSetString(GUI_P->B_D_U,"+","+");	
	GUI_P->B_D_U->Func_Down = DateUp;
	GUI_P->B_D_U->Func_Long = DateUp;
	GUI_P->B_D_U->OsPriority = ShowTaskPriority;
	GUI_P->B_D_D = GUI_ButtonCreateAsChild(GUI.CurrentForm,170,120,50,20);	
	GUI_ButtonSetString(GUI_P->B_D_D,"-","-");		
	GUI_P->B_D_D->Func_Down = DateDn;		
	GUI_P->B_D_D->Func_Long = DateDn;	
	GUI_P->B_D_D->OsPriority = ShowTaskPriority;
	GUI_P->B_OK = GUI_ButtonCreateAsChild(GUI.CurrentForm,50,180,50,20);		
	GUI_ButtonSetString(GUI_P->B_OK,"确定","确定");		
	GUI_P->B_OK->Func_Down = DateOK;		
	GUI_P->B_OK->OsPriority = ShowTaskPriority;	
	GUI_P->B_Return = GUI_ButtonCreateAsChild(GUI.CurrentForm,150,180,50,20);		
	GUI_ButtonSetString(GUI_P->B_Return,"取消","取消");		
	GUI_P->B_Return->Func_Down = SetDateReturn;		
	GUI_P->B_Return->OsPriority = ShowTaskPriority;	

	GUI_PutStringAt("年",45,45);		
	GUI_DispDecAt(RTC_Calendar.Year,45,100,2);
	GUI_PutStringAt("月",115,45);	
	GUI_DispDecAt(RTC_Calendar.Month,115,100,2);	
	GUI_PutStringAt("日",185,45);	
	GUI_DispDecAt(RTC_Calendar.Date,185,100,2);	
	
}









