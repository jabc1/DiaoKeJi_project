#ifndef __DEVICE_RTC_H_
#define __DEVICE_RTC_H_
#include "stm32f10x.h" 

typedef struct
{
	u8 Year;			//公历日月年周
	u8 Month;
	u8 Date;				
	u8 Hour;
	u8 Min;
	u8 Sec;		
	u8 Week;					
} Calendar_Struct;	//日历结构体


	extern Calendar_Struct RTC_Calendar;
	extern const u8 mon_table[];
	void InitializeRtc(void);
	u8 RtcInitialize(void);
	u8 RTC_Get(void);         																		//更新时间   
	u8 RTC_Get_Week(u16 year,u8 month,u8 day);
	u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);	//设置时间	

#endif

