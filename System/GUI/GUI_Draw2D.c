/*********************************************************************************
* 文件名			: GUI_Draw2D.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI界面一些绘图API							  
* 用法			：

**********************************************************************************/

//  
#include "GUI_DRAW2D.h" 
#include "GUI_conf.h"
#include "GUI_LCDAPI.h"
#include "GUI_font.h"
#include "GUI.h"
#if GUI_USEMSOS
#include "os.h"
#endif

//	void GUI_PutGBChar(u8 *Char,u16 x,u16 y,u16 FColor,u16 BColor,GUI_FontStruct *Font)	
//	{
//		u8 size = Font->GB_X; 
//  u16 i;
//  u16 j; 
//u16 k=0;		
//  u32 tmp_char;
////		u8 GBX_BYTE = Font->GB_Num/size;//得到字体一个字符对应点阵集所占的字节数			
//	#if(FONT_USE	==	FONT_USE_SD)		
//		u8 GBFont[72];
//		#endif
//		
//		u8* GBFont_P;	
//		
//		
//		if(size!=12&&size!=16&&size!=24)return ;	//不支持的size	
//	#if(FONT_USE	==	FONT_USE_ZIMO)			
//		GBFont_P=Get_Hz_ZIMO(Char,Font);
//		#endif		
//	#if(FONT_USE	==	FONT_USE_SD)			
//		Get_Hz_SD(Char,GBFont,Font);
//		GBFont_P = GBFont;
//		#endif				
//		if(GBFont_P==0)return ;	//得到相应大小的点阵数据 
//		GUI_LOCK();
//		GUI_SetWindows(x,y,x+Font->GB_X-1,y+Font->Y-1);
//		GUI_BeginWriteRAM(); 		
//		for (i=0;i< Font->GB_Num;i++)
//		{
//			tmp_char = GBFont_P[i];	
//			for (j=0;j<8;j++)
//			{
//				if(tmp_char&0x80)
//				{
//					GUI_OnlyWriteRAM(FColor); // 字符颜色
//				}					
//				else	
//				{
//					GUI_OnlyWriteRAM(BColor); // 背景颜色
//				}		
//				tmp_char<<=1;
//				k++;
//				if(k==Font->GB_X)
//				{
//					k=0;
//					break;
//				}				
//			}
//		}
//		GUI_EndWriteRAM();	
//		GUI_UNLOCK();		
//	}			

	
#if(FONT_USE	==	FONT_USE_SD)	
	/****************************************************************************
	* 名    称：Show_Font
	* 功    能：显示一个指定大小的汉字
	* 入口参数：x,y :汉字的坐标;Char:汉字GBK码;mode:0,正常显示,1,叠加显示;F_Color,B_Color:字体颜色
	* 出口参数：无
	* 说    明：
	****************************************************************************/

	void GUI_PutGBChar(u8 *Char,u16 x,u16 y,u8 mode,u16 F_Color,u16 B_Color,GUI_FontStruct *Font)
	{
		u8 size ;
		u8 temp,t,t1;
		u16 y0=y;
		u8 dzk[72];  	
		u8 csize;//得到字体一个字符对应点阵集所占的字节数	 
		size = Font->GB_X;
		csize = Font->GB_Num;	
		if(size!=12&&size!=16&&size!=24)return;	//不支持的size	
		GUI_LOCK();
		Get_Hz_SD(Char,dzk,Font);	//得到相应大小的点阵数据 
		for(t=0;t<csize;t++)
		{   												   
			temp=dzk[t];			//得到点阵数据                          
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
				{
					GUI_SetPoint(x,y,F_Color);
				}
				else if(mode==0)
				{
					GUI_SetPoint(x,y,B_Color);
				} 
				temp<<=1;
				y++;
				if((y-y0)==size)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
		}  
		GUI_UNLOCK();
	}

#else	
	void GUI_PutGBChar(u8 *Char,u16 x,u16 y,u8 mode,u16 F_Color,u16 B_Color,GUI_FontStruct *Font)
	{
		u16 l;
		u8 size = Font->GB_X; 
		u8 temp,t,t1;
		u16 x0=x;		
		u8 csize = Font->GB_Num;//得到字体一个字符对应点阵集所占的字节数			
		u8 const *GBFont;		
			GUI_LOCK();
			for (l=0;l<MAXGBFONTNUM;l++) //MAXGBFONTNUM标示自建汉字库中的个数，循环查询内码
			{ 	
				GBFont=&(Font->GBFont_P[l*(Font->GB_Num * Font->Y + 3)]);		
				if((GBFont[0]==Char[0])&&(GBFont[1]==Char[1]))
				{
					if(size!=12&&size!=16&&size!=24)return;	//不支持的size	
					for(t=0;t<csize;t++)
					{   												   
						temp=GBFont[3+t];			//得到点阵数据                          
						for(t1=0;t1<8;t1++)
						{
							if(temp&0x80)GUI_SetPoint(x,y,F_Color);
							else if(mode==0)GUI_SetPoint(x,y,B_Color); 
							temp<<=1;
							x0++;
							if((x-x0)==size)
							{
								x=x0;
								y++;
								break;
							}
						}  	 
					} 					
				}	
			}	
		GUI_UNLOCK();		
	}			
