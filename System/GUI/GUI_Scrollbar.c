/*********************************************************************************
* 文件名			: GUI_Scrollbar.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI模拟Scrollbar位图							  
* 用法			：

**********************************************************************************/
#include "GUI_conf.h" 
#include "GUI_Scrollbar.h" 
#include "GUI_DRAW2D.h"
 #include "malloc.h" 
 
/************************自己的函数定义********************************************************/


/**************宏定义******************************/
#define	  	D_SCOROLLBAR_BCOLOR  			0xF81F
#define	  	D_SCOROLLBAR_ONCOLOR 			GUI_Red
#define	  	D_SCOROLLBAR_OFFCOLOR 		GUI_Green
#define	  	D_SCOROLLBAR_DISCOLOR 		0XE77C

u16 ColorsScrollbar[] = {
	D_SCOROLLBAR_DISCOLOR,GUI_Black
};

//const u8 ScrollSign[6][8]={
//{0x00,0x18,0x18,0x3C,0x3C,0x7E,0x7E,0x00},		//上图标  
//{0x00,0x7E,0x7E,0x3C,0x3C,0x18,0x18,0x00},		//下图标
//{0x00,0x06,0x1E,0x7E,0x7E,0x1E,0x06,0x00}, 		//左图标
//{0x00,0x60,0x78,0x7E,0x7E,0x78,0x60,0x00},		//右图标
//{0x3C,0x7E,0x66,0x42,0x42,0x66,0x7E,0x3C},		//v中间图标
//{0x00,0x7E,0xE7,0xC3,0xC3,0xE7,0x7E,0x00},	
//};
//LCD_BITMAP bmScrollbar = {
//  8, /* XSize */
//  8, /* YSize */
//  1, /* BitsPerPixel */
//  ColorsScrollbar,  /* Pointer to picture data (indices) */
////  acScrollbar_S /* Pointer to palette */
//};
const u8 ScrollSign[5][32]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x03,0xC0,0x07,0xE0,0x0F,0xF0,0x1F,0xF8,0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},		//上图标  
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFC,0x1F,0xF8,0x0F,0xF0,0x07,0xE0,0x03,0xC0,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},		//下图标
{0x00,0x00,0x00,0x00,0x04,0x00,0x06,0x00,0x07,0x00,0x07,0x80,0x07,0xC0,0x07,0xE0,0x07,0xE0,0x07,0xC0,0x07,0x80,0x07,0x00,0x06,0x00,0x04,0x00,0x00,0x00,0x00,0x00}, 		//右图标
{0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x60,0x00,0xE0,0x01,0xE0,0x03,0xE0,0x07,0xE0,0x07,0xE0,0x03,0xE0,0x01,0xE0,0x00,0xE0,0x00,0x60,0x00,0x20,0x00,0x00,0x00,0x00},		//左图标
{0x1F,0xF8,0x3F,0xFC,0x7F,0xFE,0xFF,0xFF,0xFF,0xFF,0xFE,0x7F,0xFE,0x7F,0xF8,0x1F,0xF8,0x1F,0xFE,0x7F,0xFE,0x7F,0xFF,0xFF,0xFF,0xFF,0x7F,0xFE,0x3F,0xFC,0x1F,0xF8},		//v中间图标
};





