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



extern u8 CtrlTaskPriority ;		/*通信任务*/
//extern u8 ComTaskPriority ;		/*通信任务*/
extern u8 NRFTaskPriority ;		/*NRF解析任务*/
extern u8 MotoTaskPriority ;		/*moto解析任务*/

#endif 

