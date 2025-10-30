#include "max.h"
#include "maxlib.h"
#include "../../bwlib/maxlib/res/ascii.h"
#include "../../sUI/winManage.h"
#include "../../ax32_platform_demo/application.h"
#include "../../multimedia/unifont/ebook.h"

#define EBOOK_CHAR_GAP 1



void drawNotIntersect(uiRect* father,uiRect* child,uiColor color);
extern unsigned int R_getAsciiStringSize(char *str,unsigned short *width,unsigned short *heigth,unsigned char font);
uint32 resGetImageAddrAndSize(resID id,uint16 *width,uint16 *height)
{
	return (uint32)R_getDataAndSize(id,width,height);
}


#define  ICON_BUFF_NUM         1//32
#define  ICON_BUFF_SIZE        (20*1024)
#define  PRELOAD_ICON(ResID)  {ResID,0,0,0,0,0,NULL}
typedef struct _iconBuff
{
	resID id;
	uint16 width;
	uint16 height;
	uint32 addr;
	uint32 count;
	uint32 alloc_size;
	uiColor* buff;
}iconBuff;
static iconBuff icon[ICON_BUFF_NUM];
/*
Since the preloaded image uses SDRAM cache, please do not preload 
too many images to avoid affecting the smoothness of the video
Large image use this feature, do not use this feature for small image
*/
static iconBuff preloadIcon[]=
{
#if 0
	PRELOAD_ICON(R_ID_ICON_NUM1),
	PRELOAD_ICON(R_ID_ICON_NUM2),
	PRELOAD_ICON(R_ID_ICON_NUM3),
	PRELOAD_ICON(R_ID_ICON_NUM4),
	PRELOAD_ICON(R_ID_ICON_NUM5),
	PRELOAD_ICON(R_ID_ICON_NUM6),
	PRELOAD_ICON(R_ID_ICON_NUM7),
	PRELOAD_ICON(R_ID_ICON_NUM8),
	PRELOAD_ICON(R_ID_ICON_NUM9),
	PRELOAD_ICON(R_ID_ICON_NUM10),
#else
	PRELOAD_ICON(R_ID_ICON_MTPHOTO),
#endif
};
void preloadIconInit(void)
{
	static bool loadIcon=false;
	uint16 width,height,i;
	uint32 len;
	uint32 addr;
	if(loadIcon==true)
		return;
	loadIcon=true;
	for(i=0;i<sizeof(preloadIcon)/sizeof(preloadIcon[0]);i++)
	{
		addr=resGetImageAddrAndSize(preloadIcon[i].id,&width,&height);
		if(addr==0)
		{
			debug_wrn("get icon [%x] infor failed\n",preloadIcon[i].id);
			preloadIcon[i].id=INVALID_RES_ID;
			continue;
		}
		len=width*height;
		preloadIcon[i].buff=hal_sysMemMalloc(len,32);
		if(preloadIcon[i].buff==NULL)
		{
			debug_wrn("malloc icon [%x] buff failed\n",preloadIcon[i].id);
			preloadIcon[i].id=INVALID_RES_ID;
			continue;
		}
		if(nv_read(addr,(void *)preloadIcon[i].buff,len)<0)
		{
			debug_wrn("read icon [%x] data failed\n",preloadIcon[i].id);
			preloadIcon[i].id=INVALID_RES_ID;
			continue;
		}
		preloadIcon[i].addr=addr;
		preloadIcon[i].width=width;
		preloadIcon[i].height=height;
		preloadIcon[i].count=0;
		preloadIcon[i].alloc_size=len;
	}
}
static u8* lcdBuffStartAddr;
static u8* lcdBuffEndAddr;

void resBuffInit(void)
{
	uint32 i;
	lcdBuffStartAddr=getDrawBuff();
	lcdBuffEndAddr=lcdBuffStartAddr+LCD_WIDTH*LCD_HEIGHT;
	for(i=0;i<ICON_BUFF_NUM;i++)
	{
		icon[i].id=INVALID_RES_ID;
		icon[i].count=0;
		//icon[i].buff=uiHandleToPtr(uiAlloc(ICON_BUFF_SIZE));
		icon[i].buff=uiHandleToPtr(hal_sysMemMalloc(ICON_BUFF_SIZE,32));
		icon[i].alloc_size=ICON_BUFF_SIZE;
	}
	preloadIconInit();
}
void resBuffUninit(void)
{
	uint32 i;
	for(i=0;i<ICON_BUFF_NUM;i++)
	{
		icon[i].id=INVALID_RES_ID;
		icon[i].count=0;
		//uiFree(uiPtrToHandle(icon[i].buff));
		if (icon[i].buff != NULL) {
			hal_sysMemFree(uiPtrToHandle(icon[i].buff));
			icon[i].buff=NULL;
		}
	}
}

void resBuffConut(void)
{
	uint32 i;
	for(i=0;i<ICON_BUFF_NUM;i++)
		icon[i].count++;
}
void resUpdateIconBuff(resID id,iconBuff* icon)
{
	uint16 width,height;
	uint32 len, alloc_size;
	uint32 addr;
	bool big_size = false;
	addr=resGetImageAddrAndSize(id,&width,&height);
	if(addr==0)
		return;
	len=width*height;
	// realloc if more large or more smaller
	if (icon->alloc_size < len || icon->alloc_size > ICON_BUFF_SIZE) {
		if (icon->buff) {
			hal_sysMemFree(icon->buff);
			icon->buff = NULL;
		}
	}
	// realloc perhaps
	// some icon is 200x120, larger than ICON_BUFF_SIZE
	if (icon->buff == NULL) {
		if (len > ICON_BUFF_SIZE) {
			alloc_size = len;
			big_size = true;
		} else {
			alloc_size =  ICON_BUFF_SIZE;
		}
		icon->buff=hal_sysMemMalloc(alloc_size,32);
		if (icon->buff == NULL) {
			debug_wrn("alloc fail for icon [%x]\n",id);
			return;
		}
		icon->alloc_size = alloc_size;
	}
//	if(len>ICON_BUFF_SIZE)
//		return;
	if(nv_read(addr,(void *)icon->buff,len)<0)
		return;
	icon->addr=addr;
	icon->width=width;
	icon->height=height;
	icon->id=id;
	if (big_size) {
		icon->count=100000;
	} else {
		icon->count=0;
	}
}
uiColor* resGetImageFromIconBuff(resID id,uint32 addr,uint32 width)
{
	uint32 i,num=0;
	iconBuff* buff=NULL;
	uint32 count=0;
	for(i=0;i<ICON_BUFF_NUM;i++)
	{
		if(icon[i].id==id)
			break;
		if(icon[i].id==INVALID_RES_ID)
			buff=&icon[i];
		if(count<icon[i].count)
		{
			count=icon[i].count;
			num=i;
		}
	}
	if(i<ICON_BUFF_NUM)
	{
		if(icon[i].count>2)
			icon[i].count-=2;
		else
			icon[i].count=0;
		//debug_msg("read icon [%d] data from buff\n",id);
		return icon[i].buff+(addr-icon[i].addr);
	}
	if(buff==NULL)
		buff=&icon[num];
	resUpdateIconBuff(id,buff);
	if(buff->id!=id)
	{
		debug_wrn("load icon [%x] failed\n",id);
		return NULL;
	}
	return buff->buff+(addr-buff->addr);
}
uiColor* resGetImageData(resID id,uint32 addr,uint32 width)
{
	uint16 i;
	uiColor* resAddr;
	for(i=0;i<sizeof(preloadIcon)/sizeof(preloadIcon[0]);i++)
	{
		if(preloadIcon[i].id==id)
		{
			if(preloadIcon[i].buff==NULL)
				break;
			return preloadIcon[i].buff+(addr-preloadIcon[i].addr);
		}
	}
	resAddr=resGetImageFromIconBuff(id,addr,width);
	if(resAddr)
		return resAddr;
	return user_get_icon_line((u32_t)addr,width);
}
uint32 resGetStringInfor(resID id,uint16* width,uint16* height,charFont font)
{
	char* str;
	if(resIdIsStr(id))
	{
		str=resIdToStrAddr(id);
		if(str==NULL)
			return 0;
		if(get_ebook_font() == false)
		{
			return R_getAsciiStringSize(str,width,height,font);
		}
		else
		{
			//return MP3_get_unicode_str_size((u8*)str,width,height,font);
			int num = Ebook_GetStrSize(font,(ECHAR *)str,width,height,0);
			if (width) {
				*width += num * EBOOK_CHAR_GAP;
			}
			return num;
		}
	}
	else if(ID_TYPE(id)==R_ID_TYPE_WAV_NAME)
	{
		str=manangerGetFileName(SysCtrl.wav_list,ID_NUM(id));
		if(str==NULL)
			return 0;
		return R_getAsciiStringSize(str,width,height,font);
	}
	else if(ID_TYPE(id)==R_ID_TYPE_STR)
	{
		return resGetString(id,width,height);
	}
	return 0;
}


