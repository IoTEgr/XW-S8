/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                     MULTIMEDIA LAYER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : USER_MEDIA.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is user MEDIA header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
* 2017-04-24  : mark
*      <1>.Modify memory define,to hal_sys.c,share memory width sytem other application.
******************************************************************************/
#include "media.h"
#include "multimedia.h"

#include "../ax32_platform_demo/application.h"





//---------------memory define for multimedia------------------------------------------------

//SECTION(".big_buffer") ALIGNED(32) INT8U aviIdx1Cache[AVI_CFG_IDX1_BUFF_SIZE];  // for avi record & play,cache idx1 table

//SECTION(".big_buffer") ALIGNED(32) INT8U jpegCache[JPEG_CFG_CACHE_SIZE*JPEG_CFG_CACHE_NUM]; // for jpeg decode cache

//SECTION(".big_buffer") ALIGNED(32) INT8U audioCache[AUDIO_CFG_BUFFER_SIZE*AUDIO_CFG_BUFFER_NUM]; // for audio decode cache

//SECTION(".big_buffer") ALIGNED(64) INT8U thumbnailYBuffer[THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT],thumbnailUVBuffer[THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT/2];

MSG_T audsIStack[AUDIO_CFG_BUFFER_NUM],audsTStack[AUDIO_CFG_BUFFER_NUM],audsBStack[AUDIO_CFG_BUFFER_NUM],vidsIStack[VIDEO_CFG_BUFFER_NUM],vidsBStack[VIDEO_CFG_BUFFER_NUM],jpegIStack[JPEG_CFG_CACHE_NUM],jpegBStack[JPEG_CFG_CACHE_NUM];
//------------memory share width other application.[mark modify .2017/04/24]
INT8U *jpegCache;// = (INT8U *)&halSysMemMJPEGBuffer;
INT8U *audioCache;// = (INT8U *)&halSysMemPCMBuffer;
INT8U *aviIdx1Cache;// = halSysMemIdxBuffer;  
INT8U *aviIdx1CacheB;
// thumbnail using the frame of jpeg cache buffer
INT8U *thumbnailYBuffer;// = ((INT8U *)&halSysMemMJPEGBuffer)+JPEG_CFG_CACHE_SIZE;
INT8U *thumbnailUVBuffer;// = (((INT8U *)&halSysMemMJPEGBuffer)+JPEG_CFG_CACHE_SIZE+THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT);
#if 0

typedef struct SHARE_MEM_S
{
	void *addr;
	INT32U size;
}SHARE_MEM_T;

#define  SHARE_MEM_NUM     3
static SHARE_MEM_T SHARE_MEM_NODE[SHARE_MEM_NUM]=
{
	{// avi idx cache buffer
		.addr = (void *)&halSysMemIdxBuffer, // aviIdx1Cache
		.size = AVI_CFG_IDX1_BUFF_SIZE,
    },
    {// jpeg file cache buffer
		.addr = (void *)&halSysMemMJPEGBuffer, // jpegCache
		.size = JPEG_CFG_CACHE_SIZE*JPEG_CFG_CACHE_NUM,
    },
    {// audio cache buffer
		.addr = (void *)&halSysMemPCMBuffer,// audioCache,
		.size = AUDIO_CFG_BUFFER_SIZE*AUDIO_CFG_BUFFER_NUM,
    }

};

int multimediaInit(void)
{
    return 0;
}
/*******************************************************************************
* Function Name  : shareMemMalloc
* Description    : memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : INT32U size : need memory size 
* Output         : none
* Return         : 
*******************************************************************************/
void *shareMemMalloc(INT32U size)
{
	int idx,i;

	idx = SHARE_MEM_NUM;
	for(i=0;i<SHARE_MEM_NUM;i++)
	{
		if((SHARE_MEM_NODE[i].size&0x80000000)==0)
		{
			if(size == SHARE_MEM_NODE[i].size)
				break;
			else if(size<SHARE_MEM_NODE[i].size)
			{
				if(idx == SHARE_MEM_NUM)
					idx = i;
			}
		}
	}

	if(i>=SHARE_MEM_NUM && idx<SHARE_MEM_NUM)
		i = idx;
	if(i<SHARE_MEM_NUM)
	{
		SHARE_MEM_NODE[i].size|=0x80000000;
		return SHARE_MEM_NODE[i].addr;
	}

	return NULL;
}
/*******************************************************************************
* Function Name  : shareMemFree
* Description    : memory share for other application,memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : void *mm : memory addr
* Output         : none
* Return         : 
*******************************************************************************/
void shareMemFree(void *mm)
{
	int i;

	for(i=0;i<SHARE_MEM_NUM;i++)
	{
		if(SHARE_MEM_NODE[i].addr == mm)
			SHARE_MEM_NODE[i].size&=~0x80000000;
	}	
}


