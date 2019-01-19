#ifndef __GUI_FILELIST_H__
#define __GUI_FILELIST_H__
#include "stm32f10x.h"  
#include "GUI_form.h" 
#include "GUI_conf.h" 
#include "GUI.h"


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//GUI-�ļ��б�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2012/10/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//����˵�� 
//V1.1 20140715
//1,�޸Ĺ�����������,ʹ�ø�����
//2,ȥ��FLBOX_PATH_HEIGHT�Ⱥ궨��,���ñ�����ʽ����(��gui_phy�ṹ����������)
//////////////////////////////////////////////////////////////////////////////////

#define GUI_USEFILELIST


//�ļ�ͼ�궨��
#define FICO_DISK				0  			//����
#define FICO_FOLDER		 		1			//�ļ���
#define FICO_FILE		 		0X10			//�ļ�
#define FICO_UNKNOW				2			//δ֪�ļ�
#define FICO_BIN				3			//BIN�ļ�
#define FICO_LRC				4			//LRC�ļ�
#define FICO_NES				5			//NES�ļ�
#define FICO_TEXT				6			//�ı��ļ�
#define FICO_MUSIC				7			//�����ļ�
#define FICO_PICTURE			8			//ͼƬ�ļ�
#define FICO_VIDEO				9			//��Ƶ�ļ�

//�ļ���������λ���ܶ���
#define FLBOX_FLT_BIN 			1<<0		//BIN�ļ�
#define FLBOX_FLT_LRC	 		1<<1		//LRC�ļ�
#define FLBOX_FLT_NES	 		1<<2		//NES�ļ�
#define FLBOX_FLT_TEXT	 		1<<3		//�ı��ļ�
#define FLBOX_FLT_MUSIC	 		1<<4		//�����ļ�
#define FLBOX_FLT_PICTURE 		1<<5		//ͼƬ�ļ�
#define FLBOX_FLT_VIDEO			1<<6		//��Ƶ�ļ�
#define FLBOX_FLT_UNKNOW		1<<15		//�޷�ʶ���һ���ļ�

//////filelistbox�ϵķ��ŵ�Ĭ����ɫ 
#define D_FILELIST_LBCOLOR		0XFFFF		//�ڲ�����ɫ
#define D_FILELIST_LSCOLOR		0XFFFF		//ѡ��list���������ɫ
#define D_FILELIST_LSBCOLOR	0X001F		//0X07E0		//ѡ��list��ı���ɫ
#define D_FILELIST_FCOLOR		0X0000		//δѡ�е�list������ɫ
#define D_FILELIST_RIMCOLOR		0XF800		//�߿���ɫ
#define D_FILELIST_IBCOLOR	0XCE7E		//·��/��Ϣ���ı�����ɫ


//filelistbox�ļ��洢��ȶ���,���������������,�����ټ�����һ����ѡ��Ŀ��λ��.
#define FLBOX_PATH_DEPTH		0X0A		//�ļ�·�����
//filelistbox�����ļ�����������Ŀ���ļ�������Ŀ
#define FLBOX_MAX_DFFILE 		200		//ʹ��12*12������ 2K��,�������ļ��е�ʱ��,��Ҫռ��2K*2���ڴ�
#define		FILELIST_LIST_H 	(FileList_P->Font->Y +4)   		//��������ˮƽ�Ļ��Ǵ�ֱ��		

//�̶���߶ȶ���
#define FILEINFO_WIDTH 		19		//���������Ϊ15������ 
//filelist�ṹ��.����ṹ
typedef struct  GUI_FileQueueStruct
{
	struct GUI_FileQueueStruct * prevQueue;
	struct GUI_FileQueueStruct * nextQueue;
	u8 type;		//�ļ�����
	u8 name[21];		//�ļ�����
}GUI_FileQueueStruct;


	typedef struct
	{
		u8 ID;	
		u8 State;		
		u16 X,Y;
		u16 W, H;		
//		u16 BColor,FColor,SelectColor;	
		u16 L_B_Color;					//�ڲ�������ɫ
		u16 L_S_Color; 				//list name ѡ�к����ɫ
		u16 L_SB_Color; 				//list name ѡ�к�ı�����ɫ
		u16 F_Color; 					//list name δѡ�е���ɫ
		u16 Rim_Color;					//�߿���ɫ
		u16 I_B_Color;			    //·��/��Ϣ���ı�����ɫ		
		GUI_FontStruct *Font;
		u16 selindex;					//ѡ�е����� 0<selindex<foldercnt+filecnt;		
		u16 foldercnt;					//�ļ�����Ŀ
		u16 filecnt;					//�ļ���Ŀ		
		u8* path;						//��Ŀ¼·��
		u16 seltbl[FLBOX_PATH_DEPTH];	//ѡ�е���Ŀ�б�,FLBOX_PATH_DEPTHΪ���
		u16 fliter;						//�ļ�������,�������
					//ˢѡ�������Ķ���
		u16 fliterex;					//�ļ���չ�Ĺ�����
					//����ѡ����������С��,���֧��16�ֲ�ͬ���ļ�.Ĭ��0XFFFF,������С�����ˢѡ.

		u8 memdevflag;					//������λ״̬(�����Ա�ʾ8������)
					//bit7:0,�洢�豸7~0��λ״̬;0,����λ;1,��λ;
					//������,�洢�豸0��ʾ:SD��
					//              1��ʾ:SPI FLASH
					//              2��ʾ:U��
		//////////////////////
		u8 *fname;						//��ǰѡ�е�index������
		u16 namelen;					//name��ռ�ĵ���.
		u16 curnamepos;					//��ǰ��ƫ��
		u32 OsPriority;					//��һ�θ���ʱ�� 

		//����ֻ��Ŀ���ļ����(�����ļ���)
		u16 *findextbl;    				//Ŀ���ļ���������	

		GUI_ScrollbarStruct * Scrollbar;			//��ֱ������
		GUI_FileQueueStruct *list; 		//����	    		
		
	}GUI_FileListStruct;



	
	
