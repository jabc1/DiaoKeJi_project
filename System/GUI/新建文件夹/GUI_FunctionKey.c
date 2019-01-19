/*********************************************************************************
* �ļ���			: Funckey.c
* �汾��    	: V2.1.0
* ����      : 2015/09/29
* ˵��      : GUI����ģ�����								  
* �÷�			��

**********************************************************************************/

#include "GUI_FUNCTIONKEY.h" 
#include "GUI.h"
#include "GUI_config.h"
#if GUI_USEMSOS
#include "os.h"

#else

#endif


#define	  	D_Funckey_BCOLOR 		Blue
#define	  	D_Funckey_FCOLOR 		Grey
#define	  	D_Funckey_SELECTCOLOR 		Red

#define			D_Funckey_W				40		//���ܼ��Ŀ��Ϊ40
#define			D_Funckey_H				18		//���ܼ��ĸ߶�Ϊ20



FunckeyStringStruct D_FunckeyString[] = {"��ҳ","��","��","��","��","HC1","HC2","��ҳ"};
//static const u8 keyID[] = {GUIKEY_0,GUIKEY_1,GUIKEY_2,GUIKEY_3,GUIKEY_UP,GUIKEY_DOWN,GUIKEY_ENTER,GUIKEY_ESC}; 
/********************************************
������SYSMenu_Create
���ܣ�����SYSMenu
������ Mode����ǰ�˵�ָ��
���أ���ť���ڴ��еĵ�ַָ�� ������򷵻�0
********************************************/
FunckeyStruct *GUI_FunckeyCreate(void)
{

	FunckeyStruct *Funckey_P;	
	Funckey_P=(FunckeyStruct *)GUI_MEM_Malloc(sizeof(FunckeyStruct));	//����SYSMenu_MEMNUM���ֽڵ��ڴ�,ǰ16��Ϊϵͳ���ݣ�����Ϊ��ʾ����
	if(Funckey_P)
	{
		Funckey_P->X = 0;
		Funckey_P->Y =GUI_H - D_Funckey_H;
		Funckey_P->ID = GUIFunckeyID;
		Funckey_P->Font = &Font8x12;
		Funckey_P->BColor = D_Funckey_BCOLOR;
		Funckey_P->FColor = D_Funckey_FCOLOR;
		Funckey_P->SelectColor = D_Funckey_SELECTCOLOR;	
		#if GUI_USEMSOS
			Funckey_P->OsPriority = OsCurrentPriority;			//�洢��ǰ����FUNCKEY���߳�OS���ȼ�
		#endif	
		Funckey_P->String = &D_FunckeyString[0];
		Funckey_P->State = 0;		
		if(GUI_W == 320)		//��Ļ�Ŀ����320
		{
			Funckey_P->KeyNum = 8;			
		}
		else if(GUI_W == 240)		
		{
			Funckey_P->KeyNum = 6;			
		}
		else
		{
			GUI_MEM_Free(Funckey_P);	//�ͷ��ڴ�			
			return null;
		}
			GUI_API.FuncKey_P = (u8 *)Funckey_P;
			return Funckey_P;		
				
	}
	else
	{
		return null;
	}
	
}




/********************************************
������FunckeyShowString
���ܣ�����ʾ������ʾFunckey���ı�
������ Num:Funckeyѡ��İ������
���أ���
˵����
********************************************/
void FunckeyShowString(u8 Num)	
{
	u16 BColor;
	u16 X1,X2,Y1,Y2;
	FunckeyStruct *Funckey_P;	
	Funckey_P = (FunckeyStruct *)GUI_API.FuncKey_P;
		BColor = Funckey_P->BColor;			
	if(Funckey_P->SelectNum == Num+1)
	{
	
		Funckey_P->BColor = Funckey_P->SelectColor;
	}
	X1 = Funckey_P->X+Num*D_Funckey_W+1;
	X2 = Funckey_P->X+(Num+1)*D_Funckey_W-1;
	if(Funckey_P->Font->Y > D_Funckey_H-2) return;
	Y1= Funckey_P->Y+(D_Funckey_H - Funckey_P->Font->Y-2)/2+1;		
	Y2 = Y1+Funckey_P->Font->Y-1;
	GUI_DrawFillRectangle(X1, Funckey_P->Y,X2,Y1,Funckey_P->BColor);
	GUI_DrawFillRectangle(X1, Y2,X2,Funckey_P->Y+D_Funckey_H-1,Funckey_P->BColor);	
	GUI_ShowString((u8*)(&(D_FunckeyString[Num])),Funckey_P->Font,X1,Y1,X2,Middle,Funckey_P->FColor,Funckey_P->BColor);

	Funckey_P->BColor = BColor;

	
	

}


