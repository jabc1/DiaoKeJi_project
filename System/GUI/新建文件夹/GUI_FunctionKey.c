/*********************************************************************************
* 文件名			: Funckey.c
* 版本号    	: V2.1.0
* 日期      : 2015/09/29
* 说明      : GUI触摸模拟键盘								  
* 用法			：

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

#define			D_Funckey_W				40		//功能键的宽度为40
#define			D_Funckey_H				18		//功能键的高度为20



FunckeyStringStruct D_FunckeyString[] = {"上页","上","下","左","右","HC1","HC2","下页"};
//static const u8 keyID[] = {GUIKEY_0,GUIKEY_1,GUIKEY_2,GUIKEY_3,GUIKEY_UP,GUIKEY_DOWN,GUIKEY_ENTER,GUIKEY_ESC}; 
/********************************************
函数：SYSMenu_Create
功能：创建SYSMenu
参数： Mode：当前菜单指针
返回：按钮在内存中的地址指针 如出错则返回0
********************************************/
FunckeyStruct *GUI_FunckeyCreate(void)
{

	FunckeyStruct *Funckey_P;	
	Funckey_P=(FunckeyStruct *)GUI_MEM_Malloc(sizeof(FunckeyStruct));	//申请SYSMenu_MEMNUM个字节的内存,前16个为系统数据，后面为显示内容
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
			Funckey_P->OsPriority = OsCurrentPriority;			//存储当前创建FUNCKEY的线程OS优先级
		#endif	
		Funckey_P->String = &D_FunckeyString[0];
		Funckey_P->State = 0;		
		if(GUI_W == 320)		//屏幕的宽度是320
		{
			Funckey_P->KeyNum = 8;			
		}
		else if(GUI_W == 240)		
		{
			Funckey_P->KeyNum = 6;			
		}
		else
		{
			GUI_MEM_Free(Funckey_P);	//释放内存			
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
函数：FunckeyShowString
功能：在显示屏上显示Funckey的文本
参数： Num:Funckey选择的按键编号
返回：无
说明：
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
函数：GUI_FunckeyShowNow
功能：在显示屏上显示GUI_Funckey
参数： Funckey_P 地址指针
返回：无
说明：如内容没有改变只显示一次
********************************************/
void GUI_FunckeyShowNow(void)						   //
{
	u16 i;
	FunckeyStruct *Funckey_P;	
	Funckey_P = (FunckeyStruct *)GUI_API.FuncKey_P;
	if(Funckey_P == null)			return ;
	if(Funckey_P->ID != GUIFunckeyID) 	return ;				//不是Funckey的数据
	if(!GUIGetBit(Funckey_P->State,GUI_SHOWOK))			//控件是否显示完成
	{
		if(GUIGetBit(Funckey_P->State,GUI_DIS))			//控件是否是使能状态
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
		GUISetBit(Funckey_P->State,GUI_SHOWOK);			//控件显示完成	

}

/********************************************
函数：FunckeyTouchUp
功能：菜单由触摸屏控制时，触摸屏释放时的功能
参数： Funckey_P SYSMenu地址指针
返回：无
说明：
********************************************/
u8 GUI_FunckeyTouchUp(void)
{
	FunckeyStruct *Funckey_P;	
	u32 KeyMessage;
	Funckey_P = (FunckeyStruct *)GUI_API.FuncKey_P;	
	if(Funckey_P == null)		return false;
	if(!GUIGetBit(Funckey_P->State,GUI_DIS))		//不是DISABLE
	{
		if(GUIGetBit(Funckey_P->State,FUNCKEY_TOUCH))		//按键被按下
		{			
			KeyMessage = ((u32)KeyMessageType<<24) | (KEYOFF<<8) |(Funckey_P->SelectNum | GUI_FUNCKEY_KEY);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(Funckey_P->OsPriority, (void *)(KeyMessage));		//发送长时间按下功能键的键值
			#else
				//非OS的时候运行的程序
			#endif				
			GUIResetBit(Funckey_P->State,FUNCKEY_TOUCH);					//清除功能键按下标志
			Funckey_P->SelectNum = 0;															//清除被按下的键编号
			GUIResetBit(Funckey_P->State,GUI_SHOWOK);							//控件显示有变化
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
	if(!GUIGetBit(Funckey_P->State,GUI_DIS))		//不是DISABLE
	{
		if(GUIGetBit(Funckey_P->State,FUNCKEY_TOUCH))		//按键被按下
		{
			KeyMessage = ((u32)KeyMessageType<<24) | (KEYLONG<<8) |(Funckey_P->SelectNum | GUI_FUNCKEY_KEY);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(Funckey_P->OsPriority, (void *)(KeyMessage));		//发送长时间按下功能键的键值
			#else
				//非OS的时候运行的程序
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
	if(!GUIGetBit(Funckey_P->State,GUI_DIS))			//控件是否是使能状态
		{										//不是DIS状态			
			if(GUI_API.Touch_Y>Funckey_P->Y)	 //判断是否在按钮内部
			{	
//				X= Funckey_P->X+16;
				for (i=0;i<Funckey_P->KeyNum;i++)
				{
					if((GUI_API.Touch_X>i*D_Funckey_W)&&(GUI_API.Touch_X<((i+1)*D_Funckey_W)))		 
					{
						GUISetBit(Funckey_P->State,FUNCKEY_TOUCH);						//功能键按下标志
						Funckey_P->SelectNum = i+1;															//保存被按下的键编号			
						
						KeyMessage = ((u32)KeyMessageType<<24) | (KEYFIRST<<8) |(Funckey_P->SelectNum | GUI_FUNCKEY_KEY);
						#if GUI_USEMSOS
						
							OS.PostMessageQueue(Funckey_P->OsPriority, (void *)(KeyMessage));		//发送第一次按下功能键的键值
						#else
							//非OS的时候运行的程序
						#endif		
						GUIResetBit(Funckey_P->State,GUI_SHOWOK);							//控件显示有变化
						GUI_API.TouchChange = TOUCHNULL;															//清除GUI_API的touch标志
						return true;						

					}	
				}		
			}
		}
		return false;
}


/*******************************功能按键只支持touch，不支持硬件key*************************************************/


//void FunckeyKeyFirst(void)
//{
//	if(!GUIGetBit(GUI_API.Funckey_P->State,GUI_DIS))			//控件是否是使能状态
//	{
//		switch(GUI_API.GUI_Key)
//		{
//		case GUIKEY_UP://向上
//			FunckeyUseKeyUp();
//			break;		
//		case GUIKEY_DOWN://向下
//			FunckeyUseKeyDown();
//			break;				
//		case GUIKEY_ENTER://Enter键
//			FunckeyUseKeyEnter();							
//			break;	
//		case GUIKEY_ESC://返回键
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
//	if(GUI_API.Funckey_P->ID != GUIFunckeyID) 	return ;				//不是Funckey的数据
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_FOCUS);									//控件取消系统焦点
//	GUIResetBit(GUI_API.Funckey_P->State,FUNCKEY_FOCUSIN);						//控件取消内部焦点	
//	GUI_API.Funckey_P->SelectNum =0;																	//功能键选择归零
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);									//控件显示有变化			
//}

//void FunckeyGetFocus(void)
//{
//	if(GUI_API.Funckey_P == null)			return ;
//	if(GUI_API.Funckey_P->ID != GUIFunckeyID) 	return ;				//不是Funckey的数据
//	GUISetBit(GUI_API.Funckey_P->State,GUI_FOCUS);										//控件获取系统焦点
//	GUIResetBit(GUI_API.Funckey_P->State,FUNCKEY_FOCUSIN);						//控件取消内部焦点	
//	GUI_API.Funckey_P->SelectNum =0;																	//功能键选择归零
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);									//控件显示有变化			
//}

///********************************************
//函数：FunckeyUseKeyUp
//功能：菜单由键盘上下选择控制时，向上选择功能
//参数： Funckey_P SYSMenu地址指针
//返回：无
//说明：
//********************************************/
//void FunckeyUseKeyUp(void)
//{
//	if(GUI_API.Funckey_P->SelectNum!= 0)//到了第一项
//	{
//		GUI_API.Funckey_P->SelectNum--;		
//	}
//	else
//	{
//		GUI_API.Funckey_P->SelectNum = GUI_API.Funckey_P->KeyNum;
//	}
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);						//控件显示有变化		
//}
///********************************************
//函数：FunckeyUseKeyDown
//功能：菜单由键盘上下选择控制时，向下选择功能
//参数： Funckey_P SYSMenu地址指针
//返回：无
//说明：
//********************************************/
//void FunckeyUseKeyDown(void)
//{
//	if(GUI_API.Funckey_P->SelectNum!= GUI_API.Funckey_P->KeyNum)//到了第一项
//	{
//		GUI_API.Funckey_P->SelectNum++;		
//	}
//	else
//	{
//		GUI_API.Funckey_P->SelectNum = 0;
//	}
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);						//控件显示有变化		 
//}
///********************************************
//函数：FunckeyUseKeyEnter
//功能：菜单由键盘控制时，确认按键按下功能
//参数： Funckey_P SYSMenu地址指针
//返回：无
//说明：
//********************************************/
//void FunckeyUseKeyEnter(void)
//{
//	if(GUI_API.Funckey_P->SelectNum == 0)				//当前选择为主控菜单项时，“back”
//	{
//		GUI_API.Funckey_P->SelectNum = 1;
//	}
//	else
//	{
//		if(GUI_API.Funckey_P->RegisterFunction[GUI_API.Funckey_P->SelectNum-1] == NULL) return ;
//		(*(GUI_API.Funckey_P->RegisterFunction[GUI_API.Funckey_P->SelectNum-1]))(); 
//	}
//	GUIResetBit(GUI_API.Funckey_P->State,GUI_SHOWOK);						//控件显示有变化		 
//}

