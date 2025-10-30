#include "../../ax32_platform_demo/application.h"
#include "userInterface.h"

//UserInterface changeUserInterface = {0};

void actionUpperToBottom(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide;
	u16 lcd_w,lcd_h,video_w,video_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_lcdGetBufferResolution(&video_w,&video_h);
	if(180==video_h)
		lcd_h-=60;


	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide = lcd_h - lcd_h*cnt/ANIMATION_CNT;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = 0; j < lcd_h; j++){
		for(i = 0; i < lcd_w; i++){
			if(j < divide){
				*(dst + j*lcd_w + i) = *(dst + (j + (lcd_h - divide))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(dstUV + ((j + (lcd_h - divide)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else if(j >= divide + 3){
				*(dst + j*lcd_w + i) = *(src + (j - (divide + 3))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j - (divide + 3)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else{
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionBottomToUpper(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide;
	u16 lcd_w,lcd_h,video_w,video_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_lcdGetBufferResolution(&video_w,&video_h);
	if(180==video_h)
		lcd_h-=60;


	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide = lcd_h*cnt/ANIMATION_CNT;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = lcd_h - 1; j < lcd_h; j--){
		for(i = 0; i < lcd_w; i++){
			if(j < divide){
				*(dst + j*lcd_w + i) = *(src + (j + (lcd_h - divide))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j + (lcd_h - divide)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else if(j >= divide + 3){
				*(dst + j*lcd_w + i) = *(dst + (j - (divide + 3))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(dstUV + ((j - (divide + 3)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else{
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionSquareInsideToOutside(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h,video_w,video_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_lcdGetBufferResolution(&video_w,&video_h);
	if(180==video_h)
		lcd_h-=60;

	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = lcd_h*cnt/ANIMATION_CNT >> 1;
	divide_w = lcd_w*cnt/ANIMATION_CNT >> 1;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = lcd_h - 1; j < lcd_h; j--){
		for(i = 0; i < lcd_w; i++){
			if(((j < divide_h - 3) || (j > (lcd_h - divide_h + 3))) || ((i < divide_w - 3) || (i > (lcd_w - divide_w + 3)))){
				*(dst + j*lcd_w + i) = *(src + j*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + (j >> 1)*(lcd_w >> 1) + (i >> 1));
			}else if(((j < divide_h) || (j > (lcd_h - divide_h))) || ((i < divide_w) || (i > (lcd_w - divide_w)))){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionSquareOutsideToInside(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h,video_w,video_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_lcdGetBufferResolution(&video_w,&video_h);
	if(180==video_h)
		lcd_h-=60;

	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = (lcd_h - lcd_h*cnt/ANIMATION_CNT) >> 1;
	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT) >> 1;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = lcd_h - 1; j < lcd_h; j--){
		for(i = 0; i < lcd_w; i++){
			if(((j > divide_h) && (j < (lcd_h - divide_h))) && ((i > divide_w) && (i < (lcd_w - divide_w)))){
				*(dst + j*lcd_w + i) = *(src + j*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + (j >> 1)*(lcd_w >> 1) + (i >> 1));
			}else if(((j > divide_h - 3) && (j < (lcd_h - divide_h + 3))) && ((i > divide_w - 3) && (i < (lcd_w - divide_w + 3)))){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionLeftBottomToRightUpper(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h,video_w,video_h;;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_lcdGetBufferResolution(&video_w,&video_h);
	if(180==video_h)
		lcd_h-=60;
	
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = (lcd_h*cnt/ANIMATION_CNT);
	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT);
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = 0; j < lcd_h; j++){
		for(i = 0; i < lcd_w; i++){
			if((j < divide_h) && (i > divide_w + 3)){
				*(dst + j*lcd_w + i) = *(src + (j + (lcd_h - divide_h))*lcd_w + (i - divide_w));
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j + (lcd_h - divide_h)) >> 1)*(lcd_w >> 1) + ((i - divide_w) >> 1));
			}else if((j < divide_h + 3) && (i > divide_w)){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionLeftToRight(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
	s32 divide;
	u16 lcd_w,lcd_h,video_w,video_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_lcdGetBufferResolution(&video_w,&video_h);
	if(180==video_h)
		lcd_h-=60;

	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide =lcd_w-lcd_w*cnt/ANIMATION_CNT;//lcd_h - lcd_h*cnt/ANIMATION_CNT;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);

	//deg_Printf("------------divide=%d---------------------.\n",divide);
	
	for(j = 0; j < lcd_w; j++)
	{
		for(i = 0; i < lcd_h; i++)
		{
			if(j < divide)
			{
				*(dst + j*lcd_w + i) = *(dst + (j + (lcd_h - divide))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(dstUV + ((j + (lcd_h - divide)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else if(j >= divide + 3)
			{
				*(dst + j*lcd_w + i) = *(src + (j - (divide + 3))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j - (divide + 3)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else
			{
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionLeftUpperToRightBottom(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h,video_w,video_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_lcdGetBufferResolution(&video_w,&video_h);
	if(180==video_h)
		lcd_h-=60;

	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = (lcd_h - lcd_h*cnt/ANIMATION_CNT);
	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT);
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = 0; j < lcd_h; j++){
		for(i = 0; i < lcd_w; i++){
			if((j > divide_h + 3) && (i > divide_w + 3)){
				*(dst + j*lcd_w + i) = *(src + (j - divide_h)*lcd_w + (i - divide_w));
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j - divide_h) >> 1)*(lcd_w >> 1) + ((i - divide_w) >> 1));
			}else if((j > divide_h) && (i > divide_w)){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}


#define TFT_WIDTH		320
#define TFT_HEIGHT		240
#define GET_16BIT_DATA(addr)		(*((INT16U *)addr))

void actionCircleInside2OutSide(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
//  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h,csi_w,csi_h;
//	u16 *dstUV, *srcUV;

	INT16S x0=160, y0=120;
	INT16S x = 0, y = 0;
	INT32U offset;
	INT16U color = 0xc618;
	INT16U r=0;
	INT8U flag=0;
	INT32S d = 3 - 2 * r;

	INT16U jump_process_color=0x9fff;

	
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	hal_csiResolutionGet(&csi_w,&csi_h);
	if(720==csi_h)
		lcd_h-=60;

	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
//	divide_h = (lcd_h - lcd_h*cnt/ANIMATION_CNT);
//	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT);
//	dstUV = (u16 *)(dst + lcd_w*lcd_h);
//	srcUV = (u16 *)(src + lcd_w*lcd_h);

for(j=0;j<8;j++)	
{
	r=j*25;
	
	if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			offset = (y + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			offset = (y0 - y) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			offset = (x + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			offset = (y0 - x) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}

		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT)
		{
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(dst + (y + y0) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (y + y0) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT)
		{
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(dst + (y0 - y) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (y0 - y) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
			
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT)
		{
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(dst + (x + y0) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (x + y0) * TFT_WIDTH + x0 + y) = jump_process_color;
			}	
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT)
		{
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(dst + (y0 - x) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (y0 - x) * TFT_WIDTH + x0 + y) = jump_process_color;
			}
		}
		//#endif
		if(d < 0)
		{
			d += 4* x + 6;
			x++;
		}
		else
		{
			d += 10 + 4 * (x - y);
			x++;
			y--;
		}
		
		if(y0 - r > 0)
		{
			INT32U i = 0;
			for(i = 0; i < y0 - r; i++)
			{
				memcpy((INT32U *)(dst + i * TFT_WIDTH * 2),(INT32U *)(src + i * TFT_WIDTH * 2), TFT_WIDTH << 1);
			}
		}
	if(y0 + r + 1 < TFT_HEIGHT)
	{
		INT32U i = 0;
		for(i = y0 + r + 1; i < TFT_HEIGHT; i++)
		{
			memcpy((INT32U *)(dst + i * TFT_WIDTH * 2),
						(INT32U *)(src + i * TFT_WIDTH * 2), 
						TFT_WIDTH << 1);
		}
	}

	
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);

  }
}

actionType getActionFunc(u8 action)
{
	switch(action){
		case UPPER2BOTTOM:
			return actionUpperToBottom;
		case BOTTOM2UPPER:
			return actionBottomToUpper;
		case SQUARE_INSIDE2OUTSIDE:
			return actionSquareInsideToOutside;
		case SQUARE_OUTSIDE2INSIDE:
			return actionSquareOutsideToInside;
		case LEFTBOTTOM2RIGHTUPPER:
			return actionLeftBottomToRightUpper;
		case LEFTUPPER2RIGHTBOTTOM:
			return actionLeftUpperToRightBottom;
		case LEFT2RIGHT:
			return actionLeftToRight;
		case RIGHT2LEFT:
		case CIRCLE_INSIDE2OUTSIDE:
			return actionCircleInside2OutSide;
		case CIRCLE_OUTSIDE2INSIDE:
			return NULL;
		default:
			return NULL;
	}
}


void animationInit(UserInterface *objName)
{
	disp_frame_t *buf = NULL;
	u16 lcd_w,lcd_h;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	buf = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(buf)
	{
		hal_dispframeVideoCfg(buf,0,0,lcd_w,lcd_h);
	}
	objName->preFrameBuf=hal_sysMemMalloc(lcd_w*lcd_h*3/2,32);
	if(!objName->preFrameBuf)
	{
		deg_Printf("animationInit mem err!\n");
		return ;
	};
	memcpy(objName->preFrameBuf,buf->y_addr,lcd_w*lcd_h*3/2);
	dispLayerFreeFrame(DISP_LAYER_VIDEO,buf);

	//----
	XOSTimeDly(80);
	
	deg_Printf("sensor_change_flag:%d\n",SysCtrl.sensor_change_flag);
	if(SysCtrl.sensor_change_flag)
	{
		 if(get_sen_mode()==1)
		 {
		 	set_sen_mode(0);

		 }
		 else
		{
		 	set_sen_mode(1);
		}	
		sensor_change();
	}
	
	deg_Printf("sensor mode=%d\n",get_sen_mode());
	//----
	objName->actionFunc= getActionFunc(objName->action);	
	//dispLayerUninit(DISP_LAYER_OSD0);
	hal_osdEnable(OSD0,0);
	deg_Printf("...animationInit!\n");

}


bool animationRun(void *objName, u8 *dstBuf)
{
	UserInterface *name = (UserInterface *)objName;

	if(!name->preFrameBuf || !dstBuf)
	{
		deg_Printf("preFrameBuf or dstBuf err\n");
		name->cnt = 0;
	}
	deg_Printf("%d\n", name->cnt);
	if(!name->cnt){
		if(name->preFrameBuf){
			hal_sysMemFree(name->preFrameBuf);
			name->preFrameBuf = NULL;
		}
		//dispLayerInit(DISP_LAYER_OSD0);
		hal_osdEnable(OSD0,1);
		return true;
	}
	name->actionFunc(dstBuf, name->preFrameBuf, name->cnt);
	name->cnt--;
	return false;
}


void animationUnInit(UserInterface *objName)
{
	if(NULL!=objName->preFrameBuf)
	{
		hal_sysMemFree(objName->preFrameBuf);
		objName->preFrameBuf = NULL;
		deg_Printf("animationInit free.\n");
	}

}
INT32U jump_process_cnt=1;
INT16U jump_process_color=0x9fff;
uint8 jump_process_black=1;
uint8 jump_process_type;




//#define GET_16BIT_DATA(addr)		(*((INT16U *)addr))
void circle_fill(INT16S x0, INT16S y0, INT16U r, INT32U buff, INT32U src, INT8U flag)
{
	INT16S x = 0, y = r, i/*, len*/;
	INT32U offset;
	INT16U *_dest = (INT16U *)buff, *_src = (INT16U *)src;
	INT32S d = 3 - 2 * r;
	INT16U color = 0xc618;
	if(r == 0){
		memcpy((INT32U *)buff,(INT32U *)src, TFT_WIDTH*TFT_HEIGHT*2);
		return;
	}
	while(x <= y){
		#if 0//0, because of bug
		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			len = x0 - x;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(y + y0)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(y + y0)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}
			len =  TFT_WIDTH - (x0 + x);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + x) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(y + y0)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(y + y0)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			len = x0 - x;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(y0 - y)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(y0 - y)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}
			len =  TFT_WIDTH - (x0 + x);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + x) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(y0 - y)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(y0 - y)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			len = x0 - y;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(x + y0)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(x + y0)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}	
			len =  TFT_WIDTH - (x0 + y);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + y) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(x + y0)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(x + y0)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			len = x0 - y;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(y0 - x)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(y0 - x)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}
			len =  TFT_WIDTH - (x0 + y);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + y) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(y0 - x)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(y0 - x)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		#else
		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			offset = (y + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			offset = (y0 - y) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			offset = (x + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			offset = (y0 - x) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		#endif
		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(buff + (y + y0) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (y + y0) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(buff + (y0 - y) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (y0 - y) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
			
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(buff + (x + y0) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (x + y0) * TFT_WIDTH + x0 + y) = jump_process_color;
			}	
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(buff + (y0 - x) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (y0 - x) * TFT_WIDTH + x0 + y) = jump_process_color;
			}
		}
		//#endif
		if(d < 0){
			d += 4* x + 6;
			x++;
		}else{
			d += 10 + 4 * (x - y);
			x++;
			y--;
		}
		
	}
#if 1
	if(y0 - r > 0){
		INT32U i = 0;
		for(i = 0; i < y0 - r; i++){
			memcpy((INT32U *)(buff + i * TFT_WIDTH * 2),
						(INT32U *)(src + i * TFT_WIDTH * 2), 
						TFT_WIDTH << 1);
		}
	}
	if(y0 + r + 1 < TFT_HEIGHT){
		INT32U i = 0;
		for(i = y0 + r + 1; i < TFT_HEIGHT; i++){
			memcpy((INT32U *)(buff + i * TFT_WIDTH * 2),
						(INT32U *)(src + i * TFT_WIDTH * 2), 
						TFT_WIDTH << 1);
		}
	}
#endif
}


void type_function_circle(INT32U buff)
{
	if(jump_process_cnt < 8)
	{
		if(jump_process_type == CIRCLE_OPEN)
		{
			INT32U r = jump_process_cnt * 25;
			circle_fill(160, 120, r, buff, /*jump_process_buff*/0, jump_process_black);
		}
		else if(jump_process_type == CIRCLE_CLOSE)
		{
			
		}
		jump_process_cnt++;
	}
	else
	{
		jump_process_type = JUMP_PROCESS_TYPE_NONE;
	}
}

/************************** main function CLAHE ******************/
static void ClipHistogram (u32* pulHistogram, u32 uiNrGreylevels, u32 ulClipLimit)
/* This function performs clipping of the histogram and redistribution of bins.
 * The histogram is clipped and the number of excess pixels is counted. Afterwards
 * the excess pixels are equally redistributed across the whole histogram (providing
 * the bin count is smaller than the cliplimit).
 */
{
    u32* pulBinPointer, *pulEndPointer, *pulHisto;
    u32 ulNrExcess, ulUpper, ulBinIncr, ulStepSize, i;
    s32 lBinExcess;

    ulNrExcess = 0;  pulBinPointer = pulHistogram;
    for (i = 0; i < uiNrGreylevels; i++) 
	{ /* calculate total number of excess pixels */
    	lBinExcess = (s32) pulBinPointer[i] - (s32) ulClipLimit;
    	if (lBinExcess > 0) ulNrExcess += lBinExcess;      /* excess in current bin */
    };

    /* Second part: clip histogram and redistribute excess pixels in each bin */
    ulBinIncr = ulNrExcess / uiNrGreylevels;          /* average binincrement */
    ulUpper =  ulClipLimit - ulBinIncr;     /* Bins larger than ulUpper set to cliplimit */

    for (i = 0; i < uiNrGreylevels; i++) 
	{
	      if (pulHistogram[i] > ulClipLimit) 
	      {
		  	  pulHistogram[i] = ulClipLimit; /* clip bin */
	      }
	      else 
		  {
	      	  if (pulHistogram[i] > ulUpper) 
			  {        /* high bin count */
	          		ulNrExcess -= (ulClipLimit -pulHistogram[i]); pulHistogram[i]=ulClipLimit;
		      }
		      else 
			  {                    /* low bin count */
		          	ulNrExcess -= ulBinIncr; pulHistogram[i] += ulBinIncr;
		      }
	      }
     }

    while (ulNrExcess) 
	{   /* Redistribute remaining excess  */
	    pulEndPointer = &pulHistogram[uiNrGreylevels]; pulHisto = pulHistogram;

	    while (ulNrExcess && pulHisto < pulEndPointer) 
		{
	        ulStepSize = uiNrGreylevels / ulNrExcess;
	        if (ulStepSize < 1) ulStepSize = 1;          /* stepsize at least 1 */
	        for (pulBinPointer=pulHisto; pulBinPointer < pulEndPointer && ulNrExcess;pulBinPointer += ulStepSize) 
			{
		        if (*pulBinPointer < ulClipLimit) 
				{
		            (*pulBinPointer)++;     
					ulNrExcess--;      /* reduce excess */
		        }
	        }
	        pulHisto++;          /* restart redistributing on other bin location */
	    }
    }
}

static void MakeHistogram (u8* pImage, u32 uiXRes,
        u32 uiSizeX, u32 uiSizeY,
        u32* pulHistogram,
        u32 uiNrGreylevels, u8* pLookupTable)
/* This function classifies the greylevels present in the array image into
 * a greylevel histogram. The pLookupTable specifies the relationship
 * between the greyvalue of the pixel (typically between 0 and 4095) and
 * the corresponding bin in the histogram (usually containing only 128 bins).
 */
{
    u8* pImagePointer;
    u32 i;

    for (i = 0; i < uiNrGreylevels; i++) pulHistogram[i] = 0L; /* clear histogram */
    for (i = 0; i < uiSizeY; i++) 
	{
	    pImagePointer = &pImage[uiSizeX];
	    while (pImage < pImagePointer) pulHistogram[pLookupTable[*pImage++]]++;
	    pImagePointer += uiXRes;
	    pImage = &pImagePointer[-uiSizeX];
    }
}

static void MapHistogram (u32* pulHistogram, u8 Min, u8 Max,
           u32 uiNrGreylevels, u32 ulNrOfPixels)
/* This function calculates the equalized lookup table (mapping) by
 * cumulating the input histogram. Note: lookup table is rescaled in range [Min..Max].
 */
{
    u32 i;  u32 ulSum = 0;
    const float fScale = ((float)(Max - Min)) / ulNrOfPixels;
    const u32 ulMin = (u32) Min;

    for (i = 0; i < uiNrGreylevels; i++) 
	{
	    ulSum += pulHistogram[i]; 
		pulHistogram[i]=(u32)(ulMin+ulSum*fScale);
	    if (pulHistogram[i] > Max) pulHistogram[i] = Max;
    }
}

static void MakeLut (u8 * pLUT, u8 Min, u8 Max, u32 uiNrBins)
/* To speed up histogram clipping, the input image [Min,Max] is scaled down to
 * [0,uiNrBins-1]. This function calculates the LUT.
 */
{
    int i;
    const u8 BinSize = (u8) (1 + (Max - Min) / uiNrBins);

    for (i = Min; i <= Max; i++)  pLUT[i] = (i - Min) / BinSize;
}

static void Interpolate (u8 * pImage, int uiXRes, u32 * pulMapLU,
     u32 * pulMapRU, u32 * pulMapLB,  u32 * pulMapRB,
     u32 uiXSize, u32 uiYSize, u8 * pLUT)
/* pImage      - pointer to input/output image
 * uiXRes      - resolution of image in x-direction
 * pulMap*     - mappings of greylevels from histograms
 * uiXSize     - uiXSize of image submatrix
 * uiYSize     - uiYSize of image submatrix
 * pLUT           - lookup table containing mapping greyvalues to bins
 * This function calculates the new greylevel assignments of pixels within a submatrix
 * of the image with size uiXSize and uiYSize. This is done by a bilinear interpolation
 * between four different mappings in order to eliminate boundary artifacts.
 * It uses a division; since division is often an expensive operation, I added code to
 * perform a logical shift instead when feasible.
 */
{
    const u32 uiIncr = uiXRes-uiXSize; /* Pointer increment after processing row */
    u8 GreyValue; u32 uiNum = uiXSize*uiYSize; /* Normalization factor */
    u32 uiXCoef, uiYCoef, uiXInvCoef, uiYInvCoef, uiShift = 0;

    if (uiNum & (uiNum - 1))   /* If uiNum is not a power of two, use division */
    {
	    for (uiYCoef = 0, uiYInvCoef = uiYSize; uiYCoef < uiYSize;uiYCoef++, uiYInvCoef--,pImage+=uiIncr) 
		{
		    for (uiXCoef = 0, uiXInvCoef = uiXSize; uiXCoef < uiXSize;uiXCoef++, uiXInvCoef--) 
			{
		        GreyValue = pLUT[*pImage];           /* get histogram bin value */
		        *pImage++ = (u8 ) ((uiYInvCoef * (uiXInvCoef*pulMapLU[GreyValue]
		                      + uiXCoef * pulMapRU[GreyValue])
		                + uiYCoef * (uiXInvCoef * pulMapLB[GreyValue]
		                      + uiXCoef * pulMapRB[GreyValue])) / uiNum);
		    }
	    }
    }
    else 
	{               /* avoid the division and use a right shift instead */
	    while (uiNum >>= 1) uiShift++;           /* Calculate 2log of uiNum */
	    for (uiYCoef = 0, uiYInvCoef = uiYSize; uiYCoef < uiYSize;uiYCoef++, uiYInvCoef--,pImage+=uiIncr) 
		{
	         for (uiXCoef = 0, uiXInvCoef = uiXSize; uiXCoef < uiXSize;uiXCoef++, uiXInvCoef--) 
			{
	           GreyValue = pLUT[*pImage];      /* get histogram bin value */
	           *pImage++ = (u8)((uiYInvCoef* (uiXInvCoef * pulMapLU[GreyValue]
	                      + uiXCoef * pulMapRU[GreyValue])
	                + uiYCoef * (uiXInvCoef * pulMapLB[GreyValue]
	                      + uiXCoef * pulMapRB[GreyValue])) >> uiShift);
	        }
	    }
    }
}

 int CLAHE (u8* pImage, u32 uiXRes, u32 uiYRes,
	  u8 Min, u8 Max, u32 uiNrX, u32 uiNrY,u32 uiNrBins, float fCliplimit)
 /*   pImage - Pointer to the input/output image
  *   uiXRes - Image resolution in the X direction
  *   uiYRes - Image resolution in the Y direction
  *   Min - Minimum greyvalue of input image (also becomes minimum of output image)
  *   Max - Maximum greyvalue of input image (also becomes maximum of output image)
  *   uiNrX - Number of contextial regions in the X direction (min 2, max uiMAX_REG_X)
  *   uiNrY - Number of contextial regions in the Y direction (min 2, max uiMAX_REG_Y)
  *   uiNrBins - Number of greybins for histogram ("dynamic range")
  *   float fCliplimit - Normalized cliplimit (higher values give more contrast)
  * The number of "effective" greylevels in the output image is set by uiNrBins; selecting
  * a small value (eg. 128) speeds up processing and still produce an output image of
  * good quality. The output image will have the same minimum and maximum value as the input
  * image. A clip limit smaller than 1 results in standard (non-contrast limited) AHE.
  */
 {
	 u32 uiX, uiY;			/* counters */
	 u32 uiXSize, uiYSize, uiSubX, uiSubY; /* size of context. reg. and subimages */
	 u32 uiXL, uiXR, uiYU, uiYB;  /* auxiliary variables interpolation routine */
	 u32 ulClipLimit, ulNrPixels;/* clip limit and region pixel count */
	 u8* pImPointer;		   /* pointer to image */
	 u8  aLUT[256]; 		 /* lookup table used for scaling of input image */
	 u32* pulHist, *pulMapArray; /* pointer to histogram and mappings*/
	 u32* pulLU, *pulLB, *pulRU, *pulRB; /* auxiliary pointers interpolation */
 
	 if (uiNrX > 64) return -1; 	  /* # of regions x-direction too large */
	 if (uiNrY > 64) return -2; 	  /* # of regions y-direction too large */
	 if (uiXRes % uiNrX) return -3; 	 /* x-resolution no multiple of uiNrX */
	 if (uiYRes & uiNrY) return -4; 	 /* y-resolution no multiple of uiNrY */
	 if (Max >= 256) return -5; 		 /* maximum too large */
	 if (Min >= Max) return -6; 		 /* minimum equal or larger than maximum */
	 if (uiNrX < 2 || uiNrY < 2) return -7;/* at least 4 contextual regions required */
	 if (fCliplimit == 1.0) return 0;	   /* is OK, immediately returns original image. */
	 if (uiNrBins == 0) uiNrBins = 128; 	 /* default value when not specified */
 
	 //pulMapArray=(unsigned long *)malloc(sizeof(unsigned long)*uiNrX*uiNrY*uiNrBins);
	 

	 pulMapArray=(u32 *)hal_sysMemMalloc(sizeof(u32)*uiNrX*uiNrY*uiNrBins,32);
	 if (pulMapArray == 0) return -8;	   /* Not enough memory! (try reducing uiNrBins) */
 		
	 uiXSize = uiXRes/uiNrX; uiYSize = uiYRes/uiNrY;  /* Actual size of contextual regions */
	 ulNrPixels = (u32)uiXSize * (u32)uiYSize;
 
	 if(fCliplimit > 0.0) 
	 {			/* Calculate actual cliplimit	  */
		ulClipLimit = (u32) (fCliplimit * (uiXSize * uiYSize) / uiNrBins);
		ulClipLimit = (ulClipLimit < 1UL) ? 1UL : ulClipLimit;
	 }
	 else
	 {
		 ulClipLimit = 1UL<<14; 		 /* Large value, do not clip (AHE) */
	 }
 
	 MakeLut(aLUT, Min, Max, uiNrBins); 	 /* Make lookup table for mapping of greyvalues */
	 /* Calculate greylevel mappings for each contextual region */
	 for (uiY = 0, pImPointer = pImage; uiY < uiNrY; uiY++) 
	 {
		 for (uiX = 0; uiX < uiNrX; uiX++, pImPointer += uiXSize) 
		 {
			 pulHist = &pulMapArray[uiNrBins * (uiY * uiNrX + uiX)];
			 MakeHistogram(pImPointer,uiXRes,uiXSize,uiYSize,pulHist,uiNrBins,aLUT); 
			 ClipHistogram(pulHist, uiNrBins, ulClipLimit);
			 MapHistogram(pulHist, Min, Max, uiNrBins, ulNrPixels);
		 }
		 pImPointer += (uiYSize - 1) * uiXRes;			/* skip lines, set pointer */
	 }
	 /* Interpolate greylevel mappings to get CLAHE image */
	 for (pImPointer = pImage, uiY = 0; uiY <= uiNrY; uiY++)
	 {
		 if (uiY == 0)
		 {						/* special case: top row */
			 uiSubY = uiYSize >> 1;  uiYU = 0; uiYB = 0;
		 }
		 else
		 {
			 if (uiY == uiNrY)
			 {					/* special case: bottom row */
				 uiSubY = uiYSize >> 1;    uiYU = uiNrY-1;	   uiYB = uiYU;
			 }
			 else
			 {						/* default values */
				 uiSubY = uiYSize; uiYU = uiY - 1; uiYB = uiYU + 1;
			 }
		 }
 
		 for (uiX = 0; uiX <= uiNrX; uiX++)
		 {
			 if (uiX == 0)
			 {					/* special case: left column */
				 uiSubX = uiXSize >> 1; uiXL = 0; uiXR = 0;
			 }
			 else
			 {
				 if (uiX == uiNrX)
				 {				/* special case: right column */
					 uiSubX = uiXSize >> 1;  uiXL = uiNrX - 1; uiXR = uiXL;
				 }
				 else
				 {						/* default values */
					 uiSubX = uiXSize; uiXL = uiX - 1; uiXR = uiXL + 1;
				 }
			 }
 
			 pulLU = &pulMapArray[uiNrBins * (uiYU * uiNrX + uiXL)];
			 pulRU = &pulMapArray[uiNrBins * (uiYU * uiNrX + uiXR)];
			 pulLB = &pulMapArray[uiNrBins * (uiYB * uiNrX + uiXL)];
			 pulRB = &pulMapArray[uiNrBins * (uiYB * uiNrX + uiXR)];

			 Interpolate(pImPointer,uiXRes,pulLU,pulRU,pulLB,pulRB,uiSubX,uiSubY,aLUT);
			 pImPointer += uiSubX;				/* set pointer on next matrix */
		 }
		 pImPointer += (uiSubY - 1) * uiXRes;
	 }


	 hal_sysMemFree(pulMapArray);		/* free space for histograms */
	 return 0;							/* return status OK */
 }

  static s32 qBound(s32 min_val, s32 val, s32 max_val)
  {
	  return max(min_val, min(max_val, val));
  }

  void FloydSteinbergDithering(u8* ybuf,u16 w, u16 h,u8 thred)
  {
	  u8  *ptr_y = (u8 *)ybuf;
	  s32 i, j;
	  s32 diff;
  
#if 0
  
	  for(i = 0; i < PRINT_HEIGHT - 1; i++)
	  {
		  for(j = 0; j < PRINT_WIDTH - 1; j++)
		  {
			  if(ptr_y[i * PRINT_WIDTH + j] > 127)
			  {
				  diff = ptr_y[i * PRINT_WIDTH + j] - 255;
				  ptr_y[i * PRINT_WIDTH + j] = 255;
			  }
			  else
			  {
				  diff = ptr_y[i * PRINT_WIDTH + j];
				  ptr_y[i * PRINT_WIDTH + j] = 0;
			  }
			  
			  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 3 / 8, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j] + diff * 3 / 8, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 1 / 4, 255);
		  }
		  ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] > 127 ? 255 : 0;
	  }
	  
	  for(j = 0; j < PRINT_WIDTH; j++)
	  {
		  ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] > 127 ? 255 : 0;
	  }
  
#elif 0
  
	  for(i = 1; i < PRINT_HEIGHT - 1; i++)
	  {
		  for(j = 1; j < PRINT_WIDTH - 1; j++)
		  {
			  if(ptr_y[i * PRINT_WIDTH + j] > 127)
			  //if(ptr_y[i * PRINT_WIDTH + j] > thred)
			  {
				  diff = ptr_y[i * PRINT_WIDTH + j] - 255;
				  ptr_y[i * PRINT_WIDTH + j] = 255;
			  }
			  else
			  {
				  diff = ptr_y[i * PRINT_WIDTH + j];
				  ptr_y[i * PRINT_WIDTH + j] = 0;
			  }
			  
			  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 7 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j - 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 1] + diff * 3 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j] + diff * 5 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 1 / 16, 255);
		  }
		  ptr_y[i * PRINT_WIDTH] = ptr_y[i * PRINT_WIDTH] > 127 ? 255 : 0;
		  ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] > 127 ? 255 : 0;
		  //ptr_y[i * PRINT_WIDTH] = ptr_y[i * PRINT_WIDTH] > thred ? 255 : 0;
		  //ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] > thred ? 255 : 0;
	  }
	  
	  for(j = 0; j < PRINT_WIDTH; j++)
	  {
		  ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] > 127 ? 255 : 0;
		  //ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] > thred ? 255 : 0;
	  }
#elif 0
	  
		  for(i = 1; i < PRINT_HEIGHT - 1; i++)
		  {
			  for(j = 1; j < PRINT_WIDTH - 1; j++)
			  {
				  //if(ptr_y[i * PRINT_WIDTH + j] >255)
				  if(ptr_y[i * PRINT_WIDTH + j] > thred)
				  {
					  diff = ptr_y[i * PRINT_WIDTH + j] - 255;
					  ptr_y[i * PRINT_WIDTH + j] = 255;
				  }
				  else
				  {
					  diff = ptr_y[i * PRINT_WIDTH + j];
					  ptr_y[i * PRINT_WIDTH + j] = 0;
				  }
				  
				  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 7 / 16, 255);
				  ptr_y[(i + 1) * PRINT_WIDTH + j - 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 1] + diff * 3 / 16, 255);
				  ptr_y[(i + 1) * PRINT_WIDTH + j] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j] + diff * 5 / 16, 255);
				  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 1 / 16, 255);
			  }
			  //ptr_y[i * PRINT_WIDTH] = ptr_y[i * PRINT_WIDTH] >255 ? 255 : 0;
			  //ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] >255 ? 255 : 0;
			  ptr_y[i * PRINT_WIDTH] = ptr_y[i * PRINT_WIDTH] > thred ? 255 : 0;
			  ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] > thred ? 255 : 0;
		  }
		  
		  for(j = 0; j < PRINT_WIDTH; j++)
		  {
			  //ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] >255 ? 255 : 0;
			  ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] > thred ? 255 : 0;
		  }
#elif 1			//40  no thred
  
	  for(i = 1; i < h - 1; i++)
	  {
		  for(j = 1; j < w - 1; j++)
		  {
			  if(ptr_y[i * w + j] >127)
			  {
				  diff = ptr_y[i * w + j] - 255;
				  ptr_y[i * w + j] = 255;
			  }
			  else
			  {
				  diff = ptr_y[i * w + j];
				  ptr_y[i * w + j] = 0;
			  }
  
			  /*   1  
			  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 7 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j - 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 1] + diff * 3 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j] + diff * 5 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 1 / 16, 255);
			  
			  */
			  
			  /*	2 
			  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 8/ 32, 255);
			  ptr_y[i * PRINT_WIDTH + j + 2] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 2] + diff * 4/ 32, 255);
							  
			  ptr_y[(i + 1) * PRINT_WIDTH + j - 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 1] + diff * 2 / 32,255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j - 2] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 2] + diff * 4 / 32,255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j	 ] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j	  ] + diff * 8 / 32, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 4 / 32, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 2] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 2] + diff * 2 / 32, 255);
			  */
			  
			  
			  /*  3   */
	  
			  ptr_y[i * w + j + 1] = qBound(0, ptr_y[i * w + j + 1] + diff * 8/ 42, 255);
			  ptr_y[i * w + j + 2] = qBound(0, ptr_y[i * w + j + 2] + diff * 4/ 42, 255);
							  
			  ptr_y[(i + 1) * w + j - 1] = qBound(0, ptr_y[(i + 1) * w + j - 1] + diff * 2 / 42,255);
			  ptr_y[(i + 1) * w + j - 2] = qBound(0, ptr_y[(i + 1) * w + j - 2] + diff * 4 / 42,255);
			  ptr_y[(i + 1) * w + j    ] = qBound(0, ptr_y[(i + 1) * w + j	  ] + diff * 8 / 42, 255);
			  ptr_y[(i + 1) * w + j + 1] = qBound(0, ptr_y[(i + 1) * w + j + 1] + diff * 4 / 42, 255);
			  ptr_y[(i + 1) * w + j + 2] = qBound(0, ptr_y[(i + 1) * w + j + 2] + diff * 2 / 42, 255);
						  
	  
  
			  ptr_y[(i + 2) * w + j - 1] = qBound(0, ptr_y[(i + 2) * w + j - 1] + diff * 1 / 42,  255);
			  ptr_y[(i + 2) * w + j - 2] = qBound(0, ptr_y[(i + 2) * w + j - 2] + diff * 2 / 42,255);
			  ptr_y[(i + 2) * w + j    ] = qBound(0, ptr_y[(i + 2) * w + j	  ] + diff * 4 / 42, 255);
			  ptr_y[(i + 2) * w + j + 1] = qBound(0, ptr_y[(i + 2) * w + j + 1] + diff * 2 / 42, 255);
			  ptr_y[(i + 2) * w + j + 2] = qBound(0, ptr_y[(i + 2) * w + j + 2]  + diff * 1 / 42, 255);
  
		  }
		  //ptr_y[i * PRINT_WIDTH] = ptr_y[i * PRINT_WIDTH] >255 ? 255 : 0;
		  //ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] >255 ? 255 : 0;
		  ptr_y[i * w] = ptr_y[i * w] >127 ? 255 : 0;
		  ptr_y[(i + 1) * w - 1] = ptr_y[(i + 1) * w - 1] >127 ? 255 : 0;
	  }
	  
	  for(j = 0; j < w; j++)
	  {
		  //ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] >255 ? 255 : 0;
		  ptr_y[(h - 1) * w + j] = ptr_y[(h - 1) * w + j] >127 ? 255 : 0;
	  }
  
#elif 0	//40    thred
  
	  for(i = 1; i < PRINT_HEIGHT - 1; i++)
	  {   
		  for(j = 1; j < PRINT_WIDTH - 1; j++)
		  {
			  //if(ptr_y[i * PRINT_WIDTH + j] >255)
			  
			  //thred=get_part_yu(i,j);
			  if(ptr_y[i * PRINT_WIDTH + j] >thred)
			  {
				  diff = ptr_y[i * PRINT_WIDTH + j] - 255;	  //误差
				  ptr_y[i * PRINT_WIDTH + j] = 255;
			  }
			  else
			  {
				  diff = ptr_y[i * PRINT_WIDTH + j];
				  ptr_y[i * PRINT_WIDTH + j] = 0;
			  }
  
			  
			  /*
			  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 3/8, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j] + diff * 3/8, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 2/8, 255);   
			  */
  
			  
			  
			  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 7 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j - 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 1] + diff * 3 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j] + diff * 5 / 16, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 1 / 16, 255);
			  
		  
  
			  /*
			  ptr_y[i * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 1] + diff * 8/ 42, 255);
			  ptr_y[i * PRINT_WIDTH + j + 2] = qBound(0, ptr_y[i * PRINT_WIDTH + j + 2] + diff * 4/ 42, 255);
			  
			  ptr_y[(i + 1) * PRINT_WIDTH + j - 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 1] + diff * 2 / 42,255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j - 2] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j - 2] + diff * 4 / 42,255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j	 ] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j	  ] + diff * 8 / 42, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 1] + diff * 4 / 42, 255);
			  ptr_y[(i + 1) * PRINT_WIDTH + j + 2] = qBound(0, ptr_y[(i + 1) * PRINT_WIDTH + j + 2] + diff * 2 / 42, 255);
		  
  
  
			  ptr_y[(i + 2) * PRINT_WIDTH + j - 1] = qBound(0, ptr_y[(i + 2) * PRINT_WIDTH + j - 1] + diff * 1 / 42,  255);
			  ptr_y[(i + 2) * PRINT_WIDTH + j - 2] = qBound(0, ptr_y[(i + 2) * PRINT_WIDTH + j - 2] + diff * 2 / 42,255);
			  ptr_y[(i + 2) * PRINT_WIDTH + j	 ] = qBound(0, ptr_y[(i + 2) * PRINT_WIDTH + j	  ] + diff * 4 / 42, 255);
			  ptr_y[(i + 2) * PRINT_WIDTH + j + 1] = qBound(0, ptr_y[(i + 2) * PRINT_WIDTH + j + 1] + diff * 2 / 42, 255);
			  ptr_y[(i + 2) * PRINT_WIDTH + j + 2] = qBound(0, ptr_y[(i + 2) * PRINT_WIDTH + j + 2]  + diff * 1 / 42, 255);
			  */
			  
			  
	  
			  
		  }
		  //ptr_y[i * PRINT_WIDTH] = ptr_y[i * PRINT_WIDTH] >255 ? 255 : 0;
		  //ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] >255 ? 255 : 0;
		  ptr_y[i * PRINT_WIDTH] = ptr_y[i * PRINT_WIDTH] >thred ? 255 : 0;
		  ptr_y[(i + 1) * PRINT_WIDTH - 1] = ptr_y[(i + 1) * PRINT_WIDTH - 1] >thred ? 255 : 0;
	  }
	  
	  for(j = 0; j < PRINT_WIDTH; j++)
	  {
		  //ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] >255 ? 255 : 0;
		  ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] = ptr_y[(PRINT_HEIGHT - 1) * PRINT_WIDTH + j] >thred ? 255 : 0;
	  }
  
#endif
  
  }

#if 0
  //M2
  const u8 BayerPattern[4][4] = 
  {
	  0,  8,  2,  10,
	  12, 4,  14, 6,
	  3,  11, 1,  9,
	  15, 7,  13, 5
  };
  
#elif 0
  //M3
  const u8 BayerPattern[8][8] = 
  {
	  {0,  32, 8,  40, 2,  34, 10, 42},
	  {48, 16, 56, 24, 50, 18, 58, 26},
	  {12, 44, 4,  36, 14, 46, 6,  38},
	  {60, 28, 52, 20, 62, 30, 54, 22},
	  {3,  35, 11, 43, 1,  33, 9,  41},
	  {51, 19, 59, 27, 49, 17, 57, 25},
	  {15, 47, 7,  39, 13, 45, 5,  37},
	  {63, 31, 55, 23, 61, 29, 53, 21}
  };
  
#elif 0
  //M4
  const u8 BayerPattern[16][16] = 
  {
	  0,   128, 32,  160, 8,   136, 40,  168, 2,   130, 34,  162, 10,  138, 42,  170,
	  192, 64,	224, 96,  200, 72,	232, 104, 194, 66,	226, 98,  202, 74,	234, 106,
	  48,  176, 16,  144, 56,  184, 24,  152, 50,  178, 18,  146, 58,  186, 26,  154,
	  240, 112, 208, 80,  248, 120, 216, 88,  242, 114, 210, 82,  250, 122, 218, 90,
	  12,  140, 44,  172, 4,   132, 36,  164, 14,  142, 46,  174, 6,   134, 38,  166,
	  204, 76,	236, 108, 196, 68,	228, 100, 206, 78,	238, 110, 198, 70,	230, 102,
	  60,  188, 28,  156, 52,  180, 20,  148, 62,  190, 30,  158, 54,  182, 22,  150,
	  252, 124, 220, 92,  244, 116, 212, 84,  254, 126, 222, 94,  246, 118, 214, 86, 
	  3,   131, 35,  163, 11,  139, 43,  171, 1,   129, 33,  161, 9,   137, 41,  169,
	  195, 67,	227, 99,  203, 75,	235, 107, 193, 65,	225, 97,  201, 73,	233, 105,
	  51,  179, 19,  147, 59,  187, 27,  155, 49,  177, 17,  145, 57,  185, 25,  153,
	  243, 115, 211, 83,  251, 123, 219, 91,  241, 113, 209, 81,  249, 121, 217, 89,
	  15,  143, 47,  175, 7,   135, 39,  167, 13,  141, 45,  173, 5,   133, 37,  165,
	  207, 79,	239, 111, 199, 71,	231, 103, 205, 77,	237, 109, 197, 69,	229, 101,
	  63,  191, 31,  159, 55,  183, 23,  151, 61,  189, 29,  157, 53,  181, 21,  149,
	  255, 127, 223, 95,  247, 119, 215, 87,  253, 125, 221, 93,  245, 117, 213, 85
  };
#endif
  


#if 0
  void BayerDithering(u8* ybuf,u16 w, u16 h)
  {
	  u8  *ptr_y = (u8 *)ybuf;
	  s32 i, j;
	  u8  data;
  
	  for(i = 0; i < h; i++)
	  {
		  for(j = 0; j < w; j++)
		  {
			  data = ptr_y[w * i + j];
			  if((data >> 2) > BayerPattern[ i & 7][ j & 7])  //M3
			  //if(data > BayerPattern[i & 15][j & 15]) 		  //M4
			  {
				  ptr_y[w * i + j] = 255;
			  }
			  else
			  {
				  ptr_y[w * i + j] = 0;
			  }
		  }
	  }
  }
#endif


void y_process(u8*buf,u32 size)
{
#if 0	//gamma处理：时间太长

	double c = 1.0;  // 常数，这里设为1.0
    double inverse_gamma = r;//1.0 / gamma;
    int i = 0 ;
    for (i = 0; i < size; i++) {
            // 将像素值从[0, 255]范围归一化到[0, 1]范围
            double normalized = (double)(buf[i]) / 255.0;
            // 进行伽马矫正计算
            double corrected = pow(normalized, inverse_gamma);
            // 将矫正后的像素值从[0, 1]范围重新映射回[0, 255]范围
            buf[i] = (unsigned char)(corrected * 255.0 + 0.5);  // +0.5用于四舍五入
        	hal_wdtClear();
    }
#elif 1	//线性对比
	#if 0
		u8 min = 0xff,max = 0;
		u8 refYvalue = 0;
		u32 i;
		for(i = 0;i<WIDTH*HEIGHT;i++){
			buf[i] = buf[i]/2;
			min = (min > buf[i])?buf[i]:min;
			max = (max > buf[i])?max:buf[i];
		}

		refYvalue = max - min-20;
	    
		for(i = 0;i<WIDTH*HEIGHT;i++){
			//buf[i] = ((buf[i]-min)*255)/(max-min);
			buf[i] = ((buf[i]-min)*(255-0))/refYvalue+0;
		}
	#else	//
		int a,b,i,new_pixel_value;
		a = 8;//8//8//15//越大越白
		b = 30;//20~30//50//-20

		/*u8 min = 0xff,max = 0;
		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i])
			{
				min = (min > buf[i])?buf[i]:min;
				max = (max > buf[i])?max:buf[i];
			}
		}
		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i]){
				buf[i] = (255*buf[i])/(max-min);
			}
		}*/
		
		for (i = 0; i < size; i++) {
	        // 根据线性变换公式计算新的像素值
	        #if 0
	        if(buf[i]<50)
	        	new_pixel_value = (int)((8 * buf[i])/10 +20 );
	        else if(buf[i]>160)
	        	new_pixel_value = (int)((8 * buf[i])/10 +42 );//9 * buf[i])/10 +26 
	        else
	        	new_pixel_value = (int)buf[i] +10;
	        #elif 1
			if(buf[i]>200)
	        	new_pixel_value = (int)((7 * buf[i])/10 +60 );
	        else
	        	new_pixel_value = (int)((8 * buf[i])/10 +40 );
	        #else
            new_pixel_value = (int)((a * buf[i])/10 + b);
            #endif
            // 确保新的像素值在0 - 255范围内（针对8位灰度图像）
            if (new_pixel_value < 0) {
                new_pixel_value = 0;
            } else if (new_pixel_value > 255) {
                new_pixel_value = 255;
            }
            buf[i] = new_pixel_value;
	    }


		#if 0
		u8 min = b,max = 255;
		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i])
			{
				#if 1
				min = (min > buf[i])?buf[i]:min;
				#endif
				max = (max > buf[i])?max:buf[i];
			}
		}
		
		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i]){
				new_pixel_value = (255*(buf[i]-min))/(max-min);
			}
			if(new_pixel_value >= 255)
			  new_pixel_value = 255;

		   if(new_pixel_value <= 0)
		   {
			 new_pixel_value = 0;
		   }
		   
		   buf[i] = new_pixel_value;
		}
		#else
		u8 min = 40,max = 60;
		for(i = 0;i<size;i++){
			if(buf[i]<=max){
				new_pixel_value = (max*(buf[i]-min))/(max-min);
			}
			else
			{
				new_pixel_value = buf[i];
			}
			if(new_pixel_value >= 255)
			  new_pixel_value = 255;

		   if(new_pixel_value <= 0)
		   {
			 new_pixel_value = 0;
		   }
		   
		   buf[i] = new_pixel_value;
		}
		#endif
	    
	#endif
