/*
*�ļ���   : os.c
*����     : uC/OS-II�������
*����ʱ�� 	: 2013.7.21
*˵��			����ʼ�汾��������ΰ������:�����죻��ʶ:wangsw����msOSǶ��ʽ΢ϵͳ
*�汾			: V2.1
*2016.12.29 �޸��������ջ�����Ƿ���ͬ������ʹ�ú궨��������
*2015.10.24 1.������	uint8_t OS_IsUseMutex(uint8_t Num)
										uint8_t OSMutexLock (uint8_t OSMutexNum)
										void  OSMutexUnLock (uint8_t OSMutexNum)
						  3���������������������������8������state�еĸ�8λ����ʾ
						2.ȡ����Event�ṹ�壬�����Eventָ������������ȼ������棬��д��create��pend��post����
*2015.10.08 ������ PopQueue��PushQueue 2������
*						�޸��˲���������ȼ����޷�ͬʱpost2����Ϣ��BUG,
*						�޸���PendMessageQueue��PostMessageQueue����
********************************************************************************
*/

#include "os.h"
#include "core_cm3.h"
//***************************************************************************************
//����״̬�� TASK STATUS (state��λ����)       
#define  TaskStatusReady            0x00        					/* ���ھ���״̬   */                         
#define  TaskStatusMessageBox       0x02        					/* ��������״̬  */
#define  TaskStatusMessageQueue     0x04        					/* ����Q״̬*/



//****************************************************************************************
//�¼�����(OS_EVENT types)
#define  EventTypeUnused						0      						/* ���¼�����*/
#define  EventTypeMessageBox	   		1							    /* ����Ϊ��������*/
#define  EventTypeMessageQueue      2							    /* ��Ϣ����Ϊ��������*/

#if TaskSum == 2        //˫������ұ��ʺ�˫�����µ�msOS
	static const uint8_t UnmapTable[] = 
	{
			0, 0, 
			1, 0
	};
#endif
#if TaskSum == 3 
	static const uint8_t UnmapTable[] = 
	{
			0, 0, 1, 0,
			2, 0, 1, 0	
	};
#endif
#if TaskSum == 4        //4������ұ�
	static const uint8_t UnmapTable[] = 
	{
			0, 0, 1, 0, 2, 0, 1, 0,	
			3, 0, 1, 0, 2, 0, 1, 0	
	};
	#endif
#if TaskSum > 4                //��������ұ�����uC/OS-II���
	static const uint8_t UnmapTable[] = 
	{
			0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F */
			5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F */
			6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F */
			5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F */
			7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F */
			5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF */
			6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF */
			5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF */
			4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        /* 0xF0 to 0xFF */
	};
#endif
	
	
	
OSStruct OS;									//OS��ϵͳ�ṹ��
#if (TaskStackSumIsSame == 0)
	static uint32_t  	*TaskStack[TaskSum];    							//����ջ����	
	#if TaskSum > 0                //��������ұ�����uC/OS-II���
	uint32_t TaskStack0[TaskStackSum0];
	#endif
	#if TaskSum > 1 
	uint32_t TaskStack1[TaskStackSum1];
	#endif
	#if TaskSum > 2 
	uint32_t TaskStack2[TaskStackSum2];	
	#endif
	#if TaskSum > 3
	uint32_t TaskStack3[TaskStackSum3];
	#endif
	#if TaskSum > 4 
	uint32_t TaskStack4[TaskStackSum4];	
	#endif	
	#if TaskSum > 5 
	uint32_t TaskStack5[TaskStackSum5];	
	#endif
	#if TaskSum > 6 
	uint32_t TaskStack6[TaskStackSum6];
	#endif
	#if TaskSum > 7 
	uint32_t TaskStack7[TaskStackSum7];	
	#endif		
	
#endif	
#if (TaskStackSumIsSame == 1)	
	static uint32_t  	TaskStack[TaskSum][TaskStackSum];    							//����ջ����	
#endif


static void *  TaskMessageQueue[TaskSum-1][QueueStackSum];           //��Ϣ����	
		
uint16_t CriticalNesting = 0;
static uint8_t InterruptNesting = 0;				        /*�����ж�Ƕ�ײ���*/
static uint8_t PriorityReadyTable = 0;		                /*������*/
static uint8_t TaskCounter = 0;					        /*���������*/
static Task  TaskBlock[TaskSum];				        /*������ƿ�*/
static Queue QueueBlock[TaskSum-1];                       /*��Ϣ���п��ƿ�*/

