/*********************************************************************************
* 文件名		: menu.C
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 处理与菜单有关的函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/

#include "OS.h"
#include "app.h"
#include "system.h"
#include "..\..\System\Device\device_usart\device_usart.h"
#include <string.h>
#include "g_code.h"
#include "queue.h"
#include "motion_control.h"
/********************************************
函数：InitializeComm
功能：初始化Comm任务
参数：无
返回：无
********************************************/
void InitializeComm(void)
{
	InitializeGCodeQueue();
	SetUsartMessagePointer(ComTaskPriority);	
//	Send_Buffer[0] = 4;			//4个字节
//	Send_Buffer[1] = 'o';
//	Send_Buffer[2] = 'k';	
//	Send_Buffer[3] = 0X0d;
//	Send_Buffer[4] = 0X0a;		
	
}

u8 CommReadOneLine(u8 *Rcv_Buff_P,u16 *Num,u8* OneLine_P)
{
	u8 i=0;
	u8 k= *Num;
	u16 Len=*Rcv_Buff_P;
	Rcv_Buff_P= Rcv_Buff_P+k+1;		//数据是从Rcv_Buff_P[1]开始的
	while(1)
	{
		if((*Rcv_Buff_P>0x40)&&(*Rcv_Buff_P<0x52))	//A~Z
		{
			while(1)
			{				
				i++;		
				if((Len<	k)||(i>=64))
				{			
					OneLine_P[0] = 0;		
					*Num = k;
					return 1;	
				}				
				OneLine_P[i] = *Rcv_Buff_P;				
				Rcv_Buff_P++;
				k++;
				if((OneLine_P[i]==0x0A)||(OneLine_P[i]==0x0D))
				{			
					OneLine_P[0] = i;
					OneLine_P[i]=0x0A;
					*Num = k;
					return 0;		
				}			

			}
		}
		else
		{
			k++;	
			Rcv_Buff_P++;			
			if(Len<=	*Num)
			{			
				OneLine_P[0] = 0;
					*Num = k;
				return 1;	
			}				
		}
	}			
}



void SendUsart1(void)
{
	if(GetBit(DKState, COMM_CONNECT))		//开始联机标志
	{
		strcpy((char*)&Send_Buffer[1],(const char*)"\r\nGrbl 0.9 ['$' for help]\r\n");	//添加转义符
		Send_Buffer[0] =27; 		
	}
	else if(GetBit(DKState, COMM_STATE_OK))		//状态OK
	{
		strcpy((char*)&Send_Buffer[1],(const char*)"OK\r\n");	//添加转义符
		Send_Buffer[0] =4; 			
	}
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);//开启数据发送中断	
}


void UsartMessage(void)
{	
	u16 Num=0;
CommReadOneLine(Rcv_Buffer,&Num,OneLine);
	TransformGCode(OneLine,GCodeQueue.In);
	SetBit(DKState,COMM_STATE_OK);	
	ResetBit(DKState,COMM_CONNECT);		
	AddGCodeQueueEntries();	
	SendUsart1();
}
void CommConnect(void)
{
	if((GetBit(DKState,COMM_CONNECT))/*&&(GetBit(DKState,COMM_STATE_OK))*/)
	{
		SendUsart1();
		OS.Timer.Start(TimerMessageHandle,200,CommConnect);		//定时发送联机信息		
	}


}

void CommTask(void)
{
	u32 message;

	u32 Key;
	InitializeComm();
				SetBit(DKState, COMM_START);
				SetBit(DKState, COMM_CONNECT);
			
				CommConnect();			
					
	while(1)
	{    		 
		message = (u32)OS.PendMessageQueue(0);
		
		if(!DKAlarm	)	
		{			
//			if((GetBit(DKState,CTRL_START)&&GetBit(DKState,CTRL_PAUSH)))
//			{
				if(GCodeQueue.Entries!=0)
				{		
//					if(GetBit(DKState,CTRL_MOTORUN))
//					{
						Interpolation_Process(GCodeQueue.Out);
//					}
//					if((GetBit(DKState,CTRL_SHOWRUN))	|| (GetBit(DKState,CTRL_MOTORUN)))		
//					{
						SubGCodeQueueEntries();
//					}					
				}								
//			}
		}		
		
		
		
		
		
		switch(GetMessageType(message))
		{
			case KeyMessageType:    							//按键消息
							Key = GetMessageData(message);
						switch (Key &0xE0)
						{
							case 0XE0:
							
								break;
							case 0X60:

								break;
							case 0X40:
	
								break;
							default:
								break;
						}

				break;
			case UsartMessageType:  
				if(GetBit(DKState, COMM_START))		//开始联机标志
				{
					UsartMessage();					
				}
				break;						
			case FuntionMessageType:                    //函数运行消息
				Function(message);
				break;	
			case UseMessage_CommConnect:  
				SetBit(DKState, COMM_START);
				SetBit(DKState, COMM_CONNECT);
			
				CommConnect();
				break;

			
			default:                                
				break;
		}

	}
}





