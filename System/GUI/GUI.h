/*********************************************************************************
*文件名   : gui.h
*作用     : GUI系统库全局头文件
*创建时间 : 2015.9.29
*版本			：V0.0.1
*说明			:最初的版本
*********************************************************************************/
#ifndef __GUI_H
#define __GUI_H
#include "stm32f10x.h"  
#include <string.h>
//#include "GUI_conf.h"
// #include "malloc.h"
//  #include "os.h"
//	#include "GUI_form.h" 
/******************************GUI.C********************************************/
//#if GUI_USEMSOS	
//extern uint8_t    OsCurrentPriority;	
//	
//#endif
	
	typedef enum 
	{
		GUIState_Redraw 		= 0x01,
		GUIState_Disable 		= 0x02,
		GUIState_Enable 		= 0x03,
		GUIState_ON					= 0X04,
		GUIState_OFF				= 0X05,
	}GUIStateEnum;	
	
	typedef enum 
	{
		GUI_STRING=0x00 ,  		//显示模式为文字
		GUI_DEC=0x01 ,  			//显示模式为10进制数字
		GUI_HEX = 0X02,				//显示模式为16进制数字
		GUI_TIME = 0X04,
		GUI_DATE = 0X08,
	}GUI_STRINGMODE;	

	void GUI_MemSet(void *Mem_P,u8 Data,u32 Len);
		
	void InitializeGUI(void);
	void GUI_Exc(void);	

	void GUI_FocusSelectChang(u8 NewFocusID);
	void HexToString(u32 hex,u8* string,u8 len);
	void DecToString(u32 Dec,u8* string,u8 len);
	u32 DecStringToHex(u8* string,u8 len);
	u32 HexStringToHex(u8* string,u8 len);	
	

	

	
#endif 
