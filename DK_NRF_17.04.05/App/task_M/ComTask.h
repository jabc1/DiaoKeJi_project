/*********************************************************************************
* �ļ���		: menu.h
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ���庯��						  
* �÷�			��
* �汾˵��	��
**********************************************************************************/
#ifndef __COMTASK_H__
#define __COMTASK_H__
#include "stm32f10x.h" 


u8 CommReadOneLine(u8 *Rcv_Buff_P,u16 *Num,u8* OneLine_P);


extern void SendUsart1(void);
extern void InitializeComm(void);
extern void CommTask(void);
#endif 

