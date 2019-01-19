/*********************************************************************************
* �ļ���			: GUI_button.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI ��ť						  
* �÷�			��
v2.1.2 2016.03.03
ȡ���˰�ť�ص�������GUI����������
**********************************************************************************/
#include "GUI_Button.h" 
#include "GUI_DRAW2D.h"
 #include "malloc.h"
 #include "GUI_CONF.h" 
 #include "GUI_LCDapi.h" 
 #include "GUI.h" 



/************************�Լ��ĺ�������********************************************************/

	void GUI_ButtonLostFocus(GUI_ButtonStruct *GUI_Button_P);
	void GUI_ButtonGetFocus(GUI_ButtonStruct *GUI_Button_P);
	u8 GUI_ButtonTouchDown(GUI_ButtonStruct *GUI_Button_P,u8 Num);
	void GUI_ButtonTouchUp(GUI_ButtonStruct *GUI_Button_P);	
	void GUI_ButtonKeyFirst(GUI_ButtonStruct *GUI_Button_P);
	void GUI_ButtonKeyLong(GUI_ButtonStruct *GUI_Button_P);
	void GUI_ButtonKeyOff(GUI_ButtonStruct *GUI_Button_P);
/**************�궨��******************************/
	#define	  	D_BUTT_ONCOLOR  		GUI_Red
	#define	  	D_BUTT_OFFCOLOR 		GUI_Green
	#define	  	D_BUTT_TEXTCOLOR 		GUI_Yellow
	#define	  	D_BUTT_SHADOWCOLOR  GUI_Grey
/********************************************
������Button_Create
���ܣ�������ť
������u8 Num  ��ť�ı�ţ�  X,Y ��ť��X,Y������  W,H ��ť�Ŀ�Ⱥ͸߶� Key ��ť�ļ�ֵ
���أ���ť���ڴ��еı��
********************************************/
GUI_ButtonStruct *GUI_ButtonCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y,u16 W,u16 H)
{
	GUI_ButtonStruct *GUI_Button_P;
	if(Form_P->ID != GUIFormID) return 0;		
	GUI_Button_P=(GUI_ButtonStruct *)GUI_MEM_Malloc(sizeof(GUI_ButtonStruct));	//����13���ֽڵ��ڴ�
	GUI_MemSet((u8*)GUI_Button_P,0,sizeof(GUI_ButtonStruct));//sizeof(GUI_ButtonStruct));
	if(GUI_Button_P)
	{		
		GUI_Button_P->ID = GUIButtonID;	
		GUI_Button_P->State = GUI_SHOWOK;
		GUI_Button_P->X = X;
		GUI_Button_P->Y = Y;
		GUI_Button_P->W = W;
		GUI_Button_P->H = H;
		GUI_Button_P->Font = &Font8x12;
		GUI_Button_P->TextColor =  D_BUTT_TEXTCOLOR;
		GUI_Button_P->ONColor = D_BUTT_ONCOLOR;	
		GUI_Button_P->OFFColor  = D_BUTT_OFFCOLOR;	
		GUI_Button_P->OffString = GUI_null;
		GUI_Button_P->OnString = GUI_null;
		GUI_Button_P->OsPriority = GUI_OsCurrentPriority;
		if(GUI_FormAddChild(Form_P,(u8*)GUI_Button_P))
		{
			GUI_MEM_Free(GUI_Button_P);	//�ͷ��ڴ�
			return 0;
		}
		else
		{
//			GUI_Button_P->Father = Form_P;			
			return GUI_Button_P;		
		}		
	}
	else
	{
	return GUI_null;		
	}

}

/*******************************************************************************
* ������	: GUI_ButtonRegisterFunc
* ����	    : GUI button�Ļص�����
* �������  : GUI_Button_P:button��ָ��
*           : Func: ע��ص�ִ�к�����
* ���ز���  : ��
*******************************************************************************/
void GUI_ButtonRegisterFunc_Down(GUI_ButtonStruct *GUI_Button_P, GUI_func Func_Down)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//����Button������	
	GUI_Button_P->Func_Down = Func_Down;/*�ص�����*/
}
void GUI_ButtonRegisterFunc_Long(GUI_ButtonStruct *GUI_Button_P, GUI_func Func_Long)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//����Button������	
	GUI_Button_P->Func_Long = Func_Long;/*�ص�����*/
}
void GUI_ButtonRegisterFunc_Up(GUI_ButtonStruct *GUI_Button_P, GUI_func Func_Up)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//����Button������	
	GUI_Button_P->Func_Up = Func_Up;/*�ص�����*/
}


