/*********************************************************************************
* �ļ���		: menu.h
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ���庯��						  
* �÷�			��
* �汾˵��	��
**********************************************************************************/



#ifndef __SHOWTASK_H
#define __SHOWTASK_H
#include "stm32f10x.h"

#include "gui_conf.h"



extern void InitializeShow(void);
extern void ShowTask(void);



extern GUI_LableStruct *L_Date,*L_Time;
extern GUI_LableStruct *L_DK_X,*L_DK_Y,*L_DK_Z;
extern GUI_FileListStruct *FL_T;		//�ļ���ʾ�б�
extern u32 GUIKJ[15];


//extern 	GUI_LEDStruct *LED_Alarm_P,*LED_State_P;
extern u8 Page;
extern void Initializepage(void);
extern void Page_Routine(void);

#endif 

