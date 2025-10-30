#include "application.h"
#include "../multimedia/interface/userInterface.h"
#include "../multimedia/watermark/image_watermark.h"

extern msgDealInfor photoEncodeMsgDeal[];
extern sysTask taskPhotoEncode;
#if ENABLE_FLASH_PHOTO
#define JPG_ENC_Q_TABLE_NUM 	8
u8 jpg_encode_q_talbe[JPG_ENC_Q_TABLE_NUM]=
{
	JPEG_Q_27,JPEG_Q_36,JPEG_Q_40,JPEG_Q_42,JPEG_Q_50,JPEG_Q_62,JPEG_Q_75,JPEG_Q_81
};

INT32U sdFreesize =0;
INT32U picSum=0;

void photoImagePreSizeGet(void)
{
	if(SysCtrl.sdcard == SDC_STAT_NULL)
	{
		sdFreesize = 0;//spi_udisk_jpg_free_num();
		picSum=spi_udisk_jpg_free_num();
		deg_Printf("picsum [%d]\n",picSum);
		return ;
	}

	if(SysCtrl.sd_freesize>=REDUCE_2_5M)//5M
	{
		sdFreesize = SysCtrl.sd_freesize;
	}
	else
	{
		sdFreesize =0;
	}
	
	if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_48M)		
		picSum = sdFreesize/1680;
	/*else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_40M)		
		picSum = sdFreesize/1440;*/
	/*else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_24M)		
		picSum = sdFreesize/800;*/
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_20M)		
		picSum = sdFreesize/750;
	/*else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_18M)		
		picSum = sdFreesize/700;
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_16M)		
		picSum = sdFreesize/660;*/
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_12M)		
		picSum = sdFreesize/630;
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_10M)		
		picSum = sdFreesize/600;
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_8M)		
		picSum = sdFreesize/480;
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_5M)		
		picSum = sdFreesize/290;
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_3M)		
		picSum = sdFreesize/130;
	else  if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_2M)		
		picSum = sdFreesize/85;
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_1M)
	{
		if(sdFreesize/70>9999999)
		{
			picSum =9999999;
		}
		else
		{
			picSum = sdFreesize/70;
		}
	}
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_VGA)
	{
		if(sdFreesize/25>9999999)
			picSum =9999999;
		else
			picSum = sdFreesize/25;
	}
}

#endif