#elif 1	//直方图均衡化
	#if 1
		u32 histogram[256] = {0};
		u32 pixsum[PRINTER_GRAY_STEP] = {0};
	    u32 i,new_pixel_value;
	    u32 sum = 0;
	    u32 temp0,temp1,temp3,temp4;

		u8 min = 0xff,max = 0;
		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i])
			{
				min = (min > buf[i])?buf[i]:min;
				max = (max > buf[i])?max:buf[i];
			}
		}

		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i]){
				new_pixel_value = (255*buf[i])/(max-min);
			}
			else{
				new_pixel_value = buf[i];
			}
			 if (new_pixel_value < 0) {
                new_pixel_value = 0;
            } else if (new_pixel_value > 255) {
                new_pixel_value = 255;
            }
            
            buf[i] = new_pixel_value;
	    }
		
	     
	    for (i = 0; i < size; i++) {
	        u8 pixel_value = buf[i];
	        histogram[(pixel_value*PRINTER_GRAY_STEP)/255]++;
	    }

		 for(i=0;i<PRINTER_GRAY_STEP;i++){
		    sum+= histogram[i];
			pixsum[i] = sum;
		 }
		
		for (i = 0; i < size; i++) {
	          
			 //temp0 = pixsum[(buf[i]*PRINTER_GRAY_STEP)/255]*255/size;
			// temp1 = pixsum[(buf[i]*PRINTER_GRAY_STEP)/255+1]*255/size;
	         //temp3 = buf[i]*PRINTER_GRAY_STEP*PRINTER_GRAY_STEP/255;
	         //temp4 = ((buf[i]*PRINTER_GRAY_STEP/255)+1)*PRINTER_GRAY_STEP;
	         //buf[i] = (buf[i]-temp3)*(temp1-temp0)/(temp4-temp3)+temp0;
	         //buf[i] = (((buf[i] - ((buf[i]*255)/(PRINTER_GRAY_STEP*PRINTER_GRAY_STEP)))/PRINTER_GRAY_STEP)*(temp1-temp0))+temp0;
	         if(buf[i] != 255)
	          	new_pixel_value = pixsum[(buf[i]*PRINTER_GRAY_STEP/255)]*255/size;
	         else
	         	new_pixel_value = 255;

	         if (new_pixel_value < 0) {
                new_pixel_value = 0;
            } else if (new_pixel_value > 255) {
                new_pixel_value = 255;
            }
            
            buf[i] = new_pixel_value;	
		}
			
		
	#else //未简化
		
	
		u32 histogram[PRINTER_GRAY_STEP] = {0};
		U8 avr = 256/PRINTER_GRAY_STEP;
		u32 i;
		u8 min = 0xff,max = 0;
		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i])
			{
				min = (min > buf[i])?buf[i]:min;
				max = (max > buf[i])?max:buf[i];
			}
		}

		for(i = 0;i<size;i++){
			if(buf[i]!=255 && buf[i]){
				buf[i] = (255*buf[i])/(max-min);
			}
		}
		
		for (i = 0; i < size; i++) {
	        u8 pixel_value = buf[i]/avr;
	        if (pixel_value > (PRINTER_GRAY_STEP-1))
	        	pixel_value = PRINTER_GRAY_STEP-1;
	        histogram[pixel_value]++;
	    }
	    // 步骤2：计算累计分布函数（CDF）
	    u32 cdf[PRINTER_GRAY_STEP] = {0};
	    cdf[0] = histogram[0];
	    for (i = 1; i < PRINTER_GRAY_STEP; i++) {
	        cdf[i] = cdf[i - 1] + histogram[i];
	        //deg_Printf("%d ",cdf[i]);
	    }

	    // 步骤3：根据CDF进行灰度值映射，得到均衡化后的图像
		//deg_Printf("/n");
		//deg_Printf("===========================/n");
	    for (i = 0; i < size; i++) {
		    u8 pixel_value = buf[i]/avr;
		    // 根据公式计算均衡化后的灰度值，映射到0-255范围
		    //u8 new_pixel_value = (int)(((double)cdf[pixel_value] / size) * 255);
		    u8 new_pixel_value = (cdf[pixel_value]* 255)/size;
		    buf[i] = new_pixel_value;
	    }
	#endif

