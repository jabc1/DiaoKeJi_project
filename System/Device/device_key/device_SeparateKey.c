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
#if(ISUSESEPARATEKEY)		
	#if 	(USE_DRV ==	USE_I_DRV)				/*����I�Ű�����ض���*/
		const IO_TypeDef	IN_SeparateKey[]={
			{RCC_A , GPIOA , GPIO_Pin_0},		
			{RCC_A , GPIOA , GPIO_Pin_1},	
		};
		const u8 SeparateKeyData[]={
			21,22
		};
		#define SEPARATEKEYNUM		2
		KEY_TypeDef IO_SeparateKey[2];
	#endif
	#if 	(USE_DRV ==	USE_III_DRV)		/*����III�Ű�����ض���*/
		const IO_TypeDef	IN_SeparateKey[]={
			{RCC_D , GPIOD , GPIO_Pin_3},		
			{RCC_A , GPIOA , GPIO_Pin_8},	
			{RCC_C , GPIOC , GPIO_Pin_13},		
			{RCC_A , GPIOA , GPIO_Pin_0},		
		};	
			const u8 SeparateKeyData[]={
			10,11,14,15
		};
		#define SEPARATEKEYNUM		4
		KEY_TypeDef IO_SeparateKey[4];
	#endif
	#if 	(USE_DRV ==	USE_MIN_DRV)			/*��Сϵͳ������ض���*/
		const IO_TypeDef	IN_SeparateKey[]={
			{RCC_E , GPIOE , GPIO_Pin_0},		
			{RCC_C , GPIOC , GPIO_Pin_13},	
		};	
		const u8 SeparateKeyData[]={
			21,22
		};
		#define SEPARATEKEYNUM		2
		KEY_TypeDef IO_SeparateKey[2];
	#endif
#endif	


/********************************************
������Initialize_SeparateKeyIO
���ܣ����������˿ڳ�ʼ��
��������
���أ���
˵����ͨ���궨����ȷ���������̵Ĺܽ�
********************************************/
void Initialize_SeparateKeyIO(void)
{		
	u8 i;
	for (i=0;i<SEPARATEKEYNUM;i++){
		IO_IN_IPD(IN_SeparateKey[i].Port,IN_SeparateKey[i].Pin,IN_SeparateKey[i].Rcc);
	}	
}

/********************************************
������ReadKeyboard
���ܣ����󰴼������ж�
��������
���أ���
˵����
�����ǱȽϸ��ӡ�������״̬����ʾ��  
ÿ��һ������ִ��һ�μ���ɨ������ 
state״̬��
0���޼������м�����״̬1   
1����һ�μ쵽�м������м�����״̬2��ͬʱ����ֵ�ͻ������� ���޼�����״̬0��  
2�����м����ʱ����������600ms����״̬3��ͬʱ����ֵ�ͻ������� ���޼�����״̬1����ʾ��һ�ΰ����������������600ms����Ϊ��һ�ΰ�����  
3�����м����ʱ����������400ms�򽫼�ֵ�ͻ������� ���޼�����״̬1��   ��ʾ���������Ժ�ÿ200ms����Ϊ��һ�ΰ�����
����ִ��һ�μ���ɨ������ 
********************************************/
void ReadSeparateKey(void)
{
	u8 i;
		for(i = 0; i < SEPARATEKEYNUM; i++)
		{
			switch(IO_SeparateKey[i].State)      										//������״̬   
			{	
			case 0:             																//ԭ��������״̬Ϊû�а���
				if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin)){			//��������
					IO_SeparateKey[i].State = 1;       										//��״̬�л���״̬1����һ�ΰ��£�      
				}             
				break;         
			case 1:              					 											//ȷ�ϰ�����һ�ΰ���
			if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin))		 		//��һ�ΰ������º��ʼ��״̬
				{
					IO_SeparateKey[i].State = 2;        									//״̬��Ϊ״̬2����һ�κ���ʱ��         
					IO_SeparateKey[i].Cont = 0;          
				PostSeparateKey(SeparateKeyData[i] | KEYON<<8);										//����ֵ��ON 
				
				}             
				else                  														//�ڰ�����λ
				{	
					IO_SeparateKey[i].State = 0;   												//״̬��Ϊ״̬0���ް�������״̬��
					PostSeparateKey(SeparateKeyData[i] | KEYOFF<<8);   							//����ֵ��OFF  					
 				}             
				break;        
			case 2:             																//�����������£���time1����ʱʱ�仹û��
				if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin))			//��������
				{	
					if(IO_SeparateKey[i].Cont++ >= KEY_TIME1)    				//״̬2--time1��ʱʱ�䵽   
					{	
						IO_SeparateKey[i].State = 3;       									//״̬��Ϊ״̬3���ڶ��κ���ʱ��             
						IO_SeparateKey[i].Cont = 0;         								//��������         
						PostSeparateKey(SeparateKeyData[i] | KEYLONG<<8);								//����ֵ��ON 
					}           
				}         
				else                 														//������λ
				{	
					IO_SeparateKey[i].Cont = 0;         									//��������  
					IO_SeparateKey[i].State = 4;    
					PostSeparateKey(SeparateKeyData[i] | KEYOFF<<8);   							//����ֵ��OFF  
				}             
				break;        
			case 3:             																//�����������£�����ʱ�䳬��time1����time2�ظ���ʱ����ʱʱ�仹û��             
				if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin))			//��������
				{			
					if(IO_SeparateKey[i].Cont++ >= KEY_TIME2)							//״̬3--time2��ʱʱ�䵽   
					{               
						IO_SeparateKey[i].Cont = 0;            	   		
						PostSeparateKey(SeparateKeyData[i] | KEYLONG<<8);								//����ֵ��ON 						
					}         
				}    
				else
				{                
					IO_SeparateKey[i].Cont = 0;         //��������  
					IO_SeparateKey[i].State = 4;    
					PostSeparateKey(SeparateKeyData[i] | KEYOFF<<8);   							//����ֵ��OFF  					
				}           
				break; 
			case 4:    
        if(IO_SeparateKey[i].Cont < DoubleHitInterval)    //��ʱ���ظ�����ȡ��ʱ�䣬�¼�δ����
        {
            IO_SeparateKey[i].Cont ++;
        }
				else
				{
					IO_SeparateKey[i].State = 0; 					
				}
				break;			
			default:  
				IO_SeparateKey[i].State = 0;       //״̬Ϊ�ǳ���״̬��״̬�л���״̬0      
				break;   
			}			
		}
}






