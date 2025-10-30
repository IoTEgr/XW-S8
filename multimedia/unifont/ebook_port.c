#include "../../ax32_platform_demo/application.h"
#include "ebook.h"
#include "ebook_font.h"
//#include "ebook_draw.h"


// ebook_font.c
#define FONT_NUM 1
#define RES_FONT_0 RES_EBOOK_FONT20
//#define RES_FONT_1 RES_EBOOK_FONT16

static EFONT_HANDLE gEFontHandle[FONT_NUM] = {0};

void Ebook_Init(void)
{
	gEFontHandle[0] = EbookFont_Init(RES_FONT_0);
//	gEFontHandle[1] = EbookFont_Init(RES_FONT_1);
}

void Ebook_Uninit(void)
{
	int i;

	for (i = 0; i < sizeof(gEFontHandle)/sizeof(gEFontHandle[0]); i++) {
		EbookFont_Uninit(gEFontHandle[i]);
		gEFontHandle[i] = NULL;
	}
}

int Ebook_GetCharHeight(EFONT_CLASS font)
{
	if (font >= EFONT_MAX) {
		return 0;
	}

	return EbookFont_GetLineHeight(gEFontHandle[font]);
}

int Ebook_GetCharWidth(EFONT_CLASS font, ECHAR ch)
{
	if (font >= EFONT_MAX) {
		return 0;
	}

	return EbookFont_GetCharWidth(gEFontHandle[font], ch);
}

int Ebook_GetStrSize(EFONT_CLASS font, const ECHAR *str, EU16 *width, EU16 *height, int hgap)
{
	if (font >= EFONT_MAX) {
		return 0;
	}

	return EbookFont_GetStrSize(gEFontHandle[font], str, width, height, hgap);
}

EBOOL Ebook_GetCharPixels(EFONT_CLASS font, ECHAR ch, EBOOK_FONT_PIXEL_T *pixel_info)
{
	if (font >= EFONT_MAX) {
		return 0;
	}

	return EbookFont_GetCharPixels(gEFontHandle[font], ch, pixel_info);
}



#if 0
// ebook_draw.c
EBOOL Ebook_DrawStr(EBOOK_LCD_T *lcd, const EBOOK_STR_T *str, const EBOOK_DISP_STYLE_T *style)
{
	if (style == NULL || style->font >= EFONT_MAX) {
		return EFALSE;
	}
	return EbookDraw_Str(gEFontHandle[style->font], lcd, str, style);
}

EBOOL strToUniStr(ECHAR *dst, int dstLen, const char *src)
{
	if (dst == NULL || src == NULL) {
		return EFALSE;
	}

	while (dstLen > 1 && *src) {
		*dst++ = *src++;
		dstLen--;
	}
	*dst = 0;

	return ETRUE;
}
#endif

