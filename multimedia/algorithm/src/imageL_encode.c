/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA IMAGE ENCODE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : image_encode.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
//#include "../../media.h"
#include "../../multimedia.h"
#include "../../../hal/inc/hal_mjpeg.h"
#include "../../../ax32_platform_demo/application.h"
#include "../../watermark/image_watermark.h"


#define RE_USEMJPG_SAPCE  1		//1- en:reuse mjpbuf space ,use use more time ;  0-disable

#if MEDIA_CFG_IMAGE_ENCODE_EN >0

#define  MJPEG_TYPE_VIDEO      0
#define  MJPEG_TYPE_PHOTO      1
#define  MJPEG_TYPE_UVC         2

#define  MANAGER_NAME_LEN     19

static INT32U jpegHeaderSize;
static const INT8U jpegEoi = 0xd9;
static INT8U jpegRst;


//#define SDRAM_SIZE SDRAM_SIZE_8M
static struct
{
	INT8U *buf;
	int fd;
	UINT lenTotal;
	UINT lenUsed;
} wrCache;

static int w_test_open(int fd, UINT len)
{
	wrCache.fd = fd;
	wrCache.lenTotal = len;
	wrCache.lenUsed = 0;
	wrCache.buf = hal_sysMemMalloc(wrCache.lenTotal, 64);
	if (wrCache.buf)
		return 0;
	else
		return -9;
}

static void w_test_flush(void)
{
	UINT dataLen =0 ;
/*	const UINT WRITE_LEN = 512;
	while (wrCache.lenUsed > WRITE_LEN)
	{
		write(wrCache.fd, wrCache.buf + dataLen, WRITE_LEN);
		dataLen += WRITE_LEN;
		wrCache.lenUsed -= WRITE_LEN;
	}*/
	write(wrCache.fd, wrCache.buf + dataLen, wrCache.lenUsed);
	wrCache.lenUsed = 0;
}

static int w_test(int fd,const void *buff,UINT len)
{
	if (wrCache.buf == NULL)
		return -6;
	while (wrCache.lenUsed + len >= wrCache.lenTotal)
	{
		UINT lenPart = wrCache.lenTotal - wrCache.lenUsed;
		memcpy(wrCache.buf + wrCache.lenUsed, buff, lenPart);
		buff += lenPart;
		len -= lenPart;
		wrCache.lenUsed = wrCache.lenTotal;
		w_test_flush();
	}
	memcpy(wrCache.buf + wrCache.lenUsed, buff, len);
	wrCache.lenUsed += len;
	return 0;
}

static void w_test_close(void)
{
	if (wrCache.buf == NULL)
		return;
	w_test_flush();
	hal_sysMemFree(wrCache.buf);
	wrCache.buf = NULL;
}


int hal_mjpegPhotoQuadCfgQuarter(u16 win_w, u16 win_h, u8 quality)
{
	mjpegEncCtrl.mjpeg_width = win_w;
	mjpegEncCtrl.mjpeg_height = win_h;
	quality = hal_mjpegQualityCheck(quality);
	
	//ax32xx_mjpeg_inlinebuf_init((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer);
	ax32xx_mjpegEncodeSizeSet(HAL_CFG_MJPEG_WIDTH, HAL_CFG_MJPEG_HEIGHT, win_w, win_h);

	mjpegEncCtrl.curLen = mjpegEncCtrl.mjpsize;

	debg("PRE_JPCON1:%x,%x\n",quality,(PRE_JPCON1 >> 23)&0x0f);
	return 0;
}



//static int jpegJfxxOffset;


int hal_mjpegStitchQuadThumb(FHANDLE whole, u8 *thumb, u32 thumbSize)
{
	int ret = 0;
	INT8U app[] = {0xff, 0xe9, 0, 10, 'J', 'R', 'X', '\0', 0, 0, 0, 0};
	INT32U thumbOffset;
	w_test_flush();
	thumbOffset = fs_tell(whole);
	ret = w_test(whole, thumb, thumbSize);
	if (ret < 0)
		return ret;
	deg("thumbOffset=0x%x\n", thumbOffset);
	app[8] = thumbOffset >> 24 & 0xff;
	app[9] = thumbOffset >> 16 & 0xff;
	app[10] = thumbOffset >> 8 & 0xff;
	app[11] = thumbOffset & 0xff;
	ret = w_test(whole, app, sizeof app);
	if (ret < 0)
		return ret;
	return ret;
}
#if 0
int hal_mjpegStitchQuadThumb1(FHANDLE whole, u8 *upperLeft, u32 upperLeftSize, u8 *thumb, u32 thumbSize)
{
	int ret = 0;
	int i;
	u16 jfifLen, jfxxLen;
	u8 jfxx[] = {0xff, 0xe0, 0, 0, 'J', 'F', 'X', 'X', '\0', 0x10};
	for (i = 0; i < upperLeftSize; i++)
	{
		if (upperLeft[i] == 0xff && upperLeft[i + 1] == jfxx[1])
		{
			break;
		}
	}
	jfifLen = upperLeft[i + 2] << 8 | upperLeft[i + 3];
	jpegJfxxOffset = i + 2 + jfifLen;

	ret = w_test(whole, upperLeft, jpegJfxxOffset);
	
	return ret;
}
#endif
int hal_mjpegStitchQuadUpper(FHANDLE whole, u8 *upper[], u32 upperSize[])
{
	int ret = 0;
	INT8U marker[2];
	int offset[2];
	unsigned side;
	bool sos = false, eoi = false;
	int i;
	int jpegJfxxOffset = 0;
#if 1	
	u16 jfifLen;
	
	u8 jfxx[] = {0xff, 0xe0, 0, 0, 'J', 'F', 'X', 'X', '\0', 0x10};
	
	for (i = 0; i < upperSize[0]; i++)
	{
		if (upper[0][i] == 0xff && upper[0][i + 1] == jfxx[1])
		{
			break;
		}
	}
	
	jfifLen = upper[0][i + 2] << 8 | upper[0][i + 3];

	//ret = w_test(whole, upper[0], jpegJfxxOffset);
	
	jpegJfxxOffset = i + 2 + jfifLen;

	ret = w_test(whole, upper[0], jpegJfxxOffset);
#endif	
	offset[0] = jpegJfxxOffset;
	
	do
	{
		u8 *header;
		u16 headerLen;
		u16 width, height;
		header = upper[0] + offset[0];
		headerLen = header[2] << 8 | header[3];
		switch (header[1])
		{
		case 0xc0:
			height = header[5] << 8 | header[6];
			height *= 2;
			header[5] = height >> 8;
			header[6] = height & 0xff;
			width = header[7] << 8 | header[8];
			width *= 2;
			header[7] = width >> 8;
			header[8] = width & 0xff;
			break;
		case 0xda:
			sos = true;
			break;
		default:
			break;
		}
		offset[0] += 2 + headerLen;
	} while (!sos);
	deg("StitchUpper:SOS %d\n", offset[0]);
	ret = w_test(whole, upper[0] + jpegJfxxOffset, offset[0] - jpegJfxxOffset);
	if (ret < 0)
		return ret;

	jpegHeaderSize = offset[0];

	offset[1] = jpegHeaderSize;
	side = 0;
	jpegRst = 0xd0;
	marker[0] = 0xd0;
	marker[1] = 0xd0;
	do
	{
		int i;
		for (i = offset[side]; i < upperSize[side] - 1; i++)
		{
			if (upper[side][i] == 0xff)
			{
				if (upper[side][i + 1] == marker[side])
				{
					marker[side]++;
					CLRB(marker[side], 3);
					upper[side][i + 1] = jpegRst;
					ax32xx_sysDcacheWback((u32)&upper[side][i + 1] & ~0x3, 8);
					jpegRst++;
					CLRB(jpegRst, 3);
					break;
				}
				else if (upper[side][i + 1] == jpegEoi)
				{
					if (side == 1)
						eoi = true;
					upper[side][i + 1] = jpegRst;
					ax32xx_sysDcacheWback((u32)&upper[side][i + 1] & ~0x3, 8);
					jpegRst++;
					CLRB(jpegRst, 3);
					break;
				}
			}
		}
		ret = w_test(whole, upper[side] + offset[side], i + 2 - offset[side]);
		if (ret < 0)
			return ret;

		offset[side] = i + 2;
		side ^= 1;
	} while (!eoi);
	return ret;
}