uint8_t    OsRunning = OS_false;                       		/*�������Ѿ���ʼ=1,�����ڲ�����״̬=0*/
uint8_t    OsCurrentPriority = 0;                		/*�����������е���������ȼ�*/
uint8_t    OsHighReadyPriority = 0;			            /*�������������ȼ���ľ�����������ȼ�*/
Task *	OsCurrentTaskPointer = (Task *)0;		        /*����ָ����������������ƿ��ָ��*/
Task *	OsHighReadyTaskPointer = (Task *)0;	            /*����ָ����߼����ȼ�����������ƿ��ָ��*/


static uint32_t * InitializeTaskStack (void  (*taskPointer)(void), uint32_t  *stackTopPointer)
{
    *(stackTopPointer) = (uint32_t)0x01000000L;           /*  xPSR                        */ 
    *(--stackTopPointer) = (uint32_t)taskPointer;           /*  Entry Point of the task     */
                                                    /*  ������ڵ�ַ                */
    *(--stackTopPointer) = (uint32_t)0xFFFFFFFEL;         /*  R14 (LR)  (init value will  */
                                                    /*  cause fault if ever used)   */                                                                           
    *(--stackTopPointer) = (uint32_t)0x12121212L;         /*  R12                         */
    *(--stackTopPointer) = (uint32_t)0x03030303L;         /*  R3                          */
    *(--stackTopPointer) = (uint32_t)0x02020202L;         /*  R2                          */
    *(--stackTopPointer) = (uint32_t)0x01010101L;         /*  R1                          */
    *(--stackTopPointer) = (uint32_t)0x00000000L;         /*  R0 : argument  �������     */
                                                    /*  Remaining registers saved on*/
                                                    /*  process stack ʣ�µļĴ������浽��ջ*/                                                 
    *(--stackTopPointer) = (uint32_t)0x11111111L;         /*  R11                         */
    *(--stackTopPointer) = (uint32_t)0x10101010L;         /*  R10                         */
    *(--stackTopPointer) = (uint32_t)0x09090909L;         /*  R9                          */
    *(--stackTopPointer) = (uint32_t)0x08080808L;         /*  R8                          */
    *(--stackTopPointer) = (uint32_t)0x07070707L;         /*  R7                          */
    *(--stackTopPointer) = (uint32_t)0x06060606L;         /*  R6                          */
    *(--stackTopPointer) = (uint32_t)0x05050505L;         /*  R5                          */
    *(--stackTopPointer) = (uint32_t)0x04040404L;         /*  R4                          */
    return(stackTopPointer);
}


extern void     OsSwitchContext(void);

extern void     OsStartHighReadyTask(void);

static void InitializeTask (uint32_t *stackTopPointer)
{
    Task * taskPointer;
		
    taskPointer = &TaskBlock[TaskCounter];
	
    taskPointer->StackTopPointer  = stackTopPointer;                       	/* Load Stack pointer in TCB                */
    taskPointer->MessagePointer   = OS_null;                			/* No message received                      */
    taskPointer->Delay          = 0;                          			/* Task is not delayed                      */
    taskPointer->State          = TaskStatusReady;                	/* Task is ready to run                     */
    taskPointer->Priority       = TaskCounter;

    PriorityReadyTable  |= (1 << TaskCounter);
//    TaskCounter++;
}


  
static void Schedule (void)
{ 
    EnterCritical();
    if (InterruptNesting == 0)
//    if ((InterruptNesting == 0) && (LockNesting == 0))			
    {
        OsHighReadyPriority = UnmapTable[PriorityReadyTable];//��ȡ������ȼ�
        if (OsHighReadyPriority != OsCurrentPriority)//��⵱ǰ�Ƿ�������м�
        {                                            //��ֵ������ȼ�����ָ��
            OsHighReadyTaskPointer = &TaskBlock[OsHighReadyPriority];
            OsSwitchContext();                                     //�����л�
        }
    }
    ExitCritical();
}

static void EnterInterrupt (void)
{
    if (OsRunning == OS_true) 
    {
        InterruptNesting++;
    }
}

