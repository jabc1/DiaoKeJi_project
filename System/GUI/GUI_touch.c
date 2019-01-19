 /*********************************************************************************
* 文件名			: GUI_touch.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI触摸屏函数							  
* 用法			：

**********************************************************************************/
#include "GUI.h" 
#include "GUI_CONF.h" 
#if GUI_USETOUCH				//如果使用触摸屏的话



/**********************内部函数*************************************************/


	
//void GUI_Touch(void)
//{
//	switch (GUI.TouchChange)
//	{
//		case TOUCHFIRST:					//第一次确认触摸屏按下
//			TouchDownTime();		
//			break;
//		case TOUCHLONG:					//确认触摸屏重复按下
//			TouchLongTime();			
//			break;
//		case TOUCHOFF:					//确认触摸屏放开
//			TouchUpTime();		
//			break;	
//		default:
//			break;
//	}	
//}



void TouchUpTime(void)
{
	#ifdef	GUI_USEFUNCKEY				
	if(GUI.FuncKey_P != null)		//功能键不为空
	{
		if(GUI_FunckeyTouchUp())return;
	}	
	#endif	
	switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum])
		{
			case GUILampID:
				break;
			case GUILedID:							
				break;
			case GUILableID:
				break;					
			#ifdef	GUI_USEBUTTON					
				case GUIButtonID:
					GUI_ButtonTouchUp((GUI_ButtonStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]);
					break;	
			#endif		
			#ifdef	GUI_USEMENU					
				case GUIMenuID:
					GUI_MenuTouchUp((GUI_MenuStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]);
					break;	
			#endif		
			#ifdef	GUI_USEEDIT				
				case GUIEditID:
					GUI_EditTouchUp((GUI_EditStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]);
					break;	
			#endif			
			#ifdef	GUI_USEKEYBOARD	
				case GUIKeyBoardID:
					GUI_KeyBoardTouchUp((GUI_KeyBoardStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]);
					break;
			#endif

			#ifdef	GUI_USEFILELIST	
				case GUIFileListID:
					GUI_FileListTouchUp((GUI_FileListStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]);
					break;
			#endif

			
			default :						
				break;
		}			
	GUI.CurrentForm->TouchNum = 0;		
}
void TouchDownTime(void)
{
	u8 i;	
	#ifdef	GUI_USEFUNCKEY				
	if(GUI.FuncKey_P != null)		//功能键不为空
	{
		if(GUI_FunckeyTouchDown())return;
	}	
	#endif		
	for(i=0;i<GUI.CurrentForm->NUM ;i++){
			switch (*GUI.CurrentForm->Pointer[i])
				{
					case GUILampID:
						break;
					case GUILedID:							
						break;
					case GUILableID:
						break;			
				#ifdef	GUI_USEBUTTON							
					case GUIButtonID:
						if(GUI_ButtonTouchDown((GUI_ButtonStruct*)GUI.CurrentForm->Pointer[i],i))return;
						break;	
				#endif		
				#ifdef	GUI_USEMENU						
					case GUIMenuID:
						if(GUI_MenuTouchDown((GUI_MenuStruct*)GUI.CurrentForm->Pointer[i],i))return;
						break;	
				#endif		
				#ifdef	GUI_USEEDIT						
					case GUIEditID:
						if(GUI_EditTouchDown((GUI_EditStruct*)GUI.CurrentForm->Pointer[i],i))return;
						break;		
				#endif		
			#ifdef	GUI_USEKEYBOARD	
				case GUIKeyBoardID:
						if(GUI_KeyBoardTouchDown((GUI_KeyBoardStruct*)GUI.CurrentForm->Pointer[i],i))return;					
					break;
			#endif				

			#ifdef	GUI_USEFILELIST	
				case GUIFileListID:
						if(GUI_FileListTouchDown((GUI_FileListStruct*)GUI.CurrentForm->Pointer[i],i))return;					
					break;
			#endif	

				
					default :						
						break;
				}		
	}
	GUI.CurrentForm->TouchNum = 0;				//没有触摸到控件内
}

void TouchLongTime(void)
{
	#ifdef	GUI_USEFUNCKEY				
	if(GUI.FuncKey_P != null)		//功能键不为空
	{
		if(GUI_FunckeyTouchLong())return;
	}	
	#endif			
	switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum])
				{
					case GUILampID:
						break;
					case GUILedID:							
						break;
					case GUILableID:
						break;						
				#ifdef	GUI_USEBUTTON							
					case GUIButtonID:						
						GUI_ButtonTouchLong((GUI_ButtonStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]);
						break;	
				#endif		
				#ifdef	GUI_USEMENU						
					case GUIMenuID:
						if(GUI_MenuTouchLong((GUI_MenuStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]))return;
						break;
				#endif		
				#ifdef	GUI_USEEDIT						
					case GUIEditID:
						break;	
				#endif									
			#ifdef	GUI_USEKEYBOARD	
				case GUIKeyBoardID:
						if(GUI_KeyBoardTouchLong((GUI_KeyBoardStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]))return;				
					break;
			#endif		
			#ifdef	GUI_USEFILELIST	
				case GUIFileListID:
						if(GUI_FileListTouchLong((GUI_FileListStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->TouchNum]))return;				
					break;
			#endif	

				
					default :						
						break;
				}			
}


#endif	


