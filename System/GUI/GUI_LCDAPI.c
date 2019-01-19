
/*********************************************************************************
* �ļ���			: GUI_LCDAPI.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI��LCD�������ݽ����ĺ���					  
* �÷�			����ʹ��GUIǰ����Ѷ�Ӧ�ĺ������й���

**********************************************************************************/
#include "device_LCD.h" 

/********************************************
������GUI_InitLcd
���ܣ���ʼ��LCD
��������
���أ���
********************************************/	
void GUI_InitLcd(void)			//LCD��ʼ��
{
	InitializeLcd();	
}	
/********************************************
������GUI_BackLight
���ܣ�LCD�������
��������
���أ���
********************************************/	
void GUI_BackLight(u8 status)																		//�������	
{
	LCD_BackLight(status);
}

#if LCDAPIUSEFAST > 0		//ʹ�ÿ��ٵ�LCD���ƺ���
	void GUI_BeginWriteRAM(void)																			//LCD��ʼд��RAM
	{
		LCD_BeginWriteRAM();
	}	
	void GUI_EndWriteRAM(void)																				//LCD����д��RAM
	{
		LCD_EndWriteRAM();
	}
	void GUI_OnlyWriteRAM(u16 LCD_DATA)														//LCD����д��RAM,û�п��Ƶ�����
	{
		LCD_OnlyWriteRAM(LCD_DATA);
	}
#endif
void GUI_SetCursor(u16 X,u16 Y)																//������Ļ����
{
	LCD_SetCursor(X,Y);
}
void GUI_SetPoint(u16 X,u16 Y,u16 Color)													//LCD��㺯��
{
	LCD_SetPoint(X,Y,Color);
}
void GUI_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)		//����LCD��Ļ��Χ
{
	LCD_SetWindows(StartX,StartY,EndX,EndY);
}
u16 GUI_GetPoint(u16 X,u16 Y)					//1.44������û�ж�ȡ��IO���ʶ�ȡ����û��
{
	return LCD_GetPoint(X,Y);
}