int hal_mjpegStitchQuadLower(FHANDLE whole, u8 *lower[], u32 lowerSize[])
{
	int ret = 0;
	bool eoi = false;
	INT8U marker[2];
	int offset[2];
	unsigned side;

	offset[0] = jpegHeaderSize;
	offset[1] = jpegHeaderSize;

	side = 0;
	marker[0] = 0xd0;
	marker[1] = 0xd0;
	deg_Printf("start QuadLow while\n");
	do
	{
		int i;
		for (i = offset[side]; i < lowerSize[side] - 1; i++)
		{
			if (lower[side][i] == 0xff)
			{
				if (lower[side][i + 1] == marker[side])
				{
					marker[side]++;
					CLRB(marker[side], 3);
					lower[side][i + 1] = jpegRst;
					ax32xx_sysDcacheWback((u32)&lower[side][i + 1] & ~0x3, 8);
					jpegRst++;
					CLRB(jpegRst, 3);
					break;
				}
				else if (lower[side][i + 1] == jpegEoi)
				{
					if (side == 0)
						lower[side][i + 1] = jpegRst;
					else
						eoi = true;
					break;
				}
			}
		}
		ret = w_test(whole, lower[side] + offset[side], i + 2 - offset[side]);
		if (ret < 0)
			return ret;

		offset[side] = i + 2;
		side ^= 1;
	} while (!eoi);
	
	deg_Printf("end QuadLow while\n");
	return ret;
}



typedef struct JPEG_PHOTO_CACHE_S{
	uint32 mem;
	uint32 len;
	uint32 flag;
}JPEG_PHOTO_CACHE_T;

#define REV_BUF_SIZE 5
JPEG_PHOTO_CACHE_T jpeg_photo_cache;
JPEG_PHOTO_CACHE_T jpeg_rev_buf[REV_BUF_SIZE];

static u8 cache_cnt = 0;

void hal_mjpeg_rev_buf_init(uint32 mem,uint32 len)
{
	cache_cnt = 0;
	jpeg_photo_cache.mem = mem;
	jpeg_photo_cache.len = len;
	deg_Printf("rev buf show len:%d",len);
	memset(jpeg_rev_buf,0,sizeof(jpeg_rev_buf));
}

JPEG_PHOTO_CACHE_T  hal_mjpeg_rev_buf_malloc()
{
	return jpeg_photo_cache;
}

void hal_mjpeg_rev_buf_set(INT32 mem,uint32 len)
{
	uint32 temMem;
	
	jpeg_rev_buf[cache_cnt].mem = mem;
	jpeg_rev_buf[cache_cnt].len = len;
	jpeg_rev_buf[cache_cnt].flag = 1;

	temMem = (jpeg_photo_cache.mem + len+64)&~0x3f;
	jpeg_photo_cache.len -= (temMem-jpeg_photo_cache.mem);
	jpeg_photo_cache.mem = temMem;
	return;
}

void hal_mjpeg_rev_buf_free(INT32 mem)
{
	int i = 0;
	
	for(i=0;i<cache_cnt;i++){
		if(mem == jpeg_rev_buf[i].mem){
			jpeg_rev_buf[i].flag = 0;
			jpeg_photo_cache.mem =   jpeg_rev_buf[i].mem;
			jpeg_photo_cache.len +=  jpeg_rev_buf[i].len;
			cache_cnt--;
			break;
		}
	}

}

int hal_mjpegPhotoQuadStartThumbnail(u8 quality,u8 frame_enable,u8 timestamp,u32 _crop_levels,u8 pic_16_9)
{  
	u16 half_720W,half_720H,src_w,src_h;
	u16 thumbWidth = mjpegEncCtrl.csi_width;
	u16 thumbHeight = mjpegEncCtrl.csi_height;

	hal_csiHalf720P_ResolutionGet(&half_720W,&half_720H);
	hal_mjpegEncodeInit();
	mjpegEncCtrl.frame_enable = frame_enable;
	mjpegEncCtrl.type = MJPEG_TYPE_PHOTO;
	mjpegEncCtrl.mjpeg_width = thumbWidth;
	if(pic_16_9){
		mjpegEncCtrl.mjpeg_height = half_720H;
	}else{
		mjpegEncCtrl.mjpeg_height = thumbHeight;
	}
	//quality = JPEG_Q_62;
	ax32xx_mjpegEncodeInit(1,0);
	uart_Printf("mjpegEncCtrl.csi_width=%d,mjpegEncCtrl.csi_height=%d\n",mjpegEncCtrl.csi_width,mjpegEncCtrl.csi_height);

//	ax32xx_csiScaler(mjpegEncCtrl.csi_width,mjpegEncCtrl.csi_height, mjpegEncCtrl.csi_width, mjpegEncCtrl.csi_height,
//		0,0, mjpegEncCtrl.csi_width, mjpegEncCtrl.csi_height);
//	ax32xx_csiMJPEGFrameSet(mjpegEncCtrl.ybuffer_limage, mjpegEncCtrl.uvbuffer_limage,
//		mjpegEncCtrl.csi_height, mjpegEncCtrl.csi_width);
	
	{
		//if(SysCtrl.crop_level < ARRAY_NUM(crop_levels))
		{
			u16 crop_w,crop_h;
			u16 sx,sy,ex,ey;
			u8 *temp_uvbuf=NULL;
			u32 l = _crop_levels;//crop_levels[SysCtrl.crop_level];
			if(pic_16_9){
				src_h = half_720H;
			}else{
				src_h = mjpegEncCtrl.csi_height;
			}
			crop_w = ((mjpegEncCtrl.csi_width * l / 100) + 0x1f) & (~0x1f);
			crop_h = (src_h * l / 100) & ~1;
			sx = (mjpegEncCtrl.csi_width - crop_w) / 2;
			ex = sx + crop_w;
			sy = (mjpegEncCtrl.csi_height - crop_h) / 2;
			ey = sy + crop_h;
			//uart_Printf("crop_w=%d,crop_h=%d,sx=%d,ex=%d,sy=%d,ey=%d\n",crop_w,crop_h,sx,ex,sy,ey);
			temp_uvbuf=(u8 *)mjpegEncCtrl.ybuffer+crop_w*crop_h;
			ax32xx_mjpeg_inlinebuf_init((u8*)mjpegEncCtrl.ybuffer,(u8*)temp_uvbuf);
			ax32xx_csiScaler(mjpegEncCtrl.csi_width,mjpegEncCtrl.csi_height,crop_w,crop_h,sx,sy,ex,ey);
			ax32xx_mjpegEncodeSizeSet(crop_w,crop_h,mjpegEncCtrl.csi_width,src_h);
			ax32xx_csiMJPEGFrameSet(mjpegEncCtrl.ybuffer,(u32)temp_uvbuf,src_h,crop_w);
		}
	}

//	ax32xx_mjpeg_inlinebuf_init((u8*)mjpegEncCtrl.ybuffer_limage,(u8*)mjpegEncCtrl.uvbuffer_limage);
//	ax32xx_mjpegEncodeSizeSet(mjpegEncCtrl.csi_width, mjpegEncCtrl.csi_height, thumbWidth, thumbHeight);
	ax32xx_mjpegEncodeQuilitySet(quality);
	ax32xx_mjpegEncodeInfoSet(0);
	videoRecordImageWatermark(thumbWidth, thumbHeight, timestamp);
	//hal_streamInit(&mjpegEncCtrl.vids,mjpegEncCtrl.mjpegNode,MJPEG_ITEM_NUM,(u32)mjpegEncCtrl.mjpbuf,mjpegEncCtrl.mjpsize);
	hal_mjpeg_rev_buf_init((u32)mjpegEncCtrl.mjpbuf,mjpegEncCtrl.mjpsize);
	mjpegEncCtrl.curBuffer = jpeg_photo_cache.mem;
	mjpegEncCtrl.curLen = jpeg_photo_cache.len;
	ax32xx_mjpegEncodeBufferSet(mjpegEncCtrl.curBuffer,mjpegEncCtrl.curBuffer+mjpegEncCtrl.curLen);	
	
	ax32xx_csiISRRegiser(CSI_IRQ_JPG_FRAME_END, hal_mjpeg_manual_quad_on);
	ax32xx_csiISRRegiser(CSI_IRQ_SEN_STATE_INT, NULL);
	ax32xx_csiISRRegiser(CSI_IRQ_VXY_FRAME_EN,NULL);
	ax32xx_mjpegEA_ncodeISRRegister(hal_mjpeg_manual_quad_fdown);

	ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN, 1);
	ax32xx_csiEnable(1);

	mjpegEncCtrl.drop = 0;