/* 	 state = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----ON/OFF				= �ؼ�״̬ΪON/OFF��ͬʱlable�Ȳ���ҪON/OFF����Ϊ��Ч״̬
										|||| +-----����					
										|||+-------Init 				=0:��Ҫ��ʼ���� 1����ʼ�����
										||+--------Scrollbar		= 0������Ҫ��ʾ 1����Ҫ��ʾ	
										|+---------����			
										+----------�Ƿ�Ϊ����״̬*/

	
//	#define		SCOROLLBAR_MODE 	0x10   		//��������ˮƽ�Ļ��Ǵ�ֱ��
	#define		FILELIST_INIT 	0x10   		//��������ˮƽ�Ļ��Ǵ�ֱ��	
	#define		FILELIST_TOUCH 	0x04   		//�ؼ��Ƿ��������
	#define		FILELIST_CLOSE 	0x08   		//�رհ�ť�Ƿ񱻰���	
	#define		FILELIST_NEEDSCROLLBAR 	0x20   		//��������ˮƽ�Ļ��Ǵ�ֱ��		
	
	
	
GUI_FileListStruct * GUI_FileListCreateAsChild(GUI_FormStruct *Form_P, u16 X,u16 Y,u16 W,u16 H);	
	

void GUI_FileListShowNow(GUI_FileListStruct *FileList_P);
u8 GUI_FileListTouchDown(GUI_FileListStruct *GUI_FileList_P,u8 Num);
void GUI_FileListTouchUp(GUI_FileListStruct *GUI_FileList_P);
u8 GUI_FileListTouchLong(GUI_FileListStruct *FileList_P);

////filelistbox�ṹ�嶨��		  
//__packed typedef struct 
//{
//	u16 top; 				  		//filelistbox��������
//	u16 left;                       //filelistbox�������
//	u16 width; 				  		//���
//	u16 height;						//�߶� ����Ϊ12/16�ı���,����б߿�,�ڱ��������ϼ�2

//	u8 type;						//���ͱ����
//									//[bit7]:1,��Ҫ������������(������totalitems>itemsperpage);0,����Ҫ������.(��λ������Զ�����)
//									//[bit6:0]:�ļ�����					 
//								
//	u8 sta;							//filelistbox״̬,[bit7]:������־;[bit6]:�����Ч�ı�־;[bit5:0]:��һ�ΰ��µı��.	 
//	u8 id;							//filelistbox ��id
//	u8 dbclick;					   	//˫��,
//									//[7]:0,û��˫��.1,��˫��.
//									//[0]:0,��Ŀ���ļ�(���ļ���/����).1,Ŀ���ļ�(���ļ���/����)

//	u8 font;						//�������� 12/16
//	u16 selindex;					//ѡ�е����� 0<selindex<foldercnt+filecnt;

//	u16 foldercnt;					//�ļ�����Ŀ
//	u16 filecnt;					//�ļ���Ŀ
//	
//	u8 icosize;						//icoͼ���С 16/20/28��
// 
//	u16 lbkcolor;					//�ڲ�������ɫ
// 	u16 lnselcolor; 				//list name ѡ�к����ɫ
//  	u16 lnselbkcolor; 				//list name ѡ�к�ı�����ɫ
//	u16 lncolor; 					//list name δѡ�е���ɫ
//	u16 rimcolor;					//�߿���ɫ
//	u16 pthinfbkcolor;			    //·��/��Ϣ���ı�����ɫ

//	u8* path;						//��Ŀ¼·��
//	//u8  seltblindex;				//��ǰ��Ŀ¼����
//	u16 seltbl[FLBOX_PATH_DEPTH];	//ѡ�е���Ŀ�б�,FLBOX_PATH_DEPTHΪ���
//	u16 fliter;						//�ļ�������,�������
//									//ˢѡ�������Ķ���
//	u16 fliterex;					//�ļ���չ�Ĺ�����
//									//����ѡ����������С��,���֧��16�ֲ�ͬ���ļ�.Ĭ��0XFFFF,������С�����ˢѡ.