#endif	
	






/****************************************************************************
* 名    称：void GUI_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：GUI_PutChar(10,10,'a');
****************************************************************************/
void GUI_PutChar(u8 *c,u16 x,u16 y,u16 FColor,u16 BColor,GUI_FontStruct *Font)
{
  u16 i;
  u16 j;  
  u32 tmp_char;
	u8 ASCX_BYTE = Font->ASC_Num/Font->Y;
	u8 const *ASCFont;	
	i= (*c-0x20);
	i = i*(Font->ASC_Num);
	ASCFont = &Font->Font_P[i];
//	ASCFont = &(Font->Font_P[((c-0x20)*Font->ASC_Num)]);
	GUI_LOCK();
	
	#if LCDAPIUSEFAST > 0	
		GUI_SetWindows(x,y,x+Font->ASC_X-1,y+Font->Y-1);
		GUI_BeginWriteRAM(); 
	#endif	
		for (i=0;i< Font->Y;i++)
		{
			if(Font->ASC_X<=8)
			{
				tmp_char=ASCFont[i*ASCX_BYTE];		
				for (j=0;j<Font->ASC_X;j++)
				{
					if (((tmp_char >> (7-j)) & 0x01) == 0x01)
					{
						#if LCDAPIUSEFAST > 0	
							GUI_OnlyWriteRAM(FColor); // 字符颜色
						#else
							GUI_SetPoint(j+x,i+y,FColor); // 字符颜色
						#endif
					}					
					else	
					{
						#if LCDAPIUSEFAST > 0
							GUI_OnlyWriteRAM(BColor); // 背景颜色
						#else
							GUI_SetPoint(j+x,i+y,BColor); // 背景颜色
						#endif
					}					
				}				
			}
			else
			{
				tmp_char = *(u16*)(&ASCFont[i*ASCX_BYTE]);		
				for (j=0;j<Font->ASC_X;j++)
				{
					if (((tmp_char >> j) & 0x01) == 0x01)
					{
						#if LCDAPIUSEFAST > 0	
							GUI_OnlyWriteRAM(FColor); // 字符颜色
						#else
							GUI_SetPoint(j+x,i+y,FColor); // 字符颜色
						#endif
					}					
					else	
					{
						#if LCDAPIUSEFAST > 0
							GUI_OnlyWriteRAM(BColor); // 背景颜色
						#else
							GUI_SetPoint(j+x,i+y,BColor); // 背景颜色
						#endif
					}					
				}
				
			}
		}
	#if LCDAPIUSEFAST > 0
		GUI_EndWriteRAM();	
	#endif		
	GUI_UNLOCK();
}
/****************************************************************************
* 名    称：GUI_Clear
* 功    能：将屏幕填充成指定的颜色
* 入口参数：Color      填充值
* 出口参数：无
* 说    明：
* 调用方法：GUI_Clear(0xffff);
****************************************************************************/
void GUI_Clear(u16 Color)
{
	u16 i,j;
	GUI_LOCK();
	#if LCDAPIUSEFAST > 0
		GUI_SetWindows(0,0,GUI_W-1,GUI_H-1);
		GUI_BeginWriteRAM(); 
		for(i=0;i<GUI_W;i++)
		{
			for (j=0;j<GUI_H;j++)
			{
				GUI_OnlyWriteRAM(Color);
			}
		}
			GUI_EndWriteRAM();
	#else
	GUI_SetWindows(0,0,GUI_W-1,GUI_H-1);		
	for(i=0;i<GUI_W;i++)
	{
		for (j=0;j<GUI_H;j++)
		{
			GUI_SetPoint(j,i,Color);
		}
	}			
	#endif
	GUI_UNLOCK();
}

