/*********************************************************************************
* 文件名			: menu.C
* 版本号    	: V1.0.0
* 日期     	: 2015/07/14
* 说明     	: 处理与菜单有关的函数					  
* 用法			：
* 版本说明		：
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
函数：InitializeCtrl
功能：初始化CTRL任务
参数：无
返回：无
********************************************/
void InitializeFile(void)
{

}


/********************************************
函数：DK_File_Start
功能：雕刻机脱机SD卡文件开始雕刻
参数：无
返回：无
********************************************/
void DK_File_Start(void)
{
//	NRF_GCode_Struct *NRF_GCode_P;

	DK.N = 0;
	while(DK.File_B_P->NotEnd)
	{
		if(DK.Ctrl == CTRL_STOP)		return;		//停止
		if(DK.Alarm)	return;		//有故障退出自动
		if(NRF.Send == 0)	//等待上次发送完成
		{		
			if(DK.NRF.queue < 5)	//从机的队列小于5个
			{
				if(!ReadOneLine())		//正常读到数据
				{
					if(!TransformGCode(OneLine,&G_Code)) //数据解析正常
					{		
						ShowRun(&G_Code,Red);
						Send_GCode(&G_Code);
					}		
				}	
				else
				{
					//sd卡数据读取错误
				}
			}
			else
			{
					Send_DK_Action(NRF_CX_STATE);			//查询DK状态;
			}
		}
		else
		{
			OS.DelayTimeTick(5);		
		}
	}
}
/********************************************
函数：FileLoad
功能：文件读取
参数：message：需要读取的文件信息
返回：无
********************************************/
void FileLoad(u32 message)
{
	u8* Path;
	GUI_FileListStruct *FL_T;
	u8 chgchar[2]={0X5C,0X00};//转义符 等效"\"   
	GCodeStruct GCode_One;	
	FL_T = (GUI_FileListStruct *)((message&0xffffff)|RAM_ADDRESS);	//获取文件名信息
	//需增加文件类型判断

	Path = mymalloc(sizeof(FL_T->path)+sizeof(FL_T->fname));		
	strcpy((char*)Path,(const char*)FL_T->path);	//拷贝path到pname里面
	strcat((char*)Path,(const char*)chgchar);	//添加转义符
	strcat((char*)Path,(const char*)FL_T->fname);		//添加新增的名字
	f_res = f_open(filescr, (const TCHAR*)Path, FA_OPEN_DEFAULT);	//打开文件	
	myfree(Path);	//释放内存		
	if(f_res == FR_OK)									//读取文件正常
	{
		DK.File_B_P->Open =1;				//文件打开标志
		DK.File_B_P->NeedRead = 1;		//文件需要继续读取
		DK.File_B_P->NotEnd = 1;				//设置文件没有结束标志
		FileRead = 0;												//
		FileBuffNum = 0;		
		Page_DK_File();
			ReadFile();													//读取数据到缓存中
			DK.File_B_P->ReadStart = 1;		//文件开始读取标志		
			
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
		DK.File_B_P->Open =1;			//文件打开标志
		DK.File_B_P->NeedRead = 1;		//需要读取文件
		DK.File_B_P->NotEnd = 1;			
			
		ReadFile();													//读取数据到缓存中
		DK.File_B_P->ReadStart = 1;	//文件开始读取标志		
		DK.Ctrl = CTRL_PAUSE;			//停止
		OS.Timer.Start(TimerMessageHandle,10,DK_File_Start);
	
	}
	else
	{
		DK.Ctrl = CTRL_STOP;			//停止	
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
			case FuntionMessageType:                    //函数运行消息
				Function(message);
				break;	
			case GUI_FL_CloseMessageType:                    //文件列表消息
					Init_MainPage();
					break;
			case GUI_FL_FuncMessageType:                    //文件列表消息
					FileLoad(message);

					break;	
			
			default:                                
				break;
		}

	}
}