#if MJPEG_USE_BUF == MJPEG_DOUBLE_LINEBUF
	mjpegEncCtrl.line_buf_using = 1;
	mjpegEncCtrl.line_buf_deal = 1;
#else
	mjpegEncCtrl.line_buf_using = 0;
#endif
	return 0;
}




int hal_mjpegPhotoQuadStartQuarter(int lower, int right,u16 win_w, u16 win_h, u8 quality,u8 timestamp)
{
	int i;
	u8 *dstY = (u8 *)mjpegEncCtrl.ybuffer;
	u8 *dstUv = (u8 *)mjpegEncCtrl.uvbuffer;
	u8 *srcY; //= (u8 *)mjpegEncCtrl.ybuffer_limage;
	u8 *srcUv;// = (u8 *)mjpegEncCtrl.uvbuffer_limage;
//	INT32S ret=0;

	ax32xx_mjpegEncodeInit(1,0);
	mjpegEncCtrl.mjpeg_width = win_w;
	mjpegEncCtrl.mjpeg_height = win_h;
	//quality = hal_mjpegQualityCheck(quality);
	
	//ax32xx_mjpeg_inlinebuf_init((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer);
	ax32xx_mjpegEncodeSizeSet(HAL_CFG_MJPEG_WIDTH, HAL_CFG_MJPEG_HEIGHT, win_w, win_h);
	
	if(!lower && !right){
		//hal_streamInit(&mjpegEncCtrl.vids,mjpegEncCtrl.mjpegNode,4,(u32)mjpegEncCtrl.mjpbuf,mjpegEncCtrl.mjpsize);
		hal_mjpeg_rev_buf_init((u32)jpeg_photo_cache.mem,mjpegEncCtrl.mjpsize);
		ax32xx_sysDcacheWback(mjpegEncCtrl.ybuffer, mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/ 2);
		memcpy((u8 *)jpeg_photo_cache.mem,(u8 *)mjpegEncCtrl.ybuffer,mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/2);
	
		srcY = (u8 *)jpeg_photo_cache.mem;

		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT; i++)
		{
			memcpy(dstY, srcY, HAL_CFG_MJPEG_WIDTH);
			dstY += HAL_CFG_MJPEG_WIDTH;
			srcY += mjpegEncCtrl.csi_width;
		}
		
		srcY = (u8 *)jpeg_photo_cache.mem+mjpegEncCtrl.csi_width/2;

		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT; i++)
		{
			memcpy(dstY, srcY, HAL_CFG_MJPEG_WIDTH);
			dstY += HAL_CFG_MJPEG_WIDTH;
			srcY += mjpegEncCtrl.csi_width;
		}

		
		ax32xx_sysDcacheWback(mjpegEncCtrl.ybuffer+mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/2, mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/ 2);
		memcpy((u8 *)jpeg_photo_cache.mem,(u8 *)mjpegEncCtrl.ybuffer+mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/2,mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/2);
	

		srcY = (u8 *)jpeg_photo_cache.mem;
		
		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT; i++)
		{
			memcpy(dstY, srcY, HAL_CFG_MJPEG_WIDTH);
			dstY += HAL_CFG_MJPEG_WIDTH;
			srcY += mjpegEncCtrl.csi_width;
		}
		
		srcY = (u8 *)jpeg_photo_cache.mem+mjpegEncCtrl.csi_width/2;

		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT; i++)
		{
			memcpy(dstY, srcY, HAL_CFG_MJPEG_WIDTH);
			dstY += HAL_CFG_MJPEG_WIDTH;
			srcY += mjpegEncCtrl.csi_width;
		}
		ax32xx_sysDcacheWback(mjpegEncCtrl.uvbuffer, mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/ 2);
		memcpy((u8 *)jpeg_photo_cache.mem,(u8 *)mjpegEncCtrl.uvbuffer,mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height/2);
	
		srcUv = (u8 *)jpeg_photo_cache.mem;
		
		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT / 2; i++)
		{
			memcpy(dstUv, srcUv, HAL_CFG_MJPEG_WIDTH);
			dstUv += HAL_CFG_MJPEG_WIDTH;
			srcUv += mjpegEncCtrl.csi_width;
		}
		srcUv = (u8 *)jpeg_photo_cache.mem+mjpegEncCtrl.csi_width/2;

		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT / 2; i++)
		{
			memcpy(dstUv, srcUv, HAL_CFG_MJPEG_WIDTH);
			dstUv += HAL_CFG_MJPEG_WIDTH;
			srcUv += mjpegEncCtrl.csi_width;
		}	

		srcUv = (u8 *)jpeg_photo_cache.mem+mjpegEncCtrl.csi_width*mjpegEncCtrl.csi_height / 4;
		
		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT / 2; i++)
		{
			memcpy(dstUv, srcUv, HAL_CFG_MJPEG_WIDTH);
			dstUv += HAL_CFG_MJPEG_WIDTH;
			srcUv += mjpegEncCtrl.csi_width;
		}
		
		srcUv = (u8 *)jpeg_photo_cache.mem+mjpegEncCtrl.csi_width*mjpegEncCtrl.csi_height / 4+mjpegEncCtrl.csi_width/2;
		
		for (i = 0; i < HAL_CFG_MJPEG_HEIGHT / 2; i++)
		{
			memcpy(dstUv, srcUv, HAL_CFG_MJPEG_WIDTH);
			dstUv += HAL_CFG_MJPEG_WIDTH;
			srcUv += mjpegEncCtrl.csi_width;
		}	
	}
	

	mjpegEncCtrl.ybuffer_limage = mjpegEncCtrl.ybuffer;
	mjpegEncCtrl.uvbuffer_limage = mjpegEncCtrl.uvbuffer;
