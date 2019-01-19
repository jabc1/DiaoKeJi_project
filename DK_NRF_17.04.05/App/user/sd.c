/*********************************************************************************
* 文件名		: menu.C
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 处理与菜单有关的函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/

#include "SD.h"
#include "g_code.h"
#include "stdio.h"
#include "GUI_conf.h"
#include "app.h"
#include "malloc.h"	 
#include "device_conf.h"

/**********************************************************
                 文件系统公共文件操作区域
**********************************************************/

FATFS *fs[_VOLUMES];	//逻辑磁盘工作区
FIL *filescr;			//文件1
FIL *filedst;			//文件2
UINT br,bw;				//读写变量
FRESULT f_res;			//FatFs通用结果码

uint8_t *SDdatabuff;	//SD卡数据缓存
uint16_t rlen;			//读取到数据长度
DIR *dir;				//文件夹
FILINFO fileinfo;		//文件信息结构体


u8* SDdatabuff_T;					//SD卡读取数据的指针
u32 FileRead=0;						//已经处理的文件字节数量
u16 FileBuffNum = 0;			//读取的一个块中已经处理的字节数量




/**********************************************************
* 函数功能 ---> 文件系统信息初始化
* 入口参数 ---> none
* 返回数值 ---> 0：成功
*               1：失败
* 功能说明 ---> 主要是为变量申请内存
**********************************************************/
uint8_t myf_init(void)
{
	fs[0] = (FATFS*)mymalloc(sizeof(FATFS));	//为磁盘0工作区申请内存

	filescr = (FIL*)mymalloc(sizeof(FIL));		//为文件1申请内存
	filedst = (FIL*)mymalloc(sizeof(FIL));		//为文件2申请内存
	dir = (DIR*)mymalloc(sizeof(DIR));			//为文件夹申请内存
	SDdatabuff = (uint8_t*)mymalloc(1024);		//为SD卡数据缓存申请内存
	
	if(fs[0]&&filescr&&filedst&&dir&&SDdatabuff)	return 0;	//申请有一个失败, 即失败
	else	return 1;	//申请失败
}
/**********************************************************
* 函数功能 ---> 打印SD卡信息到串口
* 入口参数 ---> none
* 返回数值 ---> none
* 功能说明 ---> 1、打印卡的容量到串口
*               2、打印卡的类型到串口
*               3、打印卡的其他信息
**********************************************************/
void SD_Card_Printf_Info(void)
{
	switch(SDCardInfo.CardType)	//卡类型
	{
		case SDIO_HIGH_CAPACITY_SD_CARD:	//高容量卡
			printf("Card Type: SDHC V2.0\r\n");
			break;
			
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:	//标准容量V1.1
			printf("Card Type: SDSC V1.1\r\n");
			break;
			
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:	//标准容量V2.0
			printf("Card Type: SDSC V2.0\r\n");
			break;

		case SDIO_MULTIMEDIA_CARD:	//MMC卡
			printf("Card Type: MMC Card\r\n");
			break;
	}

	printf("Card ManufacturerID: %d\r\n",SDCardInfo.SD_cid.ManufacturerID);			//制造商ID
 	printf("Card RCA: %d\r\n",SDCardInfo.RCA);										//卡相对地址
	printf("Card Capacity: %d MB\r\n",(uint32_t)SDCardInfo.CardCapacity);	//显示容量
 	printf("Card BlockSize: %d\r\n\r\n",SDCardInfo.CardBlockSize);					//显示块大小
}

	u8 *fn; 
u32 TLen=0X10;

void InitSD(void)
{
//	u8 SDtatus;	//SD卡初始化状态	
	
	GUI_PutStringAt("Software Compiled Time:" ,30,30);	//获取软件编译时间	
	GUI_PutStringAt(__DATE__ ,30,50);	//获取软件编译时间		
	GUI_PutStringAt(__TIME__ ,130,50);	//获取软件编译时间		
	GUI_PutStringAt("SD card Initialize ......",60,70);	
	
	myf_init();	//为文件系统申请内存

	/******************************************************
	                      显示基本信息
	                     从LCD顶部开始 
	******************************************************/


	printf("Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//获取软件编译时间

	/*******************我是美丽的分割线******************/
	
	/******************************************************
	                      SD卡初始化 
	******************************************************/
//		SDtatus = SD_Init();	
//	if(SD_Init() != SD_OK)	//初始化失败
//	{
//		printf("The Faild is:%d\r\n", SDtatus);
//		GUI_PutStringAt("SD Card Init Faild" ,60,90);	//获取软件编译时间	

//	}
//	else	//初始化成功了
//	{
//		SD_Card_Printf_Info();	//打印卡信息到串口
//		GUI_PutStringAt("SD Card Init OK" ,60,90);			

//	}
	f_res = f_mount(0, fs[0]);	//挂载SD卡			
	
	f_res = f_open(filescr,(const TCHAR *) GUI.Font->GBFont_P, FA_OPEN_EXISTING | FA_READ);	
	
	if(f_res == FR_OK)	//打开文件成功
	{
		GUI_PutStringAt("GBK字库打开正常",70,110);	
	}
	else	//打开失败
	{
		GUI_PutStringAt("GBK12.FON open False",70,110);			
	}	
	f_res = f_close(filescr);	//关闭文件

		DelayMs(1000);
	
		GUI_Clear(GUI.B_Color);



}

void ReadFile(void)
{
	u32 len;
	if(DK.File_B_P->NeedRead)
	{
		if(filescr->fsize - FileRead>512)
		{
		len = 512;	//读取文件内容	
		}
		else
		{
			len =filescr->fsize - FileRead;
		}
		
		f_res = f_read(filescr, SDdatabuff, len, &br);	//读取文件内容			
		SDdatabuff_T = SDdatabuff;		
		DK.File_B_P->NeedRead = 0;		
	}
}

u8 ReadOneLine(void)
{
	u8 i=1;
	while(1)
	{			
		OneLine[i] = *SDdatabuff_T;
		SDdatabuff_T ++;
		FileRead++;		
		FileBuffNum ++;			
		if(FileBuffNum == 512 )
		{
			DK.File_B_P->NeedRead = 1;
			ReadFile();
			FileBuffNum = 0;
		}
		if(filescr->fsize == FileRead) 
		{
			DK.File_B_P->NotEnd = 0;
			OneLine[0] = i;
			OneLine[i+1] = 0;			
			return 0;
		}		
		if(OneLine[i]==0x0A)
		{			
			OneLine[0] = i;
			OneLine[i+1] = 0;
			return 0;		
		}					

		i++;		
		if(i>=64)
		{			
			OneLine[0] = 0;
			return 1;	
		}		
	}			
}












