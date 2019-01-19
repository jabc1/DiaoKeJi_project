/*********************************************************************************
* 文件名		: menu.h
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 定义函数						  
* 用法			：
* 版本说明	：
**********************************************************************************/
#ifndef __PAGE_H
#define __PAGE_H
#include "stm32f10x.h"
#include "device_conf.h"
#include "gui_conf.h"

/* 	 
  "",	 
  "",
MENU--+-系统设置	----+--主轴移动速度	 					11	Page_SetSpeed
			|							+--TFT显示倍率	 						12	Page_SetTFTSize
			|							+---时间设置								13	Page_SetTime
			|							+--日期设置								14	Page_SetDate
			|							+--串口设置								15	Page_SetCommSet
			|							+--USB控制								16
			+-定位																	20  Page_DingWei
			+-SD卡脱机雕刻														30
			+-串口通信雕刻														40
			+-返回坐标原点														50
			+-主轴电机控制														60
*/
#define PAGE_MAIN					0
#define PAGE_FILE					30
#define PAGE_DINGWEI			20
#define PAGE_SPEED				11
#define PAGE_TFTSIZE			12
#define PAGE_SETTIME			13
#define PAGE_SETDATE			14
#define PAGE_SHOWCN 			1


extern u8 Page;
extern u32 GUIKJ[15];

typedef struct
{	
	GUI_FileListStruct *FL_T;
}Page_File_Struct;	
typedef struct
{	
	GUI_LableStruct *L_Date,*L_Time;
	GUI_MenuStruct *Menu_P;		
}Page_Main_Struct;	

typedef struct
{
	GUI_ButtonStruct *B_Z,*B_Y,*B_S,*B_X,*B_ZS,*B_YS, *B_ZX,*B_YX,*B_XY_0;
	GUI_ButtonStruct *B_Moto_On,*B_Moto_Off;			
	GUI_ButtonStruct *B_Z_Up,*B_Z_Dn,*B_Z_0,*B_Return;			
}Page_DingWei_Struct;	

typedef struct
{
	GUI_ButtonStruct *B_XY_U,*B_XY_D,*B_Z_U,*B_Z_D,*B_FAST_U,*B_FAST_D,*B_T_UP,*B_T_DN,*B_OK,*B_Return;		
	
}Page_Speed_Struct;	
typedef struct
{
	GUI_ButtonStruct *B_Y_U,*B_Y_D,*B_M_U,*B_M_D,*B_D_U,*B_D_D,*B_OK,*B_Return;		
//	Calendar_Struct Date_P;
}Page_Date_Struct;	
typedef struct
{
	GUI_ButtonStruct *B_H_U,*B_H_D,*B_M_U,*B_M_D,*B_S_U,*B_S_D,*B_OK,*B_Return;		
//	Calendar_Struct Time_P;
	
}Page_Time_Struct;	
typedef struct
{
	GUI_ButtonStruct *B_UP,*B_DN,*B_OK,*B_Return;		
}Page_TFTSize_Struct;	
typedef struct
{
	GUI_LEDStruct *LED_Alarm,*LED_State;	
	GUI_LampStruct *LM_Strat,*LM_Stop,*LM_Pause;
	GUI_ButtonStruct *B_Start,*B_Stop,*B_Pause,*B_Return,*B_Step,*B_Z_Dn,*B_Z_Up;		
}Page_ShowCN_Struct;	




void Page_DingWei(void);		//11
void Page_DingWei_Routine(void);	
	
void Page_SetSYS(void);			//12
void Page_SetTime(void);			//21
void Page_SetDate(void);			//22
void Page_SetCommSet(void);		//23
void Page_DK_File(void);			//30
void Page_DK_Comm(void);			//40

void Page_SetUSB(void);


void FileSelectShow(void);


void Page_ShowCN_Routine(void);
void PageReturn(void);



void Page_NRF_CX_SPEED_R(u8* Rx_Buf);

void Init_MainPage(void);
void InitMenu(void);

#endif 

