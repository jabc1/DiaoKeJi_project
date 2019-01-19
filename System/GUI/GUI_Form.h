#ifndef __GUI_FORM_H__
#define __GUI_FORM_H__
#include "stm32f10x.h"  
#include "GUI_font.h"


/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----ON/OFF				= 控件状态为ON/OFF，同时lable等不需要ON/OFF的作为有效状态
										|||| +-----保留					
										|||+-------EditUseKey 				= 0：没有使用; 1：edit使用了funckey
										||+--------Longtouch				0:Long ON;1:Long OFF;	
										|+---------focusin			
										+----------是否为焦点状态*/

	
	
	#define FROM_BESTMEMNUM 16  //from中系统数据占用的内存数量
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
		u8 ID;					//FORM的控件ID
//		u8 State;
		u8 NUM;					//子控件的数量
		u8 MemNum;			//占用内存的数量
		u8 TouchNum;			//touch的是哪个子控件
		u8 FocusID;				//当前焦点的GUI在form中的编号
		u8 FocusIsSelect;	//焦点是否选择进了内部
		u8 *Pointer[30];			//这里的30个只是为了调试的时候能够监视用，实际的数量以创建的时候的数量为准
	}GUI_FormStruct;
	
	
	u8 GUI_FormDelete(GUI_FormStruct *GUI_Form_P);															//删除FROM
	u8 GUI_FormAddChild(GUI_FormStruct *GUI_Form_P,u8 *Child_P);								//Form增加子控件
	GUI_FormStruct *GUI_FormCreate(u8 ChildNum);																//创建Form
	u8 GUI_FormDeleteAllChild(GUI_FormStruct *GUI_Form_P);											//删除FROM的所有子控件
	u8 GUI_FormDeleteChild(GUI_FormStruct *GUI_Form_P,GUI_ObjStruct *Child_P);	//删除FROM的子控件
	u8  GUI_FormShowNow(GUI_FormStruct *GUI_Form_P);														//刷新FROM中的所有子控件
	u8  GUI_CurrenFormGetIDNum(void *Obj_P);
	
	u8 GUI_FormReset(GUI_FormStruct *GUI_Form_P)	;
	
	
#endif
	
	
	
