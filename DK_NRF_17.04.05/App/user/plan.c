/*  PLAN的流程图
先通过逐点插补方法算出每步需要的方向和数量，
然后PLAN根据插补算出的数据进行计划输出。
对于X轴和Y轴可以同步输出，不需要等待一个轴输出完成后再走第二个轴

                                   开始
                                    ↓
																第一个=X 
                      +-------------+-------------------+
											是                                否
                     ↓                                 ↓
										 ↓						+---------→---------------→-------------→----------------+ 
										 ↓					 ↑                     ↓                                   ↓
		 +--→---→---→--+				   ↑ 		+--→---→---→--+	          +--→---→---→----------------------------------------------------+	
		↑							 ↓		 	     ↑ 	 ↑								↓				 ↑			                   ↓                                  		 	↓
		↑							  A		 	     ↑ 	 ↑								 B				 ↑					   	           AB								      	  		    		 	BA
		↑						   ↓ 	       ↑ 	 ↑	              ↓ 	       ↑                        ↓                      	        				↓ 
		↑							Next=X 	     ↑    ↑              Next=Y 	   ↑					            Next=X 				            	         			Next=Y 
		↑					 +----+----+     ↑    ↑           +----+----+    ↑              +----------+----------+              		 +----------+----------+                                                      
 等待X轴脉冲		  是        否	   ↑ 等待Y轴脉冲     是        否    ↑		          是                     否          	     是                    否
 小于64个			  ↓        ↓  	 ↑  小于64个		   ↓        ↓    ↑             ↓                    ↓            			 ↓        						↓                                                                     
		↑			  AA(输出)     AB-→→+   	 ↑         BB(输出)    BA-→-+  		      ABA(输出)            ABB(输出)		       	BAB(输出)      				BAA(输出)	
		↑					↓							 ↑		 ↑          ↓							 ↑	            ↓		                ↓				         ↓		                ↓
		↑				Next=X			       ↑    ↑         Next=Y           ↑           Next=X               Next=X        	      Next=X                 Next=X
		↑		  +----+----+    等待X轴脉冲	 ↑ 		 +----+----+       等待Y轴脉冲  +----+----+           +----+----+      	 +----+----+            +----+----+                                  
		↑		  是        否		小于64个 	 ↑      是        否        小于64个   是        否          是        否        是        否           是          否
		↑	 		↓        ↓       	 ↑    ↑  		 ↓        ↓     	 ↑    ABAA(输出)  ABAB(输出) ABBB(输出) 只输出ABB   BABB(输出) BABA(输出)  BAAA(输出) 只输出BAA                                     
		↑	 	AAA(输出)  只输出AA  	 ↑		 ↑		BBB(输出)  只输出BB 		 ↑				 ↓			   ↓					 ↓				↓		  	 ↓			   ↓				 	 ↓					↓																			
		↑			↓				↓				 ↑		 ↑			 ↓				↓				 ↑			   ↓        ↓          ↓        ↓	     	 ↓        ↓        	 ↓        	↓
		↑			↓				↓		     ↑     +---←--+----←--+--------------------------------------←--+----←--+	---------+   		   ↓        	 ↓        	↓
		↑			↓				↓		     ↑       	                       ↑        ↓        ↓				                                 ↓        	 ↓        	↓
		+---←--+----←----+----------------------←--------------←--------←----+---------------------------------------------------------------+-----------+  	 									       																																	
				                         ↑ 															 ↑									 ↓				                                 ↓																										
																  +------------------←-----------------------←--------+				                                 ↓																																				
																																	 ↑								                                             ↓
																																	  +------------------←-----------------------←----------------+								




*/
#include "plan.h"
#include "os.h"
#include "moto_4988.h"
#include "dk_conf.h"
#include "limit.h"


PlanStruct PlanQueue;

/********************************************
函数：AddPlanQueueEntries
功能：增加1列数据到列表
参数：无
返回：无
********************************************/
void AddPlanQueueEntries(u8 Date)
{
		while(PlanQueue.Entries>=PLANSIZE)			//队列已满，等待队列
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
函数：SubPlanQueueEntries
功能：清除1列数据到列表
参数：无
返回：无
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
	while(PlanQueue.State != PLANSTATE_STOP)			//判断队列是否为零或Gcode结束
	{
		OS.DelayTimeTick(1);				//延时等待
	}
	InitPlanQueue();
	PlanQueue.State = PLANSTATE_START;	//初始化队列
//	OS.DelayTimeTick(DK_DELAY);
}



