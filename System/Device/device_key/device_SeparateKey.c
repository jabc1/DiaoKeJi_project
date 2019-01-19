/*********************************************************************************
* 文件名			: device_key.c
* 版本号    	: V2.0.0
* 日期      : 2015/07/5
* 说明      : 4*4键盘的驱动程序,可以随时扩展成m*n									  
* 用法			：
在 IN_Keyboard 和 OUT_Keyboard 中定义m*n键盘所用的端口

#define KEY_TIME1		30	//按键第一次时延时时间  =KEY_TIME1*每次扫描的周期20ms
#define KEY_TIME2		20	//按键第二次时延时时间  =KEY_TIME2*每次扫描的周期20ms

**********************************************************************************/
#include "device_key.h"
#include "device.h"
#include "device_CONF.h"

/**************************局部函数定义****************************************************/
#if(ISUSESEPARATEKEY)		
	#if 	(USE_DRV ==	USE_I_DRV)				/*神州I号按键相关定义*/
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
	#if 	(USE_DRV ==	USE_III_DRV)		/*神州III号按键相关定义*/
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
	#if 	(USE_DRV ==	USE_MIN_DRV)			/*最小系统按键相关定义*/
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
函数：Initialize_SeparateKeyIO
功能：独立按键端口初始化
参数：无
返回：无
说明：通过宏定义来确定独立键盘的管脚
********************************************/
void Initialize_SeparateKeyIO(void)
{		
	u8 i;
	for (i=0;i<SEPARATEKEYNUM;i++){
		IO_IN_IPD(IN_SeparateKey[i].Port,IN_SeparateKey[i].Pin,IN_SeparateKey[i].Rcc);
	}	
}

/********************************************
函数：ReadKeyboard
功能：矩阵按键按键判断
参数：无
返回：无
说明：
按键是比较复杂。可以用状态机表示。  
每过一定周期执行一次键盘扫描任务 
state状态：
0、无键，若有键进入状态1   
1、第一次检到有键。若有键进入状态2，同时将键值送缓冲区。 若无键进入状态0。  
2、若有键则计时，按键超过600ms进入状态3，同时将键值送缓冲区。 若无键进入状态1。表示第一次按键后如果长按键则600ms后认为有一次按键。  
3、若有键则计时，按键超过400ms则将键值送缓冲区。 若无键进入状态1。   表示长按键则以后每200ms后认为有一次按键。
周期执行一次键盘扫描任务 
********************************************/
void ReadSeparateKey(void)
{
	u8 i;
		for(i = 0; i < SEPARATEKEYNUM; i++)
		{
			switch(IO_SeparateKey[i].State)      										//按键的状态   
			{	
			case 0:             																//原来按键的状态为没有按下
				if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin)){			//按键按下
					IO_SeparateKey[i].State = 1;       										//将状态切换到状态1（第一次按下）      
				}             
				break;         
			case 1:              					 											//确认按键第一次按下
			if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin))		 		//第一次按键按下后初始化状态
				{
					IO_SeparateKey[i].State = 2;        									//状态置为状态2（第一次后延时）         
					IO_SeparateKey[i].Cont = 0;          
				PostSeparateKey(SeparateKeyData[i] | KEYON<<8);										//将键值置ON 
				
				}             
				else                  														//在按键复位
				{	
					IO_SeparateKey[i].State = 0;   												//状态置为状态0（无按键按下状态）
					PostSeparateKey(SeparateKeyData[i] | KEYOFF<<8);   							//将键值置OFF  					
 				}             
				break;        
			case 2:             																//按键持续按下，但time1的延时时间还没到
				if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin))			//按键按下
				{	
					if(IO_SeparateKey[i].Cont++ >= KEY_TIME1)    				//状态2--time1延时时间到   
					{	
						IO_SeparateKey[i].State = 3;       									//状态置为状态3（第二次后延时）             
						IO_SeparateKey[i].Cont = 0;         								//计数清零         
						PostSeparateKey(SeparateKeyData[i] | KEYLONG<<8);								//将键值置ON 
					}           
				}         
				else                 														//按键复位
				{	
					IO_SeparateKey[i].Cont = 0;         									//计数清零  
					IO_SeparateKey[i].State = 4;    
					PostSeparateKey(SeparateKeyData[i] | KEYOFF<<8);   							//将键值置OFF  
				}             
				break;        
			case 3:             																//按键持续按下，进入时间超过time1，但time2重复计时的延时时间还没到             
				if(!GPIO_IN((IN_SeparateKey[i].Port), IN_SeparateKey[i].Pin))			//按键按下
				{			
					if(IO_SeparateKey[i].Cont++ >= KEY_TIME2)							//状态3--time2延时时间到   
					{               
						IO_SeparateKey[i].Cont = 0;            	   		
						PostSeparateKey(SeparateKeyData[i] | KEYLONG<<8);								//将键值置ON 						
					}         
				}    
				else
				{                
					IO_SeparateKey[i].Cont = 0;         //计数清零  
					IO_SeparateKey[i].State = 4;    
					PostSeparateKey(SeparateKeyData[i] | KEYOFF<<8);   							//将键值置OFF  					
				}           
				break; 
			case 4:    
        if(IO_SeparateKey[i].Cont < DoubleHitInterval)    //短时间重复按键取消时间，新加未测试
        {
            IO_SeparateKey[i].Cont ++;
        }
				else
				{
					IO_SeparateKey[i].State = 0; 					
				}
				break;			
			default:  
				IO_SeparateKey[i].State = 0;       //状态为非常用状态则将状态切换到状态0      
				break;   
			}			
		}
}






