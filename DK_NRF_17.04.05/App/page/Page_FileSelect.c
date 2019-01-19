/*********************************************************************************
* 文件名		: page2.C
* 版本号    : V1.0.0
* 日期      : 2016/02/22
* 说明      : 图像显示函数					  
* 用法			：
* 版本说明	：
**********************************************************************************/
#include "page.h"
#include "app.h"
void FileSelectShow(void)
{
	Page_File_Struct *GUI_P;
	GUI_P = (Page_File_Struct *)GUIKJ;
	Page = PAGE_FILE;
	GUI_Clear(GUI.B_Color);
	GUI_FormDeleteAllChild(GUI.CurrentForm);
	GUI_P->FL_T = GUI_FileListCreateAsChild(GUI.CurrentForm,0,0,239,239);
	GUI_P->FL_T->OsPriority = FileTaskPriority;
	
}