#endif


u8 jpg_decode_buf(u32 jpg_id,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	Image_ARG_T arg;
	u16 dec_w,dec_h;
	arg.target.width=w;
	arg.target.height=h;
	arg.yout  = ybuf;
	arg.uvout = uvbuf;
	arg.media.type = MEDIA_SRC_NVFS;
	arg.media.src.fd = (FHANDLE)jpg_id;
    arg.wait = 1;  // wait decode end
	if(imageDecodeStart(&arg)<0)
	{
		deg_Printf("image decode fail.\n");
		return 1;
	}

	imageDecodeGetResolution(&dec_w,&dec_h);
	deg_Printf("dec_w=%d,dec_h=%d\n",dec_w,dec_h);

	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}

u8 jpg_buf_decode_buf(u8* jpg_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	Image_ARG_T arg;
	u16 dec_w,dec_h;
	arg.target.width=w;
	arg.target.height=h;
	arg.yout  = ybuf;
	arg.uvout = uvbuf;
	arg.media.type = MEDIA_SRC_RAM;
	arg.media.src.buff = (u32)jpg_buf;
    arg.wait = 1;  // wait decode end
	if(imageDecodeStart(&arg)<0)
	{
		deg_Printf("image decode fail.\n");
		return 1;
	}

	imageDecodeGetResolution(&dec_w,&dec_h);
	deg_Printf("dec_w=%d,dec_h=%d\n",dec_w,dec_h);

	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}

#if 0
disp_frame_t * setting_showbuf(u32 bk_id, u32 setting_id)
{
	u8 *setting_bk_buf;
	u16 setting_bk_w,setting_bk_h;
	
	u8 *setting_id_buf=NULL;
	u16 id_w,id_h;

	u8 *rgb24_buf;

	hal_lcdGetResolution(&setting_bk_w,&setting_bk_h);
	setting_bk_buf=hal_sysMemMalloc(setting_bk_w*setting_bk_h*3/2,32);
	if(NULL!=setting_bk_buf)
	{
		disp_frame_t *p_lcd_buffer=NULL;
		jpg_decode_buf(bk_id,setting_bk_buf,setting_bk_buf+setting_bk_w*setting_bk_h,setting_bk_w,setting_bk_h);

		id_w=(64+0x1)&(~0x1);	// bmp must 2pixel align
		id_h=(64+0x1)&(~0x1);
		rgb24_buf=hal_sysMemMalloc(id_w*id_h*3,32);
		if(NULL==rgb24_buf)
		{
			deg_Printf("rgb24 mem err!\n");
			goto SETTING_SHOWBUF_EXIT;
		}
		
		setting_id_buf=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(NULL!=setting_id_buf)
		{
			bmp24_to_yuv420_buf(setting_id,rgb24_buf,setting_id_buf,setting_id_buf+id_w*id_h,id_w,id_h);
		}
		else
		{
			deg_Printf("mem err!\n");
			goto SETTING_SHOWBUF_EXIT;
		}


		//disp_frame_t *p_lcd_buffer;
		p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
		if(p_lcd_buffer)
		{
			u16 lcd_w,lcd_h;
			hal_lcdGetResolution(&lcd_w,&lcd_h);
			//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
			hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

			//==draw bk==
			memcpy(p_lcd_buffer->y_addr,setting_bk_buf,setting_bk_w*setting_bk_h*3/2);
			//==draw setting id==
			yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,(320-64)/2,32,id_w,id_h,setting_id_buf,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);

			//==show==
			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
		}
		else
		{
			deg_Printf("video buf null!\n");
		}

SETTING_SHOWBUF_EXIT:
		//==free==
		if(setting_bk_buf)
		hal_sysMemFree(setting_bk_buf);
		if(setting_id_buf)
		hal_sysMemFree(setting_id_buf);
		if(rgb24_buf)
		hal_sysMemFree(rgb24_buf);

		return p_lcd_buffer;
	}
	else
	{
		deg_Printf("mem err!\n");
		return NULL;
	}
}
#endif


