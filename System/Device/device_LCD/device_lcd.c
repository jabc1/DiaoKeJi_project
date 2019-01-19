/*********************************************************************************
* 文件名			: device_lcd.c
* 版本号    	: V3.0.0
* 日期      : 2016/01/26
* 说明      : lcd界面一些绘图API							  
* 用法			：

**********************************************************************************/

  
#include "device_conf.h" 
#include "malloc.h"

#if(LCD_USE	== LCD_USE_16B)	
	#include "device_lcd_16b.c"
#endif		
#if(LCD_USE	== LCD_USE_FMSC)			
	#include "device_lcd_fmsc.c"	
#endif	
#if(LCD_USE	== LCD_USE_SPI)			
	#include "device_lcd_spi.c"	
#endif	

//	void LCD_PutGBChar(u8 *Char,u16 x,u16 y,u16 FColor,u16 BColor,FONT_Struct *Font)	
//	{
//		u8 size = Font->GB_X; 
//  u16 i;
//  u16 j;  
//  u32 tmp_char;
//		u8 GBX_BYTE = Font->GB_Num/size;//得到字体一个字符对应点阵集所占的字节数			
////		u8 GBFont[72];
//		u8* GBFont_P;	
//		if(size!=12&&size!=16&&size!=24)return ;	//不支持的size	
//		GBFont_P=Get_HzMat(Char,Font);
//		if(GBFont_P==0)return ;	//得到相应大小的点阵数据 
//		LCD_LOCK();
//		LCD_SetWindows(x,y,x+Font->GB_X-1,y+Font->Y-1);
//		LCD_BeginWriteRAM(); 		
//		for (i=0;i< Font->Y;i++)
//		{
//					UintToByte3(tmp_char)=GBFont_P[i*GBX_BYTE];
//					UintToByte2(tmp_char)=GBFont_P[1+i*GBX_BYTE];
//					UintToByte1(tmp_char)=GBFont_P[2+i*GBX_BYTE];		
//			for (j=0;j<Font->GB_X;j++)
//			{
//				if (((tmp_char >> (31-j)) & 0x01) == 0x01)
//				{
//					LCD_OnlyWriteRAM(FColor); // 字符颜色
//				}					
//				else	
//				{
//					LCD_OnlyWriteRAM(BColor); // 背景颜色
//				}					
//			}
//		}
//		LCD_EndWriteRAM();	
//		LCD_UNLOCK();		
//	}			


//	
//	
//	






