/*********************************************************************************
* �ļ���			: gui_from.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI�еĴ��ڿؼ����������Ҫ��ʾ�Ŀؼ������ڴ��ڿؼ���ע�������ʾ							  
* �÷�			��


**********************************************************************************/

#include "GUI_CONF.h" 
#include "GUI_form.h" 
#include "malloc.h"
/********************************************
������GUI_FormCreate
���ܣ�����Form
������
	ChildNum:�ӿؼ���������
	X��Y��FORM��������ꣻ
	W��H��FROM�Ŀ�Ⱥ͸߶ȣ�
���أ�
	���ڴ��еĵ�ַָ�� ������򷵻�0
********************************************/
GUI_FormStruct *GUI_FormCreate(u8 ChildNum)
{
	u8 MemNum;	
	GUI_FormStruct *GUI_Form_P;
	if(ChildNum > FROM_MAXCHILDNUM) return 0;
	MemNum = sizeof(GUI_FormStruct) + ChildNum *4;

	GUI_Form_P =(GUI_FormStruct *)GUI_MEM_Malloc(MemNum);	//����MemNum���ֽڵ��ڴ�,ǰFROM_BESTMEMNUM��ΪFROMϵͳ���ݣ�����ChildNum *4��Ϊ������ӿؼ���ָ���ַ
GUI_MemSet((u8*)GUI_Form_P,0,sizeof(GUI_FormStruct));
	if(GUI_Form_P)
	{
		GUI_Form_P->ID = GUIFormID;
//		GUI_Form_P->State = 0; 
		GUI_Form_P->NUM = 0;
		GUI_Form_P->MemNum = MemNum;	
		return GUI_Form_P;		
	}
	else
	{
		return 0;
	}

}
/********************************************
������GUI_FormAddChild
���ܣ�Form�����ӿؼ�
������ 
	GUI_Form_P��FROM��ָ�룻
	Child_P���ӿؼ���ָ��
���أ�
	0��������
	1��FROM����ָ�����
	2�����ӵ��ӿؼ���FROM�����У�
	4���ӿؼ��������������FROM�ӿؼ�����

********************************************/
u8 GUI_FormAddChild(GUI_FormStruct *GUI_Form_P,u8 *Child_P)	
{
	u8 i ;	
	if(GUI_Form_P->ID != GUIFormID) return 1;
	for(i=0;i<GUI_Form_P->NUM ;i++)
	{	
		if(GUI_Form_P->Pointer[i] == Child_P) return 2;
	}
	if (GUI_Form_P->NUM >= FROM_MAXCHILDNUM)
	{		
		return 4;	
	}
	else
	{
		GUI_Form_P->Pointer[GUI_Form_P->NUM] = Child_P;
		GUI_Form_P->NUM ++;
		return 0;
	}
}
//


u8 GUI_FormDeleteAllChild(GUI_FormStruct *GUI_Form_P)
{
	u8 i=0;		
	if(GUI_Form_P->ID != GUIFormID) return 1;
	for(i=0;i<GUI_Form_P->NUM ;i++)											//ɾ�������ӿؼ�
	{	
		if(*GUI_Form_P->Pointer[i] == GUIFileListID)							//FILELIST�ؼ���ʹ���л������������ڴ棬��ɾ��ʱ��Ҫ����ɾ������
		{
				#ifdef	GUI_USEFILELIST					
			GUI_FileListDelete((GUI_FileListStruct*)GUI_Form_P->Pointer[i]);
				#endif				
			
			

		}
		else
		{
		GUI_MEM_Free(GUI_Form_P->Pointer[i]);						//�ͷ�Child_P�ڴ�
	
		}
		GUI_Form_P->Pointer[i]=0;		
	}	
	GUI_Form_P->NUM = 0;
	GUI_Form_P->FocusID = 0;
	GUI_Form_P->FocusIsSelect = 0;
	GUI_Form_P->TouchNum = 0;
	return 0;
	
}


