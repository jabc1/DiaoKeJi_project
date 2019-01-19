#ifndef __GUI_DRAW2D_H__
#define __GUI_DRAW2D_H__
#include "stm32f10x.h"  
#include "GUI_font.h"
	typedef struct {
		u16 XSize; 					/* X轴宽度 */
		u16 YSize; 					/* Y轴宽度 */
	//  u16 BytesPerLine;
		u8 BitsPerPixel;		/* 调色板颜色位数 */
		u16  * pMethods;		/* 调色板 */
		const u8 * pData;		/* 位图数据 */
	} GUI_BITMAP;
	
void GUI_PutStringAt(u8 *String,u16 X1, u16 Y1);	
void GUI_PutGBChar(u8 *Char,u16 x,u16 y,u8 mode,u16 F_Color,u16 B_Color,GUI_FontStruct *Font);
void GUI_PutChar(u8 *c,u16 x,u16 y,u16 FColor,u16 BColor,GUI_FontStruct *Font);
void GUI_Clear(u16 Color);

void GUI_DrawHLine(u16 X, u16 Y,u16 Len, u16 FColor);
void GUI_DrawVLine( u16 X, u16 Y, u16 Len,u16 FColor);
void GUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 FColor);
void GUI_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void GUI_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void GUI_DrawCircle(u16 x0,u16 y0,u8 r,u16 FColor);
void GUI_DrawBitmap(const GUI_BITMAP *pBitmap, int x0, int y0) ;
u16 StringWidth(u8 *String,GUI_FontStruct *Font);

void GUI_ShowStringAt(u8 *String,u16 X1, u16 Y1,u16 FColor,u16 BColor,GUI_FontStruct *Font);



void GUI_DispDecAt(u32 Value,u16 X,u16 Y,u16 len);
void GUI_DispDec(s32 Value,u16 X,u16 Y,u16 len);
void GUI_DispHexAt(u32 Value,u16 X,u16 Y,u16 len);






//extern GUIStruct GUI;	

//	
//void GUI_ShowString(u8 *String,GUI_FONT *Font,u16 X1, u16 Y1,u16 X2,u8 Mode, u16 FColor,u16 BColor);	
//	
//void GUI_PutString(u8 *String, u16 X1, u16 Y1,u16 X2, u16 Y2,u8 mode, u16 FColor,u16 BColor);
//void GUI_Clear(u16 Color);
//void GUI_DrawBitmap(const LCD_BITMAP *pBitmap, int x0, int y0);
//void GUI_DrawCircle(u16 x0,u16 y0,u8 r,u16 FColor);
//void GUI_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
//void GUI_DrawHLine(u16 X, u16 Y,u16 Len, u16 FColor);
//void GUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 FColor);
//void GUI_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
//void GUI_DrawVLine( u16 X, u16 Y, u16 Len,u16 FColor);
//void GUI_PutChar(u8 c,u16 x,u16 y,u16 FColor,u16 BColor,GUI_FONT *Font);
//void GUI_PutGBChar(u8 c[2],u16 x, u16  y, u16 FColor,u16 BColor,GUI_FONT *Font);
//void GUI_PutStringAt(u8 *String,u16 X1, u16 Y1);	
#endif 
