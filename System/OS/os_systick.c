/*
********************************************************************************
*文件名     : device_systick.c
*作用       : 系统节拍
*原理       : 
*说明			：初始版本采用王绍伟（网名:凤舞天；标识:wangsw）的msOS嵌入式微系统
********************************************************************************
*版本     作者            日期            说明
*V0.2			ZZ						2014/09/29			将KEY调用改为20ms1次，同时增加了20ms1次调用GUI中的touch
*V0.1    Wangsw        2013/12/16       初始版本
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
* 函数名	: RegisterSystick
* 描述	    : 系统节拍注册函数，用于注册回调函数到不同速度的系统节拍数组中。
* 输入参数  : systemTick 系统节拍速度类型 registerFunction 注册的函数指针
* 返回参数  : bool布尔变量，返回是否成功
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
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
* 函数名	: SysTick_Handler
* 描述	    : 系统节拍中断入口，非常重要，每秒10000次，即0.1mS一次
*           : 系统节拍通过switch细分为每秒1000次，每秒100次的例行节拍
*           : 同层直接调用，高层采用注册调用
* 输入参数  : 无
* 返回参数  : 无
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
        case 9:    //1ms计数1次，
						switch(div)
            {
                case 0: 			//10ms计数1次
										if(++Counter10mS == 2)			//20ms 1次运行
										{					//20ms计数1次
											Systick50RegisterPointerBlock[0]();											
											Counter10mS = 0;
										}
										else												//另一个20ms
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



