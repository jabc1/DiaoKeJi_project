/*********************************************************************************
*文件名 		: motion_ctrl.c
*作者			：ZZ
*版本  		: V3.0.0
*日期   	: 2017.01.16
*版本说明	:运动控制,对G代码进行解析，并对电机做出相应控制
***********************************************************************************/
#include "OS.h"
#include "app.h"
#include "motion_ctrl.h"

#include "stdlib.h"
#include "moto_4988.h"
#include "device_conf.h"
#include "plan.h"



/*******************************************************************************************************/
//结构体实体化
//POSITION Position;



/*******************************************************************************************************/

u8 Interpolation_Process(GCodeStruct* GCode_One)//main中调用的线程
{
	DK.N = GCode_One->N;
	switch(GCode_One->Action)
	{
		case GC_SEEK_G0: 
			Fast_Seek(GCode_One);	
			break;
		case GC_LINEAR_G1: 
			Linear_Interpolation(GCode_One);					
			break;
		case GC_CW_ARC:
			Circula_Interpolation(GCode_One);
			break;
		case GC_CCW_ARC:
			Circula_Interpolation(GCode_One);			
			break;
		case GC_M03:
			Run_MainMoto();
			break;					
		case GC_M05:
			Stop_MainMoto();
			break;				
		
		default:;//
	}
	return 0;
}

void MC_Move_Z(GCodeStruct* GCode_One)
{
	if(GetBit(GCode_One->ValueWords,WORD_Z)) //如果需要移动Z轴
	{
		Moto_Move(AXIS_Z,GCode_One->Z* STEP_TO_PUSH -Moto[AXIS_Z].Current);
		while(Moto[AXIS_Z].Flag)   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行
		{
			OS.DelayTimeTick(1);
		}			
	}
}


//快速定位
u8 Fast_Seek(GCodeStruct* GCode_One)
{
	Moto_Wait_XY();   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行	

	if(GetBit(GCode_One->ValueWords,WORD_Z)) //如果需要移动Z轴
	MC_Move_Z(GCode_One);
	Set_Speed_XY(DK.SYS.Speed_Fast);
	if(GetBit(GCode_One->ValueWords,WORD_X))
	{
		Moto_Move(AXIS_X,GCode_One->X* STEP_TO_PUSH -Moto[AXIS_X].Current);
	}
	if(GetBit(GCode_One->ValueWords,WORD_Y))
	{		
		Moto_Move(AXIS_Y,GCode_One->Y* STEP_TO_PUSH -Moto[AXIS_Y].Current);
	}		
	Moto_Wait_XY();  //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行	
	
	Set_Speed_XY(DK.SYS.Speed_XY);
	return 0;//正常结束返回值
}





//直线插补
u8 LinearInterpolation(GCodeStruct* GCode_One)
{
	LINEAR Linear;
	u32 PlanMessage;
	
	WaitPlanQueueReady();
	MC_Move_Z(GCode_One);
/*获取目标的坐标*/
	Linear.X_Current = Moto[AXIS_X].Current/STEP_TO_PUSH;
	Linear.Y_Current = Moto[AXIS_Y].Current/STEP_TO_PUSH;	
	
	if(GetBit(GCode_One->ValueWords,WORD_X)){Linear.X_Target = GCode_One->X;}
	else{Linear.X_Target = Linear.X_Current;}
	if(GetBit(GCode_One->ValueWords,WORD_Y)){Linear.Y_Target = GCode_One->Y;}
	else{Linear.Y_Target = Linear.Y_Current;}	
/*计算需要运行的脉冲数量*/
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L1;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1以后计算实际距离用		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);
	}			
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L2;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1以后计算实际距离用		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);		
	}		
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L3;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1以后计算实际距离用				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
	}	
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L4;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1以后计算实际距离用				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
	}	
	Linear.zN=(Linear.Xe+Linear.Ye);//总步数 用于终点判别	
	if((Linear.Xe == 0)&&(Linear.Ye == 0))
	{
		PlanQueue.State = 0;
		return 0;
	}
		if(Linear.Xe == 0)//直线为垂直
		{

		Moto_Move(AXIS_Y,Linear.Y_Target * STEP_TO_PUSH -Moto[AXIS_Y].Current);
		Moto_Wait_XY();
		return 0;			
		}
		if(Linear.Ye == 0)//直线为水平
		{			
		Moto_Move(AXIS_X,Linear.X_Target * STEP_TO_PUSH -Moto[AXIS_X].Current);
		Moto_Wait_XY();
		return 0;			
		}
		Linear.F=0;//每次进行插补时必须清零
		/*打开激光*/
		//Laser_ON;

		PlanMessage = ((u32)UseMessage_PlanRun<<24);			
		OS.PostMessageQueue(MotoTaskPriority, (void *)(PlanMessage));		//

		while((Linear.zN--)&&(!DK.Alarm))
		{
			//偏差比较
			if(Linear.F >= 0)
			{
				switch (Linear.Quadrant)
				{
					case L1:{PlanQueue.DirX=POSITIVE;break;}	//设置方向为+ 或 -
					case L2:{PlanQueue.DirX=REVERSE;break;}
					case L3:{PlanQueue.DirX=REVERSE;break;}
					case L4:{PlanQueue.DirX=POSITIVE;break;}
					default: ;//Doing Nothing					
				}
//				Linear.DIR=CB_X;//设置插补进给方向为X
					AddPlanQueueEntries(PLANMOTOX);
					Linear.F -= Linear.Ye;
			}
			else
			{
				switch (Linear.Quadrant)
				{
					case L1:{PlanQueue.DirY=POSITIVE;break;}	//设置方向为+ 或 -
					case L2:{PlanQueue.DirY=POSITIVE;break;}
					case L3:{PlanQueue.DirY=REVERSE;break;}
					case L4:{PlanQueue.DirY=REVERSE;break;}
					default: ;//Doing Nothing					
				}
//				Linear.DIR=CB_Y;//设置插补进给方向为Y	
					AddPlanQueueEntries(PLANMOTOY);				
					Linear.F += Linear.Xe;	
			}
		}
		/*关闭激光*/
		//Laser_OFF;
		PlanQueue.State = PLANSTATE_END;
	while(PlanQueue.State != PLANSTATE_STOP)   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行
	{
		OS.DelayTimeTick(1);
	}
		return 0;//插补正常结束返回值

}





