#ifndef __GUI_LABLE_H__
#define __GUI_LABLE_H__

#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"

	#define 	LABLE_MAXSTRINGLEN	14							//lable �������ֳ���
	#define 	LABLE_MAXVALUEGLEN	10							//lable ������ֵ����
	#define 	GUI_USELABLE									//����ʹ��GUI_LAMP

	
	typedef struct
	{
		u8 ID;	
		u8 State;		
		u16 X,Y;

		u16 BColor, FColor;	
		u8 MaxLen;
		GUI_STRINGMODE Mode;							//��ʾ������		0��string 1��DEC			2��HEX
		GUI_FontStruct *Font;		
//		GUI_FormStruct *Father;
		u8 Value[LABLE_MAXVALUEGLEN];		//DEC��HEX,DATE,TIME��ʾ��ʱ���ֵ��ʹ��value�Ļ��Ͳ���string
		u8* String;		
	}GUI_LableStruct;	

	#define GUI_SetLableColor(GUI_P,BColor,FColor)		GUI_SetColor_T((GUI_ObjStruct*)GUI_P,BColor,FColor)
	#define GUI_SetLableXY(GUI_P,X,Y)									GUI_SetXY((GUI_ObjStruct*)GUI_P,X,Y)	
	#define		LABLE_MEMNUM				32							//lable ������ʱ��������ڴ�����										


	GUI_LableStruct *GUI_LableCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u8 MaxLen);
	void GUI_LableSetString(GUI_LableStruct *GUI_Lable_P,u8 *String);
	void GUI_LableSetDecValue(GUI_LableStruct *GUI_Lable_P,u32 Value);
	void GUI_LableSetHexValue(GUI_LableStruct *GUI_Lable_P,u32 Value);
	void GUI_LableSetDate(GUI_LableStruct *GUI_Lable_P,u8* Date);
	void GUI_LableSetTime(GUI_LableStruct *GUI_Lable_P,u8* Time);
	void GUI_LableSetState(GUI_LableStruct *GUI_Lable_P,GUIStateEnum state);		
	void GUI_LableShowNow(GUI_LableStruct *GUI_Lable_P);	
#endif 
