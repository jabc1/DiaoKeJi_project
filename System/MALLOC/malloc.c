#include "malloc.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//�ڴ��(4�ֽڶ���)
__align(4) u8 membase[MEM_MAX_SIZE];			//SRAM�ڴ��
//�ڴ�����
u16 memmapbase[MEM_ALLOC_TABLE_SIZE];			//SRAM�ڴ��MAP
//�ڴ�������	   
const u32 memtblsize=MEM_ALLOC_TABLE_SIZE;		//�ڴ���С
const u32 memblksize=MEM_BLOCK_SIZE;			//�ڴ�ֿ��С
const u32 memsize=MEM_MAX_SIZE;					//�ڴ��ܴ�С


//�ڴ���������
struct _m_mallco_dev mallco_dev=
{
	mem_init,			//�ڴ��ʼ��
	mem_perused,		//�ڴ�ʹ����
	membase,			//�ڴ��
	memmapbase,			//�ڴ����״̬��
	0,  				//�ڴ����δ����
};

//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	   
//�ڴ�����ʼ��  
void mem_init(void)  
{  
    mymemset(mallco_dev.memmap, 0,memtblsize*2);//�ڴ�״̬����������  
	mymemset(mallco_dev.membase, 0,memsize);	//�ڴ��������������  
	mallco_dev.memrdy=1;						//�ڴ�����ʼ��OK  
}  
//��ȡ�ڴ�ʹ����
//����ֵ:ʹ����(0~100)
u8 mem_perused(void)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize;i++)  
    {  
        if(mallco_dev.memmap[i])used++; 
    } 
    return (used*100)/(memtblsize);  
}  
//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
u32 mem_malloc(u32 size)  
{  
    signed long offset=0;  
    u16 nmemb;	//��Ҫ���ڴ����  
	u16 cmemb=0;//�������ڴ����
    u32 i;  
    if(!mallco_dev.memrdy)mallco_dev.init();	//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;				//����Ҫ����
    nmemb=size/memblksize;  					//��ȡ��Ҫ����������ڴ����
    if(size%memblksize)nmemb++;  
    for(offset=memtblsize-1;offset>=0;offset--)	//���������ڴ������  
    {     
		if(!mallco_dev.memmap[offset])cmemb++;	//�������ڴ��������
		else cmemb=0;							//�����ڴ������
		if(cmemb==nmemb)						//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<nmemb;i++)  				//��ע�ڴ��ǿ� 
            {  
                mallco_dev.memmap[offset+i]=nmemb;  
            }  
            return (offset*memblksize);			//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  
//�ͷ��ڴ�(�ڲ�����) 
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
u8 mem_free(u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy)//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init();    
        return 1;//δ��ʼ��  
    }  
    if(offset<memsize)//ƫ�����ڴ����. 
    {  
        int index=offset/memblksize;		//ƫ�������ڴ�����  
        int nmemb=mallco_dev.memmap[index];	//�ڴ������
        for(i=0;i<nmemb;i++)  				//�ڴ������
        {  
            mallco_dev.memmap[index+i]=0;  
        }
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}  
//�ͷ��ڴ�(�ⲿ����) 
//ptr:�ڴ��׵�ַ 
void myfree(void *ptr)  
{  
	u32 offset;  
    if(ptr==NULL)return;//��ַΪ0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase;  
    mem_free(offset);	//�ͷ��ڴ�     
}  
//�����ڴ�(�ⲿ����)
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(u32 size)  
{  
    u32 offset;  									      
	offset=mem_malloc(size);  	   				   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase+offset);  
}  
//���·����ڴ�(�ⲿ����)
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void *myrealloc(void *ptr,u32 size)  
{  
    u32 offset;  
    offset=mem_malloc(size);  
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
        myfree(ptr);  											  	//�ͷž��ڴ�
        return (void*)((u32)mallco_dev.membase+offset);  			//�������ڴ��׵�ַ
    }  
}

































//#include "malloc.h"	    
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEKս��STM32������
////�ڴ���� ��������	   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2012/9/16
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
////All rights reserved									  
////////////////////////////////////////////////////////////////////////////////////





////mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����
//#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
//#define MEM1_MAX_SIZE			3*1024  						//�������ڴ� 40K
//#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С

////mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����,�����Ĵ����ڲ�SRAM����
//#define MEM2_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
//#define MEM2_MAX_SIZE			1*64  						//�������ڴ�200K
//#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С
//		 
//		 
////�ڴ���������
//struct _m_mallco_dev
//{
//	void (*init)(u8);					//��ʼ��

//	
//	u8 (*perused)(u8);		  	    	//�ڴ�ʹ����
//	u8 	*membase[2];					//�ڴ�� ����2��������ڴ�
//	u16 *memmap[2]; 					//�ڴ����״̬��
//	u8  memrdy[2]; 						//�ڴ�����Ƿ����
//};
//extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

//void mymemset(void *s,u8 c,u32 count);	 //�����ڴ�
//void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     

//u32 mem_malloc(u8 memx,u32 size);		 //�ڴ����(�ڲ�����)
//u8 mem_free(u8 memx,u32 offset);		 //�ڴ��ͷ�(�ڲ�����)
//u8 mem_perused(u8 memx);				 //����ڴ�ʹ����(��/�ڲ�����) 

