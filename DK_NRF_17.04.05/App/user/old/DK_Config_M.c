
#include "stm32f10x.h"
#include "dk_config_m.h"

 DiaoKe_Struct DK;






void InitDK(void)
{	
	
	DK.File_B_P = (DK_B_File_Struct *)BitAddr(&(DK.File));	//״̬��λ��λ�������� 
	
	DK.SYS_B_P = (DK_B_SYS_Struct *)BitAddr(&(DK.SYS));	//״̬��λ��λ��������
	DK.Alarm_B_P = (DK_B_Alarm_Struct* )BitAddr(&(DK.Alarm));		//������λ��λ��������
	
	
}




