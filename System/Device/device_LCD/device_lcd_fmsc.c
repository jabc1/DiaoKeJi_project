/******************************************************************************
* 文件名          : guidrivce_lcd_III.c
* 作者            : ZZ
* 版本            : V2.0.0
* 日期            : 2015.09.23
* 备注		      : TFT驱动函数，TFT ID为8989

			**    硬件连接说明          **
			** STM32         ili9320    **
			** PE0~15 <----> DB0~15     **		 使用FSMC的数据线DB0~15
			** PD4    <----> nRD        **		 FSMC_NOE
			** PF0    <----> RS         **		 FSMC_A0   FSMC的AO地址线，通过对AO地址线的电平来区分是否读写LED
			** PD5    <----> nWR        **		 FSMC_NWE
			** PG12   <----> nCS        **		 FSMC_NE4
			** RESET  <----> nReset     **		 系统RESET
			** PA1    <----> BK_LED     **
V2.0.0 针对MSOS GUI进行了修改，H文件和其他的几块LCD屏公用，在宏定义中切换使用哪个屏的驱动

*******************************************************************************/
#include "device_conf.h"
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;			
#define LCD_BASE    ((u32)(0x60000000 | 0x0C000000))/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define LCD         ((LCD_TypeDef *) LCD_BASE)	
/**********LCD内部使用的函数声明********************************/
static void LCD_FSMCConfig(void);
static u16 LCD_ReadRAM(void);
u16 LCD_ReadReg(u8 LCD_Reg);
static void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue);
/**************************************************************/


void LCD_BeginWriteRAM(void)
{
	 LCD->LCD_REG = R34;
}

void LCD_EndWriteRAM(void)
{
}

void LCD_OnlyWriteRAM(u16 LCD_DATA)
{
	LCD->LCD_RAM = LCD_DATA;
}


/****************************************************************************
* 名    称：void LCD_BackLight(u8 status)
* 功    能：开、关液晶背光
* 入口参数：status     1:背光开  0:背光关
* 出口参数：无
* 说    明：
* 调用方法：LCD_BackLight(1);
****************************************************************************/
void LCD_BackLight(u8 status)
{
	if ( status >= 1 )
	{
//    Lcd_Light_ON;
PAOut(1) = 1;
	}
	else
	{
 //   Lcd_Light_OFF;
PAOut(1) = 0;
	}
}
void LCD_FSMC_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |		 RCC_APB2Periph_GPIOA |
												 RCC_APB2Periph_GPIOF, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	/*FSMC数据线FSMC_D[0:15]初始化，推挽复用输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |	GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
															/* FSMC_D2			FSMC_D3				FSMC_D13		FSMC_D14			FSMC_D15			FSMC_D0				FSMC_D1	*/
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14|GPIO_Pin_15;
																/*FSMC_D4		FSMC_D5			FSMC_D6			FSMC_D7				FSMC_D8			FSMC_D9				FSMC_D10		FSMC_D11		FSMC_D12*/
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/*FSMC地址线FSMC_A[0]初始化,为LCD RS管脚，推挽复用输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);


	/*FSMC NOE和NWE初试化，推挽复用输出*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
															/* LCD_RD				LCD_WR  */
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* PG.12为FSMC的NE4控制端，同时为LCD的CS控制口； */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	

	/*LCD_BL LCD的背光*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	
	
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	LCD_FSMC_IO();
/*-- FSMC Configuration ------------------------------------------------------*/
	/* FSMC_Bank1_NORSRAM4 timing configuration */
	p.FSMC_AddressSetupTime =3;//1
	p.FSMC_AddressHoldTime = 0;
	p.FSMC_DataSetupTime =3;
	p.FSMC_BusTurnAroundDuration = 1;//0
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;

	/* FSMC_Bank1_NORSRAM4 configured as follows:
				- Data/Address MUX = Disable
				- Memory Type = SRAM
				- Data Width = 16bit
				- Write Operation = Enable
				- Extended Mode = Disable
				- Asynchronous Wait = Disable */
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

	/* Enable FSMC_Bank1_NORSRAM4 */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}