void photo_animation_effect(UserInterface name,uint8 flag)
{
	bool change_finir = false;
	disp_frame_t * handle_frame=NULL;
//	u8 frame_static_flag=1;
	uint16 lcd_w,lcd_h;
	u8* yuv_buff = NULL;
	while(1)
	{
		//deg_Printf("a..a handel:%d\n",SysCtrl.photo_software_handle);
		if(0==SysCtrl.photo_software_handle)
		{
			//deg_Printf("b..b\n");
			
			handle_frame=yuv420_software_get_handle_frame();
			
			if(!yuv420_software_get_handle_frame())
				deg_Printf("empty\n");

			if(handle_frame){
				SysCtrl.photo_software_handle=1;

				///-------- photo frame
				if(2==SysCtrl.photo_mode_switch)	// effect mode
				{
					yuv420_effect_handle(handle_frame->y_addr,handle_frame->uv_addr,handle_frame->w,handle_frame->h);
				}
				else if(3==SysCtrl.photo_mode_switch)	// frame mode
				{
					//photo frame add to lcd
					hal_custom_frame_add_lcd(handle_frame->y_addr);
				}
				else	// face det handle 
				{
					
				}

				///-------
			}
			//deg_Printf("...\n");
		}
		else if(1==SysCtrl.photo_software_handle)
		{
			if((SysCtrl.printer_en)&&(taskCurrent()==TASK_PHOTO_ENCODE))//拍照静帧操作
			{
				//deg_Printf("c..c\n");
				SysCtrl.photo_software_handle=2;

				if(flag)
				{
					flag=0;
					hal_lcdGetResolution(&lcd_w,&lcd_h);
					lcd_w=(lcd_w + 0x1f) & (~0x1f); // add 32bit align
					memcpy(name.preFrameBuf,handle_frame->y_addr,lcd_w*lcd_h*3/2);
					ax32xx_sysDcacheFlush((u32)name.preFrameBuf,lcd_w*lcd_h*3/2);
					
					if(yuv_buff == NULL){
						yuv_buff = hal_sysMemMalloc(lcd_w*lcd_h*3/2,64);
						memcpy(yuv_buff,handle_frame->y_addr,lcd_w*lcd_h*3/2);
						ax32xx_sysDcacheFlush((u32)yuv_buff,lcd_w*lcd_h*3/2);
					}
				}

				memcpy(handle_frame->y_addr,yuv_buff,lcd_w*lcd_h*3/2);
				
				ax32xx_sysDcacheWback((u32)handle_frame->y_addr,lcd_w*lcd_h*3/2);
				
				if(!SysCtrl.frame_static)
				{
					change_finir = name.run(&name, handle_frame->y_addr);
					 hal_lcdDealBuffer(handle_frame);
					hal_lcd_get_next_frame();
				}
				else
				{

					// memcpy(handle_frame->y_addr,yuv_buff,lcd_w*lcd_h*3/2);
					 hal_lcdDealBuffer(handle_frame);
					 //hal_lcd_get_next_frame();
					 SysCtrl.lcd_frame_stop = 1;
					 break;
				}
	           
				if(change_finir == true)
				{
					SysCtrl.frame_static = 1; //reset flag
				}
			}
			else
			{
				//deg_Printf("c..c\n");
				SysCtrl.photo_software_handle=2;

				if(flag)
				{
					flag=0;
					hal_lcdGetResolution(&lcd_w,&lcd_h);
					lcd_w=(lcd_w + 0x1f) & (~0x1f); // add 32bit align
					memcpy(name.preFrameBuf,handle_frame->y_addr,lcd_w*lcd_h*3/2);
					ax32xx_sysDcacheFlush((u32)name.preFrameBuf,lcd_w*lcd_h*3/2);
				}
				
				change_finir = name.run(&name, handle_frame->y_addr);
	            hal_lcdDealBuffer(handle_frame);
				hal_lcd_get_next_frame();
				if(change_finir == true)
					break;
			}
		}
	}
	hal_sysMemFree(yuv_buff);
	SysCtrl.frame_static=0;
	deg_Printf("finish read haldle:%d\n",SysCtrl.photo_software_handle);
}

static void task_image_arg(INT16U *width,INT16U *height,INT8U *timestramp)
{
// time stramp	
	*timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
// resolution	
    INT32U value = configValue2Int(CONFIG_ID_PRESLUTION);
    *width = value>>16;
	*height = value&0xffff;
	//*width = 4000;
	//*height = 3000;
	deg_Printf("take photo : [%d:%d]\n",*width,*height);
} 
int task_image_createfile(int channel,char **filename)
{
	int fHandle,ret;
	char *name;
	TCHAR fname[64];

	if(SysCtrl.sdcard!=SDC_STAT_NORMAL)
	{
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
		return -1;
	}


	ret = managerSpaceCheck(SysCtrl.jpg_list,FILEDIR_PHOTO,1024*5);
	deg_Printf("in photo:show ret[%d]\n",ret);
	if(ret<0)
	{
		deg_Printf("sd card full\n");
		SysCtrl.sdcard = SDC_STAT_FULL;
		#if ENABLE_FLASH_PHOTO
		picSum = 0;
		#endif
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
		return -2;
	}
//---------creater file name
	name = managerFileNameCreater(SysCtrl.jpg_list,SUFFIX_JPG,FILEDIR_PHOTO);
	if(name==NULL)
	{
		deg_Printf("photo : create file name fail.\n");
		SysCtrl.sdcard = SDC_STAT_FULL;
		#if ENABLE_FLASH_PHOTO
		picSum = 0;
		#endif
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
		return -2;
	}
//---------open file from file system
    hal_sdLock();
	Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
	fHandle = open(/*name*/fname,FA_CREATE_NEW | FA_WRITE | FA_READ);
	if(fHandle<0)
	{
		deg_Printf("photo : open file fail.%s\n",name);
		
		ret =  -3;
	}
    if(filename)
		*filename = name;
	return fHandle;
}