void GUI_DrawHLine(u16 X, u16 Y,u16 Len, u16 FColor)
{
	u16 i;
	GUI_LOCK();
	for(i=0;i<Len;i++)
	{
		GUI_SetPoint(X+i,Y,FColor);//画点 
	}
	GUI_UNLOCK();
}
void GUI_DrawVLine( u16 X, u16 Y, u16 Len,u16 FColor)
{
	u16 i;
	GUI_LOCK();
	for(i=0;i<Len;i++)
	{
		GUI_SetPoint(X,Y+i,FColor);//画点 
	}	
	GUI_UNLOCK();
}
/****************************************************************************
* 名    称：GUI_DrawLine
* 功    能：在指定座标画直线
* 入口参数：x1     A点行座标
*           y1     A点列座标
*           x2     B点行座标
*           y2     B点列座标
*           FColor  线颜色
* 出口参数：无
* 说    明：
****************************************************************************/
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void GUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 FColor)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	GUI_LOCK();
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		GUI_SetPoint(uRow,uCol,FColor);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
	GUI_UNLOCK();
}   
//画矩形
void GUI_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	u16 i,j;
	u16 X,Y;
	if(x1>x2) return;
	if(y1>y2) return;	
	X= x2-x1;
	Y = y2-y1;
	GUI_LOCK();
	#if LCDAPIUSEFAST > 0		
	
	
		GUI_SetWindows(x1,y1,x2,y2);
		GUI_BeginWriteRAM(); 	
		for(i=0;i<X+1;i++)
		{
			for (j=0;j<Y+1;j++)
				{
				 GUI_OnlyWriteRAM(Color);
				}
		}
		GUI_EndWriteRAM();
	#else
		for(i=0;i<X+1;i++)
		{
			for (j=0;j<Y+1;j++)
				{
				 GUI_SetPoint(i,j,Color);
				}
		}		
	#endif
	GUI_UNLOCK();	
}


//画矩形
void GUI_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	u16 i;
	if(x1>x2)
	{
		i=x1;
		x1=x2;
		x2=i;
	}
	if(y1>y2)
	{
		i=y1;
		y1=y2;
		y2=i;
	}	
		GUI_DrawHLine(x1,y1,x2-x1+1,Color);
		GUI_DrawHLine(x1,y2,x2-x1+1,Color);
		GUI_DrawVLine(x1,y1,y2-y1+1,Color);
		GUI_DrawVLine(x2,y1,y2-y1+1,Color);	
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void GUI_DrawCircle(u16 x0,u16 y0,u8 r,u16 FColor)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	GUI_LOCK();
	while(a<=b)
	{
		GUI_SetPoint(x0-b,y0-a,FColor);             //3           
		GUI_SetPoint(x0+b,y0-a,FColor);             //0           
		GUI_SetPoint(x0-a,y0+b,FColor);             //1       
		GUI_SetPoint(x0-b,y0-a,FColor);             //7           
		GUI_SetPoint(x0-a,y0-b,FColor);             //2             
		GUI_SetPoint(x0+b,y0+a,FColor);             //4               
		GUI_SetPoint(x0+a,y0-b,FColor);             //5
		GUI_SetPoint(x0+a,y0+b,FColor);             //6 
		GUI_SetPoint(x0-b,y0+a,FColor);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		GUI_SetPoint(x0+a,y0+b,FColor);
	}
	GUI_UNLOCK();
} 

