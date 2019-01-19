/*********************************************************************************
* �ļ���			: gui_Edit.c
* �汾��    	: V2.1.0
* ����      : 2015/09/25
* ˵��      : GUI Edit�ؼ�								  
* �÷�			��
 	   STATE = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----����			
										|||| +-----����
										|||+-------NEEDREDRAW
										||+--------FOCUSEDIT		= ���selectLen>0 	0:ֻ����ѡ��״̬��1�� edit״̬ 	
										|+---------COLORTYPE
										+----------�Ƿ�Ϊ����״̬
**********************************************************************************/
#include "GUI_edit.h" 
#include "GUI_DRAW2D.h"
#include "malloc.h"
#include "gui.h"
#include "os.h"
/************************�Լ��ĺ�������********************************************************/

	void GUI_EditKeyFirst(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditKeyLong(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditKeyOff(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditLostFocus(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditGetFocus(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditSeletColor(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditShowString(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditUseKeyUp(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditUseKeyDown(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditUseKeyEnter(GUI_EditStruct *GUI_Edit_P);
	void GUI_EditUseKeyEsc(GUI_EditStruct *GUI_Edit_P);	

	void GUI_EditTouchUp(GUI_EditStruct *GUI_Edit_P);
	u8 GUI_EditTouchDown(GUI_EditStruct *GUI_Edit_P,u8 Num);
/**************�궨��******************************/
#define	  	D_EDIT_FCOLOR 		GUI_Red
#define	  	D_EDIT_BCOLOR 		GUI_Grey




/********************************************
������Edit_Create
���ܣ�����Edit
������ X X��λ�ã�	Y Y��λ��
���أ���ť���ڴ��еĵ�ַָ�� ������򷵻�0
********************************************/
GUI_EditStruct *GUI_EditCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u8 MaxLen,GUI_STRINGMODE Mode)
{
 
	GUI_EditStruct *GUI_Edit_P;
	if(Form_P->ID != GUIFormID) return 0;	
	if (MaxLen >EDIT_MAXSTRINLEN)MaxLen = EDIT_MAXSTRINLEN;
	GUI_Edit_P=(GUI_EditStruct *)GUI_MEM_Malloc(sizeof(GUI_EditStruct));	//�����ڴ�
	if(GUI_Edit_P)
	{

		GUI_Edit_P->X = X;
		GUI_Edit_P->Y =Y;
		GUI_Edit_P->ID = GUIEditID;
		GUI_Edit_P->Font = &Font8x12;
		GUI_Edit_P->BColor = D_EDIT_BCOLOR;
		GUI_Edit_P->FColor = D_EDIT_FCOLOR;
		GUI_Edit_P->MaxLen = MaxLen;
		GUI_Edit_P->State = GUI_SHOWOK;
		GUI_Edit_P->Mode = Mode;
		if(GUI_FormAddChild(Form_P,(u8*)GUI_Edit_P))
		{
			GUI_MEM_Free(GUI_Edit_P);	//�ͷ��ڴ�
			return 0;
		}
		else
		{
//			GUI_Edit_P->Father = Form_P;
			return GUI_Edit_P;
		}
	}
	else
	{
		return 0;
	}

}


/********************************************
������SetEditString
���ܣ�����EDIT���ı�
������ GUI_Edit_P EDIT��ַָ�룻String ��Ҫ���õ��ַ���
���أ���
˵����������ʾ��������⣬����ֻ֧��20���ַ���1��������2���ַ�
********************************************/
u8 GUI_EditSetString(GUI_EditStruct *GUI_Edit_P,u8 *String)	
{
	u8 i;	
	if(GUI_Edit_P->ID != GUIEditID) 	return NOTGUIID;				//����Edit������	
	if(GUI_Edit_P->Mode == GUI_STRING)
	{
		for (i=0;i<EDIT_MAXSTRINLEN;i++)			//�Ȱ�ԭ������������
		{	
			GUI_Edit_P->String[i] = 0x00;	
		}	
		for (i=0;i<EDIT_MAXSTRINLEN;i++)
		{
			if(String[i] == 0x00)
			{	
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//�ؼ����ػ�				
				return 0;
			}
			GUI_Edit_P->String[i] = String[i];			
		}	
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//�ؼ����ػ�
	return 0;	
	}
	else
	{
		return 2;
	}
}
/********************************************
������GUI_SetEditValue
���ܣ�����EDIT����ֵ
������ GUI_Edit_P EDIT��ַָ�룻value ��Ҫ���õ���ֵ
���أ���
˵�������ݴ洢��string��
********************************************/

u8 GUI_EditSetValue(GUI_EditStruct *GUI_Edit_P,u32 Value)	
{
	u8 i;	
	if(GUI_Edit_P->ID != GUIEditID) 	return NOTGUIID;				//����Edit������	
		for (i=0;i<EDIT_MAXSTRINLEN;i++)			//�Ȱ�ԭ������������
		{	
			GUI_Edit_P->String[i] = 0x00;	
		}	
			switch(GUI_Edit_P->Mode)
			{
			case GUI_STRING://EDIT����ʾģʽΪ����
				break;
			case GUI_DEC://EDIT����ʾģʽΪ10��������
				DecToString(Value,GUI_Edit_P->String,GUI_Edit_P->MaxLen);
				break;
			case GUI_HEX://EDIT����ʾģʽΪ16��������
				HexToString(Value,GUI_Edit_P->String,GUI_Edit_P->MaxLen);					
				break;	
			case GUI_TIME://EDIT����ʾģʽΪ10��������
				break;	
			case GUI_DATE://EDIT����ʾģʽΪ10��������				
				break;				
			default:
				break;
			}			
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//�ؼ����ػ�
	return 0;
}

/********************************************
������GUI_SetEditValue
���ܣ�����EDIT����ֵ
������ GUI_Edit_P EDIT��ַָ�룻value ��Ҫ���õ���ֵ
���أ���
˵�������ݴ洢��string��
********************************************/

u32 GUI_EditGetValue(GUI_EditStruct *GUI_Edit_P)	
{
	u32 Value;
	if(GUI_Edit_P->ID != GUIEditID) 	return 0;				//����Edit������	
	switch(GUI_Edit_P->Mode)
	{
	case GUI_STRING://EDIT����ʾģʽΪ����
		break;
	case GUI_DEC://EDIT����ʾģʽΪ10��������
		Value =DecStringToHex(GUI_Edit_P->String,GUI_Edit_P->MaxLen);	
		break;
	case GUI_HEX://EDIT����ʾģʽΪ16��������
		Value =HexStringToHex(GUI_Edit_P->String,GUI_Edit_P->MaxLen);					
		break;	
	case GUI_TIME://EDIT����ʾģʽΪ10��������
		break;	
	case GUI_DATE://EDIT����ʾģʽΪ10��������				
		break;				
	default:
		break;
	}			
	return Value;
}

/********************************************
������GUI_EditShowString
���ܣ���ʾEDIT�е��ı�
������ GUI_Edit_P EDIT��ַָ��
���أ���
˵����
********************************************/

void GUI_EditShowString(GUI_EditStruct *GUI_Edit_P)
{
	u16 i;
	u16 X,Y;
	u16 BColor,FColor;
	X = GUI_Edit_P->X+2;
	Y = GUI_Edit_P->Y+2;	
	
		for (i=0;i<GUI_Edit_P->MaxLen;i++)
		{//EDIT_STATE_COLORTYPE/*����ѡ���λ�ô���0 ������*/	//������ҪEDIT
			if((i == GUI_Edit_P->FocusLen-1)&& \
				((!GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))||  \
			(GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT)&&GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE))))	//��Ҫedit����ɫ��仯
			{
				BColor = GUI_Edit_P->FColor;
				FColor = GUI_Edit_P->BColor;							
			}
			else
			{
				FColor = GUI_Edit_P->FColor;
			 	BColor= GUI_Edit_P->BColor;				
			}
			if(GUI_Edit_P->String[i] != 0x00)
			{							
				if(GUI_Edit_P->String[i] < 0x80) 
				{
					X +=GUI.Font->ASC_X;
					GUI_PutChar(&(GUI_Edit_P->String[i]),X,Y,FColor,BColor,GUI_Edit_P->Font);
				}
				else
				{
					X +=GUI.Font->GB_X;					
					GUI_PutGBChar((u8*)GUI_Edit_P->String+i,X,Y,0,FColor,BColor,GUI_Edit_P->Font);
					i++;
				}
			}
			else
			{
				return;
		
			}
	}	
}
/********************************************
������GUI_EditSeletColor
���ܣ�������˸�ı�ѡ���ı�����ɫ��־����ˢ�¿ؼ���ʾ��־
������ GUI_Edit_P EDIT��ַָ��
���أ���
˵����1��1��ѭ����ÿ0.5��ı�1����ɫ
********************************************/
void GUI_EditSeletColor(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->FocusLen > 0)		//������ѡ�����0 ����edit״̬�У�ÿ��0.5����˸��־������ʾ��ɱ�־
	{
		if((OS.OSTime%1000 >500)&&(GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE)))
		{
			if(GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE))
			{
				GUIResetBit(GUI_Edit_P->State,EDIT_COLORTYPE);
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//�ؼ���ʾ
			}
		}
		if((OS.OSTime%1000 <500)&&(!GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE)))
		{
			if(!GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE))
			{			
				GUISetBit(GUI_Edit_P->State,EDIT_COLORTYPE);
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//�ؼ���ʾ
			}
		}		
	}	
	
}

