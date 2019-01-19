#ifndef __GUIDEVICE_H__
#define __GUIDEVICE_H__
#include "stm32f10x.h"
#include "device_conf.h"
/****************************TOUCH的函数定义**********************************************************************************************/
void InitializeTouch(void);
void Touch_Rd_Addata(u16 *X_Addata,u16 *Y_Addata);
u8 IsTouch(void);
	void TouchSystick200Routine(void);

#define GPIO_ADS7843_INT_VALID   !GPIO_ReadInputDataBit(GPIO_ADS7843_INT_PORT, GPIO_ADS7843_INT)
#define ADS7843_CS_LOW()     GPIO_ResetBits(GPIO_ADS7843_CS_PORT, GPIO_ADS7843_CS)
#define ADS7843_CS_HIGH()    GPIO_SetBits(GPIO_ADS7843_CS_PORT, GPIO_ADS7843_CS)

u16 _AD320(u16 adx);
u16 _AD240(u16 ady);
u16 ADS7843_Read(void);


#if (USE_MSOS == 1)
	#define PostTouch(TouchMessage) 		PostTouchMessageQueue(TouchMessage) 
#else
	#define PostTouch(TouchMessage)  		(void)0 				//不用MSOS的时候改为对应的调用函数
#endif






	#if 	LCD_USE == LCD_USE_16B  
		#define TOUCH_SPI 	SPI1	
		#define TOUCH_CS_PIN      GPIO_Pin_4
		#define TOUCH_CS_PORT     GPIOA
		#define TOUCH_CS_RCC      RCC_APB2Periph_GPIOA
		#define TOUCH_CS					PAOut(4)
		#define TFT_LE_CS_PIN      GPIO_Pin_5
		#define TFT_LE_CS_PORT     GPIOB
		#define TFT_LE_CS_RCC      RCC_APB2Periph_GPIOB
		#define TFT_LE_CS					PBOut(5)		
		#define TFT_SD_CS_PIN      GPIO_Pin_6
		#define TFT_SD_CS_PORT     GPIOB
		#define TFT_SD_CS_RCC      RCC_APB2Periph_GPIOB
		#define TFT_SD_CS					PBOut(6)			
		
		
		#define TOUCH_INT_PIN      GPIO_Pin_13
		#define TOUCH_INT_PORT     GPIOC
		#define TOUCH_INT_RCC      RCC_APB2Periph_GPIOC		
		#define TOUCH_INT					PCIn(13)
//		#define RCC_ADS7843_CS                         RCC_APB2Periph_GPIOA
//		#define GPIO_ADS7843_CS_PORT                   GPIOA 
//		#define GPIO_ADS7843_CS                        GPIO_Pin_4   
		
		
//		#define RCC_ADS7843_INT                        RCC_APB2Periph_GPIOC
//		#define GPIO_ADS7843_INT_PORT                  GPIOC 
//		#define GPIO_ADS7843_INT                       GPIO_Pin_13  
		#define GPIO_ADS7843_EXTI_LINE                 EXTI_Line13
		#define GPIO_ADS7843_EXTI_PORT_SOURCE          GPIO_PortSourceGPIOC
		#define GPIO_ADS7843_EXTI_PIN_SOURCE           GPIO_PinSource13
		#define GPIO_ADS7843_EXTI_IRQn                 EXTI15_10_IRQn 
	#endif
	#if LCD_USE == LCD_USE_FMSC
		#define TOUCH_SPI 	SPI2	
		#define TOUCH_CS_PIN      GPIO_Pin_12
		#define TOUCH_CS_PORT     GPIOB
		#define TOUCH_CS_RCC      RCC_APB2Periph_GPIOB
		#define TOUCH_CS					PBOut(12)
		
		#define TOUCH_INT_PIN      GPIO_Pin_7
		#define TOUCH_INT_PORT     GPIOG
		#define TOUCH_INT_RCC      RCC_APB2Periph_GPIOG		
		#define TOUCH_INT					PGIn(7)	

//		#define RCC_ADS7843_CS                         RCC_APB2Periph_GPIOB
//		#define GPIO_ADS7843_CS_PORT                   GPIOB 
//		#define GPIO_ADS7843_CS                        GPIO_Pin_12   

//		#define RCC_ADS7843_INT                        RCC_APB2Periph_GPIOG
//		#define GPIO_ADS7843_INT_PORT                  GPIOG  
//		#define GPIO_ADS7843_INT                       GPIO_Pin_7  
		#define GPIO_ADS7843_EXTI_LINE                 EXTI_Line7
		#define GPIO_ADS7843_EXTI_PORT_SOURCE          GPIO_PortSourceGPIOG
		#define GPIO_ADS7843_EXTI_PIN_SOURCE           GPIO_PinSource7
		#define GPIO_ADS7843_EXTI_IRQn                 EXTI9_5_IRQn 
	#endif


 #endif
