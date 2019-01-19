/*********************************************************************************
*�ļ���   : gui.h
*����     : GUI�ṹ��ͷ�ļ�
*����ʱ�� : 2015.10.29
*�汾			��V0.0.1
*˵��			:����İ汾
*********************************************************************************/
#ifndef __GUI_KEYBOARD_H__
#define __GUI_KEYBOARD_H__
#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"

#define 	GUI_USEKEYBOARD								//����ʹ��GUI_USEKEYBOARD

	#define	KeyBoard_MEMNUM			32	//	20+KEYNUM*4	 320 KEYNUM = 8;240 KEYNUM = 6

	typedef struct
	{
		u8 ID;																//KeyBoard ��ID��ţ�Ĭ�ϲ����޸�
		u8 State;															//״̬
		u16 X,Y;															//X,Y������
		u16 BColor, FColor, SelectColor;			//KeyBoard ����ɫ
		GUI_FontStruct *Font;												//����
		u8 W;																	//���������Ŀ��
		u8 H;																	//���������ĸ߶�
		u8 X_Num;															//ÿ���м�������
		u8 Y_Num;															//���м���
		u8 SelectNum;													//��ֵ
		u8 StringLen;													//���ֳ��ȣ������1λ0x00�����ֽ�����־������2���ַ��Ļ�Ҫ����3����������
		u8 Char_X,Char_Y;											//ÿ���������ֶ�Ӧ��X��Y��ƫ����	
		u8 KeyID;															//ֻ�ø�3λ����5λΪ��ֵ
		u8 OsPriority;
		GUI_STRINGMODE StringMode;						//��ʾ���������ͣ�����֧��string��dec��hex
		const u8* keyString;									//��������ָ��
	}GUI_KeyBoardStruct;		
	typedef struct
	{
		GUI_FormStruct *Form_P;
		u16 X,Y;
		u8 W;																	//���������Ŀ��
		u8 H;																	//���������ĸ߶�
		u8 X_Num;															//ÿ���м�������
		u8 Y_Num;															//���м���
		u8 Char_X;
		u8 Char_Y;		
		u8 KeyID;															//ֻ�ø�3λ����5λΪ��ֵ
		u8 StringLen;				//���ֳ��ȣ������1λ0x00�����ֽ�����־������2���ַ��Ļ�Ҫ����3����������	
		GUI_STRINGMODE StringMode;		
		const u8* keyString;
	}GUI_KeyBoardInitStruct;		
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----ON
										|||| +-----All				= ��һ����ʾ������ʾ���					
										|||+-------����	
										||+--------touch				��ť������
										|+---------focusin			
										+----------�Ƿ�Ϊ����״̬*/
										
	#define KeyBoard_INITOK	0x40
	#define KeyBoard_SHOWON	0x20	
	#define KeyBoard_TOUCH				0x20		//���ܼ�������
//	#define KeyBoard_FOCUSIN			0x80	//�������뽹���ڲ�	
	GUI_KeyBoardStruct *GUI_KeyBoardCreate(GUI_KeyBoardInitStruct *KeyBoardInit_P);
	void GUI_KeyBoardShowAll(GUI_KeyBoardStruct *KeyBoard_P);
	void GUI_KeyBoardShowNow(GUI_KeyBoardStruct *KeyBoard_P);
	u8 GUI_KeyBoardTouchUp(GUI_KeyBoardStruct *KeyBoard_P);
	u8 GUI_KeyBoardTouchLong(GUI_KeyBoardStruct *KeyBoard_P);
	u8 GUI_KeyBoardTouchDown(GUI_KeyBoardStruct *KeyBoard_P,u8 Num);


#endif
	

	