///****************************************************************************
//* 名    称：void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
//* 功    能：在指定座标显示一个8x16点阵的ascii字符
//* 入口参数：x          行座标
//*           y          列座标
//*           charColor  字符的颜色
//*           bkColor    字符背景颜色
//* 出口参数：无
//* 说    明：显示范围限定为可显示的ascii码
//* 调用方法：LCD_PutChar(10,10,'a');
//****************************************************************************/
//void LCD_PutChar(u8 *c,u16 x,u16 y,u16 FColor,u16 BColor,FONT_Struct *Font)
//{
//  u16 i;
//  u16 j;  
//  u32 tmp_char;
//	u8 ASCX_BYTE = Font->ASC_Num/Font->Y;
//	u8 const *ASCFont;	
//	i= (*c-0x20);
//	i = i*(Font->ASC_Num);
//	ASCFont = &Font->Font_P[i];
////	ASCFont = &(Font->Font_P[((c-0x20)*Font->ASC_Num)]);
//	LCD_LOCK();
//	
//	#if LCDAPIUSEFAST > 0	
//		LCD_SetWindows(x,y,x+Font->ASC_X-1,y+Font->Y-1);
//		LCD_BeginWriteRAM(); 
//	#endif	
//		for (i=0;i< Font->Y;i++)
//		{
//			if(Font->ASC_X<=8)
//			{
//				tmp_char=ASCFont[i*ASCX_BYTE];		
//				for (j=0;j<Font->ASC_X;j++)
//				{
//					if (((tmp_char >> (7-j)) & 0x01) == 0x01)
//					{
//						#if LCDAPIUSEFAST > 0	
//							LCD_OnlyWriteRAM(FColor); // 字符颜色
//						#else
//							LCD_SetPoint(j+x,i+y,FColor); // 字符颜色
//						#endif
//					}					
//					else	
//					{
//						#if LCDAPIUSEFAST > 0
//							LCD_OnlyWriteRAM(BColor); // 背景颜色
//						#else
//							LCD_SetPoint(j+x,i+y,BColor); // 背景颜色
//						#endif
//					}					
//				}				
//			}
//			else
//			{
//				tmp_char = *(u16*)(&ASCFont[i*ASCX_BYTE]);		
//				for (j=0;j<Font->ASC_X;j++)
//				{
//					if (((tmp_char >> j) & 0x01) == 0x01)
//					{
//						#if LCDAPIUSEFAST > 0	
//							LCD_OnlyWriteRAM(FColor); // 字符颜色
//						#else
//							LCD_SetPoint(j+x,i+y,FColor); // 字符颜色
//						#endif
//					}					
//					else	
//					{
//						#if LCDAPIUSEFAST > 0
//							LCD_OnlyWriteRAM(BColor); // 背景颜色
//						#else
//							LCD_SetPoint(j+x,i+y,BColor); // 背景颜色
//						#endif
//					}					
//				}
//				
//			}
//		}
//	#if LCDAPIUSEFAST > 0
//		LCD_EndWriteRAM();	
//	#endif		
//	LCD_UNLOCK();
//}
///****************************************************************************
//* 名    称：LCD_Clear
//* 功    能：将屏幕填充成指定的颜色
//* 入口参数：Color      填充值
//* 出口参数：无
//* 说    明：
//* 调用方法：LCD_Clear(0xffff);
//****************************************************************************/
//void LCD_Clear(u16 Color)
//{
//	u16 i,j;
//	LCD_LOCK();
//	#if LCDAPIUSEFAST > 0
//		LCD_SetWindows(0,0,LCD_W-1,LCD_H-1);
//		LCD_BeginWriteRAM(); 
//		for(i=0;i<LCD_W;i++)
//		{
//			for (j=0;j<LCD_H;j++)
//			{
//				LCD_OnlyWriteRAM(Color);
//			}
//		}
//			LCD_EndWriteRAM();
//	#else
//	LCD_SetWindows(0,0,LCD_W-1,LCD_H-1);		
//	for(i=0;i<LCD_W;i++)
//	{
//		for (j=0;j<LCD_H;j++)
//		{
//			LCD_SetPoint(j,i,Color);
//		}
//	}			
//	#endif
//	LCD_UNLOCK();
//}

