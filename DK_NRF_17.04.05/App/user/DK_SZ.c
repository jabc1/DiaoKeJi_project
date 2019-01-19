#include "OS.h"
#include "moto_4988.h"
#include "dk_conf.h"
#include "NRFTask.h"
#include "NRF_Ctrl.h"

void DK_NRF_SZ_Speed(u8* RX_Buf)
{
	NRF_SYS_Struct *NRF_SYS_P;
	NRF_SYS_P = (NRF_SYS_Struct *) RX_Buf;
	DK.SYS.Speed_XY =  NRF_SYS_P->Speed_XY;
	DK.SYS.Speed_Z =  NRF_SYS_P->Speed_Z;
	DK.SYS.Speed_Fast =  NRF_SYS_P->Speed_Fast;		
	DK.SYS.TFTShowSize = NRF_SYS_P->TFTShowSize;
	SaveData();
	Set_Speed_XY(DK.SYS.Speed_XY);
	Set_Speed_Z(DK.SYS.Speed_Z);
}






void DK_NRF_SZ(u8* RX_Buf,u8 *TX_Buf)
{
	switch (RX_Buf[0]&0x7F)
  {
  	case NRF_SZ_SYS:
			DK_NRF_SZ_Speed(RX_Buf);
			TX_Buf[0] = NRF_SZ_SYS;	
  		break;
  	default:
  		break;
  }
	DK_NRF_ReturnState(TX_Buf);
	
}



