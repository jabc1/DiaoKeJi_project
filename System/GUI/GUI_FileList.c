/*********************************************************************************
* �ļ���			: gui_FileList.c
* �汾��    	: V3.0.0
* ����			: ZZ
* ����      : 2017/02/04
* ˵��      : GUI FileList�ؼ�								  
* �÷�			��
 	   STATE = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0���ؼ��б仯��������ʾ��1���ؼ��ޱ仯
										|||| ||+---DIS					=	1���ؼ�Ϊ��Ч״̬
										|||| |+----����			
										|||| +-----����
										|||+-------NEEDREDRAW
										||+--------FOCUSFileList		= ���selectLen>0 	0:ֻ����ѡ��״̬��1�� FileList״̬ 	
										|+---------COLORTYPE
										+----------�Ƿ�Ϊ����״̬
**********************************************************************************/
#include "GUI_FileList.h" 
#include "GUI_DRAW2D.h"
#include "malloc.h"
#include "gui_conf.h"
#include "ff.h"
#if GUI_USEMSOS
#include "GUI_os.h"
#endif

/************************�Լ��ĺ�������********************************************************/
void GUI_FileList_Show_Path(GUI_FileListStruct *FileList_P);
u8 GUI_FileListbox_Scan(GUI_FileListStruct *FileList_P);
void GUI_FileListAddDisk(GUI_FileListStruct *FileList_P);
void GUI_FileListDrawList(GUI_FileListStruct *FileList_P);



//u8 Path[]={"0:\\abc\\bcd\\cef\\sdd\\fssd\\hjghjg\\hgjhjgj\\hgjghj"};
//u8 Path[]={"0:"};
//const u8 FileListSign[3][128]={	{	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X70,0X00,0X00,0X00,
//																0X00,0X00,0X00,0X07,0X77,0X00,0X00,0X00,0X00,0X00,0X00,0X77,0X77,0X70,0X00,0X00,
//																0X00,0X00,0X07,0X77,0X77,0X77,0X00,0X00,0X00,0X00,0X77,0X77,0X77,0X77,0X70,0X00,
//																0X00,0X07,0X77,0X77,0X77,0X77,0X77,0X00,0X00,0X77,0X77,0X77,0X77,0X77,0X73,0X00,
//																0X00,0X77,0X77,0X77,0X77,0X77,0X33,0X00,0X00,0X37,0X77,0X77,0X77,0X73,0X30,0X00,
//																0X00,0X33,0X77,0X77,0X77,0X33,0X00,0X00,0X00,0X03,0X37,0X77,0X73,0X30,0X00,0X00,
//																0X00,0X00,0X33,0X77,0X33,0X00,0X00,0X00,0X00,0X00,0X03,0X33,0X30,0X00,0X00,0X00,
//																0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//															},		//���� 
//															{ 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X75,0X55,0X55,0X55,0X50,0X00,0X00,
//																0X00,0X57,0X55,0X55,0X55,0X55,0X00,0X00,0X00,0X55,0X75,0X55,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X57,0X55,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X75,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,
//																0X00,0X05,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X00,0X55,0X77,0X00,0X00,0X00,0X00,
//																0X00,0X00,0X05,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//															},				//�ļ���
//															{ 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X55,0X55,0X55,0X55,0X00,0X00,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X50,0X00,0X00,0X05,0X30,0X00,0X00,0X00,0XA5,0X00,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,
//																0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X00,0X55,0X55,0X55,0X55,0X55,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//															}					//�ļ�
//};

//filelist����˳���,��ѡ��������̱�Ŷ�Ӧ��ϵ
//vu8 FLIST_DISK_ORDER_TBL[3];
//filelist����·����
u8*const FLIST_DISK_PATH_TBL[2]={"0:","1:"/*,"2:"*/};
//�������ֱ�
u8*const FLIST_DISK_NAME_TBL[2]={"SD Card","FLASH Disk"/*,"U Disk",*/};		    
	const u8 chgchar[2]={0X5C,0X00};//ת��� ��Ч"\"

