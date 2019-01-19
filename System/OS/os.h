/*
********************************************************************************
*�ļ���   : os.h
*����     : RTOSͷ�ļ�
*����ʱ�� : 2013.7.21
*˵��			����ʼ�汾��������ΰ������:�����죻��ʶ:wangsw����msOSǶ��ʽ΢ϵͳ
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
    TimerSystickHandle      = 0,		//��systick������
    TimerMessageHandle      = 1			//������������
}TimerhandleModeEnum;	
typedef enum
{
    Systick10000,
    Systick1000,
    Systick100,
    Systick50,	
}SystickEnum;


typedef struct                  //�������
{
    void    * * Start;          //ָ����п�ʼ
    void    * * End;            //ָ����н���
    void    * * In;             //����һ����Ϣ
    void    * * Out;            //ȡ��һ����Ϣ
    uint16_t      Size;           //���г���
    uint16_t      Entries;        //��Ϣ����
} Queue;

typedef struct 
{
    uint32_t      * StackTopPointer;    //��ǰTCB��ջ��ָ��
    void      		* MessagePointer;     //��Ϣָ��
		Queue					*	QueuePointer;				//��Ϣ�ж�ָ��
    uint32_t      Delay;              //����ȴ�ʱ����������
    uint16_t      State;              //����״̬��
    uint8_t       Priority;           //�������ȼ�(0Ϊ���, 7Ϊ���) 
} Task;  
/* 	 Task->State = 0x00;  0000 0000 0000 0000 
													|||| |||| |||| |||+--	TaskStatusReady   	 			���ھ���״̬   
													|||| |||| |||| ||+---	TaskStatusMessageBox      ��������״̬ 
													|||| |||| |||| |+----	TaskStatusMessageQueue    ����Q״̬
													|||| |||| |||| +-----	����					
													|||| |||| |+++-------	����
													|||| |||| +----------	�Ƿ�Ϊ����״̬
													|||| |||+------------	TaskStatusSuspend1				���ڹ���״̬1
													|||| ||+-------------	TaskStatusSuspend2		 		���ڹ���״̬2
													|||| |+--------------	TaskStatusSuspend3				���ڹ���״̬3
													|||| +---------------	TaskStatusSuspend4			 	���ڹ���״̬4
													|||+-----------------	TaskStatusSuspend5		 		���ڹ���״̬5
													||+------------------	TaskStatusSuspend6			 	���ڹ���״̬6
													|+-------------------	TaskStatusSuspend7				���ڹ���״̬7
													+-------------------	TaskStatusSuspend8				���ڹ���״̬8*/

#define  TaskStatusSuspend			0xFF00        					/* ���д��ڹ���״̬*/
#define  TaskStatusSuspend1			0        					/* ���ڹ���״̬*/
#define  TaskStatusSuspend2			1        					/* ���ڹ���״̬*/
#define  TaskStatusSuspend3			2        					/* ���ڹ���״̬*/
#define  TaskStatusSuspend4			3        					/* ���ڹ���״̬*/
#define  TaskStatusSuspend5			4        					/* ���ڹ���״̬*/
#define  TaskStatusSuspend6			5        					/* ���ڹ���״̬*/
#define  TaskStatusSuspend7			6        					/* ���ڹ���״̬*/
#define  TaskStatusSuspend8			7        					/* ���ڹ���״̬*/
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


extern  uint8_t    OsCurrentPriority;                		/*�����������е���������ȼ�*/
extern OSStruct OS;
extern void InitializeOs(void); 	
void MSOS_InitSystick(void);
void MSOS_InitTimer(void);
#endif






