/*******************************************************************************
* Function Name  : image_take_photo
* Description    : take a photo by user config
* Input          : none
* Output         : none                                            
* Return         : int <0 fail
*******************************************************************************/
int image_take_photo(void)
{
	char *name;
	int fHandle;
	int ret=-1,flag;
	INT16U width,height;
	INT8U timestramp;
	INT8U frame_enable;
	hal_watermarkClose(mediaVideoCtrl.tinfo);
    mediaVideoCtrl.tinfo = hal_watermarkOpen();
    if(mediaVideoCtrl.tinfo<0)
		videor_print("photo watermark open fail:%d\n",mediaVideoCtrl.tinfo);

	u32 _crop_levels = crop_levels[SysCtrl.crop_level]; //limit crop levels range
	if(_crop_levels>100){
		if(SysCtrl.crop_level>ARRAY_NUM(crop_levels)){		//use crop max
			_crop_levels=crop_levels[(sizeof(crop_levels)/sizeof(crop_levels[0]))-1];   // _crop_levels = crop_levels[(ARRAY_NUM(crop_levels))-1]
		}else{
			_crop_levels=100;
		}
	}else if(_crop_levels<crop_levels[(sizeof(crop_levels)/sizeof(crop_levels[0]))-1]){  // _crop_levels < crop_levels[(ARRAY_NUM(crop_levels))-1]
		_crop_levels=crop_levels[(sizeof(crop_levels)/sizeof(crop_levels[0]))-1];
	}
	debg("show __crop_levels[%d] num[%d] min[%d]\n",_crop_levels,(sizeof(crop_levels)/sizeof(crop_levels[0])),crop_levels[(sizeof(crop_levels)/sizeof(crop_levels[0]))-1]);

	debg("levels=%d num=%d\n",SysCtrl.crop_level,ARRAY_NUM(crop_levels));

	task_image_arg(&width,&height,&timestramp);
	if(videoRecordFrameEnGet()) //disable frame
		frame_enable = 1;
	else
		frame_enable = 0;    //enable frame

#if USENSOR_RESOLUTION == 1
	hal_watermarkBPos_Adjust(480);
#elif USENSOR_RESOLUTION == 2
	hal_watermarkBPos_Adjust(720);
#elif USENSOR_RESOLUTION == 3
	hal_watermarkBPos_Adjust(960);
#else
	hal_watermarkBPos_Adjust(480);
#endif
	flag = 0;
//-------------channel A
    if(1)
    {
#if 0

		u8 q;
	
		if(width <= 1280)
		{
			q = JPEG_Q_81;
		}
		else if(width <= 2560)
		{
			q = JPEG_Q_75;
		}
		else if(width <= 3200)
		{
			q = JPEG_Q_62;
		}
		else
		{
			q = JPEG_Q_50;
		}
#endif
	    fHandle = task_image_createfile(VIDEO_CH_A,&name);

		char *newprefix = NULL;
		char *Prefix = PREFIX_JPG;
		strcpy(newprefix ,"/");
		strcat(newprefix ,PREFIX_JPG);
		deg_Printf("newPrefix: %s\n",newprefix);

		if(fHandle<0)
			goto TAKE_PHOTO_END;

		deg_Printf("take photoA : [%d:%d]\n",width,height);
		
		//水印申请空间
		watermark_bmp2yuv_init(ST_PIXEL_W,ST_PIXEL_H,SM_PIC_ST_NUM); //place early
		
		//释放空间以复用
		doublebuf_And_resbuf_Ctrl(0);
		
		if( 1/*configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_20M || configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_48M*/){
			ret = imageEncodeQuadStart(name,newprefix, (FHANDLE)fHandle, width, height, JPEG_Q_81, timestramp,frame_enable,RES_MUSIC_TAKE_PHOTO,WATERMAKE_SET_X_POS,WATERMAKE_SET_Y_POS,WATER_CHAR_GAP,_crop_levels,PIC_16_9);
		}
		#if 0
		else{
			ret = imageEncodeStart((FHANDLE)fHandle,width,height,q,timestramp,frame_enable);
		}
		#endif

		//初始化刚刚释放的空间
		doublebuf_And_resbuf_Ctrl(1);
		
		if(ret<0)
		{
			TCHAR fname[64];
			Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
			deg_Printf("photo : take photo fail.<%d>\n",ret);
			close((FHANDLE)fHandle);
			f_unlink(/*name*/fname);
			ret =  -3;
			goto TAKE_PHOTO_END;
		}
				
		//===handle exif===
#if   (1 == JPEG_EXIF)
		int res;
		u8 *temp_ptr = (u32)hal_sysMemMalloc(fs_size(fHandle),32);
		if(NULL == temp_ptr)
		{
			deg_Printf("image encode : exif handle mem err.\n");
		}
		else
		{
			//memset(temp_ptr,0,fs_size(fHandle)+512);
			res = lseek(fHandle,0x14,0);
			res = read(fHandle,(void *)temp_ptr,fs_size(fHandle)-0x14); 
			u8 *head_ptr = (u32)hal_sysMemMalloc(0x2e8,32);
			if(NULL!=head_ptr)
			{
				u8 datatime[20];
				DATE_TIME_T *rtcTime = hal_rtcTimeGet();
				datatime[0] = (rtcTime->year/1000)%10 +'0';
				datatime[1] = (rtcTime->year/100)%10 +'0';
				datatime[2] = (rtcTime->year/10)%10 +'0';
				datatime[3] = (rtcTime->year)%10 +'0';
				datatime[4] = ':';
				datatime[5] = (rtcTime->month/10)%10 +'0';
				datatime[6] = (rtcTime->month)%10 +'0';
				datatime[7] = ':';
				datatime[8] = (rtcTime->day/10)%10 +'0';
				datatime[9] = (rtcTime->day)%10 +'0';
				datatime[10] = ' ';
				datatime[11] = (rtcTime->hour/10)%10 +'0';
				datatime[12] = (rtcTime->hour)%10 +'0';
				datatime[13] = ':';
				datatime[14] = (rtcTime->min/10)%10 +'0';
				datatime[15] = (rtcTime->min)%10 +'0';
				datatime[16] = ':';
				datatime[17] = (rtcTime->sec/10)%10 +'0';
				datatime[18] = (rtcTime->sec)%10 +'0';
				datatime[19] = 0;
				memset(head_ptr,0,0x2e8);
				res = jpeg_exif_head_init(head_ptr,width,height,datatime);
				head_ptr[0x2e6] = 0xff;
				head_ptr[0x2e7] = 0xD9;
				res = lseek(fHandle,0,0);
				res = write(fHandle,(void *)head_ptr,0x2e8);
				res = write(fHandle,(void *)temp_ptr,fs_size(fHandle)-0x14);
				hal_sysMemFree(head_ptr);
			}
			hal_sysMemFree(temp_ptr);
			deg_Printf("image encode : exif handle filesize 0x%x.\n",fs_size(fHandle));
		}

#endif
		//===end handle exif===

		deamon_fsSizeModify(-1,fs_size(fHandle));
		deg_Printf("SysCtrl.printer_en=%d,size=%d\n",SysCtrl.printer_en,fs_size(fHandle));

		close(fHandle);
		managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&name[strlen(FILEDIR_PHOTO)]);
		deg_Printf("photoA : take photo ok.<%s>\n",name);
		#if ENABLE_FLASH_PHOTO
		if(picSum > 0)
			picSum--;
		#endif
		flag|=1;
    }

    hal_sdStop();	
    ret = 0;
