
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

/*-------------------------------------��վ����-----------------------------------------------------*/
void NRF_M_Init(void)								// S ��վ����
{
		NRF.Ctrl = NRF_M_OK;
		RX_Mode();	
}
void NRF_M_Send(u8 *TX_Buf)
{
	NRF.Time_Out=0;					//���ͨѶ��ʱ										 
	TX_Mode();
	OS.DelayTimeTick(1);
	if(NRF24L01_TxPacket(TX_Buf)==TX_OK)			 //���ͳɹ�
	{
		RX_Mode();														  //���ý���ģʽ				
		while(NRF24L01_RxPacket(RX_BUF)!=0)		//�ȴ����յ�����
		{
			OS.DelayTimeTick(2);
			NRF.Time_Out++;			
			if(NRF.Time_Out >=NRF_TIME_OUT)
			{
				NRF.Ctrl = NRF_ERR_TIMEOUT;	//��ʱ�ȴ���������������ֵ  
				NRF.Time_Out++;
				return;
			}
		}
		NRFTask_RX(RX_BUF,Tx_Buf);						//���ݽ�������			
	}
	else			//����ʧ�� �ȴ��´��ط�
	{										   	
		NRF.Ctrl = NRF_ERR_SEND;
		NRF.ERR_SEND++;
	}		
}

void NRF_M_CRTL(u8 *TX_Buf)				 //M ��վ����
{
	NRF.Ctrl = NRF_M_OK;
	while(NRF.Send)		//
	{
		if(NRF.Ctrl != NRF_M_OK)
		{//ͨѶ����
			DK.ErrN ++;
			Tx_Buf[0] |= NRF_REPEAT; 
			OS.DelayTimeTick(10);			
		}			
		NRF_M_Send(TX_Buf);		
	}
}





/*-------------------------------------��վ����-----------------------------------------------------*/
u8 RXBUFF_IsSame(void)
{
	u8 i;
	for(i=0;i<32;i++)
	{
		if(RX_BUF_N[i] !=RX_BUF_O[i]) return false;
	}
	return true;
}

void NRF_S_Init(void)								// S ��վ����
{
		NRF.Ctrl = NRF_S_READY;
		RX_Mode();		

}
void 	NRF_S_Ctrl(void)
{
	if(NRF.Ctrl == NRF_S_READY){		  					//��ģʽ�Ľ���ģʽ
		if(NRF24L01_RxPacket(RX_BUF_N)==0){			//һ�����յ���Ϣ,����ʾ����.
			TX_Mode();			
			if(RX_BUF_N[0] & NRF_REPEAT)		//�����ط�
			{
				
				if(RXBUFF_IsSame()&&((RX_BUF_N[0] & NRF_REPEAT) == NRF_KZ_GCODE))			//���ϴε�G������ͬ
				{//�����ط�Ϊ G���룬�����ϴ���ͬ
					RX_BUF_N[0] = NRF_CX_STATE;
					NRFTask_RX(RX_BUF_N,Tx_Buf);
					Tx_Buf[0] = NRF_KZ_GCODE | NRF_REPEAT;
				}
				else
				{		//�������ϴ���ͬ��G������������������
					NRFTask_RX(RX_BUF_N,Tx_Buf);
					Switch_Buf();
				}
				
			}
			else
			{
					NRFTask_RX(RX_BUF_N,Tx_Buf);						//�������ݽ�������	
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







