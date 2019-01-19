/*********************************************************************************
* 文件名			: gui_lable.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI Lable控件								  
* 用法			：

**********************************************************************************/
#include "GUI_lable.h" 
#include "GUI_DRAW2D.h"
#include "GUI.h"
 #include "malloc.h"
/************************自己的函数定义********************************************************/
	

/**************宏定义******************************/

#define	  	D_LABLE_FCOLOR 		GUI_Red
#define	  	D_LABLE_BCOLOR 		GUI_Grey

/********************************************
函数：Lable_Create
功能：创建Lable
参数： X X轴位置；	Y Y轴位置
返回：按钮在内存中的地址指针 如出错则返回0
********************************************/
GUI_LableStruct *GUI_LableCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u8 MaxLen)
{
	GUI_LableStruct *GUI_Lable_P;
	if(Form_P->ID != GUIFormID) return 0;	
	if (MaxLen >LABLE_MAXSTRINGLEN)MaxLen = LABLE_MAXSTRINGLEN;
	GUI_Lable_P = (GUI_LableStruct *)GUI_MEM_Malloc(sizeof(GUI_LableStruct));	//申请LABLE_MEMMUN个字节的内存
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
			GUI_MEM_Free(GUI_Lable_P);	//释放内存
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
函数：GUI_SetLableState
功能：设置Lable的状态
参数： GUI_Lable_P Lable地址指针；state Lable的状态
返回：无
说明：
state的值应该设置
	typedef enum 
	{
		GUIState_Redraw 		= 0x01,
		GUIState_Disable 		= 0x02,
		GUIState_Enable 		= 0x03,
		GUIState_ON					= 0X04,
		GUIState_OFF				= 0X05,
	}GUIStateEnum;	
Lable没有ON/OFF状态

********************************************/
void GUI_LableSetState(GUI_LableStruct *GUI_Lable_P,GUIStateEnum state)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//不是Lable的数据
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
函数：SetLableString
功能：设置lable的文本
参数： GUI_Lable_P lable地址指针；String 需要设置的字符串
返回：无
说明：由于显示屏宽度问题，现在只支持20个字符，1个中文算2个字符
********************************************/
void GUI_LableSetString(GUI_LableStruct *GUI_Lable_P,u8 *String)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//不是Lable的数据	
	GUI_Lable_P->String = String; 
	GUI_Lable_P->Mode =  GUI_STRING;
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//控件需重画
}
/********************************************
函数：GUI_SetLableDecValue
功能：设置10进制的数值
参数： GUI_Lable_P lable地址指针；Value 需要设置的数值
返回：无
说明：Lable的显示模式为10进制数字
********************************************/
void GUI_LableSetDecValue(GUI_LableStruct *GUI_Lable_P,u32 Value)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//不是Lable的数据		
	if(GUI_Lable_P->MaxLen> LABLE_MAXVALUEGLEN) GUI_Lable_P->MaxLen = LABLE_MAXVALUEGLEN;
	GUI_Lable_P->Mode =  GUI_DEC;
	DecToString(Value,GUI_Lable_P->Value,GUI_Lable_P->MaxLen);
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//控件需重画
}
/********************************************
函数：GUI_SetLableHexValue
功能：设置16进制的数值
参数： GUI_Lable_P lable地址指针；Value 需要设置的数值
返回：无
说明：Lable的显示模式为16进制数字
********************************************/
void GUI_LableSetHexValue(GUI_LableStruct *GUI_Lable_P,u32 Value)	
{
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//不是Lable的数据		
	if(GUI_Lable_P->MaxLen> LABLE_MAXVALUEGLEN) GUI_Lable_P->MaxLen = LABLE_MAXVALUEGLEN;
	GUI_Lable_P->Mode =  GUI_HEX;
	HexToString(Value,GUI_Lable_P->Value,GUI_Lable_P->MaxLen);
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//控件需重画	
}
/********************************************
函数：GUI_SetLableDate
功能：设置年、月、日的日期
参数： GUI_Lable_P lable地址指针；Date 需要设置的日期指针
返回：无
说明：Lable只显示8位。格式为YY-MM-DD
********************************************/
void GUI_LableSetDate(GUI_LableStruct *GUI_Lable_P,u8* Date)	
{
	u8 i;
	u8 *Temp_P;
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//不是Lable的数据		
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
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//控件需重画	
}
/********************************************
函数：GUI_SetLableTime
功能：设置时、分、秒的时间
参数： GUI_Lable_P lable地址指针；Time 需要设置的时间指针
返回：无
说明：Lable只显示8位。格式为HH:MM:SS
********************************************/
void GUI_LableSetTime(GUI_LableStruct *GUI_Lable_P,u8* Time)	
{
	u8 i;
	u8 *Temp_P;
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//不是Lable的数据		
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
	GUIResetBit(GUI_Lable_P->State,GUI_SHOWOK);			//控件需重画	
}
/********************************************
函数：LableShowNow
功能：在显示屏上显示lable的文本
参数： GUI_Lable_P lable地址指针
返回：无
说明：如内容没有改变只显示一次
********************************************/
void GUI_LableShowNow(GUI_LableStruct *GUI_Lable_P)						   //
{
	u8 MaxLen;
	if(GUI_Lable_P->ID != GUILableID) 	return ;				//不是Lable的数据
	if(!GUIGetBit(GUI_Lable_P->State,GUI_SHOWOK))			//控件是否显示完成
	{
		if(GUI_Lable_P->Y>(GUI_H-16)) return;		//如果显示的高度大于能够显示的高度，直接返回	
		//允许的字符数量
		MaxLen = (GUI_W - GUI_Lable_P->X)/8;			//from中能够显示的文字长度
		if(MaxLen>GUI_Lable_P->MaxLen)									//如果能够显示的长度大于最大文字长度
		{
			MaxLen = GUI_Lable_P->MaxLen;			
		}		
		if(GUIGetBit(GUI_Lable_P->State,GUI_DIS))			//控件是否是使能状态
		{	
			
		}
		else
		{	

			switch(GUI_Lable_P->Mode)
			{
				case GUI_STRING://lable的显示模式为文字
		GUI_DrawFillRectangle(GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_Lable_P->Font->Y-1,GUI_Lable_P->BColor);
		GUI_ShowStringAt(GUI_Lable_P->String,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->FColor,GUI_Lable_P->BColor,GUI_Lable_P->Font);
					
//					GUI_PutString(GUI_Lable_P->String,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_API.Font->Y-1,0,GUI_Lable_P->FColor,GUI_Lable_P->BColor);	
					break;
				case GUI_DEC://lable的显示模式为10进制数字
				case GUI_HEX://lable的显示模式为16进制数字	
				case GUI_TIME://lable的显示模式为10进制数字
				case GUI_DATE://lable的显示模式为10进制数字				
		GUI_DrawFillRectangle(GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_Lable_P->Font->Y-1,GUI_Lable_P->BColor);
		GUI_ShowStringAt(GUI_Lable_P->Value,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->FColor,GUI_Lable_P->BColor,GUI_Lable_P->Font);
					
//					GUI_PutString(GUI_Lable_P->Value,GUI_Lable_P->X,GUI_Lable_P->Y,GUI_Lable_P->X+MaxLen*8-1,GUI_Lable_P->Y+GUI_API.Font->Y-1,0,GUI_Lable_P->FColor,GUI_Lable_P->BColor);	
					break;	
				default:
					break;
			}
		}
		GUISetBit(GUI_Lable_P->State,GUI_SHOWOK);			//控件显示完成
	}
}







