/*

*/

#ifndef __PLAN_H
#define __PLAN_H
#include "stm32f10x.h" 
#define PLANSIZE	100
#define PLANSTATE_START			1
#define PLANSTATE_RUN			2
#define PLANSTATE_END			3
#define PLANSTATE_STOP		0

#define PLANMOTOX				0x01
#define PLANMOTOY				0x02

#define P_FLAG_A			0x01
#define P_FLAG_B			0x02
#define P_FLAG_AB			0x04
#define P_FLAG_BA			0x08



typedef struct 
{
	u8 State;
	u8 Flag;
	u8 DirX;	
	u8 DirY;
	u8 Next;
	u8 In;
	u8 Out;
	u8 Entries;
	
	u8 Line[PLANSIZE];

}PlanStruct;
extern PlanStruct PlanQueue;



void InitPlanQueue(void);

void AddPlanQueueEntries(u8 date);
u8 SubPlanQueueEntries(void);

void PlanRun(void);

void WaitPlanQueueReady(void);



#endif 

