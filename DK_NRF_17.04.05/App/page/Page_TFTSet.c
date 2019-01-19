/*********************************************************************************
* �ļ���		: page0.C
* �汾��    : V1.0.0
* ����      : 2016/02/22
* ˵��      : ͼ����ʾ����					  
* �÷�			��
* �汾˵��	��
**********************************************************************************/
#include "app.h"
#include <stdlib.h>
#include "page.h"
#include "NRF_CTRL.h"
void TFTShowSizeUp(void)
{
	DK.SYS.TFTShowSize++;
	if(DK.SYS.TFTShowSize>100)
	{
		DK.SYS.TFTShowSize = 100;
	}
	GUI_DispDecAt(DK.SYS.TFTShowSize,150,100,3);	
	
}
void TFTShowSizeDn(void)
{
	if(DK.SYS.TFTShowSize<=1)
	{
		DK.SYS.TFTShowSize=1;		
	}
	else
	{
		DK.SYS.TFTShowSize--;		
	}
	GUI_DispDecAt(DK.SYS.TFTShowSize,150,100,3);		
}
void TFTShowSizeOK(void)
{	
	NRF.Send = 0;
	DK.Ctrl = CTRL_SET_TFT;
	while(NRF.Send == 0)	//�ȴ��ϴη������
	{
		OS.DelayTimeTick(1);
	}	
	Init_MainPage();	
}

void Page_SetTFTSize(void)
{
	Page_TFTSize_Struct *GUI_P;
	GUI_P=(Page_TFTSize_Struct *)GUIKJ;
	
	Page = PAGE_TFTSIZE;
	NRF.Send = 0;
	DK.Ctrl = CTRL_CX_SYS;
	while(NRF.Send == 0)	//�ȴ��ϴη������
	{
		OS.DelayTimeTick(1);
	}	
	
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);
	GUI_PutStringAt("TFT��ʾ�����޸�",90,20);	
	GUI_DispDecAt(DK.SYS.TFTShowSize,150,100,3);
	
	GUI_P->B_UP = GUI_ButtonCreateAsChild(GUI.CurrentForm,150,70,50,20);
	GUI_ButtonSetString(GUI_P->B_UP,"+","+");				
	GUI_P->B_UP->Func_Down = TFTShowSizeUp;					
	GUI_P->B_UP->Func_Long = TFTShowSizeUp;
	GUI_P->B_DN = GUI_ButtonCreateAsChild(GUI.CurrentForm,150,120,50,20);
	GUI_ButtonSetString(GUI_P->B_DN,"-","-");	
	GUI_P->B_DN->Func_Down = TFTShowSizeDn;	
	GUI_P->B_DN->Func_Long = TFTShowSizeDn;	
	
	GUI_P->B_OK = GUI_ButtonCreateAsChild(GUI.CurrentForm,90,150,50,20);	
	GUI_ButtonSetString(GUI_P->B_OK,"ȷ��","ȷ��");
	GUI_P->B_OK->Func_Down = TFTShowSizeOK;
	
	GUI_P->B_Return = GUI_ButtonCreateAsChild(GUI.CurrentForm,160,150,50,20);	
	GUI_ButtonSetString(GUI_P->B_Return,"ȡ��","ȡ��");		
	GUI_P->B_Return->Func_Down = Init_MainPage;		
}