/********************************************
������GUI_DeleteForm
���ܣ�ɾ��FROM
������ GUI_Form_P��FROM��ָ�룻
���أ�
	0��������
	1��FROM����ָ�����
********************************************/
u8 GUI_FormDelete(GUI_FormStruct *GUI_Form_P)	
{
	GUI_FormDeleteAllChild(GUI_Form_P);
	GUI_MEM_Free(GUI_Form_P);						//�ͷ�GUI_Form_P�ڴ�
	return 0;
}
/********************************************
������GUI_FormDeleteChild
���ܣ�ɾ��FROM���ӿؼ�
������
	GUI_Form_P��FROM��ָ�룻
	Child_P���ӿؼ���ָ�룻
	Mode���������ͣ�0 ֻ��FORM�е��ӿؼ�ָ��ɾ������ɾ���ӿؼ����壬 1 ͬʱɾ���ӿؼ�����
���أ�
	0��������
	1��FROM����ָ�����
	2��û���ҵ���Ӧ���ӿؼ���
********************************************/
u8 GUI_FormDeleteChild(GUI_FormStruct *GUI_Form_P,GUI_ObjStruct *Child_P)	
{
	u8 i=0;	
	GUI_ObjStruct *Obj_P;	
	if(GUI_Form_P->ID != GUIFormID) return 1;	
	for(i=0;i<GUI_Form_P->NUM ;i++)
	{	
		if(GUI_Form_P->Pointer[i]== (u8*)Child_P)
		{
			Obj_P = (GUI_ObjStruct *)GUI_Form_P->Pointer[i];
			GUI_MEM_Free(Obj_P);						//�ͷ�Child_P�ڴ�
			GUI_Form_P->NUM--;			
			GUI_Form_P->Pointer[i] = GUI_Form_P->Pointer[GUI_Form_P->NUM];
			GUI_Form_P->Pointer[GUI_Form_P->NUM] = 0;
			return 0;
		}
	}		
	return 2;
}

/********************************************
������GUI_FormShowNow
���ܣ�ˢ��FROM�е������ӿؼ�
������
	GUI_Form_P��FROM��ָ�룻
���أ�
	0��������
	1��FROM����ָ�����
********************************************/
u8  GUI_FormShowNow(GUI_FormStruct *GUI_Form_P)						   //
{
	u8 i=0;	
	if(GUI_Form_P->ID != GUIFormID) 	return 1;				//����Form������
//	if(GUI_Form_P->State == 1)				//��Ҫɾ�����пؼ�������
//	{
//	}
	#ifdef	GUI_USEFUNCKEY				
		if(GUI.FuncKey_P != null)		//���ܼ���Ϊ��
		{
				GUI_FunckeyShowNow();
		}	
	#endif			
	for(i=0;i<GUI_Form_P->NUM ;i++)
	{
			switch (*GUI_Form_P->Pointer[i])
				{
				#ifdef	GUI_USELAMP
					case GUILampID:
						GUI_LampShowNow((GUI_LampStruct*)GUI_Form_P->Pointer[i]);
						break;
				#endif
				#ifdef	GUI_USELED					
					case GUILedID:
						GUI_LEDShowNow((GUI_LEDStruct*)GUI_Form_P->Pointer[i]);								
						break;
				#endif			
				#ifdef	GUI_USELABLE					
					case GUILableID:
						GUI_LableShowNow((GUI_LableStruct*)GUI_Form_P->Pointer[i]);
						break;	
				#endif					
				#ifdef	GUI_USEBUTTON						
					case GUIButtonID:
						GUI_ButtonShowNow((GUI_ButtonStruct*)GUI_Form_P->Pointer[i]);
						break;
				#endif		
				#ifdef	GUI_USEMENU							
					case GUIMenuID:
						GUI_MenuShowNow((GUI_MenuStruct*)GUI_Form_P->Pointer[i]);
						break;
				#endif		
				#ifdef	GUI_USEEDIT	
					case GUIEditID:
						GUI_EditShowNow((GUI_EditStruct*)GUI_Form_P->Pointer[i]);
						break;
				#endif			

				#ifdef	GUI_USEKEYBOARD	
					case GUIKeyBoardID:
						GUI_KeyBoardShowNow((GUI_KeyBoardStruct*)GUI_Form_P->Pointer[i]);
						break;
				#endif			
				#ifdef	GUI_USEFILELIST	
					case GUIFileListID:
						GUI_FileListShowNow((GUI_FileListStruct*)GUI_Form_P->Pointer[i]);
						break;
				#endif							
					default :						
						break;
				}
    }	
	return 0;		
}

/********************************************
������GUI_CurrenFormGetIDNum
���ܣ���ȡ�ؼ���FROM�б��
������
	Obj_P���ؼ���ָ�룻
���أ�
	�ؼ���ţ�
0xff û���ҵ�
********************************************/
u8  GUI_CurrenFormGetIDNum(void *Obj_P)						   //
{
	u8 i=0;	

	if(GUI.CurrentForm == NULL) 	return 0XFF;				//����Form������
	
	for(i=0;i<GUI.CurrentForm->NUM ;i++)
	{
			if(GUI.CurrentForm->Pointer[i] == Obj_P) return i;
  }	
	return 0XFF;		
}



