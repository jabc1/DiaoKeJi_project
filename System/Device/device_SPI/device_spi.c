/*********************************************************************************
* �ļ���			: device_spi.c
* �汾��    	: V2.1.1
* ����			: ZZ
* ����      : 2015/09/29
* ˵��      : �������SPI�����������SPI1~SPI3; SPI�����ٶ�������drive.h�ж���  
* �÷�			��
	V2.1.1	
		���MSOS��ԭ����SPI.Hͷ�ļ�ȡ���ˡ�
		1.ʹ�ù�����drive.h�ﶨ��SPI�ٶ�
		2.ͨ��system.h�ﶨ���System������ʵ�ֺ����Ĵ���
	V2.1.0
		��SPI��ʼ������һ������
		void SPIx_Init(SPI_TypeDef* SPIx,u16 DataSize)
		void SPIx_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
		u8 SPIx_ReadWriteByte(SPI_TypeDef* SPIx,u8 TxData)
		SPIxΪ��Ӧ��SPI1~SPI3������������V1.1.0��ͬ	
	V1.1.0		
		������ԭ��û�ж����SPI��RCC	
	V1.0.0
		ʹ��SPI�Ļ��������ȳ�ʼ����
		SPI1_SetSpeed(SpeedSet)������SPI1���ٶ�
		SPI2_SetSpeed(SpeedSet)������SPI2���ٶ�
		RxData = SPI1_ReadWriteByte(TxData); ��SPI1�������ݶ�д
		RxData = SPI2_ReadWriteByte(TxData); ��SPI2�������ݶ�д
**********************************************************************************/

#include "device_SPI.h"


/********************************************
������SPI1_IO
���ܣ�SPI1�ڵ�IO��ʼ��
��������
���أ���
˵����
********************************************/
void SPI1_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*SPI1 RCC���ܿ���*/
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1, ENABLE );			 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
/*SPI1 GPIO����*/
	GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MISO | SPI1_MOSI;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_PORT, &GPIO_InitStructure);	
}
/********************************************
������SPI2_IO
���ܣ�SPI2�ڵ�IO��ʼ��
��������
���أ���
˵����
********************************************/
void SPI2_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*SPI2 RCC���ܿ���*/
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );			 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
/*SPI2 GPIO����*/
	GPIO_InitStructure.GPIO_Pin = SPI2_MISO| SPI2_MOSI| SPI2_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI2, &GPIO_InitStructure);
}
/********************************************
������SPI3_IO
���ܣ�SPI3�ڵ�IO��ʼ��
��������
���أ���
˵����
********************************************/
void SPI3_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*SPI3 RCC���ܿ���*/
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3, ENABLE );			 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
/*SPI3 GPIO����*/
	GPIO_InitStructure.GPIO_Pin = SPI3_MISO| SPI3_MOSI| SPI3_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI3, &GPIO_InitStructure);
}
/********************************************
������SPIx_SetSpeed
���ܣ�����SPIx���ٶ�
������SPIx����Ӧ��SPI�ڣ�SpeedSet���ٶ�
���أ���
˵����
********************************************/
void SPIx_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
{
	SPIx->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
	{
		SPIx->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_4)//�ķ�Ƶ 
	{
		SPIx->CR1|=1<<3;//Fsck=Fpclk/4=18Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//�˷�Ƶ 
	{
		SPIx->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//ʮ����Ƶ
	{
		SPIx->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz

	}else if(SpeedSet==SPI_SPEED_32)//��ʮ����Ƶ
	{
		SPIx->CR1|=4<<3;//Fsck=Fpclk/16=4.5Mhz
	}else if(SpeedSet==SPI_SPEED_64)//��ʮ�ķ�Ƶ
	{
		SPIx->CR1|=5<<3;//Fsck=Fpclk/16=4.5Mhz
	}else if(SpeedSet==SPI_SPEED_128)//һ���˷�Ƶ
	{
		SPIx->CR1|=6<<3;//Fsck=Fpclk/16=4.5Mhz

	}else			 	 //256��Ƶ
	{
		SPIx->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz ����ģʽ
	}
	SPIx->CR1|=1<<6; //SPI�豸ʹ��	  
} 
/********************************************
������SPIx_ReadWriteByte
���ܣ�SPIx�Ķ�д����
������SPIx����Ӧ��SPI�ڣ�TxData����Ҫд�������
���أ���ȡ������
˵����
********************************************/
u8 SPIx_ReadWriteByte(SPI_TypeDef* SPIx,u16 TxData)
{		
	u8 retry=0;				 	
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)      //���ͻ����־λΪ��
		{
		retry++;
		if(retry>200)return 0;
		}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPIx, TxData);                                    //ͨ������SPI1����һ������
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)  //���ջ����־λ��Ϊ��
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPIx);                                 //ͨ��SPI1���ؽ�������					    
}

/********************************************
������SPIx_Init
���ܣ�SPI�˿ڳ�ʼ��
������SPIx����ӦΪSPI1~SPI3��DataSize��SPI�����ݳ��ȣ�8b OR 16b
���أ���
˵��: 
********************************************/
void SPIx_Init(SPI_TypeDef* SPIx,u16 DataSize)
{
	SPI_InitTypeDef  SPI_InitStructure;
	if(SPIx==SPI1) SPI1_IO();
	if(SPIx==SPI2) SPI2_IO();		
	if(SPIx==SPI3) SPI3_IO();	
//	if (DataSize == 8)
/*SPI1��������*/
	SPI_Cmd(SPIx, DISABLE);  //�����Ƚ���,���ܸı�MODE 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  	//SPI1����Ϊ����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		               	 			//����SPI1Ϊ��ģʽ
	SPI_InitStructure.SPI_DataSize = DataSize;		            						//SPI���ͽ���8/16λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                    		//����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                    		//��һ��ʱ���ؿ�ʼ��������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                   	 			//NSS�ź��������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  	//SPI������Ԥ��ƵֵΪ8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                	//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        		//CRCֵ����Ķ���ʽ
	SPI_Init(SPIx, &SPI_InitStructure);                                 	//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI1�Ĵ���
/* Enable SPI1  */
//	SPIx_SetSpeed(SPIx,SPI_SPEED_8);																		//����SPI1���ٶ�
	SPI_Cmd(SPIx, ENABLE);                                              	//ʹ��SPI1����
	SPIx_ReadWriteByte(SPIx,0xff);                                        //��������		
}

/********************************************
������SPI_Write
���ܣ�SPIд��len���ȵ�����
������SPIx��SPI�ڣ�TxBuf��д�����ݵ�ָ�룻len��д�����ݵĳ���
���أ����ݳ���
˵��: 
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
������SPI_Read
���ܣ�SPI��ȡlen���ȵ�����
������SPIx��SPI�ڣ�RxBuf����ȡ���ݵ�ָ�룻len����ȡ���ݵĳ���
���أ����ݳ���
˵��: 
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





