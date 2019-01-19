/*********************************************************************************
* 文件名			: GUI_Led.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI模拟LED位图							  
* 用法			：

**********************************************************************************/
#include "GUI.h" 
#include "GUI_led.h" 
#include "GUI_DRAW2D.h"
 #include "malloc.h" 
 
/************************自己的函数定义********************************************************/


/**************宏定义******************************/
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
函数：GUI_LEDCreateAsChild
功能：创建LED
参数： Form_P：父窗口的指针； X X轴位置；	Y Y轴位置；len：LED灯显示的位数，最大为4组，每组固定为8个对应一个字节
返回：按钮在内存中的编号 如出错则返回OXFF
********************************************/
GUI_LEDStruct *GUI_LEDCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y,u8 len)
{
	GUI_LEDStruct *GUI_LED_P;

	
	if(Form_P->ID != GUIFormID) return 0;
	if(len>4) len = 4;					//长度大于4个字节就认为出错，设置为4
	GUI_LED_P=(GUI_LEDStruct *)GUI_MEM_Malloc(sizeof(GUI_LEDStruct));	//申请20个字节的内存
	
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
			GUI_MEM_Free(GUI_LED_P);	//释放内存
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
	GUIResetBit(GUI_LED_P->State,GUI_SHOWOK);			//控件需重画
}




void GUI_LEDShowNow(GUI_LEDStruct *GUI_LED_P)						   //0XFF  0x10  0x11   0x20   0x22
{

	u16 i,k;
//	u16 X1,Y1;
	u32 value;
	
	if(GUI_LED_P->ID != GUILedID) 	return ;				//不是LED的数据	
		if(!GUIGetBit(GUI_LED_P->State,GUI_SHOWOK))			//控件是否显示完成
		{
			if(GUI_LED_P->X>GUI_W-50) return;		//如果显示的宽度大于能够显示的高度，直接返回	
			if(GUI_LED_P->Y>(GUI_H-8*GUI_LED_P->Len)) return;		//如果显示的高度大于能够显示的高度，直接返回			

			if(GUIGetBit(GUI_LED_P->State,GUI_DIS))			//控件是否是使能状态
			{																						//控件未使能，显示DIS状态
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
			{																						//控件是使能状态，显示VALUE内数据		
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
			GUISetBit(GUI_LED_P->State,GUI_SHOWOK);			//控件显示完成
		}

}