GUI_BITMAP bmScrollbar = {
  16, /* XSize */
  16, /* YSize */
  1, /* BitsPerPixel */
  ColorsScrollbar,  /* Pointer to picture data (indices) */
//  acScrollbar_S /* Pointer to palette */
};
/********************************************
函数：GUI_ScrollbarCreateAsChild
功能：创建Scrollbar
参数： Form_P：父窗口的指针； X X轴位置；	Y Y轴位置；len：Scrollbar灯显示的位数，最大为4组，每组固定为8个对应一个字节
返回：按钮在内存中的编号 如出错则返回OXFF
********************************************/
GUI_ScrollbarStruct *GUI_ScrollbarCreate(u16 X, u16 Y,u16 len,u8 Mode)
{
	GUI_ScrollbarStruct *Scrollbar_P;
	if(len < SCOROLLBAR_MIN_LEN) return GUI_null;	;					//长度小于最小长度认为出错
	Scrollbar_P=(GUI_ScrollbarStruct *)GUI_MEM_Malloc(sizeof(GUI_ScrollbarStruct));	//申请内存
	if(Scrollbar_P)
	{
		Scrollbar_P->X = X;
		Scrollbar_P->Y = Y;
		Scrollbar_P->BColor =  D_SCOROLLBAR_BCOLOR;
		Scrollbar_P->FColor	= D_SCOROLLBAR_ONCOLOR;	
		Scrollbar_P->SelectColor  = D_SCOROLLBAR_OFFCOLOR;	
//		Scrollbar_P->Min = 1;
//		Scrollbar_P->Max = 100;
//		Scrollbar_P->OldValue =0;
//		Scrollbar_P->Value =0;		
		Scrollbar_P->ID = GUIScrollbarID;
		Scrollbar_P->itemsperpage = 10;
		if(Mode)
		{//mode >0 水平
			if(len > (GUI_W-X)) len = (GUI_W-X);						//长度超出屏幕认为出错
			GUISetBit(Scrollbar_P->State,SCOROLLBAR_MODE);//设置为水平方向
		}
		else
		{//mode =0 垂直		
			if(len > (GUI_H-Y)) len = (GUI_H-Y);
			GUIResetBit(Scrollbar_P->State,SCOROLLBAR_MODE);
		}		
		Scrollbar_P->Len = len;		
		return Scrollbar_P;
	}	
	else
	{
	return GUI_null;		
	}	


}





u16 GUI_Scrollbar_GetWZ(GUI_ScrollbarStruct *Scrollbar_P)
{
	u32 i,j;
	if(((Scrollbar_P->topitem + Scrollbar_P->itemsperpage)>Scrollbar_P->totalitems) &&(Scrollbar_P->totalitems > Scrollbar_P->itemsperpage))
	{
		Scrollbar_P->topitem = Scrollbar_P->totalitems - Scrollbar_P->itemsperpage;
	}	
	i = Scrollbar_P->topitem*(Scrollbar_P->Len- SCOROLLBAR_WIDTH*2-16);
	j = i/(Scrollbar_P->totalitems-Scrollbar_P->itemsperpage);
	
	

	return j;
	
	
}

void GUI_ScrollbarShowNow(GUI_ScrollbarStruct *Scrollbar_P)						   //0XFF  0x10  0x11   0x20   0x22
{

	u16 WeiZi;
	u16 X1,Y1,LEN;
//	u32 value;
	
	if(Scrollbar_P->ID != GUIScrollbarID) 	return ;				//不是Scrollbar的数据	
		if(!GUIGetBit(Scrollbar_P->State,GUI_SHOWOK))			//控件是否显示完成
		{		
			X1 = Scrollbar_P->X;
			Y1 = Scrollbar_P->Y;
			LEN = Scrollbar_P->Len;
			if(GUIGetBit(Scrollbar_P->State,SCOROLLBAR_MODE))			//控件是水平的	
			{
				if(!GUIGetBit(Scrollbar_P->State,SCOROLLBAR_INIT))			//控件是否需要初始化显示				
				{
					GUI_DrawHLine(X1,Y1,LEN,Scrollbar_P->BColor);
					GUI_DrawHLine(X1,Y1+SCOROLLBAR_WIDTH-1,LEN,Scrollbar_P->BColor);		
					GUI_DrawVLine(X1,Y1,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);	
					GUI_DrawVLine(X1+SCOROLLBAR_WIDTH-1,Y1,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);
					GUI_DrawVLine(X1+LEN-SCOROLLBAR_WIDTH,Y1,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);
					GUI_DrawVLine(X1+LEN-1,Y1,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);
					bmScrollbar.pData = &ScrollSign[3][0];
					GUI_DrawBitmap(&bmScrollbar,X1+1,Y1+1);
					bmScrollbar.pData = &ScrollSign[2][0];
					GUI_DrawBitmap(&bmScrollbar,X1+LEN-SCOROLLBAR_WIDTH+1,Y1+1);				
				GUISetBit(Scrollbar_P->State,SCOROLLBAR_INIT);
				}				
				
					GUI_DrawFillRectangle(X1+SCOROLLBAR_WIDTH,Y1+1,X1+LEN-SCOROLLBAR_WIDTH-1,Y1+SCOROLLBAR_WIDTH-2,D_SCOROLLBAR_DISCOLOR);			
				WeiZi = GUI_Scrollbar_GetWZ(Scrollbar_P);
				bmScrollbar.pData = &ScrollSign[4][0];
				GUI_DrawBitmap(&bmScrollbar,X1+SCOROLLBAR_WIDTH+WeiZi,Y1+1);					
			}	
			else	
			{
				if(!GUIGetBit(Scrollbar_P->State,SCOROLLBAR_INIT))			//控件是否需要初始化显示				
				{
					GUI_DrawVLine(X1,Y1,LEN,Scrollbar_P->BColor);
					GUI_DrawVLine(X1+SCOROLLBAR_WIDTH-1,Y1,LEN,Scrollbar_P->BColor);		
					GUI_DrawHLine(X1,Y1,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);	
					GUI_DrawHLine(X1,Y1+SCOROLLBAR_WIDTH-1,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);
					GUI_DrawHLine(X1,Y1+LEN-SCOROLLBAR_WIDTH,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);
					GUI_DrawHLine(X1,Y1+LEN-1,SCOROLLBAR_WIDTH,Scrollbar_P->BColor);
					bmScrollbar.pData = &ScrollSign[0][0];
					GUI_DrawBitmap(&bmScrollbar,X1+1,Y1+1);
					bmScrollbar.pData = &ScrollSign[1][0];
					GUI_DrawBitmap(&bmScrollbar,X1+1,Y1+LEN-SCOROLLBAR_WIDTH+1);				

					GUISetBit(Scrollbar_P->State,SCOROLLBAR_INIT);
				}	
				GUI_DrawFillRectangle(X1+1,Y1+SCOROLLBAR_WIDTH,X1+SCOROLLBAR_WIDTH-2,Y1+LEN-SCOROLLBAR_WIDTH-1,D_SCOROLLBAR_DISCOLOR);	//滚动条区域			
				bmScrollbar.pData = &ScrollSign[4][0];				
				WeiZi = GUI_Scrollbar_GetWZ(Scrollbar_P);
				GUI_DrawBitmap(&bmScrollbar,X1+1,Y1+SCOROLLBAR_WIDTH+WeiZi);				
				
			}
//			Scrollbar_P->OldValue = Scrollbar_P->Value;



//			GUISetBit(Scrollbar_P->State,GUI_SHOWOK);			//控件显示完成
		}

}