/****************************************************************************
* 名    称：GUI_DrawBitmap
* 功    能：在指定座标范围显示一副图片
* 入口参数： pBitmap     	图片的数据指针
*						x0     				行起始座标
*           y0     				列起始座标
* 出口参数：无
* 说    明：
****************************************************************************/
void GUI_DrawBitmap(const GUI_BITMAP *pBitmap, int x0, int y0) 
{
	u16  i,k,j;
	u8 t,t1;
	u16  XSize,YSize,* pMethods;
	u8 BitsPerPixel;
	const u8 * pData;
	XSize = pBitmap->XSize;
	YSize = pBitmap->YSize;
	BitsPerPixel = pBitmap->BitsPerPixel;
	pMethods = pBitmap->pMethods;
	pData = pBitmap->pData;
	GUI_LOCK();
	#if LCDAPIUSEFAST > 0		
		GUI_SetWindows(x0,y0,x0+XSize-1,y0 + YSize-1);
		GUI_BeginWriteRAM();					//开始写入颜色数据
		if (BitsPerPixel == 4){
			for (i=0;i<YSize;i++){
				for(k=0;k<XSize/2;k++){
					t= *pData;
					for(j=0;j<2;j++){
						t1= (t>>(4-4*j))&0x07; 
						GUI_OnlyWriteRAM(pMethods[t1]);
					}
						pData++;

				}
				if(XSize &0x01)
				{
					t= (*pData&0x70)>>4; 
					GUI_OnlyWriteRAM(pMethods[t]);
					pData++;
				}
			 }
		}
		else if(BitsPerPixel == 2){
			for (i=0;i<YSize;i++){
				for(k=0;k<XSize/4;k++){
					t= *pData;
					for(j=0;j<4;j++){
						t1= (t>>(6-2*j))&0x03; 
						GUI_OnlyWriteRAM(pMethods[t1]);
					}
						pData++;

				}
				k=XSize &0x03;
				if(k>0)
				{
					t= *pData;
					for(j=0;j<k;j++){
						t1= (t>>(6-2*j))&0x03; 
						GUI_OnlyWriteRAM(pMethods[t1]);
					}
					pData++;
				}
			 }

		}

		else if(BitsPerPixel == 1){
			for (i=0;i<YSize;i++){
				for(k=0;k<XSize/8;k++){
					t= *pData;
					for(j=0;j<8;j++){
						t1= (t>>(7-j))&0x01; 
						GUI_OnlyWriteRAM(pMethods[t1]);
					}
						pData++;

				}
				k=XSize &0x07;
				if(k>0)
				{
					t= *pData;
					for(j=0;j<k;j++){
						t1= (t>>(7-j))&0x01; 
						GUI_OnlyWriteRAM(pMethods[t1]);
					}
					pData++;
				}
			 }

		}
	GUI_EndWriteRAM();
	
	#else
		if (BitsPerPixel == 4){
			for (i=0;i<YSize;i++){
				for(k=0;k<XSize/2;k++){
					t= *pData;
					for(j=0;j<2;j++){
						t1= (t>>(4-4*j))&0x07; 
//						GUI_SetPoint(k*2+j,i,pMethods[t1]);
					}
						pData++;

				}
				if(XSize &0x01)
				{
					t= (*pData&0x70)>>4; 
//					GUI_WriteRAM(pMethods[t]);
					pData++;
				}
			 }
		}
		else if(BitsPerPixel == 2){
			for (i=0;i<YSize;i++){
				for(k=0;k<XSize/4;k++){
					t= *pData;
					for(j=0;j<4;j++){
						t1= (t>>(6-2*j))&0x03; 
//						GUI_WriteRAM(pMethods[t1]);
					}
						pData++;

				}
				k=XSize &0x03;
				if(k>0)
				{
					t= *pData;
					for(j=0;j<k;j++){
						t1= (t>>(6-2*j))&0x03; 
//						GUI_WriteRAM(pMethods[t1]);
					}
					pData++;
				}
			 }

		}

		else if(BitsPerPixel == 1){
			for (i=0;i<YSize;i++){
				for(k=0;k<XSize/8;k++){
					t= *pData;
					for(j=0;j<8;j++){
						t1= (t>>(7-j))&0x01; 
//						GUI_WriteRAM(pMethods[t1]);
					}
						pData++;

				}
				k=XSize &0x07;
				if(k>0)
				{
					t= *pData;
					for(j=0;j<k;j++){
						t1= (t>>(7-j))&0x01; 
//						GUI_WriteRAM(pMethods[t1]);
					}
					pData++;
				}
			 }

		}	
	#endif
	GUI_UNLOCK();
}


u16 StringWidth(u8 *String,GUI_FontStruct *Font)
{
	u8 i=0;
	u16 Len=0;
	while(String[i] != 0x00)
	{				
		if(String[i] < 0x80) 
		{
			Len += Font->ASC_X;				
		}
		else
		{

			Len += Font->GB_X;		
			i++;					
		}
		i++;
	}		
	
		return Len;	
}


/*----------------------------------------------------------------
                            显示字符串
可以中英文同时显示
输入参数：x 横坐标
          y 纵坐标
		  *s 需要显示的字符串
		  fColor 字符颜色
		  bColor 字符背景颜色
----------------------------------------------------------------*/
void GUI_ShowStringAt(u8 *String,u16 X1, u16 Y1,u16 FColor,u16 BColor,GUI_FontStruct *Font)
{
	u16 i=0;
//	u16 X=0;
	while(String[i] != 0x00)
	{				
		
		if(String[i] < 0x80) 
		{
			GUI_PutChar(&String[i], X1, Y1, FColor, BColor,Font);		
			X1=X1+Font->ASC_X;			
		}
		else
		{
			GUI_PutGBChar(&String[i], X1, Y1,0, FColor, BColor,Font);	
			X1=X1+GUI.Font->GB_X;
			i++;
		}
		i++;				
	}	
}

void GUI_PutStringAt(u8 *String,u16 X1, u16 Y1)
{
	GUI_ShowStringAt(String,X1,Y1,GUI.B_Color,GUI.F_Color,GUI.Font);
	
}


/****************************************************************************
* 名    称：GUI_DispDecAt
* 功    能：显示10进制数字
* 入口参数：Color      填充值
* 出口参数：无
* 说    明：
* 调用方法：GUI_Clear(0xffff);
****************************************************************************/
void GUI_DispDecAt(u32 Value,u16 X,u16 Y,u16 len)
{
	u8 String[10];
	u8 i;
	DecToString(Value,String,10);
	if(len>10) len = 10;
	for(i=0;i<len;i++)
	{
		GUI_PutChar(&String[10-len+i], X+ GUI.Font->ASC_X*i, Y, GUI.F_Color, GUI.B_Color,GUI.Font);		
	}
}

