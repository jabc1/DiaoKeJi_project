/*********************************************************************************
*文件名   : gui.h
*作用     : GUI结构体头文件
*创建时间 : 2015.10.29
*版本			：V0.0.1
*说明			:最初的版本
*********************************************************************************/
#ifndef __GUI_CONF_H
#define __GUI_CONF_H
#include "stm32f10x.h"  
#include "gui_lcdapi.h"  
#include "gui_font.h"  
#include "GUI_DRAW2D.h" 
/************************************GUI任务基本参数的宏定义*******************************************************/
#define	GUI_USEMSOS									1							//定义是否使用MSOS
#define	GUI_USEKEY									1		//定义是否使用键盘
#define	GUI_USETOUCH								1		//定义是否使用触摸屏


#define GUICTRLTIME									20				//GUI touch和key循环刷新的时间ms
#define GUISHOWTIME									100				//GUI 刷新显示的时间ms
#define LCDAPIUSEFAST								1			//0：LCD使用的API函数只用SetWindows、SetPoint 2个函数
																			//1：LCD使用的API函数使用WriteRAM、BeginWriteRAM、EndWriteRAM、SetWindows、SetPoint 5个函数
#define GUI_W												320
#define GUI_H 											240

/**********************使用字体的选择********************************************/
#define 	FONT_USE_SD										1		//使用SD卡字体
#define 	FONT_USE_25WXX								2		//使用25W16字体
#define 	FONT_USE_ZIMO									3		//使用字模字体
#define 	FONT_USE											FONT_USE_SD
#define 	MAXGBFONTNUM									64

#define		FONT_USE_4X6						//把不需要的字体前面注释掉
#define		FONT_USE_6X8
#define		FONT_USE_8X12
#define		FONT_USE_8X16
#define		FONT_USE_16X24


#if GUI_USEKEY				//如果使用按键		
//	#include "GUI_Key.h" 
#endif

#if GUI_USETOUCH
//	#include "GUIdrivce.h"
//	#define GUI_IsTouch     				IsTouch
//	#define GUI_TouchReadXY  				ADS7843_Rd_Addata		
#endif



typedef void (*GUI_func)(void);
#define GUI_Func(address)   (*(GUI_func)address)()

#define GUI_true                1
#define GUI_false               0
#define GUI_null                ((void *) 0)

#define GUI_MEM_Malloc(Size)				mymalloc(Size)
#define GUI_MEM_Free(P)		  				myfree(P)
#define GUI_MEM_Realloc(ptr,size)		myrealloc(ptr,size); 

#if GUI_USEMSOS
#include "GUI_os.h"
	#define	GUIMutex						TaskStatusSuspend8
	#define GUI_LOCK()					OS.OSMutexLock(GUIMutex)	
	#define GUI_UNLOCK()				OS.OSMutexUnLock(GUIMutex)
//	#define GUI_FILELISTMESSAGE			FileListMessageType
	#define GUI_FL_FuncMessageType		0xDF
	#define GUI_FL_CloseMessageType		0xDE
	#define GUI_OsCurrentPriority  OsCurrentPriority
#else
	#define GUI_LOCK()				
	#define GUI_UNLOCK();
	#define GUI_RunFunction(Priority,RegisterFunction) GUI_Func(RegisterFunction)
	#define GUI_OsCurrentPriority 			0			//没有
	#define GUI_FL_Close(Priority,FileList_P )  ((void *) 0)
	#define GUI_FL_OpenFile(Priority,FileList_P) ((void *) 0)

#endif




/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----ON/OFF				= 控件状态为ON/OFF，同时lable等不需要ON/OFF的作为有效状态
										|||| +-----保留					
										|+++-------保留
										+----------是否为焦点状态*/
				
	#define 	GUISetBit(data, offset)         (data |= offset)
	#define 	GUIResetBit(data, offset)       (data &= ~offset)
	#define 	GUIGetBit(data, offset)         (data & offset)

	
	#define		GUI_SHOWOK 	0x01   		//控件有变化
	#define		GUI_DIS 			0x02   			//控件无效			0	
	#define		GUI_ON 			0x04   		//控件ON/OFF			1
	#define		GUI_FOCUS 		0X80			//控件的焦点标志位
	

