/*********************************************************************************
* 文件名			: gui_Edit.c
* 版本号    	: V2.1.0
* 日期      : 2015/09/25
* 说明      : GUI Edit控件								  
* 用法			：
 	   STATE = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----保留			
										|||| +-----保留
										|||+-------NEEDREDRAW
										||+--------FOCUSEDIT		= 如果selectLen>0 	0:只是在选择状态，1： edit状态 	
										|+---------COLORTYPE
										+----------是否为焦点状态
**********************************************************************************/
#include "GUI_edit.h" 
#include "GUI_DRAW2D.h"
#include "malloc.h"
#include "gui.h"
#include "os.h"
/************************自己的函数定义********************************************************/

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
/**************宏定义******************************/
#define	  	D_EDIT_FCOLOR 		GUI_Red
#define	  	D_EDIT_BCOLOR 		GUI_Grey




/********************************************
函数：Edit_Create
功能：创建Edit
参数： X X轴位置；	Y Y轴位置
返回：按钮在内存中的地址指针 如出错则返回0
********************************************/
GUI_EditStruct *GUI_EditCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u8 MaxLen,GUI_STRINGMODE Mode)
{
 
	GUI_EditStruct *GUI_Edit_P;
	if(Form_P->ID != GUIFormID) return 0;	
	if (MaxLen >EDIT_MAXSTRINLEN)MaxLen = EDIT_MAXSTRINLEN;
	GUI_Edit_P=(GUI_EditStruct *)GUI_MEM_Malloc(sizeof(GUI_EditStruct));	//申请内存
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
			GUI_MEM_Free(GUI_Edit_P);	//释放内存
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
函数：SetEditString
功能：设置EDIT的文本
参数： GUI_Edit_P EDIT地址指针；String 需要设置的字符串
返回：无
说明：由于显示屏宽度问题，现在只支持20个字符，1个中文算2个字符
********************************************/
u8 GUI_EditSetString(GUI_EditStruct *GUI_Edit_P,u8 *String)	
{
	u8 i;	
	if(GUI_Edit_P->ID != GUIEditID) 	return NOTGUIID;				//不是Edit的数据	
	if(GUI_Edit_P->Mode == GUI_STRING)
	{
		for (i=0;i<EDIT_MAXSTRINLEN;i++)			//先把原来的数据清零
		{	
			GUI_Edit_P->String[i] = 0x00;	
		}	
		for (i=0;i<EDIT_MAXSTRINLEN;i++)
		{
			if(String[i] == 0x00)
			{	
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//控件需重画				
				return 0;
			}
			GUI_Edit_P->String[i] = String[i];			
		}	
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//控件需重画
	return 0;	
	}
	else
	{
		return 2;
	}
}
/********************************************
函数：GUI_SetEditValue
功能：设置EDIT的数值
参数： GUI_Edit_P EDIT地址指针；value 需要设置的数值
返回：无
说明：数据存储在string中
********************************************/

u8 GUI_EditSetValue(GUI_EditStruct *GUI_Edit_P,u32 Value)	
{
	u8 i;	
	if(GUI_Edit_P->ID != GUIEditID) 	return NOTGUIID;				//不是Edit的数据	
		for (i=0;i<EDIT_MAXSTRINLEN;i++)			//先把原来的数据清零
		{	
			GUI_Edit_P->String[i] = 0x00;	
		}	
			switch(GUI_Edit_P->Mode)
			{
			case GUI_STRING://EDIT的显示模式为文字
				break;
			case GUI_DEC://EDIT的显示模式为10进制数字
				DecToString(Value,GUI_Edit_P->String,GUI_Edit_P->MaxLen);
				break;
			case GUI_HEX://EDIT的显示模式为16进制数字
				HexToString(Value,GUI_Edit_P->String,GUI_Edit_P->MaxLen);					
				break;	
			case GUI_TIME://EDIT的显示模式为10进制数字
				break;	
			case GUI_DATE://EDIT的显示模式为10进制数字				
				break;				
			default:
				break;
			}			
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//控件需重画
	return 0;
}

/********************************************
函数：GUI_SetEditValue
功能：设置EDIT的数值
参数： GUI_Edit_P EDIT地址指针；value 需要设置的数值
返回：无
说明：数据存储在string中
********************************************/

u32 GUI_EditGetValue(GUI_EditStruct *GUI_Edit_P)	
{
	u32 Value;
	if(GUI_Edit_P->ID != GUIEditID) 	return 0;				//不是Edit的数据	
	switch(GUI_Edit_P->Mode)
	{
	case GUI_STRING://EDIT的显示模式为文字
		break;
	case GUI_DEC://EDIT的显示模式为10进制数字
		Value =DecStringToHex(GUI_Edit_P->String,GUI_Edit_P->MaxLen);	
		break;
	case GUI_HEX://EDIT的显示模式为16进制数字
		Value =HexStringToHex(GUI_Edit_P->String,GUI_Edit_P->MaxLen);					
		break;	
	case GUI_TIME://EDIT的显示模式为10进制数字
		break;	
	case GUI_DATE://EDIT的显示模式为10进制数字				
		break;				
	default:
		break;
	}			
	return Value;
}

/********************************************
函数：GUI_EditShowString
功能：显示EDIT中的文本
参数： GUI_Edit_P EDIT地址指针
返回：无
说明：
********************************************/

void GUI_EditShowString(GUI_EditStruct *GUI_Edit_P)
{
	u16 i;
	u16 X,Y;
	u16 BColor,FColor;
	X = GUI_Edit_P->X+2;
	Y = GUI_Edit_P->Y+2;	
	
		for (i=0;i<GUI_Edit_P->MaxLen;i++)
		{//EDIT_STATE_COLORTYPE/*焦点选择的位置大于0 总条件*/	//不是需要EDIT
			if((i == GUI_Edit_P->FocusLen-1)&& \
				((!GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))||  \
			(GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT)&&GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE))))	//需要edit但颜色需变化
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
函数：GUI_EditSeletColor
功能：设置闪烁的被选中文本的颜色标志，并刷新控件显示标志
参数： GUI_Edit_P EDIT地址指针
返回：无
说明：1秒1个循环，每0.5秒改变1次颜色
********************************************/
void GUI_EditSeletColor(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->FocusLen > 0)		//当焦点选择大于0 且在edit状态中，每过0.5秒闪烁标志并清显示完成标志
	{
		if((OS.OSTime%1000 >500)&&(GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE)))
		{
			if(GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE))
			{
				GUIResetBit(GUI_Edit_P->State,EDIT_COLORTYPE);
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//控件显示
			}
		}
		if((OS.OSTime%1000 <500)&&(!GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE)))
		{
			if(!GUIGetBit(GUI_Edit_P->State,EDIT_COLORTYPE))
			{			
				GUISetBit(GUI_Edit_P->State,EDIT_COLORTYPE);
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//控件显示
			}
		}		
	}	
	
}

