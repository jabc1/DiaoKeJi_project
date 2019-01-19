#ifndef __DEVICE_USART_H_
#define __DEVICE_USART_H_

#include "stm32f10x.h" 


//u8 Rcv_Complete;//一帧是否已经接受完成
//u8 Send_Complete;//一帧是否已经发送完成
//u8 Com_busy;//通讯繁忙，表示上一帧还未处理结束
extern u8 Rcv_Buffer[201];//用来存放接收到的完整的一帧数据	（第一个字节用来存放接收到的有效字节数，也就是数组中的有效字节数）					
extern u8 Send_Buffer[40];//用来存放待发送的完整的一帧数据（第一个字节用来存放待发送的有效字节数，也就是数组中的有效字节数）
//u8 Rcv_Data;//用来存放接收的一个字节
//u8 Send_Data;//用来存放要发送的一字节
//u8 Rcv_Num;//用来表示接收的一帧的有效字节数（从功能码到CRC校验）
//u8 Send_Num;//用来表示待发送的一帧的字节数																				
//u8 *PointToRcvBuf;//用来指向接收的数据缓存
//u8 *PointToSendBuf;//用来指向带发送的数据缓存
//u8 Comu_Busy;//用来表示能否接收下一帧数据
//u8 HaveMes;
//u16 HoldReg[100];

void InitializeUsart(void);
void SetUsartMessagePointer(u8 Pointer);
#endif

