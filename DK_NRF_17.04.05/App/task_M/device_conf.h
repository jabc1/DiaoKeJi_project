
#ifndef __DEVICE_CONF_H
#define __DEVICE_CONF_H
#include "device.h"
	#define USE_MSOS						1
	#define	USE_GUI							1
	
/************************开发板的选择******************************************/
	#define USE_I_DRV    				1			//神州I号	STM32F103RBT6
	#define USE_III_DRV					2			//神州III号	STM32F103ZET6
	#define USE_MIN_DRV					3			//最小系统板 STM32F103ZET6
	#define USE_C8T6_DRV				4			//最小系统板 STM32F103C8T6
	
	#define USE_DRV							USE_III_DRV		//当前使用开发板	

/*********************按照我自己使用的3个LCD屏分别定义***************************/
	#define LCD_USE_FMSC		1
	#define	LCD_USE_SPI			2
	#define	LCD_USE_16B			3
	#define LCDAPIUSEFAST		1				//为1时可以加快字符显示速度，为0时使用打点来显示字符
	#define LCD_MODE				1				// 0: 横屏1；1：横屏2；2：竖屏1；3：竖屏2
	#define LCD_USE					LCD_USE_FMSC
/***********************触摸屏的函数定义**************************************************/
#if (LCD_USE == LCD_USE_FMSC) || (LCD_USE == LCD_USE_16B) 
	#define TOUCHDEVICE			1				//具有touch的硬件
#else
	#define TOUCHDEVICE			0				//无touch的硬件	
#endif



/*************************键盘类型选择********************************************/
#define ISUSEKEYBOARD					0					//是否使用矩阵键盘
#define ISUSESEPARATEKEY			1					//是否使用独立按键
#if(USE_MSOS == 0)
	#define PostKey(Key) 				(void)0
#endif
/**********************触摸屏点击时间间隔***************************************************/
	#define	 TOUCH_DELAY_TIME1 		25		 //点击后第一次延时时间  *20ms
	#define	 TOUCH_DELAY_TIME2 		10		 //点击后第二次延时时间
	#define		DoubleHitInterval		10
	
	
	
/*************************包含的驱动的头文件***************************************/		
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

	//IO口操作,只对单一的IO口!确保n的值小于16!
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







