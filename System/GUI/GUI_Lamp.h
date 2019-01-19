#ifndef __GUI_LAMP_H__
#define __GUI_LAMP_H__

#include "stm32f10x.h"  
#include "GUI_form.h"

#define 	GUI_USELAMP									//����ʹ��GUI_LAMP
#define		LAMP_MEMNUM					20			//LAMPʹ��14���ڴ�
	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X, Y;
		u16 BColor, ONColor, OFFColor;	
//		GUI_FONT *Font;		
//		GUI_FormStruct *Father;		
	}GUI_LampStruct;	


	GUI_LampStruct *GUI_LampCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y);			//����LAMP
	void GUI_LampSetON(GUI_LampStruct *GUI_Lamp_P);																		//����LAMPΪON
	void GUI_LampSetOFF(GUI_LampStruct *GUI_Lamp_P);																	//����LAMPΪOFF
	extern void GUI_LampShowNow(GUI_LampStruct *P);	

#endif 