#endif	
	return;
}

void warpHandle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h, u16 *r)
{
		s32 x, y, i, j, k = buf_w/2, l = buf_h/2, /*m,*/ n, halfh = buf_h / 2, rcount;
		s32 y_offset=0,uv_offset=0,fally_offset=(buf_h - 1) * buf_w,falluv_offset=(buf_h/2-1) * buf_w;
		for(y = 0,rcount = -1; y < halfh; y += 1)
		{
			if(y < halfh)
				rcount++;
			else
				rcount--;
			if(r)
				i = r[rcount];
			else
				i = rcount;
			for(x = 0; x < buf_w; x++)
			{
				if(x < k)
					n = k - 1 -x;
				else
					n = x;

				if((n > l - i) && (n < buf_w - l + i))
				{
					j = (n+i-l)*k/(k - l + i);

					ybuf[y_offset + n]=ybuf[y_offset + j];
					ybuf[fally_offset + n]=ybuf[fally_offset + j];

					if((0==(y%2)))
					{
						s32 set_n,set_j;
						set_n=n&(~0x1);
						set_j=j&(~0x1);

						uvbuf[uv_offset+set_n]=uvbuf[uv_offset + set_j];
						uvbuf[uv_offset+set_n+1]=uvbuf[uv_offset + set_j+1];
						uvbuf[falluv_offset+set_n]=uvbuf[falluv_offset + set_j];
						uvbuf[falluv_offset+set_n+1]=uvbuf[falluv_offset + set_j+1];
					}
				}
			}
			y_offset+=buf_w;
			fally_offset-=buf_w;
			if(1==(y%2))
			{
				uv_offset+=buf_w;
				falluv_offset-=buf_w;
			}
		}
	}

