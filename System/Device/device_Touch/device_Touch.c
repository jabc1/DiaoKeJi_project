/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : ARMJISHU_TouchScreen_ADS7843.c
* Author             : www.armjishu.com Team
* Version            : V3.0.1
* Date               : 03/20/2010
* Description        : 
                        ADS7843_CS   is PB1
                        ADS7843_INT  is PC1
*******************************************************************************/

#include "device_touch.h"
#include "device_conf.h"


#if TOUCHDEVICE  > 0

#if (USE_MSOS == 1)
	#include "OS.h"
	u8 TouchMessageQueuePointer = 0;			//����KEY��Ϣ�����Ǹ��ź�ָ��		

	#if (USE_GUI == 1)
		#include "GUI_OS.h"		
		#define TouchMessageToTaskPriority	GUITaskPriority
	#else
		#define TouchMessageToTaskPriority TouchMessageQueuePointer
	#endif

#endif

	typedef enum 
	{
		TOUCH_NULL		= 0x00,
		TOUCH_DOWN	= 0x01,
		TOUCH_LONG		= 0x02,
		TOUCH_UP		= 0x04,
//		GUI_KT_NONUM	= 0xFF	
	}TOUCH_TYPE;

// A/D ͨ��ѡ�������ֺ͹����Ĵ���
#define	CHX 				0x90			//0x90 	//ͨ��Y+��ѡ�������	//0x94
#define	CHY 				0xD0			//0xd0	//ͨ��X+��ѡ�������	//0xD4
	
#if (USE_MSOS == 1)
/*******************************************************************************
* ������	: SetKeyMessagePointer
* ����	    : ���õ�ǰ����ΪKEY���͵�����ָ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SetTouchMessagePointer(u8 MessagePointer)
{
	TouchMessageQueuePointer = MessagePointer;	
}
/*******************************************************************************
* ������	: PostKeyMessageQueue
* ����	    : ����KEY��Ϣ
* �������  : messageType: ��Ϣ����
*             data:��Ϣֵ��MessageTimer����Ϊ32bit��ַ,���������¶���24bit����
* ���ز���  : ��
*******************************************************************************/
void PostTouchMessageQueue(u32 Touch)
{
	u32 Message;
	if (TouchMessageToTaskPriority >=TaskSum) return;		
	Message = TouchMessageType;
	OS.PostMessageQueue(TouchMessageToTaskPriority, (void *)((Message<<24) | Touch));		
}

#endif	
	
	
	
	
//u8 IsTouch(void)
//{
//	if(TOUCH_INT)
//	{
//		return false;
//	}
//	else
//	{
//    return true;
//	}
//}
void InitializeTouch(void)
{
	SPIx_Init(TOUCH_SPI,SPI_DataSize_8b);		//��ʼ��SPI
	IO_OUT_PP_HIGH(TOUCH_CS_PORT,TOUCH_CS_PIN,TOUCH_CS_RCC);
	TOUCH_CS = 1;		
	IO_IN_IPU(TOUCH_INT_PORT,TOUCH_INT_PIN,TOUCH_INT_RCC);	
	#if USE_DRV == USE_I_DRV

	IO_OUT_PP_HIGH(TFT_SD_CS_PORT,TFT_SD_CS_PIN,TFT_SD_CS_RCC);
	TFT_SD_CS = 1;		
	IO_OUT_PP_HIGH(TFT_LE_CS_PORT,TFT_LE_CS_PIN,TFT_LE_CS_RCC);
	TFT_LE_CS = 1;		

	#endif	
	TOUCH_CS = 0;//д�����touch������touch	  
	SPIx_ReadWriteByte(TOUCH_SPI, CHX ) ;

TOUCH_CS = 1;
#if (USE_MSOS == 1)
	
	OS.Systick.Register(Systick50, TouchSystick200Routine);
	#endif
}
	
/*=====================================================================*/
u16 _AD240(u16 ad240) //240
{
  u16 sx=0;
  int r = ad240 - 384;
  r *= 240;
  sx=r / (3758-384);
  if (sx<=0 || sx>240)
    return 0;
  return sx;
}


u16 _AD320(u16 ad320) //320
{
  u16 sy=0;
  int r = ad320 - 460;
  r *= 320;
  sy=r/(3650 - 460);
  if (sy<=0 || sy>320)
    return 0;
  return sy;
}


/*=====================================================================*/
static u16 ADS7843_Read(void)
{

    u16 ans=0,temp;  
    temp=SPIx_ReadWriteByte(TOUCH_SPI,0x00);  
    ans=temp<<8;  
    temp=SPIx_ReadWriteByte(TOUCH_SPI,0x00);  
    ans|=temp;  
    ans>>=3;  
    return ans&0x0fff;  
}  

