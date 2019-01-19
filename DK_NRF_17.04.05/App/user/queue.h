#ifndef __QUEUE_H
#define __QUEUE_H	 
#include "stm32f10x.h"	
#include "dk_conf.h"


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

void InitializeGCodeQueue(void);
void AddGCodeQueueEntries(void);
void SubGCodeQueueEntries(void);
		 					    
#endif