/********************************************
函数：EditShowNow
功能：在显示屏上显示EDIT的文本
参数： GUI_Edit_P EDIT地址指针
返回：无
说明：如内容没有改变只显示一次
********************************************/
void GUI_EditShowNow(GUI_EditStruct *GUI_Edit_P)						   //
{
	if(GUI_Edit_P->ID != GUIEditID) 	return ;				//不是Edit的数据
	GUI_EditSeletColor(GUI_Edit_P);			//闪烁判断
	
	if(!GUIGetBit(GUI_Edit_P->State,GUI_SHOWOK))			//控件是否显示完成
	{
		if(GUI_Edit_P->Y>(GUI_H-20)) return;		//如果显示的高度大于能够显示的高度，直接返回	
		if(GUI_Edit_P->X>(GUI_W-4-GUI_Edit_P->MaxLen*8)) return;//如果显示的宽度大于LCD宽度，直接返回	
	//画edit外框		
		if(GUIGetBit(GUI_Edit_P->State,GUI_FOCUS))		//是否焦点选择
		{
			GUI_DrawRectangle(GUI_Edit_P->X,GUI_Edit_P->Y,GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+3,GUI_Edit_P->Y+19,0xFFFF-GUI.B_Color);
		}
		else
		{
			GUI_DrawRectangle(GUI_Edit_P->X,GUI_Edit_P->Y,GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+3,GUI_Edit_P->Y+19,GUI_Black);			
		}
		GUI_DrawRectangle(GUI_Edit_P->X+1,GUI_Edit_P->Y+1,GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+2,GUI_Edit_P->Y+18,GUI_Black);		
	//显示EDIT数据
		if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))			//控件是否是使能状态
		{	
			GUI_EditShowString(GUI_Edit_P);
		}
		GUISetBit(GUI_Edit_P->State,GUI_SHOWOK);			//控件显示完成
	}
}

