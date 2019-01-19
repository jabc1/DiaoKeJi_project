/*********************************************************************************
* �ļ���		: page2.C
* �汾��    : V1.0.0
* ����      : 2016/02/22
* ˵��      : ͼ����ʾ����					  
* �÷�			��
* �汾˵��	��
**********************************************************************************/
#include "app.h"
#include "TFT_control.h"
#include "page.h"
#include "SD.h"
#include "DEVICE_CONF.h"
#include "NRF_ctrl.h"
#define SET_Z_MOVE_NUM   2

/********************************************
������Start
���ܣ�������̳���
��������
���أ���
********************************************/	
void Show_CN_Start(void)
{
	Page_ShowCN_Struct *GUI_P;
	GUI_P=(Page_ShowCN_Struct *)GUIKJ;
	GUI_LampSetON(GUI_P->LM_Strat);	
	GUI_LampSetOFF(GUI_P->LM_Stop);
	GUI_LampSetOFF(GUI_P->LM_Pause);
	DK.Ctrl = CTRL_START;						//��ʼ���
//	GUISetBit(GUI_P->B_TFTShow->State,GUI_DIS);			//��ʾ��ťʧЧ		��ʼ��̺����޸�״̬
//	GUISetBit(GUI_P->B_MotoRun->State,GUI_DIS);			//��̰�ťʧЧ		��ʼ��̺����޸�״̬
}
void Show_CN_Step(void)
{
	Page_ShowCN_Struct *GUI_P;
	GUI_P=(Page_ShowCN_Struct *)GUIKJ;
	GUI_LampSetOFF(GUI_P->LM_Strat);	
	GUI_LampSetOFF(GUI_P->LM_Stop);
	GUI_LampSetON(GUI_P->LM_Pause);
	if(DK.Ctrl != CTRL_STEP_A)
	{
		DK.Ctrl = CTRL_STEP_A;
	}
	else
	{
		DK.Ctrl = CTRL_STEP_B;
	}
}
void Show_CN_Pause(void)
{
	Page_ShowCN_Struct *GUI_P;
	GUI_P=(Page_ShowCN_Struct *)GUIKJ;
	GUI_LampSetOFF(GUI_P->LM_Strat);	
	GUI_LampSetOFF(GUI_P->LM_Stop);
	GUI_LampSetON(GUI_P->LM_Pause);
	DK.Ctrl = CTRL_PAUSE;

}
void Show_CN_Stop(void)
{
	Page_ShowCN_Struct *GUI_P;
	GUI_P=(Page_ShowCN_Struct *)GUIKJ;
	GUI_LampSetOFF(GUI_P->LM_Strat);	
	GUI_LampSetON(GUI_P->LM_Stop);
	GUI_LampSetOFF(GUI_P->LM_Pause);	
	DK.Ctrl = CTRL_STOP;
	GUISetBit(GUI_P->B_Start->State,GUI_DIS);			//��ʾ��ťʧЧ		��ʼ��̺����޸�״̬
	GUISetBit(GUI_P->B_Pause->State,GUI_DIS);			//��̰�ťʧЧ		��ʼ��̺����޸�״̬	
	GUISetBit(GUI_P->B_Step->State,GUI_DIS);			//��ʾ��ťʧЧ		��ʼ��̺����޸�״̬	
}

void Set_Z_Zero_Up(void)
{
//	while(Moto[AXIS_Z].Flag)   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
//	{
//		OS.DelayTimeTick(1);
//	}
	//z���ж�
//	Moto[AXIS_Z].Dir=POSITIVE;
//	Moto[AXIS_Z].Current = Moto[AXIS_Z].Current - SET_Z_MOVE_NUM;
//	Move_Z(SET_Z_MOVE_NUM);
}

void Set_Z_Zero_Dn(void)
{
//	while(Moto[AXIS_Z].Flag)   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
//	{
//		OS.DelayTimeTick(1);
//	}
//	Moto[AXIS_Z].Dir=REVERSE;
//	Moto[AXIS_Z].Current = Moto[AXIS_Z].Current + SET_Z_MOVE_NUM;
//	Move_Z(SET_Z_MOVE_NUM);
}





