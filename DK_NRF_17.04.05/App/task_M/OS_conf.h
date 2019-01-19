#ifndef __OS_CONF_H
#define __OS_CONF_H

	/*TaskSum����Ҫ��ʵ������,ʵ�ʵ�����������С��TaskSum����������쳣
	���TaskSum ����ʵ��������������ڲ��ֵ�RAM���˷ѵģ��������������
	Ϊ�˼򻯶��壬ÿ�����������ջ��Ⱥ���Ϣ������ȸ�Ϊ��ͬ�Ĵ�С
	*/
	#define TaskStackSumIsSame		0						 /*�Ƿ�ʹ����ͬ�����Ķ�ջ*/
	#define TaskSum     				8           /*�������������Ϊ8��*/
	#define QueueStackSum			20					//��Ϣ�������	
		
	#if TaskStackSumIsSame == 0	
		#define TaskStackSum0   		200         //����ջ���
		#define TaskStackSum1   		200         //����ջ���
		#define TaskStackSum2   		200         //����ջ���
		#define TaskStackSum3   		200         //����ջ���
		#define TaskStackSum4   		200         //����ջ���
		#define TaskStackSum5   		64         //����ջ���
		#define TaskStackSum6   		64         //����ջ���
		#define TaskStackSum7   		64         //����ջ���
	#endif
	#if TaskStackSumIsSame == 1	
		#define TaskStackSum   		200         //����ջ���
	#endif
typedef enum
{
	KeyMessageType      	= 0xFF,
	
	TouchMessageType				= 0xFE,
	UsartMessageType			= 0xFD,
	FileListMessageType		= 0xFC,
	FileListCloseMessageType		= 0xFB,	
	/*���������*/
	#ifdef  VECT_TAB_RAM  
		FuntionMessageType	= 0x20,
		AddressType = 0x20,
	#else  /* VECT_TAB_FLASH  */
		FuntionMessageType	= 0x08,
		AddressType = 0x08,
	#endif 
	//�Լ��������Ϣ���� 0XE0~0XEF
	TimerMessageType    = 0xF0  /*��Сֵ,��������ֵС����*/
}MessageType;

	#define GetMessageType(data)            ((u8 *)(&(data)))[3]
	#define GetMessageData(data)            (data & 0x00FFFFFF)
			   
#endif




