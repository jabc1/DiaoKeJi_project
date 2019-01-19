
#include "stm32f10x.h"
#include "NRF_S_CTRL.h"
#include "device_conf.h"
#include "NRFTask.h"





u8 Rx_Buf[32];
u8 Tx_Buf[32];
u8 NRF_Crtl = NRF_S_START;
/*----------------------------------------------------------------------*/

void NRF_S_CRTL(void)								// S 从站控制
{
		NRF_Crtl = NRF_S_READ;
		RX_Mode();		

}
void 	NRF_S_RX(void)
{
	if(NRF_Crtl == NRF_S_READ){		  					//从模式的接收模式
		if(NRF24L01_RxPacket(Rx_Buf)==0){			//一旦接收到信息,则显示出来.
				TX_Mode();			
			NRFTask_RX(Rx_Buf,Tx_Buf);						//数据解析处理
			NRF24L01_TxPacket(Tx_Buf);
			LED1=~LED1;
		}
	}
	RX_Mode();
}



void 	NRF_S_TX(u8 *txbuf)
{
		TX_Mode();
			if(NRF24L01_TxPacket(txbuf)==TX_OK)
			{
//			LCD_PutString(10,100,"S send ok");
				 		RX_Mode();		

			}else
			{										   	
//	   		 LCD_HEX(20,30,0X00,2);		   
			};
RX_Mode();	
}


