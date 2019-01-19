
#ifndef __DKCONFIG_H__
#define __DKCONFIG_H__

#include "stm32f10x.h"  
#include "g_code.h"    
#include "device_conf.h"   	
#define POSITIVE 	1			//�������
#define REVERSE 	2			//�������

//#define  BeiLv  100		//��������/MM��ʵ����100�������Ӧ1mm���ƶ�����
#define Z_UP_NUM	200		//M05����������������

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
	u32 Inches_Mode;			
	u32 Absolute_Mode;
	u32 Spindle_On;
	u32 Axis_EN;		
}DK_B_SYS_Struct;		//������λ����

//typedef struct
//{
//	u32 MainMotoRun;

//}DK_B_StateStruct;  //״̬��λ����



typedef struct
{
	u8 FC;
	u8 Ctrl;
	u8 Alarm;			//Gcode����ģʽ��0�� Z����MO3��MO5���ƣ�1�� Z����G����Z������ƣ�MO3 MO5���Ƶ����ͣ
	u8 BeiYong;	
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
}NRF_CX_Struct;  //CX  ��ѯ

typedef struct 
{
	CtrlStruct Ctrl;	
//	u8 Year;			//������������
//	u8 Month;
//	u8 Date;				
//	u8 Hour;
//	u8 Min;
//	u8 Sec;		
	u8 TFTShowSize;		//TFT��ʾ�ı���
	u8 	Mode;					//����ģʽ
	u16 Speed_XY;			//XY��岹����ʱ���ٶ�
	u16 Speed_Z;			//Z��岹����ʱ���ٶ�
	u16 Speed_Fast;		//��������ʱ���ٶ�
}NRF_CX_SYS_Struct;  //CX  ��ѯ


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

typedef struct 
{
	u8 TFTShowSize;		//TFT��ʾ�ı���
	u8 	Mode;					//����ģʽ
	u16 Speed_XY;			//XY��岹����ʱ���ٶ�
	u16 Speed_Z;			//Z��岹����ʱ���ٶ�
	u16 Speed_Fast;		//��������ʱ���ٶ�
	u16 State;
}NRF_SYS_DWStruct;
typedef struct 
{
	u8 queue;
	s16 X;
	s16 Y;
	s16 Z;
	u32 N;
}NRF_REF_DWStruct;

typedef struct 
{
	u8 Ctrl;					//��������
	u8 File;
	u8 State;					//״̬
	u8 Alarm;					//����
	NRF_SYS_DWStruct SYS;
	NRF_REF_DWStruct REF;
	

	DK_B_Alarm_Struct* Alarm_B_P;	//������λ����ṹ��
//	DK_B_StateStruct* State_B_P;
	DK_B_File_Struct* File_B_P;
	DK_B_SYS_Struct * SYS_B_P;	//ϵͳ״̬��λ����ṹ��
	
}DiaoKe_Struct;

extern DiaoKe_Struct DK;


#define CTRL_STOP					0x00		//���ֹͣ��־
#define CTRL_START				0x01		//��̿�ʼ��־
#define CTRL_PAUSE				0x02		//�����ͣ��־
#define CTRL_STEP					0x04		//�����ͣ��־
//#define CTRL_RUN					0x08		//�����ͣ��־
#define CTRL_SET_DW				0x10		//��̶�λ����
#define CTRL_SET_DW_O			0x11		//��̶�λִ�����
#define CTRL_SET_SPEED		0x20		//�����ͣ��־
#define CTRL_SET_SPEED_OK		0x21		//�����ͣ��־
#define CTRL_GOTOZERO			0x40		//�����ͣ��־


//#define SD_OR_COMM				13			//SD����ȡG���뻹��ͨ�����ڶ�ȡ  1ΪSD����ȡ��0Ϊͨ�Ŷ�ȡ
//#define COMM_START				14
//#define COMM_CONNECT			15
//#define COMM_STATE_OK			16


#define FAIL(status) return(status);

#define FAIL_WORD_REPEATED 1
#define FAIL_WORD_NOGCODE	 2


void InitDK(void);















//grbl




// Define Grbl status codes.
#define STATUS_OK 0
#define STATUS_EXPECTED_COMMAND_LETTER 1
#define STATUS_BAD_NUMBER_FORMAT 2
#define STATUS_INVALID_STATEMENT 3
#define STATUS_NEGATIVE_VALUE 4
#define STATUS_SETTING_DISABLED 5
#define STATUS_SETTING_STEP_PULSE_MIN 6
#define STATUS_SETTING_READ_FAIL 7
#define STATUS_IDLE_ERROR 8
#define STATUS_ALARM_LOCK 9
#define STATUS_SOFT_LIMIT_ERROR 10
#define STATUS_OVERFLOW 11
#define STATUS_MAX_STEP_RATE_EXCEEDED 12

#define STATUS_GCODE_UNSUPPORTED_COMMAND 20
#define STATUS_GCODE_MODAL_GROUP_VIOLATION 21
#define STATUS_GCODE_UNDEFINED_FEED_RATE 22
#define STATUS_GCODE_COMMAND_VALUE_NOT_INTEGER 23
#define STATUS_GCODE_AXIS_COMMAND_CONFLICT 24
#define STATUS_GCODE_WORD_REPEATED 25
#define STATUS_GCODE_NO_AXIS_WORDS 26
#define STATUS_GCODE_INVALID_LINE_NUMBER 27
#define STATUS_GCODE_VALUE_WORD_MISSING 28
#define STATUS_GCODE_UNSUPPORTED_COORD_SYS 29
#define STATUS_GCODE_G53_INVALID_MOTION_MODE 30
#define STATUS_GCODE_AXIS_WORDS_EXIST 31
#define STATUS_GCODE_NO_AXIS_WORDS_IN_PLANE 32
#define STATUS_GCODE_INVALID_TARGET 33
#define STATUS_GCODE_ARC_RADIUS_ERROR 34
#define STATUS_GCODE_NO_OFFSETS_IN_PLANE 35
#define STATUS_GCODE_UNUSED_WORDS 36
#define STATUS_GCODE_G43_DYNAMIC_AXIS_ERROR 37

// Define Grbl alarm codes.
#define ALARM_HARD_LIMIT_ERROR 1
#define ALARM_SOFT_LIMIT_ERROR 2
#define ALARM_ABORT_CYCLE 3
#define ALARM_PROBE_FAIL 4
#define ALARM_HOMING_FAIL 5














#endif 

