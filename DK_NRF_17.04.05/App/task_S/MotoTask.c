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
#include "device_conf.h"


void MotoTask(void)
{
	u32 message;
	InitPlanQueue();
	
	
	
  while(1)
	{    		 
		message = (u32)OS.PendMessageQueue(5000);
		
		switch(GetMessageType(message))
		{
			case UseMessage_PlanRun:    							//Plan������Ϣ
				PlanRun();
				break;
			case FuntionMessageType:                    //����ָ����Ϣ��ֱ����0x20��0x08���ж�
				Function(message);
				break;	

			default:                                
				break;
		}

	}
}

