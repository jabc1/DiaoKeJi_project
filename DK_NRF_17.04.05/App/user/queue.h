#ifndef __QUEUE_H
#define __QUEUE_H	 
#include "stm32f10x.h"	
#include "dk_conf.h"


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

void InitializeGCodeQueue(void);
void AddGCodeQueueEntries(void);
void SubGCodeQueueEntries(void);
		 					    
#endif

