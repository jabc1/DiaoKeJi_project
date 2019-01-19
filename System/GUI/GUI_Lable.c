/*********************************************************************************
* �ļ���			: gui_lable.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI Lable�ؼ�								  
* �÷�			��

**********************************************************************************/
#include "GUI_lable.h" 
#include "GUI_DRAW2D.h"
#include "GUI.h"
 #include "malloc.h"
/************************�Լ��ĺ�������********************************************************/
	

/**************�궨��******************************/

#define	  	D_LABLE_FCOLOR 		GUI_Red
#define	  	D_LABLE_BCOLOR 		GUI_Grey

/********************************************
������Lable_Create
���ܣ�����Lable
������ X X��λ�ã�	Y Y��λ��
���أ���ť���ڴ��еĵ�ַָ�� ������򷵻�0
********************************************/
GUI_LableStruct *GUI_LableCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u8 MaxLen)
{
	GUI_LableStruct *GUI_Lable_P;
	if(Form_P->ID != GUIFormID) return 0;	
	if (MaxLen >LABLE_MAXSTRINGLEN)MaxLen = LABLE_MAXSTRINGLEN;
	GUI_Lable_P = (GUI_LableStruct *)GUI_MEM_Malloc(sizeof(GUI_LableStruct));	//����LABLE_MEMMUN���ֽڵ��ڴ�
	if(GUI_Lable_P)
	{
		GUI_Lable_P->X = X;
		GUI_Lable_P->Y =Y;
		GUI_Lable_P->ID = GUILableID;
		GUI_Lable_P->MaxLen = MaxLen;
		GUI_Lable_P->Font = &Font8x12;
		GUI_Lable_P->BColor = D_LABLE_BCOLOR;
		GUI_Lable_P->FColor = D_LABLE_FCOLOR;
		GUI_Lable_P->State = GUI_SHOWOK;
		if(GUI_FormAddChild(Form_P,(u8*)GUI_Lable_P))
		{
			GUI_MEM_Free(GUI_Lable_P);	//�ͷ��ڴ�
			return 0;
		}
		else
		{
//			GUI_Lable_P->Father = Form_P;
			return GUI_Lable_P;
		}
	}
	else
	{
		return 0;
	}

}
/********************************************
������GUI_SetLableState
���ܣ�����Lable��״̬
������ GUI_Lable_P Lable��ַָ�룻state Lable��״̬
���أ���
˵����
state��ֵӦ������
	typedef enum 
	{
		GUIState_Redraw 		= 0x01,
		GUIState_Disable 		= 0x02,
		GUIState_Enable 		= 0x03,
		GUIState_ON					= 0X04,
		GUIState_OFF				= 0X05,
	}GUIStateEnum;	
Lableû��ON/OFF״̬

********************************************/
void GUI_LableSetState(GUI_LableStruct *GUI_Lable_P,GUIStateEnum state)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//����Lable������
	switch (state)
	{
		case GUIState_Redraw:
			GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);
			break;
		case GUIState_Disable:
			GUISetBit(GUI_Lable_P->State,GUI_DIS);			
			break;
		case GUIState_Enable:
			GUIResetBit(GUI_Lable_P->State,GUI_DIS);					
			break;		
		default:
			break;
	}	
}

