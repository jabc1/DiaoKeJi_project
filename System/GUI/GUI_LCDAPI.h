#ifndef __GUI_LCDAPI_H__
#define __GUI_LCDAPI_H__

#include "stm32f10x.h"  

void GUI_InitLcd(void);			//LCD��ʼ��
void GUI_BackLight(u8 status);																		//�������	
void GUI_BeginWriteRAM(void);																			//LCD��ʼд��RAM
void GUI_EndWriteRAM(void);																				//LCD����д��RAM
void GUI_OnlyWriteRAM(u16 LCD_DATA);														//LCD����д��RAM,û�п��Ƶ�����
void GUI_SetCursor(u16 X,u16 Y);																//������Ļ����
void GUI_SetPoint(u16 X,u16 Y,u16 Color);													//LCD��㺯��
void GUI_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);		//����LCD��Ļ��Χ
u16 LCD_GetPoint(u16 X,u16 Y);					//1.44������û�ж�ȡ��IO���ʶ�ȡ����û��


	
	
	
	
	
	
	
	
	
	
	
	
/*****************LCD������������*******************************/

//	#define GUI_LCDInit							InitializeLcd	//LCD��ʼ��
//	#define GUI_BackLight 					LCD_BackLight//�������
//	#define GUI_BeginWriteRAM				LCD_BeginWriteRAM//LCD��ʼд��RAM
//	#define GUI_EndWriteRAM					LCD_EndWriteRAM//LCD����д��RAM
//	#define GUI_WriteRAM						LCD_OnlyWriteRAM//LCD����д��RAM,û�п��Ƶ�����
//	#define GUI_SetCursor						LCD_SetCursor//������Ļ����
//	#define GUI_SetPoint 						LCD_SetPoint//LCD��㺯��
//	#define GUI_SetWindows					LCD_SetWindows//����LCD��Ļ��Χ
//	#define GUI_GetPoint						LCD_GetPoint//��ȡ�������ɫ
//	
	
	
	

	

#endif 