static void ExitInterrupt (void)
{
    if (OsRunning == OS_true) 
    {
        EnterCritical();
        if (InterruptNesting > 0) 
        {                       
            InterruptNesting--;
        }
//        if ((InterruptNesting == 0) && (LockNesting == 0)) 
				if (InterruptNesting == 0)
        {
            OsHighReadyPriority = UnmapTable[PriorityReadyTable];
					
            if (OsHighReadyPriority != OsCurrentPriority) 
            {              //3
                OsHighReadyTaskPointer = &TaskBlock[OsHighReadyPriority];//4
                OsSwitchContext();
            }
        }
        ExitCritical();	
    }
}

static void SystemTickRoutine(void)
{ 
    uint8_t i = 0;
    Task *taskPointer; 
    
    EnterInterrupt();
    if (OsRunning == OS_true) 
    {                                    
        while (i < TaskSum)
        {          
            taskPointer = &TaskBlock[i];  
            EnterCritical();
            if (taskPointer->Delay != 0) 
            {
                if (--taskPointer->Delay == 0) 
                {
                    if ((taskPointer->State & TaskStatusSuspend) == TaskStatusReady) 
                        PriorityReadyTable |= (1 << i);
                    else 
                        taskPointer->Delay = 1;
                }
            }
            i++;
            ExitCritical();
        }
    }
    ExitInterrupt();
}






/*******************************************************************************
* ������	: CreateMessageQueue
* ����	    : ����һ����Ϣ���У���Ϣ���п��Դ洢�����Ϣ��ռ��һ���¼��鼰
*             һ������,EventSum����MessageBox��MessageQueue���ߵ�����,QueueSum
*             ����MessageQueue������
* �������  : start:����ָ��ͷ��size�����г���
* ���ز���  : �¼���ָ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void CreateMessageQueue (void * * start, uint16_t size)
{
    Queue * queuePointer;
    Task *TaskPointer;
		
    queuePointer = &QueueBlock[TaskCounter];
    queuePointer->Start        = start;
    queuePointer->End          = &start[size];
    queuePointer->In           = start;
    queuePointer->Out          = start;
    queuePointer->Size         = size;
    queuePointer->Entries      = 0;	
		
    TaskPointer = &TaskBlock[TaskCounter];
    TaskPointer->QueuePointer = queuePointer;    			
    return ;
}
/*******************************************************************************
* ������	: CreateTask
* ����	    : �������񣬸��ݴ�����˳��������ȼ���Խ�紴���ģ����ȼ�Խ�ߣ�
*           : ��һ��Ϊ0���ȼ�,ע��MenuTask����Ϊ������ȼ���Ҳ�������һ��������
*           : �����˶��ٸ�����TaskSumҪ����Ϊ��Ӧ��ֵ��
* �������  : taskPointer: ������ָ��*           
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V2.0			ZZ					 2014/10/24				Ϊ�����÷��㣬ȡ���˵���������ջ����	���ã�ÿ�����������ջ��ͬ��С
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static uint8_t CreateTask (void (*taskPointer)(void))
{
    uint32_t * sp;
    EnterCritical();
		if(TaskCounter >= TaskSum) return 0xff;															//���������������������

		#if TaskStackSumIsSame == 0	
			sp = InitializeTaskStack(taskPointer, (TaskStack[TaskCounter]));	
		#endif	
		#if TaskStackSumIsSame == 1	
			sp = InitializeTaskStack(taskPointer, &(TaskStack[TaskCounter][TaskStackSum-1]));
		#endif
	

	
    InitializeTask(sp);																							//��������
	  CreateMessageQueue(TaskMessageQueue[TaskCounter], QueueStackSum);	//������Ϣ�б�
		TaskCounter++;	
    ExitCritical();
	return TaskCounter-1;
}
/*******************************************************************************
* ������	: PushQueue
* ����	    : ��Ϣ�б�ѹ����Ϣ������
* �������  : queuePointer: ��Ϣ����ָ�룻messagePointer����Ϣ        
* ���ز���  : �Ƿ�ɹ�
********************************************************************************
*�汾     ����            ����            ˵��
*V2.0			ZZ					 2014/10/24				
*******************************************************************************/

