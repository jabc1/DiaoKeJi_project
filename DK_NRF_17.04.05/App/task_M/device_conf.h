
#ifndef __DEVICE_CONF_H
#define __DEVICE_CONF_H
#include "device.h"
	#define USE_MSOS						1
	#define	USE_GUI							1
	
/************************�������ѡ��******************************************/
	#define USE_I_DRV    				1			//����I��	STM32F103RBT6
	#define USE_III_DRV					2			//����III��	STM32F103ZET6
	#define USE_MIN_DRV					3			//��Сϵͳ�� STM32F103ZET6
	#define USE_C8T6_DRV				4			//��Сϵͳ�� STM32F103C8T6
	
	#define USE_DRV							USE_III_DRV		//��ǰʹ�ÿ�����	

/*********************�������Լ�ʹ�õ�3��LCD���ֱ���***************************/
	#define LCD_USE_FMSC		1
	#define	LCD_USE_SPI			2
	#define	LCD_USE_16B			3
	#define LCDAPIUSEFAST		1				//Ϊ1ʱ���Լӿ��ַ���ʾ�ٶȣ�Ϊ0ʱʹ�ô������ʾ�ַ�
	#define LCD_MODE				1				// 0: ����1��1������2��2������1��3������2
	#define LCD_USE					LCD_USE_FMSC
/***********************�������ĺ�������**************************************************/
#if (LCD_USE == LCD_USE_FMSC) || (LCD_USE == LCD_USE_16B) 
	#define TOUCHDEVICE			1				//����touch��Ӳ��
#else
	#define TOUCHDEVICE			0				//��touch��Ӳ��	
#endif



/*************************��������ѡ��********************************************/
#define ISUSEKEYBOARD					0					//�Ƿ�ʹ�þ������
#define ISUSESEPARATEKEY			1					//�Ƿ�ʹ�ö�������
#if(USE_MSOS == 0)
	#define PostKey(Key) 				(void)0
#endif
/**********************���������ʱ����***************************************************/
	#define	 TOUCH_DELAY_TIME1 		25		 //������һ����ʱʱ��  *20ms
	#define	 TOUCH_DELAY_TIME2 		10		 //�����ڶ�����ʱʱ��
	#define		DoubleHitInterval		10
	
	
	
/*************************������������ͷ�ļ�***************************************/		
#include "device_RTC.h"
#include "device_key.h"
#include "device_LED.h"
#include "device_SPI.h"
//#include "device_ADC.h"
//#include "device_pwm.h"
//#include "device_TIM.h"
#include "device_usart.h"
#include "device_sd_sdio.h"
#include "device_NRF24l01.h"
#include "device_LCD.h"
#include "device_touch.h"


	#define GPIOA_IDR_ADDR    (GPIOA_BASE+8) //0x40010808 
	#define GPIOB_IDR_ADDR    (GPIOB_BASE+8) //0x40010C08 
	#define GPIOC_IDR_ADDR    (GPIOC_BASE+8) //0x40011008 
	#define GPIOD_IDR_ADDR    (GPIOD_BASE+8) //0x40011408 
	#define GPIOE_IDR_ADDR    (GPIOE_BASE+8) //0x40011808 
	#define GPIOF_IDR_ADDR    (GPIOF_BASE+8) //0x40011A08 
	#define GPIOG_IDR_ADDR    (GPIOG_BASE+8) //0x40011E08 

	#define GPIOA_ODR_ADDR    (GPIOA_BASE+12) //0x4001080C 
	#define GPIOB_ODR_ADDR    (GPIOB_BASE+12) //0x40010C0C 
	#define GPIOC_ODR_ADDR    (GPIOC_BASE+12) //0x4001100C 
	#define GPIOD_ODR_ADDR    (GPIOD_BASE+12) //0x4001140C 
	#define GPIOE_ODR_ADDR    (GPIOE_BASE+12) //0x4001180C 
	#define GPIOF_ODR_ADDR    (GPIOF_BASE+12) //0x40011A0C    
	#define GPIOG_ODR_ADDR    (GPIOG_BASE+12) //0x40011E0C    

	//IO�ڲ���,ֻ�Ե�һ��IO��!ȷ��n��ֵС��16!
	#define BIT_BAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
	#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
	#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BIT_BAND(addr, bitnum)) 

	#define	BitAddr(addr)   (vu32*)((((u32)(addr) & 0xF0000000)+0x2000000+((u32)(addr)<<5)));

	#define PAIn(n)    BIT_ADDR(GPIOA_IDR_ADDR,n)
	#define PAOut(n)   BIT_ADDR(GPIOA_ODR_ADDR,n)
	#define PBIn(n)    BIT_ADDR(GPIOB_IDR_ADDR,n)
	#define PBOut(n)   BIT_ADDR(GPIOB_ODR_ADDR,n)
	#define PCIn(n)    BIT_ADDR(GPIOC_IDR_ADDR,n)
	#define PCOut(n)   BIT_ADDR(GPIOC_ODR_ADDR,n)
	#define PDIn(n)    BIT_ADDR(GPIOD_IDR_ADDR,n)
	#define PDOut(n)   BIT_ADDR(GPIOD_ODR_ADDR,n)
	#define PEIn(n)    BIT_ADDR(GPIOE_IDR_ADDR,n)
	#define PEOut(n)   BIT_ADDR(GPIOE_ODR_ADDR,n)
	#define PFIn(n)    BIT_ADDR(GPIOF_IDR_ADDR,n)
	#define PFOut(n)   BIT_ADDR(GPIOF_ODR_ADDR,n)
	#define PGIn(n)    BIT_ADDR(GPIOG_IDR_ADDR,n)
	#define PGOut(n)   BIT_ADDR(GPIOG_ODR_ADDR,n)

	#define	GPIO_IN(Port,Pin) 	GPIO_ReadInputDataBit(((GPIO_TypeDef *)Port), Pin)
#endif







