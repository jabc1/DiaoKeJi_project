/*  PLAN������ͼ
��ͨ�����岹�������ÿ����Ҫ�ķ����������
Ȼ��PLAN���ݲ岹��������ݽ��мƻ������
����X���Y�����ͬ�����������Ҫ�ȴ�һ���������ɺ����ߵڶ�����

                                   ��ʼ
                                    ��
																��һ��=X 
                      +-------------+-------------------+
											��                                ��
                     ��                                 ��
										 ��						+---------��---------------��-------------��----------------+ 
										 ��					 ��                     ��                                   ��
		 +--��---��---��--+				   �� 		+--��---��---��--+	          +--��---��---��----------------------------------------------------+	
		��							 ��		 	     �� 	 ��								��				 ��			                   ��                                  		 	��
		��							  A		 	     �� 	 ��								 B				 ��					   	           AB								      	  		    		 	BA
		��						   �� 	       �� 	 ��	              �� 	       ��                        ��                      	        				�� 
		��							Next=X 	     ��    ��              Next=Y 	   ��					            Next=X 				            	         			Next=Y 
		��					 +----+----+     ��    ��           +----+----+    ��              +----------+----------+              		 +----------+----------+                                                      
 �ȴ�X������		  ��        ��	   �� �ȴ�Y������     ��        ��    ��		          ��                     ��          	     ��                    ��
 С��64��			  ��        ��  	 ��  С��64��		   ��        ��    ��             ��                    ��            			 ��        						��                                                                     
		��			  AA(���)     AB-����+   	 ��         BB(���)    BA-��-+  		      ABA(���)            ABB(���)		       	BAB(���)      				BAA(���)	
		��					��							 ��		 ��          ��							 ��	            ��		                ��				         ��		                ��
		��				Next=X			       ��    ��         Next=Y           ��           Next=X               Next=X        	      Next=X                 Next=X
		��		  +----+----+    �ȴ�X������	 �� 		 +----+----+       �ȴ�Y������  +----+----+           +----+----+      	 +----+----+            +----+----+                                  
		��		  ��        ��		С��64�� 	 ��      ��        ��        С��64��   ��        ��          ��        ��        ��        ��           ��          ��
		��	 		��        ��       	 ��    ��  		 ��        ��     	 ��    ABAA(���)  ABAB(���) ABBB(���) ֻ���ABB   BABB(���) BABA(���)  BAAA(���) ֻ���BAA                                     
		��	 	AAA(���)  ֻ���AA  	 ��		 ��		BBB(���)  ֻ���BB 		 ��				 ��			   ��					 ��				��		  	 ��			   ��				 	 ��					��																			
		��			��				��				 ��		 ��			 ��				��				 ��			   ��        ��          ��        ��	     	 ��        ��        	 ��        	��
		��			��				��		     ��     +---��--+----��--+--------------------------------------��--+----��--+	---------+   		   ��        	 ��        	��
		��			��				��		     ��       	                       ��        ��        ��				                                 ��        	 ��        	��
		+---��--+----��----+----------------------��--------------��--------��----+---------------------------------------------------------------+-----------+  	 									       																																	
				                         �� 															 ��									 ��				                                 ��																										
																  +------------------��-----------------------��--------+				                                 ��																																				
																																	 ��								                                             ��
																																	  +------------------��-----------------------��----------------+								




*/
#include "plan.h"
#include "os.h"
#include "moto_4988.h"
#include "dk_conf.h"
#include "limit.h"


PlanStruct PlanQueue;

/********************************************
������AddPlanQueueEntries
���ܣ�����1�����ݵ��б�
��������
���أ���
********************************************/
void AddPlanQueueEntries(u8 Date)
{
		while(PlanQueue.Entries>=PLANSIZE)			//�����������ȴ�����
		{
			OS.DelayTimeTick(1);
		}
    EnterCritical();			
		PlanQueue.Line[PlanQueue.In] = Date;
    PlanQueue.In++;	
		PlanQueue.Entries++;	
    if (PlanQueue.In == PLANSIZE)
		{			
			PlanQueue.In = 0;
		}
    ExitCritical();	
}

/********************************************
������SubPlanQueueEntries
���ܣ����1�����ݵ��б�
��������
���أ���
********************************************/
u8 SubPlanQueueEntries(void)
{
	u8 t;
	t=PlanQueue.Line[PlanQueue.Out];
    EnterCritical();			
    PlanQueue.Out++;	
		PlanQueue.Entries--;	
    if (PlanQueue.Out == PLANSIZE)
		{			
			PlanQueue.Out = 0;
		}
    ExitCritical();	
		return t;
}


