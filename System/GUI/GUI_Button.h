#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"

#define 	GUI_USEBUTTON								//定义使用GUI_BUTTON
	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X, Y;
		u16 TextColor, ONColor, OFFColor;	
		GUI_FontStruct *Font;	
		u16 W, H;	
		u8 OsPriority;						//对应是哪个线程使用的回调函数
    GUI_func Func_Down;		//回调函数		
    GUI_func Func_Long;		//回调函数				
    GUI_func Func_Up;		//回调函数		
		u8* OffString;		
		u8* OnString;

	}GUI_ButtonStruct;
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----ON/OFF				= 控件状态为ON/OFF，同时lable等不需要ON/OFF的作为有效状态
										|||| +-----保留					
										|||+-------3D/2D 				= 0：3D; 1：2D
										||+--------Long ON/OFF				0:Long ON;1:Long OFF;	
										|+---------ButtonFuntionMode			0:回调函数在注册button的线程中执行	;1:回调函数在GUI线程中执行
										+----------是否为焦点状态*/

	
	#define		BUTTON_3D 				0x10   		//控件有变化
	#define		BUTTON_LONG 			0x20   		//控件ON			1

	GUI_ButtonStruct *GUI_ButtonCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y,u16 W,u16 H);
	void GUI_ButtonSetString(GUI_ButtonStruct *GUI_Button_P,u8 *OnString,u8* OffString)	;
	void GUI_ButtonRegisterFunction(GUI_ButtonStruct *GUI_Button_P,GUI_func registerFunction);
	extern void GUI_ButtonShowNow(GUI_ButtonStruct *GUI_Button_P);
	extern void GUI_ButtonLostFocus(GUI_ButtonStruct *GUI_Button_P);
	extern void GUI_ButtonGetFocus(GUI_ButtonStruct *GUI_Button_P);	
	extern void GUI_ButtonKeyDown(GUI_ButtonStruct *GUI_Button_P);/*硬件按键第一次按下，设置button的状态和运行注册的程序*/
	extern void GUI_ButtonKeyLong(GUI_ButtonStruct *GUI_Button_P);/*硬件按键按下后的重复确认时，设置button的状态和运行注册的程序*/
	extern void GUI_ButtonKeyUp(GUI_ButtonStruct *GUI_Button_P);/*硬件按键释放时，设置button的状态*/
	extern u8 GUI_ButtonTouchDown(GUI_ButtonStruct *GUI_Button_P,u8 Num);/*触摸屏触摸按下的时候，设置button的状态*/
	extern void GUI_ButtonTouchUp(GUI_ButtonStruct *GUI_Button_P);/*触摸屏触摸释放的时候，设置button的状态*/
	extern void GUI_ButtonTouchLong(GUI_ButtonStruct *GUI_Button_P);

#endif 
