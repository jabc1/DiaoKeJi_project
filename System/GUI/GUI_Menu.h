#ifndef __GUI_MENU_H__
#define __GUI_MENU_H__

#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"
#include "GUI_Scrollbar.h" 

	#define 	GUI_USEMENU							//定义使用GUI_MENU	
	#define 	MENU_MAXITEMNUM			8				//菜单中最大的菜单数量	
	#define 	MENU_MIN_W			40				//菜单中最小宽度
	#define 	MENU_MAX_W			GUI_W				//菜单中最大宽度
	
	#define 	MENU_MIN_H			32				//菜单中最小高度		
	#define 	MENU_MAX_H			GUI_H				//菜单中最大高度	
//	#define 	MENU_MEMNUM					32
	//首先定义一个菜单结构
	typedef struct GUImenu//定义一个菜单
	{
	 u8 itemCount;//项目总数
	 u8 *menuItems[MENU_MAXITEMNUM];//菜单项目
	 struct GUImenu **subMenus;//子菜单
	 struct GUImenu *parent;//上级菜单 ,如果是顶级则为null
	 void (**func)();//选择相应项按确定键后执行的函数
	}GUIMenu;

	typedef struct
	{
		u8 ID;	
		u8 State;		
		u16 X,Y,W,H;
//		u16 BColor, FColor, SelectColor;	
		u16 L_B_Color;					//内部背景颜色
		u16 L_S_Color; 				//list name 选中后的颜色
//		u16 L_SB_Color; 				//list name 选中后的背景颜色
		u16 F_Color; 					//list name 未选中的颜色
		u16 Rim_Color;					//边框颜色
		u16 I_B_Color;			    //路径/信息栏的背景颜色				
		
		
		
		
		GUI_FontStruct *Font;		
		u8 MaxLen;
		u8 ShowItemNum;			//菜单显示的条数
		u8 Range_from;			//当前显示的项开始序号			
		u8 Selected;				//当前选择项			
//		u8 OnLongTouch;
		u8 OsPriority;
		GUI_ScrollbarStruct * Scrollbar;			//垂直滚动条
		GUIMenu *CurrentMenus;		
//		GUI_FormStruct *Father;
	}GUI_MenuStruct;
	
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----0x04  MENU_USE_LONG_TOUCH 	  		//控件常按时是否重复运行			
										|||| +-----0x08  MENU_LONG_STATE			 	   		//控件常按后第一次标志				
										|||+-------0x10  RunFirst 	控件第一次运行			= 0：NO; 1：YES
										||+--------LongTouch				0:flase;1:true;	
										|+---------ButtonFuntionMode			0:回调函数在注册button的线程中执行	;1:回调函数在GUI线程中执行
										+----------是否为焦点状态*/
	#define		MENU_USE_LONG_TOUCH 	0x04   		//控件常按时是否重复运行
	#define		MENU_LONG_STATE			 	0x08   		//控件常按后第一次标志
	#define		MENU_RUN_FRIST			 	0x10   		//控件第一次运行
	#define		GUI_FOCUS_STATE 			0X40			//控件焦点的显示状态
	
	
	GUI_MenuStruct *GUI_MenuCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u16 W,u16 H);
	void GUI_SetCurrentMenus(GUI_MenuStruct *Menu_P,GUIMenu *CurrentMenus);
	extern void GUI_MenuShowNow(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuLostFocus(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuGetFocus(GUI_MenuStruct *Menu_P);	
	extern void GUI_MenuKeyFirst(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuKeyOff(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuKeyLong(GUI_MenuStruct *Menu_P);			
	extern u8 GUI_MenuTouchDown(GUI_MenuStruct *Menu_P,u8 IDNum);
	extern void GUI_MenuTouchUp(GUI_MenuStruct *Menu_P);
	extern u8 GUI_MenuTouchLong(GUI_MenuStruct *Menu_P);	
	
	
	
	
#endif 
