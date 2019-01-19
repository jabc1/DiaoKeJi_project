#include "limit.h"
#include "stm32f10x.h"
#include "device_conf.h"
#include "dk_conf.h"







//外部中断0服务程序
//初始化PA0,PA13,PA15为中断输入.
void Moto_Limit_Init(void)
{
	IO_IN_IPD(LIMIT_XL_GPIO,LIMIT_XL_PIN,LIMIT_XL_RCC);
	IO_IN_IPD(LIMIT_XR_GPIO,LIMIT_XR_PIN,LIMIT_XR_RCC);
	IO_IN_IPD(LIMIT_YL_GPIO,LIMIT_YL_PIN,LIMIT_YL_RCC);	
	IO_IN_IPD(LIMIT_YR_GPIO,LIMIT_YR_PIN,LIMIT_YR_RCC);	
}

void Limit(void)
{	
	DK.Alarm_B_P->L[AXIS_X] = LIMIT_XL;
	DK.Alarm_B_P->L[AXIS_Y] = LIMIT_YL;	
	DK.Alarm_B_P->L[AXIS_Z] = 0;		
	DK.Alarm_B_P->R[AXIS_X] = LIMIT_XR;
	DK.Alarm_B_P->R[AXIS_Y] = LIMIT_YR;	
	DK.Alarm_B_P->R[AXIS_Z] = 0;		
}
 
