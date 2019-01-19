/*********************************************************************************
*�ļ��� 		: motion_ctrl.c
*����			��ZZ
*�汾  		: V3.0.0
*����   	: 2017.01.16
*�汾˵��	:�˶�����,��G������н��������Ե��������Ӧ����
***********************************************************************************/
#include "OS.h"
#include "app.h"
#include "motion_ctrl.h"

#include "stdlib.h"
#include "moto_4988.h"
#include "device_conf.h"
#include "plan.h"



/*******************************************************************************************************/
//�ṹ��ʵ�廯
//POSITION Position;



/*******************************************************************************************************/

u8 Interpolation_Process(GCodeStruct* GCode_One)//main�е��õ��߳�
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
	if(GetBit(GCode_One->ValueWords,WORD_Z)) //�����Ҫ�ƶ�Z��
	{
		Moto_Move(AXIS_Z,GCode_One->Z* STEP_TO_PUSH -Moto[AXIS_Z].Current);
		while(Moto[AXIS_Z].Flag)   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
		{
			OS.DelayTimeTick(1);
		}			
	}
}


//���ٶ�λ
u8 Fast_Seek(GCodeStruct* GCode_One)
{
	Moto_Wait_XY();   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����	

	if(GetBit(GCode_One->ValueWords,WORD_Z)) //�����Ҫ�ƶ�Z��
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
	Moto_Wait_XY();  //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����	
	
	Set_Speed_XY(DK.SYS.Speed_XY);
	return 0;//������������ֵ
}





//ֱ�߲岹
u8 LinearInterpolation(GCodeStruct* GCode_One)
{
	LINEAR Linear;
	u32 PlanMessage;
	
	WaitPlanQueueReady();
	MC_Move_Z(GCode_One);
/*��ȡĿ�������*/
	Linear.X_Current = Moto[AXIS_X].Current/STEP_TO_PUSH;
	Linear.Y_Current = Moto[AXIS_Y].Current/STEP_TO_PUSH;	
	
	if(GetBit(GCode_One->ValueWords,WORD_X)){Linear.X_Target = GCode_One->X;}
	else{Linear.X_Target = Linear.X_Current;}
	if(GetBit(GCode_One->ValueWords,WORD_Y)){Linear.Y_Target = GCode_One->Y;}
	else{Linear.Y_Target = Linear.Y_Current;}	
/*������Ҫ���е���������*/
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L1;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1�Ժ����ʵ�ʾ�����		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);
	}			
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L2;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1�Ժ����ʵ�ʾ�����		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);		
	}		
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L3;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1�Ժ����ʵ�ʾ�����				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
	}	
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L4;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1�Ժ����ʵ�ʾ�����				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
	}	
	Linear.zN=(Linear.Xe+Linear.Ye);//�ܲ��� �����յ��б�	
	if((Linear.Xe == 0)&&(Linear.Ye == 0))
	{
		PlanQueue.State = 0;
		return 0;
	}
		if(Linear.Xe == 0)//ֱ��Ϊ��ֱ
		{

		Moto_Move(AXIS_Y,Linear.Y_Target * STEP_TO_PUSH -Moto[AXIS_Y].Current);
		Moto_Wait_XY();
		return 0;			
		}
		if(Linear.Ye == 0)//ֱ��Ϊˮƽ
		{			
		Moto_Move(AXIS_X,Linear.X_Target * STEP_TO_PUSH -Moto[AXIS_X].Current);
		Moto_Wait_XY();
		return 0;			
		}
		Linear.F=0;//ÿ�ν��в岹ʱ��������
		/*�򿪼���*/
		//Laser_ON;

		PlanMessage = ((u32)UseMessage_PlanRun<<24);			
		OS.PostMessageQueue(MotoTaskPriority, (void *)(PlanMessage));		//

		while((Linear.zN--)&&(!DK.Alarm))
		{
			//ƫ��Ƚ�
			if(Linear.F >= 0)
			{
				switch (Linear.Quadrant)
				{
					case L1:{PlanQueue.DirX=POSITIVE;break;}	//���÷���Ϊ+ �� -
					case L2:{PlanQueue.DirX=REVERSE;break;}
					case L3:{PlanQueue.DirX=REVERSE;break;}
					case L4:{PlanQueue.DirX=POSITIVE;break;}
					default: ;//Doing Nothing					
				}
//				Linear.DIR=CB_X;//���ò岹��������ΪX
					AddPlanQueueEntries(PLANMOTOX);
					Linear.F -= Linear.Ye;
			}
			else
			{
				switch (Linear.Quadrant)
				{
					case L1:{PlanQueue.DirY=POSITIVE;break;}	//���÷���Ϊ+ �� -
					case L2:{PlanQueue.DirY=POSITIVE;break;}
					case L3:{PlanQueue.DirY=REVERSE;break;}
					case L4:{PlanQueue.DirY=REVERSE;break;}
					default: ;//Doing Nothing					
				}
//				Linear.DIR=CB_Y;//���ò岹��������ΪY	
					AddPlanQueueEntries(PLANMOTOY);				
					Linear.F += Linear.Xe;	
			}
		}
		/*�رռ���*/
		//Laser_OFF;
		PlanQueue.State = PLANSTATE_END;
	while(PlanQueue.State != PLANSTATE_STOP)   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
	{
		OS.DelayTimeTick(1);
	}
		return 0;//�岹������������ֵ

}