/********************************************
������EditShowNow
���ܣ�����ʾ������ʾEDIT���ı�
������ GUI_Edit_P EDIT��ַָ��
���أ���
˵����������û�иı�ֻ��ʾһ��
********************************************/
void GUI_EditShowNow(GUI_EditStruct *GUI_Edit_P)						   //
{
	if(GUI_Edit_P->ID != GUIEditID) 	return ;				//����Edit������
	GUI_EditSeletColor(GUI_Edit_P);			//��˸�ж�
	
	if(!GUIGetBit(GUI_Edit_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
	{
		if(GUI_Edit_P->Y>(GUI_H-20)) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���	
		if(GUI_Edit_P->X>(GUI_W-4-GUI_Edit_P->MaxLen*8)) return;//�����ʾ�Ŀ�ȴ���LCD��ȣ�ֱ�ӷ���	
	//��edit���		
		if(GUIGetBit(GUI_Edit_P->State,GUI_FOCUS))		//�Ƿ񽹵�ѡ��
		{
			GUI_DrawRectangle(GUI_Edit_P->X,GUI_Edit_P->Y,GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+3,GUI_Edit_P->Y+19,0xFFFF-GUI.B_Color);
		}
		else
		{
			GUI_DrawRectangle(GUI_Edit_P->X,GUI_Edit_P->Y,GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+3,GUI_Edit_P->Y+19,GUI_Black);			
		}
		GUI_DrawRectangle(GUI_Edit_P->X+1,GUI_Edit_P->Y+1,GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+2,GUI_Edit_P->Y+18,GUI_Black);		
	//��ʾEDIT����
		if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{	
			GUI_EditShowString(GUI_Edit_P);
		}
		GUISetBit(GUI_Edit_P->State,GUI_SHOWOK);			//�ؼ���ʾ���
	}
}

/********************************************
������GUI_EditUseKeyUp
���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
������ GUI_Edit_P Edit��ַָ��
���أ���
˵����
********************************************/
void GUI_EditUseKeyUp(GUI_EditStruct *GUI_Edit_P)
{
	if(GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))//�Ƿ���edit״̬
	{		//edit״̬
		switch(GUI_Edit_P->Mode)
		{
		case GUI_STRING://EDIT����ʾģʽΪ����
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>0x41)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<=0x5A))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]--;
			}
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x41)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]=0x5A;
			}
			break;
		case GUI_DEC://EDIT����ʾģʽΪ10��������
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>0x30)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<=0x39))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]--;
			}	
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x30)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]= 0x39;
			}			
			break;
		case GUI_HEX://EDIT����ʾģʽΪ16��������
			if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x30)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]= 0x46;
			}
			else if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>0x30)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<=0x39))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]--;
			}	
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x41)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]= 0x39;
			}
			else if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>0x41)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<=0x46))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]--;
			}
			break;
		}
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
		
	}
	else
	{										//ѡ��״̬
		if(GUI_Edit_P->FocusLen>1)
		{
			GUI_Edit_P->FocusLen--;
		}
		else
		{
			GUI_Edit_P->FocusLen = GUI_Edit_P->MaxLen;
		}
	}
}
/********************************************
������GUI_EditUseKeyDown
���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
������ GUI_Edit_P Edit��ַָ��
���أ���
˵����
********************************************/
void GUI_EditUseKeyDown(GUI_EditStruct *GUI_Edit_P)
{
	if(GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))//�Ƿ���edit״̬
	{		//edit״̬
		switch(GUI_Edit_P->Mode)
		{
		case GUI_STRING://EDIT����ʾģʽΪ����
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>=0x41)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<0x5A))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]++;
			}
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x5A)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]=0x41;
			}
			break;
		case GUI_DEC://EDIT����ʾģʽΪ10��������
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>=0x30)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<0x39))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]++;
			}	
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x39)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1] = 0x30;
			}
			break;
		case GUI_HEX://EDIT����ʾģʽΪ16��������
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>=0x30)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<0x39))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]++;
			}	
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x39)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1] = 0x41;
			}
			else if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>=0x41)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<0x46))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]++;
			}
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x46)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1] = 0x30;
			}
			break;
		}		


	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
		
	}
	else
	{//ѡ��״̬
		if(GUI_Edit_P->FocusLen<GUI_Edit_P->MaxLen)
		{
			GUI_Edit_P->FocusLen++;
		}
		else
		{
			GUI_Edit_P->FocusLen = 1;
		}		
	}   
}
/********************************************
������GUI_EditUseKeyEnter
���ܣ��˵��ɼ��̿���ʱ��ȷ�ϰ������¹���
������ GUI_Edit_P Edit��ַָ��
���أ���
˵����
********************************************/
void GUI_EditUseKeyEnter(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->FocusLen  == 0)			//ֻ�ǽ���ѡ����EDIT����û����༭ģʽ
	{
		GUI_Edit_P->FocusLen = GUI_Edit_P->MaxLen;
		GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 0 ����ѡ��״̬		
	}
	else 
	{
		if(!GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))
		{//ѡ��ģʽ
			GUISetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 1 ����༭ģʽ
		}
		else
		{//�༭ģʽ���������ݱ���
					GUI_Edit_P->FocusLen --;
			if(GUI_Edit_P->FocusLen ==0)
			{
				GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 0 ����ѡ��״̬
				GUI.CurrentForm->FocusIsSelect =GUI_false;
			}
	//	
			//save data
		}
	}	
		GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
}
/********************************************
������GUI_EditEseKeyEsc
���ܣ��˵��ɼ��̿���ʱ��ȡ���������¹���
������ GUI_Edit_P Edit��ַָ��
���أ���
˵����
********************************************/
void GUI_EditUseKeyEsc(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->FocusLen  > 0)			//ֻ�ǽ���ѡ����EDIT����û����༭ģʽ
	{
		if(!GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))
		{//ѡ��ģʽ
			GUI_Edit_P->FocusLen = 0;
			GUI.CurrentForm->FocusIsSelect =GUI_false;
		}
		else
		{//�༭ģʽ���������ݱ���
			GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 0 ����ѡ��ģʽ
		}
	}	
		GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
}