static bool g_ebook_font = false;
void set_ebook_font(bool enable)
{
	g_ebook_font = enable;
}
bool get_ebook_font(void)
{
	return g_ebook_font;
}
static uint8* ebookGetCharInfo(uint16 uniChar, uint16* width,uint16* height, charFont font)
{
	static EBOOK_FONT_PIXEL_T fontInfo;
	uint8 *dst;
	int font_h, font_w, stride;

	if (EFALSE == Ebook_GetCharPixels(font, uniChar, &fontInfo)) {
		return NULL;
	}
	font_w = fontInfo.width + EBOOK_CHAR_GAP;
	stride = (font_w + 7) / 8;
	if (width) {
		*width = font_w;
	}
	font_h = Ebook_GetCharHeight(font);
	if (height) {
		*height = font_h;
	}
	if (fontInfo.lines == 0) {
		return fontInfo.pixels;
	}
	// head blank lines
	if (fontInfo.startline > 0) {
		dst = fontInfo.pixels + fontInfo.startline * stride;
		memmove(dst, fontInfo.pixels, fontInfo.stride * fontInfo.lines);
		memset(fontInfo.pixels, 0, fontInfo.startline * stride);
	}
	// middle pixel lines
	if (stride != fontInfo.stride) {
		uint8 *src;
		int i;
		src = fontInfo.pixels + fontInfo.startline * stride;
		src += fontInfo.stride * (fontInfo.lines - 1);
		dst = fontInfo.pixels + (fontInfo.startline + fontInfo.lines - 1) * stride;
		for (i = 0; i < fontInfo.lines; i++) {
			memmove(dst,src,fontInfo.stride);
			dst[fontInfo.stride] = 0;
			dst -= stride;
			src -= fontInfo.stride;
		}
	}
	if (font_h > fontInfo.startline + fontInfo.lines) {
		dst = fontInfo.pixels + (fontInfo.startline + fontInfo.lines) * stride;
		memset(dst, 0, (font_h - fontInfo.startline + fontInfo.lines) * stride);
	}

	return fontInfo.pixels;
}

uint8* resGetCharInfor(resID id,uint8 num,uint16* width,uint16* height,charFont font,uint8* special)
{
	char* str;
	if(width)
		*width=0;
	if(height)
		*height=0;
	if(special)
		*special=0;
	if(resIdIsStr(id))
	{
		str=resIdToStrAddr(id);
		if(str==NULL)
			return 0;
		if(get_ebook_font() == false)
		{
			return (uint8 *)ascii_draw_get(str[num],(unsigned char *)width,(unsigned char *)height,font);
		}
		else
		{
			//return MP3_unicode_draw_get((u8*)str,num,(u16 *)width,(u16 *)height,font);
			
			uint16 *ebook_str = (ECHAR *)str;
			return ebookGetCharInfo(ebook_str[num], width, height, font);
		}
	}
	else if(ID_TYPE(id)==R_ID_TYPE_WAV_NAME)
	{
		str=manangerGetFileName(SysCtrl.wav_list,ID_NUM(id));
		if(str==NULL)
			return 0;
		return (uint8 *)ascii_draw_get(str[num],(unsigned char *)width,(unsigned char *)height,font);
	}
	else if(ID_TYPE(id)==R_ID_TYPE_STR)
	{
		return resGetChar(id, num, width, height,special);
	}
	return 0;
}
uint16 calPhoneticPos(uint8* srcAddr,uint16 charW,uint16 charH)
{
	uint8 findBlack=0;
	uint16 i,j;
	uint8* curAddr;
	charW=(charW+7)>>3;
	for(i=charH-3;i>0;i--)
	{
		curAddr=srcAddr+i*charW;
		for(j=0;j<charW;j++)
		{
			findBlack = *curAddr++;
			if(findBlack)
				break;
		}
		if(findBlack)
			break;
	}
	if(i>4)
		i=i-4;
	return i;
}

#if(OSD_SOFTWARE_ROTATE==0)
void dcacheFlush(uiRect* rect)
{
	/*uint8* drawBuff;
	uint16 y,width,hight;
	drawBuff=getDrawBuff();
	width=rect->x1-rect->x0+1;
	hight=rect->y1-rect->y0+1;
	drawBuff+=rect->y0*LCD_WIDTH+rect->x0;

	for(y=0;y<hight;y++)
	{
		ax32xx_sysDcacheWback(drawBuff,width);
		drawBuff+=LCD_WIDTH;
	}*/
}

