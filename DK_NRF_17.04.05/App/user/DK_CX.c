
#include "moto_4988.h"
#include "dk_conf.h"
#include "queue.h"
#include "NRF_Ctrl.h"

/********************************************
������DK_NRF_ReturnState
���ܣ���ѯ��̻�����״̬
������TX_Buf  ��������ָ��
���أ���
˵����
********************************************/
void DK_NRF_ReturnState(u8 *TX_Buf)
{
	NRF_CX_Struct *NRF_CX_P;
	NRF_CX_P=(NRF_CX_Struct*)TX_Buf;
	
	NRF_CX_P->X = Moto[AXIS_X].Current;	
	NRF_CX_P->Y = Moto[AXIS_Y].Current;	
	NRF_CX_P->Z = Moto[AXIS_Z].Current;	
	NRF_CX_P->NRF_N++;
	NRF_CX_P->N = DK.N;
	NRF_CX_P->queue = GCodeQueue.Entries;
//	NRF_CX_P->State = DK.Ctrl;	
	NRF_CX_P->end = End_D;
}
/********************************************
������DK_NRF_CX_SYS
���ܣ���ѯ��̻�ϵͳ״̬
������TX_Buf  ��������ָ��
���أ���
˵����
********************************************/
void DK_NRF_CX_SYS(u8 *TX_Buf)
{
	NRF_SYS_Struct *NRF_CX_SYS_P;
	NRF_CX_SYS_P = (NRF_SYS_Struct *)TX_Buf;
	NRF_CX_SYS_P->Speed_XY 		= DK.SYS.Speed_XY;
	NRF_CX_SYS_P->Speed_Z 		= DK.SYS.Speed_Z;
	NRF_CX_SYS_P->Speed_Fast 	= DK.SYS.Speed_Fast;	
	NRF_CX_SYS_P->TFTShowSize = DK.SYS.TFTShowSize;
	NRF_CX_SYS_P->Mode 				= DK.SYS.Mode;	
//	NRF_CX_SYS_P->Date 				= RTC_Calendar.Date;
//	NRF_CX_SYS_P->Hour 				= RTC_Calendar.Hour;
//	NRF_CX_SYS_P->Min 				= RTC_Calendar.Min;
//	NRF_CX_SYS_P->Month 			= RTC_Calendar.Month;
//	NRF_CX_SYS_P->Sec 				= RTC_Calendar.Sec;
//	NRF_CX_SYS_P->Year 				= RTC_Calendar.Year;
}

/********************************************
������DK_NRF_CX
���ܣ���ѯ����
������RX_Buf ��������ָ�룻TX_Buf  ��������ָ��
���أ���
˵����
********************************************/

void DK_NRF_CX(u8* RX_Buf,u8 *TX_Buf)
{
	switch (RX_Buf[0]&0x7F)
  {
  	case NRF_CX_STATE:
			DK_NRF_ReturnState(TX_Buf);
			TX_Buf[0] = NRF_CX_STATE;	
  		break;
  	case NRF_CX_SYS:
			DK_NRF_CX_SYS(TX_Buf);
			TX_Buf[0] = NRF_CX_SYS;	
  		break;
  	default:
  		break;
  }
	
	
}



