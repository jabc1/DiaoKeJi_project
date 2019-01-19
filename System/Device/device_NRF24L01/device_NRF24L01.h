//����III�ſ�����
#ifndef __NRF24L01_H
#define __NRF24L01_H	 		    
#include "stm32f10x.h"
#include "device_conf.h"




#if (USE_DRV ==	USE_III_DRV)				/*����I�Ű�����ض���*/
#define NRF_SPI											SPI2
	//NRF24L01���Ʋ���
	#define NRF_CE_PIN      GPIO_Pin_5
	#define NRF_CE_GPIO  		GPIOC
	#define NRF_CE_RCC  		RCC_APB2Periph_GPIOC
	#define NRF_CE      		PCOut(5)
	//NRF24L01 SPI�ӿ�CS�ź�
	#define NRF_CSN_PIN     GPIO_Pin_10
	#define NRF_CSN_GPIO  	GPIOF
	#define NRF_CSN_RCC  		RCC_APB2Periph_GPIOF
	#define NRF_CSN	     		PFOut(10)
	//NRF2401_IRQ��������
	#define NRF_IRQ_PIN     GPIO_Pin_15
	#define NRF_IRQ_GPIO  	GPIOG
	#define NRF_IRQ_RCC  		RCC_APB2Periph_GPIOG
	#define NRF_IRQ					PGIn(15)
	#define NRF_IRQ_O				PGOut(15)	
	//��������SPI�ڵ��豸��Ҫ��CS����1

		#define NRF_TH_PIN      GPIO_Pin_12
		#define NRF_TH_GPIO     GPIOB
		#define NRF_TH_RCC      RCC_APB2Periph_GPIOB
		#define NRF_TH					PBOut(12)	
	
#endif


#if (USE_DRV ==	USE_I_DRV)				/*����I�Ű�����ض���*/
#define NRF_SPI											SPI1
	//NRF24L01���Ʋ���
	#define NRF_CE_PIN      GPIO_Pin_3
	#define NRF_CE_GPIO  		GPIOA
	#define NRF_CE_RCC  		RCC_APB2Periph_GPIOA
	#define NRF_CE      		PAOut(3)
	//NRF24L01 SPI�ӿ�CS�ź�
	#define NRF_CSN_PIN     GPIO_Pin_4
	#define NRF_CSN_GPIO  	GPIOB
	#define NRF_CSN_RCC  		RCC_APB2Periph_GPIOB
	#define NRF_CSN	     		PBOut(4)
	//NRF2401_IRQ��������
	#define NRF_IRQ_PIN     GPIO_Pin_2
	#define NRF_IRQ_GPIO  	GPIOA
	#define NRF_IRQ_RCC  		RCC_APB2Periph_GPIOA
	#define NRF_IRQ					PAIn(2)
	#define NRF_IRQ_O					PAOut(2)	
	//��������SPI�ڵ��豸��Ҫ��CS����1
		#define NRF_TH_PIN      GPIO_Pin_4
		#define NRF_TH_GPIO     GPIOA
		#define NRF_TH_RCC      RCC_APB2Periph_GPIOA
		#define NRF_TH					PAOut(4)	
	
#endif
#if 	(USE_DRV ==	USE_C8T6_DRV)			/*��Сϵͳ������ض���*/
#define NRF_SPI											SPI2
	//NRF24L01���Ʋ���
	#define NRF_CE_PIN      GPIO_Pin_15
	#define NRF_CE_GPIO  		GPIOA
	#define NRF_CE_RCC  		RCC_APB2Periph_GPIOA
	#define NRF_CE      		PAOut(15)
	//NRF24L01 SPI�ӿ�CS�ź�
	#define NRF_CSN_PIN     GPIO_Pin_12
	#define NRF_CSN_GPIO  	GPIOB
	#define NRF_CSN_RCC  		RCC_APB2Periph_GPIOB
	#define NRF_CSN	     		PBOut(12)	
	//NRF2401_IRQ��������
	#define NRF_IRQ_PIN     GPIO_Pin_8
	#define NRF_IRQ_GPIO  	GPIOA
	#define NRF_IRQ_RCC  		RCC_APB2Periph_GPIOA
	#define NRF_IRQ					PAIn(8)
	#define NRF_IRQ_O				PAOut(8)
#endif	





#define NRF_SPI_ReadWriteByte(Adrr) SPIx_ReadWriteByte(NRF_SPI,Adrr)




//NRF24L01 �������� 
/****************************************************************************************************/
//NRF24L01�Ĵ�����������
#define SPI_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define SPI_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX  	    0x10  //�ﵽ����ʹ����ж�
#define TX_OK       	0x20  //TX��������ж�
#define RX_OK   	    0x40  //���յ������ж�

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
/**********************************************************************************************************/


//NRF2401Ƭѡ�ź�
#define Clr_NRF24L01_CE      NRF_CE = 0;
#define Set_NRF24L01_CE      NRF_CE = 1;

//SPIƬѡ�ź�	
#define Clr_NRF24L01_CSN     NRF_CSN  = 0;
#define Set_NRF24L01_CSN     NRF_CSN  = 1;
    
#define READ_NRF24L01_IRQ         NRF_IRQ

//NRF24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5                               //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5                               //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32                              //32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32                              //32�ֽڵ��û����ݿ��
									   	   

void NRF24L01_Init(void);                                //NRF24l01��ʼ��
void RX_Mode(void);                                      //����Ϊ����ģʽ
void TX_Mode(void);                                      //����Ϊ����ģʽ
u8 NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen); //д������
u8 NRF24L01_Read_Buf(u8 regaddr, u8 *pBuf, u8 datalen);  //��������		  
u8 NRF24L01_Read_Reg(u8 regaddr);		                 //���Ĵ���
u8 NRF24L01_Write_Reg(u8 regaddr, u8 data);              //д�Ĵ���
u8 NRF24L01_Check(void);                                 //���NRF24L01�Ƿ���λ
u8 NRF24L01_TxPacket(u8 *txbuf);                         //����һ����������
u8 NRF24L01_RxPacket(u8 *rxbuf);                         //����һ����������
#endif