void trilateral_16_window(u8* ybuf,u16 *uvbuf,u16 w,u16 h)
{
	INT32S /*x, y, z,*/ i, j, k, l, resolution = h * w,reverse = ((h >> 1) - 1) * w, starty = w*80*(h/180), startx = 160*(w/320);
	u16 a = w/4, b = w/2, c = w*3/4, d = w/4 - 1, e = w/2 - 1, f = w*3/4 - 1, g = w - 1;
	
	for(j = 0, k = 0, l = (w/20); j <= reverse; j += w,k += 55){
		if(k >= 100){
			k -= 100;
			l++;
		}
		for(i = 0; i < l; i++){
			*(ybuf + reverse - j + g - i) = *(ybuf + reverse - j + f - i) = *(ybuf + reverse - j + e - i) = *(ybuf + reverse - j + d - i) = *(ybuf + j + i + c) = *(ybuf + j + i + b) = *(ybuf + j + i + a) = *(ybuf + j + i) = *(ybuf + j + starty + i + startx);
		}
		if(0 == ((j/w)%2)){
			for(i = 0; i < l; i+=2){
				*(uvbuf + ((reverse - j) >> 2) + ((g - i) >> 1)) = *(uvbuf + ((reverse - j) >> 2) + ((f - i) >> 1)) = *(uvbuf + ((reverse - j) >> 2) + ((e - i) >> 1)) = *(uvbuf + ((reverse - j) >> 2) + ((d - i) >> 1)) = 
					*(uvbuf + (j >> 2) + ((i + c) >> 1)) = *(uvbuf + (j >> 2) + ((i + b) >> 1)) = *(uvbuf + (j >> 2) + ((i + a) >> 1)) = *(uvbuf + (j >> 2) + (i >> 1)) = *(uvbuf + ((j + starty) >> 2) + ((i + startx) >> 1));
			}
		}
	}

	resolution = (resolution >> 1);
	for(j = resolution; j < (resolution << 1); j += w){
		for(i = 0; i < w; i++){
			*(ybuf + j + i) = *(ybuf + j + i - resolution);
		}
		if(0 == ((j/w)%2)){
			for(i = 0; i < w; i+=2){
				*(uvbuf + (j >> 2) + (i >> 1)) = *(uvbuf + ((j - resolution) >> 2) + (i >> 1));
			}
		}
	}
}

