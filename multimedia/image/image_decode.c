/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA IMAGE DECODE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : image_decode.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image decode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"

#if  MEDIA_CFG_IMAGE_DECODE_EN  >0

/*******************************************************************************
* Function Name  : imageDecodeInit
* Description	 : image decode initial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0
*******************************************************************************/
int imageDecodeInit(void)
{
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : imageDecodeUninit
* Description	 : image decode uninitial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int imageDecodeUninit(void)
{
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : imageDecodeStart
* Description	 : image decode
* Input 		 : Image_ARG_T *arg : image arg
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/
int imageDecodeStart(Image_ARG_T *arg)
{
#define USE_TWICE_DECODE  1
    INT8U *jpegBuffer/*,*jpegBuffer1*/,
	 /**thumbBuffer = NULL,*/*tempBuff = NULL,*thumbBuffer_old=NULL; //--
	INT32S addr,size=0,ret,res;
	volatile int timeout;

	if(arg == NULL)
		return STATUS_FAIL;
	hal_mjpeglDecodeSetResolution(0,0);
	
	if(arg->media.type == MEDIA_SRC_FS)
	{
		if(arg->media.src.fd<0)
			{
			deg_Printf("decode fail:arg->media.src.fd\n");
			return STATUS_FAIL;
		}
			
		/*jpegBuffer = (INT8U *)hal_sysMemMalloc(fs_size(arg->media.src.fd),64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
		if(jpegBuffer)
		{
			size = fs_size(arg->media.src.fd);
			read(arg->media.src.fd,jpegBuffer,size);		
			ax32xx_sysDcacheFlush((u32)jpegBuffer,size);
		}*/


		//-------------------------
		if(fs_size(arg->media.src.fd) > hal_sysMemRemain()+64)
		{
			size = fs_size(arg->media.src.fd);
			jpegBuffer = (INT8U *)hal_sysMemMalloc(100*1024,64);
			read(arg->media.src.fd,jpegBuffer,100*1024);
			ax32xx_sysDcacheFlush((u32)jpegBuffer,100*1024);

			res = hal_mjpegHeaderParse(jpegBuffer,NULL);
			deg_Printf("show decode res:%d\n",res);
			if(res == -10 || res == 0)
			{
				
				INT8U app[12];
				lseek(arg->media.src.fd, size - sizeof app, 0);
				read(arg->media.src.fd, app, sizeof app);
				if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
				{
					u32 thumbOffset;
					thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
					deg_Printf("thumbOffset=0x%x \n", thumbOffset);
					lseek(arg->media.src.fd, thumbOffset, 0);
					read(arg->media.src.fd,jpegBuffer,100*1024); 
					ax32xx_sysDcacheFlush((u32)jpegBuffer,100*1024);
					deg_Printf("im_decode:Decode thumb ");
					deg_Printf("read jpg data : jpgSize:%d thumbSize:%d\n",size,size-thumbOffset);
				}
			}
		}else
		{
			jpegBuffer = (INT8U *)hal_sysMemMalloc(fs_size(arg->media.src.fd),64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
			
			if(jpegBuffer)
			{
				size = fs_size(arg->media.src.fd);
				read(arg->media.src.fd,jpegBuffer,size);	
				ax32xx_sysDcacheFlush((u32)jpegBuffer,size);
			}
			else
			{
				deg_Printf("mem err\n");
			}

		}


		//-------------------------
		addr = 0;
	}
	else if(arg->media.type == MEDIA_SRC_NVFS)
	{
		addr = nv_open((int)arg->media.src.fd);
		if(addr<0)
		{
		deg_Printf("decode fail:nvopen addr<0\n");
			return STATUS_FAIL;
		}
			
		jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(arg->media.src.fd),64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
		if(jpegBuffer)
		{
			size = nv_size((int)arg->media.src.fd);
			ax32xx_sysDcacheFlush((u32)jpegBuffer,size);
			
			nv_read(addr,jpegBuffer,size);	
			ax32xx_sysDcacheWback((u32)jpegBuffer,size);
		}

	}
	else if(arg->media.type == MEDIA_SRC_RAM)
	{
		if(arg->media.src.buff == 0)
		{
		deg_Printf("decode fail:arg->media.src.buff == 0\n");
			return STATUS_FAIL;
		}
			
		jpegBuffer = (INT8U *)arg->media.src.buff;
		addr = 0;
	}
	else
	{
		deg_Printf("decode fail:arg->media.type != MEDIA_SRC_RAM\n");
		return STATUS_FAIL;
	}
		
    
	if(jpegBuffer == NULL)
	{
		deg_Printf("imageDecodeStart jpeg buf null!\n");
		return STATUS_FAIL;
	}

	ret = 0;
	INT16U src_width = 0, src_height = 0,src_lwidth = 0, src_lheight = 0;
	INT16U dst_width = 0, dst_height = 0;
	INT32U temp_yout = 0, temp_uvout = 0;
	INT32U decode_twice = 0;
	
#if 0	
	if(hal_mjpegHeaderParse(jpegBuffer)<0)
	{
		ret = -1;
		deg_Printf("headparse fail\n");
		goto END_INAGE_DECODE;
    }
#else
	ret = hal_mjpegHeaderParse(jpegBuffer,&thumbBuffer_old);

	if(ret == -10)
	{
#if 1	
			INT8U app[12];
			lseek(arg->media.src.fd, size - sizeof app, 0);
			read(arg->media.src.fd, app, sizeof app);
			if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
			{
				u32 thumbOffset;
				thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
				deg("thumbOffset=0x%x  thumbsize=%d\n", thumbOffset,size-thumbOffset);
				lseek(arg->media.src.fd, thumbOffset, 0);
				read(arg->media.src.fd,jpegBuffer,300*1024);	
				ax32xx_sysDcacheFlush((u32)jpegBuffer,300*1024);
				deg("Decode thumb\n");
				ret = hal_mjpegHeaderParse(jpegBuffer,NULL);
			}else
			{
				tempBuff = jpegBuffer;
				jpegBuffer = thumbBuffer_old;
				deg("Decode thumb\n");
				ret = hal_mjpegHeaderParse(jpegBuffer,NULL);
			}
#else
		INT8U app[12];
		u32 point;
		
		point = size - sizeof app;
		memcpy(app,jpegBuffer + point,sizeof app);
		
		if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
		{
			u32 thumbOffset;
			thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
			tempBuff = jpegBuffer;
			jpegBuffer = jpegBuffer+thumbOffset;
			ret = hal_mjpegHeaderParse(jpegBuffer);
		}
#endif
	}
	if(ret < 0)
	{
		deg_Printf("headparse fail ret :%d\n",ret);
		ret = -1;
		
		goto END_INAGE_DECODE;
    }
#endif

	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	hal_mjpeglDecodeGetResolution(&src_lwidth,&src_lheight);

	//deg_Printf("....src_width:%d,src_height:%d src_lwidth:%d,src_lheight:%d\n",src_width,src_height,src_lwidth,src_lheight);
//	deg_Printf("targ width:%d, targ height:%d\n",arg->target.width,arg->target.height);
#if USE_TWICE_DECODE
	if((src_height > arg->target.height * 4)&&(arg->media.type != MEDIA_SRC_RAM))
	{
		dst_width = (arg->target.width *2+0x1f)&(~0x1f) ;
		dst_height = arg->target.height *2;
		//dst_width = (src_width/2+0x1f)&(~0x1f) ;
		//dst_height = src_height/2;
		temp_yout = (u32)hal_sysMemMalloc(dst_width*dst_height*3/2,64);
		if(temp_yout ==0){
			deg_Printf("malloc buf fail, use direct decode\n");
			decode_twice = 0;
		}else{
			temp_uvout = temp_yout + dst_width*dst_height;
			decode_twice = 1;
			deg_Printf("use twice decode\n");
		}	
	}
	else{
		decode_twice = 0;
	}
#else
	decode_twice = 0;
#endif
//    if(jpeg_decode_picture(jpegBuffer,arg->yout,arg->uvout,arg->target.width,arg->target.height) == false)
	if(decode_twice == 0)
	{
		if(hal_mjpegDecodePicture(jpegBuffer,arg->yout,arg->uvout,arg->target.width,arg->target.height)<0)
		{
			ret = -1;
			deg_Printf("hal_mjpegDecodePicture err!\n");
			goto END_INAGE_DECODE;
		}

		if(arg->wait) // wait decode end
		{
	//		timer_ticktimer_start();
			timeout = 0;
			while(hal_mjpegDecodeStatusCheck())
			{
				timeout++;
				if(timeout>=0x0fffff)
				{
					deg_Printf("decode timeout err!\n");
					break;
				}
			}
	//	    ticktimer_stop();
		}

		if(hal_mjpegDecodeErrorCheck()!=0)
		{
			deg_Printf("hal_mjpegDecodeErrorCheck err!\n");
			ret = -1;
		}
		goto END_INAGE_DECODE;
	}
	else
	{
		if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)temp_yout,(u8*)temp_uvout,dst_width,dst_height)<0)
		{
			ret = -1;
			goto END_INAGE_DECODE;
		}
		if(jpeg1_decode_check()<0)
		{
			ret = -1;
			goto END_INAGE_DECODE;
		}
		//ax32xx_mjpegDecodeStop();
		//ax32xx_mjpegDecodeEnable(0);
		
		if(ret>=0)
		{
			ax32xx_mjpegEncodeInit(1,0);
			ax32xx_mjpeg_inlinebuf_init((u8*)temp_yout,(u8*)temp_uvout);
			ax32xx_mjpegEncodeSizeSet(dst_width, dst_height, dst_width, dst_height);
			ax32xx_mjpegEncodeQuilitySet(JPEG_Q_75);
			ax32xx_mjpegEncodeBufferSet((u32)jpegBuffer,(u32)(jpegBuffer)+size);
			ax32xx_mjpegEncodeInfoSet(0);
			ax32xx_mjpeg_manual_on();
			if(ax32xx_mjpeg_encode_check()<0)
			{
				ret = -1;
				goto END_INAGE_DECODE;
			}
			
			if(hal_mjpegDecodePicture(jpegBuffer,arg->yout,arg->uvout,arg->target.width,arg->target.height)<0)
			{
				ret = -1;
				goto END_INAGE_DECODE;
			}

			if(arg->wait) // wait decode end
			{
		//		timer_ticktimer_start();
				timeout = 0;
				while(hal_mjpegDecodeStatusCheck())
				{
					timeout++;
					if(timeout>=0x0fffff)
						break;
				}
		//	    ticktimer_stop();
			}

			if(hal_mjpegDecodeErrorCheck()!=0)
				ret = -1;
			hal_mjpegDecodeSetResolution(src_width,src_height);
		}
	}
	
END_INAGE_DECODE:
	if(arg->media.type != MEDIA_SRC_RAM){
		if(tempBuff)
			hal_sysMemFree((void *)tempBuff);
		else
			hal_sysMemFree((void *)jpegBuffer);
	}
		//shareMemFree(jpegBuffer);
	if(temp_yout)
		hal_sysMemFree((void *)temp_yout);
	temp_yout = temp_uvout = 0;
	
    if(ret<0)
    {
    	deg_Printf("decode fail:ret<0\n");
	    return STATUS_FAIL;
	}

	else
		return STATUS_OK;
}
/*******************************************************************************
* Function Name  : imageDecodeGetResolution
* Description	 : image decode get jpeg resolution
* Input 		 : INT16U *width,INT16U *height
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/
int imageDecodeGetResolution(INT16U *width,INT16U *height)
{
	hal_mjpegDecodeGetResolution(width,height);

	return 0;
}

int imagelDecodeGetResolution(INT16U *width,INT16U *height)
{
	hal_mjpeglDecodeGetResolution(width,height);

	return 0;
}


#endif







