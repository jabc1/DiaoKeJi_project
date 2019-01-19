/*********************************************************************************
* �ļ���			: GUI_menu.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI�˵�����								  
* �÷�			��
��menu�����еĳ�����menu delete��ʱ����Ҫ������������return��
**********************************************************************************/

#include "GUI_menu.h" 
//#include "GUI.h" 
#include "GUI_DRAW2D.h"
 #include "malloc.h"


/**************�궨��******************************/
//////MENU�ϵķ��ŵ�Ĭ����ɫ 
#define D_MENU_LBCOLOR		0XFFFF		//�ڲ�����ɫ
#define D_MENU_LSCOLOR		GUI_Red		//ѡ��list���������ɫ
//#define D_MENU_LSBCOLOR	0X001F		//0X07E0		//ѡ��list��ı���ɫ
#define D_MENU_FCOLOR		0X0000		//δѡ�е�list������ɫ
#define D_MENU_RIMCOLOR		0XF800		//�߿���ɫ
#define D_MENU_IBCOLOR	0XCE7E		//·��/��Ϣ���ı�����ɫ

void GUI_MenuLostFocus(GUI_MenuStruct *Menu_P);
void GUI_MenuGetFocus(GUI_MenuStruct *Menu_P);

void GUI_MenuUseKeyUp(GUI_MenuStruct *Menu_P);
void GUI_MenuUseKeyDown(GUI_MenuStruct *Menu_P);
void GUI_MenuUseKeyEnter(GUI_MenuStruct *Menu_P);

/********************************************
������Menu_Create
���ܣ�����Menu
������ X X��λ�ã�	Y Y��λ��
���أ���ť���ڴ��еĵ�ַָ�� ������򷵻�0
********************************************/
GUI_MenuStruct *GUI_MenuCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u16 W,u16 H)
{
 
	GUI_MenuStruct *Menu_P;
//	GUI_ScrollbarStruct * Scrollbar_P;	
	if(Form_P->ID != GUIFormID) return NULL;	
	if(W >= MENU_MAX_W) return NULL;	
	if(H >= MENU_MAX_H) return NULL;	
	if(W <= MENU_MIN_W) return NULL;	
	if(H <= MENU_MIN_H) return NULL;		
//	if (MaxLen >STRING_MAXLEN)MaxLen = STRING_MAXLEN;
	
	Menu_P=(GUI_MenuStruct *)GUI_MEM_Malloc(sizeof(GUI_MenuStruct));	//����MemNum���ֽڵ��ڴ�,ǰ16��Ϊϵͳ���ݣ�����Ϊ��ʾ����
	if(Menu_P==NULL)return NULL;			//�ڴ���䲻��.
	GUI_MemSet((u8*)Menu_P,0,sizeof(GUI_MenuStruct));//��FileList_P��ֵȫ������Ϊ0    
//	Scrollbar_P=GUI_ScrollbarCreate(X+W-SCOROLLBAR_WIDTH-1,Y+/*FILEINFO_WIDTH*/+1,H-/*FILEINFO_WIDTH*2*/-2,0);//����scrollbar.
//	if(Scrollbar_P==NULL)
//	{
//		GUI_MEM_Free(Menu_P);//�ͷ�֮ǰ������ڴ�
//		return NULL;					//�ڴ���䲻��.
// 	}	
		Menu_P->X = X;
		Menu_P->Y = Y;
		Menu_P->W = W;
		Menu_P->H = H;	
		Menu_P->Font = &Font8x16;
		Menu_P->ID = GUIMenuID;
		Menu_P->L_B_Color=D_MENU_LBCOLOR;			//�ڲ�����ɫ
		Menu_P->L_S_Color=D_MENU_LSCOLOR;		//ѡ��list���������ɫ 
//		Menu_P->L_SB_Color=D_MENU_LSBCOLOR;	//ѡ��list��ı���ɫ
		Menu_P->F_Color=D_MENU_FCOLOR;				//δѡ�е�list������ɫ
		Menu_P->Rim_Color=D_MENU_RIMCOLOR;			//�߿���ɫ
		Menu_P->I_B_Color=D_MENU_IBCOLOR;	//·��/��Ϣ�� ������ɫ			
		Menu_P->State = GUI_SHOWOK;
	
		Menu_P->ShowItemNum = Menu_P->H / (Menu_P->Font->Y+2)-1;//�˵���ʾ������
		if(Menu_P->ShowItemNum<3)Menu_P->ShowItemNum=3;
			Menu_P->MaxLen = (Menu_P->W-4) / Menu_P->Font->ASC_X;
		
		Menu_P->Range_from = 0;			//��ǰ��ʾ���ʼ���		
		Menu_P->Selected = 0;				//��ǰѡ����		
		Menu_P->OsPriority = GUI_OsCurrentPriority;
		if(GUI_FormAddChild(Form_P,(u8*)Menu_P))
		{
			GUI_MEM_Free(Menu_P);
//			GUI_MEM_Free(Scrollbar_P);
			return 0;
		}
			return Menu_P;


}


