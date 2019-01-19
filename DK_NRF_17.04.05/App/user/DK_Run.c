#include "moto_4988.h"
#include "dk_conf.h"
#include "queue.h"
#include "os.h"
#include "motion_ctrl.h"
#include "NRFTask.h"
#include "NRF_Ctrl.h"
#include "DK_CONF.h"



#define  DW_SPEED1  	100
#define  DW_STEP1	  	5*STEP_TO_PUSH
#define  DW_SPEED2  	30
#define  DW_STEP2	  	50*STEP_TO_PUSH
#define  DW_SPEED3  	10
#define  DW_STEP3	  	150*STEP_TO_PUSH

#define  DW_SPEED4  	20
#define  DW_STEP4	  	80*STEP_TO_PUSH

#define  DW_STEP_T1	  10
#define  DW_STEP_T2	  15
#define  DW_STEP_T3	  30
#define  DW_STEP_T4	  35


	static u8 DW_Speed_XY = 0,DW_Speed_Z = 0;

void DW_Move_X(u8 Dir,u16 DW_Speed)
{
//		if((DK.Alarm_B_P->L[AXIS_X]&&(M_X_DIR == REVERSE))||(DK.Alarm_B_P->R[AXIS_X]&&(M_X_DIR ==POSITIVE))) return;  //限位		
	
	if(Dir == REVERSE)
	{
		if(DK.Alarm_B_P->L[AXIS_X]) return;  //限位				
		M_X_DIR = REVERSE;			//电机反转
		if(DW_Speed <=DW_STEP_T1)
		{
			Moto[AXIS_X].Target = Moto[AXIS_X].Current - DW_STEP1;	//电机需运行的脉冲数量
		}
		else if(DW_Speed <=DW_STEP_T2)
		{
			Moto[AXIS_X].Target = Moto[AXIS_X].Current - DW_STEP2;	//电机需运行的脉冲数量
		}
		else
		{
			Moto[AXIS_X].Target = Moto[AXIS_X].Current - DW_STEP3;	//电机需运行的脉冲数量
		}		
	}
	else
	{
		if(DK.Alarm_B_P->R[AXIS_X]) return;  //限位				
		M_X_DIR = POSITIVE;			//电机正转
		if(DW_Speed <=DW_STEP_T1)
		{
			Moto[AXIS_X].Target = Moto[AXIS_X].Current + DW_STEP1;	//电机需运行的脉冲数量
		}
		else if(DW_Speed <=DW_STEP_T2)
		{
			Moto[AXIS_X].Target = Moto[AXIS_X].Current + DW_STEP2;	//电机需运行的脉冲数量
		}
		else
		{
			Moto[AXIS_X].Target = Moto[AXIS_X].Current + DW_STEP3;	//电机需运行的脉冲数量
		}	
	}
	if(Moto[AXIS_X].Flag == 0)
	{
		Moto[AXIS_X].Flag = 1;		
	}
}
void DW_Move_Y(u8 Dir,u16 DW_Speed)
{
//	if((DK.Alarm_B_P->L[AXIS_Y]&&(M_Y_DIR == POSITIVE))||(DK.Alarm_B_P->R[AXIS_Y]&&(M_Y_DIR ==REVERSE))) return;  //限位	
	if(Dir == REVERSE)
	{
		if(DK.Alarm_B_P->L[AXIS_Y]) return;  //限位			
		M_Y_DIR = REVERSE;			//电机反转
		if(DW_Speed <=DW_STEP_T1)
		{
			Moto[AXIS_Y].Target = Moto[AXIS_Y].Current - DW_STEP1;	//电机需运行的脉冲数量
		}
		else if(DW_Speed <=DW_STEP_T2)
		{
			Moto[AXIS_Y].Target = Moto[AXIS_Y].Current - DW_STEP2;	//电机需运行的脉冲数量
		}
		else
		{
			Moto[AXIS_Y].Target = Moto[AXIS_Y].Current - DW_STEP3;	//电机需运行的脉冲数量
		}			
		
	}
	else
	{
		if(DK.Alarm_B_P->R[AXIS_Y]) return;  //限位				
		M_Y_DIR = POSITIVE;			//电机正转
		if(DW_Speed <=DW_STEP_T1)
		{
			Moto[AXIS_Y].Target = Moto[AXIS_Y].Current + DW_STEP1;	//电机需运行的脉冲数量
		}
		else if(DW_Speed <=DW_STEP_T2)
		{
			Moto[AXIS_Y].Target = Moto[AXIS_Y].Current + DW_STEP2;	//电机需运行的脉冲数量
		}
		else
		{
			Moto[AXIS_Y].Target = Moto[AXIS_Y].Current + DW_STEP3;	//电机需运行的脉冲数量
		}	
	}
	if(Moto[AXIS_Y].Flag == 0)
	{
		Moto[AXIS_Y].Flag = 1;		
	}
}

