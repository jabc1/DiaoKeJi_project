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
#include "device_CONF.h"
#include "NRF_CTRL.h"


void MotoTask(void)
{
	u32 message;

	
  while(1)
	{    		 
		message = (u32)OS.PendMessageQueue(100);
		
		switch(GetMessageType(message))
		{

			case FuntionMessageType:                    //����ָ����Ϣ��ֱ����0x20��0x08���ж�
				Function(message);
				break;	

			default:                                
				break;
		}

	}
}

