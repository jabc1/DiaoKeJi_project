/*********************************************************************************
* �ļ���		: menu.C
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ������˵��йصĺ���					  
* �÷�			��
* �汾˵��	��
**********************************************************************************/

#include "SD.h"
#include "g_code.h"
#include "stdio.h"
#include "GUI_conf.h"
#include "app.h"
#include "malloc.h"	 
#include "device_conf.h"

/**********************************************************
                 �ļ�ϵͳ�����ļ���������
**********************************************************/

FATFS *fs[_VOLUMES];	//�߼����̹�����
FIL *filescr;			//�ļ�1
FIL *filedst;			//�ļ�2
UINT br,bw;				//��д����
FRESULT f_res;			//FatFsͨ�ý����

uint8_t *SDdatabuff;	//SD�����ݻ���
uint16_t rlen;			//��ȡ�����ݳ���
DIR *dir;				//�ļ���
FILINFO fileinfo;		//�ļ���Ϣ�ṹ��


u8* SDdatabuff_T;					//SD����ȡ���ݵ�ָ��
u32 FileRead=0;						//�Ѿ�������ļ��ֽ�����
u16 FileBuffNum = 0;			//��ȡ��һ�������Ѿ�������ֽ�����




/**********************************************************
* �������� ---> �ļ�ϵͳ��Ϣ��ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> 0���ɹ�
*               1��ʧ��
* ����˵�� ---> ��Ҫ��Ϊ���������ڴ�
**********************************************************/
uint8_t myf_init(void)
{
	fs[0] = (FATFS*)mymalloc(sizeof(FATFS));	//Ϊ����0�����������ڴ�

	filescr = (FIL*)mymalloc(sizeof(FIL));		//Ϊ�ļ�1�����ڴ�
	filedst = (FIL*)mymalloc(sizeof(FIL));		//Ϊ�ļ�2�����ڴ�
	dir = (DIR*)mymalloc(sizeof(DIR));			//Ϊ�ļ��������ڴ�
	SDdatabuff = (uint8_t*)mymalloc(1024);		//ΪSD�����ݻ��������ڴ�
	
	if(fs[0]&&filescr&&filedst&&dir&&SDdatabuff)	return 0;	//������һ��ʧ��, ��ʧ��
	else	return 1;	//����ʧ��
}
/**********************************************************
* �������� ---> ��ӡSD����Ϣ������
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> 1����ӡ��������������
*               2����ӡ�������͵�����
*               3����ӡ����������Ϣ
**********************************************************/
void SD_Card_Printf_Info(void)
{
	switch(SDCardInfo.CardType)	//������
	{
		case SDIO_HIGH_CAPACITY_SD_CARD:	//��������
			printf("Card Type: SDHC V2.0\r\n");
			break;
			
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:	//��׼����V1.1
			printf("Card Type: SDSC V1.1\r\n");
			break;
			
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:	//��׼����V2.0
			printf("Card Type: SDSC V2.0\r\n");
			break;

		case SDIO_MULTIMEDIA_CARD:	//MMC��
			printf("Card Type: MMC Card\r\n");
			break;
	}

	printf("Card ManufacturerID: %d\r\n",SDCardInfo.SD_cid.ManufacturerID);			//������ID
 	printf("Card RCA: %d\r\n",SDCardInfo.RCA);										//����Ե�ַ
	printf("Card Capacity: %d MB\r\n",(uint32_t)SDCardInfo.CardCapacity);	//��ʾ����
 	printf("Card BlockSize: %d\r\n\r\n",SDCardInfo.CardBlockSize);					//��ʾ���С
}

	u8 *fn; 
u32 TLen=0X10;

void InitSD(void)
{
//	u8 SDtatus;	//SD����ʼ��״̬	
	
	GUI_PutStringAt("Software Compiled Time:" ,30,30);	//��ȡ�������ʱ��	
	GUI_PutStringAt(__DATE__ ,30,50);	//��ȡ�������ʱ��		
	GUI_PutStringAt(__TIME__ ,130,50);	//��ȡ�������ʱ��		
	GUI_PutStringAt("SD card Initialize ......",60,70);	
	
	myf_init();	//Ϊ�ļ�ϵͳ�����ڴ�

	/******************************************************
	                      ��ʾ������Ϣ
	                     ��LCD������ʼ 
	******************************************************/


	printf("Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//��ȡ�������ʱ��

	/*******************���������ķָ���******************/
	
	/******************************************************
	                      SD����ʼ�� 
	******************************************************/
//		SDtatus = SD_Init();	
//	if(SD_Init() != SD_OK)	//��ʼ��ʧ��
//	{
//		printf("The Faild is:%d\r\n", SDtatus);
//		GUI_PutStringAt("SD Card Init Faild" ,60,90);	//��ȡ�������ʱ��	

//	}
//	else	//��ʼ���ɹ���
//	{
//		SD_Card_Printf_Info();	//��ӡ����Ϣ������
//		GUI_PutStringAt("SD Card Init OK" ,60,90);			

//	}
	f_res = f_mount(0, fs[0]);	//����SD��			
	
	f_res = f_open(filescr,(const TCHAR *) GUI.Font->GBFont_P, FA_OPEN_EXISTING | FA_READ);	
	
	if(f_res == FR_OK)	//���ļ��ɹ�
	{
		GUI_PutStringAt("GBK�ֿ������",70,110);	
	}
	else	//��ʧ��
	{
		GUI_PutStringAt("GBK12.FON open False",70,110);			
	}	
	f_res = f_close(filescr);	//�ر��ļ�

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
		len = 512;	//��ȡ�ļ�����	
		}
		else
		{
			len =filescr->fsize - FileRead;
		}
		
		f_res = f_read(filescr, SDdatabuff, len, &br);	//��ȡ�ļ�����			
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












