/*********************************************************************************
* �ļ���		: menu.h
* �汾��    : V1.0.0
* ����      : 2015/07/14
* ˵��      : ���庯��						  
* �÷�			��
* �汾˵��	��
**********************************************************************************/
#ifndef __SD_H__
#define __SD_H__
#include "stm32f10x.h" 
#include "ff.h"

//���ļ���ʽ�궨�壬���f_open����
#define FA_OPEN_DEFAULT			(uint8_t)(FA_OPEN_EXISTING | FA_READ | FA_WRITE)	//�ɶ�д����
#define FA_OPEN_READONLY		(uint8_t)(FA_OPEN_EXISTING | FA_READ)				//ֻ��ȡ����ִ��д
#define FA_OPEN_ADD_DATA		(uint8_t)(FA_OPEN_ALWAYS | FA_READ | FA_WRITE)		//�ļ��������򴴽����ļ�
                                                                                    //����f_lseek�������ļ���׷������
#define FA_OPEN_NEW_FAIL		(uint8_t)(FA_CREATE_NEW | FA_READ | FA_WRITE)		//�½��ļ������������ʧ��
#define FA_OPEN_NEW_COVER		(uint8_t)(FA_CREATE_ALWAYS | FA_READ | FA_WRITE)	//�½��ļ�����������򸲸�
//�ļ�ϵͳ��������ض���
extern FATFS *fs[_VOLUMES];	//�߼����̹�����
extern FIL *filescr;			//�ļ�1
extern FIL *filedst;			//�ļ�2
extern UINT br,bw;				//��д����
extern FRESULT f_res;			//FatFsͨ�ý����

extern uint8_t *SDdatabuff;	//SD�����ݻ���
extern uint16_t rlen;			//��ȡ�����ݳ���
extern DIR *dir;				//�ļ���
extern FILINFO fileinfo;		//�ļ���Ϣ�ṹ��

void InitSD(void);
#endif 