/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
*******************************************************************************/
static void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	LCD->LCD_REG = LCD_Reg;
	/* Write 16-bit Reg */
	LCD->LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
	/* Write 16-bit Index (then Read Reg) */
	LCD->LCD_REG = LCD_Reg;
	/* Read 16-bit Reg */
	return (LCD->LCD_RAM);
}



/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
*******************************************************************************/
static u16 LCD_ReadRAM(void)
{
	u16 dummy;
	/* Write 16-bit Index (then Read Reg) */
	LCD->LCD_REG = R34; /* Select GRAM Reg */
	/* Read 16-bit Reg */
	dummy = LCD->LCD_RAM; 
	return LCD->LCD_RAM;
}
/****************************************************************************
* 名    称：void InitializeLcd()
* 功    能：初始化 ILI9320 控制器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：InitializeLcd();
****************************************************************************/

void InitializeLcd(void)
{

	u16 i;
	u16 DeviceCode;					 
	LCD_BackLight(1);
	LCD_FSMCConfig();
	DelayMs(50); /* delay 50 ms */
	LCD_WriteReg(0x0000,0x0001);  
	DelayMs(50); /* delay 50 ms */			//start internal osc

	DelayMs(50);
	DeviceCode = LCD->LCD_RAM ;//LCD_ReadReg(0x0000);	 /*本次DeviceCode为8989*/

 // DeviceCode = 0x9320;


	
if(DeviceCode==0x8989)
	{
		// power supply setting 
		// set R07h at 0021h (GON=1,DTE=0,D[1:0]=01)
		LCD_WriteReg(0x0007,0x0021);       
		// set R00h at 0001h (OSCEN=1)       
		LCD_WriteReg(0x0000,0x0001);       
		// set R07h at 0023h (GON=1,DTE=0,D[1:0]=11)       
		LCD_WriteReg(0x0007,0x0023);       
		// set R10h at 0000h (Exit sleep mode)       
		LCD_WriteReg(0x0010,0x0000);       
		// Wait 30ms       
		DelayMs(50);       
		// set R07h at 0033h (GON=1,DTE=1,D[1:0]=11)       
		LCD_WriteReg(0x0007,0x0033);       
		// Entry mode setting (R11h)       
		// R11H Entry mode    设置TFT扫描方向   
		// vsmode DFM1 DFM0 TRANS OEDef WMode DMode1 DMode0 TY1 TY0 ID1 ID0 AM LG2 LG2 LG0       
		//   0     1    1     0     0     0     0      0     0   1   1   1  *   0   0   0      
		//0x6068   0x6058  横屏
		//0x6070   0x6040	 竖屏
		#if LCD_MODE ==0		//横1
			LCD_WriteReg(0x0011,0x6058);
		#endif
		#if LCD_MODE ==1		//横2
			LCD_WriteReg(0x0011,0x6068);			
		#endif
		#if LCD_MODE ==2			//竖1
			LCD_WriteReg(0x0011,0x6070);			
		#endif
		#if LCD_MODE ==3			//竖2
			LCD_WriteReg(0x0011,0x6040);			
		#endif

		// LCD driver AC setting (R02h)       
		LCD_WriteReg(0x0002,0x0600);       
		// power control 1       
		// DCT3 DCT2 DCT1 DCT0 BT2 BT1 BT0 0 DC3 DC2 DC1 DC0 AP2 AP1 AP0 0       
		// 1     0    1    0    1   0   0  0  1   0   1   0   0   1   0  0       
		// DCT[3:0] fosc/4 BT[2:0]  DC{3:0] fosc/4       
		LCD_WriteReg(0x0003,0x0804);//0xA8A4  
		LCD_WriteReg(0x000C,0x0000);//
		LCD_WriteReg(0x000D,0x080C);//       
		// power control 4       
		// 0 0 VCOMG VDV4 VDV3 VDV2 VDV1 VDV0 0 0 0 0 0 0 0 0       
		// 0 0   1    0    1    0    1    1   0 0 0 0 0 0 0 0       
		LCD_WriteReg(0x000E,0x2900);       
		LCD_WriteReg(0x001E,0x00B8);       
		LCD_WriteReg(0x0001,0x2B3F);
		LCD_WriteReg(0x0010,0x0000);       
		LCD_WriteReg(0x0005,0x0000);       
		LCD_WriteReg(0x0006,0x0000);       
		LCD_WriteReg(0x0016,0xEF1C);       
		LCD_WriteReg(0x0017,0x0003);       
		LCD_WriteReg(0x0007,0x0233);//0x0233       
		LCD_WriteReg(0x000B,0x0000|(3<<6));       
		LCD_WriteReg(0x000F,0x0000);
		LCD_WriteReg(0x0041,0x0000);       
		LCD_WriteReg(0x0042,0x0000);       
		LCD_WriteReg(0x0048,0x0000);       
		LCD_WriteReg(0x0049,0x013F);       
		LCD_WriteReg(0x004A,0x0000);       
		LCD_WriteReg(0x004B,0x0000);       
		LCD_WriteReg(0x0044,0xEF00);       
		LCD_WriteReg(0x0045,0x0000);       
		LCD_WriteReg(0x0046,0x013F);       
		LCD_WriteReg(0x0030,0x0707);       
		LCD_WriteReg(0x0031,0x0204);       
		LCD_WriteReg(0x0032,0x0204);       
		LCD_WriteReg(0x0033,0x0502);       
		LCD_WriteReg(0x0034,0x0507);       
		LCD_WriteReg(0x0035,0x0204);       
		LCD_WriteReg(0x0036,0x0204);       
		LCD_WriteReg(0x0037,0x0502);       
		LCD_WriteReg(0x003A,0x0302);       
		LCD_WriteReg(0x003B,0x0302);       
		LCD_WriteReg(0x0023,0x0000);       
		LCD_WriteReg(0x0024,0x0000);       
		LCD_WriteReg(0x0025,0x8000);   // 65hz       
		LCD_WriteReg(0x004f,0);        //       
		LCD_WriteReg(0x004e,0);        //       
		//lcd_data_bus_test();      
		//lcd_gram_test();         
 //   LCD_Clear(BColor);   

	}
	for(i=50000;i>0;i--);
}