/*******************************************************************************
* ������			: GUI_FileListCreateAsChild
* ����	    : ����FileList�ؼ�
* �������		: Form_P�������ڵ�ָ�룻 X��X��λ�ã�	Y��Y��λ�� W:��ȣ�H:�߶ȣ�
* ���ز���		: filelistָ��
* ˵��			:
*******************************************************************************/
GUI_FileListStruct * GUI_FileListCreateAsChild(GUI_FormStruct *Form_P, u16 X,u16 Y,u16 W,u16 H)
{
 	GUI_FileListStruct * FileList_P;
	GUI_ScrollbarStruct * Scrollbar_P;	
//	if(height<=gui_phy.listheight+gui_phy.listheight+gui_phy.listheight)return NULL;//�߶�С��	    
//	if((height-gui_phy.listheight-gui_phy.listheight)%gui_phy.listheight)return NULL;//�ߴ粻�ϸ� 	 				    
//	if(width<gui_phy.listheight)return NULL;			//�ߴ粻�ϸ�
	FileList_P = (GUI_FileListStruct *)GUI_MEM_Malloc(sizeof(GUI_FileListStruct));	//�����ڴ�
	if(FileList_P==NULL)return NULL;			//�ڴ���䲻��.
	GUI_MemSet((u8*)FileList_P,0,sizeof(GUI_FileListStruct));//��FileList_P��ֵȫ������Ϊ0    
	Scrollbar_P=GUI_ScrollbarCreate(X+W-SCOROLLBAR_WIDTH-1,Y+FILEINFO_WIDTH+1,H-FILEINFO_WIDTH*2-2,0);//����scrollbar.
	if(Scrollbar_P==NULL)
	{
		GUI_MEM_Free(FileList_P);//�ͷ�֮ǰ������ڴ�
		return NULL;					//�ڴ���䲻��.
 	}
	FileList_P->Font = &Font8x12;;		//�����С 
 	Scrollbar_P->itemsperpage=(H-FILEINFO_WIDTH*2-2)/ (FileList_P->Font->Y+4);	//ÿҳ��ʾ����Ŀ��	
	FileList_P->Scrollbar=Scrollbar_P;			    			//������ 	
	FileList_P->X 				= X;
	FileList_P->Y					=	Y;
	FileList_P->W					= W;
	FileList_P->H					= H;
	FileList_P->ID = GUIFileListID;
	FileList_P->selindex=0XFF;	//��ǰѡ�е�����													
	FileList_P->L_B_Color=D_FILELIST_LBCOLOR;			//�ڲ�����ɫ
	FileList_P->L_S_Color=D_FILELIST_LSCOLOR;		//ѡ��list���������ɫ 
	FileList_P->L_SB_Color=D_FILELIST_LSBCOLOR;	//ѡ��list��ı���ɫ
	FileList_P->F_Color=D_FILELIST_FCOLOR;				//δѡ�е�list������ɫ
	FileList_P->Rim_Color=D_FILELIST_RIMCOLOR;			//�߿���ɫ
	FileList_P->I_B_Color=D_FILELIST_IBCOLOR;	//·��/��Ϣ�� ������ɫ
	FileList_P->OsPriority = GUI_OsCurrentPriority;		//POST�����ȼ�Ϊ����filelist������
	
	FileList_P->path=NULL;			    				//·��Ϊ��	 

//	FileList_P->fliter=0X0000;		    				//�����ļ�����(�������ļ���)
//	FileList_P->fliterex=0XFFFF;	    				//����С����й���(����С�������Ҫ��)

	FileList_P->fname 				= NULL;			    	    	//����Ϊ��
	FileList_P->namelen				= 0;			    	    		//��������
//	FileList_P->curnamepos		= 0;			    	    	//��ǰλ������
//	FileList_P->oldtime				= 0;			    	    		//ʱ������
	 

	FileList_P->findextbl=NULL;			    		//�ļ�������Ϊ��

 	FileList_P->list=NULL;								//������
	FileList_P->memdevflag = 0xff;
	if(GUI_FormAddChild(Form_P,(u8*)FileList_P))
	{
		GUI_MEM_Free(Scrollbar_P);	//�ͷ��ڴ�
		GUI_MEM_Free(FileList_P);	//�ͷ��ڴ�			
		return 0;
	}
	else
	{
		return FileList_P;
	}
	

}

u8 GUI_FileListDelete(GUI_FileListStruct *FileList_P)
{
	if(FileList_P->ID == GUIFileListID)
	{
		GUI_MEM_Free(FileList_P->Scrollbar);	//�ͷ��ڴ�
		if(FileList_P->list!=NULL)filelist_delete(FileList_P);	//ɾ��֮ǰ����Ŀ
		GUI_MEM_Free(FileList_P->findextbl);
		GUI_MEM_Free(FileList_P->path);	//ɾ��·��
		GUI_MEM_Free(FileList_P);	//�ͷ��ڴ�	
		
		return 0;
	}
	return 1;
}


//ɾ��filelistbox
//filelistbox_del:Ҫɾ����filelistbox
//void filelistbox_delete(GUI_FileListStruct *filelistbox_del)
//{
// 	filelist_delete(filelistbox_del);	 	//ɾ����Ŀ
//	GUI_MEM_Free(filelistbox_del->findextbl);//ɾ��Ŀ���ļ�������
//	GUI_MEM_Free(filelistbox_del->path);	//ɾ��·��
//	GUI_MEM_Free(filelistbox_del->Scrollbar);	//ɾ��������
//	GUI_MEM_Free(filelistbox_del);	  	//ɾ��filelistbox
//}



void GUI_FileListShowNow(GUI_FileListStruct *FileList_P)						   //0XFF  0x10  0x11   0x20   0x22
{
	u16 X1,Y1,X2,Y2;
	X1 = FileList_P->X;
	Y1 = FileList_P->Y;	
	X2 = FileList_P->X + FileList_P->W-1;
	Y2 = FileList_P->Y + FileList_P->H-1;	
	if(FileList_P->ID != GUIFileListID) 	return ;				//����FileList������	
		if(!GUIGetBit(FileList_P->State,GUI_SHOWOK))			//�ؼ��Ƿ���ʾ���
		{		
			if(!GUIGetBit(FileList_P->State,FILELIST_INIT))			//�ؼ��Ƿ���Ҫ��ʼ����ʾ				
			{	
				GUI_FileListbox_Scan(FileList_P);				
				GUI_DrawRectangle(X1,Y1,X2,Y2,D_FILELIST_RIMCOLOR);					
				GUI_DrawFillRectangle(X1+1,Y1+1,X2-1,Y1+FILEINFO_WIDTH,FileList_P->I_B_Color);		//������			
				GUI_DrawFillRectangle(X1+1,Y2-FILEINFO_WIDTH,X2-1,Y2-1,FileList_P->I_B_Color);		//״̬��							
				GUISetBit(FileList_P->State,FILELIST_INIT);			//�ؼ���ʼ�����
			}				
			if(GUIGetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR))
			{
			GUIResetBit(FileList_P->Scrollbar->State,GUI_SHOWOK);			//�ؼ���ʾ���
				GUI_ScrollbarShowNow(FileList_P->Scrollbar);
			}				
			GUI_FileList_Show_Path(FileList_P);
			GUI_FileListDrawList(FileList_P);
			GUISetBit(FileList_P->State,GUI_SHOWOK);			//�ؼ���ʾ���
		}

}


//��ʾfilelistbox��·�� 
//filelistbox:filelistboxָ��

