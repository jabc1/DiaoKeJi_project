/*********************************************************************************
* 文件名			: device_spi.c
* 版本号    	: V2.1.1
* 作者			: ZZ
* 日期      : 2015/09/29
* 说明      : 开发板的SPI驱动，具体分SPI1~SPI3; SPI总线速度设置在drive.h中定义  
* 用法			：
	V2.1.1	
		针对MSOS将原来的SPI.H头文件取消了。
		1.使用公共的drive.h里定义SPI速度
		2.通过system.h里定义的System变量来实现函数的传递
	V2.1.0
		将SPI初始化公用一个函数
		void SPIx_Init(SPI_TypeDef* SPIx,u16 DataSize)
		void SPIx_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
		u8 SPIx_ReadWriteByte(SPI_TypeDef* SPIx,u8 TxData)
		SPIx为对应的SPI1~SPI3，其他参数与V1.1.0相同	
	V1.1.0		
		增加了原来没有定义的SPI口RCC	
	V1.0.0
		使用SPI的话，必须先初始化：
		SPI1_SetSpeed(SpeedSet)；设置SPI1的速度
		SPI2_SetSpeed(SpeedSet)；设置SPI2的速度
		RxData = SPI1_ReadWriteByte(TxData); 对SPI1进行数据读写
		RxData = SPI2_ReadWriteByte(TxData); 对SPI2进行数据读写
**********************************************************************************/

#include "device_SPI.h"


/********************************************
函数：SPI1_IO
功能：SPI1口的IO初始化
参数：无
返回：无
说明：
********************************************/
void SPI1_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*SPI1 RCC功能开启*/
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1, ENABLE );			 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
/*SPI1 GPIO设置*/
	GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MISO | SPI1_MOSI;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_PORT, &GPIO_InitStructure);	
}
/********************************************
函数：SPI2_IO
功能：SPI2口的IO初始化
参数：无
返回：无
说明：
********************************************/
void SPI2_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*SPI2 RCC功能开启*/
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );			 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
/*SPI2 GPIO设置*/
	GPIO_InitStructure.GPIO_Pin = SPI2_MISO| SPI2_MOSI| SPI2_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI2, &GPIO_InitStructure);
}
/********************************************
函数：SPI3_IO
功能：SPI3口的IO初始化
参数：无
返回：无
说明：
********************************************/
void SPI3_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*SPI3 RCC功能开启*/
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3, ENABLE );			 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
/*SPI3 GPIO设置*/
	GPIO_InitStructure.GPIO_Pin = SPI3_MISO| SPI3_MOSI| SPI3_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI3, &GPIO_InitStructure);
}
/********************************************
函数：SPIx_SetSpeed
功能：设置SPIx的速度
参数：SPIx：对应的SPI口；SpeedSet：速度
返回：无
说明：
********************************************/
void SPIx_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
{
	SPIx->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//二分频
	{
		SPIx->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_4)//四分频 
	{
		SPIx->CR1|=1<<3;//Fsck=Fpclk/4=18Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//八分频 
	{
		SPIx->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//十六分频
	{
		SPIx->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz

	}else if(SpeedSet==SPI_SPEED_32)//三十二分频
	{
		SPIx->CR1|=4<<3;//Fsck=Fpclk/16=4.5Mhz
	}else if(SpeedSet==SPI_SPEED_64)//六十四分频
	{
		SPIx->CR1|=5<<3;//Fsck=Fpclk/16=4.5Mhz
	}else if(SpeedSet==SPI_SPEED_128)//一二八分频
	{
		SPIx->CR1|=6<<3;//Fsck=Fpclk/16=4.5Mhz

	}else			 	 //256分频
	{
		SPIx->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
	}
	SPIx->CR1|=1<<6; //SPI设备使能	  
} 
/********************************************
函数：SPIx_ReadWriteByte
功能：SPIx的读写函数
参数：SPIx：对应的SPI口；TxData：需要写入的数据
返回：读取的数据
说明：
********************************************/
u8 SPIx_ReadWriteByte(SPI_TypeDef* SPIx,u16 TxData)
{		
	u8 retry=0;				 	
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)      //发送缓存标志位为空
		{
		retry++;
		if(retry>200)return 0;
		}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPIx, TxData);                                    //通过外设SPI1发送一个数据
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)  //接收缓存标志位不为空
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPIx);                                 //通过SPI1返回接收数据					    
}

/********************************************
函数：SPIx_Init
功能：SPI端口初始化
参数：SPIx：对应为SPI1~SPI3；DataSize：SPI的数据长度，8b OR 16b
返回：无
说明: 
********************************************/
void SPIx_Init(SPI_TypeDef* SPIx,u16 DataSize)
{
	SPI_InitTypeDef  SPI_InitStructure;
	if(SPIx==SPI1) SPI1_IO();
	if(SPIx==SPI2) SPI2_IO();		
	if(SPIx==SPI3) SPI3_IO();	
//	if (DataSize == 8)
/*SPI1功能设置*/
	SPI_Cmd(SPIx, DISABLE);  //必须先禁能,才能改变MODE 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  	//SPI1设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		               	 			//设置SPI1为主模式
	SPI_InitStructure.SPI_DataSize = DataSize;		            						//SPI发送接收8/16位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                    		//串行时钟在不操作时，时钟为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                    		//第一个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                   	 			//NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  	//SPI波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                	//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        		//CRC值计算的多项式
	SPI_Init(SPIx, &SPI_InitStructure);                                 	//根据SPI_InitStruct中指定的参数初始化外设SPI1寄存器
/* Enable SPI1  */
//	SPIx_SetSpeed(SPIx,SPI_SPEED_8);																		//设置SPI1的速度
	SPI_Cmd(SPIx, ENABLE);                                              	//使能SPI1外设
	SPIx_ReadWriteByte(SPIx,0xff);                                        //启动传输		
}

/********************************************
函数：SPI_Write
功能：SPI写入len长度的数据
参数：SPIx：SPI口；TxBuf：写入数据的指针；len：写入数据的长度
返回：数据长度
说明: 
********************************************/
u8 SPI_Write(SPI_TypeDef* SPIx,u8 *TxBuf, u8 len)
{
	u8 TxData,i;

	for(i=0; i<len; ++i)
	{
		TxData = TxBuf[i];
		SPIx_ReadWriteByte(SPIx,TxData);
	}

	return len;
}
/********************************************
函数：SPI_Read
功能：SPI读取len长度的数据
参数：SPIx：SPI口；RxBuf：读取数据的指针；len：读取数据的长度
返回：数据长度
说明: 
********************************************/

u8 SPI_Read(SPI_TypeDef* SPIx,u8 *RxBuf, u8 len)
{
	u8 RxData,i;

	for(i=0; i<len;++i)
	{
		RxData = SPIx_ReadWriteByte(SPIx,0);
		RxBuf[i] = RxData;
	}
	return len;
}