/****************************************************************************
* 名    称：void LCD_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：
* 调用方法：LCD_SetCursor(10,10);
****************************************************************************/
__inline void LCD_SetCursor(u16 x,u16 y)
{	
	#if LCD_MODE ==0		//横1
		LCD_WriteReg(0x004e,y);        //行
		LCD_WriteReg(0x004f,0x13f-x);  //列
	#endif
	#if LCD_MODE ==1		//横2
		LCD_WriteReg(0x004e,239-y);        //行
		LCD_WriteReg(0x004f,x);  //列

	#endif
	#if LCD_MODE ==2			//竖1

	#endif
	#if LCD_MODE ==3			//竖2

	#endif		

}

/****************************************************************************
* 名    称：void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：LCD_SetWindows(0,0,100,100)；
****************************************************************************/
void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
	u16 V,sH,eH;

	
	
	#if LCD_MODE ==0		//横1
	V= 	(EndY<<8) | StartY;
	eH = 0x13f-StartX;
	sH = 0x13f-EndX;
	LCD_SetCursor(StartX,StartY);
	#endif
	#if LCD_MODE ==1		//横2
	V= 	((239-StartY)<<8) |(239-EndY) ;
	sH = StartX;
	eH = EndX;
	LCD_SetCursor(StartX,StartY);
	#endif
	#if LCD_MODE ==2			//竖1

	#endif
	#if LCD_MODE ==3			//竖2

	#endif			
	
	
	
	
	

	LCD_WriteReg(0x0044, V);
	LCD_WriteReg(0x0045, sH);
	LCD_WriteReg(0x0046, eH);
}



/****************************************************************************
* 名    称：u16 LCD_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=LCD_GetPoint(10,10);
****************************************************************************/
u16 LCD_GetPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);
	return (LCD_ReadRAM());
}
/****************************************************************************
* 名    称：void LCD_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：LCD_SetPoint(10,10,0x0fe0);
****************************************************************************/
void LCD_SetPoint(u16 x,u16 y,u16 Color)
{
	if ( (x>320)||(y>240) ) return;
	LCD_SetCursor(x,y);

	LCD->LCD_REG = R34;
	LCD->LCD_RAM = Color;
}





