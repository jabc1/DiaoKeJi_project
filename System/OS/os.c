/*
*文件名   : os.c
*作用     : uC/OS-II精简而来
*创建时间 	: 2013.7.21
*说明			：初始版本采用王绍伟（网名:凤舞天；标识:wangsw）的msOS嵌入式微系统
*版本			: V2.1
*2016.12.29 修改了任务堆栈数量是否相同，可以使用宏定义来区分
*2015.10.24 1.增加了	uint8_t OS_IsUseMutex(uint8_t Num)
										uint8_t OSMutexLock (uint8_t OSMutexNum)
										void  OSMutexUnLock (uint8_t OSMutexNum)
						  3个函数互斥量函数，互斥量最大8个，由state中的高8位来表示
						2.取消了Event结构体，任务的Event指针由任务的优先级来代替，重写了create，pend，post函数
*2015.10.08 增加了 PopQueue和PushQueue 2个函数
*						修改了不是最高优先级，无法同时post2个信息的BUG,
*						修改了PendMessageQueue和PostMessageQueue函数
********************************************************************************
*/

#include "os.h"
#include "core_cm3.h"
//***************************************************************************************
//任务状态字 TASK STATUS (state的位定义)       
#define  TaskStatusReady            0x00        					/* 处于就绪状态   */                         
#define  TaskStatusMessageBox       0x02        					/* 处于邮箱状态  */
#define  TaskStatusMessageQueue     0x04        					/* 处于Q状态*/



//****************************************************************************************
//事件类型(OS_EVENT types)
#define  EventTypeUnused						0      						/* 无事件类型*/
#define  EventTypeMessageBox	   		1							    /* 邮箱为数字序列*/
#define  EventTypeMessageQueue      2							    /* 消息队列为数字序列*/

#if TaskSum == 2        //双任务查找表，适合双任务下的msOS
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
#if TaskSum == 4        //4任务查找表，
	static const uint8_t UnmapTable[] = 
	{
			0, 0, 1, 0, 2, 0, 1, 0,	
			3, 0, 1, 0, 2, 0, 1, 0	
	};
	#endif
#if TaskSum > 4                //多任务查找表，兼容uC/OS-II风格
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
	
	
	
OSStruct OS;									//OS的系统结构体
#if (TaskStackSumIsSame == 0)
	static uint32_t  	*TaskStack[TaskSum];    							//任务栈数组	
	#if TaskSum > 0                //多任务查找表，兼容uC/OS-II风格
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
	static uint32_t  	TaskStack[TaskSum][TaskStackSum];    							//任务栈数组	
#endif


static void *  TaskMessageQueue[TaskSum-1][QueueStackSum];           //消息队列	
		
uint16_t CriticalNesting = 0;
static uint8_t InterruptNesting = 0;				        /*定义中断嵌套层数*/
static uint8_t PriorityReadyTable = 0;		                /*就绪表*/
static uint8_t TaskCounter = 0;					        /*任务计数器*/
static Task  TaskBlock[TaskSum];				        /*任务控制块*/
static Queue QueueBlock[TaskSum-1];                       /*消息队列控制块*/

uint8_t    OsRunning = OS_false;                       		/*多任务已经开始=1,任务处于不运行状态=0*/
uint8_t    OsCurrentPriority = 0;                		/*定义正在运行的任务的优先级*/
uint8_t    OsHighReadyPriority = 0;			            /*定义具有最高优先级别的就绪任务的优先级*/
Task *	OsCurrentTaskPointer = (Task *)0;		        /*定义指向正在运行任务控制块的指针*/
Task *	OsHighReadyTaskPointer = (Task *)0;	            /*定义指向最高级优先级就绪任务控制块的指针*/