/********************************************
������GUI_SetCurrentMenus
���ܣ�����Menu�ĵ�ǰ�˵�
������ Menu_P Menu��ַָ�룻String ��Ҫ���õ��ַ���
���أ���
˵����������ʾ��������⣬����ֻ֧��20���ַ���1��������2���ַ�
********************************************/
void GUI_SetCurrentMenus(GUI_MenuStruct *Menu_P,GUIMenu *CurrentMenus)	
{
	Menu_P->CurrentMenus = CurrentMenus; 
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯
}




void GUI_MenuShowFocus(GUI_MenuStruct *Menu_P)
{
	u16 X1,X2,Y1,Y2;
	u16 Color;
	X1= Menu_P->X;
	X2 = Menu_P->X + Menu_P->W-1;
	Y1 = Menu_P->Y;
	Y2 = Menu_P->Y + Menu_P->H-1;	
	if(GUIGetBit(Menu_P->State,GUI_FOCUS)||(!GUIGetBit(Menu_P->State,GUI_FOCUS_STATE)))			//�Ƿ񽹵�ѡ��
	{
		Color = 0xFFFF-GUI.B_Color;
		GUISetBit(Menu_P->State,GUI_FOCUS_STATE);
		GUI_DrawHLine(X1,Y1,Menu_P->W,Color);
		GUI_DrawHLine(X1,Y2,Menu_P->W,Color);	
		GUI_DrawVLine(X1,Y1,Menu_P->H,Color);	
		GUI_DrawVLine(X2,Y1,Menu_P->H,Color);				
	}	
	else if((!GUIGetBit(Menu_P->State,GUI_FOCUS))||(GUIGetBit(Menu_P->State,GUI_FOCUS_STATE)))	
	{
		Color = GUI.B_Color;
		GUIResetBit(Menu_P->State,GUI_FOCUS_STATE);
		GUI_DrawHLine(X1,Y1,Menu_P->W,Color);
		GUI_DrawHLine(X1,Y2,Menu_P->W,Color);	
		GUI_DrawVLine(X1,Y1,Menu_P->H,Color);	
		GUI_DrawVLine(X2,Y1,Menu_P->H,Color);			
		
	}
	
}
/********************************************
������MenuShowNow
���ܣ�����ʾ������ʾMenu���ı�
������ Menu_P Menu��ַָ��
���أ���
˵����������û�иı�ֻ��ʾһ��
********************************************/
void GUI_MenuShowNow(GUI_MenuStruct *Menu_P)						   //
{
	u16 i;
	u8 MaxLen;
	u16 X1,Y1,X2,Row;
	if(Menu_P->ID != GUIMenuID) 	return ;				//����Menu������
	if(!GUIGetBit(Menu_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
	{
		//������ַ�����
		Row = Menu_P->Font->Y + 4;
		MaxLen = (GUI_W - Menu_P->X)/8;			//from���ܹ���ʾ�����ֳ���
		if(MaxLen>Menu_P->MaxLen)									//����ܹ���ʾ�ĳ��ȴ���������ֳ���
		{
			MaxLen = Menu_P->MaxLen;			
		}
		if(Menu_P->Y>(GUI_H-Row)) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���			

		X1 = Menu_P->X+1;
		Y1 = Menu_P->Y+1;	
		X2 = X1+Menu_P->W-3;
		if(!GUIGetBit(Menu_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{		
			if(!GUIGetBit(Menu_P->State,MENU_RUN_FRIST))	//��һ�ε�ʱ���ػ�����
			{
				GUISetBit(Menu_P->State,MENU_RUN_FRIST);
				GUI_DrawFillRectangle(X1,Y1,X2,Y1+Row,Menu_P->I_B_Color);						
				GUI_DrawFillRectangle(X1,Y1+Row+1,X2,Menu_P->Y+Menu_P->H-2,Menu_P->L_B_Color);					
			}
			
			if(Menu_P->CurrentMenus->parent==NULL)	//��ʾ����������
			{
				GUI_ShowStringAt((u8*)"MAIN",(X1+X2-Menu_P->Font->ASC_X*4)/2+3,Y1+3,Menu_P->F_Color,Menu_P->I_B_Color,Menu_P->Font);
			}
			else
			{
				GUI_ShowStringAt((u8*)"BACK",(X1+X2-Menu_P->Font->ASC_X*4)/2+3,Y1+3,Menu_P->F_Color,Menu_P->I_B_Color,Menu_P->Font);		
			}				
				X1+=2;			
			for (i=0;i<Menu_P->ShowItemNum;i++)
			{

				Y1=Menu_P->Y+(i+1)*Row+2;
				if(i<Menu_P->CurrentMenus->itemCount)
				{
					if(Menu_P->Selected == i+Menu_P->Range_from+1)
					{
						GUI_ShowStringAt(Menu_P->CurrentMenus->menuItems[i+Menu_P->Range_from],X1,Y1,Menu_P->L_S_Color,Menu_P->L_B_Color,Menu_P->Font);			
					}
					else
					{
						GUI_ShowStringAt(Menu_P->CurrentMenus->menuItems[i+Menu_P->Range_from],X1,Y1,Menu_P->F_Color,Menu_P->L_B_Color,Menu_P->Font);				
					}
				}


			}	
		}
		GUI_MenuShowFocus(Menu_P);
	}
		GUISetBit(Menu_P->State,GUI_SHOWOK);			//�ؼ���ʾ���	

}
/********************************************
������SelectChangShow
���ܣ�����select��ֵ���жϵ�ǰӦ����ʾ��ֵ
������ Menu_P Menu��ַָ��
���أ���
˵����
********************************************/
static void SelectChangShow(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->Selected>Menu_P->CurrentMenus->itemCount)			//ѡ�����Ŀ�����������
	{
		Menu_P->Selected=Menu_P->CurrentMenus->itemCount;				//ѡ�����Ŀ�����������
	}
	if(Menu_P->Selected <= 1)																//ѡ�����ĿΪ���0
	{
		Menu_P->Range_from = 0;																			//��ʾ�Ŀ�ʼ��ĿΪ0		
	}	
	else if(Menu_P->Selected<=Menu_P->Range_from+1)							//ѡ�����ĿС�ڵ�����С��ʼ��Ŀ
	{
		Menu_P->Range_from = Menu_P->Selected -2;
	}
	else if((Menu_P->Selected>=Menu_P->Range_from +Menu_P->ShowItemNum-1)	&&(Menu_P->Selected < Menu_P->CurrentMenus->itemCount))
	{
		Menu_P->Range_from = Menu_P->Selected -Menu_P->ShowItemNum+1;
	}
}
/********************************************
������GUI_MenuUseKeyUp
���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
������ Menu_P Menu��ַָ��
���أ���
˵����
********************************************/
void GUI_MenuUseKeyUp(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->Selected!= 0)//���˵�һ��
	{
		Menu_P->Selected--;
		SelectChangShow(Menu_P);
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
		
	}
}
/********************************************
������GUI_MenuUseKeyDown
���ܣ��˵��ɼ�������ѡ�����ʱ������ѡ����
������ Menu_P Menu��ַָ��
���أ���
˵����
********************************************/
void GUI_MenuUseKeyDown(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->ShowItemNum>=Menu_P->CurrentMenus->itemCount)	//��ǰ��ʾ������Ŀ���ڵ��ڵ�ǰ�˵�����Ŀ����
	{
		if(Menu_P->Selected<Menu_P->CurrentMenus->itemCount)		//��ǰѡ���С������Ŀ����
		{
			Menu_P->Selected++;
		}
	}
	else
	{
		Menu_P->Selected++;	
		SelectChangShow(Menu_P);		
	}	
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	    
}
/********************************************
������GUI_MenuUseKeyEnter
���ܣ��˵��ɼ��̿���ʱ��ȷ�ϰ������¹���
������ Menu_P Menu��ַָ��
���أ���
˵����
********************************************/
void GUI_MenuUseKeyEnter(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->Selected == 0)				//��ǰѡ��Ϊ���ز˵���ʱ����back��
	{
		if(Menu_P->CurrentMenus->parent!=NULL)//���˵���Ϊ�գ�����ʾ���˵�
		{
		 Menu_P->CurrentMenus = Menu_P->CurrentMenus->parent;
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯		
		}		
	}
	else
	{
		if(Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1] !=NULL)
		{
			Menu_P->CurrentMenus = Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1];
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
		}
		else
		{
			if(Menu_P->CurrentMenus->func[Menu_P->Selected-1] != NULL)
			{
				Menu_P->CurrentMenus->func[Menu_P->Selected-1]();//ִ����Ӧ�ĺ���
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
			}
		}	
	}
}
/********************************************
������GUI_MenuTouchUp
���ܣ��˵��ɴ���������ʱ���������ͷ�ʱ�Ĺ���
������ Menu_P Menu��ַָ��
���أ���
˵����
********************************************/
void GUI_MenuTouchUp(GUI_MenuStruct *Menu_P)
{
//	SelectChangShow(Menu_P);
//	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
//	GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//���������־	
}
/*******************************************************************/
u8 GUI_MenuTouchLong(GUI_MenuStruct *Menu_P)
{
	if(!GUIGetBit(Menu_P->State,MENU_USE_LONG_TOUCH))  //�Ƿ���Ҫ�ڳ���ʱ�ظ����г���
	{		//��
		if(GUIGetBit(Menu_P->State,MENU_LONG_STATE))
		{
			return GUI_true;			
		}
	}
	GUISetBit(Menu_P->State,MENU_LONG_STATE);				//���ó�����־	
	if(Menu_P->Selected == 0)
	{
		if(Menu_P->CurrentMenus->parent)
		{
			Menu_P->CurrentMenus = Menu_P->CurrentMenus->parent;
			Menu_P->Selected = 0;
			Menu_P->Range_from = 0;
			GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
			GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//���������־	
			GUIResetBit(Menu_P->State,MENU_RUN_FRIST);			
		}
	}
	else
	{
		if(Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1] !=NULL)
		{
			Menu_P->CurrentMenus = Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1];
			SelectChangShow(Menu_P);
			GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯	
			GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//���������־	
			GUIResetBit(Menu_P->State,MENU_RUN_FRIST);				
		}
		else
		{
			if(Menu_P->CurrentMenus->func[Menu_P->Selected-1] != NULL)
			{
				GUI_RunFunction(Menu_P->OsPriority,  Menu_P->CurrentMenus->func[Menu_P->Selected-1]);
//				OS.PostMessageQueue(Menu_P->OsPriority, Menu_P->CurrentMenus->func[Menu_P->Selected-1]);	
			}
		}			
	}

	return GUI_true;
	
}