/********************************************
������GUI_SetButtonString
���ܣ�����lable���ı�
������ GUI_Lable_P lable��ַָ�룻String ��Ҫ���õ��ַ���
���أ���
˵����������ʾ��������⣬����ֻ֧��20���ַ���1��������2���ַ�
********************************************/
void GUI_ButtonSetString(GUI_ButtonStruct *GUI_Button_P,u8 *OnString,u8* OffString)	
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//����Button������
	GUI_Button_P->OnString = OnString;
	GUI_Button_P->OffString = OffString; 
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯												
}


/********************************************
������ButtonShowOn
���ܣ���ʾButton ON״̬
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void ButtonShowOn(GUI_ButtonStruct *GUI_Button_P)
{
	u16 X1,X2,Y1,Y2;
	u16 StringW;
	X1= GUI_Button_P->X;
	X2 = GUI_Button_P->X + GUI_Button_P->W-1;
	Y1 = GUI_Button_P->Y;
	Y2 = GUI_Button_P->Y + GUI_Button_P->H-1;
	if(GUI_Button_P->OnString != GUI_null)
	{
		StringW = StringWidth(GUI_Button_P->OnString,GUI_Button_P->Font);
		GUI_DrawFillRectangle(X1+3,Y1+3,X2-1,Y2-1,GUI_Button_P->ONColor);
		GUI_ShowStringAt(GUI_Button_P->OnString,(X1+X2-StringW)/2+3,(Y1+Y2-GUI_Button_P->Font->Y)/2+4,GUI_Button_P->TextColor,GUI_Button_P->ONColor,GUI_Button_P->Font);
	}
	GUI_DrawHLine(X1,Y1,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);
	GUI_DrawHLine(X1,Y1+1,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);
	GUI_DrawHLine(X1,Y1+2,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);	
	GUI_DrawHLine(X1,Y2,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);	
	GUI_DrawVLine(X1,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);	
	GUI_DrawVLine(X1+1,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);
	GUI_DrawVLine(X1+2,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);	
	GUI_DrawVLine(X2,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);
}
/********************************************
������ButtonShowOff
���ܣ���ʾButton OFF״̬
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void ButtonShowOff(GUI_ButtonStruct *GUI_Button_P)
{
	u16 X1,X2,Y1,Y2;
	u16 StringW;
	X1= GUI_Button_P->X;
	X2 = GUI_Button_P->X + GUI_Button_P->W-1;
	Y1 = GUI_Button_P->Y;
	Y2 = GUI_Button_P->Y + GUI_Button_P->H-1;
	
	GUI_DrawFillRectangle(X1+1,Y1+1,X2-3,Y2-3,GUI_Button_P->OFFColor);	if(GUI_Button_P->OnString != GUI_null)
	{
		StringW = StringWidth(GUI_Button_P->OnString,GUI_Button_P->Font);
		GUI_DrawFillRectangle(X1+1,Y1+1,X2-3,Y2-3,GUI_Button_P->OFFColor);
		GUI_ShowStringAt(GUI_Button_P->OffString,(X1+X2-StringW)/2+1,(Y1+Y2-GUI_Button_P->Font->Y)/2+2,GUI_Button_P->TextColor,GUI_Button_P->OFFColor,GUI_Button_P->Font);
	}
	GUI_DrawHLine(X1,Y1,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);
	GUI_DrawHLine(X1,Y2-1,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);
	GUI_DrawHLine(X1,Y2-2,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);	
	GUI_DrawHLine(X1,Y2,GUI_Button_P->W,D_BUTT_SHADOWCOLOR);	
	GUI_DrawVLine(X1,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);	
	GUI_DrawVLine(X2-2,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);
	GUI_DrawVLine(X2-1,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);
	GUI_DrawVLine(X2,Y1,GUI_Button_P->H,D_BUTT_SHADOWCOLOR);
}
/********************************************
������ButtonShowDIS
���ܣ���ʾButton DIS״̬
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void ButtonShowDIS(GUI_ButtonStruct *GUI_Button_P)
{
	u16 i,k;
	for (i=0;i<GUI_Button_P->H;i++)
	{
		for (k=0;k<GUI_Button_P->W;k++)
		{
			if((i==0)||i>(GUI_Button_P->H-4)||k==0||k>(GUI_Button_P->W-4))
			{
				GUI_SetPoint(GUI_Button_P->X+k,GUI_Button_P->Y+i,D_BUTT_SHADOWCOLOR);//��Ӱ��ɫ
			}
			else{
				GUI_SetPoint(GUI_Button_P->X+k,GUI_Button_P->Y+i,GUI.DIS_Color); //ON����ɫ
			}
		}
	}
}
/********************************************
������GUI_ButtonShowNow
���ܣ�Button��ʾˢ�³���
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void GUI_ButtonShowNow(GUI_ButtonStruct *GUI_Button_P)						   //0XFF  0x10  0x11   0x20   0x22
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//����Button������
		if(!GUIGetBit(GUI_Button_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
		{
			if((GUI_Button_P->X + GUI_Button_P->W)>(GUI_W)) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���	
			if((GUI_Button_P->Y+GUI_Button_P->H)>(GUI_H)) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���		
			if(GUIGetBit(GUI_Button_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
			{	
				ButtonShowDIS(GUI_Button_P);
			}
			else
			{
				if(GUIGetBit(GUI_Button_P->State,GUI_ON))
				{																										//�ؼ�ΪON״̬
					ButtonShowOn(GUI_Button_P);
				}
				else
				{																										//�ؼ�ΪOFF״̬
					ButtonShowOff(GUI_Button_P);
				}
			}	
			if(GUIGetBit(GUI_Button_P->State,GUI_FOCUS))			//�Ƿ񽹵�ѡ��
			{
				GUI_DrawHLine(GUI_Button_P->X,GUI_Button_P->Y,GUI_Button_P->W,0xFFFF-GUI.B_Color);
				GUI_DrawHLine(GUI_Button_P->X,GUI_Button_P->Y+GUI_Button_P->H-1,GUI_Button_P->W,0xFFFF-GUI.B_Color);	
				GUI_DrawVLine(GUI_Button_P->X,GUI_Button_P->Y,GUI_Button_P->H,0xFFFF-GUI.B_Color);	
				GUI_DrawVLine(GUI_Button_P->X+GUI_Button_P->W-1,GUI_Button_P->Y,GUI_Button_P->H,0xFFFF-GUI.B_Color);		
			}			
		}
		GUISetBit(GUI_Button_P->State,GUI_SHOWOK);			//�ؼ���ʾ���
}


/********************************************
������GUI_Button_First
���ܣ���ť���º�ĵ�һ��ȷ��ʱ������button��״̬������ע��ĳ���
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void GUI_Button_Down(GUI_ButtonStruct *GUI_Button_P)
{
	GUISetBit(GUI_Button_P->State,GUI_ON);				//����ΪON״̬
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);			//�ؼ����ػ�		
	if(GUI_Button_P->Func_Down == NULL) return ;
	GUI_RunFunction(GUI_Button_P->OsPriority, GUI_Button_P->Func_Down);
}
/********************************************
������GUI_Button_Long
���ܣ���ť���º���ظ�ȷ��ʱ������button��״̬������ע��ĳ���
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵�����Ժ���Ը�Ϊ�Լ���longע��ص�����
********************************************/
void GUI_Button_Long(GUI_ButtonStruct *GUI_Button_P) 
{
	if(GUI_Button_P->Func_Long == NULL) return ;
	GUI_RunFunction(GUI_Button_P->OsPriority, GUI_Button_P->Func_Long);	


}
/********************************************
������GUI_Button_UP
���ܣ���ť�ͷ�ʱ������button��״̬
������GUI_Button_P: Button��ַָ�룻
���أ���
˵�����Ժ���Ը�Ϊ�Լ���longע��ص�����
********************************************/
void GUI_Button_UP(GUI_ButtonStruct *GUI_Button_P)
{

	GUIResetBit(GUI_Button_P->State,GUI_ON);				//����ΪOFF״̬
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);			//�ؼ����ػ�
	if(GUI_Button_P->Func_Up == NULL) return ;
	GUI_RunFunction(GUI_Button_P->OsPriority, GUI_Button_P->Func_Up);
}