/********************************************
函数：GUI_EditUseKeyUp
功能：菜单由键盘上下选择控制时，向上选择功能
参数： GUI_Edit_P Edit地址指针
返回：无
说明：
********************************************/
void GUI_EditUseKeyUp(GUI_EditStruct *GUI_Edit_P)
{
	if(GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))//是否在edit状态
	{		//edit状态
		switch(GUI_Edit_P->Mode)
		{
		case GUI_STRING://EDIT的显示模式为文字
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>0x41)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<=0x5A))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]--;
			}
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x41)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]=0x5A;
			}
			break;
		case GUI_DEC://EDIT的显示模式为10进制数字
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>0x30)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<=0x39))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]--;
			}	
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x30)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]= 0x39;
			}			
			break;
		case GUI_HEX://EDIT的显示模式为16进制数字
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
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//控件显示有变化	
		
	}
	else
	{										//选择状态
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
函数：GUI_EditUseKeyDown
功能：菜单由键盘上下选择控制时，向下选择功能
参数： GUI_Edit_P Edit地址指针
返回：无
说明：
********************************************/
void GUI_EditUseKeyDown(GUI_EditStruct *GUI_Edit_P)
{
	if(GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))//是否在edit状态
	{		//edit状态
		switch(GUI_Edit_P->Mode)
		{
		case GUI_STRING://EDIT的显示模式为文字
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>=0x41)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<0x5A))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]++;
			}
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x5A)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]=0x41;
			}
			break;
		case GUI_DEC://EDIT的显示模式为10进制数字
			if((GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]>=0x30)&&(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]<0x39))
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]++;
			}	
			else if(GUI_Edit_P->String[GUI_Edit_P->FocusLen-1]==0x39)
			{
				GUI_Edit_P->String[GUI_Edit_P->FocusLen-1] = 0x30;
			}
			break;
		case GUI_HEX://EDIT的显示模式为16进制数字
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


	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//控件显示有变化	
		
	}
	else
	{//选择状态
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
函数：GUI_EditUseKeyEnter
功能：菜单由键盘控制时，确认按键按下功能
参数： GUI_Edit_P Edit地址指针
返回：无
说明：
********************************************/
void GUI_EditUseKeyEnter(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->FocusLen  == 0)			//只是焦点选择了EDIT，还没进入编辑模式
	{
		GUI_Edit_P->FocusLen = GUI_Edit_P->MaxLen;
		GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 0 进入选择状态		
	}
	else 
	{
		if(!GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))
		{//选择模式
			GUISetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 1 进入编辑模式
		}
		else
		{//编辑模式，进行数据保存
					GUI_Edit_P->FocusLen --;
			if(GUI_Edit_P->FocusLen ==0)
			{
				GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 0 进入选择状态
				GUI.CurrentForm->FocusIsSelect =GUI_false;
			}
	//	
			//save data
		}
	}	
		GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//控件显示有变化	
}
/********************************************
函数：GUI_EditEseKeyEsc
功能：菜单由键盘控制时，取消按键按下功能
参数： GUI_Edit_P Edit地址指针
返回：无
说明：
********************************************/
void GUI_EditUseKeyEsc(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->FocusLen  > 0)			//只是焦点选择了EDIT，还没进入编辑模式
	{
		if(!GUIGetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT))
		{//选择模式
			GUI_Edit_P->FocusLen = 0;
			GUI.CurrentForm->FocusIsSelect =GUI_false;
		}
		else
		{//编辑模式，进行数据保存
			GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);	// FOCUSEDIT = 0 进入选择模式
		}
	}	
		GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);						//控件显示有变化	
}

