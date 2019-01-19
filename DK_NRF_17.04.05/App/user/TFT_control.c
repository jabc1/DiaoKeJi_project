

#include "app.h"
#include "DEVICE_CONF.h"
#include "TFT_control.h"
//#include "stdlib.h"
#define MAXTFTSIZE		200
TFTStruct TFT;
u16 Get_ZB(u32 XY)
{
	u32 t;
	t= XY*DK.SYS.TFTShowSize/200;
	if(t>=MAXTFTSIZE) t = MAXTFTSIZE;
	return t;
	
}
void InitShowTFT(s32 X,s32 Y)
{
	TFT.X = Get_ZB(X);
	TFT.Y =MAXTFTSIZE-  Get_ZB(Y);	
}
u8 ShowRun(GCodeStruct* GCode_One,u16  Color)//main中调用的线程
{
	if(GetBit(GCode_One->ValueWords,WORD_X)){TFT.XT =Get_ZB(GCode_One->X);}
	else{TFT.XT =TFT.X;}
	if(GetBit(GCode_One->ValueWords,WORD_Y)){TFT.YT =239-  Get_ZB(GCode_One->Y);}
	else{TFT.YT =TFT.Y;}	
	
	
	
	
	switch(GCode_One->Action)
	{
		case GC_SEEK_G0: 
//					TFT.XT =Get_ZB(GCode_One->X);
//					TFT.YT =239-  Get_ZB(GCode_One->Y);					
				GUI_DrawLine(TFT.X,TFT.Y,TFT.XT,TFT.YT,0xFFFF-Color);
				TFT.X = TFT.XT;
				TFT.Y = TFT.YT;			
		
		
//					TFT.X = Get_ZB(GCode_One->X);
//					TFT.Y =239-  Get_ZB(GCode_One->Y);			
			break;
		case GC_LINEAR_G1: 
//			if(GCode_One->F ==0)
//			{
//					TFT.XT =Get_ZB(GCode_One->X);
//					TFT.YT =239-  Get_ZB(GCode_One->Y);		
//需加位置判断		
				GUI_DrawLine(TFT.X,TFT.Y,TFT.XT,TFT.YT,Color);
				TFT.X = TFT.XT;
				TFT.Y = TFT.YT;
//			}

			break;
		case GC_CW_ARC:
		case GC_CCW_ARC:
//					TFT.XT = Get_ZB(GCode_One->X);
//					TFT.YT = 239- Get_ZB(GCode_One->Y);					
				GUI_DrawLine(TFT.X,TFT.Y,TFT.XT,TFT.YT,Color);	
				TFT.X = TFT.XT;
				TFT.Y = TFT.YT;		
			break;
		
		default:;//
			break;			
	}
	return 0;
}




















