#ifndef __DEVICE_USART_H_
#define __DEVICE_USART_H_

#include "stm32f10x.h" 


//u8 Rcv_Complete;//һ֡�Ƿ��Ѿ��������
//u8 Send_Complete;//һ֡�Ƿ��Ѿ��������
//u8 Com_busy;//ͨѶ��æ����ʾ��һ֡��δ�������
extern u8 Rcv_Buffer[201];//������Ž��յ���������һ֡����	����һ���ֽ�������Ž��յ�����Ч�ֽ�����Ҳ���������е���Ч�ֽ�����					
extern u8 Send_Buffer[40];//������Ŵ����͵�������һ֡���ݣ���һ���ֽ�������Ŵ����͵���Ч�ֽ�����Ҳ���������е���Ч�ֽ�����
//u8 Rcv_Data;//������Ž��յ�һ���ֽ�
//u8 Send_Data;//�������Ҫ���͵�һ�ֽ�
//u8 Rcv_Num;//������ʾ���յ�һ֡����Ч�ֽ������ӹ����뵽CRCУ�飩
//u8 Send_Num;//������ʾ�����͵�һ֡���ֽ���																				
//u8 *PointToRcvBuf;//����ָ����յ����ݻ���
//u8 *PointToSendBuf;//����ָ������͵����ݻ���
//u8 Comu_Busy;//������ʾ�ܷ������һ֡����
//u8 HaveMes;
//u16 HoldReg[100];

void InitializeUsart(void);
void SetUsartMessagePointer(u8 Pointer);
#endif

