/*********************************************************************************
* �ļ���			: DK_Config.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/21
* ˵��      : ��̻��Ļ���ϵͳ���úͳ�ʼ��
* �÷�			��

**********************************************************************************/
#include "stm32f10x.h"
#include "dk_conf.h"
#include "queue.h"	




#define DK_CRC				0x5A00
#define DK_XY_SPEED		BKP_DR2
#define DK_Z_SPEED		BKP_DR3
#define DK_FASTSPEED	BKP_DR4
#define DK_TFTSIZE		BKP_DR5
#define DK_MODE				BKP_DR6

/********************************************
������SaveData
���ܣ�����������󱸼Ĵ���
��������
���أ���
˵����
********************************************/
void SaveData(void)
{
	u32 BKP_T;
	BKP_T = BKP_ReadBackupRegister(BKP_DR1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
	BKP_WriteBackupRegister(BKP_DR1, (BKP_T &0XFF)|DK_CRC);
	BKP_WriteBackupRegister(DK_XY_SPEED, DK.SYS.Speed_XY);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	BKP_WriteBackupRegister(DK_Z_SPEED, DK.SYS.Speed_Z);	
	BKP_WriteBackupRegister(DK_FASTSPEED, DK.SYS.Speed_Fast);		
	BKP_WriteBackupRegister(DK_TFTSIZE, DK.SYS.TFTShowSize);	
	
}

/********************************************
������LoadData
���ܣ���ȡϵͳ����
��������
���أ���
˵�����󱸼Ĵ�����������ʹ�ú󱸼Ĵ��������ݣ�û��ʹ��Ĭ������
********************************************/
void LoadSYSData(void)
{
	u32 BKP_T;
	BKP_T = BKP_ReadBackupRegister(BKP_DR1);
	if((BKP_T & 0xFF00)==DK_CRC)
	{
		DK.SYS.Speed_XY = BKP_ReadBackupRegister(DK_XY_SPEED);
		DK.SYS.Speed_Z = BKP_ReadBackupRegister(DK_Z_SPEED);		
		DK.SYS.Speed_Fast = BKP_ReadBackupRegister(DK_FASTSPEED);				
		DK.SYS.TFTShowSize = BKP_ReadBackupRegister(DK_TFTSIZE);		
		DK.SYS.Mode = BKP_ReadBackupRegister(DK_MODE);		
	}
	else
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
		PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
		DK.SYS.Speed_XY =  1;
		DK.SYS.Speed_Z =  1;
		DK.SYS.Speed_Fast =  1;		
		DK.SYS.TFTShowSize =  5;
		DK.SYS.Mode = 1;
		SaveData();
		BKP_WriteBackupRegister(BKP_DR1, (BKP_T&0X00FF) |DK_CRC);	//��ָ���ĺ󱸼Ĵ�����д���û���������
		
	}
}




