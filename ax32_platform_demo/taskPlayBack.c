#include "application.h"

extern msgDealInfor playBackMsgDeal[];
extern sysTask taskPlayBack;

#define  TASK_VIDEOPLAY_AUTOPLAY         1      //0,playback end,display last frame of current file
                                                         	  		    // 1,playback end,display first frame of current file
                                             				   // 2,playback end,display first frame of next file
#if TASK_VIDEOPLAY_AUTOPLAY >0
    #define  TASK_VIDEOPLAY_FIRSTFRAME      1
#else
    #define  TASK_VIDEOPLAY_FIRSTFRAME      1
#endif


typedef struct Video_Thumbnail_S
{
	char file_name[MANAGER_NAME_LEN+1];
	INT8U index;
	INT8U stat;
	INT16U file_index;
	INT8U *y_buffer;
	INT8U *uv_buffer;
}Video_Thumbnail_T;

//static Video_Thumbnail_T thnumbnailCtrl[THUMBNAIL_CFG_NUM];

void video_playback_jpg_scale(void *src_buf,u16 src_w,u16 src_h,u16 ch_src_xs,u16 ch_src_ys,void *dst_buf,u16 dst_w,u16 dst_h)
{
	u8 *src_ybuf = (u8 *)src_buf;
	u8 *src_uvbuf = (u8 *)src_buf+src_w*src_h;
	u8 *dst_ybuf = (u8 *)dst_buf;
	u8 *dst_uvbuf = (u8 *)dst_buf+dst_w*dst_h;
    u16 i,j;
	deg_Printf("src_w=%d,src_h=%d,dst_w=%d,dst_h=%d,ch_src_xs=%d,ch_src_ys=%d\n",src_w,src_h,dst_w,dst_h,ch_src_xs,ch_src_ys);

	//===y===
	src_ybuf += src_w*ch_src_ys;
	src_ybuf += ch_src_xs;
	for(j=0;j<src_h-ch_src_ys;j++)
	{	

		for(i=0;i<src_w;i++)
		{
			if(i < dst_w)
			{
				*dst_ybuf++ = *src_ybuf++;
			}
			else
			{
				//*src_ybuf++;
				src_ybuf++;
			}
		}

		if(j < dst_h)
		{
			dst_ybuf = (u8 *)dst_buf+(j+1)*dst_w;
		}
		else
		{
			break;
		}
	}
	//===uv===
	src_uvbuf += (src_w/2)*(ch_src_ys/2)*2;
	src_uvbuf += (ch_src_xs/2)*2;
	for(j=0;j<src_h/2-ch_src_ys/2;j++)
	{
		for(i=0;i<src_w/2;i++)
		{
			if(i< dst_w/2)
			{
				*dst_uvbuf++ = *src_uvbuf++;// U
				*dst_uvbuf++ = *src_uvbuf++;// V
			}
			else
			{
				src_uvbuf++;
				src_uvbuf++;
			}
		}

		if(j< dst_h/2)
		{
			dst_uvbuf = (u8 *)dst_buf+dst_w*dst_h+(j+1)*dst_w;
		}
		else
		{
			break;
		}
	}

	ax32xx_sysDcacheFlush((u32)dst_buf,dst_w*dst_h+dst_w*dst_h/2);
}



