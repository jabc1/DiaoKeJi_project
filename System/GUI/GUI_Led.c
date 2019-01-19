/*********************************************************************************
* �ļ���			: GUI_Led.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUIģ��LEDλͼ							  
* �÷�			��

**********************************************************************************/
#include "GUI.h" 
#include "GUI_led.h" 
#include "GUI_DRAW2D.h"
 #include "malloc.h" 
 
/************************�Լ��ĺ�������********************************************************/


/**************�궨��******************************/
#define	  	D_LED_BCOLOR  			GUI_Black
#define	  	D_LED_ONCOLOR 			GUI_Red
#define	  	D_LED_OFFCOLOR 			GUI_Green
#define	  	D_LED_DISCOLOR 			GUI_Grey


static const u8 acLED6x8[] = { 0X00,0X78,0X78,0X78,0X78,0X78,0X78,0X00};

 u16 ColorsLED6x8[] = {
	0x0000,0x5555
};

const GUI_BITMAP bmLED6x8 = {
  6, /* XSize */
  8, /* YSize */
  1, /* BitsPerPixel */
  ColorsLED6x8,  /* Pointer to picture data (indices) */
  acLED6x8  /* Pointer to palette */
};
/********************************************
������GUI_LEDCreateAsChild
���ܣ�����LED
������ Form_P�������ڵ�ָ�룻 X X��λ�ã�	Y Y��λ�ã�len��LED����ʾ��λ�������Ϊ4�飬ÿ��̶�Ϊ8����Ӧһ���ֽ�
���أ���ť���ڴ��еı�� ������򷵻�OXFF
********************************************/
GUI_LEDStruct *GUI_LEDCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y,u8 len)
{
	GUI_LEDStruct *GUI_LED_P;

	
	if(Form_P->ID != GUIFormID) return 0;
	if(len>4) len = 4;					//���ȴ���4���ֽھ���Ϊ��������Ϊ4
	GUI_LED_P=(GUI_LEDStruct *)GUI_MEM_Malloc(sizeof(GUI_LEDStruct));	//����20���ֽڵ��ڴ�
	
	GUI_MemSet((u8*)GUI_LED_P,0,sizeof(GUI_LEDStruct));
	if(GUI_LED_P)
	{
		GUI_LED_P->X = X;
		GUI_LED_P->Y =Y;
		GUI_LED_P->BColor =  D_LED_BCOLOR;
		GUI_LED_P->ONColor = D_LED_ONCOLOR;	
		GUI_LED_P->OFFColor  = D_LED_OFFCOLOR;	
		GUI_LED_P->ID = GUILedID;
		GUI_LED_P->State = GUI_SHOWOK;
		GUI_LED_P->Len = len;
		GUI_LED_P->Value = 0x00;
		if(GUI_FormAddChild(Form_P,(u8*)GUI_LED_P))
		{
			GUI_MEM_Free(GUI_LED_P);	//�ͷ��ڴ�
			return 0;
		}
		else
		{
//			GUI_LED_P->Father = Form_P;
			return GUI_LED_P;
		}		
	}	
	else
	{
	return GUI_null;		
	}	


}



void GUI_LEDSetValue(GUI_LEDStruct *GUI_LED_P,u32 value)	
{
	GUI_LED_P->Value = value; 	
	GUIResetBit(GUI_LED_P->State,GUI_SHOWOK);			//�ؼ����ػ�
}




void GUI_LEDShowNow(GUI_LEDStruct *GUI_LED_P)						   //0XFF  0x10  0x11   0x20   0x22
{

	u16 i,k;
//	u16 X1,Y1;
	u32 value;
	
	if(GUI_LED_P->ID != GUILedID) 	return ;				//����LED������	
		if(!GUIGetBit(GUI_LED_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
		{
			if(GUI_LED_P->X>GUI_W-50) return;		//�����ʾ�Ŀ�ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���	
			if(GUI_LED_P->Y>(GUI_H-8*GUI_LED_P->Len)) return;		//�����ʾ�ĸ߶ȴ����ܹ���ʾ�ĸ߶ȣ�ֱ�ӷ���			

			if(GUIGetBit(GUI_LED_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
			{																						//�ؼ�δʹ�ܣ���ʾDIS״̬
				ColorsLED6x8[0] = D_LED_DISCOLOR;
				ColorsLED6x8[1] = GUI_LED_P->BColor;
				for (i = 0; i < GUI_LED_P->Len;i++)
				{
					for (k = 0; k <8;k++)				
						{
							GUI_DrawBitmap(&bmLED6x8,GUI_LED_P->X+k*6,GUI_LED_P->Y+i*8);						
						}				
				}
			}
			else
			{																						//�ؼ���ʹ��״̬����ʾVALUE������		
				for (i = 0; i < GUI_LED_P->Len;i++)
				{
					value = (GUI_LED_P->Value >>(i*8))&0xFF;
					for (k = 0; k <8;k++)				
					{
						if((value << k)&0x80)
						{
							ColorsLED6x8[1] = GUI_LED_P->ONColor;
							ColorsLED6x8[0] = GUI_LED_P->BColor;						
						}
						else
						{
							ColorsLED6x8[1] = GUI_LED_P->OFFColor;
							ColorsLED6x8[0] = GUI_LED_P->BColor;						
						}	
						if(k<4)
						{
							GUI_DrawBitmap(&bmLED6x8,GUI_LED_P->X+k*6,GUI_LED_P->Y+i*8);										
						}
						else
						{
							GUI_DrawBitmap(&bmLED6x8,GUI_LED_P->X+k*6+1,GUI_LED_P->Y+i*8);										
						}
						if(k == 4)
						{
							GUI_SetPoint(GUI_LED_P->X+24,GUI_LED_P->Y+i*8,GUI_LED_P->BColor);
							GUI_SetPoint(GUI_LED_P->X+24,GUI_LED_P->Y+i*8+7,GUI_LED_P->BColor);							
						}
					}						
				}
			}		
			GUISetBit(GUI_LED_P->State,GUI_SHOWOK);			//�ؼ���ʾ���
		}

}

