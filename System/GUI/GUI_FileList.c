/*********************************************************************************
* 文件名			: gui_FileList.c
* 版本号    	: V3.0.0
* 作者			: ZZ
* 日期      : 2017/02/04
* 说明      : GUI FileList控件								  
* 用法			：
 	   STATE = 0x00;  0000 0000  
										|||| |||+--CHANG 				= 0：控件有变化需重新显示，1：控件无变化
										|||| ||+---DIS					=	1：控件为无效状态
										|||| |+----保留			
										|||| +-----保留
										|||+-------NEEDREDRAW
										||+--------FOCUSFileList		= 如果selectLen>0 	0:只是在选择状态，1： FileList状态 	
										|+---------COLORTYPE
										+----------是否为焦点状态
**********************************************************************************/
#include "GUI_FileList.h" 
#include "GUI_DRAW2D.h"
#include "malloc.h"
#include "gui_conf.h"
#include "ff.h"
#if GUI_USEMSOS
#include "GUI_os.h"
#endif

/************************自己的函数定义********************************************************/
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
//															},		//磁盘 
//															{ 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X75,0X55,0X55,0X55,0X50,0X00,0X00,
//																0X00,0X57,0X55,0X55,0X55,0X55,0X00,0X00,0X00,0X55,0X75,0X55,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X57,0X55,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X75,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,
//																0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X55,0X55,0X77,0X55,0X55,0X00,0X00,
//																0X00,0X05,0X55,0X77,0X55,0X55,0X00,0X00,0X00,0X00,0X55,0X77,0X00,0X00,0X00,0X00,
//																0X00,0X00,0X05,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//															},				//文件夹
//															{ 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X55,0X55,0X55,0X55,0X00,0X00,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X50,0X00,0X00,0X05,0X30,0X00,0X00,0X00,0XA5,0X00,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,
//																0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X05,0X33,0X33,0X33,0X33,0X33,0X50,0X00,0X05,0X30,0X00,0X00,0X00,0X00,0X50,0X00,
//																0X00,0X55,0X55,0X55,0X55,0X55,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//															}					//文件
//};

//filelist磁盘顺序表,即选择编号与磁盘编号对应关系
//vu8 FLIST_DISK_ORDER_TBL[3];
//filelist磁盘路径表
u8*const FLIST_DISK_PATH_TBL[2]={"0:","1:"/*,"2:"*/};
//磁盘名字表
u8*const FLIST_DISK_NAME_TBL[2]={"SD Card","FLASH Disk"/*,"U Disk",*/};		    
	const u8 chgchar[2]={0X5C,0X00};//转义符 等效"\"

/*******************************************************************************
* 函数名			: GUI_FileListCreateAsChild
* 描述	    : 创建FileList控件
* 输入参数		: Form_P：父窗口的指针； X：X轴位置；	Y：Y轴位置 W:宽度；H:高度；
* 返回参数		: filelist指针
* 说明			:
*******************************************************************************/
GUI_FileListStruct * GUI_FileListCreateAsChild(GUI_FormStruct *Form_P, u16 X,u16 Y,u16 W,u16 H)
{
 	GUI_FileListStruct * FileList_P;
	GUI_ScrollbarStruct * Scrollbar_P;	
//	if(height<=gui_phy.listheight+gui_phy.listheight+gui_phy.listheight)return NULL;//高度小了	    
//	if((height-gui_phy.listheight-gui_phy.listheight)%gui_phy.listheight)return NULL;//尺寸不合格 	 				    
//	if(width<gui_phy.listheight)return NULL;			//尺寸不合格
	FileList_P = (GUI_FileListStruct *)GUI_MEM_Malloc(sizeof(GUI_FileListStruct));	//申请内存
	if(FileList_P==NULL)return NULL;			//内存分配不够.
	GUI_MemSet((u8*)FileList_P,0,sizeof(GUI_FileListStruct));//将FileList_P的值全部设置为0    
	Scrollbar_P=GUI_ScrollbarCreate(X+W-SCOROLLBAR_WIDTH-1,Y+FILEINFO_WIDTH+1,H-FILEINFO_WIDTH*2-2,0);//创建scrollbar.
	if(Scrollbar_P==NULL)
	{
		GUI_MEM_Free(FileList_P);//释放之前申请的内存
		return NULL;					//内存分配不够.
 	}
	FileList_P->Font = &Font8x12;;		//字体大小 
 	Scrollbar_P->itemsperpage=(H-FILEINFO_WIDTH*2-2)/ (FileList_P->Font->Y+4);	//每页显示的条目数	
	FileList_P->Scrollbar=Scrollbar_P;			    			//滚动条 	
	FileList_P->X 				= X;
	FileList_P->Y					=	Y;
	FileList_P->W					= W;
	FileList_P->H					= H;
	FileList_P->ID = GUIFileListID;
	FileList_P->selindex=0XFF;	//当前选中的索引													
	FileList_P->L_B_Color=D_FILELIST_LBCOLOR;			//内部背景色
	FileList_P->L_S_Color=D_FILELIST_LSCOLOR;		//选中list后的字体颜色 
	FileList_P->L_SB_Color=D_FILELIST_LSBCOLOR;	//选中list后的背景色
	FileList_P->F_Color=D_FILELIST_FCOLOR;				//未选中的list字体颜色
	FileList_P->Rim_Color=D_FILELIST_RIMCOLOR;			//边框颜色
	FileList_P->I_B_Color=D_FILELIST_IBCOLOR;	//路径/信息栏 背景颜色
	FileList_P->OsPriority = GUI_OsCurrentPriority;		//POST的优先级为创建filelist的任务
	
	FileList_P->path=NULL;			    				//路径为空	 

//	FileList_P->fliter=0X0000;		    				//所有文件过滤(不包含文件夹)
//	FileList_P->fliterex=0XFFFF;	    				//不对小类进行过滤(所有小类均符合要求)

	FileList_P->fname 				= NULL;			    	    	//名字为空
	FileList_P->namelen				= 0;			    	    		//长度清零
//	FileList_P->curnamepos		= 0;			    	    	//当前位置清零
//	FileList_P->oldtime				= 0;			    	    		//时间清零
	 

	FileList_P->findextbl=NULL;			    		//文件索引表为空

 	FileList_P->list=NULL;								//无链表
	FileList_P->memdevflag = 0xff;
	if(GUI_FormAddChild(Form_P,(u8*)FileList_P))
	{
		GUI_MEM_Free(Scrollbar_P);	//释放内存
		GUI_MEM_Free(FileList_P);	//释放内存			
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
		GUI_MEM_Free(FileList_P->Scrollbar);	//释放内存
		if(FileList_P->list!=NULL)filelist_delete(FileList_P);	//删除之前的条目
		GUI_MEM_Free(FileList_P->findextbl);
		GUI_MEM_Free(FileList_P->path);	//删除路径
		GUI_MEM_Free(FileList_P);	//释放内存	
		
		return 0;
	}
	return 1;
}


