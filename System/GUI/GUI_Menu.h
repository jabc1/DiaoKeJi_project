#ifndef __GUI_MENU_H__
#define __GUI_MENU_H__

#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"
#include "GUI_Scrollbar.h" 

	#define 	GUI_USEMENU							//����ʹ��GUI_MENU	
	#define 	MENU_MAXITEMNUM			8				//�˵������Ĳ˵�����	
	#define 	MENU_MIN_W			40				//�˵�����С���
	#define 	MENU_MAX_W			GUI_W				//�˵��������
	
	#define 	MENU_MIN_H			32				//�˵�����С�߶�		
	#define 	MENU_MAX_H			GUI_H				//�˵������߶�	
//	#define 	MENU_MEMNUM					32
	//���ȶ���һ���˵��ṹ
	typedef struct GUImenu//����һ���˵�
	{
	 u8 itemCount;//��Ŀ����
	 u8 *menuItems[MENU_MAXITEMNUM];//�˵���Ŀ
	 struct GUImenu **subMenus;//�Ӳ˵�
	 struct GUImenu *parent;//�ϼ��˵� ,����Ƕ�����Ϊnull
	 void (**func)();//ѡ����Ӧ�ȷ������ִ�еĺ���
	}GUIMenu;

	typedef struct
	{
		u8 ID;	
		u8 State;		
		u16 X,Y,W,H;
//		u16 BColor, FColor, SelectColor;	
		u16 L_B_Color;					//�ڲ�������ɫ
		u16 L_S_Color; 				//list name ѡ�к����ɫ
//		u16 L_SB_Color; 				//list name ѡ�к�ı�����ɫ
		u16 F_Color; 					//list name δѡ�е���ɫ
		u16 Rim_Color;					//�߿���ɫ
		u16 I_B_Color;			    //·��/��Ϣ���ı�����ɫ				
		
		
		
		
		GUI_FontStruct *Font;		
		u8 MaxLen;
		u8 ShowItemNum;			//�˵���ʾ������
		u8 Range_from;			//��ǰ��ʾ���ʼ���			
		u8 Selected;				//��ǰѡ����			
//		u8 OnLongTouch;
		u8 OsPriority;
		GUI_ScrollbarStruct * Scrollbar;			//��ֱ������
		GUIMenu *CurrentMenus;		
//		GUI_FormStruct *Father;
	}GUI_MenuStruct;
	
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----0x04  MENU_USE_LONG_TOUCH 	  		//�ؼ�����ʱ�Ƿ��ظ�����			
										|||| +-----0x08  MENU_LONG_STATE			 	   		//�ؼ��������һ�α�־				
										|||+-------0x10  RunFirst 	�ؼ���һ������			= 0��NO; 1��YES
										||+--------LongTouch				0:flase;1:true;	
										|+---------ButtonFuntionMode			0:�ص�������ע��button���߳���ִ��	;1:�ص�������GUI�߳���ִ��
										+----------�Ƿ�Ϊ����״̬*/
	#define		MENU_USE_LONG_TOUCH 	0x04   		//�ؼ�����ʱ�Ƿ��ظ�����
	#define		MENU_LONG_STATE			 	0x08   		//�ؼ��������һ�α�־
	#define		MENU_RUN_FRIST			 	0x10   		//�ؼ���һ������
	#define		GUI_FOCUS_STATE 			0X40			//�ؼ��������ʾ״̬
	
	
	GUI_MenuStruct *GUI_MenuCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u16 W,u16 H);
	void GUI_SetCurrentMenus(GUI_MenuStruct *Menu_P,GUIMenu *CurrentMenus);
	extern void GUI_MenuShowNow(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuLostFocus(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuGetFocus(GUI_MenuStruct *Menu_P);	
	extern void GUI_MenuKeyFirst(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuKeyOff(GUI_MenuStruct *Menu_P);
	extern void GUI_MenuKeyLong(GUI_MenuStruct *Menu_P);			
	extern u8 GUI_MenuTouchDown(GUI_MenuStruct *Menu_P,u8 IDNum);
	extern void GUI_MenuTouchUp(GUI_MenuStruct *Menu_P);
	extern u8 GUI_MenuTouchLong(GUI_MenuStruct *Menu_P);	
	
	
	
	
#endif 
