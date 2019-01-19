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



/**************************全局变量定义****************************************************/
const u8 PaiLie[]={			//进行键位排列用，可以将按键的排列调整到和键盘一致
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
函数：Initialize_KeyboardIO
功能：矩阵按键端口初始化
参数：无
返回：无
说明：
********************************************/
void Initialize_KeyboardIO(void)
{
	u8 i;
	/* 键盘扫描输出线 输出低电平 */	
	for (i=0;i<4;i++){
		IO_OUT_PP_HIGH(OUT_Keyboard[i].Port,OUT_Keyboard[i].Pin,OUT_Keyboard[i].Rcc);
		GPIO_ResetBits(((GPIO_TypeDef *)OUT_Keyboard[i].Port), OUT_Keyboard[i].Pin);			
	}
	/* 键盘扫描输入线 键被按时输入低电平 放开输入高电平 */	
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
			key = PaiLie[k*4+j];																//	可以进行按键排列处理
			switch(IO_Keyboard[key].State)      										//按键的状态   
			{	
			case 0:             																//原来按键的状态为没有按下
				if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin)){			//按键按下
					IO_Keyboard[key].State = 1;       										//将状态切换到状态1（第一次按下）      
				}             
				break;         
			case 1:              					 											//确认按键第一次按下
			if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin))		 		//第一次按键按下后初始化状态
				{
					IO_Keyboard[key].State = 2;        									//状态置为状态2（第一次后延时）         
					IO_Keyboard[key].Cont = 0;          
				PostKeyboard(key | KEYON<<8);										//将键值置ON 
				
				}             
				else                  														//在按键复位
				{	
					IO_Keyboard[key].State = 0;   												//状态置为状态0（无按键按下状态）
					PostKeyboard(key | KEYOFF<<8);   							//将键值置OFF  					
 				}             
				break;        
			case 2:             																//按键持续按下，但time1的延时时间还没到
				if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin))			//按键按下
				{	
					if(IO_Keyboard[key].Cont++ >= KEY_TIME1)    				//状态2--time1延时时间到   
					{	
						IO_Keyboard[key].State = 3;       									//状态置为状态3（第二次后延时）             
						IO_Keyboard[key].Cont = 0;         								//计数清零         
						PostKeyboard(key | KEYON<<8);								//将键值置ON 
					}           
				}         
				else                 														//按键复位
				{	
					IO_Keyboard[key].Cont = 0;         									//计数清零  
					IO_Keyboard[key].State = 4;    
					PostKeyboard(key | KEYOFF<<8);   							//将键值置OFF  
				}             
				break;        
			case 3:             																//按键持续按下，进入时间超过time1，但time2重复计时的延时时间还没到             
				if(GPIO_IN((IN_Keyboard[j].Port), IN_Keyboard[j].Pin))			//按键按下
				{			
					if(IO_Keyboard[key].Cont++ >= KEY_TIME2)							//状态3--time2延时时间到   
					{               
						IO_Keyboard[key].Cont = 0;            	   		
						PostKeyboard(key | KEYON<<8);								//将键值置ON 						
					}         
				}    
				else
				{                
					IO_Keyboard[key].Cont = 0;         //计数清零  
					IO_Keyboard[key].State = 4;    
					PostKeyboard(key | KEYOFF<<8);   							//将键值置OFF  					
				}           
				break; 
			case 4:    
        if(IO_Keyboard[key].Cont < DoubleHitInterval)    //短时间重复按键取消时间，新加未测试
        {
            IO_Keyboard[key].Cont ++;
        }
				else
				{
					IO_Keyboard[key].State = 0; 					
				}
				break;			
			default:  
				IO_Keyboard[key].State = 0;       //状态为非常用状态则将状态切换到状态0      
				break;   
			}			
		}
	GPIO_ResetBits	(((GPIO_TypeDef *)OUT_Keyboard[k].Port), OUT_Keyboard[k].Pin);			
	}	
	
}





