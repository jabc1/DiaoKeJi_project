/*
********************************************************************************
*文件名   : os.h
*作用     : RTOS头文件
*创建时间 : 2013.7.21
*说明			：初始版本采用王绍伟（网名:凤舞天；标识:wangsw）的msOS嵌入式微系统
********************************************************************************
*/

#ifndef __OS_H
#define __OS_H
#include "OS_Conf.h" 
#include "stm32f10x.h" 



typedef void (*OS_func)(void);
#define OS_Func(address)   (*(OS_func)address)()

	#define OS_true                	1
	#define OS_false               	0
	#define OS_ON 									1
	#define OS_OFF 									0	
	#define OS_null                ((void *) 0)
	#define OS_invalid             0xFF

	#define OS_SetBit(data, offset)         (data |= 1U << offset)
	#define OS_ResetBit(data, offset)       (data &= ~(1U << offset))
	#define OS_GetBit(data, offset)         ((data >> offset) &0x01)

	




extern uint16_t CriticalNesting;
#define	EnterCritical()		__disable_irq(); CriticalNesting++;
#define ExitCritical()		if(--CriticalNesting == 0){__enable_irq();}


typedef enum 
{
    TimerSystickHandle      = 0,		//在systick中运行
    TimerMessageHandle      = 1			//在任务中运行
}TimerhandleModeEnum;	
typedef enum
{
    Systick10000,
    Systick1000,
    Systick100,
    Systick50,	
}SystickEnum;


typedef struct                  //定义队列
{
    void    * * Start;          //指向队列开始
    void    * * End;            //指向队列结束
    void    * * In;             //插入一个消息
    void    * * Out;            //取出一个消息
    uint16_t      Size;           //队列长度
    uint16_t      Entries;        //消息长度
} Queue;

typedef struct 
{
    uint32_t      * StackTopPointer;    //当前TCB的栈顶指针
    void      		* MessagePointer;     //消息指针
		Queue					*	QueuePointer;				//消息列队指针
    uint32_t      Delay;              //任务等待时的最多节拍数
    uint16_t      State;              //任务状态字
    uint8_t       Priority;           //任务优先级(0为最高, 7为最低) 
} Task;  
/* 	 Task->State = 0x00;  0000 0000 0000 0000 
													|||| |||| |||| |||+--	TaskStatusReady   	 			处于就绪状态   
													|||| |||| |||| ||+---	TaskStatusMessageBox      处于邮箱状态 
													|||| |||| |||| |+----	TaskStatusMessageQueue    处于Q状态
													|||| |||| |||| +-----	保留					
													|||| |||| |+++-------	保留
													|||| |||| +----------	是否为焦点状态
													|||| |||+------------	TaskStatusSuspend1				处于挂起状态1
													|||| ||+-------------	TaskStatusSuspend2		 		处于挂起状态2
													|||| |+--------------	TaskStatusSuspend3				处于挂起状态3
													|||| +---------------	TaskStatusSuspend4			 	处于挂起状态4
													|||+-----------------	TaskStatusSuspend5		 		处于挂起状态5
													||+------------------	TaskStatusSuspend6			 	处于挂起状态6
													|+-------------------	TaskStatusSuspend7				处于挂起状态7
													+-------------------	TaskStatusSuspend8				处于挂起状态8*/

#define  TaskStatusSuspend			0xFF00        					/* 所有处于挂起状态*/
#define  TaskStatusSuspend1			0        					/* 处于挂起状态*/
#define  TaskStatusSuspend2			1        					/* 处于挂起状态*/
#define  TaskStatusSuspend3			2        					/* 处于挂起状态*/
#define  TaskStatusSuspend4			3        					/* 处于挂起状态*/
#define  TaskStatusSuspend5			4        					/* 处于挂起状态*/
#define  TaskStatusSuspend6			5        					/* 处于挂起状态*/
#define  TaskStatusSuspend7			6        					/* 处于挂起状态*/
#define  TaskStatusSuspend8			7        					/* 处于挂起状态*/
typedef struct
{

	uint32_t OSTime;
	void (* Start)(void);
	void (* CreateIdleTask)(void);	
	uint8_t (* CreateTask)(void (*taskPointer)(void));
	void * (* PendMessageQueue)(uint32_t timeout);
	uint8_t (* PostMessageQueue)(uint8_t Priority, void *messagePointer);
	void (* DelayTimeTick)(uint32_t timeTick); 
	void (*OSMutexUnLock) (uint8_t OSMutexNum);
	uint8_t (*OSMutexLock)(uint8_t OSMutexNum);
	struct Timer
	{
		uint8_t (*Start)(TimerhandleModeEnum mode, uint32_t delay, OS_func registerFunction);
		void (*StopAt)(uint8_t id); 
		void (*StartAt)(uint8_t id, uint32_t delay, OS_func registerFunction);		
	}Timer;
	struct Systick
	{
		uint8_t (*Register)(SystickEnum systemTick, OS_func registerFunction);
	}Systick;		
}OSStruct;


extern  uint8_t    OsCurrentPriority;                		/*定义正在运行的任务的优先级*/
extern OSStruct OS;
extern void InitializeOs(void); 	
void MSOS_InitSystick(void);
void MSOS_InitTimer(void);
#endif






















