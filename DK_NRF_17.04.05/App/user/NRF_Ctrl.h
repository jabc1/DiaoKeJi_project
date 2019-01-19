
#ifndef __NRF_CTRL_H__
#define __NRF_CTRL_H__

#include "stm32f10x.h"  


typedef struct 
{
	u8 Ctrl;
	u8 Send;
	u16 Time_Out;
	u16 ERR_FC;
	u16 ERR_SEND;
	u16 ERR_S;
	u16 ERR_TIMEOUT;		
}NRF_Struct;  //CX  查询
typedef struct 
{
	u8 Buf[2][32];
	u8 New;		
	u8 Old;
}BUF_Struct;  //CX  查询
typedef struct 
{
	u8 FC;
	u8 Ctrl;					//DK控制命令
	s32 X;
	s32 Y;
	s32 Z;
	u32 N;		
	u32 State;
	u32 NRF_N;
	u8 queue;	
	u8 end;
}NRF_CX_Struct;  //CX  查询

typedef struct 
{
	u8 FC;
	u8 Ctrl;					//DK控制命令
	u8 TFTShowSize;		//TFT显示的倍率
	u8 	Mode;					//运行模式
	u16 Speed_XY;			//XY轴插补运行时的速度
	u16 Speed_Z;			//Z轴插补运行时的速度
	u16 Speed_Fast;		//快速运行时的速度
	u8 end;
}NRF_SYS_Struct;  //CX  查询
typedef struct 
{
	u8 FC;
	u8 Ctrl;					//DK控制命令
	u8 Year;			//公历日月年周
	u8 Month;
	u8 Date;				
	u8 Hour;
	u8 Min;
	u8 Sec;		
	u8 end;
}NRF_Time_Struct;  //CX  查询

typedef struct 
{
	u8 FC;
	u8 Ctrl;					//DK控制命令
	u8 Action;	//Gcode的功能码
	u16 ValueWords;
	s16 X;
	s16 Y;
	s16 I;
	s16 J;
	s16 Z;	
	u32 N;		
	u8 end;
}NRF_GCode_Struct;

typedef struct 
{
	u8 FC;
	u8 Ctrl;					//DK控制命令
	u16 DW_XY;
	u16 DW_Z;
	u16 DW_Speed;	
	u8 Spindle_On;
	u8 end;	
	
}NRF_DW_Struct;

#define	NRF_M_OK					0X00					//NRF主站就绪
#define	NRF_ERR_FC				0x10			//功能码错误错误
#define	NRF_ERR_SEND			0X11					//发送出错
#define	NRF_ERR_S					0X12					//从站接收数据校验错误
#define	NRF_ERR_TIMEOUT		0X13					//接收超时

#define	NRF_TIME_OUT			5				//超时等待次数

#define	NRF_S_START			0X10				//从站开始
#define	NRF_S_READY			0X20				//从站准备完毕

/*****通讯控制码****/
#define End_D				0x5A			//结束码
#define NRF_CX							0x10				//通讯协议：查询
#define NRF_CX_STATE				0x11				//通讯协议：查询
#define NRF_CX_SYS					0x12		


#define NRF_KZ							0x20				//通讯协议：控制
#define NRF_KZ_GCODE				0x21
#define NRF_KZ_DW						0x22
#define NRF_KZ_RESET				0x23
#define NRF_KZ_MAINMOTO_ON	0x24
#define NRF_KZ_MAINMOTO_OFF	0x25
#define NRF_KZ_MOTO_ON			0x26
#define NRF_KZ_MOTO_OFF			0x27
#define NRF_KZ_GOTOZERO			0x28

#define NRF_SZ						0x30				//通讯协议：查询

#define NRF_SZ_SYS				0x30	


#define NRF_REPEAT			0x80		//数据重发标志

extern NRF_Struct NRF;
extern u8 Tx_Buf[32];
extern BUF_Struct Rx_Buf;

#define RX_BUF 		Rx_Buf.Buf[0]
#define RX_BUF_O	Rx_Buf.Buf[Rx_Buf.Old]
#define RX_BUF_N	Rx_Buf.Buf[Rx_Buf.New]




void NRF_M_CRTL(u8 *txbuf);
void NRF_M_Init(void);



void NRF_S_Init(void);		
void NRF_S_Ctrl(void);

#endif 

