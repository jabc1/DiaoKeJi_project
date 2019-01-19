
#ifndef __NRF_S_CTRL_H__
#define __NRF_S_CTRL_H__

#include "stm32f10x.h"  
#include "dk_config.h"

#define	NRF_S_START			0X10				//从站开始
#define	NRF_S_READ			0X20				//从站准备完毕

#define	NRF_ERR_1			0x01			//超出读取范围
#define	NRF_ERR_2			0x02			//数据帧错误
#define	NRF_ERR_3			0x03			//超出读取范围
#define	NRF_ERR_4				0x04			//数据帧错误
#define End_D						0x5A			//结束码


/*****通讯控制码****/
#define NRF_CX						0x10				//通讯协议：查询
#define NRF_CX_STATE			0x11		
#define NRF_CX_STATE_R		0xF1	
#define NRF_CX_SPEED			0x12	
#define NRF_CX_SPEED_R		0xF2	
#define NRF_CX_TIME				0x13	
#define NRF_CX_TIME_R			0xF3	
#define NRF_CX_DATE				0x14	
#define NRF_CX_DATE_R			0xF4	

#define NRF_KZ						0x20				//通讯协议：控制
#define NRF_KZ_GCODE			0x21
#define NRF_KZ_GCODE_R		0xE1
#define NRF_KZ_DW					0x22
#define NRF_KZ_DW_R				0xE2
#define NRF_KZ_RESET			0x23
#define NRF_KZ_RESET_R		0xE3
#define NRF_KZ_MAINMOTO		0x24
#define NRF_KZ_MAINMOTO_R	0xE4



#define NRF_SZ						0x30				//通讯协议：查询

#define NRF_SZ_SPEED			0x32	
#define NRF_SZ_SPEED_R		0xD2	
#define NRF_SZ_TIME				0x33	
#define NRF_SZ_TIME_R			0xD3	
#define NRF_SZ_DATE				0x34	
#define NRF_SZ_DATE_R			0xD4	



typedef struct
{
	u8 FC;
	u8 Ctrl;
	u8 FileState;	
	u8 Alarm;			//Gcode运行模式，0： Z轴由MO3，MO5控制；1： Z轴由G代码Z坐标控制，MO3 MO5控制电机启停
}CtrlStruct;

typedef struct 
{
	CtrlStruct Ctrl;	
	s32 X;
	s32 Y;
	s32 Z;
	u32 N;		
	u32 State;
	u8 queue;	
	u8 end;
}NRF_CX_Struct;  //CX  查询

typedef struct 
{
	CtrlStruct Ctrl;	
	Calendar_Struct Calendar;
	DKConfigStruct Conf;
}NRF_CX_SYS_Struct;  //CX  查询



//typedef struct 
//{
//	CtrlStruct Ctrl;	
//	u32 Speed_XY;
//	u32 Speed_Z;
//	u32 Speed_Fast;
//	u8 end;
//}NRF_SPEED_Struct;  //CX  查询

typedef struct 
{
	CtrlStruct Ctrl;	
	GCodeStruct GCode;
}NRF_GCodeS;



typedef struct 
{
	CtrlStruct Ctrl;	
	u16 DW_XY;
	u16 DW_Z;
	u16 DW_Speed;	
	u8 end;	
}NRF_Ctrl_DWStruct;


extern u8 Rx_Buf[32];
extern u8 Tx_Buf[32];
extern u8 NRF_Crtl;

void NRF_S_CRTL(void);
void NRF_S_RX(void);
void NRF_S_TX(u8 *txbuf);


#endif 

