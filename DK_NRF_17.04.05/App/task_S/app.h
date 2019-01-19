/*
********************************************************************************
*                                Ƕ��ʽ΢ϵͳ
*                                   msOS
*                                ����:����ΰ
*                                ����:������
*                                ��ʶ:Wangsw
********************************************************************************
*�ļ���     : app.h
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/13       ��ʼ�汾
*
********************************************************************************
*/


#ifndef __APP_H
#define __APP_H

#include "stm32f10x.h"  
//#include "ComTask.h"
#include "dk_conf.h"
#include "NRFTask.h"
#include "MotoTask.h"
#include "ctrlTask.h"
#define UseMessage_DK_RUN			0xEF
#define UseMessage_DK_ZERO			0xEC
#define UseMessage_DK_SPEED			0xEB
#define UseMessage_DK_DW				0xEA

#define UseMessage_PlanRun			0xDF



extern u8 CtrlTaskPriority ;		/*ͨ������*/
//extern u8 ComTaskPriority ;		/*ͨ������*/
extern u8 NRFTaskPriority ;		/*NRF��������*/
extern u8 MotoTaskPriority ;		/*moto��������*/

#endif 

