/*********************************************************************************
* 文件名			: DK_Config.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/21
* 说明      : 雕刻机的基本系统配置和初始化
* 用法			：

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
函数：SaveData
功能：保存参数到后备寄存器
参数：无
返回：无
说明：
********************************************/
void SaveData(void)
{
	u32 BKP_T;
	BKP_T = BKP_ReadBackupRegister(BKP_DR1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	BKP_WriteBackupRegister(BKP_DR1, (BKP_T &0XFF)|DK_CRC);
	BKP_WriteBackupRegister(DK_XY_SPEED, DK.SYS.Speed_XY);	//向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(DK_Z_SPEED, DK.SYS.Speed_Z);	
	BKP_WriteBackupRegister(DK_FASTSPEED, DK.SYS.Speed_Fast);		
	BKP_WriteBackupRegister(DK_TFTSIZE, DK.SYS.TFTShowSize);	
	
}

/********************************************
函数：LoadData
功能：读取系统参数
参数：无
返回：无
说明：后备寄存器内有数据使用后备寄存器内数据，没有使用默认数据
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
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
		DK.SYS.Speed_XY =  1;
		DK.SYS.Speed_Z =  1;
		DK.SYS.Speed_Fast =  1;		
		DK.SYS.TFTShowSize =  5;
		DK.SYS.Mode = 1;
		SaveData();
		BKP_WriteBackupRegister(BKP_DR1, (BKP_T&0X00FF) |DK_CRC);	//向指定的后备寄存器中写入用户程序数据
		
	}
}