//void PlanLoadQueueOut(void)
//{
//	
//	switch (SubPlanQueueEntries())	
//	{
//		case PLANMOTOX:
//			if(PlanQueue.DirX)
//			{
//				Moto_Move(AXIS_X,STEP_TO_PUSH);
//			}
//			else
//			{
//				Moto_Move(AXIS_X,-STEP_TO_PUSH);
//			}
//			PlanQueue.Moto = PLANMOTOX;
//			break;
//		case PLANMOTOY:
//			if(PlanQueue.DirY)
//			{
//				Moto_Move(AXIS_Y,STEP_TO_PUSH);
//			}
//			else
//			{
//				Moto_Move(AXIS_Y,-STEP_TO_PUSH);
//			}			
//			PlanQueue.Moto = PLANMOTOY;	
//			break;
//		default:
//			break;
//	}	
//	
//}
void InitPlanQueue(void)
{
	PlanQueue.State = 0;	
	PlanQueue.In = 0;
	PlanQueue.Out = 0;
	PlanQueue.Entries = 0;

}

void WaitPlanQueueReady(void)
{
	while(PlanQueue.State != PLANSTATE_STOP)			//�ж϶����Ƿ�Ϊ���Gcode����
	{
		OS.DelayTimeTick(1);				//��ʱ�ȴ�
	}
	InitPlanQueue();
	PlanQueue.State = PLANSTATE_START;	//��ʼ������
//	OS.DelayTimeTick(DK_DELAY);
}



//u8 PlanQueueEntries(void)
//{
//	while(PlanQueue.Entries ==0)			//�ж϶����Ƿ�Ϊ���Gcode����
//	{
//		//���GCODE�����ж�
//		if(((PlanQueue.State == PLANSTATE_END)||(PlanQueue.State == PLANSTATE_RUN))&&(PlanQueue.Entries ==0)&&(Moto[AXIS_Y].Flag == 0)&&(Moto[AXIS_X].Flag == 0))
//		{
//			PlanQueue.State = PLANSTATE_STOP;
//			return 0xFF;
//		}				
//		OS.DelayTimeTick(1);				//��ʱ�ȴ�
//	}
//	return 0;
//}

u32 PanDuan_Step(s32 A,s32 B) //�ж����������Ƿ�С���ж�ֵ
{
	u8 MaiChong = STEP_TO_PUSH;
	if (A>B)
	{
		if((A-B)>MaiChong) return false;
		else return true;
	}
	else
	{
		if((B-A)>MaiChong) return false;
		else return true;
	}
}

//u8 PlanMotoRunOk(void)
//{
//	while(1)			//�ж϶����Ƿ�Ϊ���Gcode����
//	{

//		//���GCODE�����ж�
//		if(((PlanQueue.State == PLANSTATE_END)||(PlanQueue.State == PLANSTATE_RUN))&&(PlanQueue.Entries ==0)&&(Moto[AXIS_Y].Flag == 0)&&(Moto[AXIS_X].Flag == 0))
//		{
//			PlanQueue.State = PLANSTATE_STOP;
//			return 0xFF;
//		}			
//		if(PlanQueue.Flag == PLANFLAG_AA)
//		{
//			if(PanDuan_Step(Moto[AXIS_X].Current,Moto[AXIS_X].Target) && (Moto[AXIS_Y].Flag == 0))	return 0;		
//			if(PanDuan_Step(Moto[AXIS_Y].Current,Moto[AXIS_Y].Target) && (Moto[AXIS_X].Flag == 0))	return 0;				
//		}
//		if(PlanQueue.Flag == PLANFLAG_AB)
//		{
//			if(PanDuan_Step(Moto[AXIS_X].Current,Moto[AXIS_X].Target) && (PanDuan_Step(Moto[AXIS_Y].Current,Moto[AXIS_Y].Target)))	return 0;		
//		}
//		
//		
//		OS.DelayTimeTick(1);				//��ʱ�ȴ�
//	}
//}
//void Plan_Run(void)
//{
//	u8 OldMoto;