//	u32 yoffset =0,uvoffset = 0;
	 if(right == 0 && lower == 1){
		mjpegEncCtrl.ybuffer_limage+=mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height / 2;
		mjpegEncCtrl.uvbuffer_limage+= mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height / 4;	
		
	}else if(right == 1 && lower == 0){
	//`	mjpegEncCtrl.ybuffer_limage  +=mjpegEncCtrl.csi_width/2 * mjpegEncCtrl.csi_height / 2;
        mjpegEncCtrl.ybuffer_limage+=mjpegEncCtrl.csi_width* mjpegEncCtrl.csi_height / 4;
		mjpegEncCtrl.uvbuffer_limage+= mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height / 8;
	}else if(right == 1 && lower == 1){
		mjpegEncCtrl.ybuffer_limage+=mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height*3 /4;
		mjpegEncCtrl.uvbuffer_limage+= mjpegEncCtrl.csi_width * mjpegEncCtrl.csi_height*3 /8;
	}
	
	mjpegEncCtrl.curLen       = jpeg_photo_cache.len;//_JPG_SIZE_;
	mjpegEncCtrl.curBuffer =  	jpeg_photo_cache.mem;// hal_streamMalloc(&mjpegEncCtrl.vids,mjpegEncCtrl.curLen);
	if(mjpegEncCtrl.curBuffer==0)
		return -1;
	
	deg("StartQuarter the curBuff == %x,the curlen == %x......\n",mjpegEncCtrl.curBuffer,mjpegEncCtrl.curLen);
	videoRecordImageWatermark(mjpegEncCtrl.mjpeg_width, mjpegEncCtrl.mjpeg_height, timestamp);
	ax32xx_mjpeg_inlinebuf_init((u8 *)mjpegEncCtrl.ybuffer_limage,(u8 *)mjpegEncCtrl.uvbuffer_limage);
	ax32xx_mjpegEncodeBufferSet(mjpegEncCtrl.curBuffer,mjpegEncCtrl.curBuffer+mjpegEncCtrl.curLen);	

	ax32xx_mjpeg_manual_on();
	ax32xx_intEnable(IRQ_JPGA,1); // enable jpegirq

	return 0;
}


void StartQuarter_and_StartQuarter(u8 quality)
{
	u16 csi_w,csi_h;

	hal_csiResolutionGet(&csi_w,&csi_h);
	//HAL_CFG_MJPEG_WIDTH = csi_w;
	//HAL_CFG_MJPEG_HEIGHT = csi_w;
	hal_mjpegPhotoQuadCfgQuarter(csi_w, csi_h, quality);
	
	hal_mjpegPhotoQuadStartQuarter(0, 0,csi_w, csi_h, quality,0);

}


