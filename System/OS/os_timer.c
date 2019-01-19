/*
********************************************************************************
*�ļ���     : device_timer.c
*����       : �����ʱ���豸
*ԭ��       : ����ϵͳ����1000/S����1mSһ������������ʱ��
*˵��			����ʼ�汾��������ΰ������:�����죻��ʶ:wangsw����msOSǶ��ʽ΢ϵͳ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.2			ZZ					 2015/10/07				��POST������Ϊ�Զ���ȡ��ǰ�����eventPointerָ�룬�����Timer�Ķ���
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/




#include "os.h"
 
typedef struct 
{
    uint16_t  Delay;
		uint8_t Priority;
    OS_func RegisterFunction;
}Timer;

#define Timer1msSum		0x8			//���32��
#define Timer100usSum		0x4			//���8��
static uint32_t State_1ms = 0;
static uint32_t Mode_1ms = 0;
static Timer TimerBlock_1ms[Timer1msSum];

static uint8_t State_100us = 0;
static Timer TimerBlock_100us[Timer100usSum];


/*******************************************************************************
* ������	: TimerSystick1000Routine
* ����	    : ϵͳ����1000/S,��1mSһ�ε��ã�ʵ�����ⶨʱ����
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/10       ��ʼ�汾
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

//ֻ�������������г���
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
* ������	: Start
* ����	    : �����ʱ��
* �������  : handleMode: ���ִ���ʽ��һ��ֱ���ڽ����ж��д����ʺϷ��õ͵�,
*                         ��һ������Ϣ�д����ʺϴ�����øߵġ�
*           : delay:��ʱ����������ϵͳ����Ϊ��λ
*           : registerFunction: ע��ص�ִ�к�������ʱ��ʱ��ִ�д˺�����
*             data:��Ϣֵ��MessageTimer����Ϊ32bit��ַ,���������¶���24bit����
* ���ز���  : uint8_t���ͣ�����ID�ţ���0��ʼ����ʧ���򷵻�invalid(-1)
*******************************************************************************/
static uint8_t Start(TimerhandleModeEnum handleMode, uint32_t delay, OS_func registerFunction)
{
    uint8_t i;

    EnterCritical();
	
    for(i = 0; i < Timer1msSum; i++) 
    {
        if(!OS_GetBit(State_1ms, i)) 
        {			
            TimerBlock_1ms[i].Delay = delay;/*��ʱʱ��*/
            TimerBlock_1ms[i].RegisterFunction = registerFunction;/*�ص�����*/
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
* ������	: Start
* ����	    : �����ʱ��
* �������  : handleMode: ���ִ���ʽ��һ��ֱ���ڽ����ж��д����ʺϷ��õ͵�,
*                         ��һ������Ϣ�д����ʺϴ�����øߵġ�
*           : delay:��ʱ����������ϵͳ����Ϊ��λ
*           : registerFunction: ע��ص�ִ�к�������ʱ��ʱ��ִ�д˺�����
*             data:��Ϣֵ��MessageTimer����Ϊ32bit��ַ,���������¶���24bit����
* ���ز���  : uint8_t���ͣ�����ID�ţ���0��ʼ����ʧ���򷵻�invalid(-1)
*******************************************************************************/
void StartAt(uint8_t id, uint32_t delay, OS_func registerFunction)
{
	EnterCritical();	
	TimerBlock_100us[id].Delay = delay;/*��ʱʱ��*/
	TimerBlock_100us[id].RegisterFunction = registerFunction;/*�ص�����*/
	TimerBlock_100us[id].Priority = OsCurrentPriority;
	OS_SetBit(State_100us, id);
	ExitCritical();
}

/*******************************************************************************
* ������	: Stop
* ����	    : ֹͣĳһ·�������ʱ��
* �������  : idΪ0��1��2...
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/10       ��ʼ�汾
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
