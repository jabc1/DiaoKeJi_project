/*********************************************************************************
* �ļ���		: show.C
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ������˵��йصĺ���					  
* �÷�			��
* �汾˵��	��
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
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000; 
	}
}
void Page_SetUSB(void)
{
	Page = 14;
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);

	Mass_Memory_Size[0]=SDCardInfo.CardCapacity;		//�õ�SD���������ֽڣ�����SD����������4G��ʱ��,��Ҫ�õ�����u32����ʾ
	Mass_Block_Size[0] =512;							//��Ϊ������Init����������SD���Ĳ����ֽ�Ϊ512��,��������һ����512���ֽ�.
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];

	DelayMs(1800);
	USB_Port_Set(0); 	//USB�ȶϿ�
	DelayMs(700);
	USB_Port_Set(1);	//USB�ٴ����� 

	GUI_PutStringAt("USB Connecting...",70,40);		//��ʾUSB��ʼ����
	Data_Buffer=mymalloc(BULK_MAX_PACKET_SIZE*2*4);	//ΪUSB���ݻ����������ڴ�
	Bulk_Data_Buff=mymalloc(BULK_MAX_PACKET_SIZE);	//�����ڴ�
	//USB����
	USB_Interrupts_Config();    
	Set_USBClock();   
	USB_Init();			
}