/*=====================================================================*/
#define  times  6
void Touch_Rd_Addata(u16 *X_Addata,u16 *Y_Addata)
{
	u16	i,j,k,x_addata[times],y_addata[times];
	for(i=0;i<times;i++){					//����4��.
		TOUCH_CS = 0;   
		SPIx_ReadWriteByte(TOUCH_SPI,CHY ) ;
		DelayMs(1);
		y_addata[i] = ADS7843_Read() ;
		SPIx_ReadWriteByte(TOUCH_SPI,CHX ) ;
		DelayMs(1);
		x_addata[i] = ADS7843_Read() ;
    TOUCH_CS = 1;
	}

	for(i=0;i<times;i++){
		for(j=times;j<times-1;j++){
			if(x_addata[j] > x_addata[i]){
				k = x_addata[j];
				x_addata[i] = x_addata[j];
				x_addata[j] = k;
			}
    	if(y_addata[j] > y_addata[i]){
				k = y_addata[j];
				y_addata[i] = y_addata[j];
				y_addata[j] = k;
			}
		}
	}
	
	#if LCD_MODE == 0
	*X_Addata=_AD320((x_addata[1] + x_addata[2]+ x_addata[3] + x_addata[4]) >> 2);	
	*Y_Addata=_AD240((y_addata[1] + y_addata[2] + y_addata[3] + y_addata[4]) >>2);	
	#endif
	#if LCD_MODE == 1
		*X_Addata=_AD320(0xfff-((x_addata[1] + x_addata[2] + x_addata[3] + x_addata[4]) >> 2));	
	*Y_Addata=_AD240(0xfff-((y_addata[1] + y_addata[2] + y_addata[3] + y_addata[4]) >>2));		
	
	#endif	
	
	
//	#if LCD_16B > 0
//		*X_Addata=_AD320(0xfff-((x_addata[1] + x_addata[2] + x_addata[3] + x_addata[4]) >> 2));
//	#endif
//	#if LCD_USE_FMSC > 0
////	GUI_API.X = (x_addata[1] + x_addata[2]+ x_addata[3] + x_addata[4]) >> 2;
//		*X_Addata=_AD320((x_addata[1] + x_addata[2]+ x_addata[3] + x_addata[4]) >> 2);
//	#endif	
////	GUI_API.Y = (y_addata[1] + y_addata[2] + y_addata[3] + y_addata[4]) >>2;
//	*Y_Addata=_AD240((y_addata[1] + y_addata[2] + y_addata[3] + y_addata[4]) >>2);
//GUI_SetPoint(*X_Addata,*Y_Addata,Red);
}


void TouchSystick200Routine(void)
{
	static u8 TouchState=0;																					//������״̬  
	static u16 TouchCount;
	u16 X,Y;
	u32 TouchMessage;
	switch(TouchState)      										 										//������״̬  
	{	
	case 0:             																						//ԭ��������״̬Ϊû�а���			
		if(!TOUCH_INT)						//��������
		{	
			TouchState = 1;       																			//��״̬�л���״̬1����һ�ΰ��£�      
		}             		
		break;         
	case 1:              					 																	//ȷ�ϰ�����һ�ΰ���				
	if(!TOUCH_INT)		 					//��һ�ΰ������º��ʼ��״̬
		{
			Touch_Rd_Addata(&X, &Y);		//��ȡ����������
			TouchState = 2;        																			//״̬��Ϊ״̬2����һ�κ���ʱ��         
			TouchCount = 0;         
			TouchMessage = TOUCH_DOWN <<20 |(X<<10 )|Y;			//����ֵ��ON 
			PostTouch(TouchMessage);										
		
		}             
		else                  																				//�ڰ�����λ
		{	
			TouchState = 0;   																					//״̬��Ϊ״̬0���ް�������״̬��
		}             
		break;        
	case 2:             																						//�����������£���time1����ʱʱ�仹û��
		if(!TOUCH_INT)						//��������
		{	
			if(TouchCount++ >= TOUCH_DELAY_TIME1)    										//״̬2--time1��ʱʱ�䵽   
			{	
				Touch_Rd_Addata(&X, &Y);		//��ȡ����������				
				TouchState = 3;       																		//״̬��Ϊ״̬3���ڶ��κ���ʱ��             
				TouchCount = 0;         																	//��������         
			TouchMessage = TOUCH_LONG <<20 |(X<<10 )|Y;										//����ֵ��LONG 
			PostTouch(TouchMessage);	
			}           
		}         
		else                 																					//������λ
		{	
			TouchState = 4;    
			TouchCount = 0;         																		//��������  						
			TouchMessage = TOUCH_UP <<20 |(X<<10 )|Y;			//����ֵ��UP 
			PostTouch(TouchMessage);	
		}             
		break;        
	case 3:             																						//�����������£�����ʱ�䳬��time1����time2�ظ���ʱ����ʱʱ�仹û��             
		if(!TOUCH_INT)						//��������
		{			
			if(TouchCount++ >= TOUCH_DELAY_TIME2)												//״̬3--time2��ʱʱ�䵽   
			{     
				Touch_Rd_Addata(&X, &Y);		//��ȡ����������							
				TouchCount = 0;            	   		
			TouchMessage = TOUCH_LONG <<20 |(X<<10 )|Y;										//����ֵ��LONG 
			PostTouch(TouchMessage);					
			}         
		}    
		else
		{                
			TouchState = 4; 
			TouchCount = 0;         																		//��������  					   
			TouchMessage = TOUCH_UP <<20 |(X<<10 )|Y;			//����ֵ��UP 
			PostTouch(TouchMessage);				
		}           
		break; 
	case 4:    
		if(TouchCount < DoubleHitInterval)    												//��ʱ���ظ�����ȡ��ʱ��
		{
				TouchCount ++;
		}
		else
		{
			TouchState = 0; 					
		}
		break;			
	default:  
		TouchState = 0;       																				//״̬Ϊ�ǳ���״̬��״̬�л���״̬0      
		break;   
	}				
}





	#endif