static uint32_t * InitializeTaskStack (void  (*taskPointer)(void), uint32_t  *stackTopPointer)
{
    *(stackTopPointer) = (uint32_t)0x01000000L;           /*  xPSR                        */ 
    *(--stackTopPointer) = (uint32_t)taskPointer;           /*  Entry Point of the task     */
                                                    /*  任务入口地址                */
    *(--stackTopPointer) = (uint32_t)0xFFFFFFFEL;         /*  R14 (LR)  (init value will  */
                                                    /*  cause fault if ever used)   */                                                                           
    *(--stackTopPointer) = (uint32_t)0x12121212L;         /*  R12                         */
    *(--stackTopPointer) = (uint32_t)0x03030303L;         /*  R3                          */
    *(--stackTopPointer) = (uint32_t)0x02020202L;         /*  R2                          */
    *(--stackTopPointer) = (uint32_t)0x01010101L;         /*  R1                          */
    *(--stackTopPointer) = (uint32_t)0x00000000L;         /*  R0 : argument  输入参数     */
                                                    /*  Remaining registers saved on*/
                                                    /*  process stack 剩下的寄存器保存到堆栈*/                                                 
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
        OsHighReadyPriority = UnmapTable[PriorityReadyTable];//获取最高优先级
        if (OsHighReadyPriority != OsCurrentPriority)//检测当前是否最高运行级
        {                                            //赋值最高优先级任务指针
            OsHighReadyTaskPointer = &TaskBlock[OsHighReadyPriority];
            OsSwitchContext();                                     //任务切换
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
* 函数名	: CreateMessageQueue
* 描述	    : 创建一个消息队列，消息队列可以存储多个信息，占用一个事件块及
*             一个队列,EventSum等于MessageBox与MessageQueue两者的总数,QueueSum
*             等于MessageQueue总数。
* 输入参数  : start:队列指针头，size，队列长度
* 返回参数  : 事件块指针
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
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
* 函数名	: CreateTask
* 描述	    : 创建任务，根据创建的顺序决定优先级，越早创建的，优先级越高，
*           : 第一个为0优先级,注意MenuTask必须为最低优先级，也就是最后一个创建。
*           : 创建了多少个任务，TaskSum要设置为对应的值。
* 输入参数  : taskPointer: 任务函数指针*           
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V2.0			ZZ					 2014/10/24				为了设置方便，取消了单独的任务栈数组	设置，每个人物的任务栈相同大小
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
static uint8_t CreateTask (void (*taskPointer)(void))
{
    uint32_t * sp;
    EnterCritical();
		if(TaskCounter >= TaskSum) return 0xff;															//创建的数量大于最大数量

		#if TaskStackSumIsSame == 0	
			sp = InitializeTaskStack(taskPointer, (TaskStack[TaskCounter]));	
		#endif	
		#if TaskStackSumIsSame == 1	
			sp = InitializeTaskStack(taskPointer, &(TaskStack[TaskCounter][TaskStackSum-1]));
		#endif
	

	
    InitializeTask(sp);																							//创建任务
	  CreateMessageQueue(TaskMessageQueue[TaskCounter], QueueStackSum);	//创建消息列表
		TaskCounter++;	
    ExitCritical();
	return TaskCounter-1;
}
/*******************************************************************************
* 函数名	: PushQueue
* 描述	    : 消息列表压入消息队列中
* 输入参数  : queuePointer: 消息队列指针；messagePointer：消息        
* 返回参数  : 是否成功
********************************************************************************
*版本     作者            日期            说明
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
* 函数名	: PopQueue
* 描述	    : 获取消息队列中的消息
* 输入参数  : queuePointer: 消息队列指针；        
* 返回参数  : 消息
********************************************************************************
*版本     作者            日期            说明
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
* 函数名	: PendMessageQueue
* 描述	    : 等待消息队列，当消息队列为空时，所在任务挂起
* 输入参数  : eventPointer 队列事件块指针，timeout 等待时间，1mS为单位
* 返回参数  : 
********************************************************************************
*版本     作者            日期            说明
*V0.2			ZZ					 2013/10/08				修改了不能同时post2个不是最高优先级的BUG
*V0.1    Wangsw        2013/09/11       初始版本
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
    Schedule();              /*任务切换节点，如果当前没有message的话就切换任务，当有message的时候就从这里切换回来       */
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
* 函数名	: PostMessageQueue
* 描述	    : 发送一个消息到消息队列中，处于等待的任务会自动运行
* 输入参数  : eventPointer 队列事件块指针，messagePointer发送消息指针
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.2			ZZ					 2013/10/08				修改了不能同时post2个不是最高优先级的BUG
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
static uint8_t PostMessageQueue (uint8_t Priority, void *messagePointer)
{
    Queue * queuePointer;
		Task *taskPointer;	

//	  Assert(TaskCounter >= Priority);	
    EnterCritical();
	  taskPointer = &TaskBlock[Priority];
    queuePointer = taskPointer->QueuePointer;                 /* Point to queue control block                  */	
    if (queuePointer->Entries == 0x00) 				            /* 当前的队列里消息长度为0            */
    {                 
				PushQueue(queuePointer,messagePointer);								//将消息压入队列
        taskPointer->Delay = 0;
        taskPointer->MessagePointer = messagePointer;
        taskPointer->State &= ~TaskStatusMessageQueue;			
        if (taskPointer->State == TaskStatusReady)		//如果是ready状态
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
* 函数名	: DelayTimeTick
* 描述	    : 任务挂起等待时间，单位为1mS
* 输入参数  : timeTick，挂起时间，1mS为单位，比如1000,就是1秒钟
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
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
* 函数名	: OS_IsUseMutex
* 描述	    : 任务是否有互斥量
* 输入参数  : Num：互斥量编号        
* 返回参数  : 互斥量编号
********************************************************************************
*版本     作者            日期            说明
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
* 函数名	: OSMutexLock
* 描述	    : 互斥量闭锁
* 输入参数  : OSMutexNum：互斥量编号        
* 返回参数  : 0：成功；非0：不成功
********************************************************************************
*版本     作者            日期            说明
*V2.0			ZZ					 2014/10/24				
*******************************************************************************/
uint8_t OSMutexLock (uint8_t OSMutexNum)
{   //不得在中断中调用该函数   
//	Assert(OSMutexNum <8);
	if(OSMutexNum > 7) return 1;			//mutex编号大于7的话出错	0-7 对应8个通道
	EnterCritical();			//关闭中断     
	
	if (!OS_IsUseMutex(OSMutexNum))				//互斥标志为0，信号量可用  
	{
		OsCurrentTaskPointer->State |=(0x100<<OSMutexNum);     //设置互斥标志
		ExitCritical();				//开启中断
		return 0;
	}
	//互斥标志信号已经有任务置位，说明被占用
		OsCurrentTaskPointer->State |=(0x100<<OSMutexNum);     //设置互斥标志
	PriorityReadyTable &= ~(1 <<  OsCurrentPriority);						//清除优先级就绪
	ExitCritical();						//开启中断	
	Schedule();              /*任务切换节点，如果当前没有message的话就切换任务，当有message的时候就从这里切换回来       */
	return 0;
}

/*******************************************************************************
* 函数名	: OSMutexUnLock
* 描述	    : 互斥量解锁
* 输入参数  : OSMutexNum：互斥量编号        
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V2.0			ZZ					 2014/10/24				
*******************************************************************************/
void  OSMutexUnLock (uint8_t OSMutexNum)
{   //不得在中断中调用该函数
	uint8_t MutexTaskNum;
	uint8_t MutexHighPriority;  
	Task *taskPointer;
//	Assert(OSMutexNum < 8);	
	EnterCritical();			//关闭中断    
	OsCurrentTaskPointer->State &=~(0x100<<OSMutexNum);     //取消互斥标志
	MutexTaskNum = OS_IsUseMutex(OSMutexNum);
	if (!MutexTaskNum)				//没有任务有互斥信号  
	{
		ExitCritical();				//开启中断
		return ;
	}
	//还有任务有互斥信号
	MutexHighPriority = MutexTaskNum-1;			//查询的时候为了判断，预先加1了.
		taskPointer = &TaskBlock[MutexHighPriority];						//获取最高优先级的TASK的指针
//		taskPointer->State =TaskStatusReady;     //清除互斥标志，设为就绪状态
		taskPointer->Delay = 0;				
//		taskPointer->EventPointer = (Event *)0;
		taskPointer->MessagePointer = (void*)0;
		PriorityReadyTable |=  1 << MutexHighPriority;  			
		ExitCritical();
		Schedule(); 																		//任务切换
		return ;				
}


void IdleTask(void)
{
	while(1)
	{    		 
	}
}

/*******************************************************************************
* 函数名	: Idle
* 描述	    : 创建Idle，必须为最低优先级任务
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void CreateIdleTask(void)
{	
	OS.CreateTask(IdleTask);
}


static void Start (void)
{
    //断言确认任务数、事件数及队列数，防止错误
//    Assert(TaskCounter <= TaskSum);
	OS.CreateTask(IdleTask);    //创建空闲任务
    SysTick_Config(SystemCoreClock / 10000);	//启动SYSTICK节拍	
    OS.Systick.Register(Systick1000, SystemTickRoutine);			//注册OS节拍
    
    if (OsRunning == OS_false) 
    {
    //找出最高优先级任务
	    OsHighReadyPriority = UnmapTable[PriorityReadyTable];    
        OsCurrentPriority= OsHighReadyPriority;   
    //找出任务块
        OsHighReadyTaskPointer= &TaskBlock[OsCurrentPriority];
        OsCurrentTaskPointer = OsHighReadyTaskPointer;
    //最高优先级任务启动
        OsStartHighReadyTask();                 
    }
}

/*******************************************************************************
* 函数名	: OS_NVIC_Config
* 描述	    : 中断配置初始化，定义节拍切换中断优先级
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void OS_NVIC_Config(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_SetPriority(SysTick_IRQn, 0xFE);   //次优先级
	NVIC_SetPriority(PendSV_IRQn, 0xFF);    //最低优先级														//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}



/*******************************************************************************
* 函数名	: InitializeOs
* 描述	    : 初始化OS
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.2			ZZ					 2013/10/08				取消在初始化中启动systick，改为启动OS的时候启动systick
*V0.1    Wangsw        2013/09/11       初始版本
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
	OS.OSTime = 0;			//ostime初始化清零
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














