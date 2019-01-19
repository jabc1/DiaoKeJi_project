/*********************************************************************************
* �ļ���			: device_LED.c
* �汾��    	: V2.1.1
* ����			: ZZ
* ����      : 2016/04/24
* ˵��      : �������LED����;  
* �÷�			��

**********************************************************************************/

#include "stm32f10x.h" 
#include "device_LED.h"
#include "device.h"
#if (USE_DRV ==	USE_I_DRV)				/*����I�Ű�����ض���*/
	#define LED1_GPIO		GPIOA
	#define LED1_GPIO		GPIOA	

	const IO_TypeDef	LED_IO[]={
		{RCC_A , GPIOA , GPIO_Pin_2},		
		{RCC_A , GPIOA , GPIO_Pin_3},	
		{RCC_B , GPIOB , GPIO_Pin_2},	
	};
	#define LEDNUM    3
#endif
#if 	(USE_DRV ==	USE_III_DRV)		/*����III�Ű�����ض���*/
	const IO_TypeDef	LED_IO[]={
		{RCC_F , GPIOF , GPIO_Pin_6},		
		{RCC_F , GPIOF , GPIO_Pin_7},	
		{RCC_F , GPIOF , GPIO_Pin_8},		
		{RCC_F , GPIOF , GPIO_Pin_9},		
	};	
	#define LEDNUM    4
#endif
#if 	(USE_DRV ==	USE_MIN_DRV)			/*��Сϵͳ������ض���*/
	const IO_TypeDef	LED_IO[]={
		{RCC_D , GPIOD , GPIO_Pin_13},		
		{RCC_G , GPIOG , GPIO_Pin_14},	
	};	
	#define LEDNUM    2
#endif
#if 	(USE_DRV ==	USE_C8T6_DRV)			/*��Сϵͳ������ض���*/
	const IO_TypeDef	LED_IO[]={
		{RCC_C , GPIOC , GPIO_Pin_13},		
	};	
	#define LEDNUM    1
#endif	
	
	
void InitializeLED(void)
{		
	u8 i;
	for (i=0;i<LEDNUM;i++){
		IO_OUT_PP_LOW(LED_IO[i].Port,LED_IO[i].Pin,LED_IO[i].Rcc);
		GPIO_SetBits(LED_IO[i].Port,LED_IO[i].Pin);
	}	
}	
	


