
/*********************************************************************************
* 文件名			: GUI_LCDAPI.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI与LCD进行数据交换的函数					  
* 用法			：在使用GUI前必须把对应的函数进行关联

**********************************************************************************/
#include "device_LCD.h" 

/********************************************
函数：GUI_InitLcd
功能：初始化LCD
参数：无
返回：无
********************************************/	
void GUI_InitLcd(void)			//LCD初始化
{
	InitializeLcd();	
}	
/********************************************
函数：GUI_BackLight
功能：LCD背光控制
参数：无
返回：无
********************************************/	
void GUI_BackLight(u8 status)																		//背光控制	
{
	LCD_BackLight(status);
}

#if LCDAPIUSEFAST > 0		//使用快速的LCD控制函数
	void GUI_BeginWriteRAM(void)																			//LCD开始写入RAM
	{
		LCD_BeginWriteRAM();
	}	
	void GUI_EndWriteRAM(void)																				//LCD结束写入RAM
	{
		LCD_EndWriteRAM();
	}
	void GUI_OnlyWriteRAM(u16 LCD_DATA)														//LCD仅仅写入RAM,没有控制的命令
	{
		LCD_OnlyWriteRAM(LCD_DATA);
	}
#endif
void GUI_SetCursor(u16 X,u16 Y)																//设置屏幕座标
{
	LCD_SetCursor(X,Y);
}
void GUI_SetPoint(u16 X,u16 Y,u16 Color)													//LCD打点函数
{
	LCD_SetPoint(X,Y,Color);
}
void GUI_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)		//设置LCD屏幕范围
{
	LCD_SetWindows(StartX,StartY,EndX,EndY);
}
u16 GUI_GetPoint(u16 X,u16 Y)					//1.44屏由于没有读取的IO，故读取函数没有
{
	return LCD_GetPoint(X,Y);
}





