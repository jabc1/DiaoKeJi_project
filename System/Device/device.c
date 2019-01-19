/*********************************************************************************
* 文件名			: device_LED.c
* 版本号    	: V2.2.1
* 作者			: ZZ
* 日期      : 2017/01/04
* 说明      : STM32的基本驱动  
* 用法			：

**********************************************************************************/
#include "stdio.h"
#include "stm32f10x.h" 
#include "device.h"

const u8 HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define DelayUsTick MainClock / 9000000

#define DelayMsTick MainClock / 9000

void Delay(u32 delayClock)
{
	while(delayClock--);
}

void DelayUs(u32 delayTime)
{
	while(delayTime--)
	{
		Delay(DelayUsTick);
	}
}

void DelayMs(u32 delayTime)
{
	while(delayTime--)
	{
		Delay(DelayMsTick);
	}
}


u8 HexToAscii(u8 hex)
{
//	Assert(hex < 16);

    if (hex < 16) 
        return HextoAscii[hex];
    else 
         return 0x30;
}

void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=temp;       //设置jtag模式
}

/**********************************************************
   加入以下代码，支持printf函数，而不需要选择use MicroLIB
**********************************************************/

#if 1

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR & 0x40) == 0);	//循环发送，直到发送完毕   
    USART1->DR = (uint8_t)ch;	//发送数据      
	return ch;
}
#endif
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

void IO_IN_IPD(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init((Port), &GPIO_InitStructure);
}
void IO_IN_IPU(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init((Port), &GPIO_InitStructure);
}
void IO_IN_PP_FL(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init((Port), &GPIO_InitStructure);
}

void IO_OUT_PP_LOW(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init((Port), &GPIO_InitStructure);
}
void IO_AF_PP_LOW(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init((Port), &GPIO_InitStructure);
}
void IO_OUT_PP_MID(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init((Port), &GPIO_InitStructure);	
}
void IO_AF_PP_MID(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init(Port, &GPIO_InitStructure);		
}
void IO_OUT_PP_HIGH(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init(Port, &GPIO_InitStructure);	
}
void IO_AF_PP_HIGH(GPIO_TypeDef *Port,u16 Pin,u32 Rcc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Rcc, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_Init(Port, &GPIO_InitStructure);

}