/********************************************
������GUI_FunckeyShowNow
���ܣ�����ʾ������ʾGUI_Funckey
������ Funckey_P ��ַָ��
���أ���
˵����������û�иı�ֻ��ʾһ��
********************************************/
void GUI_FunckeyShowNow(void)						   //
{
	u16 i;
	FunckeyStruct *Funckey_P;	
	Funckey_P = (FunckeyStruct *)GUI_API.FuncKey_P;
	if(Funckey_P == null)			return ;
	if(Funckey_P->ID != GUIFunckeyID) 	return ;				//����Funckey������
	if(!GUIGetBit(Funckey_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
	{
		if(GUIGetBit(Funckey_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{	
			//ShowDIS(Funckey_P);
		}
		else
		{		
			for (i=0;i<Funckey_P->KeyNum;i++)
			{
				FunckeyShowString(i);
				if(i>0)
				{
					GUI_DrawVLine(Funckey_P->X+i*D_Funckey_W,Funckey_P->Y,D_Funckey_H,Funckey_P->FColor);					
				}
			}	
		}
			GUI_DrawHLine(Funckey_P->X,Funckey_P->Y,GUI_W-1,Funckey_P->FColor);
			GUI_DrawHLine(Funckey_P->X,Funckey_P->Y+D_Funckey_H-1,GUI_W-1,Funckey_P->FColor);	
			GUI_DrawVLine(Funckey_P->X,Funckey_P->Y,D_Funckey_H,Funckey_P->FColor);	
			GUI_DrawVLine(Funckey_P->X+GUI_W-1,Funckey_P->Y,D_Funckey_H,Funckey_P->FColor);	
		
		
	}
		GUISetBit(Funckey_P->State,GUI_SHOWOK);			//�ؼ���ʾ���	

}

/********************************************
������FunckeyTouchUp
���ܣ��˵��ɴ���������ʱ���������ͷ�ʱ�Ĺ���
������ Funckey_P SYSMenu��ַָ��
���أ���
˵����
********************************************/
u8 GUI_FunckeyTouchUp(void)
{
	FunckeyStruct *Funckey_P;	
	u32 KeyMessage;
	Funckey_P = (FunckeyStruct *)GUI_API.FuncKey_P;	
	if(Funckey_P == null)		return false;
	if(!GUIGetBit(Funckey_P->State,GUI_DIS))		//����DISABLE
	{
		if(GUIGetBit(Funckey_P->State,FUNCKEY_TOUCH))		//����������
		{			
			KeyMessage = ((u32)KeyMessageType<<24) | (KEYOFF<<8) |(Funckey_P->SelectNum | GUI_FUNCKEY_KEY);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(Funckey_P->OsPriority, (void *)(KeyMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
			#else
				//��OS��ʱ�����еĳ���
			#endif				
			GUIResetBit(Funckey_P->State,FUNCKEY_TOUCH);					//������ܼ����±�־
			Funckey_P->SelectNum = 0;															//��������µļ����
			GUIResetBit(Funckey_P->State,GUI_SHOWOK);							//�ؼ���ʾ�б仯
			return true;			
		}		
	}	
	return false;
}
/*******************************************************************/
u8 GUI_FunckeyTouchLong(void)
{
	FunckeyStruct *Funckey_P;	
	u32 KeyMessage;
	Funckey_P = (FunckeyStruct *)GUI_API.FuncKey_P;
	if(Funckey_P == null)		return false;
	if(!GUIGetBit(Funckey_P->State,GUI_DIS))		//����DISABLE
	{
		if(GUIGetBit(Funckey_P->State,FUNCKEY_TOUCH))		//����������
		{
			KeyMessage = ((u32)KeyMessageType<<24) | (KEYLONG<<8) |(Funckey_P->SelectNum | GUI_FUNCKEY_KEY);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(Funckey_P->OsPriority, (void *)(KeyMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
			#else
				//��OS��ʱ�����еĳ���
			#endif			
			return true;	
		}
	}
	return false;	
	
}



u8 GUI_FunckeyTouchDown(void)
{
	u16 i;
	FunckeyStruct *Funckey_P;	
	u32 KeyMessage;
	Funckey_P = (FunckeyStruct *)GUI_API.FuncKey_P;
	if(Funckey_P == null)		return false;
	if(!GUIGetBit(Funckey_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{										//����DIS״̬			
			if(GUI_API.Touch_Y>Funckey_P->Y)	 //�ж��Ƿ��ڰ�ť�ڲ�
			{	
//				X= Funckey_P->X+16;
				for (i=0;i<Funckey_P->KeyNum;i++)
				{
					if((GUI_API.Touch_X>i*D_Funckey_W)&&(GUI_API.Touch_X<((i+1)*D_Funckey_W)))		 
					{
						GUISetBit(Funckey_P->State,FUNCKEY_TOUCH);						//���ܼ����±�־
						Funckey_P->SelectNum = i+1;															//���汻���µļ����			
						
						KeyMessage = ((u32)KeyMessageType<<24) | (KEYFIRST<<8) |(Funckey_P->SelectNum | GUI_FUNCKEY_KEY);
						#if GUI_USEMSOS
						
							OS.PostMessageQueue(Funckey_P->OsPriority, (void *)(KeyMessage));		//���͵�һ�ΰ��¹��ܼ��ļ�ֵ
						#else
							//��OS��ʱ�����еĳ���
						#endif		
						GUIResetBit(Funckey_P->State,GUI_SHOWOK);							//�ؼ���ʾ�б仯
						GUI_API.TouchChange = TOUCHNULL;															//���GUI_API��touch��־
						return true;						

					}	
				}		
			}
		}
		return false;
}


/*******************************���ܰ���ֻ֧��touch����֧��Ӳ��key*************************************************/


//void FunckeyKeyFirst(void)
//{
//	if(!GUIGetBit(GUI_API.Funckey_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
//	{
//		switch(GUI_API.GUI_Key)
//		{
//		case GUIKEY_UP://����
//			FunckeyUseKeyUp();
//			break;		
//		case GUIKEY_DOWN://����
//			FunckeyUseKeyDown();
//			break;				
//		case GUIKEY_ENTER://Enter��
//			FunckeyUseKeyEnter();							
//			break;	
//		case GUIKEY_ESC://���ؼ�
//			GUI_API.CurrentForm->FocusIsSelect =false;			
//		break;

//		default:
//		 break;
//		}	
//		GUI_API.KeyChange = KEYNULL;	
//	}
//}


//void FunckeyKeyOff(FunckeyStruct *Funckey_P)
//{


//}

//void FunckeyKeyLong(FunckeyStruct *Funckey_P)
//{


//}
//void FunckeyLostFocus(void)
//{
//	if(GUI_API.Funckey_P == null)			return ;
//	if(GUI_API.Funckey_P->ID != GUIFunckeyID) 	return ;				//����Funckey������
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_FOCUS);									//�ؼ�ȡ��ϵͳ����
//	GUIResetBit(GUI_API.Funckey_P->State,FUNCKEY_FOCUSIN);						//�ؼ�ȡ���ڲ�����	
//	GUI_API.Funckey_P->SelectNum =0;																	//���ܼ�ѡ�����
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);									//�ؼ���ʾ�б仯			
//}

//void FunckeyGetFocus(void)
//{
//	if(GUI_API.Funckey_P == null)			return ;
//	if(GUI_API.Funckey_P->ID != GUIFunckeyID) 	return ;				//����Funckey������
//	GUISetBit(GUI_API.Funckey_P->State,GUI_FOCUS);										//�ؼ���ȡϵͳ����
//	GUIResetBit(GUI_API.Funckey_P->State,FUNCKEY_FOCUSIN);						//�ؼ�ȡ���ڲ�����	
//	GUI_API.Funckey_P->SelectNum =0;																	//���ܼ�ѡ�����
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);									//�ؼ���ʾ�б仯			
//}

///********************************************
//������FunckeyUseKeyUp
//���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
//������ Funckey_P SYSMenu��ַָ��
//���أ���
//˵����
//********************************************/
//void FunckeyUseKeyUp(void)
//{
//	if(GUI_API.Funckey_P->SelectNum!= 0)//���˵�һ��
//	{
//		GUI_API.Funckey_P->SelectNum--;		
//	}
//	else
//	{
//		GUI_API.Funckey_P->SelectNum = GUI_API.Funckey_P->KeyNum;
//	}
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯		
//}
///********************************************
//������FunckeyUseKeyDown
//���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
//������ Funckey_P SYSMenu��ַָ��
//���أ���
//˵����
//********************************************/
//void FunckeyUseKeyDown(void)
//{
//	if(GUI_API.Funckey_P->SelectNum!= GUI_API.Funckey_P->KeyNum)//���˵�һ��
//	{
//		GUI_API.Funckey_P->SelectNum++;		
//	}
//	else
//	{
//		GUI_API.Funckey_P->SelectNum = 0;
//	}
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯		 
//}
///********************************************
//������FunckeyUseKeyEnter
//���ܣ��˵��ɼ��̿���ʱ��ȷ�ϰ������¹���
//������ Funckey_P SYSMenu��ַָ��
//���أ���
//˵����
//********************************************/
//void FunckeyUseKeyEnter(void)
//{
//	if(GUI_API.Funckey_P->SelectNum == 0)				//��ǰѡ��Ϊ���ز˵���ʱ����back��
//	{
//		GUI_API.Funckey_P->SelectNum = 1;
//	}
//	else
//	{
//		if(GUI_API.Funckey_P->RegisterFunction[GUI_API.Funckey_P->SelectNum-1] == NULL) return ;
//		(*(GUI_API.Funckey_P->RegisterFunction[GUI_API.Funckey_P->SelectNum-1]))(); 
//	}
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯		 
//}