void DW_Move_Z(u8 Dir,u16 DW_Speed)
{
	if(Dir == REVERSE)
	{
		M_Z_DIR = REVERSE;			//电机反转
		if(DW_Speed <=DW_STEP_T3)
		{
			Moto[AXIS_Z].Target = Moto[AXIS_Z].Current - DW_STEP1;	//电机需运行的脉冲数量
		}
		else if(DW_Speed <=DW_STEP_T4)
		{
			Moto[AXIS_Z].Target = Moto[AXIS_Z].Current - DW_STEP2;	//电机需运行的脉冲数量
		}
		else
		{
			Moto[AXIS_Z].Target = Moto[AXIS_Z].Current - DW_STEP4;	//电机需运行的脉冲数量
		}		
	}
	else
	{
		M_Z_DIR = POSITIVE;			//电机正转
		if(DW_Speed <=DW_STEP_T3)
		{
			Moto[AXIS_Z].Target = Moto[AXIS_Z].Current + DW_STEP1;	//电机需运行的脉冲数量
		}
		else if(DW_Speed <=DW_STEP_T4)
		{
			Moto[AXIS_Z].Target = Moto[AXIS_Z].Current + DW_STEP2;	//电机需运行的脉冲数量
		}
		else
		{
			Moto[AXIS_Z].Target = Moto[AXIS_Z].Current + DW_STEP4;	//电机需运行的脉冲数量
		}	
	}
	if(Moto[AXIS_Z].Flag == 0)
	{
		Moto[AXIS_Z].Flag = 1;		
	}
}
void DW_Set_Speed(u8 DW_Speed)
{

		if((DW_Speed <=DW_STEP_T1)&&(DW_Speed >0))
		{
			if(DW_Speed_XY != 1)
			{
				DW_Speed_XY = 1;
				Set_Speed_XY(DW_SPEED1);
			}
		}
		else if(DW_Speed <=DW_STEP_T2)
		{
			if(DW_Speed_XY != 2)
			{
				DW_Speed_XY = 2;
				Set_Speed_XY(DW_SPEED2);
			}			
		}
		else
		{
			if(DW_Speed_XY != 3)
			{
				DW_Speed_XY = 3;
				Set_Speed_XY(DW_SPEED3);
			}			
		}	

		if((DW_Speed <=DW_STEP_T3)&&(DW_Speed >0))
		{
			if(DW_Speed_Z != 1)
			{
				DW_Speed_Z = 1;
				Set_Speed_Z(DW_SPEED1);
			}
		}
		else if(DW_Speed <=DW_STEP_T4)
		{
			if(DW_Speed_Z != 2)
			{
				DW_Speed_Z = 2;
				Set_Speed_Z(DW_SPEED2);
			}			
		}
		else
		{
			if(DW_Speed_Z != 3)
			{
				DW_Speed_Z = 3;
				Set_Speed_Z(DW_SPEED4);
			}			
		}	
		if(DW_Speed ==0)
		{
			if(DW_Speed_Z != 4)
			{
				DW_Speed_Z = 4;
				Set_Speed_Z(DK.SYS.Speed_Z);
				Set_Speed_XY(DK.SYS.Speed_XY);
			}
		}		
}

