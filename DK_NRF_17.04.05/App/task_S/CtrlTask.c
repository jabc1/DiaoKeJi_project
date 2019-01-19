/*********************************************************************************
* 文件名		: show.C
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 处理与菜单有关的函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/

#include "OS.h"
#include "app.h"
#include "plan.h"
#include "dk_conf.h"
#include "device_conf.h"
#include "motion_ctrl.h"
#include "queue.h"


void InitCtrl(void)
{



}

void CtrlTask(void)
{
	u32 message;

  while(1)
	{    		 
		message = (u32)OS.PendMessageQueue(10);
		switch(DK.Ctrl)
		{
			case CTRL_START:    	//开始运行
				DK_Ctrl_Run();
				break;
				
			case CTRL_PAUSE:    	
				break;			
			case CTRL_STEP_A:  	
				if(DK.State_B_P->DK_Step == 0)
				{
					DK.State_B_P->DK_Step = 1;
					DK_Ctrl_Run();					
				}
				break;			
			case CTRL_STEP_B:
				if(DK.State_B_P->DK_Step == 1)
				{
					DK.State_B_P->DK_Step = 0;
					DK_Ctrl_Run();					
				}
				break;
			case CTRL_STOP:
				//一些数据清零
				break;
			case CTRL_GOTOZERO:    							//Plan运行消息
				MotoGotoZero();
				break;			
			default:                                
				break;
		}

	}
}

