/*********************************************************************************
*�ļ���     : app.c
*����       : MSOS����ϵͳӦ�ó������
*˵��				��
*ԭ��       : Ӧ�ò���ڣ�Ҳ��ϵͳ��ڣ���Ҫ��ʼ����������������

����Ĳ���������OS.H �ж���
#define TaskSum     			4           �������������ڶ�����4�����������4���ͱ����޸�
#define TaskStackSum   		100         ����ջ��ȣ�����������ö�Ļ����ֵ��Ӵ�
#define QueueStackSum			20					��Ϣ������ȣ��ܹ�������20���ȴ��������Ϣ������20���Ļ��ᶪʧ��Ϣ

��������ǰ���趨��������

���ȼ����ս�����˳�����罨���������ȼ���ߣ��������� �����������
�������GUI������������ȼ�����������OS.PendMessageQueue������OS.DelayTimeTick����;������Ϊ��ѭ����

********************************************************************************
*�汾     ����            ����            ˵��
*V2.0			ZZ					 2015/10/26				�������Լ������������GUI���򣬲������˻�����
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
***********************************************************************************/

#include "OS.h"
#include "app.h"
#include "dk_conf.h"
#include "device_conf.h"
#include "malloc.h"
#include "SD.h"
/*��������߼�����Ĳ���  */

u8 NRFTaskPriority ;		/*NRF��������*/
//u8 MotoTaskPriority ;		/*NRF��������*/
u8 ShowTaskPriority ;		/*NRF��������*/
u8 FileTaskPriority;




DiaoKe_Struct DK;  //��̻������ݽṹ��
/*******************************************************************************
* ������	: SystemTickRoutine
* ����	    : Ӧ�ò�ϵͳ���ģ�����ִ�й̶�������ܣ����ʱ��ȡ����ע�ắ��
* �������  	: ��
* ���ز���  	: ��
* ˵��  		: ѭ��ִ�� ��λ״̬-->Alarm��
********************************************************************************/

void SystickRoutine(void) 
{
    //�ص�������10msһ�α�ϵͳʱ�ӵ���
	

}
/*******************************************************************************
* ������	: InitializeData
* ����	    : ��ʼ��Ĭ�ϲ���
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
u8 read_s32(u8 *line, uint8_t *char_counter, s32 *Val)     ;
void InitializeData(void)
{
	DK.State_B_P =  (DK_B_StateStruct *)BitAddr(&(DK.State));
	DK.File_B_P = (DK_B_File_Struct *)BitAddr(&(DK.File));	//״̬��λ��λ�������� 
	DK.Alarm_B_P = (DK_B_Alarm_Struct* )BitAddr(&(DK.Alarm));		//������λ��λ��������

}
/*******************************************************************************
* ������	: InitializeDevice
* ����	    : ��ʼ���豸
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitializeDevice(void)
{
	JTAG_Set(SWD_ENABLE);		//�ر�JTAG,����SWD
	InitializeRtc();
	InitializeTouch();
	InitializeLcd();
	InitializeUsart();
	InitializeLED();	
	InitializeKey();	    
}

/*******************************************************************************
* ������	: InitializeApp
* ����	    : ��ʼ��App
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
static void InitializeApp(void)
{

	InitializeData();
	mem_init();		//��ʼ���ڴ��
	InitializeDevice();	
	OS.Systick.Register(Systick100, SystickRoutine);

}
 
/*******************************************************************************
* ������	: NVIC_Config
* ����	    : �ж����ó�ʼ������������л��ж����ȼ�
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void NVIC_Config(void)
{	
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif  	
}

/*******************************************************************************
* ������	: main
* ����	    : msOS�����������
*           : InitializeData:��ʼ���洢������
*           : System.Initialize:System��ʼ����ʵ�ֵײ�ϵͳ��ʼ�� 
*           : �����ʼ�����ȵģ����ȼ�Ҳ���ȣ�TaskSumҪ��������
*           : LogicTask: ҵ���߼����񣬳�ʼ��Ϊ������ȼ� 
*           : MenuTask: �������񣬳�ʼ��Ϊ������ȼ�,ע�⣬������Ϊ������ȼ�
* �������  : ��
* ���ز���  : int
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/03       ��ʼ�汾
*******************************************************************************/
int main(void) 
{

	NVIC_Config();
                                             //��ʼ��ϵͳ��
	InitializeOs();        
	InitializeApp();                                                    //��ʼ��Ӧ�ò�
	InitializeGUI(); 
	InitSD();			//�����
	NRFTaskPriority = OS.CreateTask(NRFTask); //����ҵ���߼��������ȼ�2 	
	FileTaskPriority = OS.CreateTask(FileTask);
//	MotoTaskPriority = OS.CreateTask(MotoTask); //����ҵ���߼��������ȼ�2 	
	ShowTaskPriority = OS.CreateTask(ShowTask);
	
	GUICreateTask();			//�������н���
  OS.Start();                                                  //MSOS����
}