//u8 PlanQueueEntries(void)
//{
//	while(PlanQueue.Entries ==0)			//判断队列是否为零或Gcode结束
//	{
//		//需加GCODE结束判断
//		if(((PlanQueue.State == PLANSTATE_END)||(PlanQueue.State == PLANSTATE_RUN))&&(PlanQueue.Entries ==0)&&(Moto[AXIS_Y].Flag == 0)&&(Moto[AXIS_X].Flag == 0))
//		{
//			PlanQueue.State = PLANSTATE_STOP;
//			return 0xFF;
//		}				
//		OS.DelayTimeTick(1);				//延时等待
//	}
//	return 0;
//}

u32 PanDuan_Step(s32 A,s32 B) //判断脉冲数量是否小于判断值
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
//	while(1)			//判断队列是否为零或Gcode结束
//	{

//		//需加GCODE结束判断
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
//		OS.DelayTimeTick(1);				//延时等待
//	}
//}
//void Plan_Run(void)
//{
//	u8 OldMoto;

//	if(PlanQueue.State != PLANSTATE_STOP)	//一个GCODE开始解析
//	{																					//初始化
//		PlanQueue.Flag = 0;
//		PlanQueue.Moto = 0;
//		PlanQueue.State = PLANSTATE_RUN;
//		if(PlanQueueEntries()) return;		//判断队列和是否结束
//		PlanLoadQueueOut();						//读取第一个输出
//		while(1)											//开始循环判断
//		{
//			if(PlanQueue.Entries >0)
//			{			
//				OldMoto = PlanQueue.Moto;
//				if(OldMoto & PLANMOTO_A)	//原来为XY同时(AB)
//				{
//					if((OldMoto & 0x0F) == PlanQueue.Line[PlanQueue.Out] )	//原来为XY同时最先进入队列的与本次的相同
//					{
//						PlanLoadQueueOut();						//读取输出(AB)A
//						if(PlanQueueEntries()) return;		//判断队列和是否结束
//						if((OldMoto & 0x0F) != PlanQueue.Line[PlanQueue.Out] )
//						{
//							PlanLoadQueueOut();						//读取输出(AB)AB
//							PlanQueue.Moto = OldMoto;
//						}
//					}
//					else				//原来为XY同时最先进入队列的与本次的不相同，取值后本次判断结束
//					{
//						PlanLoadQueueOut();						//读取输出(AB)B
//					}		
//					PlanQueue.Flag = PLANFLAG_AB;
//				}
//				else
//				{						
//					if(OldMoto == PlanQueue.Line[PlanQueue.Out])	//X或Y与原来相等
//					{
//						PlanLoadQueueOut();						//读取输出(A)A
//						PlanQueue.Flag = PLANFLAG_AA;
//					}
//					else  //
//					{
//						PlanLoadQueueOut();						//读取输出		(A)B					
//						if(PlanQueueEntries()) return;		//判断队列和是否结束
//						if(OldMoto == PlanQueue.Line[PlanQueue.Out])
//						{
//							PlanLoadQueueOut();						//读取输出(A)BA
//							if(PlanQueueEntries()) return;		//判断队列和是否结束
//							if(OldMoto != PlanQueue.Line[PlanQueue.Out] )
//							{
//								PlanLoadQueueOut();						//读取输出(A)BAB
//								PlanQueue.Moto = OldMoto | PLANMOTO_A;
//							}								
//						}		
//						else
//						{
//							PlanLoadQueueOut();						//读取输出(A)BB
//						}
//						PlanQueue.Flag = PLANFLAG_AB;
//					}						
//				}
//				if(PlanMotoRunOk()) return;		//判断队列和是否结束		
//				if(PlanQueueEntries()) return;		//判断队列和是否结束		
//				if(DK.Ctrl == CTRL_STOP)return;		//停止雕刻
//			}		
//			if(PlanQueueEntries()) return;		//判断队列和是否结束					
//		}
//	}
//}





/********************************************
函数：SubPlanQueueEntries
功能：清除1列数据到列表
参数：无
返回：无
********************************************/
u8 Plan_LoadNextEntries(void)
{
	if(PlanQueue.Entries == 0)			//队列为0说明已经结束了
	{
		PlanQueue.State = PLANSTATE_STOP;//设置结束标志
		while((Moto[AXIS_Y].Flag != 0)||(Moto[AXIS_X].Flag != 0))  //X/Y轴还有脉冲需要输出
		{
			OS.DelayTimeTick(1);
		}	
		return 0xff;
	}
//	PlanQueue.Old = PlanQueue.Next;
	PlanQueue.Next = PlanQueue.Line[PlanQueue.Out];//提取下一个功能码
	return 0;
}
//输出脉冲并队列减1
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
	while(1)			//判断队列是否为零或Gcode结束
	{
		//需加GCODE结束判断
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
		OS.DelayTimeTick(1);				//延时等待
	}
}

