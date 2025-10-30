#ifndef __EBOOK__
#define __EBOOK__

#include "ebook_type.h"
//#include "ebook_parse.h"


void Ebook_Init(void);
void Ebook_Uninit(void);
int Ebook_GetCharHeight(EFONT_CLASS font);
int Ebook_GetCharWidth(EFONT_CLASS font, ECHAR ch);
int Ebook_GetStrSize(EFONT_CLASS font, const ECHAR *str, EU16 *width, EU16 *height, int hgap);
EBOOL Ebook_GetCharPixels(EFONT_CLASS font, ECHAR ch, EBOOK_FONT_PIXEL_T *pixel_info);

EBOOL Ebook_DrawStr(EBOOK_LCD_T *lcd, const EBOOK_STR_T *str, const EBOOK_DISP_STYLE_T *style);

EBOOL strToUniStr(ECHAR *dst, int dstLen, const char *src);


#endif // __EBOOK__

