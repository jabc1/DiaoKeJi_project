/*********************************************************************************
* 文件名			: GUI_key.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI硬件按键							  
* 用法			：

**********************************************************************************/
#include "GUI_Key.h" 
#include "GUI.h" 

/**********************内部函数************************************************/
	u8 IsFocasID(u8 Num);
	void GUI_FocasSelect(void);
	void GUI_KeyFocasSelectDown(void);
	void GUI_KeyFocasSelectUp(void);
	void GUI_KeyNoFocasSelectEnter(void);
	void GUI_NoFocasSelect(void);


u8 IsFocasID(u8 Num)
{
	switch (*GUI.CurrentForm->Pointer[Num])
	{
		case GUIButtonID:
		case GUIMenuID:
		case GUIEditID:
			//是需要使用焦点的控件
		 return GUI_true;			
		case GUILampID:
		case GUILedID:							
		case GUILableID:
			//不是需要使用焦点的控件
			break;							
		default :						
			break;
	}	
	return GUI_false;
}
void GUI_KeyFocasSelectUp(void)		//--
{
	u8 i,IDNum;
	if(GUI.CurrentForm->FocusID == 0)				//原来的焦点选择为0，从最大的开始进行挑选
	{
		IDNum = GUI.CurrentForm->NUM;
	}
	else																			//从当前的select开始挑选
	{
		IDNum = GUI.CurrentForm->FocusID-1;		
	}
	for(i=IDNum;i>0;i--)
	{
			 if(IsFocasID(i-1))
			{
				GUI_FocusSelectChang(i);					//更换焦点
				GUI.CurrentForm->FocusIsSelect =GUI_false;	
				GUI.KeyState = GUI_KT_NULL;
				return;
			}					
	}	
	GUI_FocusSelectChang(0);					//更换焦点
	GUI.CurrentForm->FocusIsSelect =GUI_false;	
	GUI.KeyState = GUI_KT_NULL;
	return;		
}
void GUI_KeyFocasSelectDown(void)		//++
{
	u8 IDNum;
		for(IDNum=GUI.CurrentForm->FocusID+1;IDNum<=GUI.CurrentForm->NUM;IDNum++)
		{
				 if(IsFocasID(IDNum-1))
				{
					GUI_FocusSelectChang(IDNum);					//更换焦点
//					GUI.CurrentForm->FocusID = IDNum;
					GUI.CurrentForm->FocusIsSelect =GUI_false;	
					GUI.KeyState = GUI_KT_NULL;
					return;
				}					
		}			
		GUI_FocusSelectChang(0);					//更换焦点
//					GUI.CurrentForm->FocusID = 0;
		GUI.CurrentForm->FocusIsSelect =GUI_false;	
		GUI.KeyState = GUI_KT_NULL;	
	
}
void GUI_KeyNoFocasSelectEnter(void)
{
	if((GUI.CurrentForm->FocusID > 0)&&(GUI.CurrentForm->FocusID <GUI.CurrentForm->NUM))
	{
		switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1])
		{
			case GUILampID:
					break;			
			case GUILedID:
				break;				
			case GUILableID:
				break;					
				#ifdef	GUI_USEBUTTON					
			case GUIButtonID:
				GUI_ButtonKeyDown((GUI_ButtonStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
				break;				
				#endif	
				#ifdef	GUI_USEMENU					
			case GUIMenuID:
					GUI.CurrentForm->FocusIsSelect = GUI_true;
				break;
				#endif		
				#ifdef	GUI_USEEDIT				
			case GUIEditID:
				GUI_EditKeyFirst((GUI_EditStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
				GUI.CurrentForm->FocusIsSelect = GUI_true;
				break;		
				#endif				
			default :						
				break;
		}		
	}
	
	
}
/********************************************
函数：GUI_FocasSelect
功能：GUI有焦点控件进入编辑模式
参数：无
返回：无
说明：
跳转到焦点选择的控件对按键进行判断和处理
********************************************/
void GUI_FocasSelect(void)
{
	if((GUI.CurrentForm->FocusID > 0)&&(GUI.CurrentForm->FocusID <GUI.CurrentForm->NUM))//FocusID范围正常
	{
		switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1])		//选择控件的类型
		{
			case GUILampID:
					break;			
			case GUILedID:
				break;				
			case GUILableID:
				break;	
				#ifdef	GUI_USEBUTTON				
			case GUIButtonID:
				break;				
				#endif		
				#ifdef	GUI_USEMENU					
			case GUIMenuID:																													//菜单类型可以使用键盘
				GUI_MenuKeyFirst((GUI_MenuStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);					
				break;
				#endif		
				#ifdef	GUI_USEEDIT				
			case GUIEditID:																													//菜单类型可以使用键盘
				GUI_EditKeyFirst((GUI_EditStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);					
				break;	
				#endif				
			default :						
				break;
		}		
	}	
		GUI.KeyState = GUI_KT_NULL;		
}
/********************************************
函数：GUI_NoFocasSelect
功能：GUI没有焦点控件进入编辑模式
参数：无
返回：无
说明：
当前活动的form对按键进行判断和处理
********************************************/
void GUI_NoFocasSelect(void)
{
	switch(GUI.KeyNum)
	{
	case GUIKEY_UP://向上
		GUI_KeyFocasSelectUp();
		break;		
	case GUIKEY_DOWN://向下
		GUI_KeyFocasSelectDown();
		break;				
	case GUIKEY_ENTER://Enter键
		GUI_KeyNoFocasSelectEnter();
						
		break;	
	case GUIKEY_ESC://返回键
		GUI.CurrentForm->FocusIsSelect =GUI_false;	
	
		break;

	default:
		break;
	}	
	GUI.KeyState = GUI_KT_NULL;		
}

/********************************************
函数：GUI_KeyDown
功能：GUI接受到按键按下后的处理
参数：无
返回：无
说明：
GUI判断FocusIsSelect，就是选中的焦点控件有没有进入编辑模式，即按键是否交给焦点控件来处理
********************************************/
void GUI_KeyDown(void)
{
	if(GUI.CurrentForm->FocusIsSelect)
	{
		GUI_FocasSelect();	//按键交给焦点控件处理
	}	
	else
	{
		GUI_NoFocasSelect();	//按键由form控件来处理
	}	
}
/********************************************
函数：GUI_KeyLong
功能：GUI接受到按键长时间按下后的处理
参数：无
返回：无
说明：

********************************************/
void GUI_KeyLong(void)
{
	if((GUI.CurrentForm->FocusID != 0)&&(GUI.CurrentForm->FocusID <GUI.CurrentForm->NUM))
	{
		switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1])
		{
			case GUILampID:
					break;			
			case GUILedID:	
				break;				
			case GUILableID:
				break;			
				#ifdef	GUI_USEBUTTON				
			case GUIButtonID:
				GUI_ButtonKeyLong((GUI_ButtonStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
				break;				
				#endif					
			case GUIMenuID:
				break;
					
			default :						
				break;
		}		
	}	
					GUI.KeyState = GUI_KT_NULL;	
	
	
	
}
/********************************************
函数：GUI_KeyOff
功能：GUI接受到按键释放后的处理
参数：无
返回：无
说明：

********************************************/
void GUI_KeyUp(void)
{
	
	if((GUI.CurrentForm->FocusID != 0)&&(GUI.CurrentForm->FocusID <GUI.CurrentForm->NUM))
	{
		switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1])
		{
			case GUILampID:
					break;			
			case GUILedID:	
				break;				
			case GUILableID:
				break;						
				#ifdef	GUI_USEBUTTON				
			case GUIButtonID:
				GUI_ButtonKeyUp((GUI_ButtonStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);
				break;				
				#endif					
			case GUIMenuID:
				break;
					
			default :						
				break;
		}		
	}	
					GUI.KeyState = GUI_KT_NULL;	
	
}