/********************************************
������SetLableString
���ܣ�����lable���ı�
������ GUI_Lable_P lable��ַָ�룻String ��Ҫ���õ��ַ���
���أ���
˵����������ʾ��������⣬����ֻ֧��20���ַ���1��������2���ַ�
********************************************/
void GUI_LableSetString(GUI_LableStruct *GUI_Lable_P,u8 *String)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//����Lable������	
	GUI_Lable_P->String = String; 
	GUI_Lable_P->Mode =  GUI_STRING;
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//�ؼ����ػ�
}
/********************************************
������GUI_SetLableDecValue
���ܣ�����10���Ƶ���ֵ
������ GUI_Lable_P lable��ַָ�룻Value ��Ҫ���õ���ֵ
���أ���
˵����Lable����ʾģʽΪ10��������
********************************************/
void GUI_LableSetDecValue(GUI_LableStruct *GUI_Lable_P,u32 Value)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//����Lable������		
	if(GUI_Lable_P->MaxLen> LABLE_MAXVALUEGLEN) GUI_Lable_P->MaxLen = LABLE_MAXVALUEGLEN;
	GUI_Lable_P->Mode =  GUI_DEC;
	DecToString(Value,GUI_Lable_P->Value,GUI_Lable_P->MaxLen);
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//�ؼ����ػ�
}
/********************************************
������GUI_SetLableHexValue
���ܣ�����16���Ƶ���ֵ
������ GUI_Lable_P lable��ַָ�룻Value ��Ҫ���õ���ֵ
���أ���
˵����Lable����ʾģʽΪ16��������
********************************************/
void GUI_LableSetHexValue(GUI_LableStruct *GUI_Lable_P,u32 Value)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//����Lable������		
	if(GUI_Lable_P->MaxLen> LABLE_MAXVALUEGLEN) GUI_Lable_P->MaxLen = LABLE_MAXVALUEGLEN;
	GUI_Lable_P->Mode =  GUI_HEX;
	HexToString(Value,GUI_Lable_P->Value,GUI_Lable_P->MaxLen);
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//�ؼ����ػ�	
}
/********************************************
������GUI_SetLableDate
���ܣ������ꡢ�¡��յ�����
������ GUI_Lable_P lable��ַָ�룻Date ��Ҫ���õ�����ָ��
���أ���
˵����Lableֻ��ʾ8λ����ʽΪYY-MM-DD
********************************************/
void GUI_LableSetDate(GUI_LableStruct *GUI_Lable_P,u8* Date)	
{
	u8 i;
	u8 *Temp_P;
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//����Lable������		
	GUI_Lable_P->MaxLen = 8;
	GUI_Lable_P->Mode =  GUI_DATE;
	Temp_P = GUI_Lable_P->Value;
	for(i=0;i<3;i++)
	{			
		*Temp_P = ((*Date)/10)+0x30;
		Temp_P++;
		*Temp_P = ((*Date)%10)+0x30;
		Temp_P++;		
		Date++;
		*Temp_P = 45;	
		Temp_P++;		
	}
	Temp_P--;	
	*Temp_P = 0;
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//�ؼ����ػ�	
}
/********************************************
������GUI_SetLableTime
���ܣ�����ʱ���֡����ʱ��
������ GUI_Lable_P lable��ַָ�룻Time ��Ҫ���õ�ʱ��ָ��
���أ���
˵����Lableֻ��ʾ8λ����ʽΪHH:MM:SS
********************************************/
void GUI_LableSetTime(GUI_LableStruct *GUI_Lable_P,u8* Time)	
{
	u8 i;
	u8 *Temp_P;
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//����Lable������		
	GUI_Lable_P->MaxLen = 8;
	GUI_Lable_P->Mode =  GUI_TIME;
	Temp_P = GUI_Lable_P->Value;
	for(i=0;i<3;i++)
	{			
		*Temp_P = ((*Time)/10)+0x30;
		Temp_P++;
		*Temp_P = ((*Time)%10)+0x30;
		Temp_P++;	
		Time++;		
		*Temp_P = 58;	
		Temp_P++;		
	}
	Temp_P--;	
	*Temp_P = 0;
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//�ؼ����ػ�	
}
/********************************************
������LableShowNow
���ܣ�����ʾ������ʾlable���ı�
������ GUI_Lable_P lable��ַָ��
���أ���
˵����������û�иı�ֻ��ʾһ��
********************************************/
void GUI_LableShowNow(GUI_LableStruct *GUI_Lable_P)						   //
{
	u8 MaxLen;
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//����Lable������
	if(!GUIGetBit(GUI_Lable_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
	{
		if(GUI_Lable_P->Y>(GUI_H-16)) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���	
		//������ַ�����
		MaxLen = (GUI_W - GUI_Lable_P->X)/8;			//from���ܹ���ʾ�����ֳ���
		if(MaxLen>GUI_Lable_P->MaxLen)									//����ܹ���ʾ�ĳ��ȴ���������ֳ���
		{
			MaxLen = GUI_Lable_P->MaxLen;			
		}		
		if(GUIGetBit(GUI_Lable_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{	
			
		}
		else
		{	

			switch(GUI_Lable_P->Mode)
			{
				case GUI_STRING://lable����ʾģʽΪ����
		GUI_DrawFillRectangle(GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_Lable_P->Font->Y-1,GUI_Lable_P->BColor);
		GUI_ShowStringAt(GUI_Lable_P->String,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->FColor,GUI_Lable_P->BColor,GUI_Lable_P->Font);
					
//					GUI_PutString(GUI_Lable_P->String,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_API.Font->Y-1,0,GUI_Lable_P->FColor,GUI_Lable_P->BColor);	
					break;
				case GUI_DEC://lable����ʾģʽΪ10��������
				case GUI_HEX://lable����ʾģʽΪ16��������	
				case GUI_TIME://lable����ʾģʽΪ10��������
				case GUI_DATE://lable����ʾģʽΪ10��������				
		GUI_DrawFillRectangle(GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_Lable_P->Font->Y-1,GUI_Lable_P->BColor);
		GUI_ShowStringAt(GUI_Lable_P->Value,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->FColor,GUI_Lable_P->BColor,GUI_Lable_P->Font);
					
//					GUI_PutString(GUI_Lable_P->Value,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_API.Font->Y-1,0,GUI_Lable_P->FColor,GUI_Lable_P->BColor);	
					break;	
				default:
					break;
			}
		}
		GUISetBit(GUI_Lable_P->State,GUI_SHOWOK);			//�ؼ���ʾ���
	}
}







