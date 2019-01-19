#ifndef __GUI_LCDAPI_H__
#define __GUI_LCDAPI_H__

#include "stm32f10x.h"  

void GUI_InitLcd(void);			//LCD初始化
void GUI_BackLight(u8 status);																		//背光控制	
void GUI_BeginWriteRAM(void);																			//LCD开始写入RAM
void GUI_EndWriteRAM(void);																				//LCD结束写入RAM
void GUI_OnlyWriteRAM(u16 LCD_DATA);														//LCD仅仅写入RAM,没有控制的命令
void GUI_SetCursor(u16 X,u16 Y);																//设置屏幕座标
void GUI_SetPoint(u16 X,u16 Y,u16 Color);													//LCD打点函数
void GUI_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);		//设置LCD屏幕范围
u16 LCD_GetPoint(u16 X,u16 Y);					//1.44屏由于没有读取的IO，故读取函数没有


	
	
	
	
	
	
	
	
	
	
	
	
/*****************LCD基本函数定义*******************************/

//	#define GUI_LCDInit							InitializeLcd	//LCD初始化
//	#define GUI_BackLight 					LCD_BackLight//背光控制
//	#define GUI_BeginWriteRAM				LCD_BeginWriteRAM//LCD开始写入RAM
//	#define GUI_EndWriteRAM					LCD_EndWriteRAM//LCD结束写入RAM
//	#define GUI_WriteRAM						LCD_OnlyWriteRAM//LCD仅仅写入RAM,没有控制的命令
//	#define GUI_SetCursor						LCD_SetCursor//设置屏幕座标
//	#define GUI_SetPoint 						LCD_SetPoint//LCD打点函数
//	#define GUI_SetWindows					LCD_SetWindows//设置LCD屏幕范围
//	#define GUI_GetPoint						LCD_GetPoint//获取坐标的颜色
//	
	
	
	

	

#endif 
