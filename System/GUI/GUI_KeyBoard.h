/*********************************************************************************
*文件名   : gui.h
*作用     : GUI结构体头文件
*创建时间 : 2015.10.29
*版本			：V0.0.1
*说明			:最初的版本
*********************************************************************************/
#ifndef __GUI_KEYBOARD_H__
#define __GUI_KEYBOARD_H__
#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"

#define 	GUI_USEKEYBOARD								//定义使用GUI_USEKEYBOARD

	#define	KeyBoard_MEMNUM			32	//	20+KEYNUM*4	 320 KEYNUM = 8;240 KEYNUM = 6

	typedef struct
	{
		u8 ID;																//KeyBoard 的ID编号，默认不能修改
		u8 State;															//状态
		u16 X,Y;															//X,Y轴坐标
		u16 BColor, FColor, SelectColor;			//KeyBoard 的颜色
		GUI_FontStruct *Font;												//字体
		u8 W;																	//单个按键的宽度
		u8 H;																	//单个按键的高度
		u8 X_Num;															//每行有几个按键
		u8 Y_Num;															//共有几行
		u8 SelectNum;													//键值
		u8 StringLen;													//文字长度，需另加1位0x00的文字结束标志，所以2个字符的话要设置3，包括数组
		u8 Char_X,Char_Y;											//每个键的文字对应的X、Y轴偏移量	
		u8 KeyID;															//只用高3位，后5位为键值
		u8 OsPriority;
		GUI_STRINGMODE StringMode;						//显示的文字类型，现在支持string，dec，hex
		const u8* keyString;									//文字数组指针
	}GUI_KeyBoardStruct;		
	typedef struct
	{
		GUI_FormStruct *Form_P;
		u16 X,Y;
		u8 W;																	//单个按键的宽度
		u8 H;																	//单个按键的高度
		u8 X_Num;															//每行有几个按键
		u8 Y_Num;															//共有几行
		u8 Char_X;
		u8 Char_Y;		
		u8 KeyID;															//只用高3位，后5位为键值
		u8 StringLen;				//文字长度，需另加1位0x00的文字结束标志，所以2个字符的话要设置3，包括数组	
		GUI_STRINGMODE StringMode;		
		const u8* keyString;
	}GUI_KeyBoardInitStruct;		
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----ON
										|||| +-----All				= 第一次显示，需显示框架					
										|||+-------保留	
										||+--------touch				按钮被按下
										|+---------focusin			
										+----------是否为焦点状态*/
										
	#define KeyBoard_INITOK	0x40
	#define KeyBoard_SHOWON	0x20	
	#define KeyBoard_TOUCH				0x20		//功能键被按下
//	#define KeyBoard_FOCUSIN			0x80	//按键进入焦点内部	
	GUI_KeyBoardStruct *GUI_KeyBoardCreate(GUI_KeyBoardInitStruct *KeyBoardInit_P);
	void GUI_KeyBoardShowAll(GUI_KeyBoardStruct *KeyBoard_P);
	void GUI_KeyBoardShowNow(GUI_KeyBoardStruct *KeyBoard_P);
	u8 GUI_KeyBoardTouchUp(GUI_KeyBoardStruct *KeyBoard_P);
	u8 GUI_KeyBoardTouchLong(GUI_KeyBoardStruct *KeyBoard_P);
	u8 GUI_KeyBoardTouchDown(GUI_KeyBoardStruct *KeyBoard_P,u8 Num);


#endif
	

	



