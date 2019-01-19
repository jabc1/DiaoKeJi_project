/*********************************************************************************
* �ļ���		: motion_control.h
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ���庯��						  
* �÷�			��
* �汾˵��	��
**********************************************************************************/

#ifndef __TFT_CONTROL_H__
#define __TFT_CONTROL_H__
#include "stm32f10x.h" 
#include <math.h>
#include "g_code.h"
typedef struct
{
	u16 X;		//��ǰ����
	u16 Y;
	u32 XT;		//Ŀ������
	u32 YT;	
}TFTStruct;



extern void InitShowTFT(s32 X,s32 Y);
extern u8 ShowRun(GCodeStruct* GCode_One,u16  Color);


#endif 

