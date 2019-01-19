/*********************************************************************************
*�ļ���   : gui.h
*����     : GUI�ṹ��ͷ�ļ�
*����ʱ�� : 2015.10.29
*�汾			��V0.0.1
*˵��			:����İ汾
*********************************************************************************/
#ifndef __GUI_EDIT_H__
#define __GUI_EDIT_H__
#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"


#define		GUI_USEEDIT
	#define	EDIT_MEMNUM				20+EDIT_MAXSTRINLEN			//edit ������ʱ��������ڴ�����
#define 	EDIT_MAXSTRINLEN		18
	typedef struct
	{
		u8 ID;	
		u8 State;		
		u16 X,Y;
		u16 BColor,FColor;	
		u8 MaxLen;	
		u8 FocusLen;
		GUI_FontStruct *Font;
		u8 Mode;
//		GUI_FormStruct *Father;
		u8 String[EDIT_MAXSTRINLEN];
	}GUI_EditStruct;
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----����			
										|||| +-----����
										|||+-------FOCUSEDIT		= ���selectLen>0 	0:ֻ����ѡ��״̬��1�� edit״̬ 	
										||+--------COLORTYPE		=	FocusLen ѡ������� 0: ��ʾ������ɫ��1�� ��ʾ��ת��ɫ
										|+---------����
										+----------�Ƿ�Ϊ����״̬*/
	#define EDIT_FOCUSEDIT		0X10										//edit�Ľ���״̬
	#define EDIT_COLORTYPE		0X20										//����ѡ���������ɫ״̬	

	
	GUI_EditStruct *GUI_EditCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u8 MaxLen,GUI_STRINGMODE Mode);
	u8 GUI_EditSetString(GUI_EditStruct *GUI_Edit_P,u8 *String);
	void GUI_EditGetString(GUI_EditStruct *GUI_Edit_P,u8 *String);
	u8 GUI_EditSetValue(GUI_EditStruct *GUI_Edit_P,u32 Value);	
//	u32 GUI_EditGetValue(GUI_EditStruct *GUI_Edit_P);	
	extern void GUI_EditShowNow(GUI_EditStruct *GUI_Edit_P);
	extern void GUI_EditLostFocus(GUI_EditStruct *GUI_Edit_P);
	extern void GUI_EditGetFocus(GUI_EditStruct *GUI_Edit_P);
	extern void GUI_EditKeyFirst(GUI_EditStruct *GUI_Edit_P);		
	extern void GUI_EditTouchUp(GUI_EditStruct *GUI_Edit_P);
	extern u8 GUI_EditTouchDown(GUI_EditStruct *GUI_Edit_P,u8 Num);	
	
#endif 
