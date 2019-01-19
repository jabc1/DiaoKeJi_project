/******************************************************************************
* �ļ���		: GUIDRIVCE_LCD144.c
* �汾��    : V2.0.0
* ����      : 2015.09.23
* ˵��      : �������LCD 1.44�� SPI������						  
* �÷�			��
//��2��һ�����ص�����
**    LCDӲ������˵��          	**
** STM32         			1.44lcd   **
** VCC3.3V 	<----> 		VCC      	**				+��Դ
** GND			<----> 		GND	  		**				�ӵ�
** PD14   	<----> 		CS        **				Ƭѡ
** NRST    	<----> 		RESET    	**				��λ
** PD15   	<----> 		AO        **				����/����
** PD5    	<---->		LED     	**				����
** PB15    	<----> 		SDA       **				SPI2_MOSI
** PB13  		<----> 		SCK     	**				SPI2_SCK

V2.0.0 ���MSOS GUI�������޸ģ�H�ļ��������ļ���LCD�����ã��ں궨�����л�ʹ���ĸ���������


******************************************************************************/
#include "GUIDRIVCE.h"
#define LCD144_SPI_ReadWriteByte(LCD_SPI,LCD_DATA)  SPIx_ReadWriteByte(LCD_SPI,LCD_DATA)
		#define LCD_SPI SPI2
		
	#define SPI_USE_16B true

	void LCD_WR_REG(u8 da);	 
	void LCD_WR_DATA(u16 da);
	void LCD_WR_DATA8(u16 da);

	/* 144LCD Control pins */
	#define GPIO_LCD144_AO      GPIO_Pin_15
	#define PORT_LCD144_AO     GPIOD
	#define RCC_LCD144_AO      RCC_APB2Periph_GPIOD

	#define GPIO_LCD144_LED      GPIO_Pin_5
	#define PORT_LCD144_LED     GPIOD
	#define RCC_LCD144_LED      RCC_APB2Periph_GPIOD

	#define GPIO_LCD144_CS      GPIO_Pin_14
	#define PORT_LCD144_CS     GPIOD
	#define RCC_LCD144_CS      RCC_APB2Periph_GPIOD
	#define RCC_LCD144					RCC_LCD144_AO | RCC_LCD144_LED | RCC_LCD144_CS

	#define SetCs  GPIO_SetBits(PORT_LCD144_CS, GPIO_LCD144_CS);
	#define ClrCs  GPIO_ResetBits(PORT_LCD144_CS, GPIO_LCD144_CS);

	#define SetWr_RAM  GPIO_SetBits(PORT_LCD144_AO, GPIO_LCD144_AO);
	#define SetWr_REG  GPIO_ResetBits(PORT_LCD144_AO, GPIO_LCD144_AO);

	#define SetLED  GPIO_SetBits(PORT_LCD144_LED, GPIO_LCD144_LED);
	#define ClrLED  GPIO_ResetBits(PORT_LCD144_LED, GPIO_LCD144_LED);


	/****************************************************************************
	* ��    �ƣ�void LCD_BackLight(u8 status)
	* ��    �ܣ�������Һ������
	* ��ڲ�����status     1:���⿪  0:�����
	* ���ڲ�������
	* ˵    ����
	* ���÷�����LCD_BackLight(1);
	****************************************************************************/
	void LCD_BackLight(u8 status)
	{
		if ( status >= 1 )
		{
			SetLED;
		}
		else
		{
			ClrLED;
		}
	}

	void LCD_OnlyWriteRAM(u16 LCD_DATA)
	{
//		SetWr_RAM;
		LCD144_SPI_ReadWriteByte(LCD_SPI,LCD_DATA);
	}
	void LCD_BeginWriteRAM(void)	
	{
		SetWr_RAM;
		ClrCs	
	}

	void LCD_EndWriteRAM(void)	
	{
		SetCs	
	}





	void LCD_WR_REG(u8 da)	 
	{
		SetWr_REG;
		LCD144_SPI_ReadWriteByte(LCD_SPI,da);
	}
	void LCD_WR_REG16(u16 da)	 
	{
		SetWr_REG;
		LCD144_SPI_ReadWriteByte(LCD_SPI,da);
	}

	 void LCD_WR_DATA(u16 da)
	{
		SetWr_RAM;
		LCD144_SPI_ReadWriteByte(LCD_SPI,da>>8);
		LCD144_SPI_ReadWriteByte(LCD_SPI,da);
	}	
	 void LCD_WR_DATA8(u16 da)
	{
		SetWr_RAM;
		LCD144_SPI_ReadWriteByte(LCD_SPI,da);
	}
	 void LCD_WR_DATA16(u16 da)
	{
		SetWr_RAM;
		LCD144_SPI_ReadWriteByte(LCD_SPI,da);
	}	

	void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
	{ 
		ClrCs;	
		LCD_WR_REG16(0x2A); //Set Column Address
		LCD_WR_DATA16(StartX);
		LCD_WR_DATA16(EndX);
		LCD_WR_REG16(0x2B); //Set Page Address
		LCD_WR_DATA16(StartY+32);
		LCD_WR_DATA16(EndY+32);
		LCD_WR_REG16(0x2C);
		SetCs;
	}
		






	/****************************************************************************
	* ��    �ƣ�void LCD_SetPoint(u16 x,u16 y,u16 point)
	* ��    �ܣ���ָ�����껭��
	* ��ڲ�����x      ������
	*           y      ������
	*           point  �����ɫ
	* ���ڲ�������
	* ˵    ����
	* ���÷�����LCD_SetPoint(10,10,0x0fe0);
	****************************************************************************/
	void LCD_SetPoint(u16 x,u16 y,u16 Color)
	{

		LCD_SetWindows(x,y,x,y);
		ClrCs; 		
		SetWr_RAM;
	//	System.Device.SPI.LCD144_SPI_ReadWriteByte(LCD_SPI,point>>8);
		LCD144_SPI_ReadWriteByte(LCD_SPI,Color);
		SetCs;

	}

	static void GPIO_LCD_Pins(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_LCD144, ENABLE);
		//LCD_Pin_A0
		GPIO_InitStructure.GPIO_Pin = GPIO_LCD144_AO;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(PORT_LCD144_AO, &GPIO_InitStructure);
		//LCD_Pin_LED
		GPIO_InitStructure.GPIO_Pin = GPIO_LCD144_LED;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(PORT_LCD144_LED, &GPIO_InitStructure);

		//LCD_Pin_CS
		GPIO_InitStructure.GPIO_Pin = GPIO_LCD144_CS;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(PORT_LCD144_CS, &GPIO_InitStructure);	
		GPIO_SetBits(PORT_LCD144_CS, GPIO_LCD144_CS);
	}

	/****************************************************************************
	* ��    �ƣ�void LCD_Initializtion()
	* ��    �ܣ���ʼ�� lcd ������
	* ��ڲ�������
	* ���ڲ�������
	* ˵    ����
	* ���÷�����LCD_Initializtion();
	* ��    �ߣ� www.armjishu.com TEAM
	****************************************************************************/
	void InitializeLcd(void)
	{
		GPIO_LCD_Pins();
		SPIx_Init(LCD_SPI,8);
		SetLED
		ClrCs  //��Ƭѡʹ��

		LCD_WR_REG(0x11); //Exit Sleep
		DelayMs(20);
		LCD_WR_REG(0x26); //Set Default Gamma
		LCD_WR_DATA8(0x04);
		LCD_WR_REG(0xB1);//Set Frame Rate
		LCD_WR_DATA8(0x0e);
		LCD_WR_DATA8(0x10);
		LCD_WR_REG(0xC0); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
		LCD_WR_DATA8(0x08);
		LCD_WR_DATA8(0x00);
		LCD_WR_REG(0xC1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
		LCD_WR_DATA8(0x05);
		LCD_WR_REG(0xC5); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
		LCD_WR_DATA8(0x38);
		LCD_WR_DATA8(0x40);
		
		LCD_WR_REG(0x3a); //Set Color Format
		LCD_WR_DATA8(0x05);
		LCD_WR_REG(0x36); //RGB			 ɨ�跽��
		LCD_WR_DATA8(0x1C);   //1C//C8
		
		LCD_WR_REG(0x2A); //Set Column Address
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x7F);
		LCD_WR_REG(0x2B); //Set Page Address
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(32);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(127+32);
		
		LCD_WR_REG(0xB4); 
		LCD_WR_DATA8(0x00);
		
		LCD_WR_REG(0xf2); //Enable Gamma bit
		LCD_WR_DATA8(0x01);
		LCD_WR_REG(0xE0); 
		LCD_WR_DATA8(0x3f);//p1
		LCD_WR_DATA8(0x22);//p2
		LCD_WR_DATA8(0x20);//p3
		LCD_WR_DATA8(0x30);//p4
		LCD_WR_DATA8(0x29);//p5
		LCD_WR_DATA8(0x0c);//p6
		LCD_WR_DATA8(0x4e);//p7
		LCD_WR_DATA8(0xb7);//p8
		LCD_WR_DATA8(0x3c);//p9
		LCD_WR_DATA8(0x19);//p10
		LCD_WR_DATA8(0x22);//p11
		LCD_WR_DATA8(0x1e);//p12
		LCD_WR_DATA8(0x02);//p13
		LCD_WR_DATA8(0x01);//p14
		LCD_WR_DATA8(0x00);//p15
		LCD_WR_REG(0xE1); 
		LCD_WR_DATA8(0x00);//p1
		LCD_WR_DATA8(0x1b);//p2
		LCD_WR_DATA8(0x1f);//p3
		LCD_WR_DATA8(0x0f);//p4
		LCD_WR_DATA8(0x16);//p5
		LCD_WR_DATA8(0x13);//p6
		LCD_WR_DATA8(0x31);//p7
		LCD_WR_DATA8(0x84);//p8
		LCD_WR_DATA8(0x43);//p9
		LCD_WR_DATA8(0x06);//p10
		LCD_WR_DATA8(0x1d);//p11
		LCD_WR_DATA8(0x21);//p12
		LCD_WR_DATA8(0x3d);//p13
		LCD_WR_DATA8(0x3e);//p14
		LCD_WR_DATA8(0x3f);//p15
		
		LCD_WR_REG(0x29); // Display On
		LCD_WR_REG(0x2C);
		
		SPIx_Init(LCD_SPI,16);
	//	LCD_Clear(Red);
		
	}