//==========
#define BYTE_TUNE(r) (r<0?0:(r>255?255:r)) 

//==rgb2yuv table==
#define YUV_RGB_IN_HEAP 1
#if YUV_RGB_IN_HEAP==0

static  u8 Y_R[256]; 
static  u8 Y_G[256]; 
static  u8 Y_B[256]; 

static  u8 U_R[256]; 
static  u8 U_G[256]; 
static  u8 U_B[256]; 

static  u8 V_R[256]; 
static  u8 V_G[256]; 
static  u8 V_B[256]; 
#endif

void yuv_rgb_table()
{ 
#if YUV_RGB_IN_HEAP==0

	u32 i; 
	static u8 init_one_time=0;
	if(0==init_one_time)
	{
		init_one_time=1;
		for(i = 0;i<256;i++) 
		{ 
		   //==rgb2yuv table==
		   Y_R[i]=(i*1224)>>12;
		   Y_G[i]=(i*2404)>>12;
		   Y_B[i]=(i*467)>>12;

		   U_R[i]=(i*602)>>12;
		   U_G[i]=(i*1183)>>12;
		   U_B[i]=(i*1785)>>12;

		   V_R[i]=(i*2519)>>12;
		   V_G[i]=(i*2109)>>12;
		   V_B[i]=(i*409)>>12;
		} 
	}
	#endif
} 


#if YUV_RGB_IN_HEAP
static void yuv_rgb_table_init(u8* Y_R,u8* Y_G,u8* Y_B,u8* U_R,u8* U_G,u8* U_B,u8* V_R,u8* V_G,u8* V_B)
{ 
	u32 i; 

	for(i = 0;i<256;i++) 
	{ 
		//==rgb2yuv table==
		Y_R[i]=(i*1224)>>12;
		Y_G[i]=(i*2404)>>12;
		Y_B[i]=(i*467)>>12;

		U_R[i]=(i*602)>>12;
		U_G[i]=(i*1183)>>12;
		U_B[i]=(i*1785)>>12;

		V_R[i]=(i*2519)>>12;
		V_G[i]=(i*2109)>>12;
		V_B[i]=(i*409)>>12;
	} 
} 
#endif

static void rgb24_to_yuv420(u8* rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	u8 r,g,b,r1,g1,b1; 
	s16 y,u,/*v,*/y1,/*u1,*/v1;
	u8 *rgb_line_buf,*temp_buf;
	u32 i,j;
	u32 line_align;
	#if YUV_RGB_IN_HEAP
	u8 *Y_R,*Y_G,*Y_B,*U_R,*U_G,*U_B,*V_R,*V_G,*V_B;
	u8 *yuv_rgb;
	yuv_rgb = hal_sysMemMalloc(256*9, 64);
	if (yuv_rgb == NULL) {
		deg_Printf("[%s]malloc fail\n",__func__);
		return;
	}
		debg("[%s]entry\n",__func__);
	Y_R = yuv_rgb;
	Y_G = Y_R + 256;
	Y_B = Y_G + 256;
	U_R = Y_B + 256;
	U_G = U_R + 256;
	U_B = U_G + 256;
	V_R = U_B + 256;
	V_G = V_R + 256;
	V_B = V_G + 256;
	yuv_rgb_table_init(Y_R, Y_G, Y_B, U_R, U_G, U_B, V_R, V_G, V_B);
	#endif
	line_align=(w*3+3)&~0x3;	// 4bytes align

	rgb_line_buf=rgb_buf+(h-1)*line_align; // last line
	for(j=0;j<h;j+=2)
	{
		temp_buf=rgb_line_buf;
		for(i=0;i<w;i+=2)
		{ 
			 r = *temp_buf++; 
			 g = *temp_buf++; 
			 b = *temp_buf++; 
			 r1 = *temp_buf++; 
			 g1 = *temp_buf++; 
			 b1 = *temp_buf++; 

			 y = Y_R[r]+Y_G[g]+Y_B[b];		
			 //u = (U_B[b]-U_R[r]-U_G[g])+128; 
			 //v = (V_R[r]-V_G[g]-V_B[b])+128; 
			 u = (U_B[b]-U_R[r]-U_G[g])+128; 

			 y1 = Y_R[r1]+Y_G[g1]+Y_B[b1]; 
			 //u1 = (U_B[b1]-U_R[r1]-U_G[g1])+128;
			 //v1 = (V_R[r1]-V_G[g1]-V_B[b1])+128;
			 v1 = (V_R[r1]-V_G[g1]-V_B[b1])+128;

			 *ybuf++ = BYTE_TUNE(y);
			 *uvbuf++ = BYTE_TUNE(u);
			 *ybuf++ = BYTE_TUNE(y1);
			 *uvbuf++ = BYTE_TUNE(v1);
		}

		rgb_line_buf=rgb_line_buf-line_align;
		temp_buf=rgb_line_buf;
		for(i=0;i<w;i+=2)
		{ 
			 r = *temp_buf++; 
			 g = *temp_buf++; 
			 b = *temp_buf++; 
			 r1 = *temp_buf++; 
			 g1 = *temp_buf++; 
			 b1 = *temp_buf++; 
		
			 y = Y_R[r]+Y_G[g]+Y_B[b];		
			 //v = (V_R[r]-V_G[g]-V_B[b])+128; 
			 y1 = Y_R[r1]+Y_G[g1]+Y_B[b1]; 
			 //u1 = (U_B[b1]-U_R[r1]-U_G[g1])+128; 
			 *ybuf++ = BYTE_TUNE(y);
			 *ybuf++ = BYTE_TUNE(y1);
		}
		rgb_line_buf=rgb_line_buf-line_align;

	}
		#if YUV_RGB_IN_HEAP
		hal_sysMemFree(yuv_rgb);
		#endif
}

