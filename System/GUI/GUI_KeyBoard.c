/*********************************************************************************
* �ļ���			: KeyBoard.c
* �汾��    	: V2.1.0
* ����      : 2015/09/29
* ˵��      : GUI����ģ�����								  
* �÷�			��

**********************************************************************************/
#include "malloc.h"	
#include "GUI_KeyBoard.h" 
#include "GUI_CONF.h"
#include "OS.h"
#define	  	D_KeyBoard_BCOLOR 		GUI_Red
#define	  	D_KeyBoard_FCOLOR 		GUI_Grey
#define	  	D_KeyBoard_SELECTCOLOR 		GUI_Blue

#define			D_KeyBoard_W				40		//���ܼ��Ŀ��Ϊ40
#define			D_KeyBoard_H				20		//���ܼ��ĸ߶�Ϊ20
#define 		NOSELECT						0xFF


void GUI_KeyBoardShowInit(GUI_KeyBoardStruct *KeyBoard_P)						   //
{
	u16 i,j;	
	u16 X1,X2,Y1,Y2;	
	X1 = KeyBoard_P->X;
	X2 = KeyBoard_P->W * KeyBoard_P->X_Num;
	Y1 = KeyBoard_P->Y;
	Y2 = KeyBoard_P->H * KeyBoard_P->Y_Num;
	
	GUI_DrawFillRectangle(X1, Y1,X1+X2,Y1+Y2,KeyBoard_P->BColor);			//KeyBoard��ʾ�������
	GUI_DrawHLine(X1,Y1,X2,KeyBoard_P->FColor);
	GUI_DrawVLine(X1+X2,Y1,Y2,KeyBoard_P->FColor);	
	for (i=0;i<KeyBoard_P->Y_Num;i++)
	{	
		Y1+=KeyBoard_P->H;
		GUI_DrawHLine(X1,Y1,X2,KeyBoard_P->FColor);			
	}	
	Y1= KeyBoard_P->Y;
	for (j=0;j<KeyBoard_P->X_Num;j++)		
		{
			GUI_DrawVLine(X1, Y1 , Y2,KeyBoard_P->FColor);					
			X1 += KeyBoard_P->W;		
		}	
	GUISetBit(KeyBoard_P->State,KeyBoard_INITOK);		
}
/********************************************
������KeyBoard_Create
���ܣ�����KeyBoard
������ Form_P����ǰָ��, X,Y :����,W��H�����������Ŀ�Ⱥ͸߶ȣ� Mode������
���أ���ť���ڴ��еĵ�ַָ�� ������򷵻�0
********************************************/
GUI_KeyBoardStruct *GUI_KeyBoardCreate(GUI_KeyBoardInitStruct *KeyBoardInit_P)
{
	GUI_KeyBoardStruct *KeyBoard_P;	
	KeyBoard_P=(GUI_KeyBoardStruct *)GUI_MEM_Malloc(sizeof(GUI_KeyBoardStruct));	//����KeyBoard_MEMNUM���ֽڵ��ڴ�,ǰ16��Ϊϵͳ���ݣ�����Ϊ��ʾ����
	if(KeyBoard_P)
	{
		if(GUI_FormAddChild(KeyBoardInit_P->Form_P,(u8*)KeyBoard_P))
		{
			GUI_MEM_Free(KeyBoard_P);	//�ͷ��ڴ�
			return 0;
		}		
		KeyBoard_P->X = KeyBoardInit_P->X;										//X������
		KeyBoard_P->Y =KeyBoardInit_P->Y;											//Y������
		KeyBoard_P->W = KeyBoardInit_P->W;										//�������Ŀ��
		KeyBoard_P->H =KeyBoardInit_P->H;											//�������ĸ߶�
		KeyBoard_P->Font = &Font8x12;													//����ʾ�����֣�����ʹ��Ĭ�ϵģ���Ҫ���ĵ�����������
		KeyBoard_P->ID = GUIKeyBoardID;												//KeyBoard ��ID��ţ�Ĭ�ϲ����޸�
		KeyBoard_P->SelectNum = NOSELECT;											//��ֵ
		KeyBoard_P->BColor = D_KeyBoard_BCOLOR;								//KeyBoard �ı���ɫ������ʹ��Ĭ�ϵģ���Ҫ���ĵ�����������
		KeyBoard_P->FColor = D_KeyBoard_FCOLOR;								//KeyBoard ��������ɫ������ʹ��Ĭ�ϵģ���Ҫ���ĵ�����������
		KeyBoard_P->SelectColor = D_KeyBoard_SELECTCOLOR;			//KeyBoard �ı�ѡ��ʱ����ɫ������ʹ��Ĭ�ϵģ���Ҫ���ĵ�����������
		KeyBoard_P->X_Num = KeyBoardInit_P->X_Num;						//KeyBoard X�������
		KeyBoard_P->Y_Num = KeyBoardInit_P->Y_Num;						//KeyBoard Y�������
		KeyBoard_P->Char_X = KeyBoardInit_P->Char_X;					//ÿ���������ֶ�Ӧ��X��ƫ����	
		KeyBoard_P->Char_Y = KeyBoardInit_P->Char_Y;					//ÿ���������ֶ�Ӧ��Y��ƫ����				
		KeyBoard_P->StringLen = KeyBoardInit_P->StringLen;		//ÿ���������ֳ���
		KeyBoard_P->StringMode = KeyBoardInit_P->StringMode;	//��ʾ���������ͣ�����֧��string��dec��hex
		KeyBoard_P->keyString = KeyBoardInit_P->keyString;		//��������ָ��
		KeyBoard_P->KeyID = KeyBoardInit_P->KeyID & 0xe0;			//��ֵ��ID��ʾ��ֻʹ�ø�3λ��
		KeyBoard_P->State = 0;																//״̬��ʼ��
		KeyBoard_P->OsPriority = OsCurrentPriority;
		GUI_KeyBoardShowInit(KeyBoard_P);											//��ʾ��ʼ��	
		return KeyBoard_P;			
	}
	else
	{
	return GUI_null;		
	}
	
}