void GUI_FileList_Show_Path(GUI_FileListStruct *FileList_P)
{
	u16 plen=0;
	u8 *path=FileList_P->path;

	if(path)//·����Ч
	{
		plen=strlen((const char*)path);
	} 
	if(plen)//��Ҫ��ʾ
	{
		GUI_DrawFillRectangle(FileList_P->X+1,FileList_P->Y+1,FileList_P->X + FileList_P->W-2,FileList_P->Y+FILEINFO_WIDTH,FileList_P->I_B_Color);		//������		
		if(plen*FileList_P->Font->ASC_X>(FileList_P->W-20))
		{
			
			while(plen*FileList_P->Font->ASC_X>(FileList_P->W-20-FileList_P->Font->ASC_X*3))
			{
 				path=(u8*)strstr((const char*)++path,(const char*)chgchar);	//ȥ��һ��
				plen=strlen((const char*)path);					//�õ��µĳ���
			}
			GUI_ShowStringAt((u8*)"...",FileList_P->X+4,FileList_P->Y+3, FileList_P->I_B_Color,FileList_P->F_Color,FileList_P->Font);
			GUI_ShowStringAt(path,FileList_P->X+FileList_P->Font->ASC_X*3,FileList_P->Y+3,FileList_P->I_B_Color,FileList_P->F_Color,FileList_P->Font);
		}  
		else
		{
			GUI_ShowStringAt(path,FileList_P->X+4,FileList_P->Y+3,FileList_P->I_B_Color,FileList_P->F_Color,FileList_P->Font);	
		}

	}
}	 

//���ұ��Ϊindex��flist
//flistx:��ǰ��flistָ��
//index:ҪѰ�ҵ�flist��� 0~totalitems-1;
//����ֵ:list�ṹ��
//����ҵ�,�򷵻�list�ṹ��,���û�ҵ�,����NULL.

GUI_FileQueueStruct * filelist_search(GUI_FileQueueStruct *FileQueue_P,u16 index)
{
	u16 icnt=0;
 	GUI_FileQueueStruct * FileQueue_TP=FileQueue_P;	
	if(FileQueue_P==NULL)return NULL;
	while(FileQueue_TP->prevQueue!=NULL)FileQueue_TP=FileQueue_TP->prevQueue;//׷�ݵ��ʼ��list
	while(1)//Ѱ�ұ��Ϊindex��list
	{
		if(index==icnt)break;	 
		if(FileQueue_TP->nextQueue!=NULL)
		{
			FileQueue_TP=FileQueue_TP->nextQueue;    
			icnt++;//��������
		}else return NULL;//δ�ҵ����Ϊindex��list
	} 
	return FileQueue_TP;//�����ҵ���listָ��
}

//����һ��filelistbox����Ŀ
//filelistbox:
//0,���ӳɹ�;
//1,����ʧ��
u8 GUI_FileList_addlist(GUI_FileListStruct * FileList_P,u8 *name,u8 type)
{
	GUI_FileQueueStruct * FileQueue_P;
	GUI_FileQueueStruct * filelisttemp;
	FileQueue_P=(GUI_FileQueueStruct*)GUI_MEM_Malloc(sizeof(GUI_FileQueueStruct));//�����ڴ�
	if(FileQueue_P==NULL)return 1;//�ڴ���䲻��.
	GUI_MemSet(FileQueue_P,0,sizeof(GUI_FileQueueStruct));//ȫ������Ϊ0
 	strcpy((char *)FileQueue_P->name,(const char*)name);	//����name�����ݵ�		
	FileQueue_P->type=type;								//����
 	FileQueue_P->nextQueue=NULL;
 	if(FileList_P->list==NULL)//��δ��������
	{
		FileQueue_P->prevQueue=NULL;
		FileList_P->list=FileQueue_P;
	}
	else //�Ѿ�������
	{
		filelisttemp=FileList_P->list;
		while(filelisttemp->nextQueue!=NULL)filelisttemp=filelisttemp->nextQueue;//ƫ�Ƶ����һ��list	 
		FileQueue_P->prevQueue=filelisttemp;
		filelisttemp->nextQueue=FileQueue_P;
	}
	FileList_P->Scrollbar->totalitems++;//����Ŀ������1��
	if(FileList_P->Scrollbar->totalitems > FileList_P->Scrollbar->itemsperpage)
	{
		GUISetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR);//��Ҫ��ʾ����		
		FileList_P->Scrollbar->State = 0;
	}
 	else 
	{
		GUIResetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR);//����Ҫ��ʾ����
	}
	return 0;
}

/*******************************************************************************
* ������			: GUI_FileListDrawList
* ����	    : ��ʾFileList�ؼ���list
* �������		: FileList_P�����ڵ�ָ�룻
* ���ز���		: ��
* ˵��			:
*******************************************************************************/
void GUI_FileListDrawList(GUI_FileListStruct *FileList_P)
{
	u16 i;
	u16 X1,Y1,X2,Y2;
  GUI_FileQueueStruct * FileQueue_P;		
	X1 = FileList_P->X+1;
	Y1 = FileList_P->Y+FILEINFO_WIDTH+1;	
	X2 = FileList_P->X + FileList_P->W-2;//��Ŀ��ʾ����x����
	Y2 = FileList_P->Y + FileList_P->H-2-FILEINFO_WIDTH;	
	
	if(GUIGetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR))
	{
		X2-=SCOROLLBAR_WIDTH;//��Ҫ��������.
	}else FileList_P->Scrollbar->topitem=0;
	
	GUI_DrawFillRectangle(X1,Y1,X2,Y2,FileList_P->L_B_Color);		//LIST��	
	
 	FileQueue_P=filelist_search(FileList_P->list,FileList_P->Scrollbar->topitem);//�õ�����list��Ϣ(curitem��listbox�����ڼ�¼����index)
	if(FileQueue_P==NULL)return ;//û���κ�list
	for(i=0;i<FileList_P->Scrollbar->itemsperpage;i++)//��ʾ��Ŀ
	{	
		if((FileList_P->Scrollbar->topitem+i)==FileList_P->selindex)//����Ϊѡ����Ŀ
		{
			GUI_DrawFillRectangle(X1,Y1+i*FILELIST_LIST_H,X2,Y1+(i+1)*FILELIST_LIST_H-1,FileList_P->L_SB_Color);		//LIST��
			FileList_P->fname=FileQueue_P->name;//��õ�ǰѡ�е���Ŀ������
			GUI_ShowStringAt(FileQueue_P->name,X1+20,Y1+2+i*FILELIST_LIST_H,FileList_P->L_SB_Color,FileList_P->L_S_Color,FileList_P->Font);	
//			FileList_P->namelen=strlen((const char*)FileList_P->fname)*FileList_P->font/2;//���ֵ��ܳ���
		}
		else
		{
			GUI_ShowStringAt(FileQueue_P->name,X1+20,Y1+2+i*FILELIST_LIST_H,FileList_P->L_B_Color,FileList_P->F_Color,FileList_P->Font);	
		}
//		gui_draw_icos(FileList_P->left,FileList_P->top+gui_phy.listheight+i*gui_phy.listheight+(gui_phy.listheight-FileList_P->icosize)/2,FileList_P->icosize,filelistx->type);//��ICOͼ��																				    
		
		if(FileQueue_P->nextQueue==0)break;
		FileQueue_P=FileQueue_P->nextQueue;
 	}
}


