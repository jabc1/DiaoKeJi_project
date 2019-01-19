/*********************************************************************************
*文件名     : app.c
*作用       : MSOS操作系统应用程序入口
*说明				：
*原理       : 应用层入口，也是系统入口，主要初始化各类驱动、数据

任务的参数数量在OS.H 中定义
#define TaskSum     			4           任务总数。现在定义了4个，如果超过4个就必须修改
#define TaskStackSum   		100         任务栈深度，如果函数调用多的话这个值需加大
#define QueueStackSum			20					消息队列深度，总共可以有20个等待处理的消息，超出20个的话会丢失消息

建立任务前还需定义任务句柄

优先级按照建立的顺序，最早建立的是优先级最高，空闲任务 必须最后建立，
如果不用GUI任务，则最低优先级的任务不能有OS.PendMessageQueue函数和OS.DelayTimeTick函数;，必须为死循环。

********************************************************************************
*版本     作者            日期            说明
*V2.0			ZZ					 2015/10/26				增加了自己的驱动程序和GUI程序，并增加了互斥量
*V0.1    Wangsw        2013/07/21       初始版本
***********************************************************************************/

#include "OS.h"
#include "app.h"
#include "dk_conf.h"
#include "device_conf.h"
#include "malloc.h"
#include "SD.h"
/*定义各个逻辑任务的参数  */

u8 NRFTaskPriority ;		/*NRF解析任务*/
//u8 MotoTaskPriority ;		/*NRF解析任务*/
u8 ShowTaskPriority ;		/*NRF解析任务*/
u8 FileTaskPriority;




DiaoKe_Struct DK;  //雕刻机的数据结构体
/*******************************************************************************
* 函数名	: SystemTickRoutine
* 描述	    : 应用层系统节拍，用于执行固定间隔功能，间隔时间取决于注册函数
* 输入参数  	: 无
* 返回参数  	: 无
* 说明  		: 循环执行 限位状态-->Alarm中
********************************************************************************/

void SystickRoutine(void) 
{
    //回调函数，10ms一次被系统时钟调用
	

}
/*******************************************************************************
* 函数名	: InitializeData
* 描述	    : 初始化默认参数
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
u8 read_s32(u8 *line, uint8_t *char_counter, s32 *Val)     ;
void InitializeData(void)
{
	DK.State_B_P =  (DK_B_StateStruct *)BitAddr(&(DK.State));
	DK.File_B_P = (DK_B_File_Struct *)BitAddr(&(DK.File));	//状态的位（位带操作） 
	DK.Alarm_B_P = (DK_B_Alarm_Struct* )BitAddr(&(DK.Alarm));		//报警的位（位带操作）

}
/*******************************************************************************
* 函数名	: InitializeDevice
* 描述	    : 初始化设备
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void InitializeDevice(void)
{
	JTAG_Set(SWD_ENABLE);		//关闭JTAG,开启SWD
	InitializeRtc();
	InitializeTouch();
	InitializeLcd();
	InitializeUsart();
	InitializeLED();	
	InitializeKey();	    
}

/*******************************************************************************
* 函数名	: InitializeApp
* 描述	    : 初始化App
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
static void InitializeApp(void)
{

	InitializeData();
	mem_init();		//初始化内存池
	InitializeDevice();	
	OS.Systick.Register(Systick100, SystickRoutine);

}
 
/*******************************************************************************
* 函数名	: NVIC_Config
* 描述	    : 中断配置初始化，定义节拍切换中断优先级
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void NVIC_Config(void)
{	
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif  	
}

/*******************************************************************************
* 函数名	: main
* 描述	    : msOS整个程序入口
*           : InitializeData:初始化存储数据区
*           : System.Initialize:System初始化，实现底层系统初始化 
*           : 任务初始化优先的，优先级也优先，TaskSum要配套设置
*           : LogicTask: 业务逻辑任务，初始化为最高优先级 
*           : MenuTask: 界面任务，初始化为最低优先级,注意，它必须为最低优先级
* 输入参数  : 无
* 返回参数  : int
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/03       初始版本
*******************************************************************************/
int main(void) 
{

	NVIC_Config();
                                             //初始化系统层
	InitializeOs();        
	InitializeApp();                                                    //初始化应用层
	InitializeGUI(); 
	InitSD();			//需调整
	NRFTaskPriority = OS.CreateTask(NRFTask); //创建业务逻辑任务，优先级2 	
	FileTaskPriority = OS.CreateTask(FileTask);
//	MotoTaskPriority = OS.CreateTask(MotoTask); //创建业务逻辑任务，优先级2 	
	ShowTaskPriority = OS.CreateTask(ShowTask);
	
	GUICreateTask();			//创建空闲进程
  OS.Start();                                                  //MSOS启动
}



