/*********************************************************************************
* 文件名			: KeyBoard.c
* 版本号    	: V2.1.0
* 日期      : 2015/09/29
* 说明      : GUI触摸模拟键盘								  
* 用法			：

**********************************************************************************/
#include "malloc.h"	
#include "GUI_KeyBoard.h" 
#include "GUI_CONF.h"
#include "OS.h"
#define	  	D_KeyBoard_BCOLOR 		GUI_Red
#define	  	D_KeyBoard_FCOLOR 		GUI_Grey
#define	  	D_KeyBoard_SELECTCOLOR 		GUI_Blue

#define			D_KeyBoard_W				40		//功能键的宽度为40
#define			D_KeyBoard_H				20		//功能键的高度为20
#define 		NOSELECT						0xFF


void GUI_KeyBoardShowInit(GUI_KeyBoardStruct *KeyBoard_P)						   //
{
	u16 i,j;	
	u16 X1,X2,Y1,Y2;	
	X1 = KeyBoard_P->X;
	X2 = KeyBoard_P->W * KeyBoard_P->X_Num;
	Y1 = KeyBoard_P->Y;
	Y2 = KeyBoard_P->H * KeyBoard_P->Y_Num;
	
	GUI_DrawFillRectangle(X1, Y1,X1+X2,Y1+Y2,KeyBoard_P->BColor);			//KeyBoard显示区域清空
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
函数：KeyBoard_Create
功能：创建KeyBoard
参数： Form_P：当前指针, X,Y :坐标,W，H：单个按键的宽度和高度； Mode：类型
返回：按钮在内存中的地址指针 如出错则返回0
********************************************/
GUI_KeyBoardStruct *GUI_KeyBoardCreate(GUI_KeyBoardInitStruct *KeyBoardInit_P)
{
	GUI_KeyBoardStruct *KeyBoard_P;	
	KeyBoard_P=(GUI_KeyBoardStruct *)GUI_MEM_Malloc(sizeof(GUI_KeyBoardStruct));	//申请KeyBoard_MEMNUM个字节的内存,前16个为系统数据，后面为显示内容
	if(KeyBoard_P)
	{
		if(GUI_FormAddChild(KeyBoardInit_P->Form_P,(u8*)KeyBoard_P))
		{
			GUI_MEM_Free(KeyBoard_P);	//释放内存
			return 0;
		}		
		KeyBoard_P->X = KeyBoardInit_P->X;										//X轴坐标
		KeyBoard_P->Y =KeyBoardInit_P->Y;											//Y轴坐标
		KeyBoard_P->W = KeyBoardInit_P->W;										//单个键的宽度
		KeyBoard_P->H =KeyBoardInit_P->H;											//单个键的高度
		KeyBoard_P->Font = &Font8x12;													//键显示的文字，这里使用默认的，如要更改单独函数更改
		KeyBoard_P->ID = GUIKeyBoardID;												//KeyBoard 的ID编号，默认不能修改
		KeyBoard_P->SelectNum = NOSELECT;											//键值
		KeyBoard_P->BColor = D_KeyBoard_BCOLOR;								//KeyBoard 的背景色，这里使用默认的，如要更改单独函数更改
		KeyBoard_P->FColor = D_KeyBoard_FCOLOR;								//KeyBoard 的文字颜色，这里使用默认的，如要更改单独函数更改
		KeyBoard_P->SelectColor = D_KeyBoard_SELECTCOLOR;			//KeyBoard 的被选中时的颜色，这里使用默认的，如要更改单独函数更改
		KeyBoard_P->X_Num = KeyBoardInit_P->X_Num;						//KeyBoard X轴键数量
		KeyBoard_P->Y_Num = KeyBoardInit_P->Y_Num;						//KeyBoard Y轴键数量
		KeyBoard_P->Char_X = KeyBoardInit_P->Char_X;					//每个键的文字对应的X轴偏移量	
		KeyBoard_P->Char_Y = KeyBoardInit_P->Char_Y;					//每个键的文字对应的Y轴偏移量				
		KeyBoard_P->StringLen = KeyBoardInit_P->StringLen;		//每个键的文字长度
		KeyBoard_P->StringMode = KeyBoardInit_P->StringMode;	//显示的文字类型，现在支持string，dec，hex
		KeyBoard_P->keyString = KeyBoardInit_P->keyString;		//文字数组指针
		KeyBoard_P->KeyID = KeyBoardInit_P->KeyID & 0xe0;			//键值的ID标示，只使用高3位。
		KeyBoard_P->State = 0;																//状态初始化
		KeyBoard_P->OsPriority = OsCurrentPriority;
		GUI_KeyBoardShowInit(KeyBoard_P);											//显示初始化	
		return KeyBoard_P;			
	}
	else
	{
	return GUI_null;		
	}
	
}

/********************************************
函数：KeyBoardShowString
功能：在显示屏上显示KeyBoard的文本
参数：  KeyBoard_P：当前指针, X1,Y1 :单个按键的坐标； Num:KeyBoard选择的按键编号
返回：无
说明：
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
函数：GUI_KeyBoardShowAll
功能：在显示屏上显示KeyBoard的所有内容
参数：  KeyBoard_P：当前指针
返回：无
说明：
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
			Num = j + KeyBoard_P->X_Num * i;		//计算当前显示的是第几个	
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
函数：KeyBoardShowNow
功能：在显示屏上显示KeyBoard
参数： KeyBoard_P KeyBoard地址指针
返回：无
说明：如内容没有改变只显示一次
********************************************/
void GUI_KeyBoardShowNow(GUI_KeyBoardStruct *KeyBoard_P)						   //
{
	if(KeyBoard_P->ID != GUIKeyBoardID) 	return ;				//不是KeyBoard的数据
	if(!GUIGetBit(KeyBoard_P->State,GUI_SHOWOK))			//控件是否显示完成
	{
		if(GUIGetBit(KeyBoard_P->State,GUI_DIS))			//控件是否是使能状态
		{	
			//ShowDIS(KeyBoard_P);
		}
		else								//非DISABLE状态
		{					
//			if(!GUIGetBit(KeyBoard_P->State,KeyBoard_SHOWALL))			//需要全部显示
//			{		
//				GUI_KeyBoardShowInit(KeyBoard_P);				
//				GUI_KeyBoardShowAll(KeyBoard_P);
//				GUISetBit(KeyBoard_P->State,KeyBoard_SHOWALL);
//			}
//			else																										//单个
//			{
				GUI_KeyBoardShowAll(KeyBoard_P);				
//			}	
		}
	}
		GUISetBit(KeyBoard_P->State,GUI_SHOWOK);			//控件显示完成	
}

/********************************************
函数：KeyBoardTouchUp
功能：菜单由触摸屏控制时，触摸屏释放时的功能
参数： KeyBoard_P KeyBoard地址指针
返回：无
说明：
********************************************/
u8 GUI_KeyBoardTouchUp(GUI_KeyBoardStruct *KeyBoard_P)
{
	u32 KeyMessage;
	if(KeyBoard_P->ID != GUIKeyBoardID)		return GUI_false;	
	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))		//不是DISABLE
	{
		if(GUIGetBit(KeyBoard_P->State,KeyBoard_TOUCH))		//按键被按下
		{	
			KeyMessage = ((u32)KeyMessageType<<24) | ((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| GUIKEY_UP<<8);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(KeyBoard_P->OsPriority, (void *)(KeyMessage));		//发送长时间按下功能键的键值
			#else
				//非OS的时候运行的程序
			#endif			
//			PostKeyMessageQueue((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| KEYOFF<<8);	//发送长时间按下功能键的键值
			GUIResetBit(KeyBoard_P->State,KeyBoard_TOUCH);					//清除功能键按下标志
			GUIResetBit(KeyBoard_P->State,KeyBoard_SHOWON);						//键盘需显示ON状态			
			KeyBoard_P->SelectNum = NOSELECT;															//清除被按下的键编号
			GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);							//控件显示有变化
			return GUI_true;			
		}		
	}	
	return GUI_false;
}