//删除filelistbox
//filelistbox_del:要删除的filelistbox
//void filelistbox_delete(GUI_FileListStruct *filelistbox_del)
//{
// 	filelist_delete(filelistbox_del);	 	//删除条目
//	GUI_MEM_Free(filelistbox_del->findextbl);//删除目标文件索引表
//	GUI_MEM_Free(filelistbox_del->path);	//删除路径
//	GUI_MEM_Free(filelistbox_del->Scrollbar);	//删除滚动条
//	GUI_MEM_Free(filelistbox_del);	  	//删除filelistbox
//}



void GUI_FileListShowNow(GUI_FileListStruct *FileList_P)						   //0XFF  0x10  0x11   0x20   0x22
{
	u16 X1,Y1,X2,Y2;
	X1 = FileList_P->X;
	Y1 = FileList_P->Y;	
	X2 = FileList_P->X + FileList_P->W-1;
	Y2 = FileList_P->Y + FileList_P->H-1;	
	if(FileList_P->ID != GUIFileListID) 	return ;				//不是FileList的数据	
		if(!GUIGetBit(FileList_P->State,GUI_SHOWOK))			//控件是否显示完成
		{		
			if(!GUIGetBit(FileList_P->State,FILELIST_INIT))			//控件是否需要初始化显示				
			{	
				GUI_FileListbox_Scan(FileList_P);				
				GUI_DrawRectangle(X1,Y1,X2,Y2,D_FILELIST_RIMCOLOR);					
				GUI_DrawFillRectangle(X1+1,Y1+1,X2-1,Y1+FILEINFO_WIDTH,FileList_P->I_B_Color);		//标题栏			
				GUI_DrawFillRectangle(X1+1,Y2-FILEINFO_WIDTH,X2-1,Y2-1,FileList_P->I_B_Color);		//状态栏							
				GUISetBit(FileList_P->State,FILELIST_INIT);			//控件初始化完成
			}				
			if(GUIGetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR))
			{
			GUIResetBit(FileList_P->Scrollbar->State,GUI_SHOWOK);			//控件显示完成
				GUI_ScrollbarShowNow(FileList_P->Scrollbar);
			}				
			GUI_FileList_Show_Path(FileList_P);
			GUI_FileListDrawList(FileList_P);
			GUISetBit(FileList_P->State,GUI_SHOWOK);			//控件显示完成
		}

}


//显示filelistbox的路径 
//filelistbox:filelistbox指针