void switch_yuv(FHANDLE yuvfile,u8 jpgNum,u8 pic_16_9)
{
	u8 i,j;
	u8 *srcY; //= (u8 *)mjpegEncCtrl.ybuffer_limage;
	u8 *srcUv;// = (u8 *)mjpegEncCtrl.uvbuffer_limage;
	INT32S ret=0;
	u8 num = jpgNum/4;
	u16 half_w720P,encode_h,half_h720P;
	
	if(pic_16_9){
		hal_csiHalf720P_ResolutionGet(&half_w720P,&half_h720P);
		encode_h = half_h720P;
	}else{
		encode_h = mjpegEncCtrl.csi_height;
	}
	for(j=0;j<num;j++)
	{

#if 0
		srcY = (u8 *)mjpegEncCtrl.ybuffer+j*(mjpegEncCtrl.csi_height/num)*mjpegEncCtrl.csi_width;
		for (i = 0; i < mjpegEncCtrl.csi_height/num; i++){
			ret = write(yuvfile,(u8 *)srcY,mjpegEncCtrl.csi_width/2);
			if(ret < 0){
				deg_Printf("write y0 error\n");
				return -1;
			}
			srcY+=mjpegEncCtrl.csi_width;
		}
		srcY = (u8 *)mjpegEncCtrl.ybuffer+j*(mjpegEncCtrl.csi_height/num)*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width/2;
		for (i = 0; i < mjpegEncCtrl.csi_height/num; i++){
			ret = write(yuvfile,(u8 *)srcY,mjpegEncCtrl.csi_width/2);
			if(ret < 0){
				deg_Printf("write y1 error\n");
				return -1;
			}
			srcY+=mjpegEncCtrl.csi_width;
		}
#else
		srcY = (u8 *)mjpegEncCtrl.ybuffer+j*(encode_h/num)*mjpegEncCtrl.csi_width; 
		for (i = 0; i < encode_h/num; i++)
		{
			//deg_Printf("yuvfile:%d \n",yuvfile);
			ret = write(yuvfile,(u8 *)srcY,mjpegEncCtrl.csi_width/4);//切割出一块区域，写进file里。相当于重新组合整张yuv，为了好取。
			if(ret < 0)
			{
				deg_Printf("write y0 error ret %d\n",ret);
				return ;
			}
			srcY+=mjpegEncCtrl.csi_width;
		}
		srcY = (u8 *)mjpegEncCtrl.ybuffer+j*(encode_h/num)*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width*1/4;
		for (i = 0; i < encode_h/num; i++)
		{
			ret = write(yuvfile,(u8 *)srcY,mjpegEncCtrl.csi_width/4);
			if(ret < 0){
				deg_Printf("write y1 error\n");
				return ;
			}
			srcY+=mjpegEncCtrl.csi_width;
		}
		srcY = (u8 *)mjpegEncCtrl.ybuffer+j*(encode_h/num)*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width*2/4;
		for (i = 0; i < encode_h/num; i++)
		{
			ret = write(yuvfile,(u8 *)srcY,mjpegEncCtrl.csi_width/4);
			if(ret < 0){
				deg_Printf("write y3 error\n");
				return ;
			}
			srcY+=mjpegEncCtrl.csi_width;
		}
		srcY = (u8 *)mjpegEncCtrl.ybuffer+j*(encode_h/num)*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width*3/4;
		for (i = 0; i < encode_h/num; i++)
		{
			ret = write(yuvfile,(u8 *)srcY,mjpegEncCtrl.csi_width/4);
			if(ret < 0){
				deg_Printf("write y4 error\n");
				return ;
			}
			srcY+=mjpegEncCtrl.csi_width;
		}

#endif
	}
	
	//UV process
	for(j=0;j<num;j++)
	{
#if 0
		srcUv = (u8 *)mjpegEncCtrl.uvbuffer+j*(mjpegEncCtrl.csi_height/(2*num))*mjpegEncCtrl.csi_width;
		
		for (i = 0; i < mjpegEncCtrl.csi_height/(2*num); i++)
		{
			ret = write(yuvfile,(u8 *)srcUv,mjpegEncCtrl.csi_width/2);
			if(ret < 0){
				deg_Printf("write uv0 error\n");
				return -1;
			}
			srcUv+=mjpegEncCtrl.csi_width;
		}
		srcUv = (u8 *)mjpegEncCtrl.uvbuffer+j*(mjpegEncCtrl.csi_height/(2*num))*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width/2;
		for (i = 0; i < mjpegEncCtrl.csi_height/(2*num); i++)
		{
			ret = write(yuvfile,(u8 *)srcUv,mjpegEncCtrl.csi_width/2);
			if(ret < 0){
				deg_Printf("write uv1 error\n");
				return -1;
			}
			srcUv+=mjpegEncCtrl.csi_width;
		}
#else
		srcUv = (u8 *)mjpegEncCtrl.uvbuffer+j*(encode_h/(2*num))*mjpegEncCtrl.csi_width;
		for (i = 0; i < encode_h/(2*num); i++)
		{
			ret = write(yuvfile,(u8 *)srcUv,mjpegEncCtrl.csi_width/4);
			if(ret < 0){
				deg_Printf("write uv0 error\n");
				return ;
			}
			srcUv+=mjpegEncCtrl.csi_width;
		}
		srcUv = (u8 *)mjpegEncCtrl.uvbuffer+j*(encode_h/(2*num))*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width*1/4;
		for (i = 0; i < encode_h/(2*num); i++)
		{
			ret = write(yuvfile,(u8 *)srcUv,mjpegEncCtrl.csi_width/4);
			if(ret < 0){
				deg_Printf("write uv1 error\n");
				return ;
			}
			srcUv+=mjpegEncCtrl.csi_width;
		}
		srcUv = (u8 *)mjpegEncCtrl.uvbuffer+j*(encode_h/(2*num))*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width*2/4;
		for (i = 0; i < encode_h/(2*num); i++)
		{
			ret = write(yuvfile,(u8 *)srcUv,mjpegEncCtrl.csi_width/4);
			if(ret < 0){
				deg_Printf("write uv2 error\n");
				return ;
			}
			srcUv+=mjpegEncCtrl.csi_width;
		}
		srcUv = (u8 *)mjpegEncCtrl.uvbuffer+j*(encode_h/(2*num))*mjpegEncCtrl.csi_width+mjpegEncCtrl.csi_width*3/4;
		for (i = 0; i < encode_h/(2*num); i++)
		{
			ret = write(yuvfile,(u8 *)srcUv,mjpegEncCtrl.csi_width/4);
			if(ret < 0){
				deg_Printf("write uv3 error\n");
				return ;
			}
			srcUv+=mjpegEncCtrl.csi_width;
		}

#endif
		
	}
	
	ret= lseek(yuvfile,0,0);
	deg_Printf("switch seek:%d\n",ret);
	#if !RE_USEMJPG_SAPCE
	ret = read(yuvfile,mjpegEncCtrl.ybuffer,mjpegEncCtrl.csi_width * encode_h);
	if(ret<0)
		deg_Printf("read yuvfile Ybuf failed\n");
	
	ret = read(yuvfile,mjpegEncCtrl.uvbuffer,mjpegEncCtrl.csi_width * encode_h/2);
	if(ret<0)
		deg_Printf("read yuvfile uvbuf failed\n");
		
	close(yuvfile);
	#endif
}


int hal_mjpegStitch(FHANDLE whole, u8 *upper[], u32 upperSize[],u8 grop,u8 num)
{
	int ret = 0;
	INT8U marker[4];
	int offset[4];
	unsigned side;
	bool sos = false, eoi = false;
	int jpegJfxxOffset = 0,i;
	u16 jfifLen;
	u8 jfxx[] = {0xff, 0xe0, 0, 0, 'J', 'F', 'X', 'X', '\0', 0x10};

	if(!grop)
	{
	
		for (i = 0; i < upperSize[0]; i++)
		{
			if (upper[0][i] == 0xff && upper[0][i + 1] == jfxx[1])
			{
				break;
			}
		}
		
		jfifLen = upper[0][i + 2] << 8 | upper[0][i + 3];

		//ret = w_test(whole, upper[0], jpegJfxxOffset);
		
		jpegJfxxOffset = i + 2 + jfifLen;

		ret = w_test(whole, upper[0], jpegJfxxOffset);


		offset[0] = jpegJfxxOffset;
		
		do
		{
			u8 *header;
			u16 headerLen;
			u16 width, height;
			header = upper[0] + offset[0];
			headerLen = header[2] << 8 | header[3];
			switch (header[1])
			{
			case 0xc0:
				height = header[5] << 8 | header[6];
				height *= num/4;
				header[5] = height >> 8;
				header[6] = height & 0xff;
				width = header[7] << 8 | header[8];
				width *= 4;
				header[7] = width >> 8;
				header[8] = width & 0xff;
				break;
			case 0xda:
				sos = true;
				break;
			default:
				break;
			}
			offset[0] += 2 + headerLen;
		} while (!sos);
		
		deg("StitchUpper:SOS %d\n", offset[0]);
		
		ret = w_test(whole, upper[0] + jpegJfxxOffset, offset[0] - jpegJfxxOffset);
		if (ret < 0)
			return ret;
		jpegHeaderSize = offset[0];
		jpegRst = 0xd0;
	}

	#if 0
		offset[0] = jpegHeaderSize;
		offset[1] = jpegHeaderSize;
		offset[2] = jpegHeaderSize;
		offset[3] = jpegHeaderSize;
	#else
		for(i=0;i<4;i++)
		{
			offset[i] = jpegHeaderSize;
		}
	#endif
	side = 0;
	
#if 0
	marker[0] = 0xd0;
	marker[1] = 0xd0;
	marker[2] = 0xd0;
	marker[3] = 0xd0;
#else
	for(i=0;i<4;i++)
	{
		marker[i] = 0xd0;
	}
#endif
	do
	{
		int i;
		for (i = offset[side]; i < upperSize[side] - 1; i++)
		{
			if (upper[side][i] == 0xff)
			{
				if (upper[side][i + 1] == marker[side])
				{
					marker[side]++;
					CLRB(marker[side], 3);
					upper[side][i + 1] = jpegRst;
					ax32xx_sysDcacheWback((u32)&upper[side][i + 1] & ~0x3, 8);
					jpegRst++;
					CLRB(jpegRst, 3);
					break;
				}
				else if (upper[side][i + 1] == jpegEoi)
				{
					if (side == 3)
						eoi = true;
					if((grop == num/4-1)&&eoi){
						
					}else{
						upper[side][i + 1] = jpegRst;
						ax32xx_sysDcacheWback((u32)&upper[side][i + 1] & ~0x3, 8);
						jpegRst++;
						CLRB(jpegRst, 3);
					}
					break;
				}
			}
		}
		ret = w_test(whole, upper[side] + offset[side], i + 2 - offset[side]);
		if (ret < 0)
			return ret;

		offset[side] = i + 2;
		//side ^= 1;
		side++;
		if(side>=4)
		{
			side=0;
		}
	} while (!eoi);
	return ret;
}