int video_playback_start(int index)
{
	TCHAR fname[64];
	char *name;
	int type;
	
#if ENABLE_FLASH_PHOTO
	if(SDC_STAT_NULL==SysCtrl.sdcard)
	{
		display_udisk:
		if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
			videoPlaybackStop();
		if(spi_udisk_jpg_num()>0)
		{
			Image_ARG_T arg;
			disp_frame_t *p_lcd_buffer;
			u16 x,y;
			u16 lcd_w,lcd_h;
			u8 *temp_ybuf,*temp_uv_buf;
			SysCtrl.file_type=FILE_TYPE_JPG;
			
			p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
			hal_lcdGetVideoPos(&x,&y);
			dispLayerGetResolution(DISP_LAYER_VIDEO,&lcd_w,&lcd_h);
			deg_Printf("show lcd w[%d] h[%d]\n",lcd_w,lcd_h);
			arg.target.width = lcd_w;
			arg.target.height = lcd_h;
	
			if(p_lcd_buffer)
			{
				deg_Printf("0:x:%d,y:%d,w:%d,h:%d\n",x,y,arg.target.width,arg.target.height);
				hal_dispframeVideoCfg(p_lcd_buffer,x,y,arg.target.width,arg.target.height);
			}
			else
			{
				deg_Printf("0get null buf!!\n");
			}
			arg.yout  = p_lcd_buffer->y_addr;
			arg.uvout = p_lcd_buffer->uv_addr;
	
			deg_Printf("index jpg=%d\n",index);
	
			u8 *jpg;
			u32 jpg_size =spi_udisk_read_jpg_size(index);
			jpg=hal_sysMemMalloc(jpg_size,32);
			deg_Printf("Flash photo Size:%d\n",jpg_size);
			if(NULL==jpg)
			{
				deg_Printf("jpg malloc err!! %d\n",jpg_size);
			}
			else
			{
				u8 ret=spi_udisk_read_jpg(index,jpg);
				ax32xx_sysDcacheWback(jpg,jpg_size);
				if(0!=ret)
				{
					deg_Printf("jpg read %d\n",ret);
				}
			}
			arg.media.type = MEDIA_SRC_RAM;
			arg.media.src.buff = (u32)jpg;
			arg.wait = 1;  // wait decode end
			if(imageDecodeStart(&arg)<0)
			{
				deg_Printf("jpg decode fail\n");
				dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
			}
			if(NULL!=jpg)
				hal_sysMemFree(jpg);
	
			INT16U width,height;
			imageDecodeGetResolution(&width,&height);
			deg_Printf("pass jpg decode\n");
			deg_Printf("show jpeg:w=%d,h=%d\n",lcd_w,lcd_h);
	
			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,lcd_w,lcd_h,VIDEO_ROTATE);
			deg_Printf("video player : JPG[%d:%d]\n",width,height);
		}
	
	}
	else
