/*********************************************************************************
*�ļ���   : gui.h
*����     : GUI�ṹ��ͷ�ļ�
*����ʱ�� : 2015.10.29
*�汾			��V0.0.1
*˵��			:����İ汾
*********************************************************************************/
#ifndef __GUI_CONF_H
#define __GUI_CONF_H
#include "stm32f10x.h"  
#include "gui_lcdapi.h"  
#include "gui_font.h"  
#include "GUI_DRAW2D.h" 
/************************************GUI������������ĺ궨��*******************************************************/
#define	GUI_USEMSOS									1							//�����Ƿ�ʹ��MSOS
#define	GUI_USEKEY									1		//�����Ƿ�ʹ�ü���
#define	GUI_USETOUCH								1		//�����Ƿ�ʹ�ô�����


#define GUICTRLTIME									20				//GUI touch��keyѭ��ˢ�µ�ʱ��ms
#define GUISHOWTIME									100				//GUI ˢ����ʾ��ʱ��ms
#define LCDAPIUSEFAST								1			//0��LCDʹ�õ�API����ֻ��SetWindows��SetPoint 2������
																			//1��LCDʹ�õ�API����ʹ��WriteRAM��BeginWriteRAM��EndWriteRAM��SetWindows��SetPoint 5������
#define GUI_W												320
#define GUI_H 											240

/**********************ʹ�������ѡ��********************************************/
#define 	FONT_USE_SD										1		//ʹ��SD������
#define 	FONT_USE_25WXX								2		//ʹ��25W16����
#define 	FONT_USE_ZIMO									3		//ʹ����ģ����
#define 	FONT_USE											FONT_USE_SD
#define 	MAXGBFONTNUM									64

#define		FONT_USE_4X6						//�Ѳ���Ҫ������ǰ��ע�͵�
#define		FONT_USE_6X8
#define		FONT_USE_8X12
#define		FONT_USE_8X16
#define		FONT_USE_16X24


#if GUI_USEKEY				//���ʹ�ð���		
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
	#define GUI_OsCurrentPriority 			0			//û��
	#define GUI_FL_Close(Priority,FileList_P )  ((void *) 0)
	#define GUI_FL_OpenFile(Priority,FileList_P) ((void *) 0)

#endif




/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----ON/OFF				= �ؼ�״̬ΪON/OFF��ͬʱlable�Ȳ���ҪON/OFF����Ϊ��Ч״̬
										|||| +-----����					
										|+++-------����
										+----------�Ƿ�Ϊ����״̬*/
				
	#define 	GUISetBit(data, offset)         (data |= offset)
	#define 	GUIResetBit(data, offset)       (data &= ~offset)
	#define 	GUIGetBit(data, offset)         (data & offset)

	
	#define		GUI_SHOWOK 	0x01   		//�ؼ��б仯
	#define		GUI_DIS 			0x02   			//�ؼ���Ч			0	
	#define		GUI_ON 			0x04   		//�ؼ�ON/OFF			1
	#define		GUI_FOCUS 		0X80			//�ؼ��Ľ����־λ
	

//#include "GUI_form.h"
//�Ƿ�ʹ�ö�Ӧ�Ĺ��������￪����ע�͵��Ļ��Ͳ�����	

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
#define 	FROM_MAXCHILDNUM		40			//from���ӿؼ���������� 

	typedef struct 
	{ 
		u32 GUI_OS_Run;
		
	}GUI_State_B_Struct;	

	typedef struct 
	{ 
		u16 State;
		u16 B_Color;							//LCD�ı���ɫ
		u16 F_Color;							//LCD��ǰ��ɫ
		u16 DIS_Color;						//�ؼ�ʧЧʱ����ɫ		
		u8 MemDevFlag;						//������λ״̬ÿ��λ����1�����̣��ܹ�����8��		
		u8	KeyState;							//����״̬��
		u8  KeyNum;								//����ֵ�����ڼ�����������
		u8  TouchState;						//����״̬		
		u16	Touch_X;							//������������X������
		u16	Touch_Y;							//������������Y������ 		
		GUI_FormStruct *CurrentForm;
		u8  *FuncKey_P;
		GUI_FontStruct *Font;						//GUI��ǰʹ�õ�����	
		#if GUI_USEMSOS
			void (*CreateTask)(void);
		#endif	
		GUI_State_B_Struct *B_Sate_P; 
	}GUI_Struct;	
	extern GUI_Struct GUI;



//����GUIʹ�õ�key�ļ�ֵ
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
		NOTGUIID					=	0x01,				//����GUI ID
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