/********************************************
函数：GUI_ScrollbarTouchDown
功能：触摸屏触摸按下的时候，设置Scrollbar的状态
参数： Scrollbar_P: Scrollbar地址指针；Num：在当前的FORM中的编号
返回：无
说明：
********************************************/
u8 GUI_ScrollbarTouchDown(GUI_ScrollbarStruct *Scrollbar_P,u16 X,u16 Y)
{
	u16 X1,Y1,X2,Y2;	
	X1 = Scrollbar_P->X;
	Y1 = Scrollbar_P->Y;	
	
	if(GUIGetBit(Scrollbar_P->State,SCOROLLBAR_MODE))			//水平
	{
		X2 = Scrollbar_P->X +Scrollbar_P->Len-1;//条目显示结束x坐标
		Y2 = Scrollbar_P->Y + SCOROLLBAR_WIDTH-1;
		if((Y>Y1)&&(Y<Y2))
		{
			if((X>X1)&&(X<X1+SCOROLLBAR_WIDTH))
			{
				if(Scrollbar_P->topitem>0) Scrollbar_P->topitem--;				
				return GUI_true;				
			}
			else if((X>X2 -SCOROLLBAR_WIDTH )&&(X<X2))
			{
				if(Scrollbar_P->topitem < (Scrollbar_P->totalitems - Scrollbar_P->itemsperpage)) Scrollbar_P->topitem++;				
				return GUI_true;					
			}		
		}		
		
		
	}
	else		//垂直

	{

		X2 = Scrollbar_P->X + SCOROLLBAR_WIDTH-1;//条目显示结束x坐标
		Y2 = Scrollbar_P->Y +Scrollbar_P->Len-1;
		if((X>X1)&&(X<X2))
		{
			if((Y>Y1)&&(Y<Y1+SCOROLLBAR_WIDTH))
			{
				if(Scrollbar_P->topitem>0) Scrollbar_P->topitem--;				
				return GUI_true;				
			}
			else if((Y>Y2 -SCOROLLBAR_WIDTH )&&(Y<Y2))
			{
				if(Scrollbar_P->topitem < (Scrollbar_P->totalitems - Scrollbar_P->itemsperpage)) Scrollbar_P->topitem++;				
				return GUI_true;					
			}		
		}
	}
	return GUI_false;			
}














