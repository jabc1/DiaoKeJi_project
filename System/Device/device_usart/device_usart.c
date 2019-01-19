/*

********************************************************************************
*/


#include "device_usart.h"
#include "os.h"
#define TIM_ID   3

u8 UsartMessageQueuePointer = 0;			//Ĭ����������ȼ�����



//modbus��ͨѶ����
u8 Tim_Out;//����3.5���ַ�ʱ�䣬����ȡ3ms (������9600��ʱ���Լ2�㼸����)
u8 Rcv_Complete;//һ֡�Ƿ��Ѿ��������
u8 Send_Complete;//һ֡�Ƿ��Ѿ��������
u8 Com_busy=0;//ͨѶ��æ����ʾ��һ֡��δ�������
u8 Rcv_Buffer[201];//������Ž��յ���������һ֡����	����һ���ֽ�������Ž��յ�����Ч�ֽ�����Ҳ���������е���Ч�ֽ�����					
u8 Send_Buffer[40];//������Ŵ����͵�������һ֡���ݣ���һ���ֽ�������Ŵ����͵���Ч�ֽ�����Ҳ���������е���Ч�ֽ�����
u8 Rcv_Data;//������Ž��յ�һ���ֽ�
u8 Send_Data;//�������Ҫ���͵�һ�ֽ�
u8 Rcv_Num;//������ʾ���յ�һ֡����Ч�ֽ������ӹ����뵽CRCУ�飩
u8 Send_Num;//������ʾ�����͵�һ֡���ֽ���																				
u8 Comu_Busy;//������ʾ�ܷ������һ֡����
u8 HaveMes;
//u16 HoldReg[100];



 /* �������ܣ�����Modbus��Ϣ����
	�������룺�ޡ�														
	����������ޡ�																	   */
void ModSend(void)	
 {	
	Send_Num++;	 

	USART_SendData(USART1,Send_Buffer[Send_Num]);

	if(Send_Num>=Send_Buffer[0])//�����Ѿ����
	{
		Comu_Busy=0;
		Send_Num = 0;
		Rcv_Num=0;
		//�������ݷ���
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);//�ر����ݷ����ж�	
	}
 }

/*******************************************************************************
* ������	: SetKeyMessagePointer
* ����	    : ���õ�ǰ����ΪKEY���͵�����ָ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SetUsartMessagePointer(u8 Pointer)
{
	UsartMessageQueuePointer = Pointer;		//�����
}					

#define DMA_Rec_Len 100
     
//���»ָ�DMAָ��
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_CHx,DMA_Rec_Len);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	



//�����жϺ���
void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u32 UsartMessage;		
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ReceiveData(USART1);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
		Rcv_Buffer[0] = DMA_Rec_Len-DMA_GetCurrDataCounter(DMA1_Channel5);	//����ӱ�֡���ݳ���
		UsartMessage = UsartMessageType;
		OS.PostMessageQueue(UsartMessageQueuePointer,(void *)((UsartMessage<<24)));

		//*************************************//
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //����жϱ�־
		MYDMA_Enable(DMA1_Channel5);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���
	 } 
	if(USART_GetITStatus(USART1,USART_IT_TC)!=RESET)
		{
			ModSend();
		}	
} 


void InitUsart1DMA(u8* DMA_Rece_Buf) 
{
		  DMA_InitTypeDef DMA_InitStructure;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//���������ж�
		USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   //ʹ�ܴ���1 DMA����	
    //��Ӧ��DMA����
		DMA_DeInit(DMA1_Channel5);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA����ADC����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Rece_Buf;  //DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
		DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMAͨ����DMA����Ĵ�С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
				
    DMA_Cmd(DMA1_Channel5, ENABLE);  //��ʽ����DMA����
}















void InitializeUsart(void) 
{
//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
//USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
  
//USART ��ʼ������
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
   
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

InitUsart1DMA(&Rcv_Buffer[1]);
}


/*******************************************************************************
* ������	: fputc
* ����	    : Keil������֧��printf��������Ҫ���ô˺�����ע�������ҪMicorLib֧��
* �������  : ch ʵΪbyte���ͣ�ʵ�ʷ��͵�����
*           : f �豸���������Ҫ����
* ���ز���  : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/10       ��ʼ�汾
*******************************************************************************/
//int fputc(int ch, FILE *f)
//{
//    if (ch == '\n')
//    {
//        while(!WriteByteToUsart1('\r'));
//    }

//    while(!WriteByteToUsart1((u8)ch));

//    return ch;
//}
