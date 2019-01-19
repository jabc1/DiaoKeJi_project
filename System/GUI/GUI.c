/*********************************************************************************
* �ļ���			: GUI.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI���湫������								  
* �÷�			��
V3.0.0
	1.��ʾ��API�����˵�������ԭ��������GUI_API�ṹ���ж���������ʹ��GUI��LCD�Ĺ�����������.
	2.������GUI_OS.C��ʹ��GUI_OS��ΪMSOS�Ķ������������У���������������ȼ�����.
	3.������GUI_KEY��GUI_TOUCH,ʹ��Key��Touch��GUI����������ͨ��MSOS��������Ϣ������ֱ�ӵ���
V2.2.0
	����ͷ�ļ�����config�ļ��Ϳ��ļ���ͬ���������û�����
V2.1.2	
	ȡ����
	void GUI_SetState(void *GUI_P,GUIStateEnum state);
	void GUI_SetXY(void *GUI_P,u16 X,u16 Y);
	void GUI_SetColor_T(void *GUI_P,u16 FColor,u16 BColor)	;	
	void GUI_SetColor_K(void *GUI_P,u16 BColor,u16 OnColor,u16 OffColor)	;	
	void GUI_SetFont(GUI_FONT *Font_P);
	�����������û��Լ�ֱ���ڽṹ�����޸Ķ�Ӧ��ֵ
	
V2.1.1
	����state�����ú��� set��reset get
V2.1.0
	����state�Ķ��壬��λ�ı�־���ж���
 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬										
										|||| |+----ON/OFF				= �ؼ�״̬ΪON/OFF��ͬʱlable�Ȳ���ҪON/OFF����Ϊ��Ч״̬
										|||| +-----����					
										|+++-------����
										+----------�Ƿ�Ϊ����״̬
V2.0.0
	���Ӵ���������
	
V1.1.0
	�����Ŀؼ�LCD��ʾ

	
**********************************************************************************/

#include "GUI.h" 
#include "GUI_Key.h" 
#include "GUI_Draw2d.h"
#include <math.h> 
GUI_Struct GUI;					//����GUI��ȫ�ֱ���


#if GUI_USETOUCH				//���ʹ�ô������Ļ�
	extern void GUI_Touch(void);
#endif

void GUI_Exc(void)
{
//	#if GUI_USETOUCH				//���ʹ�ô������Ļ�
//		GUI_Touch();
//	#endif
////������ֵ�жϺͿ���
//	#if GUI_USEKEY				//���ʹ�ð���		
//				switch (UintToByte1(GUI.KeyState))
//				{
//					case KEYFIRST:
//						GUI_KeyDown();
//						break;
//					case KEYOFF:
//						GUI_KeyOff();
//						break;
//					case KEYLONG:
//						GUI_KeyLong();
//						break;	
//					default:
//						break;
//				}	
//	#endif		
//		
//		
//	//ȷ�ϵ�ǰCurrentForm��form�ؼ�
//	
//	if(GUI.CurrentForm->ID == GUIFormID)
//	{
//	GUI_FormShowNow(GUI.CurrentForm);	//ˢ�¿ؼ�
//	}
}
#ifndef BitAddr
	//IO�ڲ���,ֻ�Ե�һ��IO��!ȷ��n��ֵС��16!
	#define BIT_BAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
	#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
	#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BIT_BAND(addr, bitnum)) 
	#define	BitAddr(addr)   (vu32*)((((u32)(addr) & 0xF0000000)+0x2000000+((u32)(addr)<<5)));
#endif
/********************************************
������InitializeGUI
���ܣ���ʼ��GUI
��������
���أ���
********************************************/
			
void InitializeGUI(void)
{	
	GUI.B_Sate_P = (GUI_State_B_Struct *)BitAddr(&(GUI.State));

	#if GUI_USEMSOS	
		GUI.CreateTask = GUICreateTask;//GUI��������
	#endif
	
	//GUI��һЩ��������
	GUI.B_Color = GUI_Black;											//GUI�ı���ɫ
	GUI.F_Color = GUI_Red;											//GUI��������ɫ
	GUI.DIS_Color = GUI_Grey;											//GUI��DIS�ؼ�����ɫ
	GUI.KeyState = 0;													//GUI�İ���״̬:�ް���	
	GUI.CurrentForm = NULL;										//GUI�ĵ�ǰ���ڳ�ʼ��Ϊû��
	GUI.MemDevFlag = 0;
	//��ʼ��GUI DRIVER���豸
	
//	DelayMs(1000);
	
	GUI_InitLcd();															//LCD��ʼ��
	GUI_Clear(GUI.B_Color);										//����
	#if GUI_USETOUCH								//���ʹ�ô�����
		GUI.TouchState = 0;;							//GUI�Ĵ�����״̬
		GUI.Touch_X = 0;													//GUI�Ĵ���������ʱ��X����
		GUI.Touch_Y = 0;													//GUI�Ĵ���������ʱ��Y����
//		GUI_InitializeTouch();														//��ʼ��������	
		
	#endif
	
	

	
	GUI.Font = &Font8x16;
}
/********************************************
������IsGUIID
���ܣ��ж��Ƿ�ΪGUI ID
������ID ���жϵı��
���أ���/��
********************************************/
u8 IsGUIID(u8 ID)
{
	if((ID>=MINGUIID)&&(ID<=MAXGUIID))
	{
		return GUI_true;			
	}
	else
	{			
		return GUI_false;
	}
}