//filelist��Ӵ���(��filelist����ǰ,����ִ��һ�θú���)
//filelistbox:filelistboxָ��
void GUI_FileListAddDisk(GUI_FileListStruct *FileList_P)
{
	FileList_P->selindex=0XFF;
	FileList_P->foldercnt=0;
	FileList_P->filecnt=0;
//	FileList_P->memdevflag=GUI.MemDevFlag;
	
	if(FileList_P->memdevflag&(1<<0)) 	//SD����λ
	{
			GUI_FileList_addlist(FileList_P,(u8*)FLIST_DISK_NAME_TBL[0],0);//���һ������
	}
	if(FileList_P->memdevflag&(1<<1)) 	//FLASH����λ
	{
			GUI_FileList_addlist(FileList_P,(u8*)FLIST_DISK_NAME_TBL[1],0);//���һ������
	}	 
}


//ɾ���������� 
//filelistdel,Ҫɾ���������е�ĳ����Ա
void filelist_delete(GUI_FileListStruct * filelistbox)
{
 	GUI_FileQueueStruct * filelisttemp;
  	GUI_FileQueueStruct * filelistdel;
	if(filelistbox==NULL)return;//�Ƿ��ĵ�ַ,ֱ���˳�
 	if(filelistbox->list!=NULL)
	{
		filelisttemp=filelist_search(filelistbox->list,0);//�õ���һ��filelist�Ľṹ��
		do
		{
			filelistdel=filelisttemp;
			filelisttemp=filelisttemp->nextQueue;
		    GUI_MEM_Free(filelistdel->name);	//ɾ����Ŀ������
		    GUI_MEM_Free(filelistdel);		//ɾ��һ����Ŀ
		}while(filelisttemp!=NULL);		//һ��ɾ�����е���Ŀ
	}
	filelistbox->list=NULL;//���
 	filelistbox->Scrollbar->totalitems=0;//����Ŀ����
	filelistbox->Scrollbar->topitem=0;	//������Ŀ����
}

//ɨ�赱ǰ·���µ������ļ�
//filelistbox:filelistboxָ��
//����ֵ:0,����;
//    ����,�������̳��ִ���
u8 GUI_FileListbox_Scan(GUI_FileListStruct *FileList_P)
{
 	DIR flboxdir;		//flboxר��	  
	FILINFO flboxfinfo;	 
	u8 res=0; 
	u16 type=0XFFFF;
//	u16 typeex=0;
	u16 findex;	  
  u8 *fn;   //This function is assuming non-Unicode cfg. 
	u8 mark=0;//0,�����ļ���;1,����Ŀ���ļ�;2

#if _USE_LFN						
 	flboxfinfo.lfsize=_MAX_LFN * 2 + 1;
	flboxfinfo.lfname=GUI_MEM_Malloc(flboxfinfo.lfsize);
	if(flboxfinfo.lfname==NULL)return 0XFF;//�����ڴ�ʧ�� 
   GUI_MemSet((char *)flboxfinfo.lfname,0,flboxfinfo.lfsize);
#endif	 
	 
 	if(FileList_P->path)	//·����Ч
	{
		res=f_opendir(&flboxdir,(const TCHAR*)FileList_P->path); //��һ��Ŀ¼
		if(res==FR_OK)		//��Ŀ¼�ɹ���.
		{
			if(FileList_P->list!=NULL)filelist_delete(FileList_P);	//ɾ��֮ǰ����Ŀ
			GUI_FileList_addlist(FileList_P,(u8*)"..",1);	//��ӷ�����һ��α�ļ��� 
 			FileList_P->foldercnt=0;
			FileList_P->filecnt=0;
			GUI_MEM_Free(FileList_P->findextbl);//�ͷ�֮ǰ������������ڴ�
			FileList_P->findextbl=(u16*)GUI_MEM_Malloc(FLBOX_MAX_DFFILE*2);//��Ĭ������FLBOX_MAX_DFFILE��Ŀ���ļ������ڴ�
 		} 
		while(res==FR_OK)
		{
			findex=(u16)flboxdir.index;//��¼�´��ļ���indexֵ
			res=f_readdir(&flboxdir,&flboxfinfo);//��ȡ�ļ���Ϣ
			if(res!=FR_OK||flboxfinfo.fname[0]==0)
			{
				if(mark==0)	//�ļ��в�������
				{
					mark=1;	//��ǲ���Ŀ���ļ�
//					f_closedir(&flboxdir);
					res=f_opendir(&flboxdir,(const TCHAR*)FileList_P->path); //�ٴδ򿪸�Ŀ¼
 					continue;
				}
				break;		//������/��ĩβ��,�˳�   
			}
			if (flboxfinfo.fname[0]=='.'&&flboxfinfo.fname[1]==0)continue;//���Ա���Ŀ¼
			fn=(u8*)(*flboxfinfo.lfname?flboxfinfo.lfname:flboxfinfo.fname);
			if(flboxfinfo.fattrib&(1<<4)&&mark==0)//��һ���ļ�����mark=0
			{
				type=FICO_FOLDER;	
				FileList_P->foldercnt++;
			}
			else if(flboxfinfo.fattrib&(1<<5)&&mark==1)//��һ���鵵�ļ���mark=1
			{
				type=FICO_FILE;	//�����ļ�

					if(FileList_P->filecnt<FLBOX_MAX_DFFILE)FileList_P->findextbl[FileList_P->filecnt]=findex;//��¼���ļ�����
					FileList_P->filecnt++;//Ŀ���ļ�����
			}else continue;		//��������һ�� 
			if(GUI_FileList_addlist(FileList_P,fn,type))break;//���ʧ����         
		}
		if(mark==1&&FileList_P->filecnt)
		{
			fn=GUI_MEM_Realloc(FileList_P->findextbl,FileList_P->filecnt*2);//�ͷŶ�����ڴ�
			if(fn!=NULL)FileList_P->findextbl=(u16*)fn;//�������ɹ���,��ʹ���µĵ�ַ,����Դ��ַ����
		}   
	}else//û���κ���Ч·��,˵���ص�ѡ����̵�״̬��
	{
		filelist_delete(FileList_P);		//ɾ��֮ǰ����Ŀ
		GUI_FileListAddDisk(FileList_P);	//������Ӵ���·��
 	}
	GUI_MEM_Free(flboxfinfo.lfname);
	return res;
}











