#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"

#define 	GUI_USEBUTTON								//����ʹ��GUI_BUTTON
	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X, Y;
		u16 TextColor, ONColor, OFFColor;	
		GUI_FontStruct *Font;	
		u16 W, H;	
		u8 OsPriority;						//��Ӧ���ĸ��߳�ʹ�õĻص�����
    GUI_func Func_Down;		//�ص�����		
    GUI_func Func_Long;		//�ص�����				
    GUI_func Func_Up;		//�ص�����		
		u8* OffString;		
		u8* OnString;

	}GUI_ButtonStruct;
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----ON/OFF				= �ؼ�״̬ΪON/OFF��ͬʱlable�Ȳ���ҪON/OFF����Ϊ��Ч״̬
										|||| +-----����					
										|||+-------3D/2D 				= 0��3D; 1��2D
										||+--------Long ON/OFF				0:Long ON;1:Long OFF;	
										|+---------ButtonFuntionMode			0:�ص�������ע��button���߳���ִ��	;1:�ص�������GUI�߳���ִ��
										+----------�Ƿ�Ϊ����״̬*/

	
	#define		BUTTON_3D 				0x10   		//�ؼ��б仯
	#define		BUTTON_LONG 			0x20   		//�ؼ�ON			1

	GUI_ButtonStruct *GUI_ButtonCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y,u16 W,u16 H);
	void GUI_ButtonSetString(GUI_ButtonStruct *GUI_Button_P,u8 *OnString,u8* OffString)	;
	void GUI_ButtonRegisterFunction(GUI_ButtonStruct *GUI_Button_P,GUI_func registerFunction);
	extern void GUI_ButtonShowNow(GUI_ButtonStruct *GUI_Button_P);
	extern void GUI_ButtonLostFocus(GUI_ButtonStruct *GUI_Button_P);
	extern void GUI_ButtonGetFocus(GUI_ButtonStruct *GUI_Button_P);	
	extern void GUI_ButtonKeyDown(GUI_ButtonStruct *GUI_Button_P);/*Ӳ��������һ�ΰ��£�����button��״̬������ע��ĳ���*/
	extern void GUI_ButtonKeyLong(GUI_ButtonStruct *GUI_Button_P);/*Ӳ���������º���ظ�ȷ��ʱ������button��״̬������ע��ĳ���*/
	extern void GUI_ButtonKeyUp(GUI_ButtonStruct *GUI_Button_P);/*Ӳ�������ͷ�ʱ������button��״̬*/
	extern u8 GUI_ButtonTouchDown(GUI_ButtonStruct *GUI_Button_P,u8 Num);/*�������������µ�ʱ������button��״̬*/
	extern void GUI_ButtonTouchUp(GUI_ButtonStruct *GUI_Button_P);/*�����������ͷŵ�ʱ������button��״̬*/
	extern void GUI_ButtonTouchLong(GUI_ButtonStruct *GUI_Button_P);

#endif 