//ֱ�߲岹
u8 Linear_Interpolation(GCodeStruct* GCode_One)
{
	LINEAR Linear;
	u32 PlanMessage;
	
	WaitPlanQueueReady();
	MC_Move_Z(GCode_One);
/*��ȡĿ�������*/
	Linear.X_Current = Moto[AXIS_X].Current/STEP_TO_PUSH;
	Linear.Y_Current = Moto[AXIS_Y].Current/STEP_TO_PUSH;	
	
	if(GetBit(GCode_One->ValueWords,WORD_X)){Linear.X_Target = GCode_One->X;}
	else{Linear.X_Target = Linear.X_Current;}
	if(GetBit(GCode_One->ValueWords,WORD_Y)){Linear.Y_Target = GCode_One->Y;}
	else{Linear.Y_Target = Linear.Y_Current;}	
/*������Ҫ���е���������*/
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L1;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1�Ժ����ʵ�ʾ�����		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);
		PlanQueue.DirX=POSITIVE;
		PlanQueue.DirY=POSITIVE;
	}			
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target >= Linear.Y_Current))
	{
		Linear.Quadrant = L2;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1�Ժ����ʵ�ʾ�����		
		Linear.Ye = (Linear.Y_Target - Linear.Y_Current);		
		PlanQueue.DirX=REVERSE;
		PlanQueue.DirY=POSITIVE;
	}		
	if((Linear.X_Target < Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L3;	
		Linear.Xe = (Linear.X_Current - Linear.X_Target);		//*1�Ժ����ʵ�ʾ�����				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
		PlanQueue.DirX=REVERSE;
		PlanQueue.DirY=REVERSE;
	}	
	if((Linear.X_Target >= Linear.X_Current)&&(Linear.Y_Target < Linear.Y_Current))
	{
		Linear.Quadrant = L4;	
		Linear.Xe = (Linear.X_Target - Linear.X_Current);		//*1�Ժ����ʵ�ʾ�����				
		Linear.Ye = (Linear.Y_Current - Linear.Y_Target);
		PlanQueue.DirX=POSITIVE;
		PlanQueue.DirY=REVERSE;
	}	
	Linear.zN=(Linear.Xe+Linear.Ye);//�ܲ��� �����յ��б�	
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
			//ƫ��Ƚ�
			Linear.F = (Linear.Y_Current*Linear.Xe-Linear.X_Current*Linear.Ye)*2+Linear.Xe-Linear.Ye;
			if(Linear.F > 0)
			{//���ò岹��������ΪX
					AddPlanQueueEntries(PLANMOTOX);				
					Linear.X_Current++;
			}
			else if(Linear.F < 0)
			{//���ò岹��������ΪY	
					AddPlanQueueEntries(PLANMOTOY);				
					Linear.Y_Current++;	
			}
			else //Linear.F = 0,�Ǳߵľ���Զ���Ǳ߲岹
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
	while(PlanQueue.State != PLANSTATE_STOP)   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
	{
		OS.DelayTimeTick(1);
	}
		return 0;//�岹������������ֵ

}



