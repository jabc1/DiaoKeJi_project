/*********************************************************************************
*文件名   : gui.h
*作用     : GUI结构体头文件
*创建时间 : 2015.10.29
*版本			：V0.0.1
*说明			:最初的版本
*********************************************************************************/
#ifndef __GUI_FUNCTIONKEY_H__
#define __GUI_FUNCTIONKEY_H__

#include "stm32f10x.h"  
#include "gui_FORM.h"
#include "GUI_Struct.h"
#include "GUI_config.h"

#define GUI_FUNCKEY_KEY				0xE0			//使用funckeyd的时候，发送的键值为GUI_FUNCKEY_KEY + NUM
#define 	GUI_USEFUNCKEY								//定义使用GUI_FUNCKEY

	typedef struct //定义一个菜单
	{
		u8 String[5];//项目总数
	}FunckeyStringStruct;

	#define	FUNCKEY_MEMNUM			20	
	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X,Y;
		u16 BColor, FColor, SelectColor;	
		GUI_FONT *Font;
		#if GUI_USEMSOS
			u8 OsPriority;
		#endif		

		u8 KeyNum;
		u8 SelectNum;
//		const u8* key;
		FunckeyStringStruct *String;		
	}FunckeyStruct;		
	
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----ON/OFF				= 控件状态为ON/OFF，同时lable等不需要ON/OFF的作为有效状态
										|||| +-----保留					
										|||+-------保留
										||+--------Longtouch				0:Long ON;1:Long OFF;	
										|+---------focusin			
										+----------是否为焦点状态*/
										


	#define FUNCKEY_TOUCH				0x20		//功能键被按下
	#define FUNCKEY_FOCUSIN			0x40	//按键进入焦点内部	
	FunckeyStruct *GUI_FunckeyCreate(void);
	void GUI_FunckeyShowNow(void);
	extern u8 GUI_FunckeyTouchDown(void);
	extern u8 GUI_FunckeyTouchLong(void);
	extern u8 GUI_FunckeyTouchUp(void);
	
#endif
	

	