void Show_CN_Return(void)
{
	if(DK.File_B_P->Open)
	{		
		DK.Ctrl =CTRL_STOP;
		f_res = f_close(filescr);	//�ر��ļ�	
		DK.File = 0;
		DK.Ctrl = 0;			//ֹͣ				
//		InitializeGCodeQueue();							//��ʼ������
	}
	DK.Ctrl = CTRL_STOP;			//ֹͣ
	Send_DK_Action(NRF_KZ_MOTO_OFF);			//��������ر�
	Init_MainPage();
}
void Show_CN_COM_Online(void)
{
//	Page_ShowCN_Struct *GUI_P;
//	GUI_P=(Page_ShowCN_Struct *)GUIKJ;
//	if(GetBit(DKState,CTRL_PAUSH))
//	{
//				GUI_LampSetOFF(GUI_P->LM_Paush);
//				ResetBit(DKState,CTRL_PAUSH);		
//	}
//	else
//	{
//		SetBit(DKState,CTRL_PAUSH);		
//		GUI_LampSetON(GUI_P->LM_Paush);		
//	}		
//	SetBit(DKState,CTRL_START);	
//	GUISetBit(GUI_P->B_TFTShow->State,GUI_DIS);
//	GUISetBit(GUI_P->B_MotoRun->State,GUI_DIS);	
//	if(!GetBit(DKState, COMM_START))
//	{		
////		OS.PostMessageQueue(ComTaskPriority, (void *)((u32)UseMessage_CommConnect <<24));		
//	}	
}
void InitShow_CN(u8 Mode)
{
	Page_ShowCN_Struct *GUI_P;
	GUI_P=(Page_ShowCN_Struct *)GUIKJ;
	
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);	

	GUI_P->LED_Alarm = GUI_LEDCreateAsChild(GUI.CurrentForm,260,210,1);
	GUI_P->LED_State = GUI_LEDCreateAsChild(GUI.CurrentForm,260,222,2);

	GUI_P->B_Return = GUI_ButtonCreateAsChild(GUI.CurrentForm,270,10,50,20);
	GUI_ButtonSetString(GUI_P->B_Return,"����","����");	
	GUI_P->B_Return->Func_Down = Show_CN_Return;	
	GUI_P->B_Return->OsPriority = ShowTaskPriority;
	
	GUI_P->B_Start = GUI_ButtonCreateAsChild(GUI.CurrentForm,270,35,50,20);
	GUI_ButtonSetString(GUI_P->B_Start,"��ʼ","��ʼ");	
	if(Mode)	GUI_P->B_Start->Func_Down =Show_CN_COM_Online ;		
	else			GUI_P->B_Start->Func_Down = Show_CN_Start;		
	GUI_P->B_Start->OsPriority = ShowTaskPriority;
	
	GUI_P->B_Stop = GUI_ButtonCreateAsChild(GUI.CurrentForm,270,60,50,20);		
	GUI_ButtonSetString(GUI_P->B_Stop,"ֹͣ","ֹͣ");	
	GUI_P->B_Stop->Func_Down = Show_CN_Stop;
	GUI_P->B_Stop->OsPriority = ShowTaskPriority;
	
	GUI_P->B_Pause = GUI_ButtonCreateAsChild(GUI.CurrentForm,270,85,50,20);		
	GUI_ButtonSetString(GUI_P->B_Pause,"��ͣ","��ͣ");		
	GUI_P->B_Pause->Func_Down = Show_CN_Pause;	
	GUI_P->B_Pause->OsPriority = ShowTaskPriority;	
	
	GUI_P->B_Step = GUI_ButtonCreateAsChild(GUI.CurrentForm,270,110,50,20);		
	GUI_ButtonSetString(GUI_P->B_Step,"����","����");		
	GUI_P->B_Step->Func_Down = Show_CN_Step;					
	GUI_P->B_Step->Func_Long = Show_CN_Step;				
	GUI_P->B_Step->OsPriority = ShowTaskPriority;		
	
	GUI_P->B_Z_Up = GUI_ButtonCreateAsChild(GUI.CurrentForm,270,135,50,20);
	GUI_ButtonSetString(GUI_P->B_Z_Up,"Z+","Z+");			
	GUI_P->B_Z_Up->Func_Down = Set_Z_Zero_Up;
	GUI_P->B_Z_Up->OsPriority = ShowTaskPriority;	
	GUI_P->B_Z_Dn = GUI_ButtonCreateAsChild(GUI.CurrentForm,270,160,50,20);
	GUI_ButtonSetString(GUI_P->B_Z_Dn,"Z-","Z-");			
	GUI_P->B_Z_Dn->Func_Down = Set_Z_Zero_Dn;
	GUI_P->B_Z_Dn->OsPriority = ShowTaskPriority;		
	
	
	GUI_P->LM_Strat = GUI_LampCreateAsChild(GUI.CurrentForm,250,35);
	GUI_P->LM_Stop = GUI_LampCreateAsChild(GUI.CurrentForm,250,60);	
	GUI_P->LM_Pause = GUI_LampCreateAsChild(GUI.CurrentForm,250,85);		
	DK.Ctrl = CTRL_PAUSE;			
	GUI_LampSetOFF(GUI_P->LM_Strat);	
	GUI_LampSetOFF(GUI_P->LM_Stop);
	GUI_LampSetON(GUI_P->LM_Pause);
	while(NRF.Send == 1)	//�ȴ��ϴη������
	{
		OS.DelayTimeTick(1);
	}		
	Send_DK_Action(NRF_KZ_MOTO_ON);			//ʹ�ܲ������

//	InitShowTFT(Moto[AXIS_X].Current,Moto[AXIS_Y].Current);
}
void Page_DK_File(void)
{
	Page = PAGE_SHOWCN;
	InitShow_CN(0);
}

void Page_DK_Comm(void)
{
	Page = PAGE_SHOWCN;	
	InitShow_CN(1);	
}

void Page_ShowCN_Routine(void)
{
	Page_ShowCN_Struct *GUI_P;
	GUI_P=(Page_ShowCN_Struct *)GUIKJ;
	GUI_LEDSetValue(GUI_P->LED_Alarm,DK.Alarm);

	
}