void drawLine(uint16 sx,uint16 sy,uint16 ex,uint16 ey,uint16 width,uint8 fill_color)
{
	u16_t i,j;
	uint8* buffStart;
	uint8* buffEnd;
	color_t *dest;
	color_t *dest_start;      //start point of each block
	u16_t block_width,block_height;  //the size of block which is needed to move
	s32_t hdeta,wdeta,intg,cnt;
	uiRect rect;

	if(sx>=LCD_WIDTH)
		sx = LCD_WIDTH-1;
	if(sy>=LCD_HEIGHT)
		sy = LCD_HEIGHT-1;
	if(ex>=LCD_WIDTH)
		ex = LCD_WIDTH-1;
	if(ey>=LCD_HEIGHT)
		ey = LCD_HEIGHT-1;

	block_width = width;
	block_height= ey-sy;
	hdeta = 0;
	wdeta = 0;
	intg = 0;
	if(block_height==0)
	{
		block_height = width;
		block_width = ex-sx;
	}
    else
    {
		cnt = intg = ex-sx;
		if(intg<0)
			cnt = 0-cnt;
		if(cnt)
		{
			wdeta =  cnt/block_height;
			if(wdeta==0)
				hdeta = (block_height*10)/cnt;
			else
				wdeta =  (cnt*10)/block_height;
		}
    }
	
	buffStart=dest_start = (color_t *)getDrawBuff();
	buffEnd=buffStart+LCD_WIDTH*LCD_HEIGHT;
	dest_start=dest_start+sy*LCD_WIDTH+sx;

    cnt = 0;
	for(i=0;i<block_height;i++)
	{
		dest = dest_start;
		for(j=0;j<block_width;j++)
		{
			if(dest>=buffStart&&dest<buffEnd)
				*dest = (color_t)fill_color;
			else
				debug_err("memeory operation out of bounds!!!!!!!!!!!!!!!!!!\n\n\n");
			dest++;
		}
		dest_start+=LCD_WIDTH;
		if(hdeta || wdeta)
		{
			cnt+=10;
			if(hdeta)
			{
				if(intg<0)
				{
					dest_start-=cnt/hdeta;
				}
				else
				{
					dest_start+=cnt/hdeta;
				}
				if(cnt>=hdeta)
					cnt -=hdeta;
			}
			else
			{
				if(intg<0)
				{
					dest_start-=cnt/wdeta;
				}
				else
				{
					dest_start+=cnt/wdeta;
				}
				if(cnt>=wdeta)
					cnt -=wdeta;
			}
		}
	}
	if(sx<ex)
	{
		rect.x0=sx;
		rect.x1=ex;
	}
	else if(sx==ex)
	{
		rect.x0=sx;
		rect.x1=sx+width;
	}
	else
	{
		rect.x0=ex;
		rect.x1=sx;
	}
	if(sy<ey)
	{
		rect.y0=sy;
		rect.y1=ey;
	}
	else if(sy==ey)
	{
		rect.y0=sy;
		rect.y1=sy+width;
	}
	else
	{
		rect.y0=ey;
		rect.y1=sy;
	}
	dcacheFlush(&rect);
}
void drawRectangle(uiRect* rect,uiColor color)
{
	uint16 x,y,width,hight;
	uint8* drawBuff;
	drawBuff=getDrawBuff();
	width=rect->x1-rect->x0+1;
	hight=rect->y1-rect->y0+1;
	drawBuff+=rect->y0*LCD_WIDTH+rect->x0;
	
	for(y=0;y<hight;y++)
	{
		for(x=0;x<width;x++)
			drawBuff[x]=color;
		//ax32xx_sysDcacheWback(drawBuff,width);
		drawBuff+=LCD_WIDTH;
	}
	//dcacheFlush(rect);
}

void drawImage(uiRect* winRect,uiRect* drawRect,resID id,uint8 alignment,uiColor bgColor)
{
	uint16 iconWidth=0,iconHeight=0;
	uint32 resAddr=0;
	uint8* drawBuff;
	uint8* destAddr;
	uiColor* resData;
	uint16 i,j,width,height;
	uiRect resRect;
	int16 resX,resY;
	uint32 transparentColor;
	
	resAddr=resGetImageAddrAndSize(id,&iconWidth,&iconHeight);
	if(resAddr==0)
		return;
	if(alignment&ALIGNMENT_LEFT)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+iconWidth-1;
	}
	else if(alignment&ALIGNMENT_RIGHT)
	{
		resRect.x1=winRect->x1;
		resX=resRect.x0=winRect->x1-iconWidth+1;
	}
	else
	{
		resX=resRect.x0=(winRect->x0+winRect->x1-iconWidth)>>1;
		resRect.x1=resRect.x0+iconWidth-1;
	}
	resY=resRect.y0=(winRect->y0+winRect->y1-iconHeight)>>1;
	resRect.y1=resRect.y0+iconHeight-1;
	
	if(winOverlapping(&resRect,drawRect)==false)
	{
		if(bgColor!=INVALID_COLOR)
			drawRectangle(drawRect,bgColor);
		return;
	}
	if(bgColor!=INVALID_COLOR)
		drawNotIntersect(drawRect,&resRect,bgColor);
	winIntersection(drawRect,drawRect,&resRect);
	resRect.x0=drawRect->x0-resX;
	resRect.y0=drawRect->y0-resY;
	resRect.x1=drawRect->x1-resX;
	resRect.y1=drawRect->y1-resY;

	drawBuff=getDrawBuff();
	destAddr=drawBuff+drawRect->y0*LCD_WIDTH+drawRect->x0;

	width=resRect.x1-resRect.x0+1;
	height=resRect.y1-resRect.y0+1;
	resAddr+=iconWidth*resRect.y0+resRect.x0;
	transparentColor=R_getTColor(id);
	for(i=0;i<height;i++)
	{	
   		resData = resGetImageData(id,resAddr,width);
		resAddr+=iconWidth;
    	if(resData==NULL)
			break;
		drawBuff=destAddr;
		for(j=0;j<width;j++)
		{
			if(*resData!=transparentColor)
				*destAddr = *resData;
			else
			{
				if(bgColor!=INVALID_COLOR)
					*destAddr = bgColor;
			}
			destAddr++;
		resData++;
		}
		destAddr=drawBuff;
		destAddr+=LCD_WIDTH;
	}
	dcacheFlush(drawRect);
}
void drawChar(uint8* destAddr,uint8* srcAddr,uiRect* srcRect,uint16 charW,uint16 charH,uiColor fontColor,uiColor bgColor,uiColor rimColor)
{
	uint8 temp,mask,lastDraw=2;
	uint16 i,j,drawLine;
	uint8* drawAddr,*upPointAddr,*charAddr,*temAddr;
	charAddr=srcAddr;
	charW=(charW+7)>>3;
	srcAddr+=srcRect->y0*charW;
	charH=srcRect->y1-srcRect->y0+1;
	for(i=0;i<charH;i++)
	{
		drawLine=0;
		drawAddr=destAddr;
		for(j=0;j<charW;j++)
		{
			if(srcAddr-charW>=charAddr)
				upPointAddr=srcAddr-charW;
			else
				upPointAddr=NULL;
			temp = *srcAddr++;
			for(mask=0;mask<8;mask++)
			{
				if(drawLine>=srcRect->x0&&drawLine<=srcRect->x1)
				{
					if(temp&(0x80>>mask))
					{
						if(rimColor!=INVALID_COLOR)
						{
							if(lastDraw==0)
							{
								temAddr=destAddr-1;
								if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
									*temAddr=rimColor;
							}
							if(upPointAddr)
							{
								if((*upPointAddr&(0x80>>mask))==0)
								{
									temAddr=destAddr-LCD_WIDTH;
									if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
										*temAddr=rimColor;
								}
							}
						}
					 	*destAddr = fontColor;
						lastDraw=1;
					}
					else
					{
						if(rimColor!=INVALID_COLOR)
						{
							if(lastDraw==1)
								*destAddr = rimColor;
							else if(upPointAddr&&(*upPointAddr&(0x80>>mask)))
								*destAddr = rimColor;
							else
							{
								if(bgColor!=INVALID_COLOR)
									*destAddr = bgColor;
							}
						}
						else
						{
							if(bgColor!=INVALID_COLOR)
								*destAddr = bgColor;
						}
						lastDraw=0;
					}
					destAddr++;
				}
				drawLine++;
			}
		}
		destAddr=drawAddr;
		destAddr+=LCD_WIDTH;
	}
}