/********************************************
������GUI_ButtonKeyFirst
���ܣ�Ӳ���������º�ĵ�һ��ȷ��ʱ������button��״̬������ע��ĳ���
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void GUI_ButtonKeyDown(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//��ť����DIS״̬		
		if(GUIGetBit(GUI_Button_P->State,GUI_FOCUS))						//�ؼ��ڽ�����
		{
			GUI_Button_Down(GUI_Button_P);		
		}
	}
}
/********************************************
������GUI_ButtonKeyLong
���ܣ�Ӳ���������º���ظ�ȷ��ʱ������button��״̬������ע��ĳ���
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵�����Ժ���Ը�Ϊ�Լ���longע��ص�����
********************************************/
void GUI_ButtonKeyLong(GUI_ButtonStruct *GUI_Button_P) 
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//��ť����DIS״̬		
		if(GUIGetBit(GUI_Button_P->State,GUI_FOCUS))						//�ؼ��ڽ�����
		{		
			GUI_Button_Long(GUI_Button_P);		

		}
	}



}

/********************************************
������GUI_ButtonKeyUp
���ܣ�Ӳ�������ͷ�ʱ������button��״̬
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵�����Ժ���Ը�Ϊ�Լ���longע��ص�����
********************************************/
void GUI_ButtonKeyUp(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//��ť����DIS״̬		
		GUI_Button_UP(GUI_Button_P);
	}
	
	
}




