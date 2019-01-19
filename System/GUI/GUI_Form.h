#ifndef __GUI_FORM_H__
#define __GUI_FORM_H__
#include "stm32f10x.h"  
#include "GUI_font.h"


/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----ON/OFF				= �ؼ�״̬ΪON/OFF��ͬʱlable�Ȳ���ҪON/OFF����Ϊ��Ч״̬
										|||| +-----����					
										|||+-------EditUseKey 				= 0��û��ʹ��; 1��editʹ����funckey
										||+--------Longtouch				0:Long ON;1:Long OFF;	
										|+---------focusin			
										+----------�Ƿ�Ϊ����״̬*/

	
	
	#define FROM_BESTMEMNUM 16  //from��ϵͳ����ռ�õ��ڴ�����
	typedef struct
	{
		u8 ID;	
		u8 State;	
		u16 X,Y;

		u16 Color1, Color2, Color3;		
		GUI_FontStruct *Font;	
		u16 W,H;
			
		
	}GUI_ObjStruct;	
	
	
	
	
	typedef struct
	{
		u8 ID;					//FORM�Ŀؼ�ID
//		u8 State;
		u8 NUM;					//�ӿؼ�������
		u8 MemNum;			//ռ���ڴ������
		u8 TouchNum;			//touch�����ĸ��ӿؼ�
		u8 FocusID;				//��ǰ�����GUI��form�еı��
		u8 FocusIsSelect;	//�����Ƿ�ѡ������ڲ�
		u8 *Pointer[30];			//�����30��ֻ��Ϊ�˵��Ե�ʱ���ܹ������ã�ʵ�ʵ������Դ�����ʱ�������Ϊ׼
	}GUI_FormStruct;
	
	
	u8 GUI_FormDelete(GUI_FormStruct *GUI_Form_P);															//ɾ��FROM
	u8 GUI_FormAddChild(GUI_FormStruct *GUI_Form_P,u8 *Child_P);								//Form�����ӿؼ�
	GUI_FormStruct *GUI_FormCreate(u8 ChildNum);																//����Form
	u8 GUI_FormDeleteAllChild(GUI_FormStruct *GUI_Form_P);											//ɾ��FROM�������ӿؼ�
	u8 GUI_FormDeleteChild(GUI_FormStruct *GUI_Form_P,GUI_ObjStruct *Child_P);	//ɾ��FROM���ӿؼ�
	u8  GUI_FormShowNow(GUI_FormStruct *GUI_Form_P);														//ˢ��FROM�е������ӿؼ�
	u8  GUI_CurrenFormGetIDNum(void *Obj_P);
	
	u8 GUI_FormReset(GUI_FormStruct *GUI_Form_P)	;
	
	
#endif
	
	
	
