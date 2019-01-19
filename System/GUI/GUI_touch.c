 /*********************************************************************************
* �ļ���			: GUI_touch.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI����������							  
* �÷�			��

**********************************************************************************/
#include "GUI.h" 
#include "GUI_CONF.h" 
#if GUI_USETOUCH				//���ʹ�ô������Ļ�



/**********************�ڲ�����*************************************************/


	
//void GUI_Touch(void)
//{
//	switch (GUI.TouchChange)
//	{
//		case TOUCHFIRST:					//��һ��ȷ�ϴ���������
//			TouchDownTime();		
//			break;
//		case TOUCHLONG:					//ȷ�ϴ������ظ�����
//			TouchLongTime();			
//			break;
//		case TOUCHOFF:					//ȷ�ϴ������ſ�
//			TouchUpTime();		
//			break;	
//		default:
//			break;
//	}	
//}



void TouchUpTime(void)
{
	#ifdef	GUI_USEFUNCKEY				
	if(GUI.FuncKey_P != null)		//���ܼ���Ϊ��
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
	if(GUI.FuncKey_P != null)		//���ܼ���Ϊ��
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
	GUI.CurrentForm->TouchNum = 0;				//û�д������ؼ���
}

void TouchLongTime(void)
{
	#ifdef	GUI_USEFUNCKEY				
	if(GUI.FuncKey_P != null)		//���ܼ���Ϊ��
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