uint8_t  PushQueue(Queue *queuePointer,void *messagePointer)
{	
    if (queuePointer->Entries >= queuePointer->Size) 
    {                                                               /* Make sure queue is not full                   */
        return (OS_false);
    }
    *queuePointer->In++ = messagePointer;                               /* Insert message into queue                     */
    queuePointer->Entries++;                                          /* Update the nbr of entries in the queue        */
    if (queuePointer->In == queuePointer->End) 
    {                                                               /* Wrap IN ptr if we are at end of queue         */
       	queuePointer->In = queuePointer->Start;
    }
    return (OS_true);	
}
/*******************************************************************************
* ������	: PopQueue
* ����	    : ��ȡ��Ϣ�����е���Ϣ
* �������  : queuePointer: ��Ϣ����ָ�룻        
* ���ز���  : ��Ϣ
********************************************************************************
*�汾     ����            ����            ˵��
*V2.0			ZZ					 2014/10/24				
*******************************************************************************/
void *PopQueue(Queue *queuePointer)
{
	void *messagePointer;

	if(queuePointer->Entries == 0)
	{
		return 0;
	}
	messagePointer = *queuePointer->Out++;                     		/* Yes, extract oldest message from the queue         */ 	
	queuePointer->Entries--;                        				/* Update the number of entries in the queue          */
	
	if (queuePointer->Out == queuePointer->End) 
	{          
		queuePointer->Out = queuePointer->Start;			        /* Wrap OUT pointer if we are at the end of the queue */
	}
	return (messagePointer); 
}
/*******************************************************************************
* ������	: PendMessageQueue
* ����	    : �ȴ���Ϣ���У�����Ϣ����Ϊ��ʱ�������������
* �������  : eventPointer �����¼���ָ�룬timeout �ȴ�ʱ�䣬1mSΪ��λ
* ���ز���  : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.2			ZZ					 2013/10/08				�޸��˲���ͬʱpost2������������ȼ���BUG
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void * PendMessageQueue (uint32_t timeout)
{
    void * messagePointer;
    Queue * queuePointer;

//    Assert(InterruptNesting == 0);

    EnterCritical();
		
    queuePointer = OsCurrentTaskPointer->QueuePointer;             	/* Point at queue control block                       */
    if (queuePointer->Entries > 0) 							        /* See if any messages in the queue                   */
    {                    
				messagePointer = PopQueue(queuePointer);
       	ExitCritical();
       	return (messagePointer);                            			/* Return message received                            */
    }
    OsCurrentTaskPointer->State |= TaskStatusMessageQueue;		        /* Task will have to pend for a message to be posted  */
    OsCurrentTaskPointer->Delay   = timeout;              			    /* Load timeout into TCB                  */
    PriorityReadyTable &= ~(1 <<  OsCurrentPriority);		
    ExitCritical();
    Schedule();              /*�����л��ڵ㣬�����ǰû��message�Ļ����л����񣬵���message��ʱ��ʹ������л�����       */
    EnterCritical();
		queuePointer = OsCurrentTaskPointer->QueuePointer;
		messagePointer = PopQueue(queuePointer);
    if ((messagePointer != OS_null) || (timeout == 0))                   /*Wangsw add || (timeout == 0) for direct DataQueue */
    {                      										    /* Did we get a message?                              */
        OsCurrentTaskPointer->MessagePointer      = OS_null;     		    /* Extract message from TCB (Put there by QPost)      */
        OsCurrentTaskPointer->State     = TaskStatusReady;
        ExitCritical();
        return (messagePointer);                            		    /* Return message received                            */
    }    	
    OsCurrentTaskPointer->State = TaskStatusReady;
    ExitCritical();
    return (OS_null);                          					    /* No message received                                */
}



