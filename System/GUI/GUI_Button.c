/*********************************************************************************
* 文件名			: GUI_button.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI 按钮						  
* 用法			：
v2.1.2 2016.03.03
取消了按钮回调函数在GUI任务中运行
**********************************************************************************/
#include "GUI_Button.h" 
#include "GUI_DRAW2D.h"
 #include "malloc.h"
 #include "GUI_CONF.h" 
 #include "GUI_LCDapi.h" 
 #include "GUI.h" 



/************************自己的函数定义********************************************************/

	void GUI_ButtonLostFocus(GUI_ButtonStruct *GUI_Button_P);
	void GUI_ButtonGetFocus(GUI_ButtonStruct *GUI_Button_P);
	u8 GUI_ButtonTouchDown(GUI_ButtonStruct *GUI_Button_P,u8 Num);
	void GUI_ButtonTouchUp(GUI_ButtonStruct *GUI_Button_P);	
	void GUI_ButtonKeyFirst(GUI_ButtonStruct *GUI_Button_P);
	void GUI_ButtonKeyLong(GUI_ButtonStruct *GUI_Button_P);
	void GUI_ButtonKeyOff(GUI_ButtonStruct *GUI_Button_P);
/**************宏定义******************************/
	#define	  	D_BUTT_ONCOLOR  		GUI_Red
	#define	  	D_BUTT_OFFCOLOR 		GUI_Green
	#define	  	D_BUTT_TEXTCOLOR 		GUI_Yellow
	#define	  	D_BUTT_SHADOWCOLOR  GUI_Grey
/********************************************
函数：Button_Create
功能：创建按钮
参数：u8 Num  按钮的编号；  X,Y 按钮的X,Y轴坐标  W,H 按钮的宽度和高度 Key 按钮的键值
返回：按钮在内存中的编号
********************************************/
GUI_ButtonStruct *GUI_ButtonCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y,u16 W,u16 H)
{
	GUI_ButtonStruct *GUI_Button_P;
	if(Form_P->ID != GUIFormID) return 0;		
	GUI_Button_P=(GUI_ButtonStruct *)GUI_MEM_Malloc(sizeof(GUI_ButtonStruct));	//申请13个字节的内存
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
			GUI_MEM_Free(GUI_Button_P);	//释放内存
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
* 函数名	: GUI_ButtonRegisterFunc
* 描述	    : GUI button的回调函数
* 输入参数  : GUI_Button_P:button的指针
*           : Func: 注册回调执行函数。
* 返回参数  : 无
*******************************************************************************/
void GUI_ButtonRegisterFunc_Down(GUI_ButtonStruct *GUI_Button_P, GUI_func Func_Down)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//不是Button的数据	
	GUI_Button_P->Func_Down = Func_Down;/*回调函数*/
}
void GUI_ButtonRegisterFunc_Long(GUI_ButtonStruct *GUI_Button_P, GUI_func Func_Long)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//不是Button的数据	
	GUI_Button_P->Func_Long = Func_Long;/*回调函数*/
}
void GUI_ButtonRegisterFunc_Up(GUI_ButtonStruct *GUI_Button_P, GUI_func Func_Up)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//不是Button的数据	
	GUI_Button_P->Func_Up = Func_Up;/*回调函数*/
}


/********************************************
函数：GUI_SetButtonString
功能：设置lable的文本
参数： GUI_Lable_P lable地址指针；String 需要设置的字符串
返回：无
说明：由于显示屏宽度问题，现在只支持20个字符，1个中文算2个字符
********************************************/
void GUI_ButtonSetString(GUI_ButtonStruct *GUI_Button_P,u8 *OnString,u8* OffString)	
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//不是Button的数据
	GUI_Button_P->OnString = OnString;
	GUI_Button_P->OffString = OffString; 
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);						//控件显示有变化												
}