/*******************************************************************************************************/
//�����б�
/*
�˴�Ϊ�����������б𣬲�ͬ��ֱ�ߵ�ֻ��Ҫ�����б�
�ڴ�֮ǰ����ֱ��Բ���岹���� L˳Բ�岹 R ��Բ�岹
����Circula.type���и�ֵ
*/
static u8 Circula_QuadrantJudge(u8 type,CIRCULA Circula)
{
	switch (type)
	{
		case SR:
		{
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return SR2;//��
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return SR3;//��
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return SR4;
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return SR1;//��
			break;
		}
		case NR:
		{
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return NR4;
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target>=Circula.Y_Current)) return NR1;//��
			if((Circula.X_Target<=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return NR2;//��
			if((Circula.X_Target>=Circula.X_Current)&&(Circula.Y_Target<=Circula.Y_Current)) return NR3;//��
			break;
		}
		default: ;//Doing Nothing
	}
	return 0;// not supposed to reach!
}

//Բ���岹 
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
	//λ��ת��Ϊ����(��������һ�г���֮ǰ�����������б����)
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
			
	Circula.Quadrant=Circula_QuadrantJudge(Circula.type,Circula);//Բ�������б�
	
	Circula.zN=(Circula.Xe+Circula.Ye);//�ܲ��� �����յ��б�
	Circula.F=0;//ÿ�ν��в岹ʱ��������
	Xi=(abs(GCode_One->I));
	Yi=(abs(GCode_One->J));
	/*�򿪼���*/
	//Laser_ON;

	PlanMessage = ((u32)UseMessage_PlanRun<<24);			
	OS.PostMessageQueue(MotoTaskPriority, (void *)(PlanMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
	while((Circula.zN--)&&(!DK.Alarm))
	{
		//ƫ��Ƚ�
		if(Circula.F >= 0)
		{
			switch (Circula.Quadrant)
			{
				case SR1:
					PlanQueue.DirY=REVERSE;
					Circula.DIR=CB_Y;
					Circula.F=Circula.F-2*Yi;
					Yi--;
					break;	//���÷���Ϊ+ �� -
				case NR1:{PlanQueue.DirX=REVERSE;Circula.DIR=CB_X;Circula.F=Circula.F-2*Xi;Xi--;break;}//�������ȷ				
				case SR2:{PlanQueue.DirX=POSITIVE;Circula.DIR=CB_X;Circula.F=Circula.F-2*Xi;Xi--;break;}
				case NR2:{PlanQueue.DirY=REVERSE;Circula.DIR=CB_Y;Circula.F=Circula.F-2*Yi;Yi--;break;}//�������ȷ				
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
				case SR1:{PlanQueue.DirX=POSITIVE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}	//���÷���Ϊ+ �� -
				case NR1:{PlanQueue.DirY=POSITIVE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}//�������ȷ
				
				case SR2:{PlanQueue.DirY=POSITIVE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}
				case NR2:{PlanQueue.DirX=REVERSE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}//�������ȷ
				
				case SR3:{PlanQueue.DirX=REVERSE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}
				case NR3:{PlanQueue.DirY=REVERSE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}
				
				case SR4:{PlanQueue.DirY=REVERSE;Circula.DIR=CB_Y;Circula.F=Circula.F+2*Yi;Yi++;break;}
				case NR4:{PlanQueue.DirX=POSITIVE;Circula.DIR=CB_X;Circula.F=Circula.F+2*Xi;Xi++;break;}
				default: ;//Doing Nothing					
			}
	
		}
		//���в岹
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
	/*�رռ���*/
	//Laser_OFF;
	PlanQueue.State = PLANSTATE_END;
	while(PlanQueue.State != PLANSTATE_STOP)   //�������BUSY�źţ�����������־Ϊ0��ʱ����ܽ�����һ�ε�����
	{
		OS.DelayTimeTick(1);
	}
	return 0;//�岹������������ֵ
}
