//�ؽ��ļ��б��
//����Ч�ļ�ʹ�����˳���ʹ��,����ɨ���ļ������ػ�
//filelistbox:listboxָ��
u8 filelistbox_rebuild_filelist(GUI_FileListStruct * filelistbox)
{
	u8 res=0;
	if(filelistbox!=NULL)
	{
		res=GUI_FileListbox_Scan(filelistbox);	//����ɨ���б�	  
		if(filelistbox->Scrollbar->totalitems>filelistbox->Scrollbar->itemsperpage)filelistbox->Scrollbar->topitem=filelistbox->selindex;
		GUI_FileListDrawList(filelistbox);		//�ػ� 
 	}else res=2;
	return res;
}	 


/********************************************
������GUI_FileListTouchUp
���ܣ������������ͷŵ�ʱ������FileList��״̬
������ FileList_P: FileList��ַָ�룻
���أ���
˵����
********************************************/
void GUI_FileListTouchUp(GUI_FileListStruct *FileList_P)
{
	if(!GUIGetBit(FileList_P->State,GUI_DIS))							//����DIS״̬
	{
		GUIResetBit(FileList_P->State,GUI_ON);				//����ΪOFF״̬
		GUIResetBit(FileList_P->State,GUI_SHOWOK);			//�ؼ����ػ�
		GUIResetBit(FileList_P->State,FILELIST_TOUCH);			//����������
		GUIResetBit(FileList_P->State,FILELIST_CLOSE);			//�ؼ��ر�
//		GUI.TouchChange = TOUCHNULL;
//		GUI.CurrentForm->TouchNum = NOTOUCHNUM;			
		GUI.CurrentForm->TouchNum = 0;	
	}
}


u8 GUI_FileListTouchLong(GUI_FileListStruct *FileList_P)
{
//	u32 FileListMessage;
	GUI_FileQueueStruct *FileQueue_P;	
	if(FileList_P == GUI_null)		return GUI_false;
	if(!GUIGetBit(FileList_P->State,GUI_DIS))		//����DISABLE
	{
		if(GUIGetBit(FileList_P->State,FILELIST_TOUCH))		//����������
		{
			FileQueue_P=filelist_search(FileList_P->list,FileList_P->selindex);//�õ�ѡ�е�list����ϸ��Ϣ
			if(FileQueue_P->type == FICO_FILE)
			{
				GUI_FL_OpenFile(FileList_P->OsPriority,(u32*)FileList_P);
				
//				#if GUI_USEMSOS
//				FileListMessage = (((u32)GUI_FILELISTMESSAGE<<24) | (((u32)FileList_P) & 0xFFFFFF));			
//					OS.PostMessageQueue(FileList_P->OsPriority, (void *)(FileListMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ
//				#else
//					//��OS��ʱ�����еĳ���
//				#endif	
				if(FileList_P->ID == GUIFileListID)	
				{
					GUIResetBit(FileList_P->State,FILELIST_TOUCH);
				}
			}
			else
			{
				filelistbox_check_filelist(FileList_P);
				GUIResetBit(FileList_P->State,FILELIST_TOUCH);
				GUIResetBit(FileList_P->State,GUI_SHOWOK);				
			}
			return GUI_true;	
		}
		else if(GUIGetBit(FileList_P->State,FILELIST_CLOSE))		//�رհ�ť������
		{
				GUI_FL_Close(FileList_P->OsPriority,(u32*)FileList_P);
//			#if GUI_USEMSOS
//				FileListMessage = (((u32)FileListCloseMessageType<<24) | (((u32)FileList_P) & 0xFFFFFF));		
//				OS.PostMessageQueue(FileList_P->OsPriority, (void *)(FileListMessage));		//���ͳ�ʱ�䰴�¹��ܼ��ļ�ֵ		
//			#else
//				//��OS��ʱ�����еĳ���
//			#endif				
			GUIResetBit(FileList_P->State,FILELIST_CLOSE);			
			
		}
	}
	return GUI_false;	
	
}



