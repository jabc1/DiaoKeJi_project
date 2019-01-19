/*********************************************************************************
* 文件名		: menu.h
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 定义函数						  
* 用法			：
* 版本说明	：
**********************************************************************************/



#ifndef __SHOWTASK_H
#define __SHOWTASK_H
#include "stm32f10x.h"

#include "gui_conf.h"



extern void InitializeShow(void);
extern void ShowTask(void);



extern GUI_LableStruct *L_Date,*L_Time;
extern GUI_LableStruct *L_DK_X,*L_DK_Y,*L_DK_Z;
extern GUI_FileListStruct *FL_T;		//文件显示列表
extern u32 GUIKJ[15];


//extern 	GUI_LEDStruct *LED_Alarm_P,*LED_State_P;
extern u8 Page;
extern void Initializepage(void);
extern void Page_Routine(void);

#endif 

