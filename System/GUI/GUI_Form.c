/*********************************************************************************
* 文件名			: gui_from.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI中的窗口控件，如果有需要显示的控件必须在窗口控件中注册才能显示							  
* 用法			：


**********************************************************************************/

#include "GUI_CONF.h" 
#include "GUI_form.h" 
#include "malloc.h"
/********************************************
函数：GUI_FormCreate
功能：创建Form
参数：
	ChildNum:子控件的数量；
	X、Y：FORM的起点坐标；
	W、H：FROM的宽度和高度；
返回：
	在内存中的地址指针 如出错则返回0
********************************************/
GUI_FormStruct *GUI_FormCreate(u8 ChildNum)
{
	u8 MemNum;	
	GUI_FormStruct *GUI_Form_P;
	if(ChildNum > FROM_MAXCHILDNUM) return 0;
	MemNum = sizeof(GUI_FormStruct) + ChildNum *4;

	GUI_Form_P =(GUI_FormStruct *)GUI_MEM_Malloc(MemNum);	//申请MemNum个字节的内存,前FROM_BESTMEMNUM个为FROM系统数据，后面ChildNum *4个为分配给子控件的指针地址
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
函数：GUI_FormAddChild
功能：Form增加子控件
参数： 
	GUI_Form_P：FROM的指针；
	Child_P：子控件的指针
返回：
	0：正常；
	1：FROM窗口指针错误；
	2：增加的子控件在FROM中已有；
	4：子控件数量超出申请的FROM子控件数量

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
	for(i=0;i<GUI_Form_P->NUM ;i++)											//删除所有子控件
	{	
		if(*GUI_Form_P->Pointer[i] == GUIFileListID)							//FILELIST控件在使用中会申请其他的内存，在删除时需要单独删除函数
		{
				#ifdef	GUI_USEFILELIST					
			GUI_FileListDelete((GUI_FileListStruct*)GUI_Form_P->Pointer[i]);
				#endif				
			
			

		}
		else
		{
		GUI_MEM_Free(GUI_Form_P->Pointer[i]);						//释放Child_P内存
	
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
函数：GUI_DeleteForm
功能：删除FROM
参数： GUI_Form_P：FROM的指针；
返回：
	0：正常；
	1：FROM窗口指针错误；
********************************************/
u8 GUI_FormDelete(GUI_FormStruct *GUI_Form_P)	
{
	GUI_FormDeleteAllChild(GUI_Form_P);
	GUI_MEM_Free(GUI_Form_P);						//释放GUI_Form_P内存
	return 0;
}
/********************************************
函数：GUI_FormDeleteChild
功能：删除FROM的子控件
参数：
	GUI_Form_P：FROM的指针；
	Child_P：子控件的指针；
	Mode：操作类型，0 只把FORM中的子控件指针删除，不删除子控件本体， 1 同时删除子控件本体
返回：
	0：正常；
	1：FROM窗口指针错误；
	2：没有找到对应的子控件；
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
			GUI_MEM_Free(Obj_P);						//释放Child_P内存
			GUI_Form_P->NUM--;			
			GUI_Form_P->Pointer[i] = GUI_Form_P->Pointer[GUI_Form_P->NUM];
			GUI_Form_P->Pointer[GUI_Form_P->NUM] = 0;
			return 0;
		}
	}		
	return 2;
}

/********************************************
函数：GUI_FormShowNow
功能：刷新FROM中的所有子控件
参数：
	GUI_Form_P：FROM的指针；
返回：
	0：正常；
	1：FROM窗口指针错误；
********************************************/
u8  GUI_FormShowNow(GUI_FormStruct *GUI_Form_P)						   //
{
	u8 i=0;	
	if(GUI_Form_P->ID != GUIFormID) 	return 1;				//不是Form的数据
//	if(GUI_Form_P->State == 1)				//需要删除所有控件并清屏
//	{
//	}
	#ifdef	GUI_USEFUNCKEY				
		if(GUI.FuncKey_P != null)		//功能键不为空
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
函数：GUI_CurrenFormGetIDNum
功能：获取控件在FROM中编号
参数：
	Obj_P：控件的指针；
返回：
	控件编号；
0xff 没有找到
********************************************/
u8  GUI_CurrenFormGetIDNum(void *Obj_P)						   //
{
	u8 i=0;	

	if(GUI.CurrentForm == NULL) 	return 0XFF;				//不是Form的数据
	
	for(i=0;i<GUI.CurrentForm->NUM ;i++)
	{
			if(GUI.CurrentForm->Pointer[i] == Obj_P) return i;
  }	
	return 0XFF;		
}