/********************************************
������KeyBoardShowString
���ܣ�����ʾ������ʾKeyBoard���ı�
������  KeyBoard_P����ǰָ��, X1,Y1 :�������������ꣻ Num:KeyBoardѡ��İ������
���أ���
˵����
********************************************/
void KeyBoardShowString(GUI_KeyBoardStruct *KeyBoard_P,u16 X1,u16 Y1,u8 Num)	
{

	u16 X2,Y;
	X2 = X1+KeyBoard_P->W-1;
	Y= Y1+(KeyBoard_P->H - KeyBoard_P->Font->Y-1)/2+1;		
//	Y2 = Y+KeyBoard_P->Font->Y-1;
//	GUI_DrawFillRectangle(X1+1, Y1+1,X2,Y,KeyBoard_P->BColor);
//	GUI_DrawFillRectangle(X1+1, Y2,X2,Y1+KeyBoard_P->H-1,KeyBoard_P->BColor);	


//	GUI_ShowString((u8*)(&(KeyBoard_P->keyString[Num*KeyBoard_P->StringLen])),KeyBoard_P->Font,X1+1,Y+1,X2,Middle,KeyBoard_P->FColor,KeyBoard_P->BColor);
}
void KeyBoardShowValue(GUI_KeyBoardStruct *KeyBoard_P,u16 X1,u16 Y1,u8 Num)	
{

	u16 X2,Y2;
	
	u8 String[10];
	u8 i,m;	
	u32 Value = 0;
	u8 Len;
	Len = KeyBoard_P->StringLen&0x0F;
	m = KeyBoard_P->StringLen >> 4;
	switch (m)
	{
		case 1:				//u8
//			UintToByte0(Value) = KeyBoard_P->keyString[Num*m];
//			break;
//		case 2:				//u16
//			UintToByte0(Value) = KeyBoard_P->keyString[Num*m];		
//			UintToByte1(Value) = KeyBoard_P->keyString[Num*m+1];		
//			break;
//		case 4:				//u32
//			UintToByte0(Value) = KeyBoard_P->keyString[Num*m];		
//			UintToByte1(Value) = KeyBoard_P->keyString[Num*m+1];			
//			UintToByte2(Value) = KeyBoard_P->keyString[Num*m+2];		
//			UintToByte3(Value) = KeyBoard_P->keyString[Num*m+3];		
		
			break;
		default:
			return;
//			break;
	}	
	if(KeyBoard_P->StringMode == GUI_DEC)				//DEC
	{
		DecToString(Value,String,10);
		
	}
	else if(KeyBoard_P->StringMode == GUI_HEX)		//HEX
	{
		HexToString(Value,String,10);
		
	}
	else
	{
		return;
	}
//	
//	GUI_DrawFillRectangle(X1+1+KeyBoard_P->Char_X, Y1+1,X1+KeyBoard_P->W-1,Y1+KeyBoard_P->H-1,KeyBoard_P->BColor);
X2 = X1+1+KeyBoard_P->Char_X;
Y2 = Y1+1+KeyBoard_P->Char_Y;	
	for(i=0;i<Len;i++)
	{
		if(KeyBoard_P->SelectNum == Num)
		{
			GUI_PutChar(&String[10-Len+i], X2, Y2, KeyBoard_P->SelectColor, KeyBoard_P->BColor,KeyBoard_P->Font);		
		}
		else
		{
			GUI_PutChar(&String[10-Len+i], X2, Y2, KeyBoard_P->FColor, KeyBoard_P->BColor,KeyBoard_P->Font);					
		}
		X2+= KeyBoard_P->Font->ASC_X;
	}


}