void drawString(uiRect* winRect,uiRect* invalidRect,resID id,charFont font,uint8 alignment,uiColor fontColor,uiColor bgColor,uiColor rimColor)
{
	uint16 i,resWidth,resHeight,width,strH;
	int16 resX,resY;
	uint8* drawBuff;
	uint8* destAddr;
	uint8* lastAddr;
	uint8* secSpecialAddr;
	uint8* lastSecSpecialAddr;
	uint8* lowPhoneticAddr;
	uint8* lastLowPhoneticAddr;
	uint8* charBuff;
	uint32 strNum;
	uiRect resRect,charRect,drawRect;
	uint8 special,lastSpecial=0;
	
	strNum=resGetStringInfor(id,&resWidth,&resHeight,font);
	if(strNum==0)
	{
		debug_err("find string error!!!,0x%x\n",id);
		return;
	}
	strH=resHeight;
	drawRect.x0=invalidRect->x0;
	drawRect.y0=invalidRect->y0;
	drawRect.x1=invalidRect->x1;
	drawRect.y1=invalidRect->y1;
	if(alignment&ALIGNMENT_LEFT)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+resWidth-1;
	}
	else if(alignment&ALIGNMENT_RIGHT)
	{
		resRect.x1=winRect->x1;
		resX=resRect.x0=winRect->x1-resWidth+1;
	}
	else
	{
		resX=resRect.x0=(winRect->x0+winRect->x1-resWidth)>>1;
		resRect.x1=resRect.x0+resWidth-1;
	}
	
	if(resRect.x0<winRect->x0)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+resWidth-1;
	}
	resY=resRect.y0=(winRect->y0+winRect->y1-resHeight)>>1;
	if(!resIdIsStr(id))
		resY=resRect.y0=resRect.y0+3;
	resRect.y1=resRect.y0+resHeight-1;
	if(winOverlapping(&resRect,&drawRect)==false)
	{
		if(bgColor!=INVALID_COLOR)
			drawRectangle(&drawRect,bgColor);
		return;
	}
	if(bgColor!=INVALID_COLOR)
		drawNotIntersect(&drawRect,&resRect,bgColor);
	winIntersection(&drawRect,&drawRect,&resRect);
	resRect.x0=drawRect.x0-resX;
	resRect.y0=drawRect.y0-resY;
	resRect.x1=drawRect.x1-resX;
	resRect.y1=drawRect.y1-resY;
	drawBuff=getDrawBuff();
	destAddr=drawBuff+drawRect.y0*LCD_WIDTH+drawRect.x0;
	secSpecialAddr=NULL;
	if(resY>winRect->y0)
	{
		if(resY-winRect->y0>5)
			secSpecialAddr=drawBuff+(resY-5)*LCD_WIDTH+drawRect.x0;
		else
			secSpecialAddr=drawBuff+winRect->y0*LCD_WIDTH+drawRect.x0;
	}
	lastSecSpecialAddr=secSpecialAddr;
	lowPhoneticAddr=NULL;
	if(resY+(int16)resHeight-7<=winRect->y1)
	{
		if(resY+(int16)resHeight>=11)
			lowPhoneticAddr=drawBuff+(resY+(int16)resHeight-11)*LCD_WIDTH+drawRect.x0;
	}
	lastLowPhoneticAddr=lowPhoneticAddr;
	width=0;
	lastAddr=destAddr;
	for(i=0;i<strNum;i++)
	{
		charBuff=resGetCharInfor(id, i, &resWidth, &resHeight,font,&special);
		if(charBuff==NULL)
			break;
		if(special==0)
			width+=resWidth;
		if(width>resRect.x0)
		{
			if(special)
			{
				if(lastSpecial==special)
				{
					charRect.y0=0;
					charRect.y1=resHeight-1;
				}
				else if(special==2)
				{
					charRect.y0=calPhoneticPos(charBuff,resWidth,resHeight);//resHeight*4;
					charRect.y1=charRect.y0+4;
				}
				else
				{
					if(resRect.y0>=resHeight)
						continue;
					charRect.y0=resRect.y0;
					charRect.y1=resHeight;
				}
			}
			else
			{
				if(width-resRect.x0<resWidth)
					charRect.x0=resWidth-(width-resRect.x0);
				else
					charRect.x0=0;
				if(width>resRect.x1+1)
					charRect.x1=(resWidth-(width-(resRect.x1+1))-charRect.x0)-1;
				else
					charRect.x1=resWidth-1;
					//charRect.x1=resWidth-charRect.x0-1;
				if(strH>resHeight)
				{
					charRect.y0=resRect.y0-(int16)((strH-resHeight)>>1);
					charRect.y1=resRect.y1-(int16)((strH-resHeight)>>1);
					if(charRect.y0>=resHeight||charRect.y1<0)
					{
						charRect.y0=-1;
						charRect.y1=-1;
					}
					else
					{
						if(charRect.y0<0)
							charRect.y0=0;
						if(charRect.y1>=resHeight)
							charRect.y1=resHeight-1;
					}
				}
				else
				{
					charRect.y0=resRect.y0;
					charRect.y1=resRect.y1;
				}
			}
			if(special)
			{
				if(lastSpecial==special)
				{
					if(lastSecSpecialAddr)
						drawChar(lastSecSpecialAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
				}
				else
				{
					if(special==1)
						drawChar(lastAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
					else if(special==2&&lastLowPhoneticAddr)
						drawChar(lastLowPhoneticAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
				}
			}
			else
			{
				if(strH>resHeight)
				{
					if(charRect.y0>=0)
						drawChar(destAddr+((strH-resHeight)>>1)*LCD_WIDTH,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
				}
				else
					drawChar(destAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
			}
			lastSpecial=special;
			if(special==0)
			{
				lastAddr=destAddr;
				destAddr+=(charRect.x1-charRect.x0+1);
				if(secSpecialAddr)
				{
					lastSecSpecialAddr=secSpecialAddr;
					secSpecialAddr+=(charRect.x1-charRect.x0+1);
				}
				if(lowPhoneticAddr)
				{
					lastLowPhoneticAddr=lowPhoneticAddr;
					lowPhoneticAddr+=(charRect.x1-charRect.x0+1);
				}
			}
			if(width>resRect.x1)
			{
				if(i<strNum-1)
				{
					resGetCharInfor(id, i+1, NULL, NULL,font,&special);
					if(special)
						continue;
				}
				break;
			}
		}
	}
	dcacheFlush(&drawRect);
}
#else
void dcacheFlush(uiRect* rect)
{
	uint8* drawBuff;
	uint16 y,width,hight;
	int32 lcdRotate=hal_lcdGetShowingRotate();
	drawBuff=getDrawBuff();
	///width=rect->x1-rect->x0+1;
	//hight=rect->y1-rect->y0+1;
	///drawBuff+=rect->y0*LCD_WIDTH+rect->x0;

if(lcdRotate==LCD_ROTATE_0)
{
	width=rect->x1-rect->x0+1;
	hight=rect->y1-rect->y0+1;
	drawBuff+=rect->y0*LCD_WIDTH+rect->x0;
}
else if(lcdRotate==LCD_ROTATE_270)
{
	width=rect->y1-rect->y0+1;
	hight=rect->x1-rect->x0+1;
	drawBuff+=rect->x0*LCD_HEIGHT+(LCD_HEIGHT-1-rect->y1);
}
else if(lcdRotate==LCD_ROTATE_180)
{
	width=rect->x1-rect->x0+1;
	hight=rect->y1-rect->y0+1;
	drawBuff+=(LCD_HEIGHT-1-rect->y1)*LCD_WIDTH+(LCD_WIDTH-1-rect->x1);
}
else if(lcdRotate==LCD_ROTATE_90)
{
	width=rect->y1-rect->y0+1;
	hight=rect->x1-rect->x0+1;
	drawBuff+=(LCD_WIDTH-1-rect->x1)*LCD_HEIGHT+rect->y0;
}
else
	return;
	 
	for(y=0;y<hight;y++)
	{
		//ax32xx_sysDcacheWback(drawBuff,width);
		//drawBuff+=LCD_WIDTH;
	if(lcdRotate==LCD_ROTATE_0)
	{
		ax32xx_sysDcacheWback((u32)drawBuff,width);
		drawBuff+=LCD_WIDTH;
	}
	else if(lcdRotate==LCD_ROTATE_270)
	{
		ax32xx_sysDcacheWback((u32)drawBuff,width);
		drawBuff+=LCD_HEIGHT;
	}
	else if(lcdRotate==LCD_ROTATE_180)
	{
		ax32xx_sysDcacheWback((u32)drawBuff,width);
		drawBuff+=LCD_WIDTH;
	}
	else if(lcdRotate==LCD_ROTATE_90)
	{
		ax32xx_sysDcacheWback((u32)drawBuff,width);
		drawBuff+=LCD_HEIGHT;
	}
	}
}

void drawLine(uint16 sx,uint16 sy,uint16 ex,uint16 ey,uint16 width,uint8 fill_color)
{
	u16_t i,j;
	uint8* buffStart;
	uint8* buffEnd;
	color_t *dest;
	color_t *dest_start;      //start point of each block
	u16_t block_width,block_height;  //the size of block which is needed to move
	s32_t hdeta,wdeta,intg,cnt;
	uiRect rect;
	int32 lcdRotate=hal_lcdGetShowingRotate();

	if(sx>=LCD_WIDTH)
		sx = LCD_WIDTH-1;
	if(sy>=LCD_HEIGHT)
		sy = LCD_HEIGHT-1;
	if(ex>=LCD_WIDTH)
		ex = LCD_WIDTH-1;
	if(ey>=LCD_HEIGHT)
		ey = LCD_HEIGHT-1;

	block_width = width;
	block_height= ey-sy;
	hdeta = 0;
	wdeta = 0;
	intg = 0;
	if(block_height==0)
	{
		block_height = width;
		block_width = ex-sx;
	}
    else
    {
		cnt = intg = ex-sx;
		if(intg<0)
			cnt = 0-cnt;
		if(cnt)
		{
			wdeta =  cnt/block_height;
			if(wdeta==0)
				hdeta = (block_height*10)/cnt;
			else
				wdeta =  (cnt*10)/block_height;
		}
    }
	
	buffStart=dest_start = (color_t *)getDrawBuff();
	buffEnd=buffStart+LCD_WIDTH*LCD_HEIGHT;
	//dest_start=dest_start+sy*LCD_WIDTH+sx;
if(lcdRotate==LCD_ROTATE_0)
		dest_start=dest_start+sy*LCD_WIDTH+sx;
else if(lcdRotate==LCD_ROTATE_270)
		dest_start=dest_start+sx*LCD_HEIGHT+(LCD_HEIGHT-sy-1);
else if(lcdRotate==LCD_ROTATE_180)
		dest_start=dest_start+(LCD_HEIGHT-sy-1)*LCD_WIDTH+(LCD_WIDTH-sx-1);
else if(lcdRotate==LCD_ROTATE_90)
		dest_start=dest_start+(LCD_WIDTH-sx-1)*LCD_HEIGHT+sy;
else
	return;
	 
    cnt = 0;
	for(i=0;i<block_height;i++)
	{
		dest = dest_start;
		for(j=0;j<block_width;j++)
		{
			if(dest>=buffStart&&dest<buffEnd)
				*dest = (color_t)fill_color;
			else
				debug_err("memeory operation out of bounds!!!!!!!!!!!!!!!!!!\n\n\n");
			//dest++;
		if(lcdRotate==LCD_ROTATE_0)
			dest++;
		else if(lcdRotate==LCD_ROTATE_270)
			dest+=LCD_HEIGHT;
		else if(lcdRotate==LCD_ROTATE_180)
			dest--;
		else if(lcdRotate==LCD_ROTATE_90)
			dest-=LCD_HEIGHT;
			 
		}
		//dest_start+=LCD_WIDTH;
		if(lcdRotate==LCD_ROTATE_0)
			dest_start+=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_270)
			dest_start--;
		else if(lcdRotate==LCD_ROTATE_180)
			dest_start-=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_90)
			dest_start++;
			 
		if(hdeta || wdeta)
		{
			cnt+=10;
			if(hdeta)
			{
				if(intg<0)
				{
					//dest_start-=cnt/hdeta;
					
					if(lcdRotate==LCD_ROTATE_0)
						dest_start-=cnt/hdeta;
					else if(lcdRotate==LCD_ROTATE_270)
						dest_start-=((cnt/hdeta)*LCD_HEIGHT);
					else if(lcdRotate==LCD_ROTATE_180)
						dest_start+=cnt/hdeta;
					else if(lcdRotate==LCD_ROTATE_90)
						dest_start+=((cnt/hdeta)*LCD_HEIGHT);
						 
				}
				else
				{
					//dest_start+=cnt/hdeta;
					if(lcdRotate==LCD_ROTATE_0)
						dest_start+=cnt/hdeta;
					else if(lcdRotate==LCD_ROTATE_270)
						dest_start+=((cnt/hdeta)*LCD_HEIGHT);
					else if(lcdRotate==LCD_ROTATE_180)
						dest_start-=cnt/hdeta;
					else if(lcdRotate==LCD_ROTATE_90)
						dest_start-=((cnt/hdeta)*LCD_HEIGHT);
						 
				}
				if(cnt>=hdeta)
					cnt -=hdeta;
			}
			else
			{
				if(intg<0)
				{
					//dest_start-=cnt/wdeta;
					
					if(lcdRotate==LCD_ROTATE_0)
						dest_start-=cnt/wdeta;
					else if(lcdRotate==LCD_ROTATE_270)
						dest_start-=((cnt/wdeta)*LCD_HEIGHT);
					else if(lcdRotate==LCD_ROTATE_180)
						dest_start+=cnt/wdeta;
					else if(lcdRotate==LCD_ROTATE_90)
						dest_start+=((cnt/wdeta)*LCD_HEIGHT);
						 

				}
				else
				{
					//dest_start+=cnt/wdeta;
					
					if(lcdRotate==LCD_ROTATE_0)
						dest_start+=cnt/wdeta;
					else if(lcdRotate==LCD_ROTATE_270)
						dest_start+=((cnt/wdeta)*LCD_HEIGHT);
					else if(lcdRotate==LCD_ROTATE_180)
						dest_start-=cnt/wdeta;
					else if(lcdRotate==LCD_ROTATE_90)
						dest_start-=((cnt/wdeta)*LCD_HEIGHT);
						 

				}
				if(cnt>=wdeta)
					cnt -=wdeta;
			}
		}
	}
	if(sx<ex)
	{
		rect.x0=sx;
		rect.x1=ex;
	}
	else if(sx==ex)
	{
		rect.x0=sx;
		rect.x1=sx+width;
	}
	else
	{
		rect.x0=ex;
		rect.x1=sx;
	}
	if(sy<ey)
	{
		rect.y0=sy;
		rect.y1=ey;
	}
	else if(sy==ey)
	{
		rect.y0=sy;
		rect.y1=sy+width;
	}
	else
	{
		rect.y0=ey;
		rect.y1=sy;
	}
	dcacheFlush(&rect);
}

void drawRectangle(uiRect* rect,uiColor color)
{
	uint16 x,y,width,hight;
	uint8* drawBuff;
	int32 lcdRotate=hal_lcdGetShowingRotate();
	drawBuff=getDrawBuff();
	//width=rect->x1-rect->x0+1;
	//hight=rect->y1-rect->y0+1;
	//drawBuff+=rect->y0*LCD_WIDTH+rect->x0;
	
	if(lcdRotate==LCD_ROTATE_0)
	{
		width=rect->x1-rect->x0+1;
		hight=rect->y1-rect->y0+1;
		drawBuff+=rect->y0*LCD_WIDTH+rect->x0;
	}
	else if(lcdRotate==LCD_ROTATE_270)
	{
		width=rect->y1-rect->y0+1;
		hight=rect->x1-rect->x0+1;
		drawBuff+=rect->x0*LCD_HEIGHT+(LCD_HEIGHT-1-rect->y1);
	}
	else if(lcdRotate==LCD_ROTATE_180)
	{
		width=rect->x1-rect->x0+1;
		hight=rect->y1-rect->y0+1;
		drawBuff+=(LCD_HEIGHT-1-rect->y1)*LCD_WIDTH+(LCD_WIDTH-1-rect->x1);
	}
	else if(lcdRotate==LCD_ROTATE_90)
	{
		width=rect->y1-rect->y0+1;
		hight=rect->x1-rect->x0+1;
		drawBuff+=(LCD_WIDTH-1-rect->x1)*LCD_HEIGHT+rect->y0;
	}
	else
		return;
		 
	for(y=0;y<hight;y++)
	{
		for(x=0;x<width;x++)
			drawBuff[x]=color;
		ax32xx_sysDcacheWback((u32)drawBuff,width);
		//drawBuff+=LCD_WIDTH;
		
		if(lcdRotate==LCD_ROTATE_0)
			drawBuff+=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_270)
			drawBuff+=LCD_HEIGHT;
		else if(lcdRotate==LCD_ROTATE_180)
			drawBuff+=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_90)
			drawBuff+=LCD_HEIGHT;
			 
	}
	//dcacheFlush(rect);
}