//	if(PlanQueue.State != PLANSTATE_STOP)	//һ��GCODE��ʼ����
//	{																					//��ʼ��
//		PlanQueue.Flag = 0;
//		PlanQueue.Moto = 0;
//		PlanQueue.State = PLANSTATE_RUN;
//		if(PlanQueueEntries()) return;		//�ж϶��к��Ƿ����
//		PlanLoadQueueOut();						//��ȡ��һ�����
//		while(1)											//��ʼѭ���ж�
//		{
//			if(PlanQueue.Entries >0)
//			{			
//				OldMoto = PlanQueue.Moto;
//				if(OldMoto & PLANMOTO_A)	//ԭ��ΪXYͬʱ(AB)
//				{
//					if((OldMoto & 0x0F) == PlanQueue.Line[PlanQueue.Out] )	//ԭ��ΪXYͬʱ���Ƚ�����е��뱾�ε���ͬ
//					{
//						PlanLoadQueueOut();						//��ȡ���(AB)A
//						if(PlanQueueEntries()) return;		//�ж϶��к��Ƿ����
//						if((OldMoto & 0x0F) != PlanQueue.Line[PlanQueue.Out] )
//						{
//							PlanLoadQueueOut();						//��ȡ���(AB)AB
//							PlanQueue.Moto = OldMoto;
//						}
//					}
//					else				//ԭ��ΪXYͬʱ���Ƚ�����е��뱾�εĲ���ͬ��ȡֵ�󱾴��жϽ���
//					{
//						PlanLoadQueueOut();						//��ȡ���(AB)B
//					}		
//					PlanQueue.Flag = PLANFLAG_AB;
//				}
//				else
//				{						
//					if(OldMoto == PlanQueue.Line[PlanQueue.Out])	//X��Y��ԭ�����
//					{
//						PlanLoadQueueOut();						//��ȡ���(A)A
//						PlanQueue.Flag = PLANFLAG_AA;
//					}
//					else  //
//					{
//						PlanLoadQueueOut();						//��ȡ���		(A)B					
//						if(PlanQueueEntries()) return;		//�ж϶��к��Ƿ����
//						if(OldMoto == PlanQueue.Line[PlanQueue.Out])
//						{
//							PlanLoadQueueOut();						//��ȡ���(A)BA
//							if(PlanQueueEntries()) return;		//�ж϶��к��Ƿ����
//							if(OldMoto != PlanQueue.Line[PlanQueue.Out] )
//							{
//								PlanLoadQueueOut();						//��ȡ���(A)BAB
//								PlanQueue.Moto = OldMoto | PLANMOTO_A;
//							}								
//						}		
//						else
//						{
//							PlanLoadQueueOut();						//��ȡ���(A)BB
//						}
//						PlanQueue.Flag = PLANFLAG_AB;
//					}						
//				}
//				if(PlanMotoRunOk()) return;		//�ж϶��к��Ƿ����		
//				if(PlanQueueEntries()) return;		//�ж϶��к��Ƿ����		
//				if(DK.Ctrl == CTRL_STOP)return;		//ֹͣ���
//			}		
//			if(PlanQueueEntries()) return;		//�ж϶��к��Ƿ����					
//		}
//	}
//}





/********************************************
������SubPlanQueueEntries
���ܣ����1�����ݵ��б�
��������
���أ���
********************************************/
u8 Plan_LoadNextEntries(void)
{
	if(PlanQueue.Entries == 0)			//����Ϊ0˵���Ѿ�������
	{
		PlanQueue.State = PLANSTATE_STOP;//���ý�����־
		while((Moto[AXIS_Y].Flag != 0)||(Moto[AXIS_X].Flag != 0))  //X/Y�ỹ��������Ҫ���
		{
			OS.DelayTimeTick(1);
		}	
		return 0xff;
	}
//	PlanQueue.Old = PlanQueue.Next;
	PlanQueue.Next = PlanQueue.Line[PlanQueue.Out];//��ȡ��һ��������
	return 0;
}
//������岢���м�1
void PlanRunMoto(void)
{	
	switch (SubPlanQueueEntries())	
	{
		case PLANMOTOX:
			if(PlanQueue.DirX)
			{
				Moto_Move(AXIS_X,STEP_TO_PUSH);
			}
			else
			{
				Moto_Move(AXIS_X,-STEP_TO_PUSH);
			}
			break;
		case PLANMOTOY:
			if(PlanQueue.DirY)
			{
				Moto_Move(AXIS_Y,STEP_TO_PUSH);
			}
			else
			{
				Moto_Move(AXIS_Y,-STEP_TO_PUSH);
			}			
			break;
		default:
			break;
	}	
//	PlanQueue.Old = PlanQueue.Next;
}



u8 PlanWaitRunOk(void)
{
	while(1)			//�ж϶����Ƿ�Ϊ���Gcode����
	{
		//���GCODE�����ж�
		if((PlanQueue.Entries ==0)&&(Moto[AXIS_Y].Flag == 0)&&(Moto[AXIS_X].Flag == 0))
		{
			PlanQueue.State = PLANSTATE_STOP;
			return 0xFF;
		}			
		if((PlanQueue.Flag == P_FLAG_A)||(PlanQueue.Flag == P_FLAG_AB))
		{
			if(PanDuan_Step(Moto[AXIS_X].Current,Moto[AXIS_X].Target))	return 0;				
		}		
		if((PlanQueue.Flag == P_FLAG_B)||(PlanQueue.Flag == P_FLAG_BA))
		{
			if(PanDuan_Step(Moto[AXIS_Y].Current,Moto[AXIS_Y].Target))	return 0;				
		}
		OS.DelayTimeTick(1);				//��ʱ�ȴ�
	}
}

