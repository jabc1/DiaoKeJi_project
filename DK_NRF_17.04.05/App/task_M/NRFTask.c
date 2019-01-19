#include "device_conf.h"
#include "NRFTask.h"
#include "OS.h"
#include "app.h"
#include "NRF_CTRL.h"
//#include "page.h"
#include "dk_conf.h"
void Send_DK_Action(u8 Action)
{
	NRF.Send = 1;
	Tx_Buf[0] = Action;
	Tx_Buf[1] = DK.Ctrl;
	Tx_Buf[2] = End_D;		
	OS.PostMessageQueue(NRFTaskPriority, (void *)((u32)UseMessage_NRF <<24));			
}

void Send_GCode(GCodeStruct * GCode)
{
	NRF_GCode_Struct *NRF_GCode_P;	
	NRF.Send = 1;
	NRF_GCode_P = (NRF_GCode_Struct *)Tx_Buf;	
	NRF_GCode_P->FC = NRF_KZ_GCODE;
	NRF_GCode_P->Action = GCode->Action;
	NRF_GCode_P->ValueWords = GCode->ValueWords;
	NRF_GCode_P->X = GCode->X;
	NRF_GCode_P->Y = GCode->Y;	
	NRF_GCode_P->Z = GCode->Z;	
	NRF_GCode_P->I = GCode->I;
	NRF_GCode_P->J = GCode->J;		
	NRF_GCode_P->N = DK.N;
	NRF_GCode_P->end = End_D;
	OS.PostMessageQueue(NRFTaskPriority, (void *)((u32)UseMessage_NRF <<24));
}
void Send_DW(u8 Speed,u8 XY_Key,u8 Z_Key)
{
	NRF_DW_Struct *NRF_GCode_P;	
	NRF.Send = 1;	
	NRF_GCode_P = (NRF_DW_Struct *)Tx_Buf;	
	NRF_GCode_P->FC 			= NRF_KZ_DW;
	NRF_GCode_P->Ctrl 		= CTRL_SET_DW;
	NRF_GCode_P->DW_XY 		= XY_Key;
	NRF_GCode_P->DW_Z 		= Z_Key;
	NRF_GCode_P->DW_Speed = Speed;	
	NRF_GCode_P->Spindle_On = DK.State_B_P->Spindle_On;
	NRF_GCode_P->end 			= End_D;
	OS.PostMessageQueue(NRFTaskPriority, (void *)((u32)UseMessage_NRF <<24));
}

void Send_SZ_SYS(void)
{
	NRF_SYS_Struct *Set_SYS_P;	
	NRF.Send = 1;	
	Set_SYS_P = (NRF_SYS_Struct *)Tx_Buf;
	Set_SYS_P->FC = NRF_SZ_SYS;
	Set_SYS_P->Ctrl = DK.Ctrl;		
	Set_SYS_P->Speed_XY = DK.SYS.Speed_XY;
	Set_SYS_P->Speed_Z = DK.SYS.Speed_Z;
	Set_SYS_P->Speed_Fast = DK.SYS.Speed_Fast;		
	Set_SYS_P->TFTShowSize = DK.SYS.TFTShowSize;
	Set_SYS_P->end = End_D;
	OS.PostMessageQueue(NRFTaskPriority, (void *)((u32)UseMessage_NRF <<24));
}


void Save_SYS_Buf(u8 *RX_Buf)
{
	NRF_SYS_Struct *Set_SYS_P;	
	Set_SYS_P = (NRF_SYS_Struct *)RX_Buf;
	DK.SYS.Speed_Fast = 		Set_SYS_P->Speed_Fast;
	DK.SYS.Speed_XY = 			Set_SYS_P->Speed_XY;
	DK.SYS.Speed_Z = 				Set_SYS_P->Speed_Z;
	DK.SYS.TFTShowSize = 		Set_SYS_P->TFTShowSize;
	DK.Ctrl = CTRL_STOP;
	LED2 = !LED2;
	NRF.Send = 0;
}


void Save_CX_Buf(u8 *RX_Buf)
{
	NRF_CX_Struct * DK_State_P;	
	DK_State_P = (NRF_CX_Struct *)RX_Buf;
	DK.NRF.X = DK_State_P->X >>6;
	DK.NRF.Y = DK_State_P->Y >>6;
	DK.NRF.Z = DK_State_P->Z >>6;
	
	DK.NRF.NRF_N = DK_State_P->NRF_N;		
	DK.NRF.N = DK_State_P->N;		
	DK.Alarm = DK_State_P->State;
	DK.NRF.queue = DK_State_P->queue;	
	LED2 = !LED2;
	NRF.Send = 0;
}
	








void NRFTask_RX(u8 *RX_Buf,u8 *TX_Buf)		//控制代码选择
{
			switch (RX_Buf[0])
      {
      	case NRF_CX_STATE:
      	case NRF_KZ_GCODE:
      	case NRF_KZ_DW:					
      	case NRF_KZ_RESET:
      	case NRF_KZ_MAINMOTO_ON:		
      	case NRF_KZ_MOTO_ON:					
      	case NRF_KZ_MOTO_OFF:	
      	case NRF_KZ_MAINMOTO_OFF:		
      	case NRF_KZ_GOTOZERO:							
					Save_CX_Buf(RX_Buf);			
					NRF.Ctrl=NRF_M_OK;
					NRF.Send = 0;
      		break;	
      	case NRF_SZ_SYS:	
					Save_CX_Buf(RX_Buf);		
					NRF.Ctrl=NRF_M_OK;
					NRF.Send = 0;
      		break;
      	case NRF_CX_SYS:				
					Save_SYS_Buf(RX_Buf);
					NRF.Ctrl=NRF_M_OK;
					NRF.Send = 0;
					break;				
				case 0xFF:
						NRF.Ctrl = NRF_ERR_S;
						NRF.ERR_S++;
					break;								
      	default:
						NRF.Ctrl = NRF_ERR_FC;	
						NRF.ERR_FC++;
      		break;
      }

}

void NRFTask(void)
{
	u32 message;
	NRF24L01_Init();			//初始化NRF
	while(NRF24L01_Check())//检测不到24L01
	{
		DelayMs(100);
		GUI_PutStringAt((u8*)"NO NRF24L01",70,40);	//主站没有NRF的话就不用继续运行
	}	
	NRF_M_Init();			//主站模式    清除24L01里的数据		
	DK.Ctrl = CTRL_STOP;
	Send_DK_Action(NRF_CX_SYS);			//查询DK系统状态
	NRF_M_CRTL(Tx_Buf);	
	while(NRF.Send == 1)
	{
		
	}	

	
	
	while(1)
	{
		message = (u32)OS.PendMessageQueue(5);
		
		switch(GetMessageType(message))
		{
			case UseMessage_NRF:                    //
				NRF_M_CRTL(Tx_Buf);	

				break;	

			default:     

				break;
		}		
		
		
		
		


	}
}

