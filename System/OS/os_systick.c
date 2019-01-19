/*
********************************************************************************
*�ļ���     : device_systick.c
*����       : ϵͳ����
*ԭ��       : 
*˵��			����ʼ�汾��������ΰ������:�����죻��ʶ:wangsw����msOSǶ��ʽ΢ϵͳ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.2			ZZ						2014/09/29			��KEY���ø�Ϊ20ms1�Σ�ͬʱ������20ms1�ε���GUI�е�touch
*V0.1    Wangsw        2013/12/16       ��ʼ�汾
********************************************************************************
*/

#include "os.h"




//extern void KeySystick50Routine(void);

extern void TimerSystick1000Routine(void);
extern void TimerSystick10000Routine(void);
extern void UsartSystick1000Routine(void);

extern void AdcSystick10000Routine(void);

extern void SysTick_Handler(void);



#define Systick10000Sum 2
#define Systick1000Sum  4
#define Systick100Sum   10
#define Systick50Sum   2
static void Dummy(void) {};
static OS_func Systick50RegisterPointerBlock[Systick50Sum] =
{
    Dummy, Dummy
};
static OS_func Systick10000RegisterPointerBlock[Systick10000Sum] =
{
    Dummy, Dummy
};

static OS_func Systick1000RegisterPointerBlock[Systick1000Sum] =
{
    Dummy, Dummy, Dummy, Dummy
};

static OS_func Systick100RegisterPointerBlock[Systick100Sum] =
{
    Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy
};

/*******************************************************************************
* ������	: RegisterSystick
* ����	    : ϵͳ����ע�ắ��������ע��ص���������ͬ�ٶȵ�ϵͳ���������С�
* �������  : systemTick ϵͳ�����ٶ����� registerFunction ע��ĺ���ָ��
* ���ز���  : bool���������������Ƿ�ɹ�
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static uint8_t RegisterSystick(SystickEnum systick, OS_func registerFunction)
{
	static uint8_t Systick100Counter = 0;
	static uint8_t Systick1000Counter = 0;
	static uint8_t Systick10000Counter = 0;
	static uint8_t Systick50Counter = 0;
//	Assert(registerFunction != (function)0);
	EnterCritical();
	switch(systick)
	{
		case Systick50:
					if (Systick50Counter == Systick50Sum) return(OS_false);
					
					Systick50RegisterPointerBlock[Systick50Counter++] = registerFunction;  
					ExitCritical();
					return(OS_true);
		case Systick100:
					if (Systick100Counter == Systick100Sum)
							return(OS_false);
					
					Systick100RegisterPointerBlock[Systick100Counter++] = registerFunction;  
					ExitCritical();
					return(OS_true);
			case Systick1000:
					if (Systick1000Counter == Systick1000Sum)
							return(OS_false);
					
					Systick1000RegisterPointerBlock[Systick1000Counter++] = registerFunction;   
					ExitCritical();
					return(OS_true);
			case Systick10000:
					if (Systick10000Counter == Systick10000Sum)
							return(OS_false);
					
					Systick10000RegisterPointerBlock[Systick10000Counter++] = registerFunction;   
					ExitCritical();
					return(OS_true);
			default:
					ExitCritical();
					return(OS_false);
	}
}

void MSOS_InitSystick(void)
{
    OS.Systick.Register = RegisterSystick;
//    SysTick_Config(SystemCoreClock / 10000);
}



/*******************************************************************************
* ������	: SysTick_Handler
* ����	    : ϵͳ�����ж���ڣ��ǳ���Ҫ��ÿ��10000�Σ���0.1mSһ��
*           : ϵͳ����ͨ��switchϸ��Ϊÿ��1000�Σ�ÿ��100�ε����н���
*           : ͬ��ֱ�ӵ��ã��߲����ע�����
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SysTick_Handler(void)
{
    uint8_t div;
    static uint8_t Counter = 0,Counter10mS=0;
  
    if(++Counter == 100) Counter = 0;

    Systick10000RegisterPointerBlock[0]();
    Systick10000RegisterPointerBlock[1]();
    TimerSystick10000Routine();
//    AdcSystick10000Routine();

    div = Counter / 10;

    switch(Counter % 10)
    {
        case 0:
            Systick1000RegisterPointerBlock[0]();
            break;
        case 1:
            Systick1000RegisterPointerBlock[1]();
            break;
        case 2:
            Systick1000RegisterPointerBlock[2]();
            break;
        case 3:
            Systick1000RegisterPointerBlock[3]();
            break;
        case 4:
//            UsartSystick1000Routine();
            break;
        case 5:
            TimerSystick1000Routine();
            break;
        case 6:
            break;
        case 7:
					 OS.OSTime++;
            break;
        case 8:
            Systick100RegisterPointerBlock[div]();
				    break;
        case 9:    //1ms����1�Σ�
						switch(div)
            {
                case 0: 			//10ms����1��
										if(++Counter10mS == 2)			//20ms 1������
										{					//20ms����1��
											Systick50RegisterPointerBlock[0]();											
											Counter10mS = 0;
										}
										else												//��һ��20ms
										{
											Systick50RegisterPointerBlock[1]();	
										}											
										break;
                case 1:
									
										break;									
							
							
                case 2:

                    break;
                case 3:

                    break;
                default:
                    break;
            }
            break;
    }    
}



