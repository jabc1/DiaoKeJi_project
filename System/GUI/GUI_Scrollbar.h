#ifndef __GUI_SCOROLLBAR_H__
#define __GUI_SCOROLLBAR_H__

#include "stm32f10x.h"  
#include "gui_FORM.h"


#define		GUI_USEScrollbar

#define		SCOROLLBAR_MIN_LEN					50			//S
#define		SCOROLLBAR_WIDTH					18			//




	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X, Y;
		u16 BColor,FColor,SelectColor;
		u16 Len;
 	u16 totalitems;					//����Ŀ��
	u16 itemsperpage;				//ÿҳ��ʾ����Ŀ��
	u16 topitem;					//�/��˵���Ŀ 
	u16 scbbarlen;					//��������		
		
		
//		
//		u16 Min;
//		u16 Max;
//		u16 OldValue;
//		u16 Value;		
	}GUI_ScrollbarStruct;	
	
	
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----ON/OFF				= �ؼ�״̬ΪON/OFF��ͬʱlable�Ȳ���ҪON/OFF����Ϊ��Ч״̬
										|||| +-----����					
										|||+-------H/V 				= 0��V; 1��H
										||+--------Init				=0:��Ҫ��ʼ���� 1����ʼ�����
										|+---------����			
										+----------�Ƿ�Ϊ����״̬*/

	
	#define		SCOROLLBAR_MODE 	0x10   		//��������ˮƽ�Ļ��Ǵ�ֱ��
	#define		SCOROLLBAR_INIT 	0x20   		//��������ˮƽ�Ļ��Ǵ�ֱ��
		
	extern GUI_ScrollbarStruct *GUI_ScrollbarCreate(u16 X, u16 Y,u16 len,u8 Mode);
	extern void GUI_ScrollbarSetValue(GUI_ScrollbarStruct *GUI_Scrollbar_P,u32 value)	;
	extern void GUI_ScrollbarShowNow(GUI_ScrollbarStruct *P);
	u8 GUI_ScrollbarTouchDown(GUI_ScrollbarStruct *Scrollbar_P,u16 X,u16 Y);
#endif 