void DK_Ctrl_DW(u8* RX_Buf)
{
	NRF_DW_Struct *DW_P;
	DW_P = (NRF_DW_Struct *)RX_Buf;
	DK.Ctrl = DW_P->Ctrl;
		switch(DW_P->DW_Z)
		{
			case DW_NUM_NULL:    							
				Stop_Moto(AXIS_Z);
				DW_Speed_Z = 0;		
				break;
			case DW_NUM_Z_UP:    							
				DW_Move_Z(POSITIVE,DW_P->DW_Speed);			
				break;	
			case DW_NUM_Z_DN:    							
				DW_Move_Z(REVERSE,DW_P->DW_Speed);			
				break;	
			case DW_NUM_Z_0:    						
				Moto[AXIS_Z].Current = 0;			
				break;	
			default:                                
				break;
		}		
		switch(DW_P->DW_XY)
		{
			case DW_NUM_NULL:    							
				Stop_Moto(AXIS_X);
				Stop_Moto(AXIS_Y);
				DW_Speed_XY = 0;
				break;
			case DW_NUM_ZS:    							
				DW_Move_X(REVERSE,DW_P->DW_Speed);
				DW_Move_Y(POSITIVE,DW_P->DW_Speed);			
				break;	
			case DW_NUM_S:    							
				Stop_Moto(AXIS_X);
				DW_Move_Y(POSITIVE,DW_P->DW_Speed);			
				break;	
			case DW_NUM_YS:    							
				DW_Move_X(POSITIVE,DW_P->DW_Speed);
				DW_Move_Y(POSITIVE,DW_P->DW_Speed);	
				break;	
			case DW_NUM_Z:    										
				DW_Move_X(REVERSE,DW_P->DW_Speed);		
				Stop_Moto(AXIS_Y);
				break;	
			case DW_NUM_Y:    							
				DW_Move_X(POSITIVE,DW_P->DW_Speed);	
				Stop_Moto(AXIS_Y);
				break;	
			case DW_NUM_ZX:    											
				DW_Move_X(REVERSE,DW_P->DW_Speed);			
				DW_Move_Y(REVERSE,DW_P->DW_Speed);		
				break;	
			case DW_NUM_X:    							
				Stop_Moto(AXIS_X);		
				DW_Move_Y(REVERSE,DW_P->DW_Speed);		
				break;	
			case DW_NUM_YX:    							
				DW_Move_X(POSITIVE,DW_P->DW_Speed);	
				DW_Move_Y(REVERSE,DW_P->DW_Speed);		
				break;	
			case DW_NUM_XY_0:    						
				Moto[AXIS_X].Current = 0;
				Moto[AXIS_Y].Current = 0;			
				break;	
			default:                                
				break;
		}	
		DW_Set_Speed(DW_P->DW_Speed);
		if((DW_P->Spindle_On == 1) && (DK.State_B_P->Spindle_On== 0))
		{
			Run_MainMoto();
		}
		else if((DW_P->Spindle_On == 0) && (DK.State_B_P->Spindle_On== 1))
		{
			Stop_MainMoto();
			DK.State_B_P->Spindle_On = 0;
		}
}



/********************************************
函数：DK_File_Start
功能：雕刻机脱机SD卡文件开始雕刻
参数：无
返回：无
********************************************/
void DK_Ctrl_Run(void)
{
	if(GCodeQueue.Entries!=0)
	{		
			Interpolation_Process(GCodeQueue.Out);
			SubGCodeQueueEntries();				
	}	
}


void NRF_GCode(u8 *RX_Buf)
{
	NRF_GCode_Struct *NRF_GCode_P;
	NRF_GCode_P = (NRF_GCode_Struct *)RX_Buf;
	//.... 数据保存
	GCodeQueue.In->Action = NRF_GCode_P->Action;	
	GCodeQueue.In->ValueWords = 	NRF_GCode_P->ValueWords;
	GCodeQueue.In->X = 	NRF_GCode_P->X;
	GCodeQueue.In->Y = 	NRF_GCode_P->Y;	
	GCodeQueue.In->I = 	NRF_GCode_P->I;		
	GCodeQueue.In->J = 	NRF_GCode_P->J;			
	GCodeQueue.In->Z = 	NRF_GCode_P->Z;	
	GCodeQueue.In->N = 	NRF_GCode_P->N;		
	AddGCodeQueueEntries();	
}

void DK_Reset(void)
{
	Stop_Moto(AXIS_X);
	Stop_Moto(AXIS_Y);	
	Stop_Moto(AXIS_Z);	
	Stop_MainMoto();	
	DK.Ctrl = CTRL_STOP;
}

/********************************************
函数：DK_NRF_KZ
功能：雕刻机运行命令处理
参数：无
返回：无
********************************************/
void DK_NRF_KZ(u8* RX_Buf,u8 *TX_Buf)
{
	switch (RX_Buf[0]&0x7F)
  {
  	case NRF_KZ_GCODE:
			NRF_GCode(RX_Buf);
			TX_Buf[0] = NRF_KZ_GCODE;	
  		break;
  	case NRF_KZ_DW:
			DK_Ctrl_DW(RX_Buf);
			TX_Buf[0] = NRF_KZ_DW;	
  		break;
  	case NRF_KZ_RESET:
			DK_Reset();
			TX_Buf[0] = NRF_KZ_RESET;	
  		break;		
  	case NRF_KZ_MAINMOTO_OFF:
			Stop_MainMoto();
			TX_Buf[0] = NRF_KZ_MAINMOTO_OFF;	
  		break;			
  	case NRF_KZ_GOTOZERO:
			DK.Ctrl = CTRL_GOTOZERO;
			TX_Buf[0] = NRF_KZ_GOTOZERO;	
  		break;	
  	case NRF_KZ_MOTO_OFF:
			MOTO_OFF;		//关闭步进电机驱动器
			InitializeGCodeQueue();				//初始化GCODE队列
			TX_Buf[0] = NRF_KZ_MOTO_OFF;	
  		break;
  	case NRF_KZ_MOTO_ON:
			MOTO_ON;		//使能步进电机驱动器
			InitializeGCodeQueue();				//初始化GCODE队列
			TX_Buf[0] = NRF_KZ_MOTO_ON;	
  		break;

		
  	default:
  		break;
  }
	
	
}


