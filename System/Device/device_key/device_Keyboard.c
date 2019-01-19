/*********************************************************************************
* �ļ���			: device_key.c
* �汾��    	: V2.0.0
* ����      : 2015/07/5
* ˵��      : 4*4���̵���������,������ʱ��չ��m*n									  
* �÷�			��
�� IN_Keyboard �� OUT_Keyboard �ж���m*n�������õĶ˿�

#define KEY_TIME1		30	//������һ��ʱ��ʱʱ��  =KEY_TIME1*ÿ��ɨ�������20ms
#define KEY_TIME2		20	//�����ڶ���ʱ��ʱʱ��  =KEY_TIME2*ÿ��ɨ�������20ms

**********************************************************************************/

#include "device_key.h"
#include "device.h"
#include "device_CONF.h"



/**************************�ֲ���������****************************************************/



/**************************ȫ�ֱ�������****************************************************/
const u8 PaiLie[]={			//���м�λ�����ã����Խ����������е������ͼ���һ��
	13,12,11,10,
	15,9,6,3,
	0,8,5,2,
	14,7,4,1,
};
const IO_TypeDef	OUT_Keyboard[]={
	{RCC_D , GPIOD , GPIO_Pin_0},
	{RCC_D , GPIOD , GPIO_Pin_2},	
	{RCC_D , GPIOD , GPIO_Pin_4},
	{RCC_D , GPIOD , GPIO_Pin_6},	
};

const IO_TypeDef	IN_Keyboard[]={
	{RCC_G , GPIOG , GPIO_Pin_9},		
	{RCC_G , GPIOG , GPIO_Pin_11},	
	{RCC_G , GPIOG , GPIO_Pin_13},	
	{RCC_G , GPIOG , GPIO_Pin_15},	
};




KEY_TypeDef IO_Keyboard[16];


/********************************************
������Initialize_KeyboardIO
���ܣ����󰴼��˿ڳ�ʼ��
��������
���أ���
˵����
********************************************/
void Initialize_KeyboardIO(void)
{
	u8 i;
	/* ����ɨ������� ����͵�ƽ */	
	for (i=0;i<4;i++){
		IO_OUT_PP_HIGH(OUT_Keyboard[i].Port,OUT_Keyboard[i].Pin,OUT_Keyboard[i].Rcc);
		GPIO_ResetBits(((GPIO_TypeDef *)OUT_Keyboard[i].Port), OUT_Keyboard[i].Pin);			
	}
	/* ����ɨ�������� ������ʱ����͵�ƽ �ſ�����ߵ�ƽ */	
	for (i=0;i<4;i++){
		IO_IN_IPD(IN_Keyboard[i].Port,IN_Keyboard[i].Pin,IN_Keyboard[i].Rcc);
	}	
}

void ReadKeyboard(void)
{
	u8 j,k;
	u32 key = 0;
	for(k = 0; k < 4; k++)
	{
		GPIO_SetBits (((GPIO_TypeDef *)OUT_Keyboard[k].Port), OUT_Keyboard[k].Pin);			
		for(j = 0; j < 4; j++)
		{
			key = PaiLie[k*4+j];																//	���Խ��а������д���
			switch(IO_Keyboard[key].State)      										//������״̬   
			{	
			case 0:             																//ԭ��������״̬Ϊû�а���
				if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin)){			//��������
					IO_Keyboard[key].State = 1;       										//��״̬�л���״̬1����һ�ΰ��£�      
				}             
				break;         
			case 1:              					 											//ȷ�ϰ�����һ�ΰ���
			if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin))		 		//��һ�ΰ������º��ʼ��״̬
				{
					IO_Keyboard[key].State = 2;        									//״̬��Ϊ״̬2����һ�κ���ʱ��         
					IO_Keyboard[key].Cont = 0;          
				PostKeyboard(key | KEYON<<8);										//����ֵ��ON 
				
				}             
				else                  														//�ڰ�����λ
				{	
					IO_Keyboard[key].State = 0;   												//״̬��Ϊ״̬0���ް�������״̬��
					PostKeyboard(key | KEYOFF<<8);   							//����ֵ��OFF  					
 				}             
				break;        
			case 2:             																//�����������£���time1����ʱʱ�仹û��
				if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin))			//��������
				{	
					if(IO_Keyboard[key].Cont++ >= KEY_TIME1)    				//״̬2--time1��ʱʱ�䵽   
					{	
						IO_Keyboard[key].State = 3;       									//״̬��Ϊ״̬3���ڶ��κ���ʱ��             
						IO_Keyboard[key].Cont = 0;         								//��������         
						PostKeyboard(key | KEYON<<8);								//����ֵ��ON 
					}           
				}         
				else                 														//������λ
				{	
					IO_Keyboard[key].Cont = 0;         									//��������  
					IO_Keyboard[key].State = 4;    
					PostKeyboard(key | KEYOFF<<8);   							//����ֵ��OFF  
				}             
				break;        
			case 3:             																//�����������£�����ʱ�䳬��time1����time2�ظ���ʱ����ʱʱ�仹û��             
				if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin))			//��������
				{			
					if(IO_Keyboard[key].Cont++ >= KEY_TIME2)							//״̬3--time2��ʱʱ�䵽   
					{               
						IO_Keyboard[key].Cont = 0;            	   		
						PostKeyboard(key | KEYON<<8);								//����ֵ��ON 						
					}         
				}    
				else
				{                
					IO_Keyboard[key].Cont = 0;         //��������  
					IO_Keyboard[key].State = 4;    
					PostKeyboard(key | KEYOFF<<8);   							//����ֵ��OFF  					
				}           
				break; 
			case 4:    
        if(IO_Keyboard[key].Cont < DoubleHitInterval)    //��ʱ���ظ�����ȡ��ʱ�䣬�¼�δ����
        {
            IO_Keyboard[key].Cont ++;
        }
				else
				{
					IO_Keyboard[key].State = 0; 					
				}
				break;			
			default:  
				IO_Keyboard[key].State = 0;       //״̬Ϊ�ǳ���״̬��״̬�л���״̬0      
				break;   
			}			
		}
	GPIO_ResetBits	(((GPIO_TypeDef *)OUT_Keyboard[k].Port), OUT_Keyboard[k].Pin);			
	}	
	
}