////�ڴ��(4�ֽڶ���)
//__align(4) u8 mem1base[MEM1_MAX_SIZE];													//�ڲ�SRAM�ڴ��
//__align(4) u8 mem2base[MEM2_MAX_SIZE];					//�ⲿSRAM�ڴ��
////__align(4) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//�ⲿSRAM�ڴ��
////�ڴ�����
//u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//�ڲ�SRAM�ڴ��MAP
//u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE];	//�ⲿSRAM�ڴ��MAP
////u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//�ⲿSRAM�ڴ��MAP
////�ڴ�������	   
//const u32 memtblsize[2]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};		//�ڴ���С
//const u32 memblksize[2]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};					//�ڴ�ֿ��С
//const u32 memsize[2]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};							//�ڴ��ܴ�С


////�ڴ���������
//struct _m_mallco_dev mallco_dev=
//{
//	mem_init,				//�ڴ��ʼ��
//	mem_perused,			//�ڴ�ʹ����
//	mem1base,mem2base,		//�ڴ��
//	mem1mapbase,mem2mapbase,//�ڴ����״̬��
//	0,0,  					//�ڴ����δ����
//};

////�����ڴ�
////*des:Ŀ�ĵ�ַ
////*src:Դ��ַ
////n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
//void mymemcpy(void *des,void *src,u32 n)  
//{  
//    u8 *xdes=des;
//	u8 *xsrc=src; 
//    while(n--)*xdes++=*xsrc++;  
//}  
////�����ڴ�
////*s:�ڴ��׵�ַ
////c :Ҫ���õ�ֵ
////count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
//void mymemset(void *s,u8 c,u32 count)  
//{  
//    u8 *xs = s;  
//    while(count--)*xs++=c;  
//}	    
////�ڴ�����ʼ��  
////memx:�����ڴ��
//void mem_init(u8 memx)  
//{  
//    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//�ڴ�״̬����������  
//	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//�ڴ��������������  
//	mallco_dev.memrdy[memx]=1;								//�ڴ�����ʼ��OK  
//}  
////��ȡ�ڴ�ʹ����
////memx:�����ڴ��
////����ֵ:ʹ����(0~100)
//u8 mem_perused(u8 memx)  
//{  
//    u32 used=0;  
//    u32 i;  
//    for(i=0;i<memtblsize[memx];i++)  
//    {  
//        if(mallco_dev.memmap[memx][i])used++; 
//    } 
//    return (used*100)/(memtblsize[memx]);  
//}  
////�ڴ����(�ڲ�����)
////memx:�����ڴ��
////size:Ҫ������ڴ��С(�ֽ�)
////����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
//u32 mem_malloc(u8 memx,u32 size)  
//{  
//    signed long offset=0;  
//    u16 nmemb;	//��Ҫ���ڴ����  
//	u16 cmemb=0;//�������ڴ����
//    u32 i;  
//    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
//    if(size==0)return 0XFFFFFFFF;//����Ҫ����
//    nmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
//    if(size%memblksize[memx])nmemb++;  
//    for(offset=memtblsize[memx]-1;offset>=0;offset--)//���������ڴ������  
//    {     
//		if(!mallco_dev.memmap[memx][offset])cmemb++;//�������ڴ��������
//		else cmemb=0;								//�����ڴ������
//		if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
//		{
//            for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
//            {  
//                mallco_dev.memmap[memx][offset+i]=nmemb;  
//            }  
//            return (offset*memblksize[memx]);//����ƫ�Ƶ�ַ  
//		}
//    }  
//    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
//}  
////�ͷ��ڴ�(�ڲ�����) 
////memx:�����ڴ��
////offset:�ڴ��ַƫ��
////����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
//u8 mem_free(u8 memx,u32 offset)  
//{  
//    int i;  
//    if(!mallco_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
//	{
//		mallco_dev.init(memx);    
//        return 1;//δ��ʼ��  
//    }  
//    if(offset<memsize[memx])//ƫ�����ڴ����. 
//    {  
//        int index=offset/memblksize[memx];			//ƫ�������ڴ�����  
//        int nmemb=mallco_dev.memmap[memx][index];	//�ڴ������
//        for(i=0;i<nmemb;i++)  						//�ڴ������
//        {  
//            mallco_dev.memmap[memx][index+i]=0;  
//        }  
//        return 0;  
//    }else return 2;//ƫ�Ƴ�����.  
//}  
////�ͷ��ڴ�(�ⲿ����) 
////memx:�����ڴ��
////ptr:�ڴ��׵�ַ 
//void myfree(u8 memx,void *ptr)  
//{  
//	u32 offset;  
//    if(ptr==NULL)return;//��ַΪ0.  
// 	offset=(u32)ptr-(u32)mallco_dev.membase[memx];  
//    mem_free(memx,offset);//�ͷ��ڴ�     
//}  
////�����ڴ�(�ⲿ����)
////memx:�����ڴ��
////size:�ڴ��С(�ֽ�)
////����ֵ:���䵽���ڴ��׵�ַ.
//void *mymalloc(u8 memx,u32 size)  
//{  
//    u32 offset;  									      
//	offset=mem_malloc(memx,size);  	   				   
//    if(offset==0XFFFFFFFF)return NULL;  
//    else return (void*)((u32)mallco_dev.membase[memx]+offset);  
//}  
////���·����ڴ�(�ⲿ����)
////memx:�����ڴ��
////*ptr:���ڴ��׵�ַ
////size:Ҫ������ڴ��С(�ֽ�)
////����ֵ:�·��䵽���ڴ��׵�ַ.
//void *myrealloc(u8 memx,void *ptr,u32 size)  
//{  
//    u32 offset;  
//    offset=mem_malloc(memx,size);  
//    if(offset==0XFFFFFFFF)return NULL;     
//    else  
//    {  									   
//	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
//        myfree(memx,ptr);  											  		//�ͷž��ڴ�
//        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//�������ڴ��׵�ַ
//    }  
//}