INT32U window_stream_flag = 0;
void stream_10_window(u8* ybuf,u16 *uvbuf,u16 w,u16 h)	
{
	INT32S x, y, i, j, k, l, m, n, a, b, c, d, flag, l_is_even;
	if(h == 180)
		l_is_even = w / 4;
	else
		l_is_even = 0;
	k = w / 4;
	l = h / 4;
	m = l * w;
	n = (h - l) * w;
	flag = k -1 - window_stream_flag % k;
	
	for(y = 0,j = l * w; y < m; y += w,j += w << 1){
		
		for(x = flag,i = 2 * k; x < flag + k; x++,i += 2){
			a = y + x;
			b = a + k;
			c = b + k;
			d = c + k;
			if(d > y + w - 1)
				d = d - w;
			ybuf[n + a] = ybuf[n + b] = ybuf[n + c] = ybuf[n + d] =ybuf[a] = ybuf[b] = ybuf[c] = ybuf[d] = ybuf[j + i];
		}
		if(0 == ((y/w)%2)){
			for(x = flag,i = 2 * k; x < flag + k; x+=2,i += 4){
				a = (y >> 2) + (x >> 1);
				b = a + (k >> 1);
				c = b + (k >> 1);
				d = c + (k >> 1);
				if(d > (y >> 2) + ((w - 1) >> 1))
					d = d - (w >> 1);
				uvbuf[(n >> 2) + a] = uvbuf[(n >> 2) + b] = uvbuf[(n >> 2) + c] = uvbuf[(n >> 2) + d] =uvbuf[a] = uvbuf[b] = uvbuf[c] = uvbuf[d] = uvbuf[(j >> 2) + (i >> 1) - l_is_even];
			}
		}
	}
	k = w / 2;
	l = h / 2;
	m = (l + h / 4) * w;
	flag = window_stream_flag % k;
	for(j = h / 4 * w; y < m; y += w,j += w){
		
		for(x = flag,i = w/2; x < flag + k; x++,i++){
			ybuf[x + y] = ybuf[j + i];
		}
		for(x = flag; x < flag + k; x++){
			a = x + k;
			if(a > w - 1)
				a -= w;
			ybuf[a + y] = ybuf[x + y];
		}
		if(0 == ((y/w)%2)){
			for(x = flag,i = w/2; x < flag + k; x+=2,i+=2){
				uvbuf[(y >> 2) + (x >> 1)] = uvbuf[(j >> 2) + (i >> 1)];
			}
			for(x = flag; x < flag + k; x+=2){
				a = ((x + k) >> 1);
				if(a > ((w - 1) >> 1))
					a -= (w >> 1);
				uvbuf[a + (y >> 2)] = uvbuf[(y >> 2) + (x >> 1)];
			}
		}
	}
	//window_stream_flag++;
}


