/*********************************************************************************
* 文件名		: motion_control.h
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 定义函数						  
* 用法			：
* 版本说明	：
**********************************************************************************/

#ifndef __TFT_CONTROL_H__
#define __TFT_CONTROL_H__
#include "stm32f10x.h" 
#include <math.h>
#include "g_code.h"
typedef struct
{
	u16 X;		//当前坐标
	u16 Y;
	u32 XT;		//目标坐标
	u32 YT;	
}TFTStruct;



extern void InitShowTFT(s32 X,s32 Y);
extern u8 ShowRun(GCodeStruct* GCode_One,u16  Color);


#endif 

