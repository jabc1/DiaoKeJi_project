/*********************************************************************************
* 文件名		: show.C
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 处理与菜单有关的函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/

#include "OS.h"
#include "Page.h"
#include "device_conf.h"
#include "malloc.h"	
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h"

void usb_port_set(u8 enable)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000; 
	}
}
void Page_SetUSB(void)
{
	Page = 14;
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);

	Mass_Memory_Size[0]=SDCardInfo.CardCapacity;		//得到SD卡容量（字节），当SD卡容量超过4G的时候,需要用到两个u32来表示
	Mass_Block_Size[0] =512;							//因为我们在Init里面设置了SD卡的操作字节为512个,所以这里一定是512个字节.
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];

	DelayMs(1800);
	USB_Port_Set(0); 	//USB先断开
	DelayMs(700);
	USB_Port_Set(1);	//USB再次连接 

	GUI_PutStringAt("USB Connecting...",70,40);		//提示USB开始连接
	Data_Buffer=mymalloc(BULK_MAX_PACKET_SIZE*2*4);	//为USB数据缓存区申请内存
	Bulk_Data_Buff=mymalloc(BULK_MAX_PACKET_SIZE);	//申请内存
	//USB配置
	USB_Interrupts_Config();    
	Set_USBClock();   
	USB_Init();			
}


