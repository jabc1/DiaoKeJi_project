#ifndef __GUI_SCOROLLBAR_H__
#define __GUI_SCOROLLBAR_H__

#include "stm32f10x.h"  
#include "gui_FORM.h"


#define		GUI_USEScrollbar

#define		SCOROLLBAR_MIN_LEN					50			//S
#define		SCOROLLBAR_WIDTH					18			//




	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X, Y;
		u16 BColor,FColor,SelectColor;
		u16 Len;
 	u16 totalitems;					//总条目数
	u16 itemsperpage;				//每页显示的条目数
	u16 topitem;					//最顶/左端的条目 
	u16 scbbarlen;					//滚条长度		
		
		
//		
//		u16 Min;
//		u16 Max;
//		u16 OldValue;
//		u16 Value;		
	}GUI_ScrollbarStruct;	
	
	
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----ON/OFF				= 控件状态为ON/OFF，同时lable等不需要ON/OFF的作为有效状态
										|||| +-----保留					
										|||+-------H/V 				= 0：V; 1：H
										||+--------Init				=0:需要初始化； 1：初始化完成
										|+---------保留			
										+----------是否为焦点状态*/

	
	#define		SCOROLLBAR_MODE 	0x10   		//滚动条是水平的还是垂直的
	#define		SCOROLLBAR_INIT 	0x20   		//滚动条是水平的还是垂直的
		
	extern GUI_ScrollbarStruct *GUI_ScrollbarCreate(u16 X, u16 Y,u16 len,u8 Mode);
	extern void GUI_ScrollbarSetValue(GUI_ScrollbarStruct *GUI_Scrollbar_P,u32 value)	;
	extern void GUI_ScrollbarShowNow(GUI_ScrollbarStruct *P);
	u8 GUI_ScrollbarTouchDown(GUI_ScrollbarStruct *Scrollbar_P,u16 X,u16 Y);
#endif 
