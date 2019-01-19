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

			case FuntionMessageType:                    //函数指针消息，直接以0x20或0x08来判断
				Function(message);
				break;	

			default:                                
				break;
		}

	}
}