//
void PlanRunFlag_A(void)
{
	
}
void PlanRun(void)
{

	if(PlanQueue.State != PLANSTATE_STOP)	//一个GCODE开始解析
	{																					//初始化
		PlanQueue.Flag = 0;
		PlanQueue.State = PLANSTATE_RUN;
		if(Plan_LoadNextEntries()) return;		//读取第1个输出
		PlanRunMoto();												//输出脉冲
//		PlanQueue.Old = PlanQueue.Next;
		if(PlanQueue.Next == PLANMOTOX)
		{	
			PlanQueue.Flag = P_FLAG_A;
		}
		else
		{
			PlanQueue.Flag = P_FLAG_B;
		}
		while(1)											//开始循环判断
		{
			if(PlanQueue.Flag == P_FLAG_A)  				//A
			{
				if(Plan_LoadNextEntries()) return;		//读取第2个输出 
				PlanRunMoto();												//输出脉冲				
				if(PlanQueue.Next== PLANMOTOX)		//新读取的数据与原来相同  AA
				{
					if(Plan_LoadNextEntries()) return;		//读取第3个输出  
					if(PlanQueue.Next== PLANMOTOX)		//新读取的数据与原来相同  AAA
					{
						PlanRunMoto();											//输出脉冲		
					}
//				else																			AAB	,不处理，等待																					
				}
				else																////新读取的数据与原来不同		AB
				{
					PlanQueue.Flag = P_FLAG_AB;						
				}				
			}
			else if(PlanQueue.Flag == P_FLAG_B)
			{
				if(Plan_LoadNextEntries()) return;		//读取第2个输出 
				PlanRunMoto();												//输出脉冲				
				if(PlanQueue.Next== PLANMOTOY)		//新读取的数据与原来相同  BB
				{
					if(Plan_LoadNextEntries()) return;		//读取第3个输出  
					if(PlanQueue.Next== PLANMOTOY)		//新读取的数据与原来相同  BBB
					{
						PlanRunMoto();											//输出脉冲		
					}
//				else																			BBA,不处理，等待																					
				}
				else																////新读取的数据与原来不同		BA
				{
					PlanQueue.Flag = P_FLAG_BA;						
				}									
			}
			if(PlanQueue.Flag == P_FLAG_AB) // OLD =A
			{
				if(Plan_LoadNextEntries()) return;		//读取第3个输出 
				PlanRunMoto();												//输出脉冲					ABA/ABB 全部需要输出
				if(PlanQueue.Next== PLANMOTOX)		//ABA
				{
					if(Plan_LoadNextEntries()) return;		//读取第4个输出  全部需要输出
					PlanRunMoto();											//输出脉冲		ABAA/ABAB		
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
					if(Plan_LoadNextEntries()) return;		//读取第4个输出   
						if(PlanQueue.Next != PLANMOTOX)		//  ABBB 输出，ABBA 不输出
					{
						PlanRunMoto();											//输出脉冲		ABBB
					}
//				else																			ABBA	,不处理，等待	
					PlanQueue.Flag = P_FLAG_B;								
				}		
	
			}			
			if(PlanQueue.Flag == P_FLAG_BA)
			{
				if(Plan_LoadNextEntries()) return;		//读取第3个输出 
				PlanRunMoto();												//输出脉冲					BAB/BAA 全部需要输出
				if(PlanQueue.Next== PLANMOTOY)		//BAB
				{
					if(Plan_LoadNextEntries()) return;		//读取第4个输出  全部需要输出
					PlanRunMoto();											//输出脉冲		BABB/BABA		
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
					if(Plan_LoadNextEntries()) return;		//读取第4个输出   
						if(PlanQueue.Next != PLANMOTOY)		//  BAAA 输出，BABA 不输出
					{
						PlanRunMoto();											//输出脉冲		BAAA
					}
//				else																			BAAB	,不处理，等待	
					PlanQueue.Flag = P_FLAG_A;								
				}		
			}			
			

				if(PlanWaitRunOk()) return;		//判断队列和是否结束		
				if(DK.Ctrl == CTRL_STOP)return;		//停止雕刻	
		}
	}
}