/********************************************
������GUI_KeyBoardShowAll
���ܣ�����ʾ������ʾKeyBoard����������
������  KeyBoard_P����ǰָ��
���أ���
˵����
********************************************/
void GUI_KeyBoardShowAll(GUI_KeyBoardStruct *KeyBoard_P)						   //
{
	u16 i,j;	
	u16 X,Y;	
	u8 Num;	
	Y=KeyBoard_P->Y;	
	for (i=0;i<KeyBoard_P->Y_Num;i++)
	{	
		X=KeyBoard_P->X;		
		for (j=0;j<KeyBoard_P->X_Num;j++)		
		{
			Num = j + KeyBoard_P->X_Num * i;		//���㵱ǰ��ʾ���ǵڼ���	
			if(KeyBoard_P->StringMode == GUI_STRING)
			{	
				KeyBoardShowString(KeyBoard_P,X,Y,Num);	
			}
			else
			{
				KeyBoardShowValue(KeyBoard_P,X,Y,Num);
			}
			X += KeyBoard_P->W;		
		}	
		Y+=KeyBoard_P->H;
	}					
}


/********************************************
������KeyBoardShowNow
���ܣ�����ʾ������ʾKeyBoard
������ KeyBoard_P KeyBoard��ַָ��
���أ���
˵����������û�иı�ֻ��ʾһ��
********************************************/
void GUI_KeyBoardShowNow(GUI_KeyBoardStruct *KeyBoard_P)						   //
{
	if(KeyBoard_P->ID != GUIKeyBoardID) 	return ;				//����KeyBoard������
	if(!GUIGetBit(KeyBoard_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
	{
		if(GUIGetBit(KeyBoard_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{	
			//ShowDIS(KeyBoard_P);
		}
		else								//��DISABLE״̬
		{					
//			if(!GUIGetBit(KeyBoard_P->State,KeyBoard_SHOWALL))			//��Ҫȫ����ʾ
//			{		
//				GUI_KeyBoardShowInit(KeyBoard_P);				
//				GUI_KeyBoardShowAll(KeyBoard_P);
//				GUISetBit(KeyBoard_P->State,KeyBoard_SHOWALL);
//			}
//			else																										//����
//			{
				GUI_KeyBoardShowAll(KeyBoard_P);				
//			}	
		}
	}
		GUISetBit(KeyBoard_P->State,GUI_SHOWOK);			//�ؼ���ʾ���	
}

/********************************************
������KeyBoardTouchUp
���ܣ��˵��ɴ���������ʱ���������ͷ�ʱ�Ĺ���
������ KeyBoard_P KeyBoard��ַָ��
���أ���
˵����
********************************************/
u8 GUI_KeyBoardTouchUp(GUI_KeyBoardStruct *KeyBoard_P)
{
	u32 KeyMessage;
	if(KeyBoard_P->ID != GUIKeyBoardID)		return GUI_false;	
	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))		//����DISABLE
	{
		if(GUIGetBit(KeyBoard_P->State,KeyBoard_TOUCH))		//����������
		{	
			KeyMessage = ((u32)KeyMessageType<<24) | ((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| GUIKEY_UP<<8);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(KeyBoard_P->OsPriority, (void *)(KeyMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
			#else
				//��OS��ʱ�����еĳ���
			#endif			
//			PostKeyMessageQueue((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| KEYOFF<<8);	//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
			GUIResetBit(KeyBoard_P->State,KeyBoard_TOUCH);					//������ܼ����±�־
			GUIResetBit(KeyBoard_P->State,KeyBoard_SHOWON);						//��������ʾON״̬			
			KeyBoard_P->SelectNum = NOSELECT;															//��������µļ����
			GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);							//�ؼ���ʾ�б仯
			return GUI_true;			
		}		
	}	
	return GUI_false;
}

/********************************************
������GUI_KeyBoardTouchLong
���ܣ��˵��ɴ���������ʱ����������ʱ�䰴��ʱ�Ĺ���
������ KeyBoard_P KeyBoard��ַָ��
���أ���
˵����
********************************************/
u8 GUI_KeyBoardTouchLong(GUI_KeyBoardStruct *KeyBoard_P)
{
	u32 KeyMessage;
	if(KeyBoard_P->ID != GUIKeyBoardID)		return GUI_false;	
	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))		//����DISABLE
	{
		if(GUIGetBit(KeyBoard_P->State,KeyBoard_TOUCH))		//����������
		{
			KeyMessage = ((u32)KeyMessageType<<24) | ((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| GUIKEY_ENTER<<8);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(KeyBoard_P->OsPriority, (void *)(KeyMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
			#else
				//��OS��ʱ�����еĳ���
			#endif					
//			PostKeyMessageQueue((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)|KEYLONG<<8);	//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
			return GUI_true;	
		}
	}
	return GUI_false;	
	
}


/********************************************
������GUI_KeyBoardTouchDown
���ܣ������ɴ���������ʱ������������ʱ�Ĺ���
������ KeyBoard_P ��ַָ�룻Num:������FORM�еı��
���أ�false	�����ڼ��̷�Χ�ڵĴ�����true �������ڼ��̷�Χ��
˵����
********************************************/
u8 GUI_KeyBoardTouchDown(GUI_KeyBoardStruct *KeyBoard_P,u8 Num)
{
	u32 KeyMessage;
	u16 i,j;
	u16 T_X,T_Y,X,Y;
	if(KeyBoard_P->ID != GUIKeyBoardID)		return GUI_false;	
	T_X = GUI.Touch_X;
	T_Y = GUI.Touch_Y;	
	X = KeyBoard_P->X + KeyBoard_P->W * KeyBoard_P->X_Num;
	Y = KeyBoard_P->Y + KeyBoard_P->H * KeyBoard_P->Y_Num; 

	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{										//����DIS״̬			
			if((T_X>KeyBoard_P->X)&&(T_X<X) && (T_Y>KeyBoard_P->Y)&&(T_Y<Y)) //�ж��Ƿ��ڰ�ť�ڲ�
			{	
				Y= KeyBoard_P->Y;
				for (i=0;i<KeyBoard_P->Y_Num;i++)
				{
					Y += KeyBoard_P->H;
					if(T_Y < Y)
					{
						X= KeyBoard_P->X;		
						for (j=0;j<KeyBoard_P->X_Num;j++)	
						{
							X += KeyBoard_P->W;					
							if(T_X < X)
							{						
								KeyBoard_P->SelectNum = i*KeyBoard_P->X_Num +j;					//���汻���µļ����
								
								KeyMessage = ((u32)KeyMessageType<<24) | ((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| GUIKEY_DOWN<<8);
								#if GUI_USEMSOS
								
									OS.PostMessageQueue(KeyBoard_P->OsPriority, (void *)(KeyMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
								#else
									//��OS��ʱ�����еĳ���
								#endif	
								
								
//								PostKeyMessageQueue((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| KEYFIRST<<8);			//���͵�һ�ΰ��¹��ܼ��ļ�ֵ	

								
								GUISetBit(KeyBoard_P->State,KeyBoard_TOUCH);						//���ܼ����±�־
								GUISetBit(KeyBoard_P->State,KeyBoard_SHOWON);						//��������ʾON״̬
								GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);							//�ؼ���ʾ�б仯
								GUI.CurrentForm->TouchNum = Num;
								return GUI_true;									
							}		
						}	
					}
				}		
			}
		}
		return GUI_false;
}


/*******************************���ܰ���ֻ֧��touch����֧��Ӳ��key*************************************************/





//void KeyBoardKeyOff(GUI_KeyBoardStruct *KeyBoard_P)
//{


//}

//void KeyBoardKeyLong(GUI_KeyBoardStruct *KeyBoard_P)
//{


//}
//void KeyBoardLostFocus(void)
//{
//	if(GUI.KeyBoard_P == null)			return ;
//	if(GUI.KeyBoard_P->ID != GUIKeyBoardID) 	return ;				//����KeyBoard������
//	GUIResetBit(GUI.KeyBoard_P->State,GUI_FOCUS);									//�ؼ�ȡ��ϵͳ����
//	GUIResetBit(GUI.KeyBoard_P->State,KeyBoard_FOCUSIN);						//�ؼ�ȡ���ڲ�����	
//	GUI.KeyBoard_P->SelectNum =0;																	//���ܼ�ѡ�����
//	GUIResetBit(GUI.KeyBoard_P->State,GUI_SHOWOK);									//�ؼ���ʾ�б仯			
//}

//void KeyBoardGetFocus(void)
//{
//	if(GUI.KeyBoard_P == null)			return ;
//	if(GUI.KeyBoard_P->ID != GUIKeyBoardID) 	return ;				//����KeyBoard������
//	GUISetBit(GUI.KeyBoard_P->State,GUI_FOCUS);										//�ؼ���ȡϵͳ����
//	GUIResetBit(GUI.KeyBoard_P->State,KeyBoard_FOCUSIN);						//�ؼ�ȡ���ڲ�����	
//	GUI.KeyBoard_P->SelectNum =0;																	//���ܼ�ѡ�����
//	GUIResetBit(GUI.KeyBoard_P->State,GUI_SHOWOK);									//�ؼ���ʾ�б仯			
//}

/********************************************
������KeyBoardUseKeyUp
���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
������ KeyBoard_P KeyBoard��ַָ��
���أ���
˵����
********************************************/
void KeyBoardUseKeyUp(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(KeyBoard_P->SelectNum!= 0)//���˵�һ��
	{
		KeyBoard_P->SelectNum--;		
	}
	else
	{
		KeyBoard_P->SelectNum = KeyBoard_P->X_Num*KeyBoard_P->Y_Num;
	}
	GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯		
}
/********************************************
������KeyBoardUseKeyDown
���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
������ KeyBoard_P KeyBoard��ַָ��
���أ���
˵����
********************************************/
void KeyBoardUseKeyDown(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(KeyBoard_P->SelectNum!= KeyBoard_P->X_Num*KeyBoard_P->Y_Num)//���˵�һ��
	{
		KeyBoard_P->SelectNum++;		
	}
	else
	{
		KeyBoard_P->SelectNum = 0;
	}
	GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯		 
}
/********************************************
������KeyBoardUseKeyEnter
���ܣ��˵��ɼ��̿���ʱ��ȷ�ϰ������¹���
������ KeyBoard_P KeyBoard��ַָ��
���أ���
˵����
********************************************/
void KeyBoardUseKeyEnter(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(KeyBoard_P->SelectNum == 0)				//��ǰѡ��Ϊ���ز˵���ʱ����back��
	{
		KeyBoard_P->SelectNum = 1;
	}
	else
	{
//		if(KeyBoard_P->RegisterFunction[KeyBoard_P->SelectNum-1] == NULL) return ;
//		(*(KeyBoard_P->RegisterFunction[KeyBoard_P->SelectNum-1]))(); 
	}
	GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯		 
}



void KeyBoardKeyFirst(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
	{
		switch(GUI.KeyNum)
		{
		case GUIKEY_UP://����
			KeyBoardUseKeyUp(KeyBoard_P);
			break;		
		case GUIKEY_DOWN://����
			KeyBoardUseKeyDown(KeyBoard_P);
			break;				
		case GUIKEY_ENTER://Enter��
			KeyBoardUseKeyEnter(KeyBoard_P);							
			break;	
		case GUIKEY_ESC://���ؼ�
			GUI.CurrentForm->FocusIsSelect =GUI_false;			
		break;

		default:
		 break;
		}	
	}
}

























//void KeyBoardShowString1(GUI_KeyBoardStruct *KeyBoard_P,u16 X1,u16 Y1,u8 X_NUM,u8 Y_NUM)	
//{
//	u16 BColor;
//	u16 X2,Y,Y2;
//	u8 Num;
//	Num = X_NUM+Y_NUM*KeyBoard_P->Y_Num;
//	if(KeyBoard_P->Font->Y > KeyBoard_P->H-1) return;	
//	BColor = KeyBoard_P->BColor;			
//	if(KeyBoard_P->SelectNum == Num)
//	{	
//		KeyBoard_P->BColor = KeyBoard_P->SelectColor;
//	}

//	X2 = X1+KeyBoard_P->W-1;

//	Y= Y1+(KeyBoard_P->H - KeyBoard_P->Font->Y-1)/2+1;		
//	
//	Y2 = Y+KeyBoard_P->Font->Y-1;
//	
//	GUI_DrawFillRectangle(X1+1, Y1+1,X2,Y,KeyBoard_P->BColor);
//	GUI_DrawFillRectangle(X1+1, Y2,X2,Y1+KeyBoard_P->H-1,KeyBoard_P->BColor);	
//	
////	
////	
//	GUI_ShowString(	KeyBoard_P,(u8*)(&(KeyBoard_P->keyString[Num])),X1+1,Y+1,X2,Middle);

//	KeyBoard_P->BColor = BColor;
//}










//void GUI_KeyBoardShowFirst(GUI_KeyBoardStruct *KeyBoard_P)						   //
//{
//	u16 i,j;	
//	u16 X,Y;
//	GUI_DrawHLine(KeyBoard_P->X,KeyBoard_P->Y,KeyBoard_P->W*KeyBoard_P->X_Num,KeyBoard_P->FColor);
//	GUI_DrawVLine(KeyBoard_P->X+KeyBoard_P->W*KeyBoard_P->X_Num,KeyBoard_P->Y,KeyBoard_P->H*KeyBoard_P->Y_Num,KeyBoard_P->FColor);	


//	Y=KeyBoard_P->Y;	

//	for (i=0;i<KeyBoard_P->Y_Num;i++)
//	{	
//	X=KeyBoard_P->X;		
//		for (j=0;j<KeyBoard_P->X_Num;j++)		
//		{
//			if(i == 0)
//			{
//				GUI_DrawVLine(X, KeyBoard_P->Y , KeyBoard_P->H*KeyBoard_P->Y_Num,KeyBoard_P->FColor);					
//			}	
//			

//			KeyBoardShowString(KeyBoard_P,X,Y,j,i);					
//			X += KeyBoard_P->W;			
//	
//		}	
//		Y+=KeyBoard_P->H;
//		GUI_DrawHLine(KeyBoard_P->X,Y,KeyBoard_P->W*KeyBoard_P->X_Num,KeyBoard_P->FColor);	

//		
//	}		
//	
//	
//	
//}






