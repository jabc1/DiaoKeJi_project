/*********************************************************************************
*文件名 		: motion_ctrl.c
*作者			：ZZ
*版本  		: V3.0.0
*日期   	: 2017.01.16
*版本说明	:运动控制,对G代码进行解析，并对电机做出相应控制
***********************************************************************************/

#ifndef __MOTION_CONTROL_H__
#define __MOTION_CONTROL_H__
#include "stm32f10x.h" 
//#include <math.h>
#include "dk_conf.h"	

#define CB_X 1				//X轴进行插补
#define CB_Y 2				//Y轴进行插补
//直线插补参数结构体
typedef struct
{
	s32 X_Target;
	s32 X_Current;
	s32 Y_Target;
	s32 Y_Current;	
	u16 zN;//插补总步数
	u16 Xe;//X坐标应进给步数
	u16 Ye;//Y坐标应进给步数
	s16 F; //新偏差计算
	u8  Quadrant;//线型标志位（用来赋值 L1 2 3 4）
//	u8  DIR;//插补进给方向
	u8  DirX;//插补进给方向	
	u8  DirY;//插补进给方向	
}LINEAR;
//圆弧插补参数结构体
#define SR 1	//顺圆 标志
#define NR 2	//逆圆 标志
typedef struct
{
	s32 X_Target;
	s32 X_Current;
	s32 Y_Target;
	s32 Y_Current;	
	s32 I;//圆心增量坐标
	s32 J;/*即圆心坐标-圆弧起点坐标的值*/
	u16 zN;//插补总步数
	u16 Xe;//X坐标应进给步数
	u16 Ye;//Y坐标应进给步数
	s16 F; //新偏差计算
	u8  Quadrant;//象限标志位（用来赋值 SRx NRx）
	u8  DIR;//插补进给方向	
	u8 type;	//顺 逆圆 标志位

//	XY Centre;//圆心坐标
	
}CIRCULA;

//直线插补象限判别标志位
#define L1 1	//第一象限
#define L2 2	//  二
#define L3 3	//  三
#define L4 4	//  四
//圆弧插补象限判别标志位
//Clockwise顺时针 AnticloCkwise逆时针
#define SR1	1		//第一象限 顺圆     
#define NR1 5	//第一象限 逆圆
#define SR2 2		//  二
#define NR2 6
#define SR3 3
#define NR3 7
#define SR4 4
#define NR4 8
/////////////////////////////////////////////////////////////////
/*该区域内的定义实现了函数的封装，main中只需要调用Interpolation_Running(void);*/





//extern struct_Interp itp;


void MotoGotoZero(void);


//圆弧插补 返回值为0：正常结束	返回值为1：超量程
u8 Circula_Interpolation(GCodeStruct* GCode_One);


//快速定位
u8 Fast_Seek(GCodeStruct* GCode_One);
//直线插补 返回值为0：正常结束	返回值为1：超量程
u8 Linear_Interpolation(GCodeStruct* GCode_One);

u8 Interpolation_Process(GCodeStruct* GCode_One);


#endif 

