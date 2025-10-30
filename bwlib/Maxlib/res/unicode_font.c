#include "R.h"
#include "application.h"



ALIGNED(32) FontLibrary mp3font;
ALIGNED(32)FontGroup mp3_font_group;
ALIGNED(32) u8 *mp3_unicode_buf=NULL; //[MP3_UNICODE_W*MP3_UNICODE_H];	// w*h
//static u32 mp3_str_num;
u8 *mp3_font_buf=NULL;

//==ret : 0 is ok , other is err==
u8 MP3_unicode_font_init()
{

	u32 addr;
	u32 font_size;
	memset(&mp3font,0,sizeof(FontLibrary));

	addr = nv_open(RES_MP3FONT);
	if(addr<0)
	{
		return 1;
	}

	font_size=nv_size(RES_MP3FONT);

	if((font_size<8)||(font_size>4*1024*1024))
	{
		deg_Printf("mp3 font size err\n");
		return 2;
	}
	
	nv_read(addr,&mp3font,4);
	if(mp3font.NumOfUnicode<1)
	{
		deg_Printf("mp3 font null\n");
	}
	//mp3font.UniAddrOffset=addr+4;	// spi flash addr
	mp3font.UniAddrOffset=4;		// sdram addr
	deg_Printf("mp3font.NumOfUnicode=0x%x\n",mp3font.NumOfUnicode);

	if(NULL==mp3_font_buf)
	{
		mp3_font_buf=hal_sysMemMalloc(font_size,32);
		if(NULL==mp3_font_buf)
		{
			deg_Printf("font mem err\n");
			return 3;
		}
		
		nv_read(addr,mp3_font_buf,font_size);
		ax32xx_sysDcacheFlush((u32)mp3_font_buf,font_size);
	}

	if(NULL==mp3_unicode_buf)
	{
		mp3_unicode_buf=hal_sysMemMalloc(MP3_UNICODE_W*MP3_UNICODE_H,32);
		if(NULL==mp3_unicode_buf)
		{
			deg_Printf("unicode buf mem err\n");
			return 4;
		}
	}
	return 0;

}

u8 MP3_unicode_font_uninit()
{
	if(mp3_font_buf)
	{
		hal_sysMemFree(mp3_font_buf);
		mp3_font_buf=NULL;
	}
	if(mp3_unicode_buf)
	{
		hal_sysMemFree(mp3_unicode_buf);
		mp3_unicode_buf=NULL;
	}
	return 0;
}

void MP3_get_unicode_idx(u16 unicode,FontIndexInfo *pFont)
{
	ALIGNED(32) FontSegInfo FontSegInf;
	u32 AddrOffset;
	AddrOffset = mp3font.UniAddrOffset;

    u16 low, mid, high;
    low = 1;
    high = mp3font.NumOfUnicode+1;
    while (1)
    {
        mid = (low + high) / 2;
        //nv_read(AddrOffset+(mid-1)*sizeof(FontSegInf),&FontSegInf,sizeof(FontSegInf));
        memcpy(&FontSegInf,mp3_font_buf+AddrOffset+(mid-1)*sizeof(FontSegInf),sizeof(FontSegInf));

        if (unicode < FontSegInf.UnicoValue)
            high = mid -1;
        else if(unicode > FontSegInf.UnicoValue)
            low = mid + 1;
        else
            break;

        if(low > high)
		{
            //can't find unicode ,set unicode 0x002a
            //deg_Printf("can't find 0x%x\n",unicode);
        	unicode = 0x002A;
            low = 1;
            high = mp3font.NumOfUnicode+1;
        }
    }

 	//deg_Printf("UnicoValue=0x%x,StartAddr=0x%x\n",FontSegInf.UnicoValue,FontSegInf.StartAddr);
	AddrOffset=mp3font.UniAddrOffset+FontSegInf.StartAddr;
	//nv_read(AddrOffset,pFont,8);
	memcpy(pFont,mp3_font_buf+AddrOffset,8);
	pFont->BaseAddr+=mp3font.UniAddrOffset;
	//deg_Printf("unicode=0x%x,w=%d,h=%d,length=%d,addr=0x%x\n",unicode,pFont->w,pFont->h,pFont->length,pFont->BaseAddr);
}

extern u32 Ascii_to_Unicode(u8*,u16*);
u32 MP3_get_unicode_str_size(u8 *str,u16 *width,u16 *heigth,u8 font)
{
//	u8 string_flag = 0;
	u16 UniStr[MP3_MAX_FONT_NUMS]/*,HandleStr[5]*/;
	int i/*,j,k*/ ;

	memset(&mp3_font_group,0,sizeof(FontGroup));
	mp3_font_group.num = Ascii_to_Unicode(str,UniStr);
 
	for(i=0;i<mp3_font_group.num;i++)
	{
		MP3_get_unicode_idx(UniStr[i],(FontIndexInfo*)(&(mp3_font_group.FontInfo[i])));

		mp3_font_group.fontw += mp3_font_group.FontInfo[i].w;
		if(mp3_font_group.FontInfo[i].h>mp3_font_group.fonth)
		{
			mp3_font_group.fonth = mp3_font_group.FontInfo[i].h;
		}
		//deg_Printf("unicode:0x%x\n",UniStr[i]);
	}

	*width=mp3_font_group.fontw;
	*heigth=mp3_font_group.fonth;
	//deg_Printf("str=0x%x,w=%d,h=%d\n",str,mp3_font_group.fontw,mp3_font_group.fonth);
	//deg_Printf("str unicode num:%d\n",mp3_font_group.num);

	return mp3_font_group.num;
}

u8 *MP3_unicode_draw_get(u8* str,u8 num,u16 *width,u16 *heigth,u8 font)
{
	*width=mp3_font_group.FontInfo[num].w;
	*heigth=mp3_font_group.FontInfo[num].h;

	if(mp3_font_group.FontInfo[num].length<=MP3_UNICODE_W*MP3_UNICODE_H)
	{
		//nv_read(mp3_font_group.FontInfo[num].BaseAddr,mp3_unicode_buf,mp3_font_group.FontInfo[num].length);
		memcpy(mp3_unicode_buf,mp3_font_buf+mp3_font_group.FontInfo[num].BaseAddr,mp3_font_group.FontInfo[num].length);

		//deg_Printf("num=%d,*width=%d,*heigth=%d,len=%d\n",num,*width,*heigth,mp3_font_group.FontInfo[num].length);
		//deg_PrintfBuf(mp3_unicode_buf,mp3_font_group.FontInfo[num].length);
		return mp3_unicode_buf;
	}

	return NULL;
}




