/*
********************************************************************************
*文件名     : device_timer.c
*作用       : 软件定时器设备
*原理       : 基于系统节拍1000/S，即1mS一次虚拟出软件定时器
*说明			：初始版本采用王绍伟（网名:凤舞天；标识:wangsw）的msOS嵌入式微系统
********************************************************************************
*版本     作者            日期            说明
*V0.2			ZZ					 2015/10/07				将POST函数改为自动获取当前任务的eventPointer指针，变更了Timer的定义
*V0.1    Wangsw        2013/07/21       初始版本
********************************************************************************
*/




#include "os.h"
 
typedef struct 
{
    uint16_t  Delay;
		uint8_t Priority;
    OS_func RegisterFunction;
}Timer;

#define Timer1msSum		0x8			//最大32个
#define Timer100usSum		0x4			//最大8个
static uint32_t State_1ms = 0;
static uint32_t Mode_1ms = 0;
static Timer TimerBlock_1ms[Timer1msSum];

static uint8_t State_100us = 0;
static Timer TimerBlock_100us[Timer100usSum];


/*******************************************************************************
* 函数名	: TimerSystick1000Routine
* 描述	    : 系统节拍1000/S,即1mS一次调用，实现虚拟定时器。
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/10       初始版本
*******************************************************************************/
void TimerSystick1000Routine(void)
{
    uint8_t i = 0;
    uint32_t stateCopy;
//     Task *taskPointer;    
    if(State_1ms == 0x00) 
    {
        return;
    }
	
    EnterCritical();
	
    stateCopy = State_1ms;
    while(stateCopy)	
    {
        if((stateCopy & 0x01) == 1) 
        {
            if((--TimerBlock_1ms[i].Delay) == 0) 
            {
                if(OS_GetBit(Mode_1ms, i)) 
                {
									OS.PostMessageQueue(TimerBlock_1ms[i].Priority,TimerBlock_1ms[i].RegisterFunction);
                } 
                else 
                {
                    (*(TimerBlock_1ms[i].RegisterFunction))(); 
                }
                OS_ResetBit(State_1ms, i);
            }
        }
		
        stateCopy = stateCopy >> 1;
        i++;
    }
    ExitCritical();
}

//只能在任务中运行程序
void TimerSystick10000Routine(void)
{
    uint8_t i = 0;
    uint32_t stateCopy;
//     Task *taskPointer;    
    if(State_100us == 0x00) 
    {
        return;
    }
	
    EnterCritical();
	
    stateCopy = State_100us;
    while(stateCopy)	
    {
        if((stateCopy & 0x01) == 1) 
        {
            if((--TimerBlock_100us[i].Delay) == 0) 
            {
							(*(TimerBlock_100us[i].RegisterFunction))(); 
              OS_ResetBit(State_100us, i);
            }
        }
		
        stateCopy = stateCopy >> 1;
        i++;
    }
    ExitCritical();
}
/*******************************************************************************
* 函数名	: Start
* 描述	    : 软件定时器
* 输入参数  : handleMode: 两种处理方式，一种直接在节拍中断中处理，适合费用低的,
*                         另一种在消息中处理，适合处理费用高的。
*           : delay:延时节拍数，以系统节拍为单位
*           : registerFunction: 注册回调执行函数，延时超时后，执行此函数。
*             data:消息值，MessageTimer类型为32bit地址,其他类型下都是24bit数据
* 返回参数  : uint8_t类型，返回ID号，从0开始，若失败则返回invalid(-1)
*******************************************************************************/
static uint8_t Start(TimerhandleModeEnum handleMode, uint32_t delay, OS_func registerFunction)
{
    uint8_t i;

    EnterCritical();
	
    for(i = 0; i < Timer1msSum; i++) 
    {
        if(!OS_GetBit(State_1ms, i)) 
        {			
            TimerBlock_1ms[i].Delay = delay;/*延时时间*/
            TimerBlock_1ms[i].RegisterFunction = registerFunction;/*回调函数*/
            if(handleMode) 
            {
                OS_SetBit(Mode_1ms, i);
								TimerBlock_1ms[i].Priority = OsCurrentPriority;
            } 
            else 
            {
                OS_ResetBit(Mode_1ms, i);
            }
			
            OS_SetBit(State_1ms, i);
            ExitCritical();
            return(i);
        }
    }
    ExitCritical();
    return(OS_invalid);
}
/*******************************************************************************
* 函数名	: Start
* 描述	    : 软件定时器
* 输入参数  : handleMode: 两种处理方式，一种直接在节拍中断中处理，适合费用低的,
*                         另一种在消息中处理，适合处理费用高的。
*           : delay:延时节拍数，以系统节拍为单位
*           : registerFunction: 注册回调执行函数，延时超时后，执行此函数。
*             data:消息值，MessageTimer类型为32bit地址,其他类型下都是24bit数据
* 返回参数  : uint8_t类型，返回ID号，从0开始，若失败则返回invalid(-1)
*******************************************************************************/
void StartAt(uint8_t id, uint32_t delay, OS_func registerFunction)
{
	EnterCritical();	
	TimerBlock_100us[id].Delay = delay;/*延时时间*/
	TimerBlock_100us[id].RegisterFunction = registerFunction;/*回调函数*/
	TimerBlock_100us[id].Priority = OsCurrentPriority;
	OS_SetBit(State_100us, id);
	ExitCritical();
}

/*******************************************************************************
* 函数名	: Stop
* 描述	    : 停止某一路的软件定时器
* 输入参数  : id为0、1、2...
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/10       初始版本
*******************************************************************************/
static void StopAt(uint8_t id) 
{
//    Assert(id < Timer1msSum);
    
    EnterCritical();
	
    OS_ResetBit(State_100us, id);
	
    ExitCritical();
}


void MSOS_InitTimer(void)
{
   OS.Timer.Start = Start;
   OS.Timer.StopAt = StopAt; 
   OS.Timer.StartAt = StartAt;	
}