/********************************************
������GUI_FileListTouchDown
���ܣ��������������µ�ʱ������FileList��״̬
������ FileList_P: FileList��ַָ�룻Num���ڵ�ǰ��FORM�еı��
���أ���
˵����
********************************************/
u8 GUI_FileListTouchDown(GUI_FileListStruct *FileList_P,u8 Num)
{
	u16 i,Max;
	u16 X1,Y1,X2,Y2;	
	X1 = FileList_P->X;
	Y1 = FileList_P->Y+FILEINFO_WIDTH+1;	
	X2 = FileList_P->X + FileList_P->W-1;//��Ŀ��ʾ����x����
	Y2 = FileList_P->Y + FileList_P->H-1-FILEINFO_WIDTH-1;	
	if(FileList_P == GUI_null)		return GUI_false;
	FileList_P->selindex = 0XFF;
	GUIResetBit(FileList_P->State,FILELIST_TOUCH);
	if(!GUIGetBit(FileList_P->State,GUI_DIS))			//�ؼ��Ƿ���ʹ��״̬
		{										//����DIS״̬				
			if((GUI.Touch_Y>Y1)&&(GUI.Touch_Y<Y2))		 //�ж��Ƿ���Y�����ڲ�
			{
				if((GUI.Touch_X > X2-SCOROLLBAR_WIDTH)&&(GUI.Touch_X<X2))		 //�ж��Ƿ��ڹ������ڲ�		
				{				
					if(GUIGetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR))		//�Ƿ��й�����
					{
						//SCOROLLBAR
						if(GUI_ScrollbarTouchDown(FileList_P->Scrollbar,GUI.Touch_X,GUI.Touch_Y))
						{
								GUIResetBit(FileList_P->State,GUI_SHOWOK);								
						}
						X2 =  X2-SCOROLLBAR_WIDTH;
					}

				}			
				
				if((GUI.Touch_X > X1)&&(GUI.Touch_X<X2))		 //�ж��Ƿ����ڲ�
				{
					if(FileList_P->Scrollbar->totalitems > FileList_P->Scrollbar->itemsperpage) 
					{
						Max = FileList_P->Scrollbar->itemsperpage;
					}
					else
					{
						Max = FileList_P->Scrollbar->totalitems;
					}
					for (i=0;i<Max;i++)
					{
						if((GUI.Touch_Y>Y1+i*FILELIST_LIST_H)&&(GUI.Touch_Y<(Y1+(i+1)*FILELIST_LIST_H)))		 
						{

							FileList_P->selindex = i+FileList_P->Scrollbar->topitem;															//���汻���µļ����			
							GUI.CurrentForm->TouchNum = Num;
//							GUI.TouchChange = TOUCHNULL;															//���GUI_API��touch��־
							GUISetBit(FileList_P->State,FILELIST_TOUCH);
							GUIResetBit(FileList_P->State,GUI_SHOWOK);
							return GUI_true;						

						}	
					}					
					return GUI_true;					
				}
			}
			else if((GUI.Touch_Y >FileList_P->Y)&&(GUI.Touch_Y<Y1)&&(GUI.Touch_X > X2-SCOROLLBAR_WIDTH)&&(GUI.Touch_X<X2))	//�ڹرտؼ���������
					{
							GUI.CurrentForm->TouchNum = Num;
//							GUI.TouchChange = TOUCHNULL;															//���GUI_API��touch��־
							GUISetBit(FileList_P->State,FILELIST_CLOSE);
							GUIResetBit(FileList_P->State,GUI_SHOWOK);						
					return GUI_true;	
					}
		return GUI_false;		
	}
		return GUI_false;			
}























////���������ַ���
//u8*const FLBOX_DISK_SIZE_CAPTION_TBL[3]=
//{
//	"��������:","�űP����:","Capacity:",	
//};
////���̿��ÿռ��ַ���
//u8*const FLBOX_DISK_FREE_CAPTION_TBL[3]=
//{
//	"���ÿռ�:","���ÿ��g:","Free:",	
//};
////�ļ����ַ���
//u8*const FLBOX_FOLDER_CAPTION_TBL[3]=
//{
//	"�ļ���:","�ļ��A:","Folder:",	
//};
////���̿��ÿռ��ַ���
//u8*const FLBOX_FILES_CAPTION_TBL[3]=
//{
//	"  �ļ�:","  �ļ�:","  Files:",	
//}; 



//��ʾ��Ϣ  
//filelistbox:filelistboxָ��
//mode:0,�Ǵ��ļ���/���̵Ĳ���.1,���ļ��л����,��Ҫ����ȫ����Ϣ
void filelistbox_show_info(GUI_FileListStruct *filelistbox)
{
//	GUI_FileListStruct *filelistx;
//	FATFS *fs1;
//	u8 res;
//    u32 freclust=0,fresec=0,totsec=0;
// 	u8 * temp;
//	u8 * temp1;
//	u8 * mark;
//	temp=gui_memin_malloc(60);	//���ڴ���ַ���
//	if(temp==NULL)return;
// 	temp1=gui_memin_malloc(30);	//���ڴ�����ִ�
//	if(temp1==NULL)
//	{
//		gui_memin_free(temp);	
//		return;  
//	}
// 	gui_memset((char *)temp,0,60);
//  	gui_memset((char *)temp1,0,30);
//	gui_fill_rectangle(filelistbox->left,filelistbox->top+filelistbox->height-gui_phy.listheight,filelistbox->width,gui_phy.listheight,filelistbox->pthinfbkcolor);//��ʾ�ļ���Ϣ����ɫ
//	gui_draw_hline(filelistbox->left,filelistbox->top+filelistbox->height-gui_phy.listheight,filelistbox->width,0X000);//��ʾ��ɫ�ĺ���		
//	filelistx=filelist_search(filelistbox->list,filelistbox->selindex);//�õ�ѡ�е�list����ϸ��Ϣ
//	if(filelistx->type==FICO_DISK)//��ǰѡ�е��Ǵ���
//	{
//		mark=(u8*)FLIST_DISK_PATH_TBL[FLIST_DISK_ORDER_TBL[filelistbox->selindex]]; 
//		res=f_getfree((const TCHAR*)mark,(DWORD*)&freclust,&fs1);//���ʣ������
//	    if(res==0)
//		{											   
//		    totsec=(fs1->n_fatent-2)*fs1->csize;//�õ���������
//		    fresec=freclust*fs1->csize;			//�õ�����������	   
//#if _MAX_SS!=512								//��������������512�ֽڴ�С�Ĵ���
//			totsec*=fs1->ssize/512;
//			fresec*=fs1->ssize/512;
//#endif	  
//		}
//		if(totsec<20480)//������С��10M
//		{
//			totsec>>=1,fresec>>=1;
//			mark="KB ";
//		}else
//		{
//			totsec>>=11,fresec>>=11;
//			mark="MB ";
// 		}
//		strcpy((char*)temp,(const char*)FLBOX_DISK_SIZE_CAPTION_TBL[gui_phy.language]);//��������	
//		gui_num2str(temp1,totsec);				//������תΪ�ַ���
//		strcat((char*)temp,(const char*)temp1);	
//		strcat((char*)temp,(const char*)mark);	//��ӵ�λ
//  		strcat((char*)temp,(const char*)FLBOX_DISK_FREE_CAPTION_TBL[gui_phy.language]);//���ÿռ�		
// 		gui_num2str(temp1,fresec);				//���ÿռ�תΪ�ַ���
//		strcat((char*)temp,(const char*)temp1);	
//		strcat((char*)temp,(const char*)mark);	//��ӵ�λ
//	}else	//ѡ�е����ļ�/�ļ���
//	{
// 		if(filelistbox->selindex<filelistbox->foldercnt)fresec=filelistbox->selindex+1;	//���ļ�������Ŀ����.
//		else freclust=filelistbox->selindex-filelistbox->foldercnt+1;		//���ļ�����Ŀ����
//		strcpy((char*)temp,(const char*)FLBOX_FOLDER_CAPTION_TBL[gui_phy.language]);//�ļ���	
//		gui_num2str(temp1,fresec);					//��ǰ�ļ��б��תΪ�ַ���
//		strcat((char*)temp,(const char*)temp1);	
// 		strcat((char*)temp,(const char*)"/");		
//		gui_num2str(temp1,filelistbox->foldercnt);	//�ļ�������תΪ�ַ���
// 		strcat((char*)temp,(const char*)temp1);	
// 		strcat((char*)temp,(const char*)FLBOX_FILES_CAPTION_TBL[gui_phy.language]);//�ļ�		
//		gui_num2str(temp1,freclust);				//��ǰ�ļ����תΪ�ַ���
//		strcat((char*)temp,(const char*)temp1);	
// 		strcat((char*)temp,(const char*)"/");		
//		gui_num2str(temp1,filelistbox->filecnt);	//�ļ�����תΪ�ַ���
// 		strcat((char*)temp,(const char*)temp1);	
//	}
//	gui_show_ptstr(filelistbox->left,filelistbox->top+filelistbox->height-gui_phy.listheight+(gui_phy.listheight-gui_phy.listfsize)/2,filelistbox->width+filelistbox->left,gui_phy.lcdheight,0,filelistbox->lncolor,gui_phy.listfsize,temp,1);//��ʾ·��
//	gui_memin_free(temp);
// 	gui_memin_free(temp1);
}


