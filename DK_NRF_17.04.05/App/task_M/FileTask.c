/*********************************************************************************
* �ļ���			: menu.C
* �汾��    	: V1.0.0
* ����     	: 2015/07/14
* ˵��     	: ������˵��йصĺ���					  
* �÷�			��
* �汾˵��		��
**********************************************************************************/
#include "TFT_control.h"
#include "OS.h"
#include "app.h"
#include "ff.h"
#include "page.h"
#include "malloc.h"	
#include "SD.h"
#include "g_code.h"
#include "NRF_CTRL.h"

/********************************************
������InitializeCtrl
���ܣ���ʼ��CTRL����
��������
���أ���
********************************************/
void InitializeFile(void)
{

}


/********************************************
������DK_File_Start
���ܣ���̻��ѻ�SD���ļ���ʼ���
��������
���أ���
********************************************/
void DK_File_Start(void)
{
//	NRF_GCode_Struct *NRF_GCode_P;

	DK.N = 0;
	while(DK.File_B_P->NotEnd)
	{
		if(DK.Ctrl == CTRL_STOP)		return;		//ֹͣ
		if(DK.Alarm)	return;		//�й����˳��Զ�
		if(NRF.Send == 0)	//�ȴ��ϴη������
		{		
			if(DK.NRF.queue < 5)	//�ӻ��Ķ���С��5��
			{
				if(!ReadOneLine())		//������������
				{
					if(!TransformGCode(OneLine,&G_Code)) //���ݽ�������
					{		
						ShowRun(&G_Code,Red);
						Send_GCode(&G_Code);
					}		
				}	
				else
				{
					//sd�����ݶ�ȡ����
				}
			}
			else
			{
					Send_DK_Action(NRF_CX_STATE);			//��ѯDK״̬;
			}
		}
		else
		{
			OS.DelayTimeTick(5);		
		}
	}
}
/********************************************
������FileLoad
���ܣ��ļ���ȡ
������message����Ҫ��ȡ���ļ���Ϣ
���أ���
********************************************/
void FileLoad(u32 message)
{
	u8* Path;
	GUI_FileListStruct *FL_T;
	u8 chgchar[2]={0X5C,0X00};//ת��� ��Ч"\"   
	GCodeStruct GCode_One;	
	FL_T = (GUI_FileListStruct *)((message&0xffffff)|RAM_ADDRESS);	//��ȡ�ļ�����Ϣ
	//�������ļ������ж�

	Path = mymalloc(sizeof(FL_T->path)+sizeof(FL_T->fname));		
	strcpy((char*)Path,(const char*)FL_T->path);	//����path��pname����
	strcat((char*)Path,(const char*)chgchar);	//���ת���
	strcat((char*)Path,(const char*)FL_T->fname);		//�������������
	f_res = f_open(filescr, (const TCHAR*)Path, FA_OPEN_DEFAULT);	//���ļ�	
	myfree(Path);	//�ͷ��ڴ�		
	if(f_res == FR_OK)									//��ȡ�ļ�����
	{
		DK.File_B_P->Open =1;				//�ļ��򿪱�־
		DK.File_B_P->NeedRead = 1;		//�ļ���Ҫ������ȡ
		DK.File_B_P->NotEnd = 1;				//�����ļ�û�н�����־
		FileRead = 0;												//
		FileBuffNum = 0;		
		Page_DK_File();
			ReadFile();													//��ȡ���ݵ�������
			DK.File_B_P->ReadStart = 1;		//�ļ���ʼ��ȡ��־		
			
			while(DK.File_B_P->NotEnd)
			{
				if(ReadOneLine()==0)
				{
					TransformGCode(OneLine,&GCode_One);	
					ShowRun(&GCode_One,Green);				
				}		
			}			
		f_res = f_lseek(filescr, 0); 
		FileRead = 0;	
		FileBuffNum = 0;	
		DK.File_B_P->Open =1;			//�ļ��򿪱�־
		DK.File_B_P->NeedRead = 1;		//��Ҫ��ȡ�ļ�
		DK.File_B_P->NotEnd = 1;			
			
		ReadFile();													//��ȡ���ݵ�������
		DK.File_B_P->ReadStart = 1;	//�ļ���ʼ��ȡ��־		
		DK.Ctrl = CTRL_PAUSE;			//ֹͣ
		OS.Timer.Start(TimerMessageHandle,10,DK_File_Start);
	
	}
	else
	{
		DK.Ctrl = CTRL_STOP;			//ֹͣ	
	}	
}


void FileTask(void)
{
	u32 message;
	InitializeFile();
	
	while(1)
	{    		 
		message = (u32)OS.PendMessageQueue(0);
		switch(GetMessageType(message))
		{
			case FuntionMessageType:                    //����������Ϣ
				Function(message);
				break;	
			case GUI_FL_CloseMessageType:                    //�ļ��б���Ϣ
					Init_MainPage();
					break;
			case GUI_FL_FuncMessageType:                    //�ļ��б���Ϣ
					FileLoad(message);

					break;	
			
			default:                                
				break;
		}

	}
}