void GUI_DispDec(s32 Value,u16 X,u16 Y,u16 len)
{
	u8 String[10];
	u8 i;

	if(Value >= 0 )
	{
		String[0] = 0x2B;

		GUI_PutChar(String, X, Y, GUI.F_Color, GUI.B_Color,GUI.Font);		
		DecToString(Value,String,10);			
	}
	else
	{
		String[0] = 0x2D;
		GUI_PutChar(String, X, Y, GUI.F_Color, GUI.B_Color,GUI.Font);			
		DecToString((0-Value),String,10);		
	}
	
	if(len>10) len = 10;
	for(i=0;i<len;i++)
	{
		GUI_PutChar(&String[10-len+i], X+ GUI.Font->ASC_X*(i+1), Y, GUI.F_Color, GUI.B_Color,GUI.Font);		
	}
}











/****************************************************************************
* 名    称：GUI_DispHexAt
* 功    能：显示16进制数字
* 入口参数：Color      填充值
* 出口参数：无
* 说    明：
* 调用方法：GUI_Clear(0xffff);
****************************************************************************/
void GUI_DispHexAt(u32 Value,u16 X,u16 Y,u16 len)
{
	u8 String[8];
	u8 i;
	HexToString(Value,String,8);
	if(len>8) len = 8;
	for(i=0;i<len;i++)
	{
		GUI_PutChar(&String[8-len+i], X+ GUI.Font->ASC_X*i, Y, GUI.F_Color, GUI.B_Color,GUI.Font);		
	}
}








































//GUIStruct GUI;	
//#if(GUI_USESDFONT>0)
//	#include "ff.h"
//	FIL *FontFile;			//文件1
//	FRESULT Res;			//FatFs通用结果码
//	UINT ByteReadNum;		//number of bytes read

//	/****************************************************************************
//	* 名    称：Get_HzMat
//	* 功    能：获取汉字点阵
//	* 入口参数：code：字符指针开始；mat：数据存放地址；Font：字体
//	* 出口参数：无
//	* 说    明：
//	****************************************************************************/
//	void Get_HzMat(u8 *code,u8 *mat,GUI_FONT *Font)
//	{		    
//		unsigned char qh,ql;
//		unsigned char i;					  
//		unsigned long foffset; 
//		FontFile = (FIL*)GUI_MEM_Malloc(sizeof(FIL));		//为文件1申请内存
//		qh=*code;
//		ql=*(++code);
//		if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
//		{   		    
//				for(i=0;i<Font->GB_Num;i++)*mat++=0x00;//填充满格
//				return; //结束访问
//		}          
//		if(ql<0x7f)ql-=0x40;//注意!
//		else ql-=0x41;
//		qh-=0x81;   
//		foffset=((unsigned long)190*qh+ql)*Font->GB_Num;	//得到字库中的字节偏移量  	
//		Res = f_open(FontFile, (const TCHAR *)Font->GBFont_P, FA_OPEN_EXISTING | FA_READ);		
//		Res = f_lseek(FontFile, foffset); 
//		Res = f_read(FontFile, mat, Font->GB_Num, &ByteReadNum);	//读取文件内容
//		Res = f_close(FontFile);	//关闭文件				
//		GUI_MEM_Free(FontFile);											    
//	}  


//	/****************************************************************************
//	* 名    称：Show_Font
//	* 功    能：显示一个指定大小的汉字
//	* 入口参数：x,y :汉字的坐标;Char:汉字GBK码;mode:0,正常显示,1,叠加显示;F_Color,B_Color:字体颜色
//	* 出口参数：无
//	* 说    明：
//	****************************************************************************/
//	void Show_Font(u8 *Char,u16 x,u16 y,u8 mode,u16 F_Color,u16 B_Color,GUI_FONT *Font)
//	{
//		u8 size ;
//		u8 temp,t,t1;
//		u16 y0=y;
//		u8 dzk[72];  	
//		u8 csize;//得到字体一个字符对应点阵集所占的字节数	 
//		size = Font->GB_X;
//		csize = Font->GB_Num;	
//		if(size!=12&&size!=16&&size!=24)return;	//不支持的size	
//		GUI_LOCK();
//		Get_HzMat(Char,dzk,Font);	//得到相应大小的点阵数据 
//		for(t=0;t<csize;t++)
//		{   												   
//			temp=dzk[t];			//得到点阵数据                          
//			for(t1=0;t1<8;t1++)
//			{
//				if(temp&0x80)
//				{
//					GUI.SetPoint(x,y,F_Color);
//				}
//				else if(mode==0)
//				{
//					GUI.SetPoint(x,y,B_Color);
//				} 
//				temp<<=1;
//				y++;
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					break;
//				}
//			}  	 
//		}  
//		GUI_UNLOCK();
//	}