//���·����Ŀ¼���
//path:·��
u8 filelistbox_get_pathdepth(u8* path)
{
	u8 depth=0;
	while(*path!=0)
	{
		if(*path==0X5C)//�ҵ�'\'��
		{
			depth++;
		}
		path++;
	}
	return depth;
	
	
}

//��name���뵽path����.�Զ����\�ͽ�����.����Ҫȷ��pname���ڴ�ռ��㹻,������ܵ�������.
//pname:��·�����ļ���.path���ڴ��С����Ϊ:strlen(path)+strlen(name)+2!
//path:·��
//name:����
//����ֵ:��·�����ļ���
u8* gui_path_name(u8 *pname,u8* path,u8 *name)
{
//	const u8 chgchar[2]={0X5C,0X00};//ת��� ��Ч"\"    
	strcpy((char *)pname,(const char*)path);	//����path��pname����
	strcat((char*)pname,(const char*)chgchar);	//���ת���
	strcat((char*)pname,(const char*)name);		//�������������
	return pname;
}
//ɨ��·���µ��ļ�
//filelistbox:filelistָ��
void filelistbox_check_filelist(GUI_FileListStruct *filelistbox)
{
	u8 *str;
	u8 *oldpath;
	u16 temp;
	u8 res=0;
	GUI_FileQueueStruct *filelistx;
	u16 ttlen;//�ܳ���
	filelistx=filelist_search(filelistbox->list,filelistbox->selindex);//�õ�ѡ�е�list����ϸ��Ϣ
 	if(filelistx->type==0||filelistx->type==1)//���ڴ���/�ļ���,���	
	{   
		temp=filelistbox_get_pathdepth(filelistbox->path);//�õ���ǰĿ¼�����
		if(filelistx->type==0)//�Ǵ���
		{	
			str=(u8*)FLIST_DISK_PATH_TBL[filelistbox->selindex];  
			GUI_MEM_Free(filelistbox->path);//�ͷ�֮ǰ������ڴ�
			filelistbox->path=(u8*)GUI_MEM_Malloc(strlen((const char*)str)+1);//���������ڴ�
			if(filelistbox->path==NULL)return;//�����ڴ�ʧ��.
 			strcpy((char *)filelistbox->path,(const char*)str);//����str��path����
		}else //���ļ���
		{
			if(strcmp((const char*)filelistx->name,".."))//����..�ļ���
			{
				if(temp<FLBOX_PATH_DEPTH)filelistbox->seltbl[temp]=filelistbox->selindex;//��¼�µ�ǰĿ¼��ѡ�е�
			  	ttlen=strlen((const char*)filelistbox->path);//�õ�ԭ����·���ĳ���(������������)
				ttlen+=strlen((const char*)filelistx->name);	//�õ����ֵĳ���
				oldpath=(u8*)GUI_MEM_Malloc(strlen((const char*)filelistbox->path)+1);//�����ڴ�
				if(oldpath==NULL)return;//�����ڴ�ʧ��
 				oldpath=(u8*)strcpy((char *)oldpath,(const char*)filelistbox->path);//����filelistbox->path��oldpath����
	 			GUI_MEM_Free(filelistbox->path);//�ͷ�filelistbox->path֮ǰ������ڴ�
				filelistbox->path=(u8*)GUI_MEM_Malloc(ttlen+2);//���������ڴ�
				if(filelistbox->path==NULL)
				{
					GUI_MEM_Free(oldpath);
					return;//�����ڴ�ʧ��.
				}
 				filelistbox->path=gui_path_name(filelistbox->path,oldpath,filelistx->name);//·�����ļ������
				GUI_MEM_Free(oldpath);//�ͷ�������ڴ�.
				temp=0;//������Ŀ¼,ѡ��0λ��.
			}else //..,������һ��Ŀ¼
			{
				filelistbox_back(filelistbox);//�˻���һ��Ŀ¼
				return ;  
 			}
		} 
		res=GUI_FileListbox_Scan(filelistbox);	//����ɨ���б�
		filelistbox->selindex=temp;				//���һ��ѡ�е���Ŀ
		if(filelistbox->Scrollbar->totalitems>filelistbox->Scrollbar->itemsperpage)filelistbox->Scrollbar->topitem=temp;
		if(res==0&&filelistx->type==0&&filelistbox->foldercnt==0&&filelistbox->filecnt==0)	//���̸�Ŀ¼ɨ������,�Ҹ�Ŀ¼��û���κ��ļ�/�ļ���
		{	    
 			GUI_FileList_addlist(filelistbox,"..",1);	//��ӷ�����һ��α�ļ��� 
		}
 	}else 
	{
//		filelistbox->dbclick|=1<<0;//�����Ŀ���ļ�
	}	 
}