u8 bmp24_to_yuv420_buf(u32 id,u8 *rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	u32	addr,size;
	u32 line_align;
	line_align=(w*3+3)&~0x3;	// 4bytes align

	addr= nv_open(id);
	if(addr<0)
	{
		deg_Printf("read id err\n");
		return 1;
	}
	size = nv_size(id);
	if(size<54)
	{
		deg_Printf("size err\n");
		return 1;
	}
	size=size-54;	// offset bmp header
	
	if(size>line_align*h)	// not over rgb_buf size
	{
		size=line_align*h;
	}
	else if(size<w*h*3)
	{
		deg_Printf("bmp format err?\n");
	}
	
	nv_read(addr+54,rgb_buf,size);	// offset bmp header
	//deg_Printf("data size=%d,w*h*3=%d\n",size-54,w*h*3);
	rgb24_to_yuv420(rgb_buf,ybuf,uvbuf,w,h);
	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}



//==tips: draw_x,drawy,src_w,src_h is 2X size,X is pixel num==
void yuv420_draw_buf(u8 *dst_ybuf,u16 dst_w,u16 dst_h,s16 draw_x,s16 draw_y,u16 draw_w,u16 draw_h,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y,u8 alpha_uv)
{
	u16 i,j;
	u16 draw_offset_x,draw_offset_y;
	u8 *dy,*duv,*sy,*suv;

	draw_x&=~0x1;		// align
	draw_y&=~0x1;		// align
	draw_w&=~0x1;		// align
	draw_h&=~0x1;		// align

	if((draw_x+draw_w<=0)||(draw_y+draw_h<=0)||(draw_x>=dst_w)||(draw_y>=dst_h))	// draw nothing
	{
		return;
	}

	if(draw_x<0)
	{
		draw_offset_x=-draw_x;
		draw_x=0;
		//deg_Printf("draw_offset_x=%d\n",draw_offset_x);
	}
	else
	{
		draw_offset_x=0;
	}
	
	if(draw_y<0)
	{
		draw_offset_y=-draw_y;
		draw_y=0;
		//deg_Printf("draw_offset_y=%d\n",draw_offset_y);
		// not finish!!
	}
	else
	{
		draw_offset_y=0;
	}

	if(draw_x+src_w>dst_w)
	{
		draw_w=dst_w-draw_x;
	}
	else
	{
		draw_w=src_w-draw_offset_x;
	}

	if(draw_y+src_h>dst_h)
	{
		draw_h=dst_h-draw_y;
	}
	else
	{
		draw_h=src_h-draw_offset_y;
	}
	
	//deg_Printf("draw_x=%d,draw_y=%d,draw_w=%d,draw_h=%d\n",draw_x,draw_y,draw_w,draw_h);
	dy=dst_ybuf+dst_w*draw_y+draw_x;
	duv=dst_ybuf+dst_w*dst_h+dst_w*draw_y/2+draw_x;
	sy=src_ybuf;
	suv=src_ybuf+src_w*src_h;

	//==draw ==
	for(j=0;j<draw_h;j+=2)
	{
		for(i=0;i<draw_w;i++)
		{
			if((*(sy+i+draw_offset_x)!=alpha_y)||(*(suv+i+draw_offset_x)!=alpha_uv))
			{
				*(dy+i)=*(sy+i+draw_offset_x);
				*(duv+i)=*(suv+i+draw_offset_x);
			}
		}

		dy+=dst_w;
		sy+=src_w;

		for(i=0;i<draw_w;i++)
		{
			if((*(sy+i+draw_offset_x)!=alpha_y)||(*(suv+i+draw_offset_x)!=alpha_uv))
			{
				*(dy+i)=*(sy+i+draw_offset_x);
				*(duv+i)=*(suv+i+draw_offset_x);
			}
		}
		
		dy+=dst_w;
		sy+=src_w;
		duv+=dst_w;
		suv+=src_w;

	}

}