//#else	
//	void Show_Font(u8 *Char,u16 x,u16 y,u8 mode,u16 F_Color,u16 B_Color,GUI_FONT *Font)	
//	{
//		u16 l;
//		u8 size = Font->GB_X; 
//		u8 temp,t,t1;
//		u16 x0=x;		
//		u8 csize = Font->GB_Num;//得到字体一个字符对应点阵集所占的字节数			
//		u8 const *GBFont;		
//			GUI_LOCK();
//			for (l=0;l<MAXGBFONTNUM;l++) //MAXGBFONTNUM标示自建汉字库中的个数，循环查询内码
//			{ 	
//				GBFont=&(Font->GBFont_P[l*(Font->GB_Num * Font->Y + 3)]);		
//				if((GBFont[0]==Char[0])&&(GBFont[1]==Char[1]))
//				{
//					if(size!=12&&size!=16&&size!=24)return;	//不支持的size	
//					for(t=0;t<csize;t++)
//					{   												   
//						temp=GBFont[3+t];			//得到点阵数据                          
//						for(t1=0;t1<8;t1++)
//						{
//							if(temp&0x80)GUI.SetPoint(x,y,F_Color);
//							else if(mode==0)GUI.SetPoint(x,y,B_Color); 
//							temp<<=1;
//							x0++;
//							if((x-x0)==size)
//							{
//								x=x0;
//								y++;
//								break;
//							}
//						}  	 
//					} 					
//				}	
//			}	
//		GUI_UNLOCK();		
//	}			
//#endif
///****************************************************************************
//* 名    称：Show_FontAt
//* 功    能：显示一个指定大小的汉字
//* 入口参数：x,y :汉字的坐标;Char:汉字GBK码;mode:0,正常显示,1,叠加显示
//* 出口参数：无
//* 说    明：使用GUI的颜色来显示汉字
//****************************************************************************/
//void Show_FontAt(u8 *Char,u16 x,u16 y,u8 mode)
//{
//	Show_Font(Char,x,y,0,GUI.F_Color,GUI.B_Color,GUI.Font);
//}
//	
//	
//	
///*----------------------------------------------------------------
//                            显示汉字
//输入参数：x 横坐标
//          y 纵坐标
//		  c 需要显示的汉字码
//		  fColor 字符颜色
//		  bColor 字符背景颜色
//----------------------------------------------------------------*/
//void GUI_PutGBChar(u8 c[2],u16 x, u16  y, u16 FColor,u16 BColor,GUI_FONT *Font)
//{
//	 Show_Font(c,x,y,0,FColor,BColor,Font);
//}






///****************************************************************************
//* 名    称：void GUI_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
//* 功    能：在指定座标显示一个8x16点阵的ascii字符
//* 入口参数：x          行座标
//*           y          列座标
//*           charColor  字符的颜色
//*           bkColor    字符背景颜色
//* 出口参数：无
//* 说    明：显示范围限定为可显示的ascii码
//* 调用方法：GUI_PutChar(10,10,'a');
//****************************************************************************/
//void GUI_PutChar(u8 c,u16 x,u16 y,u16 FColor,u16 BColor,GUI_FONT *Font)
//{
//  u16 i;
//  u16 j;  
//  u32 tmp_char;
//	u8 const *ASCFont;	
//	ASCFont = &(Font->Font_P[((c-0x20)*Font->ASC_Num*Font->Y)]);
//	GUI_LOCK();
//	
//	#if LCDAPIUSEFAST > 0		
//		GUI.SetWindows(x,y,x+Font->ASC_X-1,y+Font->Y-1);
//		GUI.BeginWriteRAM(); 		
//		for (i=0;i< Font->Y;i++)
//		{
//					UintToByte3(tmp_char)=ASCFont[i*Font->ASC_Num];
//					UintToByte2(tmp_char)=ASCFont[1+i*Font->ASC_Num];
//					UintToByte1(tmp_char)=ASCFont[2+i*Font->ASC_Num];
////				UintToByte0(tmp_char)=GBFont[3+i*Font->Num];		
//			for (j=0;j<Font->ASC_X;j++)
//			{
//				if (((tmp_char >> (31-j)) & 0x01) == 0x01)
//				{
//					GUI.WriteRAM(FColor); // 字符颜色
//				}					
//				else	
//				{
//					GUI.WriteRAM(BColor); // 背景颜色
//				}					
//			}
//		}
//		GUI.EndWriteRAM();	
//	#else
//		for (i=0;i< Font->Y;i++)
//		{
//					UintToByte3(tmp_char)=ASCFont[i*Font->ASC_Num];
//					UintToByte2(tmp_char)=ASCFont[1+i*Font->ASC_Num];
//					UintToByte1(tmp_char)=ASCFont[2+i*Font->ASC_Num];
////				UintToByte0(tmp_char)=GBFont[3+i*Font->Num];		
//			for (j=0;j<Font->ASC_X;j++)
//			{
//				if (((tmp_char >> (31-j)) & 0x01) == 0x01)	
//					GUI.SetPoint(j+x,i+y,FColor); // 字符颜色
//				else																				
//					GUI.SetPoint(j+x,i+y,BColor); // 背景颜色
//			}
//		}		
//	#endif
//		
//		
//	GUI_UNLOCK();
//}






