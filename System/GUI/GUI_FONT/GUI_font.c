/*********************************************************************************
* 文件名			: GUI_font.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/01/10
* 说明      : LCD用字体  
* 用法			：

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
		const u32 Font12Address[]=0X0;			// 按实际地址设置
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
		const u32 Font16Address[]=0X0;			// 按实际地址设置
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
		const u32 Font24Address[]=0X0;			// 按实际地址设置
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
	
	FIL *FontFile;			//文件1
	FRESULT Res;			//FatFs通用结果码
	UINT ByteReadNum;		//number of bytes read

	/****************************************************************************
	* 名    称：Get_HzMat
	* 功    能：获取汉字点阵
	* 入口参数：code：字符指针开始；mat：数据存放地址；Font：字体
	* 出口参数：无
	* 说    明：
	****************************************************************************/
	void Get_Hz_SD(u8 *code,u8 *mat,GUI_FontStruct *Font)
	{		    
		unsigned char qh,ql;
		unsigned char i;					  
		unsigned long foffset; 
		FontFile = (FIL*)GUI_MEM_Malloc(sizeof(FIL));		//为文件1申请内存
		qh=*code;
		ql=*(++code);
		if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
		{   		    
				for(i=0;i<Font->GB_Num;i++)*mat++=0x00;//填充满格
				return; //结束访问
		}          
		if(ql<0x7f)ql-=0x40;//注意!
		else ql-=0x41;
		qh-=0x81;   
		foffset=((unsigned long)190*qh+ql)*Font->GB_Num;	//得到字库中的字节偏移量  
//INTX_DISABLE();		
		Res = f_open(FontFile, (const TCHAR *)Font->GBFont_P, FA_OPEN_EXISTING | FA_READ);		
		Res = f_lseek(FontFile, foffset); 
		Res = f_read(FontFile, mat, Font->GB_Num, &ByteReadNum);	//读取文件内容
		Res = f_close(FontFile);	//关闭文件	
//INTX_ENABLE();//开启总中断		
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
	* 名    称：Get_HzMat
	* 功    能：获取汉字点阵
	* 入口参数：code：字符指针开始；mat：数据存放地址；Font：字体
	* 出口参数：无
	* 说    明：
	****************************************************************************/
	u8* Get_Hz_ZIMO(u8 *code,GUI_FontStruct *Font)
	{		
		u8 i;    
		u8 *GBFont;
		for (i=0;i<MAXGBFONTNUM;i++) //MAXGBFONTNUM标示自建汉字库中的个数，循环查询内码
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
	
	
	
