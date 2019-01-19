
#ifndef __DEVICE_H
#define __DEVICE_H
#include "stm32f10x_gpio.h"



	//ADC1 DR寄存器基地址
	#define ADC1_DR_ADDRESS                  ((uint32_t)0x4001244C) 
	#define RAM_ADDRESS	0x20000000
	#define ROM_ADDRESS	0x08000000	

	typedef void (*function)(void);

	#define Function(address)   (*(function)address)()

	#define true                1
	#define false               0
	#define ON 									1
	#define OFF 								0	
	#define null                ((void *) 0)
	#define invalid             0xFF
	#define MainClock           72000000

	#define SetBit(data, offset)         (data |= 1U << offset)
	#define ResetBit(data, offset)       (data &= ~(1U << offset))
	#define GetBit(data, offset)         ((data >> offset) &0x01)

	#define UintToByte3(data)               ((u8 *)(&(data)))[3]
	#define UintToByte2(data)               ((u8 *)(&(data)))[2]
	#define UintToByte1(data)               ((u8 *)(&(data)))[1]
	#define UintToByte0(data)               ((u8 *)(&(data)))[0]

	#define UintToUshort1(data)             ((u16 *)(&(data)))[1]
	#define UintToUshort0(data)             ((u16 *)(&(data)))[0]



	#define GetByteFrom(address)                *((u8 *)(address)) 
	#define GetUshortFrom(address)              *((u16 *)(address)) 
	#define GetUintFrom(address)                *((u32 *)(address)) 

	#define Assert(express) (void)0 /* if (!(express)){printf("\nASSERT: " #express "\n");}   */
		
	
	typedef struct 	   //按键端口数据类型定义
{	
	u32 Rcc;
	GPIO_TypeDef * Port;
	u16 Pin;									
} IO_TypeDef ;

	void NVIC_Config(void);

	extern void Delay(u32 delayClock);
	extern void DelayUs(u32 delayTime);
	extern void DelayMs(u32 delayTime);
	extern u8 HexToAscii(u8 hex);

//	extern void JTAG_Set(u8 mode);
//	extern int fputc(int ch, FILE *f);




	#define	RCC_A				RCC_APB2Periph_GPIOA
	#define	RCC_B				RCC_APB2Periph_GPIOB
	#define	RCC_C				RCC_APB2Periph_GPIOC
	#define	RCC_D				RCC_APB2Periph_GPIOD
	#define	RCC_E				RCC_APB2Periph_GPIOE
	#define	RCC_F				RCC_APB2Periph_GPIOF
	#define	RCC_G				RCC_APB2Periph_GPIOG

	//JTAG模式设置定义
	#define JTAG_SWD_DISABLE   0X02
	#define SWD_ENABLE         0X01
	#define JTAG_SWD_ENABLE    0X00	

void Delay(u32 delayClock);
void DelayUs(u32 delayTime);
void DelayMs(u32 delayTime);
u8 HexToAscii(u8 hex);
void JTAG_Set(u8 mode);


void INTX_DISABLE(void);
void INTX_ENABLE(void);


	void IO_IN_IPD(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_IN_IPU(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_IN_PP_FL(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_OUT_PP_LOW(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_AF_PP_LOW(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_OUT_PP_MID(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_AF_PP_MID(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_OUT_PP_HIGH(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);
	void IO_AF_PP_HIGH(GPIO_TypeDef *Port,u16 Pin,u32 Rcc);

#endif