void GUI_EditKeyFirst(GUI_EditStruct *GUI_Edit_P)
{
	if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
	{
		switch(GUI.KeyNum)
		{
		case GUIKEY_UP://����
			GUI_EditUseKeyUp(GUI_Edit_P);
			break;		
		case GUIKEY_DOWN://����
			GUI_EditUseKeyDown(GUI_Edit_P);
			break;				
		case GUIKEY_ENTER://Enter��
			GUI_EditUseKeyEnter(GUI_Edit_P);							
			break;	
		case GUIKEY_ESC://���ؼ�
			GUI_EditUseKeyEsc(GUI_Edit_P);	
			break;

		default:
			break;
		}	
		
	}
}

void GUI_EditKeyLong(GUI_EditStruct *GUI_Edit_P)
{

}

void GUI_EditKeyOff(GUI_EditStruct *GUI_Edit_P)
{

}


void GUI_EditLostFocus(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->ID != GUIEditID) 	return ;				//����Edit������		
	GUIResetBit(GUI_Edit_P->State,GUI_FOCUS);						//�ؼ�ȡ������
	GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);						//�ؼ�ȡ���ڲ�����	
	GUI_Edit_P->FocusLen = 0;														//�ؼ�ȡ���ڲ�ѡ��
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);					//�ؼ���ʾ�б仯		
}

