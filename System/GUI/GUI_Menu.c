/*********************************************************************************
* 文件名			: GUI_menu.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI菜单界面								  
* 用法			：
当menu中运行的程序会把menu delete的时候，需要在运行完程序后return。
**********************************************************************************/

#include "GUI_menu.h" 
//#include "GUI.h" 
#include "GUI_DRAW2D.h"
 #include "malloc.h"


/**************宏定义******************************/
//////MENU上的符号的默认颜色 
#define D_MENU_LBCOLOR		0XFFFF		//内部背景色
#define D_MENU_LSCOLOR		GUI_Red		//选中list后的字体颜色
//#define D_MENU_LSBCOLOR	0X001F		//0X07E0		//选中list后的背景色
#define D_MENU_FCOLOR		0X0000		//未选中的list字体颜色
#define D_MENU_RIMCOLOR		0XF800		//边框颜色
#define D_MENU_IBCOLOR	0XCE7E		//路径/信息栏的背景颜色

void GUI_MenuLostFocus(GUI_MenuStruct *Menu_P);
void GUI_MenuGetFocus(GUI_MenuStruct *Menu_P);

void GUI_MenuUseKeyUp(GUI_MenuStruct *Menu_P);
void GUI_MenuUseKeyDown(GUI_MenuStruct *Menu_P);
void GUI_MenuUseKeyEnter(GUI_MenuStruct *Menu_P);

/********************************************
函数：Menu_Create
功能：创建Menu
参数： X X轴位置；	Y Y轴位置
返回：按钮在内存中的地址指针 如出错则返回0
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
	
	Menu_P=(GUI_MenuStruct *)GUI_MEM_Malloc(sizeof(GUI_MenuStruct));	//申请MemNum个字节的内存,前16个为系统数据，后面为显示内容
	if(Menu_P==NULL)return NULL;			//内存分配不够.
	GUI_MemSet((u8*)Menu_P,0,sizeof(GUI_MenuStruct));//将FileList_P的值全部设置为0    
//	Scrollbar_P=GUI_ScrollbarCreate(X+W-SCOROLLBAR_WIDTH-1,Y+/*FILEINFO_WIDTH*/+1,H-/*FILEINFO_WIDTH*2*/-2,0);//创建scrollbar.
//	if(Scrollbar_P==NULL)
//	{
//		GUI_MEM_Free(Menu_P);//释放之前申请的内存
//		return NULL;					//内存分配不够.
// 	}	
		Menu_P->X = X;
		Menu_P->Y = Y;
		Menu_P->W = W;
		Menu_P->H = H;	
		Menu_P->Font = &Font8x16;
		Menu_P->ID = GUIMenuID;
		Menu_P->L_B_Color=D_MENU_LBCOLOR;			//内部背景色
		Menu_P->L_S_Color=D_MENU_LSCOLOR;		//选中list后的字体颜色 
//		Menu_P->L_SB_Color=D_MENU_LSBCOLOR;	//选中list后的背景色
		Menu_P->F_Color=D_MENU_FCOLOR;				//未选中的list字体颜色
		Menu_P->Rim_Color=D_MENU_RIMCOLOR;			//边框颜色
		Menu_P->I_B_Color=D_MENU_IBCOLOR;	//路径/信息栏 背景颜色			
		Menu_P->State = GUI_SHOWOK;
	
		Menu_P->ShowItemNum = Menu_P->H / (Menu_P->Font->Y+2)-1;//菜单显示的条数
		if(Menu_P->ShowItemNum<3)Menu_P->ShowItemNum=3;
			Menu_P->MaxLen = (Menu_P->W-4) / Menu_P->Font->ASC_X;
		
		Menu_P->Range_from = 0;			//当前显示的项开始序号		
		Menu_P->Selected = 0;				//当前选择项		
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
函数：GUI_SetCurrentMenus
功能：设置Menu的当前菜单
参数： Menu_P Menu地址指针；String 需要设置的字符串
返回：无
说明：由于显示屏宽度问题，现在只支持20个字符，1个中文算2个字符
********************************************/
void GUI_SetCurrentMenus(GUI_MenuStruct *Menu_P,GUIMenu *CurrentMenus)	
{
	Menu_P->CurrentMenus = CurrentMenus; 
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化
}