void GUI_EditKeyFirst(GUI_EditStruct *GUI_Edit_P)
{
	if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))			//控件是否是使能状态
	{
		switch(GUI.KeyNum)
		{
		case GUIKEY_UP://向上
			GUI_EditUseKeyUp(GUI_Edit_P);
			break;		
		case GUIKEY_DOWN://向下
			GUI_EditUseKeyDown(GUI_Edit_P);
			break;				
		case GUIKEY_ENTER://Enter键
			GUI_EditUseKeyEnter(GUI_Edit_P);							
			break;	
		case GUIKEY_ESC://返回键
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
	if(GUI_Edit_P->ID != GUIEditID) 	return ;				//不是Edit的数据		
	GUIResetBit(GUI_Edit_P->State,GUI_FOCUS);						//控件取消焦点
	GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);						//控件取消内部焦点	
	GUI_Edit_P->FocusLen = 0;														//控件取消内部选择
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);					//控件显示有变化		
}

void GUI_EditGetFocus(GUI_EditStruct *GUI_Edit_P)
{
	if(GUI_Edit_P->ID != GUIEditID) 	return ;				//不是Edit的数据		
	GUISetBit(GUI_Edit_P->State,GUI_FOCUS);						//控件取消焦点
	GUIResetBit(GUI_Edit_P->State,EDIT_FOCUSEDIT);						//控件取消内部焦点	
	GUI_Edit_P->FocusLen = 0;														//控件取消内部选择
	GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);					//控件显示有变化				
}
/********************************************
函数：GUI_EditTouchUp
功能：触摸屏触摸释放的时候，设置Edit的状态
参数： GUI_Edit_P: Edit地址指针；
返回：无
说明：
********************************************/
void GUI_EditTouchUp(GUI_EditStruct *GUI_Edit_P)
{
	if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))							//不是DIS状态
	{
//		GUI_EditUseKeyEsc(GUI_Edit_P);
		GUI.CurrentForm->TouchNum = 0;	
	}
}
/********************************************
函数：GUI_EditTouchDown
功能：触摸屏触摸按下的时候，设置Edit的状态
参数： GUI_Edit_P: Edit地址指针；Num：在当前的FORM中的编号
返回：无
说明：
********************************************/
u8 GUI_EditTouchDown(GUI_EditStruct *GUI_Edit_P,u8 Num)
{
	u16 X1,Y1,X2,Y2;
	X1 = GUI_Edit_P->X;
	X2 = GUI_Edit_P->X+GUI_Edit_P->MaxLen*8+2;	
	Y1 = GUI_Edit_P->Y;
	Y2 = GUI_Edit_P->Y+18;
	if(!GUIGetBit(GUI_Edit_P->State,GUI_DIS))										//按钮不是DIS状态		
		{													
			if((GUI.Touch_X >X1)&&(GUI.Touch_Y>Y1)&&(GUI.Touch_X<X2)&&(GUI.Touch_Y<Y2))		 //判断是否在按钮内部
			{	
				GUI_EditUseKeyEnter(GUI_Edit_P);
				GUIResetBit(GUI_Edit_P->State,GUI_SHOWOK);			//控件需重画
//				GUI.CurrentForm->FocusID =  Num+1;
//				GUI.CurrentForm->FocusID =  GUI_CurrenFormGetIDNum((u8*)(GUI_Edit_P));				
				GUI.CurrentForm->TouchNum = Num;
				GUI.CurrentForm->FocusIsSelect = GUI_true;
//				GUI.TouchChange = TOUCHNULL;
				GUI_FocusSelectChang(Num+1);		//变更焦点
			//增加Edit的按键控制
		
				
			return 1;
			}
		}
		return 0;
}