//直线插补
u8 Linear_Interpolation(GCodeStruct* GCode_One)
{
	LINEAR Linear;
	u32 PlanMessage;
	
	WaitPlanQueueReady();
	MC_Move_Z(GCode_One);
/*获取目标的坐标*/
	Linear.X_Current = Moto[AXIS_X].Current/STEP_TO_PUSH;
	Linear.Y_Current = Moto[AXIS_Y].Current/STEP_TO_PUSH;	
	
	if(GetBit(GCode_One->ValueWords,WORD_X)){Linear.X_Target = GCode_One->X;}
	else{Linear.X_Target = Linear.X_Current;}
	if(GetBit(GCode_One->ValueWords,WORD_Y)){Linear.Y_Target = GCode_One->Y;}
	else{Linear.Y_Target = Linear.Y_Current;}	
/*计算需要运行的脉冲数量*/
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L1;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1以后计算实际距离用		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);
		PlanQueue.DirX=POSITIVE;
		PlanQueue.DirY=POSITIVE;
	}			
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L2;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1以后计算实际距离用		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);		
		PlanQueue.DirX=REVERSE;
		PlanQueue.DirY=POSITIVE;
	}		
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L3;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1以后计算实际距离用				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
		PlanQueue.DirX=REVERSE;
		PlanQueue.DirY=REVERSE;
	}	
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L4;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1以后计算实际距离用				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
		PlanQueue.DirX=POSITIVE;
		PlanQueue.DirY=REVERSE;
	}	
	Linear.zN=(Linear.Xe+Linear.Ye);//总步数 用于终点判别	
	if((Linear.Xe == 0)&&(Linear.Ye == 0))
	{
		PlanQueue.State = 0;
		return 0;
	}
	Linear.X_Current = 0;
	Linear.Y_Current = 0;	
		
		PlanMessage = ((u32)UseMessage_PlanRun<<24);			
		OS.PostMessageQueue(MotoTaskPriority, (void *)(PlanMessage));		//

		while((Linear.zN--)&&(!DK.Alarm))
		{
			//偏差比较
			Linear.F = (Linear.Y_Current*Linear.Xe-Linear.X_Current*Linear.Ye)*2+Linear.Xe-Linear.Ye;
			if(Linear.F > 0)
			{//设置插补进给方向为X
					AddPlanQueueEntries(PLANMOTOX);				
					Linear.X_Current++;
			}
			else if(Linear.F < 0)
			{//设置插补进给方向为Y	
					AddPlanQueueEntries(PLANMOTOY);				
					Linear.Y_Current++;	
			}
			else //Linear.F = 0,那边的距离远就那边插补
			{
				if(Linear.Xe>Linear.Ye)
				{
					AddPlanQueueEntries(PLANMOTOX);				
					Linear.X_Current++;					
				}
				else
				{					
					AddPlanQueueEntries(PLANMOTOY);				
					Linear.Y_Current++;	
				}
			}
		}
		PlanQueue.State = PLANSTATE_END;
	while(PlanQueue.State != PLANSTATE_STOP)   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行
	{
		OS.DelayTimeTick(1);
	}
		return 0;//插补正常结束返回值

}



/*******************************************************************************************************/
//线型判别
/*
此处为真正的象限判别，不同于直线的只需要线型判别
在此之前必须分辨出圆弧插补类型 L顺圆插补 R 逆圆插补
并对Circula.type进行赋值
*/
static u8 Circula_QuadrantJudge(u8 type,CIRCULA Circula)
{
	switch (type)
	{
		case SR:
		{
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return SR2;//对
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return SR3;//对
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return SR4;
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return SR1;//对
			break;
		}
		case NR:
		{
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return NR4;
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return NR1;//对
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return NR2;//对
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return NR3;//对
			break;
		}
		default: ;//Doing Nothing
	}
	return 0;// not supposed to reach!
}