void drawImage(uiRect* winRect,uiRect* drawRect,resID id,uint8 alignment,uiColor bgColor)
{
	uint16 iconWidth=0,iconHeight=0;
	uint32 resAddr=0;
	uint8* drawBuff;
	uint8* destAddr;
	uiColor* resData;
	uint16 i,j,width,height;
	uiRect resRect;
	int16 resX,resY;
	uint32 transparentColor;
	int32 lcdRotate=hal_lcdGetShowingRotate();
	
	resAddr=resGetImageAddrAndSize(id,&iconWidth,&iconHeight);
	if(resAddr==0)
		return;
	if(alignment&ALIGNMENT_LEFT)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+iconWidth-1;
	}
	else if(alignment&ALIGNMENT_RIGHT)
	{
		resRect.x1=winRect->x1;
		resX=resRect.x0=winRect->x1-iconWidth+1;
	}
	else
	{
		resX=resRect.x0=(winRect->x0+winRect->x1+1-iconWidth)>>1;
		resRect.x1=resRect.x0+iconWidth-1;
	}
	resY=resRect.y0=(winRect->y0+winRect->y1+1-iconHeight)>>1;
	resRect.y1=resRect.y0+iconHeight-1;
	
	if(winOverlapping(&resRect,drawRect)==false)
	{
		if(bgColor!=INVALID_COLOR)
			drawRectangle(drawRect,bgColor);
		return;
	}
	if(bgColor!=INVALID_COLOR)
		drawNotIntersect(drawRect,&resRect,bgColor);
	winIntersection(drawRect,drawRect,&resRect);
	resRect.x0=drawRect->x0-resX;
	resRect.y0=drawRect->y0-resY;
	resRect.x1=drawRect->x1-resX;
	resRect.y1=drawRect->y1-resY;

	drawBuff=getDrawBuff();
	//destAddr=drawBuff+drawRect->y0*LCD_WIDTH+drawRect->x0;
	
	if(lcdRotate==LCD_ROTATE_0)
		destAddr=drawBuff+drawRect->y0*LCD_WIDTH+drawRect->x0;
	else if(lcdRotate==LCD_ROTATE_270)
		destAddr=drawBuff+drawRect->x0*LCD_HEIGHT+(LCD_HEIGHT-drawRect->y0-1);
	else if(lcdRotate==LCD_ROTATE_180)
		destAddr=drawBuff+(LCD_HEIGHT-drawRect->y0-1)*LCD_WIDTH+(LCD_WIDTH-drawRect->x0-1);
	else if(lcdRotate==LCD_ROTATE_90)
		destAddr=drawBuff+(LCD_WIDTH-drawRect->x0-1)*LCD_HEIGHT+drawRect->y0;
	else
		return;
		 

	width=resRect.x1-resRect.x0+1;
	height=resRect.y1-resRect.y0+1;
	resAddr+=iconWidth*resRect.y0+resRect.x0;
	transparentColor=R_getTColor(id);
	for(i=0;i<height;i++)
	{	
   		resData = resGetImageData(id,resAddr,width);
		resAddr+=iconWidth;
    	if(resData==NULL)
			break;
		drawBuff=destAddr;
		for(j=0;j<width;j++)
		{
			if(*resData!=transparentColor)
				*destAddr = *resData;
			else
			{
				if(bgColor!=INVALID_COLOR)
					*destAddr = bgColor;
			}
			//destAddr++;
			if(lcdRotate==LCD_ROTATE_0)
				destAddr++;
			else if(lcdRotate==LCD_ROTATE_270)
				destAddr+=LCD_HEIGHT;
			else if(lcdRotate==LCD_ROTATE_180)
				destAddr--;
			else if(lcdRotate==LCD_ROTATE_90)
				destAddr-=LCD_HEIGHT;
				 
		resData++;
		}
		destAddr=drawBuff;
		//destAddr+=LCD_WIDTH;
		if(lcdRotate==LCD_ROTATE_0)
			destAddr+=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_270)
			destAddr--;
		else if(lcdRotate==LCD_ROTATE_180)
			destAddr-=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_90)
			destAddr++;
			 
	}
	dcacheFlush(drawRect);
}
void drawChar(uint8* destAddr,uint8* srcAddr,uiRect* srcRect,uint16 charW,uint16 charH,uiColor fontColor,uiColor bgColor,uiColor rimColor)
{
	uint8 temp,mask,lastDraw=2;
	uint16 i,j,drawLine;
	uint8 *drawAddr,*upPointAddr,*charAddr,*temAddr;
	int32 lcdRotate=hal_lcdGetShowingRotate();
	charAddr=srcAddr;
	charW=(charW+7)>>3;
	srcAddr+=srcRect->y0*charW;
	charH=srcRect->y1-srcRect->y0+1;
	for(i=0;i<charH;i++)
	{
		drawLine=0;
		drawAddr=destAddr;
		for(j=0;j<charW;j++)
		{
			if(srcAddr-charW>=charAddr)
				upPointAddr=srcAddr-charW;
			else
				upPointAddr=NULL;
			temp = *srcAddr++;
			for(mask=0;mask<8;mask++)
			{
				if(drawLine>=srcRect->x0&&drawLine<=srcRect->x1)
				{
					if(temp&(0x80>>mask))
					{
						if(rimColor!=INVALID_COLOR)
						{
							if(lastDraw==0)
							{
								if(lcdRotate==LCD_ROTATE_0)
								{
									temAddr=destAddr-1;
									if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
										*temAddr=rimColor;
								}
								else if(lcdRotate==LCD_ROTATE_270)
								{
									temAddr=destAddr-LCD_HEIGHT;
									if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
										*temAddr=rimColor;
								}
								else if(lcdRotate==LCD_ROTATE_180)
								{
									temAddr=destAddr+1;
									if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
										*temAddr=rimColor;
								}
								else if(lcdRotate==LCD_ROTATE_90)
								{
									temAddr=destAddr+LCD_HEIGHT;
									if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
										*temAddr=rimColor;
								}
							}
							if(upPointAddr)
							{
								if((*upPointAddr&(0x80>>mask))==0)
								{
									if(lcdRotate==LCD_ROTATE_0)
									{
										temAddr=destAddr-LCD_WIDTH;
										if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
											*temAddr=rimColor;
									}
									else if(lcdRotate==LCD_ROTATE_270)
									{
										temAddr=destAddr-1;
										if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
											*temAddr=rimColor;
									}
									else if(lcdRotate==LCD_ROTATE_180)
									{
										temAddr=destAddr+LCD_WIDTH;
										if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
											*temAddr=rimColor;
									}
									else if(lcdRotate==LCD_ROTATE_90)
									{
										temAddr=destAddr+1;
										if(temAddr>=lcdBuffStartAddr&&temAddr<lcdBuffEndAddr)
											*temAddr=rimColor;
									}
								}
							}
						}
					 	*destAddr = fontColor;
						lastDraw=1;
					}
					else
					{
						if(rimColor!=INVALID_COLOR)
						{
							if(lastDraw==1)
								*destAddr =rimColor;
							else if(upPointAddr&&(*upPointAddr&(0x80>>mask)))
								*destAddr =rimColor;
							else
							{
								if(bgColor!=INVALID_COLOR)
									*destAddr = bgColor;
							}
						}
						else
						{
							if(bgColor!=INVALID_COLOR)
								*destAddr = bgColor;
						}
						lastDraw=0;
					}
					//destAddr++;
					
					if(lcdRotate==LCD_ROTATE_0)
						destAddr++;
					else if(lcdRotate==LCD_ROTATE_270)
						destAddr+=LCD_HEIGHT;
					else if(lcdRotate==LCD_ROTATE_180)
						destAddr--;
					else if(lcdRotate==LCD_ROTATE_90)
						destAddr-=LCD_HEIGHT;
						 
				}
				drawLine++;
			}
		}
		destAddr=drawAddr;
		//destAddr+=LCD_WIDTH;
		
		if(lcdRotate==LCD_ROTATE_0)
			destAddr+=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_270)
			destAddr--;
		else if(lcdRotate==LCD_ROTATE_180)
			destAddr-=LCD_WIDTH;
		else if(lcdRotate==LCD_ROTATE_90)
			destAddr++;
			 
	}
}