//#include "GUI_form.h"
//是否使用对应的功能在这里开启，注释掉的话就不包含	

#include "GUI_lamp.h"
#include "GUI_lable.h"
//#include "GUI_edit.h"	
#include "GUI_LED.h"		
#include "GUI_button.h"			
#include "GUI_MENU.h"		
//#include "GUI_FUNCTIONKEY.h"			
//#include "GUI_KeyBoard.h" 	
#include "GUI_Scrollbar.h" 
#include "GUI_FileList.h" 




/* LCD color */
#define GUI_White          0xFFFF
#define GUI_Black          0x0000
#define GUI_Grey           0xF7DE
#define GUI_Blue           0x001F
#define GUI_Blue2          0x051F
#define GUI_Red            0xF800
#define GUI_Magenta        0xF81F
#define GUI_Green          0x07E0
#define GUI_Cyan           0x7FFF
#define GUI_Yellow         0xFFE0


#define 	STRING_MAXLEN				20
#define 	FROM_MAXCHILDNUM		40			//from中子控件的最大数量 

	typedef struct 
	{ 
		u32 GUI_OS_Run;
		
	}GUI_State_B_Struct;	

	typedef struct 
	{ 
		u16 State;
		u16 B_Color;							//LCD的背景色
		u16 F_Color;							//LCD的前景色
		u16 DIS_Color;						//控件失效时的颜色		
		u8 MemDevFlag;						//磁盘在位状态每个位代表1个磁盘，总共可以8个		
		u8	KeyState;							//按键状态，
		u8  KeyNum;								//按键值，即第几个按键按下
		u8  TouchState;						//触摸状态		
		u16	Touch_X;							//触摸屏触摸点X轴坐标
		u16	Touch_Y;							//触摸屏触摸点Y轴坐标 		
		GUI_FormStruct *CurrentForm;
		u8  *FuncKey_P;
		GUI_FontStruct *Font;						//GUI当前使用的字体	
		#if GUI_USEMSOS
			void (*CreateTask)(void);
		#endif	
		GUI_State_B_Struct *B_Sate_P; 
	}GUI_Struct;	
	extern GUI_Struct GUI;



//定义GUI使用的key的键值
#define	GUIKEY_0		0
#define	GUIKEY_1		1
#define	GUIKEY_2		2
#define	GUIKEY_3		3
#define	GUIKEY_4		4
#define	GUIKEY_5		5
#define	GUIKEY_6		6
#define	GUIKEY_7		7
#define	GUIKEY_8		8
#define	GUIKEY_9		9
#define	GUIKEY_A		10
#define	GUIKEY_B		11
#define	GUIKEY_C		12
#define	GUIKEY_D		13
#define	GUIKEY_E		14
#define	GUIKEY_F		15

#define	GUIKEY_UP			11
#define	GUIKEY_DOWN		10
#define	GUIKEY_ENTER	14
#define	GUIKEY_ESC		15

	typedef enum 
	{
		GUIFormID 				= 0x80,
		GUILampID 				= 0x81,
		GUILedID 					= 0x82,
		GUILableID 				= 0x83,
		GUIEditID 				= 0x84,
		GUIButtonID 			= 0x85,
		GUIMenuID 				= 0x86,	
//		GUISYSMenuID 			= 0x90,	
		GUIFunckeyID		 	= 0X91,			
		GUIKeyBoardID			= 0X92,		
		GUIFileListID			= 0X93,	
		GUIScrollbarID		= 0X94,
		NOTGUIID					=	0x01,				//不是GUI ID
	}GUIIDEnum;
	#define MINGUIID		0X80
	#define MAXGUIID		0X94	

	
	typedef enum 
	{
		GUI_KT_NULL		= 0x00,
		GUI_KT_DOWN	= 0x01,
		GUI_KT_LONG		= 0x02,
		GUI_KT_UP		= 0x04,
//		GUI_KT_NONUM	= 0xFF	
	}KEY_TYPE;


	
#endif 
