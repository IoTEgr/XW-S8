#ifndef  _UNICODE_H
#define  _UNICODE_H


#define MP3_MAX_FONT_NUMS	128
#define MP3_UNICODE_W	32
#define MP3_UNICODE_H 	48



typedef struct _SegIndexInfo
{
	u8 w;
	u8 h;
	u16 length;
	u32 BaseAddr;
}FontIndexInfo;


typedef struct _FontGroup
{
	u32 num;
	u16 fontw;
	u16 fonth;
	FontIndexInfo FontInfo[MP3_MAX_FONT_NUMS];
}FontGroup ;

typedef struct _SegInfo
{
	u32 UnicoValue;
	u32 StartAddr;
}FontSegInfo;

typedef struct _FontLibrary
{
	u32 NumOfUnicode;				// unicode num
	u32 UniAddrOffset;				// unicode addr
}FontLibrary;





//==ret : 0 is ok , other is err==
u8 MP3_unicode_font_init();
u8 MP3_unicode_font_uninit();

void MP3_get_unicode_idx(u16 unicode,FontIndexInfo *pFont);

u32 MP3_get_unicode_str_size(u8 *str,u16 *width,u16 *heigth,u8 font);
u8 *MP3_unicode_draw_get(u8* str,u8 num,u16 *width,u16 *heigth,u8 font);









#endif