/*******************************************************************************
* ������	: PostMessageQueue
* ����	    : ����һ����Ϣ����Ϣ�����У����ڵȴ���������Զ�����
* �������  : eventPointer �����¼���ָ�룬messagePointer������Ϣָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.2			ZZ					 2013/10/08				�޸��˲���ͬʱpost2������������ȼ���BUG
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static uint8_t PostMessageQueue (uint8_t Priority, void *messagePointer)
{
    Queue * queuePointer;
		Task *taskPointer;	

//	  Assert(TaskCounter >= Priority);	
    EnterCritical();
	  taskPointer = &TaskBlock[Priority];
    queuePointer = taskPointer->QueuePointer;                 /* Point to queue control block                  */	
    if (queuePointer->Entries == 0x00) 				            /* ��ǰ�Ķ�������Ϣ����Ϊ0            */
    {                 
				PushQueue(queuePointer,messagePointer);								//����Ϣѹ�����
        taskPointer->Delay = 0;
        taskPointer->MessagePointer = messagePointer;
        taskPointer->State &= ~TaskStatusMessageQueue;			
        if (taskPointer->State == TaskStatusReady)		//�����ready״̬
        {
					PriorityReadyTable |=  1 << taskPointer->Priority;  
					ExitCritical();
					Schedule();                                                 /* Find highest priority task ready to run       */
					return (OS_true);          
        }
				PushQueue(queuePointer,messagePointer);		
        ExitCritical();
        return (OS_true);
    }
    if (queuePointer->Entries >= queuePointer->Size) 
    {                                                               /* Make sure queue is not full                   */
        ExitCritical();
        return (OS_false);
    }
		PushQueue(queuePointer,messagePointer);
    ExitCritical();
    return (OS_true);
}
/*******************************************************************************
* ������	: DelayTimeTick
* ����	    : �������ȴ�ʱ�䣬��λΪ1mS
* �������  : timeTick������ʱ�䣬1mSΪ��λ������1000,����1����
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void DelayTimeTick (uint32_t timeTick)
{
    if (timeTick > 0) 
    {                                                   
       	EnterCritical();
        PriorityReadyTable &= ~(1 << OsCurrentPriority);
       	OsCurrentTaskPointer->Delay = timeTick;
       	ExitCritical();
       	Schedule();
    }
}
/*******************************************************************************
* ������	: OS_IsUseMutex
* ����	    : �����Ƿ��л�����
* �������  : Num�����������        
* ���ز���  : ���������
********************************************************************************
*�汾     ����            ����            ˵��
*V2.0			ZZ					 2014/10/24				
*******************************************************************************/

uint8_t OS_IsUseMutex(uint8_t Num)
{
	uint8_t i;
	for(i=0;i<TaskSum;i++)
	{
		if(TaskBlock[i].State&(0x100<<Num) )return (i+1);
	}
	return 0;	
}
/*******************************************************************************
* ������	: OSMutexLock
* ����	    : ����������
* �������  : OSMutexNum�����������        
* ���ز���  : 0���ɹ�����0�����ɹ�
********************************************************************************
*�汾     ����            ����            ˵��
*V2.0			ZZ					 2014/10/24				
*******************************************************************************/
uint8_t OSMutexLock (uint8_t OSMutexNum)
{   //�������ж��е��øú���   
//	Assert(OSMutexNum <8);
	if(OSMutexNum > 7) return 1;			//mutex��Ŵ���7�Ļ�����	0-7 ��Ӧ8��ͨ��
	EnterCritical();			//�ر��ж�     
	
	if (!OS_IsUseMutex(OSMutexNum))				//�����־Ϊ0���ź�������  
	{
		OsCurrentTaskPointer->State |=(0x100<<OSMutexNum);     //���û����־
		ExitCritical();				//�����ж�
		return 0;
	}
	//�����־�ź��Ѿ���������λ��˵����ռ��
		OsCurrentTaskPointer->State |=(0x100<<OSMutexNum);     //���û����־
	PriorityReadyTable &= ~(1 <<  OsCurrentPriority);						//������ȼ�����
	ExitCritical();						//�����ж�	
	Schedule();              /*�����л��ڵ㣬�����ǰû��message�Ļ����л����񣬵���message��ʱ��ʹ������л�����       */
	return 0;
}