void rismatic_multiwindow(u8* ybuf,u16 *uvbuf,u16 w,u16 h)  //
{
		INT32S a, b, c, x, y, /*i,*/ j, m, n, resolution = h * w,halfh = h / 2 * w;
		j = h*3/4 * w;
		for(y = h/2 * w,b = w/2 - 1,c = h/2 * w,m = w/2 - 1,n = w/2; y < j; y += w,b--,c -= w,m --,n ++){
			for(x = m,a = c; x < n; x++,a += w){
				ybuf[resolution - a + (w - 2) - b] = ybuf[a + b] = ybuf[resolution - y + (w - 2) - x] = ybuf[y + x];
			}
			if(0 == ((y/w)%2)){
				for(x = m,a = c; x < n; x+=2,a += w << 1){
					uvbuf[((resolution - a) >> 2) + (((w - 2) - b) >> 1)] = uvbuf[(a >> 2) + (b >> 1)] = uvbuf[((resolution - y) >> 2) + (((w - 2) - x) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
		}
		j = h * w;
		for(; y < j; y += w,b--,c += w,m ++,n --){
			for(x = m,a = c; x < n; x++,a += w){
				ybuf[resolution - a + (w - 2) - b] = ybuf[a + b] = ybuf[resolution - y + (w - 2) - x] = ybuf[y + x];
			}
			if(0 == ((y/w)%2)){
				for(x = m,a = c; x < n; x+=2,a += w << 1){
					uvbuf[((resolution - a) >> 2) + (((w - 2) - b) >> 1)] = uvbuf[(a >> 2) + (b >> 1)] = uvbuf[((resolution - y) >> 2) + (((w - 2) - x) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
		}
		n = 0;
		for(y = h/9 * w; y < h*8/9 * w; y += w){
		    if(y < halfh)
		        n++;
		    else
		        n--;
		    for(x = 0; x < n; x++){
		        ybuf[y + x] = ybuf[y + w + x]; // 修改为下方1行
		        ybuf[y + (w - 2) - x] = ybuf[y + (w - 2) - w - x];
		    }
		    ybuf[y + (w - 1)] = ybuf[y + (w - 1) - w]; // 修改为下方1行
		    if(0 == ((y/w)%2)){
		        for(x = 0; x < n; x+=2){
		            uvbuf[(y >> 2) + (x >> 1)] = uvbuf[(y >> 2) + ((w + x) >> 1)]; // 下方1行
		            uvbuf[(y >> 2) + (((w - 2) - x) >> 1)] = uvbuf[(y >> 2) + (((w - 2) - w - x) >> 1)];
		        }
		    }
		}
		
		for(y = 0,m = w/2 - 1,n = w/2; y < halfh; y += w, m--, n++){
			for(x = m; x < n; x++){
				a = x - (h >> 1);
				b = x + (h >> 1);
				if(a >= 0){
					ybuf[y + halfh + a] = ybuf[y + x];
					ybuf[resolution - y - halfh + a] = ybuf[resolution - y + x];
				}
				if(b < w){
					ybuf[y + halfh + b] = ybuf[y + x];
					ybuf[resolution - y - halfh + b] = ybuf[resolution - y + x];
				}
			}
			if(0 == ((y/w)%2)){
				for(x = m; x < n; x+=2){
					a = x - (h >> 1);
					b = x + (h >> 1);
					if(a >= 0){
						uvbuf[((y + halfh) >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
						uvbuf[((resolution - y - halfh) >> 2) + (a >> 1)] = uvbuf[((resolution - y) >> 2) + (x >> 1)];
					}
					if(b < w){
						uvbuf[((y + halfh) >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
						uvbuf[((resolution - y - halfh) >> 2) + (b >> 1)] = uvbuf[((resolution - y) >> 2) + (x >> 1)];
					}
				}
			}
		}
}

void hexagon_multiwindow(u8* ybuf,u16 *uvbuf,u16 w,u16 h)
{
	INT32S a, b, c, d, x, y, /*i,*/ j, p1 = (h/180)*27, p2 = p1*2, p3 = p1*3 & (~0x1), p4 = p1*4, m, n, resolution = h * w, evenX, evenY;
	j = (h - p4)/2 * w + p1 * w;
	for(y = (h - p4)/2 * w,m = (w/2 - 1),n = w/2; y < j; y += w,m -= 2,n += 2){
		evenY = ((y/w)%2) ? 0 : 1;
		for(x = m; x < n; x++){
			evenX = x & 1 ? 0 : 1;
			ybuf[y + p3*w + x + p2] = ybuf[y + x];
			ybuf[y + p3*w + x - p2] = ybuf[y + x];
			if(evenX && evenY){
				uvbuf[((y + p3*w) >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				uvbuf[((y + p3*w) >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
			}
			a = x + p4;
			if(a < w){
				ybuf[y + a] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				a += p2;
				if(a < w){
					ybuf[y + p3*w + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y + p3*w) >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
			b = x - p4;
			if(b >= 0){
				ybuf[y + b] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				b -= p2;
				if(b >= 0){
					ybuf[y + p3*w + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y + p3*w) >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
		}
	}



	j += p2 * w;
	for(; y < j; y += w){
		evenY = ((y/w)%2) ? 0 : 1;
		c = y + p3*w;
		d = y - p3*w;
		for(x = m; x < n; x++){
			evenX = x & 1 ? 0 : 1;
			
			if(c < resolution){
				ybuf[c + x + p2] = ybuf[y + x];
				ybuf[c + x - p2] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(c >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					uvbuf[(c >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
			if(d >= 0){
				ybuf[d + x + p2] = ybuf[y + x];
				ybuf[d + x - p2] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(d >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					uvbuf[(d >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}

			a = x + p4;
			if(a < w){
				ybuf[y + a] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
			a = x + p4 + p2;
			if(a < w){
				if(c < resolution){
					ybuf[c + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(c >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
				if(d >= 0){
					ybuf[d + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(d >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}

			
			b = x - p4;
			if(b >= 0){
				ybuf[y + b] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
			b = x - p4 - p2;
			if(b >= 0){
				if(c < resolution){
					ybuf[c + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(c >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
				if(d >= 0){
					ybuf[d + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(d >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
		}
	}

	j += p1 * w;
	for(; y < j; y += w,m += 2,n -= 2){
		evenY = ((y/w)%2) ? 0 : 1;
		for(x = m; x < n; x++){
			evenX = x & 1 ? 0 : 1;
			ybuf[y - p3*w + x + p2] = ybuf[y + x];
			ybuf[y - p3*w + x - p2] = ybuf[y + x];
			if(evenX && evenY){
				uvbuf[((y - p3*w) >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				uvbuf[((y - p3*w) >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
			}
			a = x + p4;
			if(a < w){
				ybuf[y + a] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				a += p2;
				if(a < w){
					ybuf[y - p3*w + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y - p3*w) >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
			b = x - p4;
			if(b >= 0){
				ybuf[y + b] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				b -= p2;
				if(b >= 0){
					ybuf[y - p3*w + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y - p3*w) >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
		}
	}
}


