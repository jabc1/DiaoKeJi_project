/*********************************************************************************
* �ļ���			: GUI_key.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUIӲ������							  
* �÷�			��

**********************************************************************************/
#include "GUI_Key.h" 
#include "GUI.h" 

/**********************�ڲ�����************************************************/
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
			//����Ҫʹ�ý���Ŀؼ�
		 return GUI_true;			
		case GUILampID:
		case GUILedID:							
		case GUILableID:
			//������Ҫʹ�ý���Ŀؼ�
			break;							
		default :						
			break;
	}	
	return GUI_false;
}
void GUI_KeyFocasSelectUp(void)		//--
{
	u8 i,IDNum;
	if(GUI.CurrentForm->FocusID == 0)				//ԭ���Ľ���ѡ��Ϊ0�������Ŀ�ʼ������ѡ
	{
		IDNum = GUI.CurrentForm->NUM;
	}
	else																			//�ӵ�ǰ��select��ʼ��ѡ
	{
		IDNum = GUI.CurrentForm->FocusID-1;		
	}
	for(i=IDNum;i>0;i--)
	{
			 if(IsFocasID(i-1))
			{
				GUI_FocusSelectChang(i);					//��������
				GUI.CurrentForm->FocusIsSelect =GUI_false;	
				GUI.KeyState = GUI_KT_NULL;
				return;
			}					
	}	
	GUI_FocusSelectChang(0);					//��������
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
					GUI_FocusSelectChang(IDNum);					//��������
//					GUI.CurrentForm->FocusID = IDNum;
					GUI.CurrentForm->FocusIsSelect =GUI_false;	
					GUI.KeyState = GUI_KT_NULL;
					return;
				}					
		}			
		GUI_FocusSelectChang(0);					//��������
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
������GUI_FocasSelect
���ܣ�GUI�н���ؼ�����༭ģʽ
��������
���أ���
˵����
��ת������ѡ��Ŀؼ��԰��������жϺʹ���
********************************************/
void GUI_FocasSelect(void)
{
	if((GUI.CurrentForm->FocusID > 0)&&(GUI.CurrentForm->FocusID <GUI.CurrentForm->NUM))//FocusID��Χ����
	{
		switch (*GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1])		//ѡ��ؼ�������
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
			case GUIMenuID:																													//�˵����Ϳ���ʹ�ü���
				GUI_MenuKeyFirst((GUI_MenuStruct*)GUI.CurrentForm->Pointer[GUI.CurrentForm->FocusID-1]);					
				break;
				#endif		
				#ifdef	GUI_USEEDIT				
			case GUIEditID:																													//�˵����Ϳ���ʹ�ü���
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
������GUI_NoFocasSelect
���ܣ�GUIû�н���ؼ�����༭ģʽ
��������
���أ���
˵����
��ǰ���form�԰��������жϺʹ���
********************************************/
void GUI_NoFocasSelect(void)
{
	switch(GUI.KeyNum)
	{
	case GUIKEY_UP://����
		GUI_KeyFocasSelectUp();
		break;		
	case GUIKEY_DOWN://����
		GUI_KeyFocasSelectDown();
		break;				
	case GUIKEY_ENTER://Enter��
		GUI_KeyNoFocasSelectEnter();
						
		break;	
	case GUIKEY_ESC://���ؼ�
		GUI.CurrentForm->FocusIsSelect =GUI_false;	
	
		break;

	default:
		break;
	}	
	GUI.KeyState = GUI_KT_NULL;		
}

/********************************************
������GUI_KeyDown
���ܣ�GUI���ܵ��������º�Ĵ���
��������
���أ���
˵����
GUI�ж�FocusIsSelect������ѡ�еĽ���ؼ���û�н���༭ģʽ���������Ƿ񽻸�����ؼ�������
********************************************/
void GUI_KeyDown(void)
{
	if(GUI.CurrentForm->FocusIsSelect)
	{
		GUI_FocasSelect();	//������������ؼ�����
	}	
	else
	{
		GUI_NoFocasSelect();	//������form�ؼ�������
	}	
}
/********************************************
������GUI_KeyLong
���ܣ�GUI���ܵ�������ʱ�䰴�º�Ĵ���
��������
���أ���
˵����

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
������GUI_KeyOff
���ܣ�GUI���ܵ������ͷź�Ĵ���
��������
���أ���
˵����

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