//u8 SizeString(u8 *String)
//{
//	u8 i;
//		for (i=0;i<STRING_MAXLEN;i++)
//		{
//			if(String[i] == 0x00)
//			{				
//				return i;
//			}
//	}		
//				return STRING_MAXLEN;	
//}



///*----------------------------------------------------------------
//                            显示字符串
//可以中英文同时显示
//输入参数：x 横坐标
//          y 纵坐标
//		  *s 需要显示的字符串
//		  fColor 字符颜色
//		  bColor 字符背景颜色
//----------------------------------------------------------------*/
//void GUI_ShowString(u8 *String,GUI_FONT *Font,u16 X1, u16 Y1,u16 X2,u8 Mode, u16 FColor,u16 BColor)
//{
//	u16 i,X,Y2;
//	u16 MaxLen,Len;

//	MaxLen = (X2-X1+1)/8;							//最大能显示的文字数量
//	Len = SizeString(String);						//字符串的长度
//	if(Len>MaxLen) Len = MaxLen;		
//	Y2= Font->Y + Y1-1;
//	switch (Mode)										//对其方式，暂时先做顶部对齐
//		{
//			case Left:
//				X=X1;

//				GUI_DrawFillRectangle(X1+Len*8,Y1,X2,Y2,BColor);	//将区域内显示为背景色 
//				break;
//			case Right:
//				X=X2-Len*8+1;		
//				GUI_DrawFillRectangle(X1,Y1,X,Y2,BColor);	//将区域内显示为背景色 		
//				break;
//			case Middle:	
//				X=(X1+X2+1)/2-Len*4+1;
//				GUI_DrawFillRectangle(X1,Y1,X,Y2,BColor);	//将区域内显示为背景色 		
//				GUI_DrawFillRectangle(X+Len*8-1,Y1,X2,Y2,BColor);	//将区域内显示为背景色 
//			default:
//				break;
//		}	
//		for (i=0;i<Len;i++)
//		{
//			if(String[i] != 0x00)
//			{				
//				if(String[i] < 0x80) 
//				{
//					GUI_PutChar(String[i], X, Y1,FColor,BColor,Font);		
//					X +=Font->ASC_X;					
//				}
//				else
//				{
//	
//					GUI_PutGBChar(((u8*)String)+i, X, Y1, FColor,BColor,Font);		
//					i++;					
//					X +=Font->GB_X;		
//				}
//			}
//			else
//			{
//				return;
//		
//			}
//	}			

//		
//		
//		
//		
//}

//void GUI_PutString(u8 *String,u16 X1, u16 Y1,u16 X2, u16 Y2,u8 mode, u16 FColor,u16 BColor)
//{
//	u16 i,X,Y;
//	u16 MaxLen,Len;
//	if(Y2<(Y1+GUI.Font->Y-1))return;						//显示的坐标中，Y坐标高度差小于16则返回
//	MaxLen = (X2-X1+1)/8;							//最大能显示的文字数量
//	Len =SizeString(String);						//字符串的长度
//	if(Len>MaxLen) Len = MaxLen;		
//	switch (mode)										//对其方式，暂时先做顶部对齐
//		{
//			case TopLeft:
//				X=X1;
//				Y=Y1;
//				GUI_DrawFillRectangle(X1+Len*8,Y1,X2,Y1+GUI.Font->Y-1,BColor);	//将区域内显示为背景色 			
//				GUI_DrawFillRectangle(X1,Y1+GUI.Font->Y-1,X2,Y2,BColor);	//将区域内显示为背景色 			
//			
//				break;
//			case TopRight:
//				X=X2-Len*8+1;
//				Y=Y1;			
//				GUI_DrawFillRectangle(X1,Y1,X,Y1+GUI.Font->Y-1,BColor);	//将区域内显示为背景色 			
//				GUI_DrawFillRectangle(X1,Y1+GUI.Font->Y-1,X2,Y2,BColor);	//将区域内显示为背景色 					
//			
//				break;
//			case TopMiddle:	
//				X=(X1+X2+1)/2-Len*4+1;
//				Y=Y1;	
//				GUI_DrawFillRectangle(X1,Y1,X,Y1+GUI.Font->Y-1,BColor);	//将区域内显示为背景色 		
//				GUI_DrawFillRectangle(X+Len*8,Y1,X2,Y1+GUI.Font->Y-1,BColor);	//将区域内显示为背景色 			
//				GUI_DrawFillRectangle(X1,Y1+GUI.Font->Y-1,X2,Y2,BColor);	//将区域内显示为背景色 		

