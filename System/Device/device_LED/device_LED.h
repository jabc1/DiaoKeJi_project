#ifndef __DEVICE_LED_H_
#define __DEVICE_LED_H_

#include "stm32f10x.h" 
#include "Device_conf.h" 

#if 	(USE_DRV ==	USE_I_DRV)				/*����I�Ű�����ض���*/
	#define LED1	PAOut(2)
	#define LED2	PBOut(2)
	#define LED3	PAOut(3)
#endif
#if 	(USE_DRV ==	USE_III_DRV)		/*����III�Ű�����ض���*/
	#define LED1	PFOut(6)
	#define LED2	PFOut(7)
	#define LED3	PFOut(8)
	#define LED4	PFOut(9)
#endif
#if 	(USE_DRV ==	USE_MIN_DRV)			/*��Сϵͳ������ض���*/
	#define LED1	PDOut(13)
	#define LED2	PGOut(14)
#endif
#if 	(USE_DRV ==	USE_C8T6_DRV)			/*��Сϵͳ������ض���*/
	#define LED1	PCOut(13)
#endif	
	





void InitializeLED(void);

#endif

