#ifndef __GUI_LED_H__
#define __GUI_LED_H__

#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"

#define		GUI_USELED
#define		LED_MEMNUM					24			//LAMP使用14个内存
	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X, Y;
		u16 BColor, ONColor, OFFColor;	
		GUI_FontStruct *Font;
		u8 Len;
		u32 Value;		
//		GUI_FormStruct *Father;
	}GUI_LEDStruct;	
	extern GUI_LEDStruct *GUI_LEDCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y,u8 len);
	extern void GUI_LEDSetValue(GUI_LEDStruct *GUI_LED_P,u32 value)	;
	extern void GUI_LEDShowNow(GUI_LEDStruct *P);
#endif 
