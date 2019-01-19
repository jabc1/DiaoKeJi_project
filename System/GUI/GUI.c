/*********************************************************************************
* 文件名			: GUI.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI界面公共程序								  
* 用法			：
V3.0.0
	1.显示的API进行了调整，由原来定义在GUI_API结构体中独立出来，使得GUI和LCD的关联独立出来.
	2.调整了GUI_OS.C，使得GUI_OS作为MSOS的独立任务来运行，而不再是最低优先级任务.
	3.调整了GUI_KEY和GUI_TOUCH,使得Key和Touch与GUI独立出来，通过MSOS来传递信息而不是直接调用
V2.2.0
	调整头文件，将config文件和库文件相同单独放至用户程序处
V2.1.2	
	取消了
	void GUI_SetState(void *GUI_P,GUIStateEnum state);
	void GUI_SetXY(void *GUI_P,u16 X,u16 Y);
	void GUI_SetColor_T(void *GUI_P,u16 FColor,u16 BColor)	;	
	void GUI_SetColor_K(void *GUI_P,u16 BColor,u16 OnColor,u16 OffColor)	;	
	void GUI_SetFont(GUI_FONT *Font_P);
	函数，改由用户自己直接在结构体中修改对应的值
	
V2.1.1
	更改state的设置函数 set和reset get
V2.1.0
	更改state的定义，按位的标志进行定义
 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态										
										|||| |+----ON/OFF				= 控件状态为ON/OFF，同时lable等不需要ON/OFF的作为有效状态
										|||| +-----保留					
										|+++-------保留
										+----------是否为焦点状态
V2.0.0
	增加触摸屏控制
	
V1.1.0
	基本的控件LCD显示

	
**********************************************************************************/

#include "GUI.h" 
#include "GUI_Key.h" 
#include "GUI_Draw2d.h"
#include <math.h> 
GUI_Struct GUI;					//定义GUI的全局变量


#if GUI_USETOUCH				//如果使用触摸屏的话
	extern void GUI_Touch(void);
#endif

void GUI_Exc(void)
{
//	#if GUI_USETOUCH				//如果使用触摸屏的话
//		GUI_Touch();
//	#endif
////按键键值判断和控制
//	#if GUI_USEKEY				//如果使用按键		
//				switch (UintToByte1(GUI.KeyState))
//				{
//					case KEYFIRST:
//						GUI_KeyDown();
//						break;
//					case KEYOFF:
//						GUI_KeyOff();
//						break;
//					case KEYLONG:
//						GUI_KeyLong();
//						break;	
//					default:
//						break;
//				}	
//	#endif		
//		
//		
//	//确认当前CurrentForm是form控件
//	
//	if(GUI.CurrentForm->ID == GUIFormID)
//	{
//	GUI_FormShowNow(GUI.CurrentForm);	//刷新控件
//	}
}
#ifndef BitAddr
	//IO口操作,只对单一的IO口!确保n的值小于16!
	#define BIT_BAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
	#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
	#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BIT_BAND(addr, bitnum)) 
	#define	BitAddr(addr)   (vu32*)((((u32)(addr) & 0xF0000000)+0x2000000+((u32)(addr)<<5)));
#endif
/********************************************
函数：InitializeGUI
功能：初始化GUI
参数：无
返回：无
********************************************/
			
void InitializeGUI(void)
{	
	GUI.B_Sate_P = (GUI_State_B_Struct *)BitAddr(&(GUI.State));

	#if GUI_USEMSOS	
		GUI.CreateTask = GUICreateTask;//GUI创建任务
	#endif
	
	//GUI的一些基本参数
	GUI.B_Color = GUI_Black;											//GUI的背景色
	GUI.F_Color = GUI_Red;											//GUI的文字颜色
	GUI.DIS_Color = GUI_Grey;											//GUI的DIS控件的颜色
	GUI.KeyState = 0;													//GUI的按键状态:无按键	
	GUI.CurrentForm = NULL;										//GUI的当前窗口初始化为没有
	GUI.MemDevFlag = 0;
	//初始化GUI DRIVER的设备
	
//	DelayMs(1000);
	
	GUI_InitLcd();															//LCD初始化
	GUI_Clear(GUI.B_Color);										//清屏
	#if GUI_USETOUCH								//如果使用触摸屏
		GUI.TouchState = 0;;							//GUI的触摸屏状态
		GUI.Touch_X = 0;													//GUI的触摸屏触摸时的X坐标
		GUI.Touch_Y = 0;													//GUI的触摸屏触摸时的Y坐标
//		GUI_InitializeTouch();														//初始化触摸屏	
		
	#endif
	
	

	
	GUI.Font = &Font8x16;
}
/********************************************
函数：IsGUIID
功能：判断是否为GUI ID
参数：ID 需判断的编号
返回：是/否
********************************************/
u8 IsGUIID(u8 ID)
{
	if((ID>=MINGUIID)&&(ID<=MAXGUIID))
	{
		return GUI_true;			
	}
	else
	{			
		return GUI_false;
	}
}