//void LCD_DrawHLine(u16 X, u16 Y,u16 Len, u16 FColor)
//{
//	u16 i;
//	LCD_LOCK();
//	for(i=0;i<Len;i++)
//	{
//		LCD_SetPoint(X+i,Y,FColor);//画点 
//	}
//	LCD_UNLOCK();
//}
//void LCD_DrawVLine( u16 X, u16 Y, u16 Len,u16 FColor)
//{
//	u16 i;
//	LCD_LOCK();
//	for(i=0;i<Len;i++)
//	{
//		LCD_SetPoint(X,Y+i,FColor);//画点 
//	}	
//	LCD_UNLOCK();
//}
///****************************************************************************
//* 名    称：LCD_DrawLine
//* 功    能：在指定座标画直线
//* 入口参数：x1     A点行座标
//*           y1     A点列座标
//*           x2     B点行座标
//*           y2     B点列座标
//*           FColor  线颜色
//* 出口参数：无
//* 说    明：
//****************************************************************************/
////画线
////x1,y1:起点坐标
////x2,y2:终点坐标  
//void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 FColor)
//{
//	u16 t; 
//	int xerr=0,yerr=0,delta_x,delta_y,distance; 
//	int incx,incy,uRow,uCol; 
//	LCD_LOCK();
//	delta_x=x2-x1; //计算坐标增量 
//	delta_y=y2-y1; 
//	uRow=x1; 
//	uCol=y1; 
//	if(delta_x>0)incx=1; //设置单步方向 
//	else if(delta_x==0)incx=0;//垂直线 
//	else {incx=-1;delta_x=-delta_x;} 
//	if(delta_y>0)incy=1; 
//	else if(delta_y==0)incy=0;//水平线 
//	else{incy=-1;delta_y=-delta_y;} 
//	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
//	else distance=delta_y; 
//	for(t=0;t<=distance+1;t++ )//画线输出 
//	{  
//		LCD_SetPoint(uRow,uCol,FColor);//画点 
//		xerr+=delta_x ; 
//		yerr+=delta_y ; 
//		if(xerr>distance) 
//		{ 
//			xerr-=distance; 
//			uRow+=incx; 
//		} 
//		if(yerr>distance) 
//		{ 
//			yerr-=distance; 
//			uCol+=incy; 
//		} 
//	}  
//	LCD_UNLOCK();
//}   
////画矩形
//void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
//{
//	u16 i,j;
//	u16 X,Y;
//	if(x1>x2) return;
//	if(y1>y2) return;	
//	X= x2-x1;
//	Y = y2-y1;
//	LCD_LOCK();
//	#if LCDAPIUSEFAST > 0		
//	
//	
//		LCD_SetWindows(x1,y1,x2,y2);
//		LCD_BeginWriteRAM(); 	
//		for(i=0;i<X+1;i++)
//		{
//			for (j=0;j<Y+1;j++)
//				{
//				 LCD_OnlyWriteRAM(Color);
//				}
//		}
//		LCD_EndWriteRAM();
//	#else
//		for(i=0;i<X+1;i++)
//		{
//			for (j=0;j<Y+1;j++)
//				{
//				 LCD_SetPoint(i,j,Color);
//				}
//		}		
//	#endif
//	LCD_UNLOCK();	
//}


////画矩形
//void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
//{
//	u16 i;
//	if(x1>x2)
//	{
//		i=x1;
//		x1=x2;
//		x2=i;
//	}
//	if(y1>y2)
//	{
//		i=y1;
//		y1=y2;
//		y2=i;
//	}	
//		LCD_DrawHLine(x1,y1,x2-x1+1,Color);
//		LCD_DrawHLine(x1,y2,x2-x1+1,Color);
//		LCD_DrawVLine(x1,y1,y2-y1+1,Color);
//		LCD_DrawVLine(x2,y1,y2-y1+1,Color);	
//}

////在指定位置画一个指定大小的圆
////(x,y):中心点
////r    :半径
//void LCD_DrawCircle(u16 x0,u16 y0,u8 r,u16 FColor)
//{
//	int a,b;
//	int di;
//	a=0;b=r;	  
//	di=3-(r<<1);             //判断下个点位置的标志
//	LCD_LOCK();
//	while(a<=b)
//	{
//		LCD_SetPoint(x0-b,y0-a,FColor);             //3           
//		LCD_SetPoint(x0+b,y0-a,FColor);             //0           
//		LCD_SetPoint(x0-a,y0+b,FColor);             //1       
//		LCD_SetPoint(x0-b,y0-a,FColor);             //7           
//		LCD_SetPoint(x0-a,y0-b,FColor);             //2             
//		LCD_SetPoint(x0+b,y0+a,FColor);             //4               
//		LCD_SetPoint(x0+a,y0-b,FColor);             //5
//		LCD_SetPoint(x0+a,y0+b,FColor);             //6 
//		LCD_SetPoint(x0-b,y0+a,FColor);             
//		a++;
//		//使用Bresenham算法画圆     
//		if(di<0)di +=4*a+6;	  
//		else
//		{
//			di+=10+4*(a-b);   
//			b--;
//		} 
//		LCD_SetPoint(x0+a,y0+b,FColor);
//	}
//	LCD_UNLOCK();
//} 