TAKE_PHOTO_END:
	hal_sdUnlock();

    if(flag)
		return 0;
	else
		return -1;
}


int image_take_photo_to_sdram(u32 *jpg_sdram_addr,u32 *jpg_size)
{
	s32 res;
	u32 addr,size,timeout/*,allsize*/;
	void *buff;//,*tbuff;
	s32 sync, sync_next;
	u16 /*image_width,image_height,*/csi_w,csi_h,encode_h;
#if !ENABLE_FLASH_PHOTO
	u8 timestramp;
#else
	u8 timestramp,q_idx;
#endif
	u8 frame_enable;
	u32 *thumber_buff_noCard = NULL;
	
	timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	
	//水印申请空间
	watermark_bmp2yuv_init(ST_PIXEL_W,ST_PIXEL_H,SM_PIC_ST_NUM); //place early

	//释放空间以复用
	doublebuf_And_resbuf_Ctrl(0);


	hal_csiResolutionGet(&csi_w,&csi_h);
	#if PIC_16_9
		u16 half_w720P,half_h720P;
		hal_csiHalf720P_ResolutionGet(&half_w720P,&half_h720P); 
		encode_h = half_h720P;
	#else
		encode_h = csi_h;
	#endif

	if(videoRecordFrameEnGet())
		frame_enable = 1;
	else
		frame_enable = 0;

	hal_mjpegEncodeResolutionImage(csi_w,csi_h,PIC_16_9);
	deg_Printf("no card before malloc, remain:0x%x\n",hal_sysMemRemain());
	if(hal_mjpegMemInit(1)<0)
	{
		deg_Printf("image encode no card: memory malloc fail\n");
		return -1;
	}else{
		deg_Printf("image encode no card: memory malloc success ramain:0x%x\n",hal_sysMemRemain());
	}
	
	u8 q;	
	q = JPEG_Q_27;
	
	#if ENABLE_FLASH_PHOTO
	q_idx = 5;
	#endif
	
	
	hal_mjpegPhotoStart(csi_w,encode_h,q,0,frame_enable,0); //disenable watermark
	

		//thumber_buff_noCard = hal_sysMemMalloc(1024*512,64); //512k

	
	
	timeout = XOSTimeGet();
	//allsize = 0;
	while(1)
	{
		buff = hal_mjpegRawBufferGet(buff,&addr,&size,&sync,&sync_next);
		
		if(buff)
		{
			/*if(thumber_buff_noCard)
			{
				memcpy(thumber_buff_noCard,buff,size);				
				deg_Printf("no Card show size:%d\n",size);
			}else 
				break;
			allsize += size;*/
			thumber_buff_noCard = buff;
			hal_mjpegRawDataSet(buff);
			res = 0;
			break;
		}
		if((timeout+2000)<XOSTimeGet())
		{
			deg_Printf("image encode : timeout 2-second.\n");
			res = -1;
			break;
		}
	}
	//hal_mjpegEncodeStop();

	

	//if(timestramp)
		//videoRecordImageWatermark(image_width,image_height,0);	// disable
		
///--------------
	mjpegEncCtrl.uvbuffer = mjpegEncCtrl.ybuffer+mjpegEncCtrl.csi_width*encode_h;

		system_audio_play(RES_MUSIC_TAKE_PHOTO,1,0);
		ax32xx_sysDcacheWback((u32)thumber_buff_noCard,size);
		
		
		ax32xx_csiEnable(1);
		dispLayerSetPIPMode(DISP_PIP_DISABLE);
		XOSTimeDly(20);
		u32 ticks=XOSTimeGet();
		Image_ARG_T arg;
		arg.target.width = mjpegEncCtrl.csi_width;
		arg.target.height = encode_h;
		arg.yout  = (u8*)mjpegEncCtrl.ybuffer;
		arg.uvout = (u8*)mjpegEncCtrl.uvbuffer;
		arg.media.type = MEDIA_SRC_RAM;
		arg.media.src.buff = (u32)thumber_buff_noCard;//thumbNode;
		arg.wait = 1;  // wait decode end
		if(imageDecodeStart(&arg)<0)
		{
			deg("jpg decode fail\n");
		}
		else
		{
			ticks=XOSTimeGet()-ticks;
			deg("jpg decode ok,ticks=%d\n",ticks);
			u16 dec_width,dec_height;
			imageDecodeGetResolution(&dec_width,&dec_height);
			deg("jpg decode:w=%d,h=%d\n",dec_width,dec_height);
		}
		hal_csiEnable(1);
		dispLayerSetPIPMode(SysCtrl.pip_mode);
		deg_Printf("--------buff free!!\n");
		//hal_sysMemFree(thumber_buff_noCard);  //free temp thumber buffer
		//==end decode jpg==
	

	
		if(mjpegEncCtrl.frame_enable==1)
		{
			//deg_Printf("----------frame_enable %d----------\n",mjpegEncCtrl.line_buf_using);
				hal_custom_frame_add_mjpeg((u8*)mjpegEncCtrl.ybuffer);
				deg_Printf("no card add frame finish!\n");
		}
		
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

	if(timestramp)
	{
		watermark_bmp2yuv_draw((u8*)mjpegEncCtrl.ybuffer,WATERMAKE_SET_X_POS,WATERMAKE_SET_Y_POS,WATER_CHAR_GAP);
		deg_Printf("add watermark finish \n");
	}
	
	


	ax32xx_sysDcacheWback(mjpegEncCtrl.ybuffer,mjpegEncCtrl.csi_width*encode_h*3/2);
	#if PIC_16_9
	hal_mjpegPhotoStart(1280,720,q,0,frame_enable,1);
	#else
	hal_mjpegPhotoStart(csi_w,encode_h,q,0,frame_enable,1);
	#endif

	timeout = XOSTimeGet();
	//allsize = 0;
	while(1)
	{
		buff = hal_mjpegRawBufferGet(buff,&addr,&size,&sync,&sync_next);
		if(buff)
		{
			hal_mjpegRawDataSet(buff);
			res = 0;
			break;
		}
		if((timeout+2000)<XOSTimeGet())
		{
			deg_Printf("image encode : timeout 2-second.\n");
			res = -1;
			break;
		}
	}

	

///-----------------
	deg_Printf("---------noCard_%d__\n",__LINE__);
	//hal_sysMemPrint();

	if(0==res)
	{
		#if SDRAM_SIZE == SDRAM_SIZE_2M //if use 8M RAM,don't need to reuse buf
		if(mjpegEncCtrl.ybuffer)//reuse ybuffer 
		{
			hal_sysMemFree((void *)mjpegEncCtrl.ybuffer);
			mjpegEncCtrl.ybuffer = 0;
		}
		#endif
		u8 *jpeg_buf = (u8*)hal_sysMemMalloc(size,32);
		//u8 *jpeg_buf = mjpegEncCtrl.ybuffer;
		deg_Printf("malloc no card jpgbuf,remain:0x%x jpgSize:%d add:%x\n",hal_sysMemRemain(),size,jpeg_buf);
		if(NULL != jpeg_buf)
		{
			memcpy(jpeg_buf,(void*)addr,size);
			ax32xx_sysDcacheFlush((u32)jpeg_buf,size);
			*jpg_sdram_addr=(u32)jpeg_buf;
			*jpg_size=size;
		}
		else
		{
			deg_Printf("image encode second malloc: memory err ,now remain:0x%x\n",hal_sysMemRemain());
			res=-2;
			goto NO_CARD_PGOTO_END;
		}
		deg_Printf("---------noCard_%d__\n",__LINE__);

		//------------------------------------
	#if ENABLE_FLASH_PHOTO	
		if(spi_udisk_jpg_free_num()==0)
		{
			SysCtrl.photo_err_tip = 1;
		}
		else{
			u32 jpg_addr;
			ENCODE_AGAIN:
			//==scale down image handle==
			#if SDRAM_SIZE == SDRAM_SIZE_2M 
			//res=hal_mjpg_scale_down((u8*)addr,PHOTO_2_FLASH_W,PHOTO_2_FLASH_H,jpg_encode_q_talbe[q_idx],&jpg_addr,&size);
			#else
			res=hal_mjpg_scale_down((u8*)addr,csi_w/2,encode_h/2,jpg_encode_q_talbe[q_idx],&jpg_addr,&size);
			#endif
			jpg_addr = addr;
			ax32xx_sysDcacheWback(jpg_addr,size);
			deg_Printf("read write card jpg size:%d\n",size);
	#if WRITE_CARD
		#if 0
				
				write(fHandle,(void *)jpg_addr,(UINT)size);
				deamon_fsSizeModify(-1,fs_size(fHandle));
				deg_Printf("SysCtrl.printer_en=%d,size=%d\n",SysCtrl.printer_en,fs_size(fHandle));
				close(fHandle);
				managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&name[strlen(FILEDIR_PHOTO)]);
		#endif
	#endif
			deg_Printf("res=%d,%d\n",res,size);
			if(0==res)
			{
				if(size>FLASH_UDISK_JPG_SIZE)
				{
					if(q_idx>0)
					{
						q_idx--;
					}
					goto ENCODE_AGAIN;
				}
				ax32xx_sysDcacheWback(jpg_addr,size);
				spi_udisk_write_jpg((u8 *)jpg_addr,size);
			}
	
		}
	
	#endif
	//------------------------------------	

		
	}