#endif
	{

		#if ENABLE_FLASH_PHOTO
		if( (spi_udisk_jpg_num()>0)&&(managerFileCount(SysCtrl.avi_list) <= 0) )
		{
			goto  display_udisk;
		}
		#endif
		if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
	          	videoPlaybackStop();
		if(managerFileCount(SysCtrl.avi_list)<1)
			return -1;
		deg_Printf("video play : index = %d ",index);
		name = manangerGetFileFullPathName(SysCtrl.avi_list,index,&type);
		if(name == NULL)
		{
			return -1;
		}
	    SysCtrl.file_index = index;
	    SysCtrl.file_type=type;
	    deg_Printf(": %s\n",name);
		Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
		if(type == FILE_TYPE_JPG)
		{
			Image_ARG_T arg;
			disp_frame_t *p_lcd_buffer;
			u16 x,y;
			u16 lcd_w,lcd_h;
	//		u8 *temp_ybuf,*temp_uv_buf;
			p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
			hal_lcdGetVideoPos(&x,&y);
			dispLayerGetResolution(DISP_LAYER_VIDEO,&lcd_w,&lcd_h);
			if (0 == SysCtrl.jpeg_scale_flag)
			{
				#if (1 == LCD_PLAY_SCALE)
					if((320 == lcd_w)&&(240 == lcd_h))
					{
						x = 0;
						y = 0;
						arg.target.width = 320;
						arg.target.height = 240;
						lcd_w = arg.target.width;
						lcd_h = arg.target.height;
					}
					else
					{
						arg.target.width = lcd_w;
						arg.target.height = lcd_h;
					}
				#else
					arg.target.width = lcd_w;
					arg.target.height = lcd_h;

				#endif
				
				if(p_lcd_buffer)
				{
					deg_Printf("0:x:%d,y:%d,w:%d,h:%d\n",x,y,arg.target.width,arg.target.height);
					hal_dispframeVideoCfg(p_lcd_buffer,x,y,arg.target.width,arg.target.height);
				}
				else
				{
					deg_Printf("0get null buf!!\n");
				}
				arg.yout  = p_lcd_buffer->y_addr;
				arg.uvout = p_lcd_buffer->uv_addr;
			}
			else
			{
				x = 0;
				y = 0;
				arg.target.width = lcd_w;
				arg.target.height = lcd_h;
				
				if(p_lcd_buffer)
				{
					deg_Printf("1:x:%d,y:%d,w:%d,h:%d\n",x,y,arg.target.width,arg.target.height);
					hal_dispframeVideoCfg(p_lcd_buffer,x,y,arg.target.width,arg.target.height);
				}
				else
				{
					deg_Printf("1get null buf!!\n");
				}

				arg.yout  = p_lcd_buffer->y_addr;
				arg.uvout = p_lcd_buffer->uv_addr;
			}

#if (1 == LCD_PLAY_JPEG_SCALE)
			if(0 != SysCtrl.jpeg_scale_flag)
			{
				if(1 == SysCtrl.jpeg_scale_flag)
				{
					arg.target.width = 480;
					arg.target.height = 270;
					x = (arg.target.width-lcd_w)/2;
					y = (arg.target.height-lcd_h)/2;
				}
				else if(2 == SysCtrl.jpeg_scale_flag)
				{
					arg.target.width = 640;
					arg.target.height = 360;
					x = (arg.target.width-lcd_w)/2;
					y = (arg.target.height-lcd_h)/2;
				}
				else if(3 == SysCtrl.jpeg_scale_flag)
				{
					arg.target.width = 960;
					arg.target.height = 540;
					x = (arg.target.width-lcd_w)/2;
					y = (arg.target.height-lcd_h)/2;
				}

				u32 yuv_size = arg.target.width*arg.target.height+arg.target.width*arg.target.height/2;
				arg.yout = hal_sysMemMalloc(yuv_size,32);
				if(NULL == arg.yout)
				{
					deg_Printf("yuv malloc err!! %d\n",yuv_size);
				}
				arg.uvout = arg.yout+arg.target.width*arg.target.height;
				deg_Printf("malloc ok\n");
			}
#endif
			
			arg.media.type = MEDIA_SRC_FS;
			arg.media.src.fd = (FHANDLE)open(/*name*/fname,FA_READ);
			if((int)arg.media.src.fd<0)
				return -1;
			//layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,index,NULL));
			arg.wait = 1;  // wait decode end
			if(imageDecodeStart(&arg)<0)
			{
				deg_Printf("jpg decode fail\n");
				close(arg.media.src.fd);
				dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
				backBrushBlack();
				return -1;
			}
			INT16U width,height;
			imageDecodeGetResolution(&width,&height);
			deg_Printf("pass jpg decode\n");
#if (1 == LCD_PLAY_JPEG_SCALE)
			if(0 != SysCtrl.jpeg_scale_flag)
			{
				deg_Printf("into video_playback_jpg_scale\n");
				video_playback_jpg_scale(arg.yout,arg.target.width,arg.target.height,x,y,p_lcd_buffer->y_addr,lcd_w,lcd_h);
				if(NULL!=arg.yout)
					hal_sysMemFree(arg.yout);
			}
#endif
			deg_Printf("show jpeg:w=%d,h=%d\n",lcd_w,lcd_h);


			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,lcd_w,lcd_h,VIDEO_ROTATE);

			close(arg.media.src.fd);
			
			deg_Printf("video player : JPG[%d:%d]\n",width,height);

		}
		else
		{
			VIDEO_PARG_T arg;
			INT16U width,height;
			
			arg.media.type = MEDIA_SRC_FS;
		    arg.media.src.fd = (FHANDLE)open(/*name*/fname,FA_READ);
			if((int)arg.media.src.fd<0)
				return -1;
	        //layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,index,NULL));
			dispLayerGetResolution(DISP_LAYER_VIDEO,&width,&height);
			arg.tar.width = width;
			arg.tar.height = height;
			hal_lcdGetVideoPos((u16*)&arg.pos.x,(u16*)&arg.pos.y);
			
			deg_Printf("1playback dislayer W:%d , H:%d\n",arg.tar.width,arg.tar.height);
			#if (1 == LCD_PLAY_SCALE)
			if(240 == arg.tar.height)
			{
				arg.pos.x = 0;
				arg.pos.y = 0;
				arg.tar.width = 320;
				arg.tar.height = 240;
			}else 
			{
				arg.pos.x = 0;
				arg.pos.y = 30;
				arg.tar.width = 320;
				arg.tar.height = 180;
			}
			#endif
			//dispRatioResolutionCalculate(VIDEO_RATIO,&arg.tar.width,&arg.tar.height); 
			//arg.pos.x = (width-arg.tar.width)>>1;
			//arg.pos.y = (height-arg.tar.height)>>1;
			arg.rotate = VIDEO_ROTATE;
			arg.firstframe = TASK_VIDEOPLAY_FIRSTFRAME; // pause at first frame waiting user key
			
	        fs_fastseek_init(arg.media.src.fd);  // enable fast seek for this file
	       
			while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end
			if(videoPlaybackStart(&arg)<0)
			{
				playbackPlayTimeCancel();
				backBrushBlack();
				deg_Printf("avi decode fail<0x%x>\n",(int)arg.media.src.fd);
				close(arg.media.src.fd);
				return -1;
			}
			
			Media_Res_T *res= videoPlaybackGetResolution();
			INT32U totatime;
			videoPlaybackGetTime(&totatime,NULL);
			deg_Printf("video player : AVI[%d:%d],total time = %dm %ds %dms\n",res->width,res->height,(totatime/1000)/60,(totatime/1000)%60,totatime%1000);
			(void)res;
		}
	    //layout_video_playback_name(manangerGetFileName(SysCtrl.avi_list,index));
		deg_Printf("video play start.<%s>\n",name);
	}
	XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_TIME_UPDATE,0));
	return type;
	
}



