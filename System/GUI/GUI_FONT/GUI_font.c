/*********************************************************************************
* �ļ���			: GUI_font.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/01/10
* ˵��      : LCD������  
* �÷�			��

**********************************************************************************/

#include "GUI_conf.h"
#include "GUI_font.h"

#ifdef FONT_USE_4X6
	#include "F4x6.c"
	GUI_FontStruct Font4x6=
	{
		4,
		0,
		6,
		0x20,
		6,
		0,
		(u8 const *)&acFont4x6
	};
#endif

#ifdef FONT_USE_6X8
	#include "F6x8.c"		
	GUI_FontStruct Font6x8=
	{
		6,
		0,
		8,
		0x20,
		8,
		0,
		(u8 const *)&acFont6x8
	};
#endif
#ifdef FONT_USE_8X12
	#include "F8x12.c"
	#if(FONT_USE	==	FONT_USE_ZIMO)	
		#include "F12x12GB.c"			
	#endif	
	#if(FONT_USE	== FONT_USE_25WXX)		
		const u32 Font12Address[]=0X0;			// ��ʵ�ʵ�ַ����
	#endif	
	#if(FONT_USE	== FONT_USE_SD)			
		const u8 Font12Address[]={"0:/SYSTEM/FONT/GBK12.FON"};
	#endif			
	GUI_FontStruct Font8x12=
	{
		8,
		12,
		12,
		0x20,
		12,	
		24,	
		(u8 const *)&acFont8x12,				
		#if(FONT_USE	==	FONT_USE_SD)
			(u8 const *)&Font12Address,
		#endif				
		#if(FONT_USE	== FONT_USE_25WXX)		
			(u8 const *)&Font12Address,
		#endif				
		#if(FONT_USE	==	FONT_USE_ZIMO)		
			(u8 const *)&acFont12x12GB,
		#endif
	};	
#endif
	
#ifdef FONT_USE_8X16
	#include "F8x16.c"		
	#if(FONT_USE	==	FONT_USE_ZIMO)	
		#include "F16x16GB.c"		
	#endif	
	#if(FONT_USE	== FONT_USE_25WXX)		
		const u32 Font16Address[]=0X0;			// ��ʵ�ʵ�ַ����
	#endif	
	#if(FONT_USE	== FONT_USE_SD)			
		const u8 Font16Address[]={"0:/SYSTEM/FONT/GBK16.FON"};
	#endif			
	GUI_FontStruct Font8x16=
	{
		8,
		16,
		16,
		0x20,
		16,
		32,
		(u8 const *)&acFont8x16,				
		#if(FONT_USE	==	FONT_USE_SD)
			(u8 const *)&Font16Address,
		#endif				
		#if(FONT_USE	== FONT_USE_25WXX)		
			(u8 const *)&Font16Address,
		#endif				
		#if(FONT_USE	==	FONT_USE_ZIMO)		
			(u8 const *)&acFont16x16GB,
		#endif			
	};		
#endif

#ifdef FONT_USE_16X24
	#include "F16x24.c"			
	#if(FONT_USE	==	FONT_USE_ZIMO)	
//		#include "F16x24GB.c"		
	#endif	
	#if(FONT_USE	== FONT_USE_25WXX)		
		const u32 Font24Address[]=0X0;			// ��ʵ�ʵ�ַ����
	#endif	
	#if(FONT_USE	== FONT_USE_SD)			
		const u8 Font24Address[]={"0:/SYSTEM/FONT/GBK24.FON"};
	#endif	
	GUI_FontStruct Font16x24=
	{
		16,
		24,
		24,
		0x20,
		48,
		72,
		(u8 const *)&acFont16x24,	
		#if(FONT_USE	==	FONT_USE_SD)
			(u8 const *)&Font24Address,
		#endif				
		#if(FONT_USE	== FONT_USE_25WXX)		
			(u8 const *)&Font24Address,
		#endif				
		#if(FONT_USE	==	FONT_USE_ZIMO)		
//			(u8 const *)&acFont16x24GB,
		#endif				
	};
#endif

#if(FONT_USE	==	FONT_USE_SD)	
	#include "ff.h"
	#include "malloc.h"	
	
	FIL *FontFile;			//�ļ�1
	FRESULT Res;			//FatFsͨ�ý����
	UINT ByteReadNum;		//number of bytes read

	/****************************************************************************
	* ��    �ƣ�Get_HzMat
	* ��    �ܣ���ȡ���ֵ���
	* ��ڲ�����code���ַ�ָ�뿪ʼ��mat�����ݴ�ŵ�ַ��Font������
	* ���ڲ�������
	* ˵    ����
	****************************************************************************/
	void Get_Hz_SD(u8 *code,u8 *mat,GUI_FontStruct *Font)
	{		    
		unsigned char qh,ql;
		unsigned char i;					  
		unsigned long foffset; 
		FontFile = (FIL*)GUI_MEM_Malloc(sizeof(FIL));		//Ϊ�ļ�1�����ڴ�
		qh=*code;
		ql=*(++code);
		if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
		{   		    
				for(i=0;i<Font->GB_Num;i++)*mat++=0x00;//�������
				return; //��������
		}          
		if(ql<0x7f)ql-=0x40;//ע��!
		else ql-=0x41;
		qh-=0x81;   
		foffset=((unsigned long)190*qh+ql)*Font->GB_Num;	//�õ��ֿ��е��ֽ�ƫ����  
//INTX_DISABLE();		
		Res = f_open(FontFile, (const TCHAR *)Font->GBFont_P, FA_OPEN_EXISTING | FA_READ);		
		Res = f_lseek(FontFile, foffset); 
		Res = f_read(FontFile, mat, Font->GB_Num, &ByteReadNum);	//��ȡ�ļ�����
		Res = f_close(FontFile);	//�ر��ļ�	
//INTX_ENABLE();//�������ж�		
		GUI_MEM_Free(FontFile);			
	}  
#endif
#if(FONT_USE	==	FONT_USE_25WXX)		
	u8 *Get_Hz_25WXX(u8 *code,u8 *mat,GUI_FontStruct *Font)
	{			
	}
#endif	
#if(FONT_USE	==	FONT_USE_ZIMO)	
	/****************************************************************************
	* ��    �ƣ�Get_HzMat
	* ��    �ܣ���ȡ���ֵ���
	* ��ڲ�����code���ַ�ָ�뿪ʼ��mat�����ݴ�ŵ�ַ��Font������
	* ���ڲ�������
	* ˵    ����
	****************************************************************************/
	u8* Get_Hz_ZIMO(u8 *code,GUI_FontStruct *Font)
	{		
		u8 i;    
		u8 *GBFont;
		for (i=0;i<MAXGBFONTNUM;i++) //MAXGBFONTNUM��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
		{ 	
			GBFont=(u8*)&(Font->GBFont_P[i*(Font->GB_Num + 3)]);		
			if((GBFont[0]==code[0])&&(GBFont[1]==code[1]))
			{
				GBFont = GBFont +3;
				return GBFont;
			}	
		}	
		return 0;			
	}  
#endif
	
	
	
