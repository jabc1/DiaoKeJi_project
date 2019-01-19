
#ifndef __DKCONFIG_H__
#define __DKCONFIG_H__

#include "stm32f10x.h" 
#include "g_code.h"  	

#include "MOTO_4988.h"  	
#include "plan.h" 	
#include "limit.h"	
	
#define POSITIVE 	1			//电机正向
#define REVERSE 	0			//电机反向

#define  BeiLv  100		//多少脉冲/MM。实际中100个脉冲对应1mm的移动距离
#define Z_UP_NUM	200		//M05命令上升多少脉冲

//#define GC_DEFAULT 						21
#define GC_DWELL_G4 					5
#define GC_GO_HOME_G28 				28
#define GC_RESET_XYZ_G92 			92
#define GC_STOP 							22
#define GC_SEEK_G0 						1 // G0 
#define GC_LINEAR_G1 					2 // G1
//#define GC_EXTRUDER_STOP 			7
//#define GC_EXTRUDER_ON 				8
//#define GC_EXTRUDER_FAST_T 		9
//#define GC_EXTRUDER_WAIT_T 		10
#define GC_CW_ARC 						3
#define GC_CCW_ARC 						4

#define GC_M03 						103
#define GC_M04 						104
#define GC_M05 						105






#define DK_DELAY					5		//每个G代码间等待的延时时间  *1ms


////定义XYZ分别使用哪个出口输出
#define AXIS_X  1			
#define AXIS_Y  2
#define AXIS_Z  0

#define FAIL(status) return(status);

/*  CTRL：0000  0000  
			    |+++  ++++
					| |     +- 控制代码 	0000：雕刻停止
															0001：雕刻开始
															0010：单步命令A
															0011：单步命令B
					| +------- 通讯代码 	000：查询SYS
					|					 					001：设置SYS，控制代码必须为0
					|					 					010：发送G代码
					|					 					011：查询状态
					|										100：定位控制
					|					 					101：返回零位，控制代码必须为0
					|					 					110：关闭主轴，控制代码必须为0
					+----------是否为重发数据
										

*/

#define CTRL_STOP					0x00		//雕刻停止命令
#define CTRL_START				0x10		//雕刻开始命令
#define CTRL_PAUSE				0x11		//雕刻暂停命令
#define CTRL_STEP_A				0x12		//雕刻单步命令
#define CTRL_STEP_B				0x13		//雕刻单步完成，等待下次单步命令或运行命令
#define CTRL_SET_DW				0x18		//雕刻定位命令
#define CTRL_GOTOZERO			0x19		//雕刻定位命令

#define DW_NUM_NULL			0

#define DW_NUM_ZS				0x01
#define DW_NUM_S				0x02
#define DW_NUM_YS				0x03
#define DW_NUM_Z				0x04
#define DW_NUM_Y				0x05
#define DW_NUM_ZX				0x06
#define DW_NUM_X				0x07
#define DW_NUM_YX				0x08
#define DW_NUM_XY_0			0x0F

#define DW_NUM_Z_UP			0x11
#define DW_NUM_Z_DN			0x12
#define DW_NUM_Z_0			0x1F

typedef struct
{
u32 End;
u32 NotEnd;
u32 NeedRead;
u32 Open;
u32 ReadStart;
u32 ShowTFT;
}DK_B_File_Struct;		//报警的位处理



typedef struct
{
u32 L[3];			
u32 R[3];
}DK_B_Alarm_Struct;		//报警的位处理

typedef struct
{
//	u32 MainMotoRun;			
	u32 DK_Step;					//步进操作时的步进状态 与控制命令配合使用
	u32 Inches_Mode;			//英寸模式
	u32 Absolute_Mode;		//绝对坐标模式
	u32 Spindle_On;				//主轴状态
	u32 Axis_EN;					//步进电机使能状态
}DK_B_StateStruct;  //状态的位处理

///雕刻机的结构体
typedef struct 
{
	u8 TFTShowSize;		//TFT显示的倍率
	u8 	Mode;					//运行模式
	u16 Speed_XY;			//XY轴插补运行时的速度
	u16 Speed_Z;			//Z轴插补运行时的速度
	u16 Speed_Fast;		//快速运行时的速度
//	u16 State;
}DK_SYS_Struct;
typedef struct 
{
//	u8 NRF_FC;
	u8 queue;
	s32 X;
	s32 Y;
	s32 Z;
	u32 N;
	u32 NRF_N;
}DK_NRF_Struct;

typedef struct 
{
	u8 Ctrl;					//DK控制命令
//	u8 NRF_FC;				//NRF功能码
	u8 File;					//sd卡文件中用
	u8 State;					//状态
	u8 Alarm;					//报警
	u8 GC_Status;  //状态代码 在g_code.c中使用
	u32 N;				//G代码的数量
	u32 ErrN;
	DK_SYS_Struct SYS;
	DK_NRF_Struct NRF;	
	
	DK_B_File_Struct* File_B_P;	
	DK_B_Alarm_Struct* Alarm_B_P;	//报警的位处理结构体
	DK_B_StateStruct* State_B_P;	//状态的位处理结构体
}DiaoKe_Struct;

#define FAIL(status) return(status);

#define FAIL_WORD_REPEATED 1
#define FAIL_WORD_NOGCODE	 2

extern DiaoKe_Struct DK;

void SaveData(void);
void LoadData(void);
void LoadSYSData(void);

void DK_Ctrl_Run(void);
void DK_Ctrl_DW(u8* Rx_Buf);
void DK_NRF_KZ(u8* RX_Buf,u8 *TX_Buf);

void DK_NRF_ReturnState(u8 *TX_Buf);
void DK_NRF_CX(u8* RX_Buf,u8 *TX_Buf);
void DK_NRF_SZ(u8* RX_Buf,u8 *TX_Buf);

#endif 

