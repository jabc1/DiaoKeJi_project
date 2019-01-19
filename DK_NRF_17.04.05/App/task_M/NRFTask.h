
#ifndef __NRFTASK_H__
#define __NRFTASK_H__
#include "stm32f10x.h"
#include "G_CODE.h"


//void Send_CX_SYS(void);
void Send_SZ_StopMainMoto(void);
void Send_SZ_GotoZero(void);
//void Send_CX_State(void);
void Send_GCode(GCodeStruct * GCode);
void Send_DW(u8 Speed,u8 XY_Key,u8 Z_Key);
void Send_GotoZero(u8 Speed,u8 XY_Key,u8 Z_Key);
void Send_SZ_SYS(void);
void Save_SYS_Buf(u8 *RX_Buf);
void Save_CX_Buf(u8 *RX_Buf);


void Send_DK_Action(u8 Action);



void NRFTask_RX(u8 *RX_Buf,u8 *TX_Buf);		//¿ØÖÆ´úÂëÑ¡Ôñ
void NRFTask(void);

#endif



