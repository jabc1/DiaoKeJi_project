#ifndef __GUI_OS_H__
#define __GUI_OS_H__
#include "stm32f10x.h" 
#include "gui_conf.h" 
#include <string.h>
/******************************GUI.C********************************************/


#if GUI_USEMSOS	
extern uint8_t    OsCurrentPriority;	//MSOS当前任务
extern 	u8 GUITaskPriority ;		/*GUI解析任务*/
extern void GUICreateTask(void);	

void GUITask (void);
void GUI_RunFunction(u8 Priority,GUI_func RegisterFunction);

void GUI_FL_OpenFile(u8 Priority,u32 *FileList_P);
void GUI_FL_Close(u8 Priority,u32 *FileList_P);

#endif		
	
#endif	

	