/*
1: init Buf  ,0:uninit Buf
*/
void doublebuf_And_resbuf_Ctrl(u8 en)
{
	if(en)
	{
		resBuffInit();
		useDoubleBuff(TRUE);

	}
	else
	{
		resBuffUninit();
		useDoubleBuff(false);
	}

}
int imageEncodeQuadStart(const char *path,const char *prefix, FHANDLE fd, u16 width, u16 height, u8 quality, u8 timestamp,u8 frame_enable,u32 res_idx,u16 pos_X,u16 pos_Y,u8 gap,u32 _crop_levels,u8 pic_16_9)
{
	int ret = 0;
	u8 *quarterBuf[2][4] = {{NULL,NULL},{NULL,NULL}};
	void /**quarterNode = NULL, *thumbNode = NULL,*/ *thumbBuf = NULL;
	u32 quarterSize[2][4], thumbSize;
	TCHAR YuvPath[32 + MANAGER_NAME_LEN + 1];
	char jpgPath[32 + MANAGER_NAME_LEN + 1];
	INT32U timeout;
	u16 half_w720P,encode_h,half_h720P=0;
	cache_cnt = 0;
	FHANDLE yuvfile=-1;

	//doublebuf_And_resbuf_Ctrl(0);	//uninit useDoubleBuff & resBuffInit ,90k to photo

	if(pic_16_9){
		hal_csiHalf720P_ResolutionGet(&half_w720P,&half_h720P);
		encode_h = half_h720P;
	}else{
		encode_h = mjpegEncCtrl.csi_height;
	}
	hal_mjpegEncodeResolutionImage(width, height,pic_16_9);


	//ret = hal_mjpegMemInit(1);
	ret = hal_mjpegMemInit(5);
	if(ret<0)
		goto ENCODE_END;
	


	ret = w_test_open(fd, 1 << 13);
	if (ret < 0)
		goto ENCODE_END;
	
	hal_mjpegEncodeResolutionImageQuad(width, height,pic_16_9);
	if (ret < 0)
	{
		deg("image encode: malloc fail\n");
		//return ret;
		goto ENCODE_END;
	}

	//videoRecordImageWatermark(width, height, timestamp);
	hal_mjpegPhotoQuadStartThumbnail(quality,frame_enable,0,_crop_levels,pic_16_9); //encode thumb
	

	
	timeout = XOSTimeGet() + 2000;							//wait encode  finish 
	do
	{
		if(jpeg_rev_buf[cache_cnt].flag){
			thumbBuf = (void *)jpeg_rev_buf[cache_cnt].mem;			//get data
			thumbSize =  jpeg_rev_buf[cache_cnt].len;
			break;
		}
		if (XOSTimeGet() > timeout)
		{
			deg("thumber image encode: timeout\n");
			//return -1;
			ret= -1;
			goto ENCODE_END;
		}
	} while (true);
	cache_cnt++;	
	if (timestamp)
		videoRecordImageWatermark(width, height, 0);

	deg_Printf("imageL:read thumberSize:%x\n",thumbSize);


/**********************************************/
	//==set lcd image stop==
	hal_wdtClear();
	hal_csiEnable(0);	
	dispLayerSetPIPMode(DISP_PIP_DISABLE);	
	deg_Printf("play sound\n");

	system_audio_play(res_idx,1,0);
	

//==decode jpg==
	//if(0!=SysCtrl.crop_level)
	{
		//ax32xx_csiEnable(1);
		dispLayerSetPIPMode(DISP_PIP_DISABLE);
		XOSTimeDly(20);
		u32 ticks=XOSTimeGet();
		Image_ARG_T arg;
		arg.target.width = mjpegEncCtrl.csi_width;
		arg.target.height = encode_h;
		arg.yout  = (u8*)mjpegEncCtrl.ybuffer;
		arg.uvout = (u8*)mjpegEncCtrl.uvbuffer;
		arg.media.type = MEDIA_SRC_RAM;
		arg.media.src.buff = (INT32U)thumbBuf;//thumbNode;
		arg.wait = 1;  // wait decode end
		if(imageDecodeStart(&arg)<0)
		{
			deg("jpg decode fail\n");
		}
		else
		{
			//解码第2张图片完成 640*480
			ticks=XOSTimeGet()-ticks;
			deg("jpg decode ok,ticks=%d\n",ticks);
			u16 dec_width,dec_height;
			imageDecodeGetResolution(&dec_width,&dec_height);
			deg("jpg decode:w=%d,h=%d\n",dec_width,dec_height);
		}
		ax32xx_sysDcacheWback(	(INT32U)mjpegEncCtrl.ybuffer,
								(INT32U)mjpegEncCtrl.ybuffer+mjpegEncCtrl.csi_width*encode_h*3/2);
		//hal_csiEnable(1);
		//dispLayerSetPIPMode(SysCtrl.pip_mode);
	}
//==end decode jpg==



/********************************************/
	
	if(mjpegEncCtrl.frame_enable==1)
	{
		hal_custom_frame_add_Lmjpeg((u8 *)mjpegEncCtrl.ybuffer,pic_16_9);

		
		///hal_mjpegPhotoQuadStartThumbnail(quality,frame_enable);
		
	}
#if 1
	deg_Printf("-------- imageL photo mode switch:[%d]\n",SysCtrl.photo_mode_switch);
	if(2==SysCtrl.photo_mode_switch)	// effect mode
	{
		yuv420_effect_handle((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer,mjpegEncCtrl.csi_width,encode_h);

	}
	else if(0==SysCtrl.photo_mode_switch)
	{
		if((7==SysCtrl.photo_mode_idx)||(8==SysCtrl.photo_mode_idx))//==pencil effect==
		{
			u8 color;
			if(7==SysCtrl.photo_mode_idx)
			{
				color=0;
			}
			else
			{
				color=1;
			}
			yuv420_pencil_sketch((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer,mjpegEncCtrl.csi_width,encode_h,color);
		}
	}
#else
	deg_Printf("---check--- in func:%s,line:%d\n",__func__,__LINE__);
	deg_Printf("ready to in new pencil effect!!\n");
	photo_effect_ctrl((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer,mjpegEncCtrl.csi_width,encode_h);

#endif
		// if (timestamp)
		// videoRecordImageWatermark(width, height, 0);

		if(timestamp)
			watermark_bmp2yuv_draw((u8*)mjpegEncCtrl.ybuffer,pos_X,pos_Y,gap);

	hal_sysMemPrint();
	ax32xx_sysDcacheWback(	(INT32U)mjpegEncCtrl.ybuffer, 
							(INT32U)mjpegEncCtrl.ybuffer+mjpegEncCtrl.csi_width*encode_h*3/2);

/********************************************/



	//if((mjpegEncCtrl.frame_enable==1)||(2==SysCtrl.photo_mode_switch))
	{
		//StartQuarter_and_StartQuarter(quality);
		//hal_mjpegPhotoQuadCfgQuarter(width, height, quality);
		//hal_mjpegPhotoQuadStartQuarter(0, 0,width, height, quality,0);
		
		hal_mjpeg_rev_buf_free((INT32)thumbBuf);
		watermark_buf_bmp2yuv_free();
		//hal_mjpeg_rev_buf_init((u32)mjpegEncCtrl.mjpbuf,mjpegEncCtrl.mjpsize);	
		deg_Printf("show mjpgbuf add:%x size:%x\n",mjpegEncCtrl.mjpbuf,mjpegEncCtrl.mjpsize);
		quality = hal_mjpegQualityCheck(quality);
		mjpegEncCtrl.curLen 	  = jpeg_photo_cache.len;//_JPG_SIZE_;
		mjpegEncCtrl.curBuffer =	jpeg_photo_cache.mem;

		ax32xx_mjpegEncodeInit(1,0);

		//ax32xx_mjpeg_inlinebuf_init((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer);
		ax32xx_mjpegEncodeSizeSet(mjpegEncCtrl.csi_width, encode_h, mjpegEncCtrl.csi_width, encode_h);

		
		videoRecordImageWatermark(mjpegEncCtrl.mjpeg_width, mjpegEncCtrl.mjpeg_height, 0);
		ax32xx_mjpeg_inlinebuf_init((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer);
		ax32xx_mjpegEncodeBufferSet(mjpegEncCtrl.curBuffer,mjpegEncCtrl.curBuffer+mjpegEncCtrl.curLen); 
		
		ax32xx_mjpeg_manual_on();
		ax32xx_intEnable(IRQ_JPGA,1); // enable jpegirq


		timeout = XOSTimeGet() + 2000;
		//cache_cnt=0;
		do
		{
			if(jpeg_rev_buf[cache_cnt].flag){
				thumbBuf = (void *)jpeg_rev_buf[cache_cnt].mem;
				thumbSize =  jpeg_rev_buf[cache_cnt].len;
				break;
			}
			if (XOSTimeGet() > timeout)
			{
				deg("thumber image encode: timeout\n");
				//return -1;
				ret= -1;
				goto ENCODE_END;
			}
		} while (true);
		cache_cnt++;	
		}


		hal_mjpegPhotoQuadCfgQuarter(width, height, quality);


		ptrdiff_t infixOffset;
		infixOffset = strstr(path, prefix) + 2 - path;
		//strcpy(YuvPath, path);
		Ascii2Tchar(path, YuvPath, sizeof(YuvPath)/sizeof(YuvPath[0]));
		YuvPath[infixOffset] =	 'u';
		YuvPath[infixOffset+1] = 'v';
		/*FHANDLE*/ yuvfile = open(YuvPath, FA_CREATE_NEW | FA_WRITE | FA_READ);
		switch_yuv(yuvfile,48,pic_16_9);	//16:9 off
		u8 num = 48,i;
		u16 width1,height1;
		u8 *tempY,*tempUV;
		width1 =  width /4;//2240;//width /4;
		height1 = (height*4/num);//864;;//(height*4/num)&~1;
		width1=(width1 + 0x1f) & (~0x1f);  // add 32bit alignment
		height1=(height1 + 0x1f) & (~0x1f);  // add 32bit alignment
		
		#if RE_USEMJPG_SAPCE
		debg("csi_w:%d h:%d\n",mjpegEncCtrl.csi_width,encode_h);
		hal_mjpeg_rev_buf_init((u32)mjpegEncCtrl.ybuffer,(mjpegEncCtrl.csi_width*encode_h)*3/2);	//use yuvbuf's 460kb space to encode buf
		tempY = (mjpegEncCtrl.mjpbuf+thumbSize+0x1f) &(~0x1f);		//offset thumbSize , save the thumbuf
		#endif
		debg("first tempY:%x\n",tempY);


		for(i=0;i<num;i++)
		{
			ax32xx_mjpegEncodeInit(1,0);
			ax32xx_mjpegEncodeSizeSet(mjpegEncCtrl.csi_width/4,encode_h*4/num, width1, height1);


			if((timestamp==1)&&(i==num-4)) //last 4 piece
			{
				videoRecordImageWatermark(width1, height1, 0);
			}
			
			//if(i%4 == 0)
			//	hal_streamInit(&mjpegEncCtrl.vids,mjpegEncCtrl.mjpegNode,MJPEG_ITEM_NUM,(u32)mjpegEncCtrl.mjpbuf,mjpegEncCtrl.mjpsize);

			//mjpegEncCtrl.curLen	  = (100*1024);//_JPG_SIZE_MIN_DEF2_;
			//SysCtrl.photo_finish_flag = 1;
			//mjpegEncCtrl.curBuffer = hal_streamMalloc(&mjpegEncCtrl.vids,mjpegEncCtrl.curLen);
			mjpegEncCtrl.curLen 	  = jpeg_photo_cache.len;//_JPG_SIZE_;
			mjpegEncCtrl.curBuffer =	jpeg_photo_cache.mem;// hal_streamMalloc(&mjpegEncCtrl.vids,mjpegEncCtrl.curLen);
			if(mjpegEncCtrl.curBuffer==0)
				return -1;


			#if !RE_USEMJPG_SAPCE
				tempY  = mjpegEncCtrl.ybuffer +i*(mjpegEncCtrl.csi_width/4)*(encode_h*4/num);
				tempUV = mjpegEncCtrl.uvbuffer+i*(mjpegEncCtrl.csi_width/8)*(encode_h*4/num);
			#else
				lseek(yuvfile,i*(mjpegEncCtrl.csi_width/4)*(encode_h*4/num),0);	//lseek yuvfile data
				read(yuvfile,tempY,(mjpegEncCtrl.csi_width/4)*(encode_h*4/num)); //read file w&h, use src w&h
				tempUV = tempY+(mjpegEncCtrl.csi_width/4)*(encode_h*4/num);
				lseek(yuvfile,mjpegEncCtrl.csi_width*encode_h+i*(mjpegEncCtrl.csi_width/8)*(encode_h*4/num),0);
				read(yuvfile,tempUV,(mjpegEncCtrl.csi_width/4)*(encode_h*4/num)/2); 
				
				ax32xx_sysDcacheWback(tempY,(mjpegEncCtrl.csi_width/4)*(encode_h*4/num)*3/2);
		
			#endif
			
			ax32xx_mjpeg_inlinebuf_init(tempY,tempUV);
			ax32xx_mjpegEncodeBufferSet(mjpegEncCtrl.curBuffer,mjpegEncCtrl.curBuffer+mjpegEncCtrl.curLen); 
			ax32xx_mjpeg_manual_on();
			ax32xx_intEnable(IRQ_JPGA,1); // enable jpegirq
			
			//deg_Printf("i=%d,infixOffset=%d.\n",i,infixOffset);
			
#if 0		//every small pic save in card (12pic will all save)
			strcpy(jpgPath, path);
			deg_Printf("path[%s] ",path);
			int jpgFd;
			jpgPath[infixOffset] =	 i/10+'0';
			jpgPath[infixOffset+1] = i%10+'0';
			deg_Printf("jpgPath:[%s]\n",jpgPath);
			jpgFd = open(jpgPath, FA_CREATE_NEW | FA_WRITE | FA_READ);
			if (jpgFd < 0)
			{
				deg_Printf("image encode: open upper file fail\n");
				ret = -3;
				//return ret;
				goto ENCODE_END;
			}

			/*if(hal_imageWriteFile(jpgFd)<0)
			{
				deg_Printf("write file quarterFd[0][1] failed\n");
				ret = -3;
				goto ENCODE_END;
			}	
			close(jpgFd);*/
			timeout = XOSTimeGet() + 3500;
			do
			{		
				if(jpeg_rev_buf[cache_cnt].flag){
					quarterBuf[0][i%4] = jpeg_rev_buf[cache_cnt].mem;
					quarterSize[0][i%4] =  jpeg_rev_buf[cache_cnt].len;
					write(jpgFd,(void *)jpeg_rev_buf[cache_cnt].mem, jpeg_rev_buf[cache_cnt].len);
					break;
				}
				
				if (XOSTimeGet() > timeout)
				{
					deg("image encode: timeout\n");
					//return -1;
					ret = -1;
					goto ENCODE_END;
				}
			} while (true);
			cache_cnt++;
			hal_mjpeg_rev_buf_free(jpeg_rev_buf[cache_cnt-1].mem);
			ax32xx_wdtClear();
			close(jpgFd);
#else
			timeout = XOSTimeGet() + 3500;
			do
			{		
				if(jpeg_rev_buf[cache_cnt].flag){
					quarterBuf[0][i%4] = jpeg_rev_buf[cache_cnt].mem;
					quarterSize[0][i%4] =  jpeg_rev_buf[cache_cnt].len;
					break;
				}
				
				if (XOSTimeGet() > timeout)
				{
					deg("image encode: timeout\n");
					//return -1;
					ret = -1;
					goto ENCODE_END;
				}
			} while (true);
			cache_cnt++;
			if(i%4==3)
			{

			#if 0	
				char *name;
				int fHandle;
				fHandle = task_image_createfile(VIDEO_CH_A,&name);

				write(fHandle,(void *)thumbBuf,thumbSize);
				deg_Printf("imageL save thumb :%x Size:%x\n",thumbBuf,thumbSize);
				deamon_fsSizeModify(-1,fs_size(fHandle));
				//deg_Printf("SysCtrl.printer_en=%d,size=%d\n",SysCtrl.printer_en,fs_size(fHandle));
				close(fHandle);
							//managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&name[strlen(FILEDIR_PHOTO)]);
										
		#endif
				hal_mjpegStitch(fd, quarterBuf[0], quarterSize[0],i/4,num);
				debg("before free remain:%x\n",hal_sysMemRemain());
				hal_mjpeg_rev_buf_free(quarterBuf[0][3]);
				hal_mjpeg_rev_buf_free(quarterBuf[0][2]);
				hal_mjpeg_rev_buf_free(quarterBuf[0][1]);
				hal_mjpeg_rev_buf_free(quarterBuf[0][0]);
			}
#endif
			
			ax32xx_wdtClear();
			//return res;
		}




	#if 0
		hal_mjpegPhotoQuadStartQuarter(0, 0,width, height, quality,0);

	

	
	deg("+-\n--\n");
	
	timeout = XOSTimeGet() + 3500;
	do
	{		
 		if(jpeg_rev_buf[cache_cnt].flag){
			quarterBuf[0][0] = (u8 *)jpeg_rev_buf[cache_cnt].mem;
			quarterSize[0][0] =  jpeg_rev_buf[cache_cnt].len;
			break;
		}
		
		if (XOSTimeGet() > timeout)
		{
			deg("image encode: timeout\n");
			//return -1;
			ret = -1;
			goto ENCODE_END;
		}
	} while (true);
	cache_cnt++;
	//hal_mjpegStitchQuadThumb1(fd, quarterBuf[0][0], quarterSize[0][0], thumbBuf, thumbSize);
	
	deg("++\n--\n");
	hal_mjpegPhotoQuadStartQuarter(0, 1,width, height, quality,0);
	
	timeout = XOSTimeGet() + 2000;
	do
	{
		if(jpeg_rev_buf[cache_cnt].flag){
			quarterBuf[0][1] = (u8 *)jpeg_rev_buf[cache_cnt].mem;
			quarterSize[0][1] =  jpeg_rev_buf[cache_cnt].len;
			break;
		}
		if (XOSTimeGet() > timeout)
		{
			deg("image encode: timeout\n");
			//return -1;
			ret = -1;
			goto ENCODE_END;
		}
	} while (true);
	cache_cnt++;
	hal_mjpegStitchQuadUpper(fd, quarterBuf[0], quarterSize[0]);
	hal_mjpeg_rev_buf_free((INT32)(quarterBuf[0][1]));
	hal_mjpeg_rev_buf_free((INT32)(quarterBuf[0][0]));
	//videoRecordImageWatermark(width, height, timestamp);
	deg("++\n+-\n");
	hal_mjpegPhotoQuadStartQuarter(1, 0,width, height, quality,0);
	timeout = XOSTimeGet() + 2000;
	do
	{	
		if(jpeg_rev_buf[cache_cnt].flag){
			quarterBuf[1][0] = (u8 *)jpeg_rev_buf[cache_cnt].mem;
			quarterSize[1][0] =  jpeg_rev_buf[cache_cnt].len;
			break;
		}
		if (XOSTimeGet() > timeout)
		{
			deg("image encode: timeout\n");
			//return -1;
			ret = -1;
			goto ENCODE_END;
		}
	} while (true);
		cache_cnt++;
	//if (timestamp)
		//videoRecordImageWatermark(width, height, 0);
	deg("++\n++\n");
	hal_mjpegPhotoQuadStartQuarter(1, 1,width, height, quality,0);
	timeout = XOSTimeGet() + 2000;
	do
	{	
		if(jpeg_rev_buf[cache_cnt].flag){
			quarterBuf[1][1] = (u8 *)jpeg_rev_buf[cache_cnt].mem;
			quarterSize[1][1] =  jpeg_rev_buf[cache_cnt].len;
			break;
		}
		if(XOSTimeGet() > timeout)
		{
			deg("image encode: timeout\n");
			//return -1;
			ret = -1;
			goto ENCODE_END;
		}
	} while (true);
		
	cache_cnt++;
	ret = 0;
	deg("the encode end test\n");

	deg_Printf(" QuadLower\n");
	hal_mjpegStitchQuadLower(fd, quarterBuf[1], quarterSize[1]);
	#endif
	deg_Printf(" QuadThumb\n");
	hal_mjpegStitchQuadThumb(fd, (u8*)thumbBuf, thumbSize);
	//hal_sysMemPrint();
	deg("the encode end\n");
ENCODE_END:	
	hal_csiEnable(1);
	dispLayerSetPIPMode(SysCtrl.pip_mode);
	w_test_close();	
#if SDRAM_SIZE == SDRAM_SIZE_2M 
	mjpegEncCtrl.mjpbuf = 0;
#endif
	hal_mjpegMemUninit();
	hal_mjpegEncodeStop();
#if SDRAM_SIZE == SDRAM_SIZE_2M
	hal_mjpegMjpegReuseUninit();
#endif
	//watermark_buf_bmp2yuv_free();
	close(yuvfile);
	f_unlink(YuvPath);
	//doublebuf_And_resbuf_Ctrl(1);

	return ret;
}

#endif