/*******************************************************************************
* ������	: OSMutexUnLock
* ����	    : ����������
* �������  : OSMutexNum�����������        
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V2.0			ZZ					 2014/10/24				
*******************************************************************************/
void  OSMutexUnLock (uint8_t OSMutexNum)
{   //�������ж��е��øú���
	uint8_t MutexTaskNum;
	uint8_t MutexHighPriority;  
	Task *taskPointer;
//	Assert(OSMutexNum < 8);	
	EnterCritical();			//�ر��ж�    
	OsCurrentTaskPointer->State &=~(0x100<<OSMutexNum);     //ȡ�������־
	MutexTaskNum = OS_IsUseMutex(OSMutexNum);
	if (!MutexTaskNum)				//û�������л����ź�  
	{
		ExitCritical();				//�����ж�
		return ;
	}
	//���������л����ź�
	MutexHighPriority = MutexTaskNum-1;			//��ѯ��ʱ��Ϊ���жϣ�Ԥ�ȼ�1��.
		taskPointer = &TaskBlock[MutexHighPriority];						//��ȡ������ȼ���TASK��ָ��
//		taskPointer->State =TaskStatusReady;     //��������־����Ϊ����״̬
		taskPointer->Delay = 0;				
//		taskPointer->EventPointer = (Event *)0;
		taskPointer->MessagePointer = (void*)0;
		PriorityReadyTable |=  1 << MutexHighPriority;  			
		ExitCritical();
		Schedule(); 																		//�����л�
		return ;				
}


void IdleTask(void)
{
	while(1)
	{    		 
	}
}

/*******************************************************************************
* ������	: Idle
* ����	    : ����Idle������Ϊ������ȼ�����
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void CreateIdleTask(void)
{	
	OS.CreateTask(IdleTask);
}


static void Start (void)
{
    //����ȷ�����������¼���������������ֹ����
//    Assert(TaskCounter <= TaskSum);
	OS.CreateTask(IdleTask);    //������������
    SysTick_Config(SystemCoreClock / 10000);	//����SYSTICK����	
    OS.Systick.Register(Systick1000, SystemTickRoutine);			//ע��OS����
    
    if (OsRunning == OS_false) 
    {
    //�ҳ�������ȼ�����
	    OsHighReadyPriority = UnmapTable[PriorityReadyTable];    
        OsCurrentPriority= OsHighReadyPriority;   
    //�ҳ������
        OsHighReadyTaskPointer= &TaskBlock[OsCurrentPriority];
        OsCurrentTaskPointer = OsHighReadyTaskPointer;
    //������ȼ���������
        OsStartHighReadyTask();                 
    }
}

/*******************************************************************************
* ������	: OS_NVIC_Config
* ����	    : �ж����ó�ʼ������������л��ж����ȼ�
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void OS_NVIC_Config(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_SetPriority(SysTick_IRQn, 0xFE);   //�����ȼ�
	NVIC_SetPriority(PendSV_IRQn, 0xFF);    //������ȼ�														//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}



/*******************************************************************************
* ������	: InitializeOs
* ����	    : ��ʼ��OS
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.2			ZZ					 2013/10/08				ȡ���ڳ�ʼ��������systick����Ϊ����OS��ʱ������systick
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/

void InitializeOs(void)
{
	OS_NVIC_Config();
	OS.Start = Start;
	OS.CreateIdleTask = CreateIdleTask;
	OS.CreateTask = CreateTask;
	OS.PendMessageQueue= PendMessageQueue;
	OS.PostMessageQueue= PostMessageQueue;
	OS.DelayTimeTick= DelayTimeTick;
	OS.OSTime = 0;			//ostime��ʼ������
	OS.OSMutexLock = OSMutexLock;		
	OS.OSMutexUnLock = OSMutexUnLock;
	#if TaskStackSumIsSame == 0	
		#if TaskSum > 0	
			TaskStack[0] = &TaskStack0[TaskStackSum0-1];	
		#endif
		#if TaskSum > 1	
			TaskStack[1] = &TaskStack1[TaskStackSum1-1];		
		#endif		
		#if TaskSum > 2	
			TaskStack[2] = &TaskStack2[TaskStackSum2-1];	
		#endif
		#if TaskSum > 3	
			TaskStack[3] = &TaskStack3[TaskStackSum3-1];		
		#endif	
		#if TaskSum > 4	
			TaskStack[4] = &TaskStack4[TaskStackSum4-1];			
		#endif
		#if TaskSum > 5	
			TaskStack[5] = &TaskStack5[TaskStackSum5-1];			
		#endif		
		#if TaskSum > 6	
			TaskStack[6] = &TaskStack6[TaskStackSum6-1];			
		#endif
		#if TaskSum > 7
			TaskStack[7] = &TaskStack7[TaskStackSum7-1];			
		#endif	
	#endif	
	

	MSOS_InitSystick();
	MSOS_InitTimer();		
	
	
}














