/*********************************************************************************
*�ļ��� 		: motion_ctrl.c
*����			��ZZ
*�汾  		: V3.0.0
*����   	: 2017.01.16
*�汾˵��	:�˶�����,��G������н��������Ե��������Ӧ����
***********************************************************************************/

#ifndef __MOTION_CONTROL_H__
#define __MOTION_CONTROL_H__
#include "stm32f10x.h" 
//#include <math.h>
#include "dk_conf.h"	

#define CB_X 1				//X����в岹
#define CB_Y 2				//Y����в岹
//ֱ�߲岹�����ṹ��
typedef struct
{
	s32 X_Target;
	s32 X_Current;
	s32 Y_Target;
	s32 Y_Current;	
	u16 zN;//�岹�ܲ���
	u16 Xe;//X����Ӧ��������
	u16 Ye;//Y����Ӧ��������
	s16 F; //��ƫ�����
	u8  Quadrant;//���ͱ�־λ��������ֵ L1 2 3 4��
//	u8  DIR;//�岹��������
	u8  DirX;//�岹��������	
	u8  DirY;//�岹��������	
}LINEAR;
//Բ���岹�����ṹ��
#define SR 1	//˳Բ ��־
#define NR 2	//��Բ ��־
typedef struct
{
	s32 X_Target;
	s32 X_Current;
	s32 Y_Target;
	s32 Y_Current;	
	s32 I;//Բ����������
	s32 J;/*��Բ������-Բ����������ֵ*/
	u16 zN;//�岹�ܲ���
	u16 Xe;//X����Ӧ��������
	u16 Ye;//Y����Ӧ��������
	s16 F; //��ƫ�����
	u8  Quadrant;//���ޱ�־λ��������ֵ SRx NRx��
	u8  DIR;//�岹��������	
	u8 type;	//˳ ��Բ ��־λ

//	XY Centre;//Բ������
	
}CIRCULA;

//ֱ�߲岹�����б��־λ
#define L1 1	//��һ����
#define L2 2	//  ��
#define L3 3	//  ��
#define L4 4	//  ��
//Բ���岹�����б��־λ
//Clockwise˳ʱ�� AnticloCkwise��ʱ��
#define SR1	1		//��һ���� ˳Բ     
#define NR1 5	//��һ���� ��Բ
#define SR2 2		//  ��
#define NR2 6
#define SR3 3
#define NR3 7
#define SR4 4
#define NR4 8
/////////////////////////////////////////////////////////////////
/*�������ڵĶ���ʵ���˺����ķ�װ��main��ֻ��Ҫ����Interpolation_Running(void);*/





//extern struct_Interp itp;


void MotoGotoZero(void);


//Բ���岹 ����ֵΪ0����������	����ֵΪ1��������
u8 Circula_Interpolation(GCodeStruct* GCode_One);


//���ٶ�λ
u8 Fast_Seek(GCodeStruct* GCode_One);
//ֱ�߲岹 ����ֵΪ0����������	����ֵΪ1��������
u8 Linear_Interpolation(GCodeStruct* GCode_One);

u8 Interpolation_Process(GCodeStruct* GCode_One);


#endif 

