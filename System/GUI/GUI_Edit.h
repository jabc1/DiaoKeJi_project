/*********************************************************************************
*文件名   : gui.h
*作用     : GUI结构体头文件
*创建时间 : 2015.10.29
*版本			：V0.0.1
*说明			:最初的版本
*********************************************************************************/
#ifndef __GUI_EDIT_H__
#define __GUI_EDIT_H__
#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"


#define		GUI_USEEDIT
	#define	EDIT_MEMNUM				20+EDIT_MAXSTRINLEN			//edit 创建的时候申请的内存数量
#define 	EDIT_MAXSTRINLEN		18
	typedef struct
	{
		u8 ID;	
		u8 State;		
		u16 X,Y;
		u16 BColor,FColor;	
		u8 MaxLen;	
		u8 FocusLen;
		GUI_FontStruct *Font;
		u8 Mode;
//		GUI_FormStruct *Father;
		u8 String[EDIT_MAXSTRINLEN];
	}GUI_EditStruct;
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----保留			
										|||| +-----保留
										|||+-------FOCUSEDIT		= 如果selectLen>0 	0:只是在选择状态，1： edit状态 	
										||+--------COLORTYPE		=	FocusLen 选择的文字 0: 显示正常颜色；1： 显示反转颜色
										|+---------保留
										+----------是否为焦点状态*/
	#define EDIT_FOCUSEDIT		0X10										//edit的焦点状态
	#define EDIT_COLORTYPE		0X20										//焦点选择的文字颜色状态	

	
	GUI_EditStruct *GUI_EditCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u8 MaxLen,GUI_STRINGMODE Mode);
	u8 GUI_EditSetString(GUI_EditStruct *GUI_Edit_P,u8 *String);
	void GUI_EditGetString(GUI_EditStruct *GUI_Edit_P,u8 *String);
	u8 GUI_EditSetValue(GUI_EditStruct *GUI_Edit_P,u32 Value);	
//	u32 GUI_EditGetValue(GUI_EditStruct *GUI_Edit_P);	
	extern void GUI_EditShowNow(GUI_EditStruct *GUI_Edit_P);
	extern void GUI_EditLostFocus(GUI_EditStruct *GUI_Edit_P);
	extern void GUI_EditGetFocus(GUI_EditStruct *GUI_Edit_P);
	extern void GUI_EditKeyFirst(GUI_EditStruct *GUI_Edit_P);		
	extern void GUI_EditTouchUp(GUI_EditStruct *GUI_Edit_P);
	extern u8 GUI_EditTouchDown(GUI_EditStruct *GUI_Edit_P,u8 Num);	
	
#endif 