void drawString(uiRect* winRect,uiRect* invalidRect,resID id,charFont font,uint8 alignment,uiColor fontColor,uiColor bgColor,uiColor rimColor)
{
	uint16 i,resWidth,resHeight,width,strH;
	int16 resX,resY;
	uint8* drawBuff;
	uint8* destAddr;
	uint8* lastAddr;
	uint8* charBuff;
	uint32 strNum;
	uiRect resRect,charRect,drawRect;
	uint8 special;
	int32 lcdRotate=hal_lcdGetShowingRotate();
	
	strNum=resGetStringInfor(id,&resWidth,&resHeight,font);
	if(strNum==0)
	{
		debug_err("find string error!!!,0x%x\n",id);
		return;
	}
	strH=resHeight;
	drawRect.x0=invalidRect->x0;
	drawRect.y0=invalidRect->y0;
	drawRect.x1=invalidRect->x1;
	drawRect.y1=invalidRect->y1;
	if(alignment&ALIGNMENT_LEFT)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+resWidth-1;
	}
	else if(alignment&ALIGNMENT_RIGHT)
	{
		resRect.x1=winRect->x1;
		resX=resRect.x0=winRect->x1-resWidth+1;
	}
	else
	{
		resX=resRect.x0=(winRect->x0+winRect->x1-resWidth)>>1;
		resRect.x1=resRect.x0+resWidth-1;
	}
	if(resRect.x0<winRect->x0)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+resWidth-1;
	}
	resY=resRect.y0=(winRect->y0+winRect->y1-resHeight)>>1;
	if(!resIdIsStr(id))
		resY=resRect.y0=resRect.y0+3;
	resRect.y1=resRect.y0+resHeight-1;
	if(winOverlapping(&resRect,&drawRect)==false)
	{
		if(bgColor!=INVALID_COLOR)
			drawRectangle(&drawRect,bgColor);
		return;
	}
	if(bgColor!=INVALID_COLOR)
		drawNotIntersect(&drawRect,&resRect,bgColor);
	winIntersection(&drawRect,&drawRect,&resRect);
	resRect.x0=drawRect.x0-resX;
	resRect.y0=drawRect.y0-resY;
	resRect.x1=drawRect.x1-resX;
	resRect.y1=drawRect.y1-resY;
	drawBuff=getDrawBuff();
	//destAddr=drawBuff+drawRect.y0*LCD_WIDTH+drawRect.x0;
	
	if(lcdRotate==LCD_ROTATE_0)
		destAddr=drawBuff+drawRect.y0*LCD_WIDTH+drawRect.x0;
	else if(lcdRotate==LCD_ROTATE_270)
		destAddr=drawBuff+drawRect.x0*LCD_HEIGHT+(LCD_HEIGHT-drawRect.y0-1);
	else if(lcdRotate==LCD_ROTATE_180)
		destAddr=drawBuff+(LCD_HEIGHT-drawRect.y0-1)*LCD_WIDTH+(LCD_WIDTH-drawRect.x0-1);
	else if(lcdRotate==LCD_ROTATE_90)
		destAddr=drawBuff+(LCD_WIDTH-drawRect.x0-1)*LCD_HEIGHT+drawRect.y0;
	else
		return;

		 
	width=0;
	lastAddr=destAddr;
	for(i=0;i<strNum;i++)
	{
		charBuff=resGetCharInfor(id, i, &resWidth, &resHeight,font,&special);
		if(charBuff==NULL)
			break;
		if(special==0)
			width+=resWidth;
		if(width>resRect.x0)
		{
			if(special)
			{
				if(resRect.y0>=resHeight)
					continue;
				charRect.y0=resRect.y0;
				charRect.y1=resHeight;
			}
			else
			{
				if(width-resRect.x0<resWidth)
					charRect.x0=resWidth-(width-resRect.x0);
				else
					charRect.x0=0;
				if(width>resRect.x1+1)
					charRect.x1=(resWidth-(width-(resRect.x1+1))-charRect.x0)-1;
				else
					charRect.x1=resWidth-1;
					//charRect.x1=resWidth-charRect.x0-1;
				if(strH>resHeight)
				{
					charRect.y0=resRect.y0-(int16)((strH-resHeight)>>1);
					charRect.y1=resRect.y1-(int16)((strH-resHeight)>>1);
					if(charRect.y0>=resHeight||charRect.y1<0)
					{
						charRect.y0=-1;
						charRect.y1=-1;
					}
					else
					{
						if(charRect.y0<0)
							charRect.y0=0;
						if(charRect.y1>=resHeight)
							charRect.y1=resHeight-1;
					}
				}
				else
				{
					charRect.y0=resRect.y0;
					charRect.y1=resRect.y1;
				}
			}
			if(special)
				drawChar(lastAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
			else
			{
				if(strH>resHeight)
				{
					if(charRect.y0>=0)
					{
						if(lcdRotate==LCD_ROTATE_0)
							drawChar(destAddr+((strH-resHeight)>>1)*LCD_WIDTH,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
						else if(lcdRotate==LCD_ROTATE_270)
							drawChar(destAddr-((strH-resHeight)>>1),charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
						else if(lcdRotate==LCD_ROTATE_180)
							drawChar(destAddr-((strH-resHeight)>>1)*LCD_WIDTH,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
						else if(lcdRotate==LCD_ROTATE_90)
							drawChar(destAddr+((strH-resHeight)>>1),charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
					}
				}
				else
					drawChar(destAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
			}
			if(special==0)
			{
				//destAddr+=(charRect.x1-charRect.x0+1);
				lastAddr=destAddr;
				if(lcdRotate==LCD_ROTATE_0)
					destAddr+=(charRect.x1-charRect.x0+1);
				else if(lcdRotate==LCD_ROTATE_270)
					destAddr+=(charRect.x1-charRect.x0+1)*LCD_HEIGHT;
				else if(lcdRotate==LCD_ROTATE_180)
					destAddr-=(charRect.x1-charRect.x0+1);
				else if(lcdRotate==LCD_ROTATE_90)
					destAddr-=(charRect.x1-charRect.x0+1)*LCD_HEIGHT;
					 
			}
			if(width>resRect.x1)
			{
				if(i<strNum-1)
				{
					resGetCharInfor(id, i+1, NULL, NULL,font,&special);
					if(special)
						continue;
				}
				break;
			}
		}
	}
	dcacheFlush(&drawRect);
}

#endif
void drawNotIntersect(uiRect* father,uiRect* child,uiColor color)
{
	uiRect invalidRect;
	if(father->x0<child->x0)
	{
		invalidRect.x0=father->x0;
		invalidRect.x1=child->x0-1;
		invalidRect.y0=father->y0;
		invalidRect.y1=father->y1;
		drawRectangle(&invalidRect,color);
	}
	if(father->y0<child->y0)
	{
		invalidRect.x0=MAX(father->x0,child->x0);
		invalidRect.x1=father->x1;
		invalidRect.y0=father->y0;
		invalidRect.y1=child->y0-1;
		drawRectangle(&invalidRect,color);
	}
	if(father->x1>child->x1)
	{
		invalidRect.x0=child->x1+1;
		invalidRect.x1=father->x1;
		invalidRect.y0=MAX(father->y0,child->y0);
		invalidRect.y1=father->y1;
		drawRectangle(&invalidRect,color);
	}
	if(father->y1>child->y1)
	{
		invalidRect.x0=MAX(father->x0,child->x0);
		invalidRect.x1=MIN(father->x1,child->x1);
		invalidRect.y0=child->y1+1;
		invalidRect.y1=father->y1;
		drawRectangle(&invalidRect,color);
	}
}


