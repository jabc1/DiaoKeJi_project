/*********************************************************************************
*�ļ���   : gui.h
*����     : GUI�ṹ��ͷ�ļ�
*����ʱ�� : 2015.10.29
*�汾			��V0.0.1
*˵��			:����İ汾
*********************************************************************************/
#ifndef __GUI_FUNCTIONKEY_H__
#define __GUI_FUNCTIONKEY_H__

#include "stm32f10x.h"  
#include "gui_FORM.h"
#include "GUI_Struct.h"
#include "GUI_config.h"

#define GUI_FUNCKEY_KEY				0xE0			//ʹ��funckeyd��ʱ�򣬷��͵ļ�ֵΪGUI_FUNCKEY_KEY + NUM
#define 	GUI_USEFUNCKEY								//����ʹ��GUI_FUNCKEY

	typedef struct //����һ���˵�
	{
		u8 String[5];//��Ŀ����
	}FunckeyStringStruct;

	#define	FUNCKEY_MEMNUM			20	
	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X,Y;
		u16 BColor, FColor, SelectColor;	
		GUI_FONT *Font;
		#if GUI_USEMSOS
			u8 OsPriority;
		#endif		

		u8 KeyNum;
		u8 SelectNum;
//		const u8* key;
		FunckeyStringStruct *String;		
	}FunckeyStruct;		
	
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----ON/OFF				= �ؼ�״̬ΪON/OFF��ͬʱlable�Ȳ���ҪON/OFF����Ϊ��Ч״̬
										|||| +-----����					
										|||+-------����
										||+--------Longtouch				0:Long ON;1:Long OFF;	
										|+---------focusin			
										+----------�Ƿ�Ϊ����״̬*/
										


	#define FUNCKEY_TOUCH				0x20		//���ܼ�������
	#define FUNCKEY_FOCUSIN			0x40	//�������뽹���ڲ�	
	FunckeyStruct *GUI_FunckeyCreate(void);
	void GUI_FunckeyShowNow(void);
	extern u8 GUI_FunckeyTouchDown(void);
	extern u8 GUI_FunckeyTouchLong(void);
	extern u8 GUI_FunckeyTouchUp(void);
	
#endif
	

	



