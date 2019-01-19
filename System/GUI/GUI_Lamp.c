/*********************************************************************************
* �ļ���			: GUI_LAMP.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��     	: GUI ģ��ָʾ��								  
* �÷�			:

**********************************************************************************/
//#include "GUI_LAMP.h" 
#include "GUI_DRAW2D.h"
#include "malloc.h"
#include "GUI_CONF.h" 



/**************�궨��******************************/
#define	  	D_LAMP_BCOLOR  			0x8490
#define	  	D_LAMP_ONCOLOR 			GUI_Red
#define	  	D_LAMP_OFFCOLOR 		GUI_Green
#define	  	D_LAMP_DISCOLOR 		GUI_Grey

/**************ȫ�ֱ���******************************/
static const u8 acLamp16[] = {
  0xFF, 0xC0, 0x03, 0xFF,
  0xFF, 0x2A, 0xA8, 0xFF,
  0xF0, 0xAF, 0xAA, 0x0F,
  0xF2, 0xFA, 0xAA, 0x8F,
  0xCB, 0xAA, 0xAA, 0xA3,
  0x2B, 0xAA, 0xAA, 0xA8,
  0x2E, 0xAA, 0xAA, 0xA8,
  0x2E, 0xAA, 0xAA, 0xA8,
  0x2E, 0xAA, 0xAA, 0xA8,
  0x2B, 0xAA, 0xAA, 0xA8,
  0x2A, 0xAA, 0xAA, 0xA8,
  0xCA, 0xAA, 0xAA, 0xA3,
  0xF2, 0xAA, 0xAA, 0x8F,
  0xF0, 0xAA, 0xAA, 0x0F,
  0xFF, 0x2A, 0xA8, 0xFF,
  0xFF, 0xC0, 0x03, 0xFF
};
 u16 ColorsLamp16[] = {
	0x0000,0x5555,0xAAAA,0xFFFF
};
const GUI_BITMAP bmLamp16 = {
  16, /* X���� */
  16, /* Y���� */
  2, /* ��ɫ����ɫλ�� */
  ColorsLamp16,  /* ��ɫ�� */
  acLamp16  /* λͼ���� */
};

/********************************************
������GUI_LampCreateAsChild
���ܣ�����LAMP
������Form_P�������ڵ�ָ�룻 X X��λ�ã�	Y Y��λ��
���أ���ť���ڴ��еĵ�ַָ�� ������򷵻�O
********************************************/
GUI_LampStruct *GUI_LampCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y)
{
	GUI_LampStruct *GUI_Lamp_P;
	if(Form_P->ID != GUIFormID) return 0;
	GUI_Lamp_P=(GUI_LampStruct *)GUI_MEM_Malloc(sizeof(GUI_LampStruct));	//����LAMP_MEMNUM���ֽڵ��ڴ�
	if(GUI_Lamp_P)
	{
		GUI_Lamp_P->ID = GUILampID;	
		GUI_Lamp_P->State = GUI_SHOWOK;
		GUI_Lamp_P->X = X;
		GUI_Lamp_P->Y =Y;
		GUI_Lamp_P->BColor =  D_LAMP_BCOLOR;
		GUI_Lamp_P->ONColor = D_LAMP_ONCOLOR;	
		GUI_Lamp_P->OFFColor  = D_LAMP_OFFCOLOR;	
		if(GUI_FormAddChild(Form_P,(u8*)GUI_Lamp_P))
		{//���������ӿؼ�ʧ��
			GUI_MEM_Free(GUI_Lamp_P);	//�ͷ��ڴ�
			return 0;
		}
		else
		{//���������ӿؼ��ɹ�
//			GUI_Lamp_P->Father = Form_P;
			return GUI_Lamp_P;
		}		
	}
	else
	{
	return GUI_null;		
	}
}


/********************************************
������GUI_LampSetON
���ܣ�����LAMPΪON״̬
������ GUI_Lamp_P: lamp��ַָ�룻
���أ���
˵����
********************************************/
void GUI_LampSetON(GUI_LampStruct *GUI_Lamp_P)	
{
	if(GUI_Lamp_P->ID != GUILampID) 	return ;				//����Lamp������
	GUISetBit(GUI_Lamp_P->State,GUI_ON);
	GUIResetBit(GUI_Lamp_P->State,GUI_SHOWOK);
}
/********************************************
������GUI_LampSetOFF
���ܣ�����LAMPΪOFF״̬
������ GUI_Lamp_P: lamp��ַָ�룻
���أ���
˵����
********************************************/
void GUI_LampSetOFF(GUI_LampStruct *GUI_Lamp_P)	
{
	if(GUI_Lamp_P->ID != GUILampID) 	return ;				//����Lamp������
	GUIResetBit(GUI_Lamp_P->State,GUI_ON);
	GUIResetBit(GUI_Lamp_P->State,GUI_SHOWOK);
}

















/********************************************
������GUI_LampShowNow
���ܣ�lamp��ʾˢ�³���
������ GUI_Lamp_P: lamp��ַָ�룻
���أ���
˵����
********************************************/
void GUI_LampShowNow(GUI_LampStruct *GUI_Lamp_P)						   //0XFF  0x10  0x11   0x20   0x22
{
	if(GUI_Lamp_P->ID != GUILampID) 	return ;				//����LAMP������	
		if(!GUIGetBit(GUI_Lamp_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
		{	
			if(GUI_Lamp_P->X>GUI_W-8) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���	
			if(GUI_Lamp_P->Y>(GUI_H-8)) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���		
			if(GUIGetBit(GUI_Lamp_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
			{	
				ColorsLamp16[2] = D_LAMP_DISCOLOR;
				ColorsLamp16[3] = GUI_Lamp_P->BColor;
				GUI_DrawBitmap(&bmLamp16,GUI_Lamp_P->X,GUI_Lamp_P->Y);
			}
			else
			{
				if(GUIGetBit(GUI_Lamp_P->State,GUI_ON))				
				{																				//�ؼ�ΪON״̬
				ColorsLamp16[2] = GUI_Lamp_P->ONColor;
				ColorsLamp16[3] = GUI_Lamp_P->BColor;
				GUI_DrawBitmap(&bmLamp16,GUI_Lamp_P->X,GUI_Lamp_P->Y);
				}
				else
				{																										//�ؼ�ΪOFF״̬
				ColorsLamp16[2] = GUI_Lamp_P->OFFColor;
				ColorsLamp16[3] = GUI_Lamp_P->BColor;
				GUI_DrawBitmap(&bmLamp16,GUI_Lamp_P->X,GUI_Lamp_P->Y);
				}
			}			
		}
		GUISetBit(GUI_Lamp_P->State,GUI_SHOWOK);			//�ؼ���ʾ���	
}








