#include "os.h"
#include "queue.h"

DataQueue GCodeQueue;						//GCODE�����б�
GCodeStruct Line[LINESIZE];			//��������




/********************************************
������InitializeGCodeQueue
���ܣ���ʼ�������б�
��������
���أ���
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
������AddGCodeQueueEntries
���ܣ�����1�����ݵ��б�
��������
���أ���
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
		while(GCodeQueue.Entries>=LINESIZE)			//�����������ȴ�����
		{
			OS.DelayTimeTick(1);
		}
}

/********************************************
������SubGCodeQueueEntries
���ܣ����1�����ݵ��б�
��������
���أ���
********************************************/
void SubGCodeQueueEntries(void)
{
		while(GCodeQueue.Entries==0)			//�����ѿգ��ȴ�����
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









