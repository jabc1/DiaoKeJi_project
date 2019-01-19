
#include "stm32f10x.h"
#include "NRF_CTRL.h"
#include "app.h"
#include "device_conf.h"
#include "os.h"

u8 Tx_Buf[32];
BUF_Struct Rx_Buf;
NRF_Struct NRF;
//u8 NRF_Ctrl = NRF_M_OK;
//u16 NRFTimeOut=0;

void Switch_Buf(void)
{
	if(Rx_Buf.Old == 0)
	{
		Rx_Buf.Old = 1;
		Rx_Buf.New = 0;
	}
	else
	{
		Rx_Buf.Old = 0;
		Rx_Buf.New = 1;		
	}
}

/*-------------------------------------主站控制-----------------------------------------------------*/
void NRF_M_Init(void)								// S 从站控制
{
		NRF.Ctrl = NRF_M_OK;
		RX_Mode();	
}
void NRF_M_Send(u8 *TX_Buf)
{
	NRF.Time_Out=0;					//清除通讯超时										 
	TX_Mode();
	OS.DelayTimeTick(1);
	if(NRF24L01_TxPacket(TX_Buf)==TX_OK)			 //发送成功
	{
		RX_Mode();														  //设置接收模式				
		while(NRF24L01_RxPacket(RX_BUF)!=0)		//等待接收的数据
		{
			OS.DelayTimeTick(2);
			NRF.Time_Out++;			
			if(NRF.Time_Out >=NRF_TIME_OUT)
			{
				NRF.Ctrl = NRF_ERR_TIMEOUT;	//超时等待次数超过了设置值  
				NRF.Time_Out++;
				return;
			}
		}
		NRFTask_RX(RX_BUF,Tx_Buf);						//数据解析处理			
	}
	else			//发送失败 等待下次重发
	{										   	
		NRF.Ctrl = NRF_ERR_SEND;
		NRF.ERR_SEND++;
	}		
}

void NRF_M_CRTL(u8 *TX_Buf)				 //M 主站控制
{
	NRF.Ctrl = NRF_M_OK;
	while(NRF.Send)		//
	{
		if(NRF.Ctrl != NRF_M_OK)
		{//通讯错误
			DK.ErrN ++;
			Tx_Buf[0] |= NRF_REPEAT; 
			OS.DelayTimeTick(10);			
		}			
		NRF_M_Send(TX_Buf);		
	}
}





/*-------------------------------------从站控制-----------------------------------------------------*/
u8 RXBUFF_IsSame(void)
{
	u8 i;
	for(i=0;i<32;i++)
	{
		if(RX_BUF_N[i] !=RX_BUF_O[i]) return false;
	}
	return true;
}

void NRF_S_Init(void)								// S 从站控制
{
		NRF.Ctrl = NRF_S_READY;
		RX_Mode();		

}
void 	NRF_S_Ctrl(void)
{
	if(NRF.Ctrl == NRF_S_READY){		  					//从模式的接收模式
		if(NRF24L01_RxPacket(RX_BUF_N)==0){			//一旦接收到信息,则显示出来.
			TX_Mode();			
			if(RX_BUF_N[0] & NRF_REPEAT)		//数据重发
			{
				
				if(RXBUFF_IsSame()&&((RX_BUF_N[0] & NRF_REPEAT) == NRF_KZ_GCODE))			//与上次的G代码相同
				{//数据重发为 G代码，且与上次相同
					RX_BUF_N[0] = NRF_CX_STATE;
					NRFTask_RX(RX_BUF_N,Tx_Buf);
					Tx_Buf[0] = NRF_KZ_GCODE | NRF_REPEAT;
				}
				else
				{		//除了与上传相同的G代码外其他正常处理
					NRFTask_RX(RX_BUF_N,Tx_Buf);
					Switch_Buf();
				}
				
			}
			else
			{
					NRFTask_RX(RX_BUF_N,Tx_Buf);						//正常数据解析处理	
					Switch_Buf();		
			}		
			
			if(NRF24L01_TxPacket(Tx_Buf)==TX_OK){
			LED1=1;
			}
			else
			{
			LED1=0;
			}


		}
	}
	RX_Mode();
}