//	u8 memdevflag;					//������λ״̬(�����Ա�ʾ8������)
//									//bit7:0,�洢�豸7~0��λ״̬;0,����λ;1,��λ;
//									//������,�洢�豸0��ʾ:SD��
//									//              1��ʾ:SPI FLASH
//									//              2��ʾ:U��
////////////////////////
//	u8 *fname;						//��ǰѡ�е�index������
//	u16 namelen;					//name��ռ�ĵ���.
//	u16 curnamepos;					//��ǰ��ƫ��
//	u32 oldtime;					//��һ�θ���ʱ�� 

//    //����ֻ��Ŀ���ļ����(�����ļ���)
// 	u16 *findextbl;    				//Ŀ���ļ���������	

//	GUI_ScrollbarStruct * scbv;			//��ֱ������
//	_filelistbox_list *list; 		//����	    
//}GUI_FileListStruct;


GUI_FileListStruct * filelistbox_creat(u16 left,u16 top,u16 width,u16 height,u8 type,u8 font);//����filelistbox
GUI_FileQueueStruct * filelist_search(GUI_FileQueueStruct *filelistx,u16 index);		//���ұ��Ϊindex��list,��ȡ����ϸ��Ϣ
u8 GUI_FILELIST_addlist(GUI_FileListStruct * filelistbox,u8 *name,u8 type);	//����һ��filelist



void filelist_delete(GUI_FileListStruct * filelistbox);						//ɾ��filelist����
//void filelistbox_delete(GUI_FileListStruct *filelistbox_del);	 				//ɾ��filelistbox
u8 filelistbox_rebuild_filelist(GUI_FileListStruct * filelistbox);			//�ؽ�filelistbox
u8 filelistbox_check(GUI_FileListStruct * filelistbox,void * in_key);			//���filelistbox�İ���״̬

//void filelistbox_draw_list(GUI_FileListStruct *filelistbox);					//��filelist
//void filelistbox_draw_listbox(GUI_FileListStruct *filelistbox);				//��filelistbox
void filelistbox_show_path(GUI_FileListStruct *filelistbox); 					//��ʾ·��
void filelistbox_show_info(GUI_FileListStruct *filelistbox);
u8   filelistbox_scan_filelist(GUI_FileListStruct *filelistbox);
void filelistbox_add_disk(GUI_FileListStruct *filelistbox);
void filelistbox_check_filelist(GUI_FileListStruct *filelistbox);
void filelistbox_back(GUI_FileListStruct *filelistbox);
void filelistbox_2click_hook(GUI_FileListStruct *filelistbox);				//˫�����Ӻ���

u8 GUI_FileListDelete(GUI_FileListStruct *FileList_P);			//ɾ��FILELIST�ؼ�





	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
//	#define		GUI_USEFileList
//	#define	FileList_MEMNUM				20+FileList_MAXSTRINLEN			//FileList ������ʱ��������ڴ�����
//#define 	FileList_MAXSTRINLEN		18

	
	
	
	
	
	
	

//	
//	
//	
///* 	 state = 0x00;  0000 0000  
//										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
//										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
//										|||| |+----����			
//										|||| +-----����
//										|||+-------FOCUSFileList		= ���selectLen>0 	0:ֻ����ѡ��״̬��1�� FileList״̬ 	
//										||+--------COLORTYPE		=	FocusLen ѡ������� 0: ��ʾ������ɫ��1�� ��ʾ��ת��ɫ
//										|+---------����
//										+----------�Ƿ�Ϊ����״̬*/
//	#define FileList_FOCUSFileList		0X10										//FileList�Ľ���״̬
//	#define FileList_COLORTYPE		0X20										//����ѡ���������ɫ״̬	

//	
//	GUI_FileListStruct *GUI_FileListCreateAsChild(GUI_FormStruct *Form_P, u16 X, u16 Y ,u16 W,u16 H,u8 Type,u8 Font);
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	u8 GUI_FileListSetString(GUI_FileListStruct *FileList_P,u8 *String);
//	void GUI_FileListGetString(GUI_FileListStruct *FileList_P,u8 *String);
//	u8 GUI_FileListSetValue(GUI_FileListStruct *FileList_P,u32 Value);	
////	u32 GUI_FileListGetValue(GUI_FileListStruct *FileList_P);	
//	extern void GUI_FileListShowNow(GUI_FileListStruct *FileList_P);
//	extern void GUI_FileListLostFocus(GUI_FileListStruct *FileList_P);
//	extern void GUI_FileListGetFocus(GUI_FileListStruct *FileList_P);
//	extern void GUI_FileListKeyFirst(GUI_FileListStruct *FileList_P);		
//	extern void GUI_FileListTouchUp(GUI_FileListStruct *FileList_P);
//	extern u8 GUI_FileListTouchDown(GUI_FileListStruct *FileList_P,u8 Num);	
	
#endif 