void GUI_EditGetFocus(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->ID != GUIEditID) 	return ;				//����Edit������		
	GUISetBit(GUI_Edit_P->State,GUI_FOCUS);						//�ؼ�ȡ������
	GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);						//�ؼ�ȡ���ڲ�����	
	GUI_Edit_P->FocusLen = 0;														//�ؼ�ȡ���ڲ�ѡ��
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);					//�ؼ���ʾ�б仯				
}
/********************************************
������GUI_EditTouchUp
���ܣ������������ͷŵ�ʱ������Edit��״̬
������ GUI_Edit_P: Edit��ַָ�룻
���أ���
˵����
********************************************/
void GUI_EditTouchUp(GUI_EditStruct *GUI_Edit_P)
{
	if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))							//����DIS״̬
	{
//		GUI_EditUseKeyEsc(GUI_Edit_P);
		GUI.CurrentForm->TouchNum = 0;	
	}
}
/********************************************
������GUI_EditTouchDown
���ܣ��������������µ�ʱ������Edit��״̬
������ GUI_Edit_P: Edit��ַָ�룻Num���ڵ�ǰ��FORM�еı��
���أ���
˵����
********************************************/
u8 GUI_EditTouchDown(GUI_EditStruct *GUI_Edit_P,u8 Num)
{
	u16 X1,Y1,X2,Y2;
	X1 = GUI_Edit_P->X;
	X2 = GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+2;	
	Y1 = GUI_Edit_P->Y;
	Y2 = GUI_Edit_P->Y+18;
	if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))										//��ť����DIS״̬		
		{													
			if((GUI.Touch_X >X1)&&(GUI.Touch_Y>Y1)&&(GUI.Touch_X<X2)&&(GUI.Touch_Y<Y2))		 //�ж��Ƿ��ڰ�ť�ڲ�
			{	
				GUI_EditUseKeyEnter(GUI_Edit_P);
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//�ؼ����ػ�
//				GUI.CurrentForm->FocusID =  Num+1;
//				GUI.CurrentForm->FocusID =  GUI_CurrenFormGetIDNum((u8*)(GUI_Edit_P));				
				GUI.CurrentForm->TouchNum = Num;
				GUI.CurrentForm->FocusIsSelect = GUI_true;
//				GUI.TouchChange = TOUCHNULL;
				GUI_FocusSelectChang(Num+1);		//�������
			//����Edit�İ�������
		
				
			return 1;
			}
		}
		return 0;
}