/********************************************
函数：GUI_FocusSelectChang
功能：设置新的焦点
参数：NewFocusID：新的焦点控件在form中的编号
返回：无
********************************************/
void GUI_FocusSelectChang(u8 NewFocusID)
{
	if(NewFocusID != GUI.CurrentForm->FocusID)		//焦点选择有变化
	{
		if(GUI.CurrentForm->FocusID == 0)
		{
//			FunckeyLostFocus();			
		}
		else
		{
			switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1])			//原来的焦点清除
			{
			#ifdef	GUI_USEBUTTON					
				case GUIButtonID:
					GUI_ButtonLostFocus((GUI_ButtonStruct *)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);				
					break;			
			#endif		
			#ifdef	GUI_USEMENU					
				case GUIMenuID:
					GUI_MenuLostFocus((GUI_MenuStruct *)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
					break;	
			#endif		
			#ifdef	GUI_USEEDIT				
				case GUIEditID:
					GUI_EditLostFocus((GUI_EditStruct *)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
					break;		
			#endif		
			default :						
				break;
			}			
		}
		if(NewFocusID == 0)
		{
//			FunckeyGetFocus();
		}
		else
		{
			switch (*GUI.CurrentForm->Pointer[NewFocusID-1])
			{	
			#ifdef	GUI_USEBUTTON						
				case GUIButtonID:
					GUI_ButtonGetFocus((GUI_ButtonStruct *)GUI.CurrentForm->Pointer[NewFocusID-1]);				
					break;
			#endif		
			#ifdef	GUI_USEMENU					
				case GUIMenuID:		
					GUI_MenuGetFocus((GUI_MenuStruct *)GUI.CurrentForm->Pointer[NewFocusID-1]);		
					break;	
			#endif		
			#ifdef	GUI_USEEDIT					
				case GUIEditID:	
					GUI_EditGetFocus((GUI_EditStruct *)GUI.CurrentForm->Pointer[NewFocusID-1]);	
					break;
			#endif			
				default :						
					break;
			}	
		}
			GUI.CurrentForm->FocusID = NewFocusID;							//设置新焦点			
	}		
}

static const u8 HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
void HexToString(u32 hex,u8* string,u8 len)
{
	u8 i;
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;								
		*string = HextoAscii[hex%0x10];
		hex = hex/0x10;				
	}		
}
void DecToString(u32 Dec,u8* string,u8 len)
{
	u8 i;
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;								
		*string = Dec%10+0x30;		// 0x30 = "0"
		Dec = Dec/10;
	}
}

u32 DecStringToHex(u8* string,u8 len)
{
	u8 i;
	u32 hex = 0;
	
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;	
		if((*string>=0x30)&&(*string<=0x39))			//是数字0-9
		{
			hex += (*string-0x30) * pow(10,i);
		}
		else
		{
			return hex;
		}

	}	
	return hex;
}

u32 HexStringToHex(u8* string,u8 len)
{
	u8 i;
	u32 hex = 0;	
	string = string+len;
	for(i=0;i<len;i++)
	{
		string--;	
		if((*string>=0x30)&&(*string<=0x39))			//是数字0-9
		{
			hex += (*string-0x30) * pow(0x10,i);
		}
		else if((*string>=0x41)&&(*string<=0x46))			//是数字A-F
		{
			hex += (*string-0x41+10) *  pow(0x10,i);
		}
		else
		{
			return hex;
		}

	}	
	return hex;
}


/********************************************
函数：GUI_MemSet
功能：内存复制函数(因为用系统memset函数会导致莫名的死机,故全部换成这个函数)
参数：Mem_P：内存首地址;Data:要统一设置的值;Len:设置长度
返回：无
********************************************/
void GUI_MemSet(void *Mem_P,u8 Data,u32 Len)
{
	u8 *Temp_P=(u8*)Mem_P;
	while(Len)
	{
		*Temp_P=Data;
		Temp_P++;
		Len--;		
	}
}





