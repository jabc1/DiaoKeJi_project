/*********************************************************************************
* 文件名			: GUI_LAMP.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明     	: GUI 模拟指示灯								  
* 用法			:

**********************************************************************************/
//#include "GUI_LAMP.h" 
#include "GUI_DRAW2D.h"
#include "malloc.h"
#include "GUI_CONF.h" 



/**************宏定义******************************/
#define	  	D_LAMP_BCOLOR  			0x8490
#define	  	D_LAMP_ONCOLOR 			GUI_Red
#define	  	D_LAMP_OFFCOLOR 		GUI_Green
#define	  	D_LAMP_DISCOLOR 		GUI_Grey

/**************全局变量******************************/
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
  16, /* X轴宽度 */
  16, /* Y轴宽度 */
  2, /* 调色板颜色位数 */
  ColorsLamp16,  /* 调色板 */
  acLamp16  /* 位图数据 */
};

/********************************************
函数：GUI_LampCreateAsChild
功能：创建LAMP
参数：Form_P：父窗口的指针； X X轴位置；	Y Y轴位置
返回：按钮在内存中的地址指针 如出错则返回O
********************************************/
GUI_LampStruct *GUI_LampCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y)
{
	GUI_LampStruct *GUI_Lamp_P;
	if(Form_P->ID != GUIFormID) return 0;
	GUI_Lamp_P=(GUI_LampStruct *)GUI_MEM_Malloc(sizeof(GUI_LampStruct));	//申请LAMP_MEMNUM个字节的内存
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
		{//父窗口增加控件失败
			GUI_MEM_Free(GUI_Lamp_P);	//释放内存
			return 0;
		}
		else
		{//父窗口增加控件成功
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
函数：GUI_LampSetON
功能：设置LAMP为ON状态
参数： GUI_Lamp_P: lamp地址指针；
返回：无
说明：
********************************************/
void GUI_LampSetON(GUI_LampStruct *GUI_Lamp_P)	
{
	if(GUI_Lamp_P->ID != GUILampID) 	return ;				//不是Lamp的数据
	GUISetBit(GUI_Lamp_P->State,GUI_ON);
	GUIResetBit(GUI_Lamp_P->State,GUI_SHOWOK);
}
/********************************************
函数：GUI_LampSetOFF
功能：设置LAMP为OFF状态
参数： GUI_Lamp_P: lamp地址指针；
返回：无
说明：
********************************************/
void GUI_LampSetOFF(GUI_LampStruct *GUI_Lamp_P)	
{
	if(GUI_Lamp_P->ID != GUILampID) 	return ;				//不是Lamp的数据
	GUIResetBit(GUI_Lamp_P->State,GUI_ON);
	GUIResetBit(GUI_Lamp_P->State,GUI_SHOWOK);
}

















/********************************************
函数：GUI_LampShowNow
功能：lamp显示刷新程序
参数： GUI_Lamp_P: lamp地址指针；
返回：无
说明：
********************************************/
void GUI_LampShowNow(GUI_LampStruct *GUI_Lamp_P)						   //0XFF  0x10  0x11   0x20   0x22
{
	if(GUI_Lamp_P->ID != GUILampID) 	return ;				//不是LAMP的数据	
		if(!GUIGetBit(GUI_Lamp_P->State,GUI_SHOWOK))			//控件是否显示完成
		{	
			if(GUI_Lamp_P->X>GUI_W-8) return;		//如果显示的高度大于能够显示的高度，直接返回	
			if(GUI_Lamp_P->Y>(GUI_H-8)) return;		//如果显示的高度大于能够显示的高度，直接返回		
			if(GUIGetBit(GUI_Lamp_P->State,GUI_DIS))			//控件是否是使能状态
			{	
				ColorsLamp16[2] = D_LAMP_DISCOLOR;
				ColorsLamp16[3] = GUI_Lamp_P->BColor;
				GUI_DrawBitmap(&bmLamp16,GUI_Lamp_P->X,GUI_Lamp_P->Y);
			}
			else
			{
				if(GUIGetBit(GUI_Lamp_P->State,GUI_ON))				
				{																				//控件为ON状态
				ColorsLamp16[2] = GUI_Lamp_P->ONColor;
				ColorsLamp16[3] = GUI_Lamp_P->BColor;
				GUI_DrawBitmap(&bmLamp16,GUI_Lamp_P->X,GUI_Lamp_P->Y);
				}
				else
				{																										//控件为OFF状态
				ColorsLamp16[2] = GUI_Lamp_P->OFFColor;
				ColorsLamp16[3] = GUI_Lamp_P->BColor;
				GUI_DrawBitmap(&bmLamp16,GUI_Lamp_P->X,GUI_Lamp_P->Y);
				}
			}			
		}
		GUISetBit(GUI_Lamp_P->State,GUI_SHOWOK);			//控件显示完成	
}