u8 GUI_MenuTouchDown(GUI_MenuStruct *Menu_P,u8 IDNum)
{
	u16 i,Y,Row;	
	if(!GUIGetBit(Menu_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{										//����DIS״̬		
			if(Menu_P->ID == GUIMenuID)
			{
				Row = Menu_P->Font->Y + 4;
				if((GUI.Touch_X > Menu_P->X)&&(GUI.Touch_Y>Menu_P->Y)&&\
					(GUI.Touch_X<(Menu_P->X+Menu_P->MaxLen*8))&&(GUI.Touch_Y<(Menu_P->Y+Menu_P->ShowItemNum*Row+Row-1)))		 //�ж��Ƿ��ڰ�ť�ڲ�
				{	
						if((GUI.Touch_Y>Menu_P->Y)&&(GUI.Touch_Y<(Menu_P->Y+Row)))		 //�ж��Ƿ��ڶ��˲˵���	
						{
							Menu_P->Selected = 0;
							Menu_P->Range_from = 0;
							
						}					
						else		//�ж��Ƿ���ѡ��˵���
						{
							Y= Menu_P->Y+Row;
							for (i=0;i<Menu_P->ShowItemNum;i++)
							{
								if((GUI.Touch_Y>Y+i*Row)&&(GUI.Touch_Y<(Y+i*Row+Row-1)))		 
								{
									Menu_P->Selected = Menu_P->Range_from+i+1;
									break;
								}	
							}						
						}
					GUIResetBit(Menu_P->State,GUI_SHOWOK);						//�ؼ���ʾ�б仯
					GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//���������־
					GUI.CurrentForm->TouchNum = IDNum;
					GUI_FocusSelectChang(IDNum+1);		//�������
					return 1;
				}
			}
	
		}
		return 0;
		
}

void GUI_MenuKeyFirst(GUI_MenuStruct *Menu_P)
{
	if(!GUIGetBit(Menu_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
	{
		switch(GUI.KeyState)
		{
		case GUIKEY_UP://����
			GUI_MenuUseKeyUp(Menu_P);
			break;		
		case GUIKEY_DOWN://����
			GUI_MenuUseKeyDown(Menu_P);
			break;				
		case GUIKEY_ENTER://Enter��
			GUI_MenuUseKeyEnter(Menu_P);							
			break;	
		case GUIKEY_ESC://���ؼ�
			GUI.CurrentForm->FocusIsSelect =GUI_false;			
		break;

		default:
		 break;
		}	
	}
}

void GUI_MenuKeyOff(GUI_MenuStruct *Menu_P)
{


}

void GUI_MenuKeyLong(GUI_MenuStruct *Menu_P)
{


}

void GUI_MenuLostFocus(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->ID != GUIMenuID) 	return ;				//����Menu������		
	GUIResetBit(Menu_P->State,GUI_FOCUS);						//�ؼ�ȡ������
	Menu_P->Selected = 0;														//�ؼ�ȡ���ڲ�ѡ��
	Menu_P->Range_from = 0;
	GUIResetBit(Menu_P->State,GUI_SHOWOK);					//�ؼ���ʾ�б仯		
}

void GUI_MenuGetFocus(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->ID != GUIMenuID) 	return ;				//����Menu������		
	GUISetBit(Menu_P->State,GUI_FOCUS);						//�ؼ�ȡ������
	Menu_P->Selected = 0;														//�ؼ�ȡ���ڲ�ѡ��
	Menu_P->Range_from = 0;
	GUIResetBit(Menu_P->State,GUI_SHOWOK);					//�ؼ���ʾ�б仯				
}
///********************************************
//������SYSMenu_Create
//���ܣ�����SYSMenu
//������ CurrentMenus����ǰ�˵�ָ��
//���أ�
//********************************************/
//void GUI_SYSMenuCreate(GUIMenu *CurrentMenus)
//{
//	u8* MEM_P;	 
//	GUI_MenuStruct *GUI_SYSMenu_P;
//	MEM_P=mymalloc(GUIUSERAM,SYSMenu_MEMNUM);	//����SYSMenu_MEMNUM���ֽڵ��ڴ�,ǰ16��Ϊϵͳ���ݣ�����Ϊ��ʾ����
//	if(MEM_P)
//	{
//		GUI_SYSMenu_P = (GUI_MenuStruct *)MEM_P;
//		GUI_SYSMenu_P->X = 0;
//		GUI_SYSMenu_P->Y =GUI.LCDHeight-16;
//		GUI_SYSMenu_P->ID = GUISYSMenuID;
//		GUI_SYSMenu_P->BColor = D_SYSMenu_BCOLOR;
//		GUI_SYSMenu_P->FColor = D_SYSMenu_FCOLOR;
//		GUI_SYSMenu_P->SelectColor = D_SYSMenu_SELECTCOLOR;		
//		GUI_SYSMenu_P->State = 0;
//		GUI_SYSMenu_P->ShowItemNum = 4;			//�˵���ʾ������
//		GUI_SYSMenu_P->Range_from = 0;			//��ǰ��ʾ���ʼ���		
//		GUI_SYSMenu_P->Selected = 0;				//��ǰѡ����			
//		GUI_SYSMenu_P->CurrentMenus = CurrentMenus;
//		GUI.CurrentSYSMenu = GUI_SYSMenu_P;
//	}
//}
///********************************************
//������SYSMenuShowNow
//���ܣ�����ʾ������ʾSYSMenu���ı�
//������ GUI_SYSMenu_P SYSMenu��ַָ��
//���أ���
//˵����������û�иı�ֻ��ʾһ��
//********************************************/
//void GUI_SYSMenuShowNow(void)						   //
//{
//	u16 i;
//	u16 X,Y;
//	GUI_MenuStruct *GUI_SYSMenu_P;
//	GUI_SYSMenu_P = GUI.CurrentSYSMenu;
//	if(GUI_SYSMenu_P->ID != GUISYSMenuID) 	return ;				//����SYSMenu������
//	if(!GUI_GetBit(GUI_SYSMenu_P->State,GUI_STATE_CHANG))			//�ؼ��Ƿ���ʾ���
//	{
//		//������ַ�����
//		X=GUI_SYSMenu_P->X;
//		Y=GUI_SYSMenu_P->Y;	
//		if(GUI_GetBit(GUI_SYSMenu_P->State,GUI_STATE_DIS))			//�ؼ��Ƿ���ʹ��״̬
//		{	
//			//ShowDIS(GUI_SYSMenu_P);
//		}
//		else
//		{		
//			if(GUI_SYSMenu_P->CurrentMenus->parent==NULL)
//			{
//			GUI_PutString("MAIN",X+1,Y,X+56,Y+15,TopMiddle,GUI_SYSMenu_P->BColor,GUI_SYSMenu_P->FColor);	
//			}
//			else
//			{
//			GUI_PutString("BACK",X+1,Y,X+56,Y+15,TopMiddle,GUI_SYSMenu_P->BColor,GUI_SYSMenu_P->FColor);				
//			}
//			for (i=0;i<4;i++)
//			{
//				X=58+65*i;
//				if(i<GUI_SYSMenu_P->CurrentMenus->itemCount)
//				{
//					if(GUI_SYSMenu_P->Selected == i+GUI_SYSMenu_P->Range_from+1)
//					{
//						GUI_PutString(GUI_SYSMenu_P->CurrentMenus->menuItems[i+GUI_SYSMenu_P->Range_from],X,Y,X+63,Y+15,TopLeft,GUI_SYSMenu_P->FColor,GUI_SYSMenu_P->SelectColor);			
//					}
//					else
//					{
//						GUI_PutString(GUI_SYSMenu_P->CurrentMenus->menuItems[i+GUI_SYSMenu_P->Range_from],X,Y,X+63,Y+15,TopLeft,GUI_SYSMenu_P->FColor,GUI_SYSMenu_P->BColor);				
//					}
//				}
//				else
//				{
//					GUI_DrawFillRectangle(X,Y,X+65,Y+15,GUI_SYSMenu_P->BColor);
//				}

//			}	
//		}
//		if(GUI_GetBit(GUI_SYSMenu_P->State,GUI_STATE_FOCUS))			//�Ƿ񽹵�ѡ�� �����ɫ��ͬ
//	{
//		GUI_DrawHLine(GUI_SYSMenu_P->X,GUI_SYSMenu_P->Y,320-1,0xFFFF-GUI.B_Color);
//		GUI_DrawHLine(GUI_SYSMenu_P->X,GUI_SYSMenu_P->Y+16-1,320-1,0xFFFF-GUI.B_Color);	
//		GUI_DrawVLine(GUI_SYSMenu_P->X,GUI_SYSMenu_P->Y,16,0xFFFF-GUI.B_Color);	
//		GUI_DrawVLine(GUI_SYSMenu_P->X+320-1,GUI_SYSMenu_P->Y,16,0xFFFF-GUI.B_Color);		
//	}		
//	else
//	{
//		
//	}	
//		
//	}
//		GUI_SetBit(GUI_SYSMenu_P->State,GUI_STATE_CHANG);			//�ؼ���ʾ���	

//}

