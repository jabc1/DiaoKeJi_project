/*********************************************************************************
* �ļ���		: menu.C
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ������˵��йصĺ���					  
* �÷�			��
* �汾˵��	��
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
������InitializeComm
���ܣ���ʼ��Comm����
��������
���أ���
********************************************/
void InitializeComm(void)
{
	InitializeGCodeQueue();
	SetUsartMessagePointer(ComTaskPriority);	
//	Send_Buffer[0] = 4;			//4���ֽ�
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
	Rcv_Buff_P= Rcv_Buff_P+k+1;		//�����Ǵ�Rcv_Buff_P[1]��ʼ��
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
	if(GetBit(DKState, COMM_CONNECT))		//��ʼ������־
	{
		strcpy((char*)&Send_Buffer[1],(const char*)"\r\nGrbl 0.9 ['$' for help]\r\n");	//���ת���
		Send_Buffer[0] =27; 		
	}
	else if(GetBit(DKState, COMM_STATE_OK))		//״̬OK
	{
		strcpy((char*)&Send_Buffer[1],(const char*)"OK\r\n");	//���ת���
		Send_Buffer[0] =4; 			
	}
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);//�������ݷ����ж�	
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
		OS.Timer.Start(TimerMessageHandle,200,CommConnect);		//��ʱ����������Ϣ		
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
			case KeyMessageType:    							//������Ϣ
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
				if(GetBit(DKState, COMM_START))		//��ʼ������־
				{
					UsartMessage();					
				}
				break;						
			case FuntionMessageType:                    //����������Ϣ
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





