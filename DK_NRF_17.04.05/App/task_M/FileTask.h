/*********************************************************************************
* �ļ���		: menu.h
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ���庯��						  
* �÷�			��
* �汾˵��	��
**********************************************************************************/
#ifndef __FILETASK_H__
#define __FILETASK_H__
#include "stm32f10x.h" 











#define LINESIZE			20

typedef struct 
{
	GCodeStruct    * Start;    //ָ����п�ʼ 
	GCodeStruct    * End;      //ָ����н��� 
	GCodeStruct    * In;       //����һ����Ϣ  
	GCodeStruct    * Out;      //ȡ��һ����Ϣ  
	u8    Size;     // ���г��� 
	u8    Entries;  //��Ϣ����      
}DataQueue;
extern DataQueue GCodeQueue;
extern u32 FileRead;
extern u8* SDdatabuff_T;
extern u32 FileRead;
extern u16 FileBuffNum ;

extern u8 OneLine[64];




void ReadFile(void);
u8 ReadOneLine(void);






void InitializeGCodeQueue(void);
void AddGCodeQueueEntries(void);
void SubGCodeQueueEntries(void);



extern void InitializeCtrl(void);
extern void FileTask(void);
#endif 