/********************************************
函数：GUI_KeyBoardTouchLong
功能：菜单由触摸屏控制时，触摸屏长时间按下时的功能
参数： KeyBoard_P KeyBoard地址指针
返回：无
说明：
********************************************/
u8 GUI_KeyBoardTouchLong(GUI_KeyBoardStruct *KeyBoard_P)
{
	u32 KeyMessage;
	if(KeyBoard_P->ID != GUIKeyBoardID)		return GUI_false;	
	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))		//不是DISABLE
	{
		if(GUIGetBit(KeyBoard_P->State,KeyBoard_TOUCH))		//按键被按下
		{
			KeyMessage = ((u32)KeyMessageType<<24) | ((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| GUIKEY_ENTER<<8);
			#if GUI_USEMSOS
			
				OS.PostMessageQueue(KeyBoard_P->OsPriority, (void *)(KeyMessage));		//发送长时间按下功能键的键值
			#else
				//非OS的时候运行的程序
			#endif					
//			PostKeyMessageQueue((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)|KEYLONG<<8);	//发送长时间按下功能键的键值
			return GUI_true;	
		}
	}
	return GUI_false;	
	
}


/********************************************
函数：GUI_KeyBoardTouchDown
功能：键盘由触摸屏控制时，触摸屏按下时的功能
参数： KeyBoard_P 地址指针；Num:键盘在FORM中的编号
返回：false	：不在键盘范围内的触摸；true ：触摸在键盘范围内
说明：
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

	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))			//控件是否是使能状态
		{										//不是DIS状态			
			if((T_X>KeyBoard_P->X)&&(T_X<X) && (T_Y>KeyBoard_P->Y)&&(T_Y<Y)) //判断是否在按钮内部
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
								KeyBoard_P->SelectNum = i*KeyBoard_P->X_Num +j;					//保存被按下的键编号
								
								KeyMessage = ((u32)KeyMessageType<<24) | ((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| GUIKEY_DOWN<<8);
								#if GUI_USEMSOS
								
									OS.PostMessageQueue(KeyBoard_P->OsPriority, (void *)(KeyMessage));		//发送长时间按下功能键的键值
								#else
									//非OS的时候运行的程序
								#endif	
								
								
//								PostKeyMessageQueue((KeyBoard_P->SelectNum | KeyBoard_P->KeyID)| KEYFIRST<<8);			//发送第一次按下功能键的键值	

								
								GUISetBit(KeyBoard_P->State,KeyBoard_TOUCH);						//功能键按下标志
								GUISetBit(KeyBoard_P->State,KeyBoard_SHOWON);						//键盘需显示ON状态
								GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);							//控件显示有变化
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


/*******************************功能按键只支持touch，不支持硬件key*************************************************/





