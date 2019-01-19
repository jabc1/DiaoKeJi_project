#include "device_conf.h"
#include "OS.h"
#include "APP.h"
#include "NRF_Ctrl.h"


/*----------------------------------------------------------------------*/

void DK_Select(u8 *RX_Buf,u8 *TX_Buf)
{
	
}


void NRFTask_RX(u8 *RX_Buf,u8 *TX_Buf)		//���ƴ���ѡ��
{
	switch (RX_Buf[0]&0x70)													 //ѡȡ��ȡģ��
	{
	case	NRF_CX:			 //ͨѶЭ�飺��ѯ
		DK_NRF_CX(RX_Buf,TX_Buf);
		DK.Ctrl = RX_Buf[1];			//��������
		break;
	case	NRF_KZ:			 //ͨѶЭ�飺����
		DK_NRF_KZ(RX_Buf,TX_Buf);
	DK_NRF_ReturnState(TX_Buf);
		break;	
	case	NRF_SZ:			 //ͨѶЭ�飺����
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
	
	while(NRF24L01_Check())//��ⲻ��24L01 
	{
		LED1 = 1;
		DelayMs(100);
	}	
	LED1 = 0;		
	NRF_S_Init();//����ģʽ 	
	while(1)
	{
		message = (u32)OS.PendMessageQueue(2);
		NRF_S_Ctrl();//�Ժ��Ϊ�жϽ���ģʽ
	}
}




