/*

*/

#ifndef __Moto_4988_H
#define __Moto_4988_H
#include "stm32f10x.h" 
//#define POSITIVE 	1			//电机正向
//#define REVERSE 	0			//电机反向


	#define MOTO1_STEP_PIN      			GPIO_Pin_1
	#define MOTO1_STEP_GPIO     		 	GPIOA
	#define MOTO1_STEP_RCC       			RCC_APB2Periph_GPIOA
	#define MOTO1_DIR_PIN      				GPIO_Pin_0
	#define MOTO1_DIR_GPIO     				GPIOA
	#define MOTO1_DIR_RCC      				RCC_APB2Periph_GPIOA	
	#define MOTO1_DIR_O									PAOut(0)
	#define MOTO1_DIR_I									PAIn(0)	

	#define MOTO2_STEP_PIN      			GPIO_Pin_1
	#define MOTO2_STEP_GPIO     			GPIOB
	#define MOTO2_STEP_RCC      			RCC_APB2Periph_GPIOB
	#define MOTO2_DIR_PIN      				GPIO_Pin_0
	#define MOTO2_DIR_GPIO     				GPIOB
	#define MOTO2_DIR_RCC      				RCC_APB2Periph_GPIOB
	#define MOTO2_DIR_O									PBOut(0)
	#define MOTO2_DIR_I									PBIn(0)	

	#define MOTO3_STEP_PIN      			GPIO_Pin_7
	#define MOTO3_STEP_GPIO     			GPIOB
	#define MOTO3_STEP_RCC      			RCC_APB2Periph_GPIOB
	#define MOTO3_DIR_PIN      			GPIO_Pin_8
	#define MOTO3_DIR_GPIO     		 	GPIOB
	#define MOTO3_DIR_RCC       			RCC_APB2Periph_GPIOB
	#define MOTO3_DIR_O							PBOut(8)
	#define MOTO3_DIR_I							PBIn(8)	

	#define MOTO_MAIN_PIN      				GPIO_Pin_9
	#define MOTO_MAIN_GPIO     				GPIOB
	#define MOTO_MAIN_RCC      				RCC_APB2Periph_GPIOB
	#define MOTO_MAIN_ON							PBOut(9) = 0
	#define MOTO_MAIN_OFF							PBOut(9) = 1
	
	#define MOTO_EN_PIN      					GPIO_Pin_2
	#define MOTO_EN_GPIO     					GPIOA
	#define MOTO_EN_RCC      					RCC_APB2Periph_GPIOA
	#define MOTO_ON										PAOut(2) = 0;DK.State_B_P->Axis_EN = 0;
	#define MOTO_OFF									PAOut(2) = 1;DK.State_B_P->Axis_EN = 1;

	
	#define STEP_TO_PUSH			64
	
	#define M_X_DIR *(Moto[AXIS_X].DIR)		
	#define M_Y_DIR *(Moto[AXIS_Y].DIR)	
	#define M_Z_DIR 					*(Moto[AXIS_Z].DIR)	
	
	#define Moto_DIR(AXIS)	*(Moto[AXIS].DIR)	
	#define Moto_CCR(AXIS)	 *(Moto[AXIS].TIM_CCR)	
	#define Moto_PSC(AXIS)	 *(Moto[AXIS].TIM_PSC)	
	
	
	

	
	#define WAIT_Z_FLAG while(Moto[AXIS_Z].Flag)OS.DelayTimeTick(1);   //电机运行BUSY信号，必须等这个标志为0的时候才能进行下一次的运行


typedef struct
{
	s32 Current;
	s32 Target;
	vu32 *DIR;
	vu16 *TIM_CCR;
	vu16 *TIM_PSC;
	u8 Flag;	
//	u8 SpeedState;
	u16* Speed;
}MOTO_Struct;

extern MOTO_Struct Moto[3];

extern void Initialize_Moto(void);
extern void Stop_Moto(u8 Axis);

extern void Moto_Move(u8 AXIS,s32 Step);//正向运行 step为需要运行的脉冲数量，不管1个周期需要多少脉冲
extern void MotoGotoZero(void);
extern void Moto_Wait_XY(void);
extern void Set_Speed_Z(u16 Speed);
extern void Set_Speed_XY(u16 Speed);
extern void Run_MainMoto(void);
extern void Stop_MainMoto(void);


#endif 

