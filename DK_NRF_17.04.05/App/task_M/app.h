/*
********************************************************************************
*                                嵌入式微系统
*                                   msOS
*                                作者:王绍伟
*                                网名:凤舞天
*                                标识:Wangsw
********************************************************************************
*文件名     : app.h
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/13       初始版本
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


extern u8 NRFTaskPriority ;		/*NRF解析任务*/
//extern u8 MotoTaskPriority ;		/*NRF解析任务*/
extern u8 ShowTaskPriority ;		/*NRF解析任务*/
extern u8 FileTaskPriority;
#endif 