//
void PlanRunFlag_A(void)
{
	
}
void PlanRun(void)
{

	if(PlanQueue.State != PLANSTATE_STOP)	//һ��GCODE��ʼ����
	{																					//��ʼ��
		PlanQueue.Flag = 0;
		PlanQueue.State = PLANSTATE_RUN;
		if(Plan_LoadNextEntries()) return;		//��ȡ��1�����
		PlanRunMoto();												//�������
//		PlanQueue.Old = PlanQueue.Next;
		if(PlanQueue.Next == PLANMOTOX)
		{	
			PlanQueue.Flag = P_FLAG_A;
		}
		else
		{
			PlanQueue.Flag = P_FLAG_B;
		}
		while(1)											//��ʼѭ���ж�
		{
			if(PlanQueue.Flag == P_FLAG_A)  				//A
			{
				if(Plan_LoadNextEntries()) return;		//��ȡ��2����� 
				PlanRunMoto();												//�������				
				if(PlanQueue.Next== PLANMOTOX)		//�¶�ȡ��������ԭ����ͬ  AA
				{
					if(Plan_LoadNextEntries()) return;		//��ȡ��3�����  
					if(PlanQueue.Next== PLANMOTOX)		//�¶�ȡ��������ԭ����ͬ  AAA
					{
						PlanRunMoto();											//�������		
					}
//				else																			AAB	,�������ȴ�																					
				}
				else																////�¶�ȡ��������ԭ����ͬ		AB
				{
					PlanQueue.Flag = P_FLAG_AB;						
				}				
			}
			else if(PlanQueue.Flag == P_FLAG_B)
			{
				if(Plan_LoadNextEntries()) return;		//��ȡ��2����� 
				PlanRunMoto();												//�������				
				if(PlanQueue.Next== PLANMOTOY)		//�¶�ȡ��������ԭ����ͬ  BB
				{
					if(Plan_LoadNextEntries()) return;		//��ȡ��3�����  
					if(PlanQueue.Next== PLANMOTOY)		//�¶�ȡ��������ԭ����ͬ  BBB
					{
						PlanRunMoto();											//�������		
					}
//				else																			BBA,�������ȴ�																					
				}
				else																////�¶�ȡ��������ԭ����ͬ		BA
				{
					PlanQueue.Flag = P_FLAG_BA;						
				}									
			}
			if(PlanQueue.Flag == P_FLAG_AB) // OLD =A
			{
				if(Plan_LoadNextEntries()) return;		//��ȡ��3����� 
				PlanRunMoto();												//�������					ABA/ABB ȫ����Ҫ���
				if(PlanQueue.Next== PLANMOTOX)		//ABA
				{
					if(Plan_LoadNextEntries()) return;		//��ȡ��4�����  ȫ����Ҫ���
					PlanRunMoto();											//�������		ABAA/ABAB		
					if(PlanQueue.Next== PLANMOTOX)		//ABAA
					{
						PlanQueue.Flag = P_FLAG_A;	
					}
//					else																	//ABAB
//					{
//						PlanQueue.Flag = P_FLAG_AB;	
//					}	
				}
				else																		//ABB
				{
					if(Plan_LoadNextEntries()) return;		//��ȡ��4�����   
						if(PlanQueue.Next != PLANMOTOX)		//  ABBB �����ABBA �����
					{
						PlanRunMoto();											//�������		ABBB
					}
//				else																			ABBA	,�������ȴ�	
					PlanQueue.Flag = P_FLAG_B;								
				}		
	
			}			
			if(PlanQueue.Flag == P_FLAG_BA)
			{
				if(Plan_LoadNextEntries()) return;		//��ȡ��3����� 
				PlanRunMoto();												//�������					BAB/BAA ȫ����Ҫ���
				if(PlanQueue.Next== PLANMOTOY)		//BAB
				{
					if(Plan_LoadNextEntries()) return;		//��ȡ��4�����  ȫ����Ҫ���
					PlanRunMoto();											//�������		BABB/BABA		
					if(PlanQueue.Next== PLANMOTOY)		//BABB
					{
						PlanQueue.Flag = P_FLAG_B;	
					}
//					else																	//BABA
//					{
//						PlanQueue.Flag = P_FLAG_BA;	
//					}	
				}
				else																		//BAA
				{
					if(Plan_LoadNextEntries()) return;		//��ȡ��4�����   
						if(PlanQueue.Next != PLANMOTOY)		//  BAAA �����BABA �����
					{
						PlanRunMoto();											//�������		BAAA
					}
//				else																			BAAB	,�������ȴ�	
					PlanQueue.Flag = P_FLAG_A;								
				}		
			}			
			

				if(PlanWaitRunOk()) return;		//�ж϶��к��Ƿ����		
				if(DK.Ctrl == CTRL_STOP)return;		//ֹͣ���	
		}
	}
}