NO_CARD_PGOTO_END:
#if SDRAM_SIZE == SDRAM_SIZE_2M  
	mjpegEncCtrl.mjpbuf = 0;
#endif
	hal_mjpegEncodeStop();
	watermark_buf_bmp2yuv_free();
#if SDRAM_SIZE == SDRAM_SIZE_2M  
	hal_mjpegMjpegReuseUninit();
#endif
	#if ENABLE_FLASH_PHOTO
	if(picSum > 0)
		picSum--;
	photoImagePreSizeGet();
	#endif
	deg_Printf("------------------free ----------------------------\n");
	//初始化刚刚释放的空间
	doublebuf_And_resbuf_Ctrl(1);
	return res;
}

int image_take_photo_to_sdram2(u8 *src_ybuf,u16 src_width,u16 src_height)
{

	u32 addr,size=0;
	u8 ret=0;
	u8 q_idx = 3;
	u8 *jpg_addr;
    u32 jpg_size;
	if(hal_mjpegMemInit(3)<0)
	{
		deg_Printf("image encode : memory malloc fail\n");
		return 1;
	}
//	uint32 timeout = XOSTimeGet();
	
	deg_Printf("---------noCard_%d__\n",__LINE__);
	memcpy(mjpegEncCtrl.ybuffer,src_ybuf,src_width*src_height*3/2);
	ax32xx_sysDcacheWback(mjpegEncCtrl.ybuffer,src_width*src_height*3/2);

    ax32xx_csiISRRegiser(CSI_IRQ_JPG_FRAME_END, NULL);
    ax32xx_csiISRRegiser(CSI_IRQ_SEN_STATE_INT,(void*)NULL);
    ax32xx_csiISRRegiser(CSI_IRQ_VXY_FRAME_EN,NULL);   
    ax32xx_mjpegEA_ncodeISRRegister(NULL);

	//==scale down image handle==
ENCODE_AGAIN:
	ax32xx_mjpegEncodeInit(1,0);
	ax32xx_mjpeg_inlinebuf_init((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer);
	ax32xx_mjpegEncodeSizeSet(src_width, src_height, src_width, src_height);
	ax32xx_mjpegEncodeQuilitySet(jpg_encode_q_talbe[q_idx]);
	ax32xx_mjpegEncodeBufferSet((u32)mjpegEncCtrl.mjpbuf,(u32)(mjpegEncCtrl.mjpbuf+mjpegEncCtrl.mjpsize));
	ax32xx_mjpegEncodeInfoSet(0);
	ax32xx_mjpeg_manual_on();
	if(ax32xx_mjpeg_encode_check()<0)
	{
		ret = -12;
	}

	jpg_addr=(u32)mjpegEncCtrl.mjpbuf;
	jpg_size=(u32)PRE_JPOLTA-(u32)mjpegEncCtrl.mjpbuf;
	deg_Printf("jpg_addr=0x%x,jpg_size=%d\n",jpg_addr,jpg_size);
	deg_Printf("ret = %d\n",ret);

	if(jpg_size>FLASH_UDISK_JPG_SIZE)
	{
		if(q_idx>0)
		{
			q_idx--;
		}
		goto ENCODE_AGAIN;
	}
	spi_udisk_write_jpg((u8 *)jpg_addr,jpg_size);

    //==lcd image active==
    photoImagePreSizeGet();
    hal_wdtClear();
   // hal_csiEnable(1);
   // dispLayerSetPIPMode(SysCtrl.pip_mode);
    //hal_curBuffer_clear();
   // boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
    hal_mjpegEncodeStop();
	watermark_buf_bmp2yuv_free();
	return ret;
}

extern u32 sensor_rotate_flag;
u32 csi_en_flag = 0;
void taskPhotoEncodeOpen(uint32 arg1)
{
	csi_en_flag = 1;
	deg_Printf("image encode task enter.\n");
	hal_mjpegEncodeInit();
	#if HAL_CFG_ISP_SUPPORT
	//isp_switch_senser_set_gain_exp();
	#endif
	
	photo_cmos_init();

//==close frame===
	videoRecordCmdSet(CMD_COM_FRAME,0);
	hal_custom_frame_add_enable(0);
	SysCtrl.photo_frame_idx = 0;
//==normal color===
	SysCtrl.spec_color_index=0;
	cmos_spec_color(SysCtrl.spec_color_index);
//==close effect===
#if ENABLE_FLASH_PHOTO
		photoImagePreSizeGet();
#endif

    SysCtrl.crop_level = 0;
    dispLayerSetFrontCrop(SysCtrl.crop_level,0);

	#if (2 == SENSOR_NUM)
	//sensorInit_table();
	//XOSTimeDly(400);
	#endif

	SysCtrl.pip_mode = DISP_PIP_FRONT;

	
	XOSTimeDly(300);

    dispLayerSetPIPMode(SysCtrl.pip_mode);

	dispLayerInit(DISP_LAYER_VIDEO); // preview

	
	hal_csiEnable(1);

	SysCtrl.photo_task=1;
	SysCtrl.photo_mode_switch=0; 
	SysCtrl.photo_mode_idx=0;
	
	XOSTimeDly(50);
	//msgFuncRegister(photoEncodeMsgDeal);
	uiOpenWindow(&photoEncodeWindow,0);
	//XOSTimeDly(50);
	
	//XOSTimeDly(50);

	//XOSTimeDly(200);
}

void taskPhotoEncodeService(uint32 arg)
{

	photoFocusIconCancel(photoEncodeWindow.handle);
	if(0==SysCtrl.photo_software_handle)	// not effect handle 
	{
		SysCtrl.photo_software_handle=1;
		disp_frame_t * handle_frame=yuv420_software_get_handle_frame();
		if(NULL!=handle_frame)
		{
			if(2==SysCtrl.photo_mode_switch)	// effect mode
			{
				yuv420_effect_handle(handle_frame->y_addr,handle_frame->uv_addr,handle_frame->w,handle_frame->h);
			}
			else if(3==SysCtrl.photo_mode_switch)	// frame mode
			{
				//photo frame add to lcd
				hal_custom_frame_add_lcd(handle_frame->y_addr);
			}
			else	// face det handle 
			{
				
			}

			SysCtrl.photo_software_handle=2;	// software handle finish
            hal_lcdDealBuffer(handle_frame);
			hal_lcd_get_next_frame();
		}
		else
		{
			deg_Printf("null\n");
		}
	}

}
void taskPhotoEncodeClose(uint32 arg)
{
	csi_en_flag = 0;
	SysCtrl.photo_task=0;
	SysCtrl.photo_mode_switch=0; 
	SysCtrl.photo_mode_idx=0;
//==cancal frame===
	videoRecordCmdSet(CMD_COM_FRAME,0);
	hal_custom_frame_add_enable(0);
//==normal color===
	SysCtrl.spec_color_index=0;
	cmos_spec_color(SysCtrl.spec_color_index);
//==close effect===

	if(0==SysCtrl.photo_software_handle)
	{
		SysCtrl.photo_software_handle=1;
		disp_frame_t * handle_frame=yuv420_software_get_handle_frame();
		if(NULL!=handle_frame)
		{
			SysCtrl.photo_software_handle=2;	// effect handle finish
			hal_lcdDealBuffer(handle_frame);
			hal_lcd_get_next_frame();
		}
		else
		{
			deg_Printf("null\n");
		}
	}
	dispLayerSetPIPMode(SysCtrl.pip_mode);
#if FILE_SYNC_DELETE > 0
    while(f_unlink_ext_service());
#endif	


	//dispLayerUninit(DISP_LAYER_VIDEO);
	//imageEncodeUninit();
	videoRecordUninit();
	if(SysCtrl.sdcard == SDC_STAT_FULL)
	{
		SysCtrl.sdcard = SDC_STAT_NORMAL;
	}

	deg_Printf("image encode task exit.\n");
}
sysTask taskPhotoEncode=
{
	"Photo Encode",
	(u32)NULL,
	taskPhotoEncodeOpen,
	taskPhotoEncodeClose,
	taskPhotoEncodeService,
};


