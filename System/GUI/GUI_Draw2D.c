/*********************************************************************************
* �ļ���			: GUI_Draw2D.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI����һЩ��ͼAPI							  
* �÷�			��

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
////		u8 GBX_BYTE = Font->GB_Num/size;//�õ�����һ���ַ���Ӧ������ռ���ֽ���			
//	#if(FONT_USE	==	FONT_USE_SD)		
//		u8 GBFont[72];
//		#endif
//		
//		u8* GBFont_P;	
//		
//		
//		if(size!=12&&size!=16&&size!=24)return ;	//��֧�ֵ�size	
//	#if(FONT_USE	==	FONT_USE_ZIMO)			
//		GBFont_P=Get_Hz_ZIMO(Char,Font);
//		#endif		
//	#if(FONT_USE	==	FONT_USE_SD)			
//		Get_Hz_SD(Char,GBFont,Font);
//		GBFont_P = GBFont;
//		#endif				
//		if(GBFont_P==0)return ;	//�õ���Ӧ��С�ĵ������� 
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
//					GUI_OnlyWriteRAM(FColor); // �ַ���ɫ
//				}					
//				else	
//				{
//					GUI_OnlyWriteRAM(BColor); // ������ɫ
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
	* ��    �ƣ�Show_Font
	* ��    �ܣ���ʾһ��ָ����С�ĺ���
	* ��ڲ�����x,y :���ֵ�����;Char:����GBK��;mode:0,������ʾ,1,������ʾ;F_Color,B_Color:������ɫ
	* ���ڲ�������
	* ˵    ����
	****************************************************************************/

	void GUI_PutGBChar(u8 *Char,u16 x,u16 y,u8 mode,u16 F_Color,u16 B_Color,GUI_FontStruct *Font)
	{
		u8 size ;
		u8 temp,t,t1;
		u16 y0=y;
		u8 dzk[72];  	
		u8 csize;//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
		size = Font->GB_X;
		csize = Font->GB_Num;	
		if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size	
		GUI_LOCK();
		Get_Hz_SD(Char,dzk,Font);	//�õ���Ӧ��С�ĵ������� 
		for(t=0;t<csize;t++)
		{   												   
			temp=dzk[t];			//�õ���������                          
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
		u8 csize = Font->GB_Num;//�õ�����һ���ַ���Ӧ������ռ���ֽ���			
		u8 const *GBFont;		
			GUI_LOCK();
			for (l=0;l<MAXGBFONTNUM;l++) //MAXGBFONTNUM��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
			{ 	
				GBFont=&(Font->GBFont_P[l*(Font->GB_Num * Font->Y + 3)]);		
				if((GBFont[0]==Char[0])&&(GBFont[1]==Char[1]))
				{
					if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size	
					for(t=0;t<csize;t++)
					{   												   
						temp=GBFont[3+t];			//�õ���������                          
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
* ��    �ƣ�void GUI_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����GUI_PutChar(10,10,'a');
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
							GUI_OnlyWriteRAM(FColor); // �ַ���ɫ
						#else
							GUI_SetPoint(j+x,i+y,FColor); // �ַ���ɫ
						#endif
					}					
					else	
					{
						#if LCDAPIUSEFAST > 0
							GUI_OnlyWriteRAM(BColor); // ������ɫ
						#else
							GUI_SetPoint(j+x,i+y,BColor); // ������ɫ
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
							GUI_OnlyWriteRAM(FColor); // �ַ���ɫ
						#else
							GUI_SetPoint(j+x,i+y,FColor); // �ַ���ɫ
						#endif
					}					
					else	
					{
						#if LCDAPIUSEFAST > 0
							GUI_OnlyWriteRAM(BColor); // ������ɫ
						#else
							GUI_SetPoint(j+x,i+y,BColor); // ������ɫ
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
* ��    �ƣ�GUI_Clear
* ��    �ܣ�����Ļ����ָ������ɫ
* ��ڲ�����Color      ���ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����GUI_Clear(0xffff);
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
		GUI_SetPoint(X+i,Y,FColor);//���� 
	}
	GUI_UNLOCK();
}
void GUI_DrawVLine( u16 X, u16 Y, u16 Len,u16 FColor)
{
	u16 i;
	GUI_LOCK();
	for(i=0;i<Len;i++)
	{
		GUI_SetPoint(X,Y+i,FColor);//���� 
	}	
	GUI_UNLOCK();
}
/****************************************************************************
* ��    �ƣ�GUI_DrawLine
* ��    �ܣ���ָ�����껭ֱ��
* ��ڲ�����x1     A��������
*           y1     A��������
*           x2     B��������
*           y2     B��������
*           FColor  ����ɫ
* ���ڲ�������
* ˵    ����
****************************************************************************/
//����
//x1,y1:�������
//x2,y2:�յ�����  
void GUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 FColor)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	GUI_LOCK();
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		GUI_SetPoint(uRow,uCol,FColor);//���� 
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
//������
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


//������
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

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void GUI_DrawCircle(u16 x0,u16 y0,u8 r,u16 FColor)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
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
* ��    �ƣ�GUI_DrawBitmap
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ����� pBitmap     	ͼƬ������ָ��
*						x0     				����ʼ����
*           y0     				����ʼ����
* ���ڲ�������
* ˵    ����
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
		GUI_BeginWriteRAM();					//��ʼд����ɫ����
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
                            ��ʾ�ַ���
������Ӣ��ͬʱ��ʾ
���������x ������
          y ������
		  *s ��Ҫ��ʾ���ַ���
		  fColor �ַ���ɫ
		  bColor �ַ�������ɫ
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
* ��    �ƣ�GUI_DispDecAt
* ��    �ܣ���ʾ10��������
* ��ڲ�����Color      ���ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����GUI_Clear(0xffff);
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
* ��    �ƣ�GUI_DispHexAt
* ��    �ܣ���ʾ16��������
* ��ڲ�����Color      ���ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����GUI_Clear(0xffff);
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
//	FIL *FontFile;			//�ļ�1
//	FRESULT Res;			//FatFsͨ�ý����
//	UINT ByteReadNum;		//number of bytes read

//	/****************************************************************************
//	* ��    �ƣ�Get_HzMat
//	* ��    �ܣ���ȡ���ֵ���
//	* ��ڲ�����code���ַ�ָ�뿪ʼ��mat�����ݴ�ŵ�ַ��Font������
//	* ���ڲ�������
//	* ˵    ����
//	****************************************************************************/
//	void Get_HzMat(u8 *code,u8 *mat,GUI_FONT *Font)
//	{		    
//		unsigned char qh,ql;
//		unsigned char i;					  
//		unsigned long foffset; 
//		FontFile = (FIL*)GUI_MEM_Malloc(sizeof(FIL));		//Ϊ�ļ�1�����ڴ�
//		qh=*code;
//		ql=*(++code);
//		if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
//		{   		    
//				for(i=0;i<Font->GB_Num;i++)*mat++=0x00;//�������
//				return; //��������
//		}          
//		if(ql<0x7f)ql-=0x40;//ע��!
//		else ql-=0x41;
//		qh-=0x81;   
//		foffset=((unsigned long)190*qh+ql)*Font->GB_Num;	//�õ��ֿ��е��ֽ�ƫ����  	
//		Res = f_open(FontFile, (const TCHAR *)Font->GBFont_P, FA_OPEN_EXISTING | FA_READ);		
//		Res = f_lseek(FontFile, foffset); 
//		Res = f_read(FontFile, mat, Font->GB_Num, &ByteReadNum);	//��ȡ�ļ�����
//		Res = f_close(FontFile);	//�ر��ļ�				
//		GUI_MEM_Free(FontFile);											    
//	}  


//	/****************************************************************************
//	* ��    �ƣ�Show_Font
//	* ��    �ܣ���ʾһ��ָ����С�ĺ���
//	* ��ڲ�����x,y :���ֵ�����;Char:����GBK��;mode:0,������ʾ,1,������ʾ;F_Color,B_Color:������ɫ
//	* ���ڲ�������
//	* ˵    ����
//	****************************************************************************/
//	void Show_Font(u8 *Char,u16 x,u16 y,u8 mode,u16 F_Color,u16 B_Color,GUI_FONT *Font)
//	{
//		u8 size ;
//		u8 temp,t,t1;
//		u16 y0=y;
//		u8 dzk[72];  	
//		u8 csize;//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
//		size = Font->GB_X;
//		csize = Font->GB_Num;	
//		if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size	
//		GUI_LOCK();
//		Get_HzMat(Char,dzk,Font);	//�õ���Ӧ��С�ĵ������� 
//		for(t=0;t<csize;t++)
//		{   												   
//			temp=dzk[t];			//�õ���������                          
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
//		u8 csize = Font->GB_Num;//�õ�����һ���ַ���Ӧ������ռ���ֽ���			
//		u8 const *GBFont;		
//			GUI_LOCK();
//			for (l=0;l<MAXGBFONTNUM;l++) //MAXGBFONTNUM��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
//			{ 	
//				GBFont=&(Font->GBFont_P[l*(Font->GB_Num * Font->Y + 3)]);		
//				if((GBFont[0]==Char[0])&&(GBFont[1]==Char[1]))
//				{
//					if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size	
//					for(t=0;t<csize;t++)
//					{   												   
//						temp=GBFont[3+t];			//�õ���������                          
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
//* ��    �ƣ�Show_FontAt
//* ��    �ܣ���ʾһ��ָ����С�ĺ���
//* ��ڲ�����x,y :���ֵ�����;Char:����GBK��;mode:0,������ʾ,1,������ʾ
//* ���ڲ�������
//* ˵    ����ʹ��GUI����ɫ����ʾ����
//****************************************************************************/
//void Show_FontAt(u8 *Char,u16 x,u16 y,u8 mode)
//{
//	Show_Font(Char,x,y,0,GUI.F_Color,GUI.B_Color,GUI.Font);
//}
//	
//	
//	
///*----------------------------------------------------------------
//                            ��ʾ����
//���������x ������
//          y ������
//		  c ��Ҫ��ʾ�ĺ�����
//		  fColor �ַ���ɫ
//		  bColor �ַ�������ɫ
//----------------------------------------------------------------*/
//void GUI_PutGBChar(u8 c[2],u16 x, u16  y, u16 FColor,u16 BColor,GUI_FONT *Font)
//{
//	 Show_Font(c,x,y,0,FColor,BColor,Font);
//}






///****************************************************************************
//* ��    �ƣ�void GUI_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
//* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
//* ��ڲ�����x          ������
//*           y          ������
//*           charColor  �ַ�����ɫ
//*           bkColor    �ַ�������ɫ
//* ���ڲ�������
//* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
//* ���÷�����GUI_PutChar(10,10,'a');
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
//					GUI.WriteRAM(FColor); // �ַ���ɫ
//				}					
//				else	
//				{
//					GUI.WriteRAM(BColor); // ������ɫ
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
//					GUI.SetPoint(j+x,i+y,FColor); // �ַ���ɫ
//				else																				
//					GUI.SetPoint(j+x,i+y,BColor); // ������ɫ
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
//                            ��ʾ�ַ���
//������Ӣ��ͬʱ��ʾ
//���������x ������
//          y ������
//		  *s ��Ҫ��ʾ���ַ���
//		  fColor �ַ���ɫ
//		  bColor �ַ�������ɫ
//----------------------------------------------------------------*/
//void GUI_ShowString(u8 *String,GUI_FONT *Font,u16 X1, u16 Y1,u16 X2,u8 Mode, u16 FColor,u16 BColor)
//{
//	u16 i,X,Y2;
//	u16 MaxLen,Len;

//	MaxLen = (X2-X1+1)/8;							//�������ʾ����������
//	Len = SizeString(String);						//�ַ����ĳ���
//	if(Len>MaxLen) Len = MaxLen;		
//	Y2= Font->Y + Y1-1;
//	switch (Mode)										//���䷽ʽ����ʱ������������
//		{
//			case Left:
//				X=X1;

//				GUI_DrawFillRectangle(X1+Len*8,Y1,X2,Y2,BColor);	//����������ʾΪ����ɫ 
//				break;
//			case Right:
//				X=X2-Len*8+1;		
//				GUI_DrawFillRectangle(X1,Y1,X,Y2,BColor);	//����������ʾΪ����ɫ 		
//				break;
//			case Middle:	
//				X=(X1+X2+1)/2-Len*4+1;
//				GUI_DrawFillRectangle(X1,Y1,X,Y2,BColor);	//����������ʾΪ����ɫ 		
//				GUI_DrawFillRectangle(X+Len*8-1,Y1,X2,Y2,BColor);	//����������ʾΪ����ɫ 
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
//	if(Y2<(Y1+GUI.Font->Y-1))return;						//��ʾ�������У�Y����߶Ȳ�С��16�򷵻�
//	MaxLen = (X2-X1+1)/8;							//�������ʾ����������
//	Len =SizeString(String);						//�ַ����ĳ���
//	if(Len>MaxLen) Len = MaxLen;		
//	switch (mode)										//���䷽ʽ����ʱ������������
//		{
//			case TopLeft:
//				X=X1;
//				Y=Y1;
//				GUI_DrawFillRectangle(X1+Len*8,Y1,X2,Y1+GUI.Font->Y-1,BColor);	//����������ʾΪ����ɫ 			
//				GUI_DrawFillRectangle(X1,Y1+GUI.Font->Y-1,X2,Y2,BColor);	//����������ʾΪ����ɫ 			
//			
//				break;
//			case TopRight:
//				X=X2-Len*8+1;
//				Y=Y1;			
//				GUI_DrawFillRectangle(X1,Y1,X,Y1+GUI.Font->Y-1,BColor);	//����������ʾΪ����ɫ 			
//				GUI_DrawFillRectangle(X1,Y1+GUI.Font->Y-1,X2,Y2,BColor);	//����������ʾΪ����ɫ 					
//			
//				break;
//			case TopMiddle:	
//				X=(X1+X2+1)/2-Len*4+1;
//				Y=Y1;	
//				GUI_DrawFillRectangle(X1,Y1,X,Y1+GUI.Font->Y-1,BColor);	//����������ʾΪ����ɫ 		
//				GUI_DrawFillRectangle(X+Len*8,Y1,X2,Y1+GUI.Font->Y-1,BColor);	//����������ʾΪ����ɫ 			
//				GUI_DrawFillRectangle(X1,Y1+GUI.Font->Y-1,X2,Y2,BColor);	//����������ʾΪ����ɫ 		

//			case Middle:	
//				X=(X1+X2+1)/2-Len*4+1;
//				Y=(Y1+Y2+1-16)/2;	
//				GUI_DrawFillRectangle(X1,Y1,X,Y2,BColor);	//����������ʾΪ����ɫ 		
//				GUI_DrawFillRectangle(X1,Y1,X2,Y,BColor);	//����������ʾΪ����ɫ 			
//				GUI_DrawFillRectangle(X+Len*8-1,Y1,X2,Y2,BColor);	//����������ʾΪ����ɫ 
//				GUI_DrawFillRectangle(X1,Y+GUI.Font->Y-1,X2,Y2,BColor);	//����������ʾΪ����ɫ



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













////��ָ��λ�û�һ��ָ����С��Բ
////(x,y):���ĵ�
////r    :�뾶
//void GUI_DrawCircle(u16 x0,u16 y0,u8 r,u16 FColor)
//{
//	int a,b;
//	int di;
//	a=0;b=r;	  
//	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
//		//ʹ��Bresenham�㷨��Բ     
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