void GUI_FileList_Show_Path(GUI_FileListStruct *FileList_P)
{
	u16 plen=0;
	u8 *path=FileList_P->path;

	if(path)//路径有效
	{
		plen=strlen((const char*)path);
	} 
	if(plen)//需要显示
	{
		GUI_DrawFillRectangle(FileList_P->X+1,FileList_P->Y+1,FileList_P->X + FileList_P->W-2,FileList_P->Y+FILEINFO_WIDTH,FileList_P->I_B_Color);		//标题栏		
		if(plen*FileList_P->Font->ASC_X>(FileList_P->W-20))
		{
			
			while(plen*FileList_P->Font->ASC_X>(FileList_P->W-20-FileList_P->Font->ASC_X*3))
			{
 				path=(u8*)strstr((const char*)++path,(const char*)chgchar);	//去掉一截
				plen=strlen((const char*)path);					//得到新的长度
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

//查找编号为index的flist
//flistx:当前的flist指针
//index:要寻找的flist编号 0~totalitems-1;
//返回值:list结构体
//如果找到,则返回list结构体,如果没找到,返回NULL.

GUI_FileQueueStruct * filelist_search(GUI_FileQueueStruct *FileQueue_P,u16 index)
{
	u16 icnt=0;
 	GUI_FileQueueStruct * FileQueue_TP=FileQueue_P;	
	if(FileQueue_P==NULL)return NULL;
	while(FileQueue_TP->prevQueue!=NULL)FileQueue_TP=FileQueue_TP->prevQueue;//追溯到最开始的list
	while(1)//寻找编号为index的list
	{
		if(index==icnt)break;	 
		if(FileQueue_TP->nextQueue!=NULL)
		{
			FileQueue_TP=FileQueue_TP->nextQueue;    
			icnt++;//计数增加
		}else return NULL;//未找到编号为index的list
	} 
	return FileQueue_TP;//返回找到的list指针
}

//增加一条filelistbox的条目
//filelistbox:
//0,增加成功;
//1,增加失败
u8 GUI_FileList_addlist(GUI_FileListStruct * FileList_P,u8 *name,u8 type)
{
	GUI_FileQueueStruct * FileQueue_P;
	GUI_FileQueueStruct * filelisttemp;
	FileQueue_P=(GUI_FileQueueStruct*)GUI_MEM_Malloc(sizeof(GUI_FileQueueStruct));//分配内存
	if(FileQueue_P==NULL)return 1;//内存分配不够.
	GUI_MemSet(FileQueue_P,0,sizeof(GUI_FileQueueStruct));//全部设置为0
 	strcpy((char *)FileQueue_P->name,(const char*)name);	//复制name的内容到		
	FileQueue_P->type=type;								//类型
 	FileQueue_P->nextQueue=NULL;
 	if(FileList_P->list==NULL)//还未创建链表
	{
		FileQueue_P->prevQueue=NULL;
		FileList_P->list=FileQueue_P;
	}
	else //已经创建了
	{
		filelisttemp=FileList_P->list;
		while(filelisttemp->nextQueue!=NULL)filelisttemp=filelisttemp->nextQueue;//偏移到最后一个list	 
		FileQueue_P->prevQueue=filelisttemp;
		filelisttemp->nextQueue=FileQueue_P;
	}
	FileList_P->Scrollbar->totalitems++;//总条目数增加1条
	if(FileList_P->Scrollbar->totalitems > FileList_P->Scrollbar->itemsperpage)
	{
		GUISetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR);//需要显示滚条		
		FileList_P->Scrollbar->State = 0;
	}
 	else 
	{
		GUIResetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR);//不需要显示滚条
	}
	return 0;
}

/*******************************************************************************
* 函数名			: GUI_FileListDrawList
* 描述	    : 显示FileList控件的list
* 输入参数		: FileList_P：窗口的指针；
* 返回参数		: 无
* 说明			:
*******************************************************************************/
void GUI_FileListDrawList(GUI_FileListStruct *FileList_P)
{
	u16 i;
	u16 X1,Y1,X2,Y2;
  GUI_FileQueueStruct * FileQueue_P;		
	X1 = FileList_P->X+1;
	Y1 = FileList_P->Y+FILEINFO_WIDTH+1;	
	X2 = FileList_P->X + FileList_P->W-2;//条目显示结束x坐标
	Y2 = FileList_P->Y + FileList_P->H-2-FILEINFO_WIDTH;	
	
	if(GUIGetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR))
	{
		X2-=SCOROLLBAR_WIDTH;//需要滚动条了.
	}else FileList_P->Scrollbar->topitem=0;
	
	GUI_DrawFillRectangle(X1,Y1,X2,Y2,FileList_P->L_B_Color);		//LIST栏	
	
 	FileQueue_P=filelist_search(FileList_P->list,FileList_P->Scrollbar->topitem);//得到顶部list信息(curitem在listbox中用于记录顶部index)
	if(FileQueue_P==NULL)return ;//没有任何list
	for(i=0;i<FileList_P->Scrollbar->itemsperpage;i++)//显示条目
	{	
		if((FileList_P->Scrollbar->topitem+i)==FileList_P->selindex)//此项为选中项目
		{
			GUI_DrawFillRectangle(X1,Y1+i*FILELIST_LIST_H,X2,Y1+(i+1)*FILELIST_LIST_H-1,FileList_P->L_SB_Color);		//LIST栏
			FileList_P->fname=FileQueue_P->name;//获得当前选中的条目的名字
			GUI_ShowStringAt(FileQueue_P->name,X1+20,Y1+2+i*FILELIST_LIST_H,FileList_P->L_SB_Color,FileList_P->L_S_Color,FileList_P->Font);	
//			FileList_P->namelen=strlen((const char*)FileList_P->fname)*FileList_P->font/2;//名字的总长度
		}
		else
		{
			GUI_ShowStringAt(FileQueue_P->name,X1+20,Y1+2+i*FILELIST_LIST_H,FileList_P->L_B_Color,FileList_P->F_Color,FileList_P->Font);	
		}
//		gui_draw_icos(FileList_P->left,FileList_P->top+gui_phy.listheight+i*gui_phy.listheight+(gui_phy.listheight-FileList_P->icosize)/2,FileList_P->icosize,filelistx->type);//画ICO图标																				    
		
		if(FileQueue_P->nextQueue==0)break;
		FileQueue_P=FileQueue_P->nextQueue;
 	}
}


//filelist添加磁盘(在filelist启动前,必须执行一次该函数)
//filelistbox:filelistbox指针
void GUI_FileListAddDisk(GUI_FileListStruct *FileList_P)
{
	FileList_P->selindex=0XFF;
	FileList_P->foldercnt=0;
	FileList_P->filecnt=0;
//	FileList_P->memdevflag=GUI.MemDevFlag;
	
	if(FileList_P->memdevflag&(1<<0)) 	//SD卡在位
	{
			GUI_FileList_addlist(FileList_P,(u8*)FLIST_DISK_NAME_TBL[0],0);//添加一个磁盘
	}
	if(FileList_P->memdevflag&(1<<1)) 	//FLASH盘在位
	{
			GUI_FileList_addlist(FileList_P,(u8*)FLIST_DISK_NAME_TBL[1],0);//添加一个磁盘
	}	 
}


//删除所有链表 
//filelistdel,要删除的链表中的某个成员
void filelist_delete(GUI_FileListStruct * filelistbox)
{
 	GUI_FileQueueStruct * filelisttemp;
  	GUI_FileQueueStruct * filelistdel;
	if(filelistbox==NULL)return;//非法的地址,直接退出
 	if(filelistbox->list!=NULL)
	{
		filelisttemp=filelist_search(filelistbox->list,0);//得到第一个filelist的结构体
		do
		{
			filelistdel=filelisttemp;
			filelisttemp=filelisttemp->nextQueue;
		    GUI_MEM_Free(filelistdel->name);	//删除条目的名字
		    GUI_MEM_Free(filelistdel);		//删除一个条目
		}while(filelisttemp!=NULL);		//一次删除所有的条目
	}
	filelistbox->list=NULL;//清空
 	filelistbox->Scrollbar->totalitems=0;//总条目清零
	filelistbox->Scrollbar->topitem=0;	//顶部条目清零
}

//扫描当前路径下的所有文件
//filelistbox:filelistbox指针
//返回值:0,正常;
//    其他,操作磁盘出现错误
u8 GUI_FileListbox_Scan(GUI_FileListStruct *FileList_P)
{
 	DIR flboxdir;		//flbox专用	  
	FILINFO flboxfinfo;	 
	u8 res=0; 
	u16 type=0XFFFF;
//	u16 typeex=0;
	u16 findex;	  
  u8 *fn;   //This function is assuming non-Unicode cfg. 
	u8 mark=0;//0,查找文件夹;1,查找目标文件;2

#if _USE_LFN						
 	flboxfinfo.lfsize=_MAX_LFN * 2 + 1;
	flboxfinfo.lfname=GUI_MEM_Malloc(flboxfinfo.lfsize);
	if(flboxfinfo.lfname==NULL)return 0XFF;//申请内存失败 
   GUI_MemSet((char *)flboxfinfo.lfname,0,flboxfinfo.lfsize);
#endif	 
	 
 	if(FileList_P->path)	//路径有效
	{
		res=f_opendir(&flboxdir,(const TCHAR*)FileList_P->path); //打开一个目录
		if(res==FR_OK)		//打开目录成功了.
		{
			if(FileList_P->list!=NULL)filelist_delete(FileList_P);	//删除之前的条目
			GUI_FileList_addlist(FileList_P,(u8*)"..",1);	//添加返回上一层伪文件夹 
 			FileList_P->foldercnt=0;
			FileList_P->filecnt=0;
			GUI_MEM_Free(FileList_P->findextbl);//释放之前申请的索引表内存
			FileList_P->findextbl=(u16*)GUI_MEM_Malloc(FLBOX_MAX_DFFILE*2);//先默认申请FLBOX_MAX_DFFILE个目标文件索引内存
 		} 
		while(res==FR_OK)
		{
			findex=(u16)flboxdir.index;//记录下此文件的index值
			res=f_readdir(&flboxdir,&flboxfinfo);//读取文件信息
			if(res!=FR_OK||flboxfinfo.fname[0]==0)
			{
				if(mark==0)	//文件夹查找完了
				{
					mark=1;	//标记查找目标文件
//					f_closedir(&flboxdir);
					res=f_opendir(&flboxdir,(const TCHAR*)FileList_P->path); //再次打开该目录
 					continue;
				}
				break;		//错误了/到末尾了,退出   
			}
			if (flboxfinfo.fname[0]=='.'&&flboxfinfo.fname[1]==0)continue;//忽略本级目录
			fn=(u8*)(*flboxfinfo.lfname?flboxfinfo.lfname:flboxfinfo.fname);
			if(flboxfinfo.fattrib&(1<<4)&&mark==0)//是一个文件夹且mark=0
			{
				type=FICO_FOLDER;	
				FileList_P->foldercnt++;
			}
			else if(flboxfinfo.fattrib&(1<<5)&&mark==1)//是一个归档文件且mark=1
			{
				type=FICO_FILE;	//类型文件

					if(FileList_P->filecnt<FLBOX_MAX_DFFILE)FileList_P->findextbl[FileList_P->filecnt]=findex;//记录下文件索引
					FileList_P->filecnt++;//目标文件增加
			}else continue;		//继续找下一个 
			if(GUI_FileList_addlist(FileList_P,fn,type))break;//添加失败了         
		}
		if(mark==1&&FileList_P->filecnt)
		{
			fn=GUI_MEM_Realloc(FileList_P->findextbl,FileList_P->filecnt*2);//释放多余的内存
			if(fn!=NULL)FileList_P->findextbl=(u16*)fn;//如果申请成功了,则使用新的地址,否则源地址不变
		}   
	}else//没有任何有效路径,说明回到选择磁盘的状态了
	{
		filelist_delete(FileList_P);		//删除之前的条目
		GUI_FileListAddDisk(FileList_P);	//重新添加磁盘路径
 	}
	GUI_MEM_Free(flboxfinfo.lfname);
	return res;
}











//重建文件列表框
//当有效文件使用完退出后使用,重新扫描文件，并重画
//filelistbox:listbox指针
u8 filelistbox_rebuild_filelist(GUI_FileListStruct * filelistbox)
{
	u8 res=0;
	if(filelistbox!=NULL)
	{
		res=GUI_FileListbox_Scan(filelistbox);	//重新扫描列表	  
		if(filelistbox->Scrollbar->totalitems>filelistbox->Scrollbar->itemsperpage)filelistbox->Scrollbar->topitem=filelistbox->selindex;
		GUI_FileListDrawList(filelistbox);		//重画 
 	}else res=2;
	return res;
}	 


/********************************************
函数：GUI_FileListTouchUp
功能：触摸屏触摸释放的时候，设置FileList的状态
参数： FileList_P: FileList地址指针；
返回：无
说明：
********************************************/
void GUI_FileListTouchUp(GUI_FileListStruct *FileList_P)
{
	if(!GUIGetBit(FileList_P->State,GUI_DIS))							//不是DIS状态
	{
		GUIResetBit(FileList_P->State,GUI_ON);				//设置为OFF状态
		GUIResetBit(FileList_P->State,GUI_SHOWOK);			//控件需重画
		GUIResetBit(FileList_P->State,FILELIST_TOUCH);			//按键被按下
		GUIResetBit(FileList_P->State,FILELIST_CLOSE);			//控件关闭
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
	if(!GUIGetBit(FileList_P->State,GUI_DIS))		//不是DISABLE
	{
		if(GUIGetBit(FileList_P->State,FILELIST_TOUCH))		//按键被按下
		{
			FileQueue_P=filelist_search(FileList_P->list,FileList_P->selindex);//得到选中的list的详细信息
			if(FileQueue_P->type == FICO_FILE)
			{
				GUI_FL_OpenFile(FileList_P->OsPriority,(u32*)FileList_P);
				
//				#if GUI_USEMSOS
//				FileListMessage = (((u32)GUI_FILELISTMESSAGE<<24) | (((u32)FileList_P) & 0xFFFFFF));			
//					OS.PostMessageQueue(FileList_P->OsPriority, (void *)(FileListMessage));		//发送长时间按下功能键的键值
//				#else
//					//非OS的时候运行的程序
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
		else if(GUIGetBit(FileList_P->State,FILELIST_CLOSE))		//关闭按钮被按下
		{
				GUI_FL_Close(FileList_P->OsPriority,(u32*)FileList_P);
//			#if GUI_USEMSOS
//				FileListMessage = (((u32)FileListCloseMessageType<<24) | (((u32)FileList_P) & 0xFFFFFF));		
//				OS.PostMessageQueue(FileList_P->OsPriority, (void *)(FileListMessage));		//发送长时间按下功能键的键值		
//			#else
//				//非OS的时候运行的程序
//			#endif				
			GUIResetBit(FileList_P->State,FILELIST_CLOSE);			
			
		}
	}
	return GUI_false;	
	
}



/********************************************
函数：GUI_FileListTouchDown
功能：触摸屏触摸按下的时候，设置FileList的状态
参数： FileList_P: FileList地址指针；Num：在当前的FORM中的编号
返回：无
说明：
********************************************/
u8 GUI_FileListTouchDown(GUI_FileListStruct *FileList_P,u8 Num)
{
	u16 i,Max;
	u16 X1,Y1,X2,Y2;	
	X1 = FileList_P->X;
	Y1 = FileList_P->Y+FILEINFO_WIDTH+1;	
	X2 = FileList_P->X + FileList_P->W-1;//条目显示结束x坐标
	Y2 = FileList_P->Y + FileList_P->H-1-FILEINFO_WIDTH-1;	
	if(FileList_P == GUI_null)		return GUI_false;
	FileList_P->selindex = 0XFF;
	GUIResetBit(FileList_P->State,FILELIST_TOUCH);
	if(!GUIGetBit(FileList_P->State,GUI_DIS))			//控件是否是使能状态
		{										//不是DIS状态				
			if((GUI.Touch_Y>Y1)&&(GUI.Touch_Y<Y2))		 //判断是否在Y坐标内部
			{
				if((GUI.Touch_X > X2-SCOROLLBAR_WIDTH)&&(GUI.Touch_X<X2))		 //判断是否在滚动条内部		
				{				
					if(GUIGetBit(FileList_P->State,FILELIST_NEEDSCROLLBAR))		//是否有滚动条
					{
						//SCOROLLBAR
						if(GUI_ScrollbarTouchDown(FileList_P->Scrollbar,GUI.Touch_X,GUI.Touch_Y))
						{
								GUIResetBit(FileList_P->State,GUI_SHOWOK);								
						}
						X2 =  X2-SCOROLLBAR_WIDTH;
					}

				}			
				
				if((GUI.Touch_X > X1)&&(GUI.Touch_X<X2))		 //判断是否在内部
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

							FileList_P->selindex = i+FileList_P->Scrollbar->topitem;															//保存被按下的键编号			
							GUI.CurrentForm->TouchNum = Num;
//							GUI.TouchChange = TOUCHNULL;															//清除GUI_API的touch标志
							GUISetBit(FileList_P->State,FILELIST_TOUCH);
							GUIResetBit(FileList_P->State,GUI_SHOWOK);
							return GUI_true;						

						}	
					}					
					return GUI_true;					
				}
			}
			else if((GUI.Touch_Y >FileList_P->Y)&&(GUI.Touch_Y<Y1)&&(GUI.Touch_X > X2-SCOROLLBAR_WIDTH)&&(GUI.Touch_X<X2))	//在关闭控件的区域内
					{
							GUI.CurrentForm->TouchNum = Num;
//							GUI.TouchChange = TOUCHNULL;															//清除GUI_API的touch标志
							GUISetBit(FileList_P->State,FILELIST_CLOSE);
							GUIResetBit(FileList_P->State,GUI_SHOWOK);						
					return GUI_true;	
					}
		return GUI_false;		
	}
		return GUI_false;			
}























////磁盘容量字符串
//u8*const FLBOX_DISK_SIZE_CAPTION_TBL[3]=
//{
//	"磁盘容量:","磁盤容量:","Capacity:",	
//};
////磁盘可用空间字符串
//u8*const FLBOX_DISK_FREE_CAPTION_TBL[3]=
//{
//	"可用空间:","可用空間:","Free:",	
//};
////文件夹字符串
//u8*const FLBOX_FOLDER_CAPTION_TBL[3]=
//{
//	"文件夹:","文件夾:","Folder:",	
//};
////磁盘可用空间字符串
//u8*const FLBOX_FILES_CAPTION_TBL[3]=
//{
//	"  文件:","  文件:","  Files:",	
//}; 



//显示信息  
//filelistbox:filelistbox指针
//mode:0,非打开文件夹/磁盘的操作.1,打开文件夹或磁盘,需要更新全部信息
void filelistbox_show_info(GUI_FileListStruct *filelistbox)
{
//	GUI_FileListStruct *filelistx;
//	FATFS *fs1;
//	u8 res;
//    u32 freclust=0,fresec=0,totsec=0;
// 	u8 * temp;
//	u8 * temp1;
//	u8 * mark;
//	temp=gui_memin_malloc(60);	//用于存放字符串
//	if(temp==NULL)return;
// 	temp1=gui_memin_malloc(30);	//用于存放数字串
//	if(temp1==NULL)
//	{
//		gui_memin_free(temp);	
//		return;  
//	}
// 	gui_memset((char *)temp,0,60);
//  	gui_memset((char *)temp1,0,30);
//	gui_fill_rectangle(filelistbox->left,filelistbox->top+filelistbox->height-gui_phy.listheight,filelistbox->width,gui_phy.listheight,filelistbox->pthinfbkcolor);//显示文件信息背景色
//	gui_draw_hline(filelistbox->left,filelistbox->top+filelistbox->height-gui_phy.listheight,filelistbox->width,0X000);//显示黑色的横线		
//	filelistx=filelist_search(filelistbox->list,filelistbox->selindex);//得到选中的list的详细信息
//	if(filelistx->type==FICO_DISK)//当前选中的是磁盘
//	{
//		mark=(u8*)FLIST_DISK_PATH_TBL[FLIST_DISK_ORDER_TBL[filelistbox->selindex]]; 
//		res=f_getfree((const TCHAR*)mark,(DWORD*)&freclust,&fs1);//获得剩余容量
//	    if(res==0)
//		{											   
//		    totsec=(fs1->n_fatent-2)*fs1->csize;//得到总扇区数
//		    fresec=freclust*fs1->csize;			//得到空闲扇区数	   
//#if _MAX_SS!=512								//对于扇区区不是512字节大小的磁盘
//			totsec*=fs1->ssize/512;
//			fresec*=fs1->ssize/512;
//#endif	  
//		}
//		if(totsec<20480)//总容量小于10M
//		{
//			totsec>>=1,fresec>>=1;
//			mark="KB ";
//		}else
//		{
//			totsec>>=11,fresec>>=11;
//			mark="MB ";
// 		}
//		strcpy((char*)temp,(const char*)FLBOX_DISK_SIZE_CAPTION_TBL[gui_phy.language]);//磁盘容量	
//		gui_num2str(temp1,totsec);				//总容量转为字符串
//		strcat((char*)temp,(const char*)temp1);	
//		strcat((char*)temp,(const char*)mark);	//添加单位
//  		strcat((char*)temp,(const char*)FLBOX_DISK_FREE_CAPTION_TBL[gui_phy.language]);//可用空间		
// 		gui_num2str(temp1,fresec);				//可用空间转为字符串
//		strcat((char*)temp,(const char*)temp1);	
//		strcat((char*)temp,(const char*)mark);	//添加单位
//	}else	//选中的是文件/文件夹
//	{
// 		if(filelistbox->selindex<filelistbox->foldercnt)fresec=filelistbox->selindex+1;	//在文件夹总数目以内.
//		else freclust=filelistbox->selindex-filelistbox->foldercnt+1;		//在文件总数目以内
//		strcpy((char*)temp,(const char*)FLBOX_FOLDER_CAPTION_TBL[gui_phy.language]);//文件夹	
//		gui_num2str(temp1,fresec);					//当前文件夹编号转为字符串
//		strcat((char*)temp,(const char*)temp1);	
// 		strcat((char*)temp,(const char*)"/");		
//		gui_num2str(temp1,filelistbox->foldercnt);	//文件夹总数转为字符串
// 		strcat((char*)temp,(const char*)temp1);	
// 		strcat((char*)temp,(const char*)FLBOX_FILES_CAPTION_TBL[gui_phy.language]);//文件		
//		gui_num2str(temp1,freclust);				//当前文件编号转为字符串
//		strcat((char*)temp,(const char*)temp1);	
// 		strcat((char*)temp,(const char*)"/");		
//		gui_num2str(temp1,filelistbox->filecnt);	//文件总数转为字符串
// 		strcat((char*)temp,(const char*)temp1);	
//	}
//	gui_show_ptstr(filelistbox->left,filelistbox->top+filelistbox->height-gui_phy.listheight+(gui_phy.listheight-gui_phy.listfsize)/2,filelistbox->width+filelistbox->left,gui_phy.lcdheight,0,filelistbox->lncolor,gui_phy.listfsize,temp,1);//显示路径
//	gui_memin_free(temp);
// 	gui_memin_free(temp1);
}


//获得路径的目录深度
//path:路径
u8 filelistbox_get_pathdepth(u8* path)
{
	u8 depth=0;
	while(*path!=0)
	{
		if(*path==0X5C)//找到'\'了
		{
			depth++;
		}
		path++;
	}
	return depth;
	
	
}

//把name加入到path后面.自动添加\和结束符.但是要确保pname的内存空间足够,否则可能导致死机.
//pname:带路径的文件名.path的内存大小至少为:strlen(path)+strlen(name)+2!
//path:路径
//name:名字
//返回值:带路径的文件名
u8* gui_path_name(u8 *pname,u8* path,u8 *name)
{
//	const u8 chgchar[2]={0X5C,0X00};//转义符 等效"\"    
	strcpy((char *)pname,(const char*)path);	//拷贝path到pname里面
	strcat((char*)pname,(const char*)chgchar);	//添加转义符
	strcat((char*)pname,(const char*)name);		//添加新增的名字
	return pname;
}
//扫描路径下的文件
//filelistbox:filelist指针
void filelistbox_check_filelist(GUI_FileListStruct *filelistbox)
{
	u8 *str;
	u8 *oldpath;
	u16 temp;
	u8 res=0;
	GUI_FileQueueStruct *filelistx;
	u16 ttlen;//总长度
	filelistx=filelist_search(filelistbox->list,filelistbox->selindex);//得到选中的list的详细信息
 	if(filelistx->type==0||filelistx->type==1)//属于磁盘/文件夹,则打开	
	{   
		temp=filelistbox_get_pathdepth(filelistbox->path);//得到当前目录的深度
		if(filelistx->type==0)//是磁盘
		{	
			str=(u8*)FLIST_DISK_PATH_TBL[filelistbox->selindex];  
			GUI_MEM_Free(filelistbox->path);//释放之前申请的内存
			filelistbox->path=(u8*)GUI_MEM_Malloc(strlen((const char*)str)+1);//重新申请内存
			if(filelistbox->path==NULL)return;//申请内存失败.
 			strcpy((char *)filelistbox->path,(const char*)str);//拷贝str到path里面
		}else //是文件夹
		{
			if(strcmp((const char*)filelistx->name,".."))//不是..文件名
			{
				if(temp<FLBOX_PATH_DEPTH)filelistbox->seltbl[temp]=filelistbox->selindex;//记录下当前目录下选中的
			  	ttlen=strlen((const char*)filelistbox->path);//得到原来的路径的长度(不包含结束符)
				ttlen+=strlen((const char*)filelistx->name);	//得到名字的长度
				oldpath=(u8*)GUI_MEM_Malloc(strlen((const char*)filelistbox->path)+1);//申请内存
				if(oldpath==NULL)return;//申请内存失败
 				oldpath=(u8*)strcpy((char *)oldpath,(const char*)filelistbox->path);//拷贝filelistbox->path到oldpath里面
	 			GUI_MEM_Free(filelistbox->path);//释放filelistbox->path之前申请的内存
				filelistbox->path=(u8*)GUI_MEM_Malloc(ttlen+2);//重新申请内存
				if(filelistbox->path==NULL)
				{
					GUI_MEM_Free(oldpath);
					return;//申请内存失败.
				}
 				filelistbox->path=gui_path_name(filelistbox->path,oldpath,filelistx->name);//路径和文件名结合
				GUI_MEM_Free(oldpath);//释放申请的内存.
				temp=0;//进入新目录,选择0位置.
			}else //..,进入上一层目录
			{
				filelistbox_back(filelistbox);//退回上一层目录
				return ;  
 			}
		} 
		res=GUI_FileListbox_Scan(filelistbox);	//重新扫描列表
		filelistbox->selindex=temp;				//最后一次选中的条目
		if(filelistbox->Scrollbar->totalitems>filelistbox->Scrollbar->itemsperpage)filelistbox->Scrollbar->topitem=temp;
		if(res==0&&filelistx->type==0&&filelistbox->foldercnt==0&&filelistbox->filecnt==0)	//磁盘根目录扫描正常,且跟目录下没有任何文件/文件夹
		{	    
 			GUI_FileList_addlist(filelistbox,"..",1);	//添加返回上一层伪文件夹 
		}
 	}else 
	{
//		filelistbox->dbclick|=1<<0;//标记是目标文件
	}	 
}

//向上一级操作.(返回操作)
//此函数可以单独调用,自动返回上一层.自带了扫描及重画功能.
//filelistbox:filelistbox指针
void filelistbox_back(GUI_FileListStruct *filelistbox)
{
	u8 *oldpath;
	u16 temp=0; 
	oldpath=filelistbox->path;
	while(*oldpath!=0)//一直偏移到最后.
	{
		oldpath++;
		temp++;
	}
	if(temp<4)
	{
		if(filelistbox->path)GUI_MEM_Free(filelistbox->path);//释放之前申请到的内存
		filelistbox->path=NULL;//退到最顶层
		temp=0;
	}else	   
	{	  
		while(*oldpath!=0x5c)oldpath--;	//追述到倒数第一个"\"处 
		*oldpath='\0';//用结束符代替"\".从而回到上一级目录
		temp=filelistbox_get_pathdepth(filelistbox->path);//得到新目录的深度
		if(temp<FLBOX_PATH_DEPTH)temp=filelistbox->seltbl[temp];
		else temp=0;
	}   
	GUI_FileListbox_Scan(filelistbox);	//重新扫描列表
	filelistbox->selindex=temp;				//最后一次选中的条目
	if(filelistbox->Scrollbar->totalitems>filelistbox->Scrollbar->itemsperpage)filelistbox->Scrollbar->topitem=temp;
//	filelistbox_draw_listbox(filelistbox);	//重画
}

//滚动条条目双击钩子函数
//filelistbox:filelistbox指针
void filelistbox_2click_hook(GUI_FileListStruct *filelistbox)
{	 	
//	u8 *pname; 
//	u8 t=0;
//	GUI_FileListStruct *filelistx;
//	filelistx=filelist_search(filelistbox->list,filelistbox->selindex);//得到选中的list的详细信息
// 	if(filelistx->type==FICO_PICTURE)//属于图片文件
//	{
//		pname=gui_memin_malloc(strlen((const char*)filelistx->name)+strlen((const char*)filelistbox->path)+2);//申请内存
//		if(pname==NULL)return;
//	    pname=gui_path_name(pname,filelistbox->path,filelistx->name);//文件名加入路径  
//		pic_viewer(pname);//打开这个文件.
//		filelistbox_draw_listbox(filelistbox);//重画filelistbox
//		gui_memin_free(pname);
//	}		  
// 	if(filelistx->type==FICO_MUSIC)//属于音乐文件
//	{
//		pname=gui_memin_malloc(strlen((const char*)filelistx->name)+strlen((const char*)filelistbox->path)+2);//申请内存
//		if(pname==NULL)return;
//	    pname=gui_path_name(pname,filelistbox->path,filelistx->name);//文件名加入路径  
//
//		mp3sta&=~(1<<0);//请求终止解码
//		while(mp3sta&0x80)delay_ms(10);//等待MP3停止播放
//		OSMboxPost(mp3mbox,pname);//发送邮箱
//		mp3sta|=1<<0;	//允许播放
//		while((mp3sta&0x80)==0&&t<200)//等待MP3开始播放
//		{
//			delay_ms(10);
//			t++;
//		}
//		filelistbox_draw_listbox(filelistbox);//重画filelistbox
//		gui_memin_free(pname);
//	}
//	printf("listbox%d index %d 2click!\r\n",filelistbox->id,filelistbox->selindex);
} 

///////////////////////////////////////测试序列//////////////////////////
GUI_FileListStruct * tfilelistbox;
//创建
void tsflistbox_creat(u16 x,u16 y,u16 lenth,u16 height,u8 type,u8 fsize)
{
//	tfilelistbox=filelistbox_creat(x,y,lenth,height,type,fsize);
}
//设置路径
void tsflistbox_setpath(u8 *path)
{	 
//	gui_memin_free(tfilelistbox->path);//释放之前的.		   
//	tfilelistbox->path=(u8*)gui_memin_malloc(strlen((const char*)path)+1);//分配内存
//	if(tfilelistbox->path==NULL)return  ;//内存分配不够.	
//	tfilelistbox->path=(u8*)strcpy((char *)tfilelistbox->path,(const char*)path);
}
//删除
void tsflistbox_del(void)
{
//	filelistbox_delete(tfilelistbox);
//	tfilelistbox=NULL;
//	LCD_Clear(0XFFFF);
}	  
//添加1个listbox条目
u16 tsflistbox_addlist(u8 *name,u8 type)
{
//	GUI_FileList_addlist(tfilelistbox,name,type);
//	return tfilelistboxFileList_P->totalitems; 
	return 0;
}
//画出listbox
void tsflistbox_draw(void)
{
//	filelistbox_draw_listbox(tfilelistbox);	 
}