//			case Middle:	
//				X=(X1+X2+1)/2-Len*4+1;
//				Y=(Y1+Y2+1-16)/2;	
//				GUI_DrawFillRectangle(X1,Y1,X,Y2,BColor);	//将区域内显示为背景色 		
//				GUI_DrawFillRectangle(X1,Y1,X2,Y,BColor);	//将区域内显示为背景色 			
//				GUI_DrawFillRectangle(X+Len*8-1,Y1,X2,Y2,BColor);	//将区域内显示为背景色 
//				GUI_DrawFillRectangle(X1,Y+GUI.Font->Y-1,X2,Y2,BColor);	//将区域内显示为背景色



//			
//			default:
//				break;
//		} 
//		
//		
//		for (i=0;i<Len;i++)
//		{
//			if(String[i] != 0x00)
//			{				
//				if(String[i] < 0x80) 
//				{
//					GUI_PutChar(*(String+i),X+i*8,Y,FColor,BColor,GUI.Font);
//				}
//				else
//				{
//					GUI_PutGBChar(((u8*)String)+i,X+i*8,Y,FColor,BColor,GUI.Font);
//					i++;
//				}
//			}
//			else
//			{
//				return;
//		
//			}
//	}		
//}

//void GUI_ShowStringAt(u8 *String,u16 X1, u16 Y1,GUI_FONT *Font,u16 BColor,u16 FColor)
//{
//	u16 i = 0;
//	u16 X ;
//	X = X1;
//	while(String[i] != 0x00)
//	{				
//		if(String[i] < 0x80) 
//		{

//			GUI_PutChar(String[i], X, Y1, FColor,BColor,Font);		
//			X +=Font->ASC_X;
//		}
//		else
//		{	
//			GUI_PutGBChar(((u8*)String)+i, X, Y1, FColor, BColor,Font);		
//			i++;
//			X +=Font->GB_X;		
//		}
//		i++;
//	}	
//}
//void GUI_PutStringAt(u8 *String,u16 X1, u16 Y1)
//{
//	u16 i = 0;
//	u16 X ;
//	X = X1;
//	while(String[i] != 0x00)
//	{				
//		if(String[i] < 0x80) 
//		{

//			GUI_PutChar(String[i], X, Y1, GUI.F_Color, GUI.B_Color,GUI.Font);		
//			X +=GUI.Font->ASC_X;
//		}
//		else
//		{	
//			GUI_PutGBChar(((u8*)String)+i, X, Y1, GUI.F_Color, GUI.B_Color,GUI.Font);		
//			i++;
//			X +=GUI.Font->GB_X;		
//		}
//		i++;
//	}	
//}













////在指定位置画一个指定大小的圆
////(x,y):中心点
////r    :半径
//void GUI_DrawCircle(u16 x0,u16 y0,u8 r,u16 FColor)
//{
//	int a,b;
//	int di;
//	a=0;b=r;	  
//	di=3-(r<<1);             //判断下个点位置的标志
//	GUI_LOCK();
//	while(a<=b)
//	{
//		GUI.SetPoint(x0-b,y0-a,FColor);             //3           
//		GUI.SetPoint(x0+b,y0-a,FColor);             //0           
//		GUI.SetPoint(x0-a,y0+b,FColor);             //1       
//		GUI.SetPoint(x0-b,y0-a,FColor);             //7           
//		GUI.SetPoint(x0-a,y0-b,FColor);             //2             
//		GUI.SetPoint(x0+b,y0+a,FColor);             //4               
//		GUI.SetPoint(x0+a,y0-b,FColor);             //5
//		GUI.SetPoint(x0+a,y0+b,FColor);             //6 
//		GUI.SetPoint(x0-b,y0+a,FColor);             
//		a++;
//		//使用Bresenham算法画圆     
//		if(di<0)di +=4*a+6;	  
//		else
//		{
//			di+=10+4*(a-b);   
//			b--;
//		} 
//		GUI.SetPoint(x0+a,y0+b,FColor);
//	}
//	GUI_UNLOCK();
//} 



