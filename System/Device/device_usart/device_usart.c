/*

********************************************************************************
*/


#include "device_usart.h"
#include "os.h"
#define TIM_ID   3

u8 UsartMessageQueuePointer = 0;			//默认是最高优先级运行



//modbus用通讯参数
u8 Tim_Out;//大于3.5个字符时间，保守取3ms (波特率9600的时候大约2点几毫秒)
u8 Rcv_Complete;//一帧是否已经接受完成
u8 Send_Complete;//一帧是否已经发送完成
u8 Com_busy=0;//通讯繁忙，表示上一帧还未处理结束
u8 Rcv_Buffer[201];//用来存放接收到的完整的一帧数据	（第一个字节用来存放接收到的有效字节数，也就是数组中的有效字节数）					
u8 Send_Buffer[40];//用来存放待发送的完整的一帧数据（第一个字节用来存放待发送的有效字节数，也就是数组中的有效字节数）
u8 Rcv_Data;//用来存放接收的一个字节
u8 Send_Data;//用来存放要发送的一字节
u8 Rcv_Num;//用来表示接收的一帧的有效字节数（从功能码到CRC校验）
u8 Send_Num;//用来表示待发送的一帧的字节数																				
u8 Comu_Busy;//用来表示能否接收下一帧数据
u8 HaveMes;
//u16 HoldReg[100];



 /* 函数功能：用于Modbus信息发送
	函数输入：无。														
	函数输出：无。																	   */
void ModSend(void)	
 {	
	Send_Num++;	 

	USART_SendData(USART1,Send_Buffer[Send_Num]);

	if(Send_Num>=Send_Buffer[0])//发送已经完成
	{
		Comu_Busy=0;
		Send_Num = 0;
		Rcv_Num=0;
		//启动数据发送
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);//关闭数据发送中断	
	}
 }

/*******************************************************************************
* 函数名	: SetKeyMessagePointer
* 描述	    : 设置当前任务为KEY发送的任务指针
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void SetUsartMessagePointer(u8 Pointer)
{
	UsartMessageQueuePointer = Pointer;		//需更改
}					

#define DMA_Rec_Len 100
     
//重新恢复DMA指针
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA_CHx,DMA_Rec_Len);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	



//串口中断函数
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u32 UsartMessage;		
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ReceiveData(USART1);//读取数据 注意：这句必须要，否则不能够清除中断标志位。我也不知道为啥！
		Rcv_Buffer[0] = DMA_Rec_Len-DMA_GetCurrDataCounter(DMA1_Channel5);	//算出接本帧数据长度
		UsartMessage = UsartMessageType;
		OS.PostMessageQueue(UsartMessageQueuePointer,(void *)((UsartMessage<<24)));

		//*************************************//
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //清除中断标志
		MYDMA_Enable(DMA1_Channel5);                   //恢复DMA指针，等待下一次的接收
	 } 
	if(USART_GetITStatus(USART1,USART_IT_TC)!=RESET)
		{
			ModSend();
		}	
} 


void InitUsart1DMA(u8* DMA_Rece_Buf) 
{
		  DMA_InitTypeDef DMA_InitStructure;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启空闲中断
		USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   //使能串口1 DMA接收	
    //相应的DMA配置
		DMA_DeInit(DMA1_Channel5);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA外设ADC基地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Rece_Buf;  //DMA内存基地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
		DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMA通道的DMA缓存的大小
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
				
    DMA_Cmd(DMA1_Channel5, ENABLE);  //正式驱动DMA传输
}















void InitializeUsart(void) 
{
//GPIO端口设置
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

//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
//USART 初始化设置
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
   
    USART_Cmd(USART1, ENABLE);                    //使能串口 

InitUsart1DMA(&Rcv_Buffer[1]);
}


/*******************************************************************************
* 函数名	: fputc
* 描述	    : Keil编译器支持printf函数，需要调用此函数，注意这个需要MicorLib支持
* 输入参数  : ch 实为byte类型，实际发送的数据
*           : f 设备，这儿不需要处理
* 返回参数  : 
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/10       初始版本
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
