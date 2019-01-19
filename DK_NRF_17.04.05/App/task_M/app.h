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
#include "ComTask.h"
#include "dk_conf.h"
#include "NRFTask.h"
//#include "MotoTask.h"
#include "CTRLTask.h"
#include "FileTask.h"
#include "showTask.h"
#define UseMessage_FileOpen				0xEF
#define UseMessage_CommConnect		0xEE
#define UseMessage_ShowDKState		0xEC

#define UseMessage_NRF						0xED
#define UseMessage_DingWei				0xEB
#define UseMessage_SetSpeed				0xEA


extern u8 NRFTaskPriority ;		/*NRF��������*/
//extern u8 MotoTaskPriority ;		/*NRF��������*/
extern u8 ShowTaskPriority ;		/*NRF��������*/
extern u8 FileTaskPriority;
#endif 