void taskPlayBackOpen(uint32 arg)
{
	deg_Printf("video playback task enter.\n");
	deg_Printf("video a count = %d\n",managerFileCountEx(SysCtrl.avia_list));
	deg_Printf("video b count = %d\n",managerFileCountEx(SysCtrl.avib_list));
	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end
	videoPlaybackInit();
	dispLayerSetPIPMode(DISP_PIP_DISABLE);
	
	#if ENABLE_FLASH_PHOTO
	if(NULL==SysCtrl.sdcard)
	{
		deg_Printf("resad udisk num:%d\n",spi_udisk_jpg_num());
		if(spi_udisk_jpg_num()>0)
		{
			SysCtrl.file_index = 0;
			video_playback_start(SysCtrl.file_index); 
//			exist_file = true;
		}
	}else
	#endif
	{
		if(managerFileCount(SysCtrl.avi_list)>0)
		{
			SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
			video_playback_start(SysCtrl.file_index); 	
			deg("----in taskplayopen:begin show index[%d]----\n",SysCtrl.file_index);
		}
	}
	//msgFuncRegister(playBackMsgDeal);
	uiOpenWindow(&playBackWindow,0);
	
}

void taskPlayBackService(uint32 arg)
{

	static uint32 lastPlayTime=0;
	u32 totoltime=0;
	uint32 playTime=0;

	if(SDC_STAT_NULL==SysCtrl.sdcard)
	{
		return ;
	}
	
	#if TASK_VIDEOPLAY_AUTOPLAY>0
	if((videoPlaybackGetStatus() == MEDIA_STAT_STOP) && (SysCtrl.file_type==FILE_TYPE_AVI))
	{
        #if (TASK_VIDEOPLAY_AUTOPLAY > 1)
		SysCtrl.file_index++;
		if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
			SysCtrl.file_index = 1;
        #endif
		video_playback_start(SysCtrl.file_index);	
	}
    #endif
	videoPlaybackGetTime(&totoltime,&playTime);
	if(playTime>totoltime)
	{
		playTime=totoltime;
	}
	playTime = playTime/1000;
	if(lastPlayTime!=playTime)
	{
		lastPlayTime=playTime;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_TIME_UPDATE,0));
	}

	videoPlaybackService();
}
void taskPlayBackClose(uint32 arg)
{
	if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
          videoPlaybackStop();
	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end	
	videoPlaybackUninit();
	//hal_mjpB_ecdown();
	deg_Printf("video playback task exit.\n");
}
sysTask taskPlayBack=
{
	"Play Back",
	/*NULL*/0,
	taskPlayBackOpen,
	taskPlayBackClose,
	taskPlayBackService,	
};