void GUI_MenuShowFocus(GUI_MenuStruct *Menu_P)
{
	u16 X1,X2,Y1,Y2;
	u16 Color;
	X1= Menu_P->X;
	X2 = Menu_P->X + Menu_P->W-1;
	Y1 = Menu_P->Y;
	Y2 = Menu_P->Y + Menu_P->H-1;	
	if(GUIGetBit(Menu_P->State,GUI_FOCUS)||(!GUIGetBit(Menu_P->State,GUI_FOCUS_STATE)))			//是否焦点选择
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
函数：MenuShowNow
功能：在显示屏上显示Menu的文本
参数： Menu_P Menu地址指针
返回：无
说明：如内容没有改变只显示一次
********************************************/
void GUI_MenuShowNow(GUI_MenuStruct *Menu_P)						   //
{
	u16 i;
	u8 MaxLen;
	u16 X1,Y1,X2,Row;
	if(Menu_P->ID != GUIMenuID) 	return ;				//不是Menu的数据
	if(!GUIGetBit(Menu_P->State,GUI_SHOWOK))			//控件是否显示完成
	{
		//允许的字符数量
		Row = Menu_P->Font->Y + 4;
		MaxLen = (GUI_W - Menu_P->X)/8;			//from中能够显示的文字长度
		if(MaxLen>Menu_P->MaxLen)									//如果能够显示的长度大于最大文字长度
		{
			MaxLen = Menu_P->MaxLen;			
		}
		if(Menu_P->Y>(GUI_H-Row)) return;		//如果显示的高度大于能够显示的高度，直接返回			

		X1 = Menu_P->X+1;
		Y1 = Menu_P->Y+1;	
		X2 = X1+Menu_P->W-3;
		if(!GUIGetBit(Menu_P->State,GUI_DIS))			//控件是否是使能状态
		{		
			if(!GUIGetBit(Menu_P->State,MENU_RUN_FRIST))	//第一次的时候重画背景
			{
				GUISetBit(Menu_P->State,MENU_RUN_FRIST);
				GUI_DrawFillRectangle(X1,Y1,X2,Y1+Row,Menu_P->I_B_Color);						
				GUI_DrawFillRectangle(X1,Y1+Row+1,X2,Menu_P->Y+Menu_P->H-2,Menu_P->L_B_Color);					
			}
			
			if(Menu_P->CurrentMenus->parent==NULL)	//显示标题栏内容
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
		GUISetBit(Menu_P->State,GUI_SHOWOK);			//控件显示完成	

}
/********************************************
函数：SelectChangShow
功能：根据select的值来判断当前应该显示的值
参数： Menu_P Menu地址指针
返回：无
说明：
********************************************/
static void SelectChangShow(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->Selected>Menu_P->CurrentMenus->itemCount)			//选择的条目大于最大数量
	{
		Menu_P->Selected=Menu_P->CurrentMenus->itemCount;				//选择的条目等于最大数量
	}
	if(Menu_P->Selected <= 1)																//选择的条目为最顶端0
	{
		Menu_P->Range_from = 0;																			//显示的开始条目为0		
	}	
	else if(Menu_P->Selected<=Menu_P->Range_from+1)							//选择的条目小于等于最小开始条目
	{
		Menu_P->Range_from = Menu_P->Selected -2;
	}
	else if((Menu_P->Selected>=Menu_P->Range_from +Menu_P->ShowItemNum-1)	&&(Menu_P->Selected < Menu_P->CurrentMenus->itemCount))
	{
		Menu_P->Range_from = Menu_P->Selected -Menu_P->ShowItemNum+1;
	}
}
/********************************************
函数：GUI_MenuUseKeyUp
功能：菜单由键盘上下选择控制时，向上选择功能
参数： Menu_P Menu地址指针
返回：无
说明：
********************************************/
void GUI_MenuUseKeyUp(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->Selected!= 0)//到了第一项
	{
		Menu_P->Selected--;
		SelectChangShow(Menu_P);
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化	
		
	}
}
/********************************************
函数：GUI_MenuUseKeyDown
功能：菜单由键盘上下选择控制时，向下选择功能
参数： Menu_P Menu地址指针
返回：无
说明：
********************************************/
void GUI_MenuUseKeyDown(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->ShowItemNum>=Menu_P->CurrentMenus->itemCount)	//当前显示的总条目大于等于当前菜单的条目数量
	{
		if(Menu_P->Selected<Menu_P->CurrentMenus->itemCount)		//当前选择的小于总条目数量
		{
			Menu_P->Selected++;
		}
	}
	else
	{
		Menu_P->Selected++;	
		SelectChangShow(Menu_P);		
	}	
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化	    
}
/********************************************
函数：GUI_MenuUseKeyEnter
功能：菜单由键盘控制时，确认按键按下功能
参数： Menu_P Menu地址指针
返回：无
说明：
********************************************/
void GUI_MenuUseKeyEnter(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->Selected == 0)				//当前选择为主控菜单项时，“back”
	{
		if(Menu_P->CurrentMenus->parent!=NULL)//父菜单不为空，将显示父菜单
		{
		 Menu_P->CurrentMenus = Menu_P->CurrentMenus->parent;
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化		
		}		
	}
	else
	{
		if(Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1] !=NULL)
		{
			Menu_P->CurrentMenus = Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1];
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化	
		}
		else
		{
			if(Menu_P->CurrentMenus->func[Menu_P->Selected-1] != NULL)
			{
				Menu_P->CurrentMenus->func[Menu_P->Selected-1]();//执行相应的函数
	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化	
			}
		}	
	}
}
/********************************************
函数：GUI_MenuTouchUp
功能：菜单由触摸屏控制时，触摸屏释放时的功能
参数： Menu_P Menu地址指针
返回：无
说明：
********************************************/
void GUI_MenuTouchUp(GUI_MenuStruct *Menu_P)
{
//	SelectChangShow(Menu_P);
//	GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化	
//	GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//清除常按标志	
}
/*******************************************************************/
u8 GUI_MenuTouchLong(GUI_MenuStruct *Menu_P)
{
	if(!GUIGetBit(Menu_P->State,MENU_USE_LONG_TOUCH))  //是否需要在常按时重复运行程序
	{		//否
		if(GUIGetBit(Menu_P->State,MENU_LONG_STATE))
		{
			return GUI_true;			
		}
	}
	GUISetBit(Menu_P->State,MENU_LONG_STATE);				//设置常按标志	
	if(Menu_P->Selected == 0)
	{
		if(Menu_P->CurrentMenus->parent)
		{
			Menu_P->CurrentMenus = Menu_P->CurrentMenus->parent;
			Menu_P->Selected = 0;
			Menu_P->Range_from = 0;
			GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化	
			GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//清除常按标志	
			GUIResetBit(Menu_P->State,MENU_RUN_FRIST);			
		}
	}
	else
	{
		if(Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1] !=NULL)
		{
			Menu_P->CurrentMenus = Menu_P->CurrentMenus->subMenus[Menu_P->Selected-1];
			SelectChangShow(Menu_P);
			GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化	
			GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//清除常按标志	
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
	if(!GUIGetBit(Menu_P->State,GUI_DIS))			//控件是否是使能状态
		{										//不是DIS状态		
			if(Menu_P->ID == GUIMenuID)
			{
				Row = Menu_P->Font->Y + 4;
				if((GUI.Touch_X > Menu_P->X)&&(GUI.Touch_Y>Menu_P->Y)&&\
					(GUI.Touch_X<(Menu_P->X+Menu_P->MaxLen*8))&&(GUI.Touch_Y<(Menu_P->Y+Menu_P->ShowItemNum*Row+Row-1)))		 //判断是否在按钮内部
				{	
						if((GUI.Touch_Y>Menu_P->Y)&&(GUI.Touch_Y<(Menu_P->Y+Row)))		 //判断是否在顶端菜单内	
						{
							Menu_P->Selected = 0;
							Menu_P->Range_from = 0;
							
						}					
						else		//判断是否在选择菜单内
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
					GUIResetBit(Menu_P->State,GUI_SHOWOK);						//控件显示有变化
					GUIResetBit(Menu_P->State,MENU_LONG_STATE);				//清除常按标志
					GUI.CurrentForm->TouchNum = IDNum;
					GUI_FocusSelectChang(IDNum+1);		//变更焦点
					return 1;
				}
			}
	
		}
		return 0;
		
}

void GUI_MenuKeyFirst(GUI_MenuStruct *Menu_P)
{
	if(!GUIGetBit(Menu_P->State,GUI_DIS))			//控件是否是使能状态
	{
		switch(GUI.KeyState)
		{
		case GUIKEY_UP://向上
			GUI_MenuUseKeyUp(Menu_P);
			break;		
		case GUIKEY_DOWN://向下
			GUI_MenuUseKeyDown(Menu_P);
			break;				
		case GUIKEY_ENTER://Enter键
			GUI_MenuUseKeyEnter(Menu_P);							
			break;	
		case GUIKEY_ESC://返回键
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
	if(Menu_P->ID != GUIMenuID) 	return ;				//不是Menu的数据		
	GUIResetBit(Menu_P->State,GUI_FOCUS);						//控件取消焦点
	Menu_P->Selected = 0;														//控件取消内部选择
	Menu_P->Range_from = 0;
	GUIResetBit(Menu_P->State,GUI_SHOWOK);					//控件显示有变化		
}

void GUI_MenuGetFocus(GUI_MenuStruct *Menu_P)
{
	if(Menu_P->ID != GUIMenuID) 	return ;				//不是Menu的数据		
	GUISetBit(Menu_P->State,GUI_FOCUS);						//控件取消焦点
	Menu_P->Selected = 0;														//控件取消内部选择
	Menu_P->Range_from = 0;
	GUIResetBit(Menu_P->State,GUI_SHOWOK);					//控件显示有变化				
}
///********************************************
//函数：SYSMenu_Create
//功能：创建SYSMenu
//参数： CurrentMenus：当前菜单指针
//返回：
//********************************************/
//void GUI_SYSMenuCreate(GUIMenu *CurrentMenus)
//{
//	u8* MEM_P;	 
//	GUI_MenuStruct *GUI_SYSMenu_P;
//	MEM_P=mymalloc(GUIUSERAM,SYSMenu_MEMNUM);	//申请SYSMenu_MEMNUM个字节的内存,前16个为系统数据，后面为显示内容
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
//		GUI_SYSMenu_P->ShowItemNum = 4;			//菜单显示的条数
//		GUI_SYSMenu_P->Range_from = 0;			//当前显示的项开始序号		
//		GUI_SYSMenu_P->Selected = 0;				//当前选择项			
//		GUI_SYSMenu_P->CurrentMenus = CurrentMenus;
//		GUI.CurrentSYSMenu = GUI_SYSMenu_P;
//	}
//}
///********************************************
//函数：SYSMenuShowNow
//功能：在显示屏上显示SYSMenu的文本
//参数： GUI_SYSMenu_P SYSMenu地址指针
//返回：无
//说明：如内容没有改变只显示一次
//********************************************/
//void GUI_SYSMenuShowNow(void)						   //
//{
//	u16 i;
//	u16 X,Y;
//	GUI_MenuStruct *GUI_SYSMenu_P;
//	GUI_SYSMenu_P = GUI.CurrentSYSMenu;
//	if(GUI_SYSMenu_P->ID != GUISYSMenuID) 	return ;				//不是SYSMenu的数据
//	if(!GUI_GetBit(GUI_SYSMenu_P->State,GUI_STATE_CHANG))			//控件是否显示完成
//	{
//		//允许的字符数量
//		X=GUI_SYSMenu_P->X;
//		Y=GUI_SYSMenu_P->Y;	
//		if(GUI_GetBit(GUI_SYSMenu_P->State,GUI_STATE_DIS))			//控件是否是使能状态
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
//		if(GUI_GetBit(GUI_SYSMenu_P->State,GUI_STATE_FOCUS))			//是否焦点选择 外框颜色不同
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
//		GUI_SetBit(GUI_SYSMenu_P->State,GUI_STATE_CHANG);			//控件显示完成	

//}

