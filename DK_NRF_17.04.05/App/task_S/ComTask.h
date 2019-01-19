/*********************************************************************************
* 文件名		: menu.h
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 定义函数						  
* 用法			：
* 版本说明	：
**********************************************************************************/
#ifndef __COMTASK_H__
#define __COMTASK_H__
#include "stm32f10x.h" 


u8 CommReadOneLine(u8 *Rcv_Buff_P,u16 *Num,u8* OneLine_P);


extern void SendUsart1(void);
extern void InitializeComm(void);
extern void CommTask(void);
#endif 

