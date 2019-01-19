#include "os.h"
#include "queue.h"

DataQueue GCodeQueue;						//GCODE队列列表
GCodeStruct Line[LINESIZE];			//队列数组




/********************************************
函数：InitializeGCodeQueue
功能：初始化队列列表
参数：无
返回：无
********************************************/
void InitializeGCodeQueue(void)	
{
    GCodeQueue.Start = &Line[0];
    GCodeQueue.End = &Line[LINESIZE];
    GCodeQueue.In = GCodeQueue.Start;
    GCodeQueue.Out = GCodeQueue.Start;
    GCodeQueue.Size = LINESIZE;
    GCodeQueue.Entries = 0;
}
/********************************************
函数：AddGCodeQueueEntries
功能：增加1列数据到列表
参数：无
返回：无
********************************************/
void AddGCodeQueueEntries(void)
{
    EnterCritical();	
    *GCodeQueue.In++;	
		GCodeQueue.Entries++;	

    if (GCodeQueue.In == GCodeQueue.End)
		{			
			GCodeQueue.In = GCodeQueue.Start;
		}
    ExitCritical();	
		while(GCodeQueue.Entries>=LINESIZE)			//队列已满，等待队列
		{
			OS.DelayTimeTick(1);
		}
}

/********************************************
函数：SubGCodeQueueEntries
功能：清除1列数据到列表
参数：无
返回：无
********************************************/
void SubGCodeQueueEntries(void)
{
		while(GCodeQueue.Entries==0)			//队列已空，等待队列
		{
			OS.DelayTimeTick(1);
		}		
    EnterCritical();			
    *GCodeQueue.Out++;	
		GCodeQueue.Entries--;	
    if (GCodeQueue.Out == GCodeQueue.End)
		{			
			GCodeQueue.Out = GCodeQueue.Start;
		}
    ExitCritical();	
}









