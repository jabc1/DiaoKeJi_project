/*********************************************************************************
* �ļ���		: show.C
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ������˵��йصĺ���					  
* �÷�			��
* �汾˵��	��
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
			case CTRL_START:    	//��ʼ����
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
				//һЩ��������
				break;
			case CTRL_GOTOZERO:    							//Plan������Ϣ
				MotoGotoZero();
				break;			
			default:                                
				break;
		}

	}
}

