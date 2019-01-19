/******************************************************************************
* 文件名          : guidrivce_lcd_I.c
* 作者            : ZZ
* 版本            : V2.0.0
* 日期            : 2015.09.23
* 备注		      : TFT驱动函数，TFT ID为8989

				**    硬件连接说明          **
				** STM32         ili9320   **
				** PC0~7 	<----> DB0~7     **		 
				** PB8~15 <----> DB8~15    **		 	
				** PC8    <----> RS        **		 
				** PC9   	<----> CS        **		 			
				** PC10   <----> WR        **		 				
				** PC11   <----> RD        **		 
				** RESET  <----> nReset    **		 系统RESET
				** PC13   <----> BK_LED    **
				
V2.0.0 针对MSOS GUI进行了修改，H文件和其他的几块LCD屏公用，在宏定义中切换使用哪个屏的驱动

*******************************************************************************/
#include "device_lcd.h"
		#define LCD_Write(LCD_DATA)	{\
																	GPIO_Write(GPIOC, ((GPIOC->ODR&0XFF00)|(LCD_DATA&0x00FF)));\
																	GPIO_Write(GPIOB, ((GPIOB->ODR&0X00FF)|(LCD_DATA&0xFF00)));\
																}	   //数据输出	
		#define LCD_Read()  (GPIO_ReadInputData(GPIOB)&0XFF00)|(GPIO_ReadInputData(GPIOC)&0X00FF)	  //数据输入	

	u16 LCD_BGR2RGB(u16 c);
	static void LCD_DB_AS_InPut(void);
	static void LCD_DB_AS_OutPut(void);
	static void LCD_Pins_Config(void);
	static u16 LCD_ReadRAM(void);
	static u16 LCD_ReadReg(u8 LCD_Reg);
	static u16 LCD_ReadSta(void);
	//static void LCD_WriteCommand(u16 LCD_RegValue);
	static void LCD_WriteRAM(u16 RGB_Code);
	static void LCD_WriteRAM_Prepare(void);
	static void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue);

	#define LCD_Light_ON  GPIO_SetBits(LCD_PORT_BL, LCD_Pin_BL); 
	#define LCD_Light_OFF GPIO_ResetBits(LCD_PORT_BL, LCD_Pin_BL);

	#define SetCs  GPIO_SetBits(LCD_PORT_CS, LCD_Pin_CS);
	#define ClrCs  GPIO_ResetBits(LCD_PORT_CS, LCD_Pin_CS);
	 

	#define SetWr  GPIO_SetBits(LCD_PORT_WR, LCD_Pin_WR);
	#define ClrWr  GPIO_ResetBits(LCD_PORT_WR, LCD_Pin_WR);

	#define SetRs  GPIO_SetBits(LCD_PORT_RS, LCD_Pin_RS);
	#define ClrRs  GPIO_ResetBits(LCD_PORT_RS, LCD_Pin_RS);

	#define SetRd  GPIO_SetBits(LCD_PORT_RD, LCD_Pin_RD);
	#define ClrRd  GPIO_ResetBits(LCD_PORT_RD, LCD_Pin_RD);



	////////////////////////////////////////////////////////////////////////////////////////
	/* LCD Control pins */
	#define LCD_Pin_BL      GPIO_Pin_12
	#define LCD_PORT_BL     GPIOC
	#define LCD_CLK_BL      RCC_APB2Periph_GPIOC

	#define LCD_Pin_WR      GPIO_Pin_10
	#define LCD_PORT_WR     GPIOC
	#define LCD_CLK_WR      RCC_APB2Periph_GPIOC

	#define LCD_Pin_CS      GPIO_Pin_9
	#define LCD_PORT_CS     GPIOC
	#define LCD_CLK_CS      RCC_APB2Periph_GPIOC

	#define LCD_Pin_RS      GPIO_Pin_8
	#define LCD_PORT_RS     GPIOC
	#define LCD_CLK_RS      RCC_APB2Periph_GPIOC

	#define LCD_Pin_RD      GPIO_Pin_11
	#define LCD_PORT_RD     GPIOC
	#define LCD_CLK_RD      RCC_APB2Periph_GPIOC
	u16 DeviceIdCode;

		/*****************************
		**    硬件连接说明          **
		** STM32         lcd    **
		** PC0~7  <----> DB0~7      **
		** PB8~15	<----> DB8~15	  **
		** PC11   <----> nRD        **
		** PC8    <----> RS         **
		** PC10   <----> nWR        **
		** PC9    <----> nCS        **
		** Reset  <----> nReset     **
		** PC12    <----> BK_LED     **
		******************************/
		
	/* Private typedef -----------------------------------------------------------*/

	/*******************************************************************************
	* Function Name  : LCD_DB_AS_InPut
	* Description    : config MCU LCD_DB pins AS InPut
	* Input          : None
	* Output         : None
	* Return         : None.
	*******************************************************************************/
	static void LCD_DB_AS_InPut(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		// DB15--0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}

	/*******************************************************************************
	* Function Name  : LCD_DB_AS_OutPut
	* Description    : config MCU LCD_DB pins AS OutPut
	* Input          : None
	* Output         : None
	* Return         : None.
	*******************************************************************************/
	static void LCD_DB_AS_OutPut(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		// DB15--0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

	}

	/*******************************************************************************
	* Function Name  : LCD_Pins_Config
	* Description    : Configures LCD Pins
										 Push-Pull mode.
	* Input          : None
	* Output         : None
	* Return         : None
	*******************************************************************************/
	static void LCD_Pins_Config(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(LCD_CLK_RS | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC|
													 LCD_CLK_WR | LCD_CLK_RD | LCD_CLK_CS | LCD_CLK_BL, ENABLE);

		// DB15--0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		//LCD_Pin_BL
		GPIO_InitStructure.GPIO_Pin = LCD_Pin_BL;
		GPIO_Init(LCD_PORT_BL, &GPIO_InitStructure);
		//LCD_Pin_WR
		GPIO_InitStructure.GPIO_Pin = LCD_Pin_WR;
		GPIO_Init(LCD_PORT_WR, &GPIO_InitStructure);

		//LCD_Pin_CS
		GPIO_InitStructure.GPIO_Pin = LCD_Pin_CS;
		GPIO_Init(LCD_PORT_CS, &GPIO_InitStructure);

		//LCD_Pin_RS
		GPIO_InitStructure.GPIO_Pin = LCD_Pin_RS;
		GPIO_Init(LCD_PORT_RS, &GPIO_InitStructure);

		//LCD_Pin_RD
		GPIO_InitStructure.GPIO_Pin = LCD_Pin_RD;
		GPIO_Init(LCD_PORT_RD, &GPIO_InitStructure);
		
		SetCs
	}

	void LCD_BeginWriteRAM(void)	
	{
		ClrCs; 
		ClrRs
		ClrWr
		LCD_Write(R34);
		SetWr
//		SetCs

		SetRs
	
	}
	

	void LCD_EndWriteRAM(void)	
	{
		SetCs	
		SetWr
		SetRs		
	}


	
	void LCD_OnlyWriteRAM(u16 RGB_Code)	
	{
		ClrWr
		LCD_Write(RGB_Code);
		SetWr
	}

	/*******************************************************************************
	* Function Name  : LCD_WriteReg
	* Description    : Writes to the selected LCD register.
	* Input          : - LCD_Reg: address of the selected register.
	*                  - LCD_RegValue: value to write to the selected register.
	* Output         : None
	* Return         : None
	* author         : www.armjishu.com 
	*******************************************************************************/
	static void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
	{
		/* Write 16-bit Index, then Write Reg */
		LCD_Light_ON
		ClrCs
		ClrRs
		ClrWr
		LCD_Write(LCD_Reg);
		SetWr
		/* Write 16-bit Reg */
		SetRs
		ClrWr
		LCD_Write(LCD_RegValue);
		SetWr
		SetCs
	}

	/*******************************************************************************
	* Function Name  : LCD_ReadReg
	* Description    : Reads the selected LCD Register.
	* Input          : None
	* Output         : None
	* Return         : LCD Register Value.
	*******************************************************************************/
	static u16 LCD_ReadReg(u8 LCD_Reg)
	{
		u16 data;
		
		/* Write 16-bit Index (then Read Reg) */
	 // LCD_Light_ON
		ClrCs
		ClrRs
		ClrWr
		LCD_Write(LCD_Reg);
		SetWr

		/* Read 16-bit Reg */
		SetRs
		ClrRd
		SetRd
		data = LCD_Read(); 
	 SetCs
			
	 return    data;
	}

	static u16 LCD_ReadSta(void)
	{
		u16 data;

		/* Write 16-bit Index, then Write Reg */
		//LCD_Light_ON
		SetRs
		ClrRd
		SetRd
		data = LCD_Read(); 
		SetCs
			
		return    data;
	}

	/*******************************************************************************
	* Function Name  : LCD_WriteRAM_Prepare
	* Description    : Prepare to write to the LCD RAM.
	* Input          : None
	* Output         : None
	* Return         : None
	*******************************************************************************/
	static void LCD_WriteRAM_Prepare(void)
	{
		/* Write 16-bit Index, then Write Reg */
	 // LCD_Light_ON
		ClrCs
		ClrRs
		ClrWr
		LCD_Write(R34);
		SetWr
		SetCs
	}




	/*******************************************************************************
	* Function Name  : LCD_WriteRAM
	* Description    : Writes to the LCD RAM.
	* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
	* Output         : None
	* Return         : None
	*******************************************************************************/
	static void LCD_WriteRAM(u16 RGB_Code)					 
	{
		/* Write 16-bit Index, then Write Reg */
	 // LCD_Light_ON
		ClrCs
		SetRs
		ClrWr
		LCD_Write(RGB_Code);
		SetWr
		SetCs
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
		u16 data;
		LCD_WriteRAM_Prepare();
		LCD_DB_AS_InPut();
		dummy = LCD_ReadSta();
		dummy++;
		data = LCD_ReadSta();
		LCD_DB_AS_OutPut();
		
		return    data;
	}
			
	///****************************************************************************
	//* 名    称：void LCD_Clear(u16 dat)
	//* 功    能：将屏幕填充成指定的颜色，如清屏，则填充 0xffff
	//* 入口参数：dat      填充值
	//* 出口参数：无
	//* 说    明：
	//* 调用方法：LCD_Clear(0xffff);
	//****************************************************************************/
	void LCDClear(u16 Color)
	{
		u32 index=0;
		LCD_SetCursor(0,0); 
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

		// 视频加速 www.armjishu.com
		ClrCs
		SetRs
		for(index=0;index<76800;index++)
		 {
				ClrWr
				LCD_Write(Color);
				SetWr
		 }
		SetCs
			
		/*
		for(index=0;index<76800;index++)
		 {
			 LCD_WriteRAM(Color);
		 }
		 */
	}

	/****************************************************************************
	* 名    称：void LCD_Initializtion()
	* 功    能：初始化 lcd 控制器
	* 入口参数：无
	* 出口参数：无
	* 说    明：
	* 调用方法：LCD_Initializtion();
	* 作    者： www.armjishu.com TEAM
	****************************************************************************/
	void InitializeLcd(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
		LCD_Pins_Config();
		DelayMs(50); /* delay 50 ms */
		LCD_WriteReg(0x0000,0x0001);  
		DelayMs(50);/* delay 50 ms */			//start internal osc
		LCD_DB_AS_InPut();
		DelayMs(50); /* delay 50 ms */			//start internal osc
		DeviceIdCode = LCD_ReadReg(0x0000);


		LCD_DB_AS_OutPut();
		DelayUs(1); /* delay 50 ms */			//start internal osc

	DeviceIdCode=0x8989;
	 
								 
		if(DeviceIdCode==0x8989)
		{
				LCD_WriteReg(0x0000,0x0001);		//打开晶振
				LCD_WriteReg(0x0003,0xA8A4);		//0xA8A4
				LCD_WriteReg(0x000C,0x0000);        
				LCD_WriteReg(0x000D,0x080C);        
				LCD_WriteReg(0x000E,0x2B00);        
				LCD_WriteReg(0x001E,0x00B0);        
				LCD_WriteReg(0x0001,0x293F);		//驱动输出控制320*240  0x693F  0x2B3F
				LCD_WriteReg(0x0002,0x0600);     	//LCD Driving Waveform control
				LCD_WriteReg(0x0010,0x0000);     
				LCD_WriteReg(0x0011,0x6078);		//0x4030	//定义数据格式  16位色	横屏 0x6058
				LCD_WriteReg(0x0005,0x0000);     
				LCD_WriteReg(0x0006,0x0000);     
				LCD_WriteReg(0x0016,0xEF1C);     
				LCD_WriteReg(0x0017,0x0003);     
				LCD_WriteReg(0x0007,0x0233);		//0x0233       
				LCD_WriteReg(0x000B,0x0000);     
				LCD_WriteReg(0x000F,0x0000);		//扫描开始地址
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
				LCD_WriteReg(0x0025,0x8000);     
				LCD_WriteReg(0x004e,0);        //列(X)首址0
				LCD_WriteReg(0x004f,0);        //行(Y)首址0
		}
	//    DelayMs(10);

			LCDClear(Black);
	//    StartX = (320 - 16*len)/2;
	//    for (i=0;i<len;i++)
	//    {
	//        LCD_PutChar_16x24((StartX+16*i),100,str[i],Yellow, Red);
	//    }

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
	void LCD_SetCursor(u16 x,u16 y)
	{

			LCD_WriteReg(0x004e,y);        //行
				LCD_WriteReg(0x004f,x);  //列

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
	//inline void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
	void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
	{
	//  LCD_SetCursor(StartX,StartY);
	//  LCD_WriteReg(0x0050, StartX);
	//  LCD_WriteReg(0x0052, StartY);
	//  LCD_WriteReg(0x0051, EndX);
	//  LCD_WriteReg(0x0053, EndY);
		u16 V,sH,eH;
		V= 	(EndY<<8) | StartY;
		eH =EndX ;
		sH = StartX;
	//	eH = 0x13f-StartX;
	//	sH = 0x13f-EndX;


		LCD_SetCursor(StartX,StartY);
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
		return (LCD_BGR2RGB(LCD_ReadRAM()));
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
		//if ( (x>320)||(y>240) ) return;
		LCD_SetCursor(x,y);

		LCD_WriteRAM_Prepare();
		LCD_WriteRAM(Color);
	}
	/****************************************************************************
	* 名    称：u16 LCD_BGR2RGB(u16 c)
	* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
	* 入口参数：c      BRG 颜色值
	* 出口参数：RGB 颜色值
	* 说    明：内部函数调用
	* 调用方法：
	****************************************************************************/
	u16 LCD_BGR2RGB(u16 c)
	{
		u16  r, g, b, rgb;

		b = (c>>0)  & 0x1f;
		g = (c>>5)  & 0x3f;
		r = (c>>11) & 0x1f;
		
		rgb =  (b<<11) + (g<<5) + (r<<0);

		return( rgb );
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
			LCD_Light_ON;
		}
		else
		{
			LCD_Light_OFF;
		}
	}
	