//void KeyBoardKeyOff(GUI_KeyBoardStruct *KeyBoard_P)
//{


//}

//void KeyBoardKeyLong(GUI_KeyBoardStruct *KeyBoard_P)
//{


//}
//void KeyBoardLostFocus(void)
//{
//	if(GUI.KeyBoard_P == null)			return ;
//	if(GUI.KeyBoard_P->ID != GUIKeyBoardID) 	return ;				//不是KeyBoard的数据
//	GUIResetBit(GUI.KeyBoard_P->State,GUI_FOCUS);									//控件取消系统焦点
//	GUIResetBit(GUI.KeyBoard_P->State,KeyBoard_FOCUSIN);						//控件取消内部焦点	
//	GUI.KeyBoard_P->SelectNum =0;																	//功能键选择归零
//	GUIResetBit(GUI.KeyBoard_P->State,GUI_SHOWOK);									//控件显示有变化			
//}

//void KeyBoardGetFocus(void)
//{
//	if(GUI.KeyBoard_P == null)			return ;
//	if(GUI.KeyBoard_P->ID != GUIKeyBoardID) 	return ;				//不是KeyBoard的数据
//	GUISetBit(GUI.KeyBoard_P->State,GUI_FOCUS);										//控件获取系统焦点
//	GUIResetBit(GUI.KeyBoard_P->State,KeyBoard_FOCUSIN);						//控件取消内部焦点	
//	GUI.KeyBoard_P->SelectNum =0;																	//功能键选择归零
//	GUIResetBit(GUI.KeyBoard_P->State,GUI_SHOWOK);									//控件显示有变化			
//}

/********************************************
函数：KeyBoardUseKeyUp
功能：菜单由键盘上下选择控制时，向上选择功能
参数： KeyBoard_P KeyBoard地址指针
返回：无
说明：
********************************************/
void KeyBoardUseKeyUp(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(KeyBoard_P->SelectNum!= 0)//到了第一项
	{
		KeyBoard_P->SelectNum--;		
	}
	else
	{
		KeyBoard_P->SelectNum = KeyBoard_P->X_Num*KeyBoard_P->Y_Num;
	}
	GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);						//控件显示有变化		
}
/********************************************
函数：KeyBoardUseKeyDown
功能：菜单由键盘上下选择控制时，向下选择功能
参数： KeyBoard_P KeyBoard地址指针
返回：无
说明：
********************************************/
void KeyBoardUseKeyDown(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(KeyBoard_P->SelectNum!= KeyBoard_P->X_Num*KeyBoard_P->Y_Num)//到了第一项
	{
		KeyBoard_P->SelectNum++;		
	}
	else
	{
		KeyBoard_P->SelectNum = 0;
	}
	GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);						//控件显示有变化		 
}
/********************************************
函数：KeyBoardUseKeyEnter
功能：菜单由键盘控制时，确认按键按下功能
参数： KeyBoard_P KeyBoard地址指针
返回：无
说明：
********************************************/
void KeyBoardUseKeyEnter(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(KeyBoard_P->SelectNum == 0)				//当前选择为主控菜单项时，“back”
	{
		KeyBoard_P->SelectNum = 1;
	}
	else
	{
//		if(KeyBoard_P->RegisterFunction[KeyBoard_P->SelectNum-1] == NULL) return ;
//		(*(KeyBoard_P->RegisterFunction[KeyBoard_P->SelectNum-1]))(); 
	}
	GUIResetBit(KeyBoard_P->State,GUI_SHOWOK);						//控件显示有变化		 
}



void KeyBoardKeyFirst(GUI_KeyBoardStruct *KeyBoard_P)
{
	if(!GUIGetBit(KeyBoard_P->State,GUI_DIS))			//控件是否是使能状态
	{
		switch(GUI.KeyNum)
		{
		case GUIKEY_UP://向上
			KeyBoardUseKeyUp(KeyBoard_P);
			break;		
		case GUIKEY_DOWN://向下
			KeyBoardUseKeyDown(KeyBoard_P);
			break;				
		case GUIKEY_ENTER://Enter键
			KeyBoardUseKeyEnter(KeyBoard_P);							
			break;	
		case GUIKEY_ESC://返回键
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






