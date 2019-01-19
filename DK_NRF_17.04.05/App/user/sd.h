/*********************************************************************************
* 文件名		: menu.h
* 版本号    : V1.0.0
* 日期      : 2015/07/14
* 说明      : 定义函数						  
* 用法			：
* 版本说明	：
**********************************************************************************/
#ifndef __SD_H__
#define __SD_H__
#include "stm32f10x.h" 
#include "ff.h"

//打开文件方式宏定义，针对f_open函数
#define FA_OPEN_DEFAULT			(uint8_t)(FA_OPEN_EXISTING | FA_READ | FA_WRITE)	//可读写操作
#define FA_OPEN_READONLY		(uint8_t)(FA_OPEN_EXISTING | FA_READ)				//只读取，不执行写
#define FA_OPEN_ADD_DATA		(uint8_t)(FA_OPEN_ALWAYS | FA_READ | FA_WRITE)		//文件不存在则创建新文件
                                                                                    //可用f_lseek函数在文件上追加数据
#define FA_OPEN_NEW_FAIL		(uint8_t)(FA_CREATE_NEW | FA_READ | FA_WRITE)		//新建文件，如果存在则失败
#define FA_OPEN_NEW_COVER		(uint8_t)(FA_CREATE_ALWAYS | FA_READ | FA_WRITE)	//新建文件，如果存在则覆盖
//文件系统工作区相关定义
extern FATFS *fs[_VOLUMES];	//逻辑磁盘工作区
extern FIL *filescr;			//文件1
extern FIL *filedst;			//文件2
extern UINT br,bw;				//读写变量
extern FRESULT f_res;			//FatFs通用结果码

extern uint8_t *SDdatabuff;	//SD卡数据缓存
extern uint16_t rlen;			//读取到数据长度
extern DIR *dir;				//文件夹
extern FILINFO fileinfo;		//文件信息结构体

void InitSD(void);
#endif 

