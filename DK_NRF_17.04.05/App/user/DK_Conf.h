
#ifndef __DKCONFIG_H__
#define __DKCONFIG_H__

#include "stm32f10x.h" 
#include "g_code.h"  	

#include "MOTO_4988.h"  	
#include "plan.h" 	
#include "limit.h"	
	
#define POSITIVE 	1			//�������
#define REVERSE 	0			//�������

#define  BeiLv  100		//��������/MM��ʵ����100�������Ӧ1mm���ƶ�����
#define Z_UP_NUM	200		//M05����������������

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






#define DK_DELAY					5		//ÿ��G�����ȴ�����ʱʱ��  *1ms


////����XYZ�ֱ�ʹ���ĸ��������
#define AXIS_X  1			
#define AXIS_Y  2
#define AXIS_Z  0

#define FAIL(status) return(status);

/*  CTRL��0000  0000  
			    |+++  ++++
					| |     +- ���ƴ��� 	0000�����ֹͣ
															0001����̿�ʼ
															0010����������A
															0011����������B
					| +------- ͨѶ���� 	000����ѯSYS
					|					 					001������SYS�����ƴ������Ϊ0
					|					 					010������G����
					|					 					011����ѯ״̬
					|										100����λ����
					|					 					101��������λ�����ƴ������Ϊ0
					|					 					110���ر����ᣬ���ƴ������Ϊ0
					+----------�Ƿ�Ϊ�ط�����
										

*/

#define CTRL_STOP					0x00		//���ֹͣ����
#define CTRL_START				0x10		//��̿�ʼ����
#define CTRL_PAUSE				0x11		//�����ͣ����
#define CTRL_STEP_A				0x12		//��̵�������
#define CTRL_STEP_B				0x13		//��̵�����ɣ��ȴ��´ε����������������
#define CTRL_SET_DW				0x18		//��̶�λ����
#define CTRL_GOTOZERO			0x19		//��̶�λ����

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
}DK_B_File_Struct;		//������λ����



typedef struct
{
u32 L[3];			
u32 R[3];
}DK_B_Alarm_Struct;		//������λ����

typedef struct
{
//	u32 MainMotoRun;			
	u32 DK_Step;					//��������ʱ�Ĳ���״̬ ������������ʹ��
	u32 Inches_Mode;			//Ӣ��ģʽ
	u32 Absolute_Mode;		//��������ģʽ
	u32 Spindle_On;				//����״̬
	u32 Axis_EN;					//�������ʹ��״̬
}DK_B_StateStruct;  //״̬��λ����

///��̻��Ľṹ��
typedef struct 
{
	u8 TFTShowSize;		//TFT��ʾ�ı���
	u8 	Mode;					//����ģʽ
	u16 Speed_XY;			//XY��岹����ʱ���ٶ�
	u16 Speed_Z;			//Z��岹����ʱ���ٶ�
	u16 Speed_Fast;		//��������ʱ���ٶ�
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
	u8 Ctrl;					//DK��������
//	u8 NRF_FC;				//NRF������
	u8 File;					//sd���ļ�����
	u8 State;					//״̬
	u8 Alarm;					//����
	u8 GC_Status;  //״̬���� ��g_code.c��ʹ��
	u32 N;				//G���������
	u32 ErrN;
	DK_SYS_Struct SYS;
	DK_NRF_Struct NRF;	
	
	DK_B_File_Struct* File_B_P;	
	DK_B_Alarm_Struct* Alarm_B_P;	//������λ����ṹ��
	DK_B_StateStruct* State_B_P;	//״̬��λ����ṹ��
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

