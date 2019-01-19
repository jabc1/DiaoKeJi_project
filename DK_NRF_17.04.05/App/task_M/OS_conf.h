#ifndef __OS_CONF_H
#define __OS_CONF_H

	/*TaskSum必须要跟实际配套,实际的任务数必须小于TaskSum，否则出现异常
	如果TaskSum 大于实际任务数，则大于部分的RAM是浪费的，但程序可以运行
	为了简化定义，每个任务的任务栈深度和消息队列深度改为相同的大小
	*/
	#define TaskStackSumIsSame		0						 /*是否使用相同数量的堆栈*/
	#define TaskSum     				8           /*任务总数，最大为8个*/
	#define QueueStackSum			20					//消息队列深度	
		
	#if TaskStackSumIsSame == 0	
		#define TaskStackSum0   		200         //任务栈深度
		#define TaskStackSum1   		200         //任务栈深度
		#define TaskStackSum2   		200         //任务栈深度
		#define TaskStackSum3   		200         //任务栈深度
		#define TaskStackSum4   		200         //任务栈深度
		#define TaskStackSum5   		64         //任务栈深度
		#define TaskStackSum6   		64         //任务栈深度
		#define TaskStackSum7   		64         //任务栈深度
	#endif
	#if TaskStackSumIsSame == 1	
		#define TaskStackSum   		200         //任务栈深度
	#endif
typedef enum
{
	KeyMessageType      	= 0xFF,
	
	TouchMessageType				= 0xFE,
	UsartMessageType			= 0xFD,
	FileListMessageType		= 0xFC,
	FileListCloseMessageType		= 0xFB,	
	/*请填充类型*/
	#ifdef  VECT_TAB_RAM  
		FuntionMessageType	= 0x20,
		AddressType = 0x20,
	#else  /* VECT_TAB_FLASH  */
		FuntionMessageType	= 0x08,
		AddressType = 0x08,
	#endif 
	//自己定义的消息类型 0XE0~0XEF
	TimerMessageType    = 0xF0  /*最小值,不允许有值小于它*/
}MessageType;

	#define GetMessageType(data)            ((u8 *)(&(data)))[3]
	#define GetMessageData(data)            (data & 0x00FFFFFF)
			   
#endif