/********************************************
������GUI_ButtonTouchDown
���ܣ��������������µ�ʱ������button��״̬
������ GUI_Button_P: Button��ַָ�룻Num���ڵ�ǰ��FORM�еı��
���أ���
˵����
********************************************/
u8 GUI_ButtonTouchDown(GUI_ButtonStruct *GUI_Button_P,u8 Num)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))										//��ť����DIS״̬		
		{													
			if((GUI.Touch_X > GUI_Button_P->X)&&(GUI.Touch_Y>GUI_Button_P->Y)&&(GUI.Touch_X<(GUI_Button_P->X+GUI_Button_P->W))&&(GUI.Touch_Y<(GUI_Button_P->Y+GUI_Button_P->H)))		 //�ж��Ƿ��ڰ�ť�ڲ�
			{	
				GUI.CurrentForm->TouchNum = Num;
				GUI_FocusSelectChang(Num+1);		//�������				
				GUI_Button_Down(GUI_Button_P);									
			return 1;
			}
		}
		return 0;
}
/********************************************
������GUI_ButtonTouchUp
���ܣ������������ͷŵ�ʱ������button��״̬
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void GUI_ButtonTouchLong(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//��ť����DIS״̬		
		GUI_Button_Long(GUI_Button_P);
	}
}
/********************************************
������GUI_ButtonTouchUp
���ܣ������������ͷŵ�ʱ������button��״̬
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵����
********************************************/
void GUI_ButtonTouchUp(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//��ť����DIS״̬		
		GUI_Button_UP(GUI_Button_P);
	}
}



/********************************************
������GUI_ButtonLostFocus
���ܣ�buttonʧȥ����
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵�����Ժ���Ը�Ϊ�Լ���longע��ص�����
********************************************/

void GUI_ButtonLostFocus(GUI_ButtonStruct *GUI_Button_P)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//����Button������		
	GUIResetBit(GUI_Button_P->State,GUI_FOCUS);						//�ؼ�ȡ������
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);					//�ؼ���ʾ�б仯		
}
/********************************************
������GUI_ButtonGetFocus
���ܣ�button��ý���
������ GUI_Button_P: Button��ַָ�룻
���أ���
˵�����Ժ���Ը�Ϊ�Լ���longע��ص�����
********************************************/
void GUI_ButtonGetFocus(GUI_ButtonStruct *GUI_Button_P)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//����Button������		
	GUISetBit(GUI_Button_P->State,GUI_FOCUS);						//�ؼ�ȡ������
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);					//�ؼ���ʾ�б仯				
}