/********************************************
函数：ButtonShowOn
功能：显示Button ON状态
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
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
函数：ButtonShowOff
功能：显示Button OFF状态
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
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
函数：ButtonShowDIS
功能：显示Button DIS状态
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
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
				GUI_SetPoint(GUI_Button_P->X+k,GUI_Button_P->Y+i,D_BUTT_SHADOWCOLOR);//阴影颜色
			}
			else{
				GUI_SetPoint(GUI_Button_P->X+k,GUI_Button_P->Y+i,GUI.DIS_Color); //ON的颜色
			}
		}
	}
}
/********************************************
函数：GUI_ButtonShowNow
功能：Button显示刷新程序
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
********************************************/
void GUI_ButtonShowNow(GUI_ButtonStruct *GUI_Button_P)						   //0XFF  0x10  0x11   0x20   0x22
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//不是Button的数据
		if(!GUIGetBit(GUI_Button_P->State,GUI_SHOWOK))			//控件是否显示完成
		{
			if((GUI_Button_P->X + GUI_Button_P->W)>(GUI_W)) return;		//如果显示的高度大于能够显示的高度，直接返回	
			if((GUI_Button_P->Y+GUI_Button_P->H)>(GUI_H)) return;		//如果显示的高度大于能够显示的高度，直接返回		
			if(GUIGetBit(GUI_Button_P->State,GUI_DIS))			//控件是否是使能状态
			{	
				ButtonShowDIS(GUI_Button_P);
			}
			else
			{
				if(GUIGetBit(GUI_Button_P->State,GUI_ON))
				{																										//控件为ON状态
					ButtonShowOn(GUI_Button_P);
				}
				else
				{																										//控件为OFF状态
					ButtonShowOff(GUI_Button_P);
				}
			}	
			if(GUIGetBit(GUI_Button_P->State,GUI_FOCUS))			//是否焦点选择
			{
				GUI_DrawHLine(GUI_Button_P->X,GUI_Button_P->Y,GUI_Button_P->W,0xFFFF-GUI.B_Color);
				GUI_DrawHLine(GUI_Button_P->X,GUI_Button_P->Y+GUI_Button_P->H-1,GUI_Button_P->W,0xFFFF-GUI.B_Color);	
				GUI_DrawVLine(GUI_Button_P->X,GUI_Button_P->Y,GUI_Button_P->H,0xFFFF-GUI.B_Color);	
				GUI_DrawVLine(GUI_Button_P->X+GUI_Button_P->W-1,GUI_Button_P->Y,GUI_Button_P->H,0xFFFF-GUI.B_Color);		
			}			
		}
		GUISetBit(GUI_Button_P->State,GUI_SHOWOK);			//控件显示完成
}


/********************************************
函数：GUI_Button_First
功能：按钮按下后的第一次确认时，设置button的状态和运行注册的程序
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
********************************************/
void GUI_Button_Down(GUI_ButtonStruct *GUI_Button_P)
{
	GUISetBit(GUI_Button_P->State,GUI_ON);				//设置为ON状态
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);			//控件需重画		
	if(GUI_Button_P->Func_Down == NULL) return ;
	GUI_RunFunction(GUI_Button_P->OsPriority, GUI_Button_P->Func_Down);
}
/********************************************
函数：GUI_Button_Long
功能：按钮按下后的重复确认时，设置button的状态和运行注册的程序
参数： GUI_Button_P: Button地址指针；
返回：无
说明：以后可以改为自己的long注册回调函数
********************************************/
void GUI_Button_Long(GUI_ButtonStruct *GUI_Button_P) 
{
	if(GUI_Button_P->Func_Long == NULL) return ;
	GUI_RunFunction(GUI_Button_P->OsPriority, GUI_Button_P->Func_Long);	


}
/********************************************
函数：GUI_Button_UP
功能：按钮释放时，设置button的状态
参数：GUI_Button_P: Button地址指针；
返回：无
说明：以后可以改为自己的long注册回调函数
********************************************/
void GUI_Button_UP(GUI_ButtonStruct *GUI_Button_P)
{

	GUIResetBit(GUI_Button_P->State,GUI_ON);				//设置为OFF状态
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);			//控件需重画
	if(GUI_Button_P->Func_Up == NULL) return ;
	GUI_RunFunction(GUI_Button_P->OsPriority, GUI_Button_P->Func_Up);
}