/********************************************
������GUI_FocusSelectChang
���ܣ������µĽ���
������NewFocusID���µĽ���ؼ���form�еı��
���أ���
********************************************/
void GUI_FocusSelectChang(u8 NewFocusID)
{
	if(NewFocusID != GUI.CurrentForm->FocusID)		//����ѡ���б仯
	{
		if(GUI.CurrentForm->FocusID == 0)
		{
//			FunckeyLostFocus();			
		}
		else
		{
			switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1])			//ԭ���Ľ������
			{
			#ifdef	GUI_USEBUTTON					
				case GUIButtonID:
					GUI_ButtonLostFocus((GUI_ButtonStruct *)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);				
					break;			
			#endif		
			#ifdef	GUI_USEMENU					
				case GUIMenuID:
					GUI_MenuLostFocus((GUI_MenuStruct *)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
					break;	
			#endif		
			#ifdef	GUI_USEEDIT				
				case GUIEditID:
					GUI_EditLostFocus((GUI_EditStruct *)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
					break;		
			#endif		
			default :						
				break;
			}			
		}
		if(NewFocusID == 0)
		{
//			FunckeyGetFocus();
		}
		else
		{
			switch (*GUI.CurrentForm->Pointer[NewFocusID-1])
			{	
			#ifdef	GUI_USEBUTTON						
				case GUIButtonID:
					GUI_ButtonGetFocus((GUI_ButtonStruct *)GUI.CurrentForm->Pointer[NewFocusID-1]);				
					break;
			#endif		
			#ifdef	GUI_USEMENU					
				case GUIMenuID:		
					GUI_MenuGetFocus((GUI_MenuStruct *)GUI.CurrentForm->Pointer[NewFocusID-1]);		
					break;	
			#endif		
			#ifdef	GUI_USEEDIT					
				case GUIEditID:	
					GUI_EditGetFocus((GUI_EditStruct *)GUI.CurrentForm->Pointer[NewFocusID-1]);	
					break;
			#endif			
				default :						
					break;
			}	
		}
			GUI.CurrentForm->FocusID = NewFocusID;							//�����½���			
	}		
}

static const u8 HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
void HexToString(u32 hex,u8* string,u8 len)
{
	u8 i;
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;								
		*string = HextoAscii[hex%0x10];
		hex = hex/0x10;				
	}		
}
void DecToString(u32 Dec,u8* string,u8 len)
{
	u8 i;
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;								
		*string = Dec%10+0x30;		// 0x30 = "0"
		Dec = Dec/10;
	}
}

u32 DecStringToHex(u8* string,u8 len)
{
	u8 i;
	u32 hex = 0;
	
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;	
		if((*string>=0x30)&&(*string<=0x39))			//������0-9
		{
			hex += (*string-0x30) * pow(10,i);
		}
		else
		{
			return hex;
		}

	}	
	return hex;
}

u32 HexStringToHex(u8* string,u8 len)
{
	u8 i;
	u32 hex = 0;	
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;	
		if((*string>=0x30)&&(*string<=0x39))			//������0-9
		{
			hex += (*string-0x30) * pow(0x10,i);
		}
		else if((*string>=0x41)&&(*string<=0x46))			//������A-F
		{
			hex += (*string-0x41+10) *  pow(0x10,i);
		}
		else
		{
			return hex;
		}

	}	
	return hex;
}


/********************************************
������GUI_MemSet
���ܣ��ڴ渴�ƺ���(��Ϊ��ϵͳmemset�����ᵼ��Ī��������,��ȫ�������������)
������Mem_P���ڴ��׵�ַ;Data:Ҫͳһ���õ�ֵ;Len:���ó���
���أ���
********************************************/
void GUI_MemSet(void *Mem_P,u8 Data,u32 Len)
{
	u8 *Temp_P=(u8*)Mem_P;
	while(Len)
	{
		*Temp_P=Data;
		Temp_P++;
		Len--;		
	}
}





