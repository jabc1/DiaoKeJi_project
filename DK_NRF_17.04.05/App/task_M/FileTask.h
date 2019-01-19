/*********************************************************************************
* 文件名		: menu.h
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 定义函数						  
* 用法			：
* 版本说明	：
**********************************************************************************/
#ifndef __FILETASK_H__
#define __FILETASK_H__
#include "stm32f10x.h" 











#define LINESIZE			20

typedef struct 
{
	GCodeStruct    * Start;    //指向队列开始 
	GCodeStruct    * End;      //指向队列结束 
	GCodeStruct    * In;       //插入一个消息  
	GCodeStruct    * Out;      //取出一个消息  
	u8    Size;     // 队列长度 
	u8    Entries;  //消息长度      
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