/********************************************
函数：GUI_ButtonKeyFirst
功能：硬件按键按下后的第一次确认时，设置button的状态和运行注册的程序
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
********************************************/
void GUI_ButtonKeyDown(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//按钮不是DIS状态		
		if(GUIGetBit(GUI_Button_P->State,GUI_FOCUS))						//控件在焦点上
		{
			GUI_Button_Down(GUI_Button_P);		
		}
	}
}
/********************************************
函数：GUI_ButtonKeyLong
功能：硬件按键按下后的重复确认时，设置button的状态和运行注册的程序
参数： GUI_Button_P: Button地址指针；
返回：无
说明：以后可以改为自己的long注册回调函数
********************************************/
void GUI_ButtonKeyLong(GUI_ButtonStruct *GUI_Button_P) 
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//按钮不是DIS状态		
		if(GUIGetBit(GUI_Button_P->State,GUI_FOCUS))						//控件在焦点上
		{		
			GUI_Button_Long(GUI_Button_P);		

		}
	}



}

/********************************************
函数：GUI_ButtonKeyUp
功能：硬件按键释放时，设置button的状态
参数： GUI_Button_P: Button地址指针；
返回：无
说明：以后可以改为自己的long注册回调函数
********************************************/
void GUI_ButtonKeyUp(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//按钮不是DIS状态		
		GUI_Button_UP(GUI_Button_P);
	}
	
	
}




/********************************************
函数：GUI_ButtonTouchDown
功能：触摸屏触摸按下的时候，设置button的状态
参数： GUI_Button_P: Button地址指针；Num：在当前的FORM中的编号
返回：无
说明：
********************************************/
u8 GUI_ButtonTouchDown(GUI_ButtonStruct *GUI_Button_P,u8 Num)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))										//按钮不是DIS状态		
		{													
			if((GUI.Touch_X > GUI_Button_P->X)&&(GUI.Touch_Y>GUI_Button_P->Y)&&(GUI.Touch_X<(GUI_Button_P->X+GUI_Button_P->W))&&(GUI.Touch_Y<(GUI_Button_P->Y+GUI_Button_P->H)))		 //判断是否在按钮内部
			{	
				GUI.CurrentForm->TouchNum = Num;
				GUI_FocusSelectChang(Num+1);		//变更焦点				
				GUI_Button_Down(GUI_Button_P);									
			return 1;
			}
		}
		return 0;
}
/********************************************
函数：GUI_ButtonTouchUp
功能：触摸屏触摸释放的时候，设置button的状态
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
********************************************/
void GUI_ButtonTouchLong(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//按钮不是DIS状态		
		GUI_Button_Long(GUI_Button_P);
	}
}
/********************************************
函数：GUI_ButtonTouchUp
功能：触摸屏触摸释放的时候，设置button的状态
参数： GUI_Button_P: Button地址指针；
返回：无
说明：
********************************************/
void GUI_ButtonTouchUp(GUI_ButtonStruct *GUI_Button_P)
{
	if(!GUIGetBit(GUI_Button_P->State,GUI_DIS))
	{										//按钮不是DIS状态		
		GUI_Button_UP(GUI_Button_P);
	}
}



/********************************************
函数：GUI_ButtonLostFocus
功能：button失去焦点
参数： GUI_Button_P: Button地址指针；
返回：无
说明：以后可以改为自己的long注册回调函数
********************************************/

void GUI_ButtonLostFocus(GUI_ButtonStruct *GUI_Button_P)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//不是Button的数据		
	GUIResetBit(GUI_Button_P->State,GUI_FOCUS);						//控件取消焦点
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);					//控件显示有变化		
}
/********************************************
函数：GUI_ButtonGetFocus
功能：button获得焦点
参数： GUI_Button_P: Button地址指针；
返回：无
说明：以后可以改为自己的long注册回调函数
********************************************/
void GUI_ButtonGetFocus(GUI_ButtonStruct *GUI_Button_P)
{
	if(GUI_Button_P->ID != GUIButtonID) 	return ;				//不是Button的数据		
	GUISetBit(GUI_Button_P->State,GUI_FOCUS);						//控件取消焦点
	GUIResetBit(GUI_Button_P->State,GUI_SHOWOK);					//控件显示有变化				
}