//====pencil handle===
#define SUPPORT_PENCIL_HANDLE 0
#if SUPPORT_PENCIL_HANDLE

static u32 grad[63][63] = {
#if 1	// 1.4 31

{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5, 4, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5, 6, 7, 7, 7, 7, 6, 6, 5, 4, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 6, 8, 9, 10, 11, 10, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 6, 9, 11, 13, 15, 15, 14, 13, 12, 10, 9, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 9, 12, 15, 18, 20, 20, 19, 17, 15, 12, 10, 9, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 5, 8, 11, 15, 20, 24, 27, 27, 24, 21, 17, 14, 12, 10, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 9, 13, 18, 24, 30, 31, 31, 29, 24, 20, 16, 13, 11, 9, 8, 6, 5, 4, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 7, 10, 15, 20, 27, 31, 31, 31, 31, 26, 21, 17, 14, 12, 10, 8, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 5, 7, 11, 15, 20, 27, 31, 31, 31, 31, 26, 21, 18, 15, 12, 10, 9, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 5, 7, 10, 14, 19, 24, 29, 31, 31, 28, 24, 21, 17, 14, 12, 10, 9, 7, 6, 5, 4, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 5, 7, 10, 13, 17, 21, 24, 26, 26, 24, 22, 19, 16, 14, 12, 10, 9, 7, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 4, 6, 9, 12, 15, 17, 20, 21, 21, 21, 19, 17, 15, 13, 11, 10, 8, 7, 6, 5, 5, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 6, 8, 10, 12, 14, 16, 17, 18, 17, 16, 15, 13, 12, 10, 9, 8, 7, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5, 7, 9, 10, 12, 13, 14, 15, 14, 14, 13, 12, 11, 10, 8, 7, 7, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 9, 10, 11, 12, 12, 12, 12, 11, 10, 10, 9, 8, 7, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 9, 8, 8, 7, 6, 6, 5, 5, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 7, 7, 7, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 4, 5, 5, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

#endif
};
#endif


void yuv420_pencil_sketch(u8 *ybuf,u8 *uvbuf,u16 w,u16 h,u8 color)
{
#if SUPPORT_PENCIL_HANDLE

	s32 a, b, x, y,grad_x, grad_y,ybuf_size;
	u8 weight;
	u8 *yb;
	ybuf_size=w*h;
	yb=hal_sysMemMalloc(ybuf_size,32);
	if(ybuf)
	{
		memcpy(yb,ybuf,ybuf_size);

		for(y = 0; y < ybuf_size; y += w)
		{
			for(x = 0; x < w; x++)
			{
				if(x==0)
				{
					grad_y = ((((*(yb+y+1)) >> 3)-((*(yb+y)) >> 3))<<1) + 31;
				}
				else if(x==(w-1))
				{
					grad_y = ((((*(yb+y+x)) >> 3)-((*(yb+y+x-1)) >> 3))<<1) + 31;
				}
				else if((x==1)||(x==(w-2)))
				{
					grad_y = (((*(yb+y+x+1)) >> 3)-((*(yb+y+x-1)) >> 3)) + 31;
				}
				else
				{
					a = ((*(yb+y+x+2)) >> 3)-((*(yb+y+x-2)) >> 3);
					b = ((*(yb+y+x+1)) >> 3)-((*(yb+y+x-1)) >> 3);
					grad_y = ((a + b + (a << 1)) >> 2) + 31;
				}
				
				if(y==0)
				{
					grad_x = ((((*(yb+w+x)) >> 3)-((*(yb+x)) >> 3)) << 1) + 31;
				}
				else if(y==ybuf_size-w)
				{
					grad_x = ((((*(yb+y+x)) >> 3)-((*(yb+y-w+x)) >> 3)) << 1) + 31;
				}
				else if((y==1)||(y==ybuf_size-w-w))
				{
					grad_x = (((*(yb+y+w+x)) >> 3)-((*(yb+y-w+x)) >> 3)) + 31;
				}
				else
				{
					a = ((*(yb + y + w + w + x)) >> 3) - ((*(yb+y-w-w+x)) >> 3);
					b = ((*(yb + y + w + x)) >> 3) - ((*(yb+y-w+x)) >> 3);
					grad_x = ((a + b + (a << 1)) >> 2) + 31;
				}

/*
				if(grad_x>62)
				{
					deg_Printf("x:%d\n",grad_x);
					grad_x=62;
				}
				if(grad_y>62)
				{
					deg_Printf("y:%d\n",grad_y);
					grad_y=62;
				}
*/
				weight = grad[grad_x][grad_y];
				ybuf[y + x] = (weight << 3);
			}
		}
	}
	else
	{
		deg_Printf("mem err\n");
		return ;
	}

	if(0==color)
	{
		if(NULL!=uvbuf)
		{
			memset(uvbuf,0x80,ybuf_size/2);
			ax32xx_sysDcacheFlush((u32)uvbuf,w*h/2);
		}
	}

	ax32xx_sysDcacheFlush((u32)ybuf,w*h);

	if(yb)
	{
		hal_sysMemFree(yb);
	}
	deg_Printf("pass pencil handle\n");
	#endif
}


//==cam effect ==
static disp_frame_t * software_handle_frame;

void yuv420_software_set_handle_frame(disp_frame_t * frame)
{
	software_handle_frame=frame;
}

disp_frame_t * yuv420_software_get_handle_frame()
{
	return  software_handle_frame;
}

void yuv420_effect_handle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h)
{
	u16 i,j;

	if(CAM_EFFECT_0==SysCtrl.photo_effect_idx)
	{
		u8 *y,*uv;
		u16 half_w=buf_w>>1;
		y=ybuf;
		uv=uvbuf;
		for(j=0;j<buf_h;j+=2)
		{
			for(i=0;i<half_w;i++)
			{
				*(y+half_w-i-1)=*(y+half_w+i);
			}
			y+=buf_w;
			for(i=0;i<half_w;i++)
			{
				*(y+half_w-i-1)=*(y+half_w+i);
			}

			for(i=0;i<half_w;i+=2)
			{
				*(uv+half_w-i-2)=*(uv+half_w+i);
				*(uv+half_w-i-1)=*(uv+half_w+i+1);
			}

			y+=buf_w;
			uv+=buf_w;
		}
	}
	else if(CAM_EFFECT_1==SysCtrl.photo_effect_idx){
		warpHandle(ybuf, uvbuf, buf_w, buf_h, NULL);
	}
	else if(CAM_EFFECT_2==SysCtrl.photo_effect_idx){
		if(180 == buf_h){
			u16 r[] = {49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 62, 63, 64, 65, 66, 66, 67, 68, 68, 69, 70, 70, 71, 72, 72, 73, 73, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 79, 80, 80, 81, 81, 82, 82, 82, 83, 83, 83, 84, 84, 84, 85, 85, 85, 85, 86, 86, 86, 86, 87, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
		else if(720 == buf_h){
			u16 r[] = {197, 198, 199, 200, 201, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 235, 236, 237, 238, 239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 248, 249, 250, 251, 252, 253, 253, 254, 255, 256, 257, 257, 258, 259, 260, 260, 261, 262, 263, 264, 264, 265, 266, 266, 267, 268, 269, 269, 270, 271, 272, 272, 273, 274, 274, 275, 276, 276, 277, 278, 278, 279, 280, 280, 281, 282, 282, 283, 284, 284, 285, 286, 286, 287, 287, 288, 289, 289, 290, 290, 291, 292, 292, 293, 293, 294, 295, 295, 296, 296, 297, 297, 298, 299, 299, 300, 300, 301, 301, 302, 302, 303, 303, 304, 305, 305, 306, 306, 307, 307, 308, 308, 309, 309, 310, 310, 311, 311, 312, 312, 313, 313, 314, 314, 314, 315, 315, 316, 316, 317, 317, 318, 318, 319, 319, 319, 320, 320, 321, 321, 322, 322, 322, 323, 323, 324, 324, 324, 325, 325, 326, 326, 326, 327, 327, 328, 328, 328, 329, 329, 330, 330, 330, 331, 331, 331, 332, 332, 332, 333, 333, 333, 334, 334, 334, 335, 335, 335, 336, 336, 336, 337, 337, 337, 338, 338, 338, 339, 339, 339, 340, 340, 340, 340, 341, 341, 341, 342, 342, 342, 342, 343, 343, 343, 343, 344, 344, 344, 345, 345, 345, 345, 346, 346, 346, 346, 346, 347, 347, 347, 347, 348, 348, 348, 348, 349, 349, 349, 349, 349, 350, 350, 350, 350, 350, 351, 351, 351, 351, 351, 352, 352, 352, 352, 352, 352, 353, 353, 353, 353, 353, 353, 354, 354, 354, 354, 354, 354, 355, 355, 355, 355, 355, 355, 355, 355, 356, 356, 356, 356, 356, 356, 356, 356, 357, 357, 357, 357, 357, 357, 357, 357, 357, 357, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
	}
	else if(CAM_EFFECT_3==SysCtrl.photo_effect_idx){
		if(180 == buf_h){
			u16 r[] = {40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 23, 22, 21, 21, 20, 19, 19, 18, 17, 17, 16, 16, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 10, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
		else if(720 == buf_h){
			u16 r[] = {162, 161, 160, 159, 158, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 124, 123, 122, 121, 120, 119, 118, 117, 116, 116, 115, 114, 113, 112, 111, 111, 110, 109, 108, 107, 106, 106, 105, 104, 103, 102, 102, 101, 100, 99, 99, 98, 97, 96, 95, 95, 94, 93, 93, 92, 91, 90, 90, 89, 88, 87, 87, 86, 85, 85, 84, 83, 83, 82, 81, 81, 80, 79, 79, 78, 77, 77, 76, 75, 75, 74, 73, 73, 72, 72, 71, 70, 70, 69, 69, 68, 67, 67, 66, 66, 65, 64, 64, 63, 63, 62, 62, 61, 60, 60, 59, 59, 58, 58, 57, 57, 56, 56, 55, 54, 54, 53, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 45, 44, 44, 43, 43, 42, 42, 41, 41, 40, 40, 40, 39, 39, 38, 38, 37, 37, 37, 36, 36, 35, 35, 35, 34, 34, 33, 33, 33, 32, 32, 31, 31, 31, 30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25, 25, 24, 24, 24, 23, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 19, 19, 19, 18, 18, 18, 17, 17, 17, 17, 16, 16, 16, 16, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 13, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
	}
	else if(CAM_EFFECT_4==SysCtrl.photo_effect_idx){
		trilateral_16_window(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_5==SysCtrl.photo_effect_idx){
		stream_10_window(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_6==SysCtrl.photo_effect_idx){
		rismatic_multiwindow(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_7==SysCtrl.photo_effect_idx){
		hexagon_multiwindow(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	ax32xx_sysDcacheFlush((u32)ybuf,buf_w*buf_h*3/2);
}

u32 get_random_addr()	//��ȡ������е�ַ
{
	return SPI_FLASH_SIZE-512;
}
u32 get_accredit_addr() //��ȡ������Ȩ��ַ
{
	return SPI_FLASH_SIZE-256;
}



void photo_effect_ctrl(u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	deg_Printf("in %s:w[%d] h[%d] modeSwitch[%d]\n",__func__,w,h,SysCtrl.photo_mode_switch);

	if(2==SysCtrl.photo_mode_switch)	// effect mode
	{
		yuv420_effect_handle(ybuf,uvbuf,w,h);

	}else if(SysCtrl.photo_mode_switch>=4)
	{
		u8 color;
		if(SysCtrl.photo_mode_switch==4)
		{
			color=0;
		}
		else
		{
			color=1;
		}
		yuv420_pencil_sketch(ybuf,uvbuf,w,h,color);
	}

}