//����һ������.(���ز���)
//�˺������Ե�������,�Զ�������һ��.�Դ���ɨ�輰�ػ�����.
//filelistbox:filelistboxָ��
void filelistbox_back(GUI_FileListStruct *filelistbox)
{
	u8 *oldpath;
	u16 temp=0; 
	oldpath=filelistbox->path;
	while(*oldpath!=0)//һֱƫ�Ƶ����.
	{
		oldpath++;
		temp++;
	}
	if(temp<4)
	{
		if(filelistbox->path)GUI_MEM_Free(filelistbox->path);//�ͷ�֮ǰ���뵽���ڴ�
		filelistbox->path=NULL;//�˵����
		temp=0;
	}else	   
	{	  
		while(*oldpath!=0x5c)oldpath--;	//׷����������һ��"\"�� 
		*oldpath='\0';//�ý���������"\".�Ӷ��ص���һ��Ŀ¼
		temp=filelistbox_get_pathdepth(filelistbox->path);//�õ���Ŀ¼�����
		if(temp<FLBOX_PATH_DEPTH)temp=filelistbox->seltbl[temp];
		else temp=0;
	}   
	GUI_FileListbox_Scan(filelistbox);	//����ɨ���б�
	filelistbox->selindex=temp;				//���һ��ѡ�е���Ŀ
	if(filelistbox->Scrollbar->totalitems>filelistbox->Scrollbar->itemsperpage)filelistbox->Scrollbar->topitem=temp;
//	filelistbox_draw_listbox(filelistbox);	//�ػ�
}

//��������Ŀ˫�����Ӻ���
//filelistbox:filelistboxָ��
void filelistbox_2click_hook(GUI_FileListStruct *filelistbox)
{	 	
//	u8 *pname; 
//	u8 t=0;
//	GUI_FileListStruct *filelistx;
//	filelistx=filelist_search(filelistbox->list,filelistbox->selindex);//�õ�ѡ�е�list����ϸ��Ϣ
// 	if(filelistx->type==FICO_PICTURE)//����ͼƬ�ļ�
//	{
//		pname=gui_memin_malloc(strlen((const char*)filelistx->name)+strlen((const char*)filelistbox->path)+2);//�����ڴ�
//		if(pname==NULL)return;
//	    pname=gui_path_name(pname,filelistbox->path,filelistx->name);//�ļ�������·��  
//		pic_viewer(pname);//������ļ�.
//		filelistbox_draw_listbox(filelistbox);//�ػ�filelistbox
//		gui_memin_free(pname);
//	}		  
// 	if(filelistx->type==FICO_MUSIC)//���������ļ�
//	{
//		pname=gui_memin_malloc(strlen((const char*)filelistx->name)+strlen((const char*)filelistbox->path)+2);//�����ڴ�
//		if(pname==NULL)return;
//	    pname=gui_path_name(pname,filelistbox->path,filelistx->name);//�ļ�������·��  
//
//		mp3sta&=~(1<<0);//������ֹ����
//		while(mp3sta&0x80)delay_ms(10);//�ȴ�MP3ֹͣ����
//		OSMboxPost(mp3mbox,pname);//��������
//		mp3sta|=1<<0;	//������
//		while((mp3sta&0x80)==0&&t<200)//�ȴ�MP3��ʼ����
//		{
//			delay_ms(10);
//			t++;
//		}
//		filelistbox_draw_listbox(filelistbox);//�ػ�filelistbox
//		gui_memin_free(pname);
//	}
//	printf("listbox%d index %d 2click!\r\n",filelistbox->id,filelistbox->selindex);
} 

///////////////////////////////////////��������//////////////////////////
GUI_FileListStruct * tfilelistbox;
//����
void tsflistbox_creat(u16 x,u16 y,u16 lenth,u16 height,u8 type,u8 fsize)
{
//	tfilelistbox=filelistbox_creat(x,y,lenth,height,type,fsize);
}
//����·��
void tsflistbox_setpath(u8 *path)
{	 
//	gui_memin_free(tfilelistbox->path);//�ͷ�֮ǰ��.		   
//	tfilelistbox->path=(u8*)gui_memin_malloc(strlen((const char*)path)+1);//�����ڴ�
//	if(tfilelistbox->path==NULL)return  ;//�ڴ���䲻��.	
//	tfilelistbox->path=(u8*)strcpy((char *)tfilelistbox->path,(const char*)path);
}
//ɾ��
void tsflistbox_del(void)
{
//	filelistbox_delete(tfilelistbox);
//	tfilelistbox=NULL;
//	LCD_Clear(0XFFFF);
}	  
//���1��listbox��Ŀ
u16 tsflistbox_addlist(u8 *name,u8 type)
{
//	GUI_FileList_addlist(tfilelistbox,name,type);
//	return tfilelistboxFileList_P->totalitems; 
	return 0;
}
//����listbox
void tsflistbox_draw(void)
{
//	filelistbox_draw_listbox(tfilelistbox);	 
}