//圆弧插补 
u8 Circula_Interpolation(GCodeStruct* GCode_One)
{
	CIRCULA Circula;
	u32 PlanMessage;
	u32 Xi=0,Yi=0;
	WaitPlanQueueReady();
	MC_Move_Z(GCode_One);

	if(GCode_One->Action == GC_CW_ARC)
	{
	Circula.type = SR;
	}
	else
	{
	Circula.type = NR;		
	}
	Circula.X_Target = GCode_One->X  + 0;
	Circula.Y_Target = GCode_One->Y  + 0;		
	Circula.X_Current = Moto[AXIS_X].Current/STEP_TO_PUSH;
	Circula.Y_Current = Moto[AXIS_Y].Current/STEP_TO_PUSH;		
	//位置转化为步数(必须在下一行程序之前，否则象限判别出错！)
	if(Circula.X_Target > Circula.X_Current)
	{
		Circula.Xe=(u16)(Circula.X_Target - Circula.X_Current);		
	}
	else
	{
		Circula.Xe=(u16)(Circula.X_Current - Circula.X_Target);				
	}
	if(Circula.Y_Target > Circula.Y_Current)
	{
		Circula.Ye=(u16)(Circula.Y_Target - Circula.Y_Current);		
	}
	else
	{
		Circula.Ye=(u16)(Circula.Y_Current - Circula.Y_Target);				
	}
			
	Circula.Quadrant=Circula_QuadrantJudge(Circula.type,Circula);//圆弧象限判别
	
	Circula.zN=(Circula.Xe+Circula.Ye);//总步数 用于终点判别
	Circula.F=0;//每次进行插补时必须清零
	Xi=(abs(GCode_One->I));
	Yi=(abs(GCode_One->J));
	/*打开激光*/
	//Laser_ON;

	PlanMessage = ((u32)UseMessage_PlanRun<<24);			
	OS.PostMessageQueue(MotoTaskPriority, (void *)(PlanMessage));		//发送长时间按下功能键的键值
	while((Circula.zN--)&&(!DK.Alarm))
	{
		//偏差比较
		if(Circula.F >= 0)
		{
			switch (Circula.Quadrant)
			{
				case SR1:
					PlanQueue.DirY=REVERSE;
					Circula.DIR=CB_Y;
					Circula.F=Circula.F-2*Yi;
					Yi--;
					break;	//设置方向为+ 或 -
				case NR1:{PlanQueue.DirX=REVERSE;Circula.DIR=CB_X;Circula.F=Circula.F-2*Xi;Xi--;break;}//经检测正确				
				case SR2:{PlanQueue.DirX=POSITIVE;Circula.DIR=CB_X;Circula.F=Circula.F-2*Xi;Xi--;break;}
				case NR2:{PlanQueue.DirY=REVERSE;Circula.DIR=CB_Y;Circula.F=Circula.F-2*Yi;Yi--;break;}//经检测正确				
				case SR3:{PlanQueue.DirY=POSITIVE;Circula.DIR=CB_Y;Circula.F=Circula.F-2*Yi;Yi--;break;}
				case NR3:{PlanQueue.DirX=POSITIVE;Circula.DIR=CB_X;Circula.F=Circula.F-2*Xi;Xi--;break;}				
				case SR4:{PlanQueue.DirX=REVERSE;Circula.DIR=CB_X;Circula.F=Circula.F-2*Xi;Xi--;break;}
				case NR4:{PlanQueue.DirY=POSITIVE;Circula.DIR=CB_Y;Circula.F=Circula.F-2*Yi;Yi--;break;}
				default: ;//Doing Nothing					
			}

		}
		else
		{
			switch (Circula.Quadrant)
			{
				case SR1:{PlanQueue.DirX=POSITIVE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}	//设置方向为+ 或 -
				case NR1:{PlanQueue.DirY=POSITIVE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}//经检测正确
				
				case SR2:{PlanQueue.DirY=POSITIVE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}
				case NR2:{PlanQueue.DirX=REVERSE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}//经检测正确
				
				case SR3:{PlanQueue.DirX=REVERSE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}
				case NR3:{PlanQueue.DirY=REVERSE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}
				
				case SR4:{PlanQueue.DirY=REVERSE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}
				case NR4:{PlanQueue.DirX=POSITIVE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}
				default: ;//Doing Nothing					
			}
	
		}
		//进行插补
		switch (Circula.DIR)
		{
			case CB_X:
//				Move_X(1);
				AddPlanQueueEntries(PLANMOTOX);
				break;
			case CB_Y:
//				Move_Y(1);
				AddPlanQueueEntries(PLANMOTOY);			
			
				break;
			default: ;//Doing Nothing
		}
		
	}
	/*关闭激光*/
	//Laser_OFF;
	PlanQueue.State = PLANSTATE_END;
	while(PlanQueue.State != PLANSTATE_STOP)   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行
	{
		OS.DelayTimeTick(1);
	}
	return 0;//插补正常结束返回值
}
























