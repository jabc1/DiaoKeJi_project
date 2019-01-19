#include "device_conf.h"
#include "OS.h"
#include "APP.h"
#include "NRF_Ctrl.h"


/*----------------------------------------------------------------------*/

void DK_Select(u8 *RX_Buf,u8 *TX_Buf)
{
	
}


void NRFTask_RX(u8 *RX_Buf,u8 *TX_Buf)		//控制代码选择
{
	switch (RX_Buf[0]&0x70)													 //选取读取模块
	{
	case	NRF_CX:			 //通讯协议：查询
		DK_NRF_CX(RX_Buf,TX_Buf);
		DK.Ctrl = RX_Buf[1];			//控制命令
		break;
	case	NRF_KZ:			 //通讯协议：控制
		DK_NRF_KZ(RX_Buf,TX_Buf);
	DK_NRF_ReturnState(TX_Buf);
		break;	
	case	NRF_SZ:			 //通讯协议：设置
		DK_NRF_SZ(RX_Buf,TX_Buf);
		break;			
	default:
		TX_Buf[0] = 0xFF;
		TX_Buf[1] = 0x00;
		TX_Buf[2] = NRF_ERR_FC;
		TX_Buf[3] = End_D;				
	}			
}
void NRFTask(void)
{
	u32 message;

	
	NRF24L01_Init();
	Rx_Buf.New = 1;
	Rx_Buf.Old = 0;	
		LED1 = 0;	
	
	while(NRF24L01_Check())//检测不到24L01 
	{
		LED1 = 1;
		DelayMs(100);
	}	
	LED1 = 0;		
	NRF_S_Init();//接收模式 	
	while(1)
	{
		message = (u32)OS.PendMessageQueue(2);
		NRF_S_Ctrl();//以后改为中断接收模式
	}
}




