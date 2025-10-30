#ifndef __EBOOK_FONT__
#define __EBOOK_FONT__

#include "ebook_type.h"

#if 0
// 假定最大字符是40x40点阵
#define MAX_PIXEL_BYTE (40*40/8)
#endif

typedef void* EFONT_HANDLE;

#if 0
typedef struct {
	unsigned char startline;   // 从第几行显示
	unsigned char lines;       // 行数
	unsigned char stride;      // 每行字节数
	unsigned char width;       // 每行点阵数
	unsigned char pixels[MAX_PIXEL_BYTE]; // 点阵信息,共stride * lines 字节有效数据
}EBOOK_FONT_PIXEL_T;
#endif

EFONT_HANDLE EbookFont_Init(int font_id);
EBOOL EbookFont_Uninit(EFONT_HANDLE hFont);
int EbookFont_GetLineHeight(EFONT_HANDLE hFont);
int EbookFont_GetCharWidth(EFONT_HANDLE hFont, ECHAR ch);
int EbookFont_GetStrSize(EFONT_HANDLE hFont, const ECHAR *str, EU16 *width, EU16 *heigth, int hgap);
EBOOL EbookFont_GetCharPixels(EFONT_HANDLE hFont, ECHAR ch, EBOOK_FONT_PIXEL_T *pixel_info);

#endif // __EBOOK_FONT__

