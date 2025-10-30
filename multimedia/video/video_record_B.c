/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA video record
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : video_record.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is video record  file
*               
* History     : 
* 2016-09-22  : mark
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
* 2017-07-13  : mark
*      <1>.modify sync timer when fps is lower than 20hz.The timer min-frq is 20hz
*      <2>.add option for insert vids frame.insert a old frame width real data or idx1 only 
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"


extern int video_avi_write(int fd,void *buffer,INT32U len);
extern int video_avi_seek(int fd,int offset,int flag);

/*static void videoRecordTimeServiceB(void)
{
//	videor_print("!");
	if(mediaVideoCtrlB.stat == MEDIA_STAT_START)
	{
		mediaVideoCtrlB.syncCounter++;
		if(mediaVideoCtrlB.syncCounter<mediaVideoCtrlB.syncValue)
			return ;
		mediaVideoCtrlB.syncCounter = 0;
		mediaVideoCtrlB.videoRecordSync++;
		mediaVideoCtrlB.videoRecordTimeCounter+=mediaVideoCtrlB.ftime;
		mediaVideoCtrlB.tCounter+=mediaVideoCtrlB.ftime;
		if(mediaVideoCtrlB.tCounter>=999)
		{
			mediaVideoCtrlB.tCounter = 0;
			//mediaVideoCtrlB.fCounter = 0;
		}
#if VIDEO_DBG_REC_EN >0
		mediaVideoCtrlB.dbg_fps_c_time+=mediaVideoCtrlB.ftime;
#endif
	}		
}
 */
static void videoRecordWatermarkB(void)
{
    INT8U *buffer;
	INT16U w,h;	
	
	char string[20];

	
	buffer = (INT8U *)hal_watermarkRam(mediaVideoCtrlB.tinfo,&w,&h);
    if(buffer && w && h)
    {
		DATE_TIME_T *rtcTime = hal_rtcTimeGet();
		hal_rtcTime2StringExt(string,rtcTime);
	    string[19] = 0;
	    streamSprintf(string,buffer,w,mediaVideoCtrlB.tfont);
    }

}
void videoRecordImageWatermarkB(INT16U width,INT16U height,INT8U en)
{
	INT32U value,inv;
    if(en)
    {
		value = streamFont(width,height);
        if(mediaVideoCtrlB.tfont!=(value&0xff));//change font,
		    hal_watermarkClear(mediaVideoCtrlB.tinfo);
		mediaVideoCtrlB.tfont = value&0xff;
		inv = 8;//((height*10)/240)*8/10;
		//deg_Printf("watermark pos : %d,%d,%d\n",height-(value>>8)-inv,value,inv);
		hal_watermarkPosition(mediaVideoCtrlB.tinfo,4,height-(value>>8)-inv);
		videoRecordWatermarkB(); // update frame
		hal_watermarkEnable(mediaVideoCtrlB.tinfo,1);
    }
	else
		hal_watermarkEnable(mediaVideoCtrlB.tinfo,0);
}
int videoRecordInitB(VIDEO_ARG_T *arg)
{		
	if(arg == NULL)
		return STATUS_FAIL;

    if(arg->fps ==  0)
		arg->fps = 25;  // default using 30fps 
	if(arg->fps == 25)
		mediaVideoCtrlB.arg.sync_fps = 7;
	else if(arg->fps == 30)
		mediaVideoCtrlB.arg.sync_fps = 8;
	//-------------------------video information-------------------------------------------
	mediaVideoCtrlB.arg.audio = arg->audio;
	mediaVideoCtrlB.arg.callback= arg->callback;
	mediaVideoCtrlB.arg.fps = arg->fps;
	mediaVideoCtrlB.arg.looprecord = arg->looprecord;
	mediaVideoCtrlB.arg.rectime = arg->rectime*1000;  // change to ms
	mediaVideoCtrlB.arg.resolution.width= arg->resolution.width;
	mediaVideoCtrlB.arg.resolution.height=arg->resolution.height;
	//mediaVideoCtrlB.arg.quality = arg->quality;
	mediaVideoCtrlB.arg.timestramp = arg->timestramp;
	mediaVideoCtrlB.arg.ftype = arg->ftype;
	mediaVideoCtrlB.arg.syncwr = arg->syncwr;
	mediaVideoCtrlB.stat = MEDIA_STAT_STOP;

	mediaVideoCtrl.aviFd = -1;
	mediaVideoCtrl.idxFd = -1;
	
	mediaVideoCtrlB.tinfo = -1;
	mediaVideoCtrlB.ftime = 1000/arg->fps;
	
	mediaVideoCtrlB.mjpBStart=false;
	mediaVideoCtrlB.restart_flag=true;
	//mediaVideoCtrlB.stop_flag=true;
	mediaVideoCtrlB.firstframe=true;
	mediaVideoCtrlB.dropframecount=0;
		
//-----------initial sensor-----------------------------		
//	sensor_actech_to_csi(); // initial cmos sensor
//-----------initial timestramp------------------------
  /*  mediaVideoCtrlB.tinfo = hal_watermarkOpen();
    if(mediaVideoCtrlB.tinfo<0)
		videor_print("video record : watermark open fail\n");
	else
		hal_watermarkCallbackRegister(mediaVideoCtrlB.tinfo,videoRecordWatermarkB);
   */ 
	mediaVideoCtrlB.mjpbuff_full=false;
	mediaVideoCtrlB.usensor_in = false;
  
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : videoRecordUninit
* Description    : uninitial video record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoRecordUninitB(void)
{
#if MEDIA_CFG_SYNC_TIMER_EN == 0
    XWorkDestory(mediaVideoCtrlB.videoSync);
#else
	hal_timerStop(MEDIA_CFG_TIMER);
#endif
	
	mediaVideoCtrlB.videoSync = NULL;
    hal_watermarkClose(mediaVideoCtrlB.tinfo);
	mediaVideoCtrlB.tinfo = -1;
	if(aviIdx1CacheB)
		hal_sysMemFree(aviIdx1CacheB);
	aviIdx1CacheB = NULL;


    
	return STATUS_OK;
}

/*******************************************************************************
* Function Name  : videoRecordGetStatus
* Description    : get video record 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoRecordGetStatusB(void)
{
	return mediaVideoCtrlB.stat;
}
/*******************************************************************************
* Function Name  : videoRecordGetTime
* Description    : get video record time in ms
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoRecordGetTimeB(void)
{
	return ((aviEncodeGetFrameCount(mediaVideoCtrlB.fencoder)/mediaVideoCtrlB.arg.fps)*1000);
//	return mediaVideoCtrl.videoRecordTimeCounter;
}

int videoRecordFileStartB(void)
{
	int ret;
	FHANDLE fd[2];

	if(mediaVideoCtrlB.arg.callback == NULL)
		return STATUS_FAIL;

    hal_sdLock();

	ret = mediaVideoCtrlB.arg.callback(VIDEO_CH_B,CMD_VIDEO_RECORD_START,(INT32U)&fd[0]); // user create file
	if(ret<0)
	{
		hal_sdUnlock();
		return STATUS_FAIL;   // start fail
	}
    mediaVideoCtrlB.space = ret;
	//deg_Printf("space:%d\n",mediaVideoCtrlB.space);
	if(mediaVideoCtrlB.arg.ftype == AVI_TYPE_STANDARD)
		mediaVideoCtrl.space = mediaVideoCtrlB.space;
	mediaVideoCtrlB.aviFd = fd[0];
    if(mediaVideoCtrlB.arg.ftype == AVI_TYPE_OPENDML)
    {
		mediaVideoCtrlB.idxFd = fd[0]; // using the same file when fast write enable
	//	fs_fastwrite_start(mediaVideoCtrl.aviFd,0);
		videor_print("video record B: AVI TYPE OPEN DML.\n");
    }
    else if(mediaVideoCtrlB.arg.ftype == AVI_TYPE_STANDARD)
    {
	     mediaVideoCtrlB.idxFd = fd[1];
		 if(aviIdx1CacheB==NULL)
			 aviIdx1CacheB = hal_sysMemMalloc(AVI_CFG_IDX1_BUFF_SIZE,64);
		 videor_print("video record B: AVI type->STANDARD.\n");
    }
	else
	{
		videor_print("video record B: unknow AVI type.%d\n",mediaVideoCtrlB.arg.ftype);
		hal_sdUnlock();
		return STATUS_FAIL;   // start fail
	}
	ret = aviEncodeInit(mediaVideoCtrlB.arg.ftype,video_avi_write,video_avi_seek);
	if(ret < 0)
	{
		aviEncodeUninit(mediaVideoCtrlB.fencoder);
		ret = aviEncodeInit(mediaVideoCtrlB.arg.ftype,video_avi_write,video_avi_seek);
	}	
	mediaVideoCtrlB.fencoder = ret;
	//mediaVideoCtrlB.fencoder = aviEncodeInit(mediaVideoCtrlB.arg.ftype,video_avi_write);

	ret = aviEncodeParse(mediaVideoCtrlB.fencoder,mediaVideoCtrlB.aviFd,mediaVideoCtrlB.arg.resolution.width,mediaVideoCtrlB.arg.resolution.height,mediaVideoCtrlB.arg.fps,1,MEDIA_CFG_AVI_SAMPLERATE); // audio off
	if(ret<0)
	{
		videor_print("video record B: encode parse fail.%d\n",ret);
		mediaVideoCtrlB.arg.callback(VIDEO_CH_B,CMD_COM_ERROR,(INT32U)&fd[0]); // video start fail
		hal_sdUnlock();
		return STATUS_FAIL;
	}
	mediaVideoCtrlB.avihead = ret;
  //  mediaVideoCtrlB.error = false;
    mediaVideoCtrlB.videoRecordTimeCounter = 0;
    mediaVideoCtrlB.videoRecordSync = 0;// sync clear
    mediaVideoCtrlB.fCounter = 0;
	mediaVideoCtrlB.tCounter = 0;
	mediaVideoCtrlB.fStart = 0;
	mediaVideoCtrlB.videoRecordAudioCnt = 0;
	mediaVideoCtrlB.videoFrameSize = 0;
	mediaVideoCtrlB.syncCounter = 0;
	mediaVideoCtrlB.write_flag=false;
	mediaVideoCtrlB.looptime=false;
	//mediaVideoCtrlB.firstframe=true;
	//mediaVideoCtrlB.dropframecount=0;
#if VIDEO_DBG_REC_EN >0    
    mediaVideoCtrlB.dbg_fps_c_time = 0;
	mediaVideoCtrlB.dbg_fps_n_cnt  = 0;
	mediaVideoCtrlB.dbg_fps_e_cnt = 0;
	mediaVideoCtrlB.dbg_fps_s_cnt = 0;
#endif
	mediaVideoCtrlB.mjpbuff_full=false;
	mediaVideoCtrlB.mjp_last_write=false;
	mediaVideoCtrlB.insertframe=false;
//	mediaVideoCtrlB.jframe_cnt=0;
	mediaVideoCtrlB.u_tsync_old=0;
	mediaVideoCtrlB.abchangetime=0;
	mediaVideoCtrlB.iframe_ctr=0;
	mediaVideoCtrlB.dropframecount=0;
	
  return STATUS_OK;
}
int videoRecordFileStopB(void)
{
	INT32U length;
    FHANDLE fd[2];

	videor_print("video record B: ready to stop\n");
	mediaVideoCtrlB.stat= MEDIA_STAT_STOP;
	mediaVideoCtrlB.mjpBStart=false;
	mediaVideoCtrlB.usensor_in = false;
    if(mediaVideoCtrlB.arg.ftype == AVI_TYPE_OPENDML)
    {
	     aviEncodeEnd(mediaVideoCtrlB.fencoder,NULL,0,0);  // 
    }
    else
    {
	//---------------finish index file & update idx1 length
	    length = aviEncodeGetIdx1Length(mediaVideoCtrlB.fencoder)%AVI_CFG_IDX1_BUFF_SIZE;
		write(mediaVideoCtrlB.idxFd,aviIdx1CacheB,length);  // write remainning idx1 data

		fs_readsector(mediaVideoCtrlB.idxFd,aviIdx1CacheB,1);  // read 1-secotor from first sector
	    aviEncodeSetIdx1(mediaVideoCtrlB.fencoder,aviIdx1CacheB);  // update idx1 length
		fs_writesector(mediaVideoCtrlB.idxFd,aviIdx1CacheB,1);

	//--------------merge avi file & index file
		fs_merge(mediaVideoCtrlB.aviFd,mediaVideoCtrlB.idxFd);  

	    lseek(mediaVideoCtrlB.aviFd,0,0);
	    read(mediaVideoCtrlB.aviFd,aviIdx1CacheB,mediaVideoCtrlB.avihead);

		length = fs_size(mediaVideoCtrlB.aviFd);

		aviEncodeEnd(mediaVideoCtrlB.fencoder,aviIdx1CacheB,length,length-aviEncodeGetIdx1Length(mediaVideoCtrlB.fencoder));

		lseek(mediaVideoCtrlB.aviFd,0,0);
	    write(mediaVideoCtrlB.aviFd,aviIdx1CacheB,mediaVideoCtrlB.avihead);
	}
    aviEncodeUninit(mediaVideoCtrlB.fencoder);
   
	if(mediaVideoCtrlB.arg.callback)
    {
		fd[0] = mediaVideoCtrlB.aviFd;
		fd[1] = mediaVideoCtrlB.idxFd;
		mediaVideoCtrlB.arg.callback(VIDEO_CH_B,CMD_VIDEO_RECORD_STOP,(INT32U)&fd[0]); 
    }
   // hal_sdStop();
	
  // hal_sdUnlock();
	mediaVideoCtrlB.aviFd = -1;
	mediaVideoCtrlB.idxFd = -1;
	
	videor_print("video record B : stop.total frame = %d\n",aviEncodeGetFrameCount(mediaVideoCtrlB.fencoder));
	videor_print("video record B: audio time = %dms,video time = %dms,timer = %dms\n",mediaVideoCtrlB.videoRecordAudioCnt/32,(aviEncodeGetFrameCount(mediaVideoCtrlB.fencoder)*1000)/30,mediaVideoCtrl.videoRecordTimeCounter);
    return STATUS_OK;
}
int videoRecordFileErrorB(void)
{
	FHANDLE fd[2];
	fd[0] = mediaVideoCtrlB.aviFd;
	fd[1] = mediaVideoCtrlB.idxFd;
	mediaVideoCtrlB.arg.callback(VIDEO_CH_B,CMD_COM_ERROR,(INT32U)&fd[0]);   // close file & release file handle
	aviEncodeUninit(mediaVideoCtrlB.fencoder);
	mediaVideoCtrlB.stat = MEDIA_STAT_STOP;
	mediaVideoCtrlB.mjpBStart=false;
	mediaVideoCtrlB.restart_flag=true;
	mediaVideoCtrlB.usensor_in=false;
	return STATUS_FAIL;
}
int videoRecordStopB(void)
{

 //   mediaVideoCtrlB.error = false;
    if(mediaVideoCtrlB.stat != MEDIA_STAT_START)
	{
		if(mediaVideoCtrlB.usensor_in){
			//mediaVideoCtrlB.usensor_in = false;
			videoRecordFileStopB();
		}
		return STATUS_OK;
	}
	videor_print("video record B : stop\n");
	api_mjp_avi_stop();
	//mediaVideoCtrlB.stop_flag=true;
//	hal_auadcStop();

	//videoRecordImageWatermarkB(0,0,0);
  
	return videoRecordFileStopB();
}
void videoRecordErrorB(u8 err)
{
    //if(err<5)  // sdc read /write error
    {
#if MEDIA_CFG_SYNC_TIMER_EN == 0
    XWorkDestory(mediaVideoCtrl.videoSync);
#else
	hal_timerStop(MEDIA_CFG_TIMER);
#endif		
		hal_mjpegEncodeStop();
		
		hal_auadcStop();
		
		api_mjp_avi_stop();
		
		videoRecordImageWatermark(0,0,0);
		
		if(mediaVideoCtrlB.arg.callback)
	    {
			FHANDLE fd[2];
			fd[0] = mediaVideoCtrlB.aviFd;
			fd[1] = mediaVideoCtrlB.idxFd;
			mediaVideoCtrlB.arg.callback(VIDEO_CH_B,CMD_COM_ERROR,(INT32U)&fd[0]);   // close file & release file handle
	    }/**/

		//hal_sdStop();
		
	    //hal_sdUnlock();
		aviEncodeUninit(mediaVideoCtrlB.fencoder);
		mediaVideoCtrlB.stat = MEDIA_STAT_STOP;
		mediaVideoCtrlB.mjpBStart=false;
		if(mediaVideoCtrl.stat == MEDIA_STAT_START)
		{
			videoRecordError(0);
		}
    }/*
    else
	{	
	    videoRecordStopB();
	}
	 */
	videor_print("video record B : error\n");
}
int videoRecordRestartB(void)
{
//	mediaVideoCtrlB.error = false;
    if(mediaVideoCtrlB.stat != MEDIA_STAT_START)
		return STATUS_OK;
	int stime=0;
	stime++;
    stime = XOSTimeGet();
	videoRecordFileStopB();
	videor_print("video record B : restart.%d\n",XOSTimeGet()-stime);
	return STATUS_OK;
}
int videoRecordUpdateAviIdx1B(INT8U type,INT32U len,INT8U key)
{
	INT32S ret;

	ret = aviEncodeAddIdx(mediaVideoCtrlB.fencoder,aviIdx1CacheB,type,key,len,AVI_CFG_IDX1_BUFF_SIZE);
	if(ret<=0)
		return ret;
	if(mediaVideoCtrlB.arg.ftype == AVI_TYPE_STANDARD)

	{
		ret = write(mediaVideoCtrlB.idxFd,aviIdx1CacheB,AVI_CFG_IDX1_BUFF_SIZE);
		if(ret<0)
			return -1;
		if(ret!=AVI_CFG_IDX1_BUFF_SIZE)
			return -1;

		fs_bound(mediaVideoCtrlB.aviFd,mediaVideoCtrlB.idxFd);
        if(mediaVideoCtrl.arg.syncwr)
        {
		    u32 size,*ptemp;
		    ptemp = (u32 *)aviIdx1CacheB;
			
			fs_readsector(mediaVideoCtrlB.idxFd,aviIdx1CacheB,1);  // read 1-secotor from first sector
		    ptemp[3] = fs_size(mediaVideoCtrlB.idxFd)-16;
			fs_writesector(mediaVideoCtrlB.idxFd,aviIdx1CacheB,1);
		    size = fs_size(mediaVideoCtrlB.idxFd);
			
			size +=fs_size2(mediaVideoCtrlB.aviFd);
		    fs_readsector(mediaVideoCtrlB.aviFd,aviIdx1CacheB,mediaVideoCtrlB.avihead>>9);
			aviEncodeEnd(mediaVideoCtrlB.fencoder,aviIdx1CacheB,size,size-aviEncodeGetIdx1Length(mediaVideoCtrlB.fencoder));
			fs_writesector(mediaVideoCtrlB.aviFd,aviIdx1CacheB,mediaVideoCtrlB.avihead>>9);
	    }
	}
	return 0;
}
int videoRecordServiceB(void)
{
//	static INT8U errFlag = 0;
	INT32S res,icnt;
	INT32U insert;
	INT32U len,id,addr1,size1;
	s32 u_tsync,u_tsync_next, auds_tsync, auds_tsync_next;
	u8 *pbuf;
	void *buff_auds;
	INT32U *temp1;
	s32 alignsize_B=0;
	bool mjpB_auds_flag=false;
	if(mediaVideoCtrl.stat != MEDIA_STAT_START)
		return mediaVideoCtrl.stat;
//--------------backrec sensor online check
	if((!backrec_adpt())&&(mediaVideoCtrlB.mjpBStart))
	{
		mediaVideoCtrlB.restart_flag=true;
		mediaVideoCtrlB.usensor_in = false;
		if(STATUS_OK!=videoRecordStop()){
			videor_print("usensor off but file stop fail.\n");
			return mediaVideoCtrlB.stat;
		}
		else{
			videor_print("usensor off and file stop sucess.\n");
			if(!backrec_lowerpower()){  //when battery is low power, not start record
				videoRecordStart();
			}
			return mediaVideoCtrlB.stat;
		}
	}
	if(!backrec_adpt())
	{
		if(mediaVideoCtrlB.usensor_in == false)
			mediaVideoCtrlB.restart_flag = true;
		return mediaVideoCtrlB.stat;	
	}
//----------File start and mjpB on
	if(!mediaVideoCtrlB.mjpBStart)
	{
		/*if(mediaVideoCtrlB.error == true)
			return mediaVideoCtrlB.stat;
		*/
		
		if(mediaVideoCtrlB.restart_flag)
		{
			
			mediaVideoCtrlB.restart_flag=false;
			/*if(!mediaVideoCtrlB.stop_flag){
				videoRecordRestart();
				return mediaVideoCtrlB.stat;
			}		
			else*/
			{
				videoRecordStop();
				videoRecordStart();
				return mediaVideoCtrlB.stat;
			}
		}
		else if (mediaVideoCtrlB.usensor_in){
			if(hal_auadcMemInitB()<0) // auadc memory malloc 
			{
				videor_print("video record B: auadc memory malloc fail.\n");
				goto VIDEO_REC_ERROR;
			}
			if(hal_mjpB_actech_as_avi(mediaVideoCtrlB.arg.resolution.width,mediaVideoCtrlB.arg.resolution.height,0)<0)
			{
				videor_print("video record B : mjpegB start fail\n");
				goto VIDEO_REC_ERROR;
			}
			hal_auadcStartForAVIB(MEDIA_CFG_AVI_SAMPLERATE,mediaVideoCtrlB.arg.audio*100); // set adc for audio sample rate
			mediaVideoCtrlB.mjpBStart=true;
			mediaVideoCtrlB.stat = MEDIA_STAT_START;
			videor_print("video record B: [%d:%d]start\n",mediaVideoCtrlB.arg.resolution.width,mediaVideoCtrlB.arg.resolution.height);
			return mediaVideoCtrlB.stat;
		}
	/*	if(videoRecordFileStartB()!=STATUS_OK)  
		{
			//mediaVideoCtrlB.error = true;
			videoRecordErrorB(0);
			return mediaVideoCtrlB.stat;
		}	 
		if(mediaVideoCtrlB.stop_flag)
		{
			if(hal_mjpB_actech_as_avi(mediaVideoCtrlB.arg.resolution.width,mediaVideoCtrlB.arg.resolution.height,0)<0)

			{
				goto VIDEO_REC_ERROR;
			}
			mediaVideoCtrlB.stop_flag=false;
		}
		mediaVideoCtrlB.mjpBStart=true;
		mediaVideoCtrlB.stat = MEDIA_STAT_START;
*/

		//debg("resolutionB:%d:%d",mediaVideoCtrl.arg.resolutionB.width,mediaVideoCtrl.arg.resolutionB.height);
			
	}
	if(mediaVideoCtrlB.stat != MEDIA_STAT_START)
		return mediaVideoCtrlB.stat;
	if(!mediaVideoCtrlB.mjpBStart){
		return mediaVideoCtrlB.stat;
	}
//--------------mjpB buffer full check: if prefull,write data
//loop record control for A&B record
	if(mediaVideoCtrlB.looptime)
		return mediaVideoCtrlB.stat;
	if(mediaVideoCtrl.looptime)
		goto VIDEO_RECORD_VIDEO_FRAME;
	//--------------fill 32k align junk
  	if((mediaVideoCtrlB.mjp_last_write)&&(mediaVideoCtrlB.arg.ftype == AVI_TYPE_OPENDML))//align 32k
	{
		alignsize_B=hal_sdNextLBA()&(0x3f);
		if(alignsize_B)
		{
			temp1 = (INT32U *)audiobuff;
			temp1[0]=0x4B4E554A;
			temp1[1]=(64-alignsize_B)*512-8;
			len=(64-alignsize_B)*512;
			res = write(mediaVideoCtrlB.aviFd,(void *)audiobuff,len);
			if(res!=len)
			{
				sysLog("error no 5");
				res = 4;
				goto VIDEO_RECORD_ERROR;
			}
			aviEncodeAddofs(mediaVideoCtrlB.aviFd,len);
			//videor_print("Fill B Junk:%x\n",len);
			
		}
		mediaVideoCtrlB.mjp_last_write=false;
		mediaVideoCtrlB.write_flag=false;
		return mediaVideoCtrlB.stat;
	}
	if(mediaVideoCtrl.mjpA_write_first) //when mjpA and mjpB buff are full at the same time,not write mjpB
		return mediaVideoCtrlB.stat;
		
	//when mjpB buff is not pre full, not write data,but check whether buff pre full
	if(!mediaVideoCtrlB.mjpbuff_full){ 
		if(!hal_mjpB_Buffer_prefull()){
			mediaVideoCtrlB.mjpbuff_full=false;
			mediaVideoCtrl.mjpbuff_full=true;
			
			return mediaVideoCtrlB.stat;
		}
		else{
			mediaVideoCtrlB.mjpbuff_full=true;
			mediaVideoCtrl.mjpbuff_full=false;
			if(mediaVideoCtrlB.fStart){
				mediaVideoCtrl.mjp_last_write=true;//judge mjpA last write,have to align 32k
				//if(hal_mjpB_Buffer_prefull()) deg_Printf("mjpB buff full\n");
				mediaVideoCtrlB.abchangetime = XOSTimeGet();	
			//	videor_print("video B write,A->B change time:%dms\n",mediaVideoCtrlB.abchangetime-mediaVideoCtrl.abchangetime);
			}
			else{
				mediaVideoCtrlB.abchangetime = XOSTimeGet();	
			}
			return mediaVideoCtrlB.stat;
		}
	}	
//-----------------video frame-----------if system bus is enough,user can add one more functions to take photo when recording
VIDEO_RECORD_VIDEO_FRAME:
	mediaVideoCtrlB.write_flag=true;
	if(hal_mjpB_usensor_fram_rd(&pbuf, &len, &id,&u_tsync, &u_tsync_next)){
		//hal_mjpB_usensor_fram_free();
		//debg(">");
	}
	buff_auds = hal_auadcBufferGetB(&addr1,&size1,&auds_tsync, &auds_tsync_next);
		
	
	if ((!mediaVideoCtrlB.fStart)&&(auds_tsync!=0)&&((u_tsync>=auds_tsync)||(!pbuf)))//err: auds start before vids 
	{
		videor_print("<warning!>VIDEOB auds start before vids\n");
		if(buff_auds)
			hal_auadcBufferReleaseB(buff_auds);
		mediaVideoCtrlB.jframe_cnt=auds_tsync;
		mediaVideoCtrlB.u_tsync_old=auds_tsync;
		mediaVideoCtrlB.iframe_ctr=0;
		return mediaVideoCtrlB.stat;
	}
	if (((u_tsync>=auds_tsync+mediaVideoCtrlB.arg.sync_fps)||((u_tsync>=mediaVideoCtrlB.jframe_cnt+mediaVideoCtrlB.arg.sync_fps)&&(u_tsync>=auds_tsync)))\
		&&(pbuf)&&(buff_auds)) //when drop too many vids frame
	{
		deg_Printf("VIDEOB vids:%d,auds:%d,fc:%d\n",u_tsync,auds_tsync,mediaVideoCtrlB.jframe_cnt);
		if(buff_auds)
		{
			videor_print("<warning!>VIDEOB drop too many vids\n");
			hal_auadcBufferReleaseB(buff_auds);
			mediaVideoCtrlB.jframe_cnt=auds_tsync;
			mediaVideoCtrlB.u_tsync_old=auds_tsync;
			mediaVideoCtrlB.iframe_ctr=0;
			return mediaVideoCtrlB.stat;
		}
	}
	if ((pbuf)&&(u_tsync>=mediaVideoCtrlB.jframe_cnt+mediaVideoCtrlB.arg.sync_fps))
	{
		deg_Printf("VIDEOB vids:%d,auds:%d,fc:%d\n",u_tsync,auds_tsync,mediaVideoCtrlB.jframe_cnt);
		mediaVideoCtrlB.jframe_cnt=u_tsync;
		mediaVideoCtrlB.iframe_ctr=0;
		
	}
	if((u_tsync==auds_tsync)&&(buff_auds)&&(mediaVideoCtrlB.iframe_ctr == 0)){
		
		hal_auadcBufferReleaseB(buff_auds);
		mediaVideoCtrlB.jframe_cnt=auds_tsync;
		return mediaVideoCtrlB.stat;
	}
	if((!mediaVideoCtrlB.fStart)&&(pbuf))
	{
		mediaVideoCtrlB.u_tsync_old=u_tsync;
		//mediaVideoCtrl.jframe_cnt = sync_vids;
		mediaVideoCtrlB.iframe_ctr=0;
	}
	if((!pbuf)&&(buff_auds))//write auds frame
		mjpB_auds_flag=true;
	icnt = 0;		


//    fflag = 0;
	if((pbuf)&&(len))
	{
		if(	mediaVideoCtrlB.firstframe)
		{
			hal_mjpB_usensor_fram_free();
			mediaVideoCtrlB.dropframecount++;
			mediaVideoCtrlB.videoRecordSync--;
			mediaVideoCtrlB.jframe_cnt++;
			mediaVideoCtrlB.iframe_ctr++;
			if(mediaVideoCtrlB.u_tsync_old != u_tsync)
			{
				//mediaVideoCtrlB.jframe_cnt=0;
				mediaVideoCtrlB.iframe_ctr=0;
				mediaVideoCtrlB.u_tsync_old = u_tsync;
			}
			if(mediaVideoCtrlB.dropframecount>Framedrop_back){
				mediaVideoCtrlB.firstframe=false;
				//mediaVideoCtrlB.dropframecount=0;
			}
			
		}
		else{
			if(mediaVideoCtrlB.u_tsync_old==u_tsync)
			{
				mediaVideoCtrlB.iframe_ctr++;	
			}else{
				mediaVideoCtrlB.iframe_ctr = 0;
				mediaVideoCtrlB.icnt_control = u_tsync-mediaVideoCtrlB.u_tsync_old;
				if(mediaVideoCtrlB.icnt_control>mediaVideoCtrlB.arg.sync_fps)  mediaVideoCtrlB.icnt_control = mediaVideoCtrlB.arg.sync_fps;
				mediaVideoCtrlB.u_tsync_old = u_tsync;
			}	
			icnt=0;
VIDEO_RECORD_SYNC_FPS:
			
			if(len>mediaVideoCtrlB.videoFrameSize)
				mediaVideoCtrlB.videoFrameSize = len;
			aviEncodeFrameTag(mediaVideoCtrlB.fencoder,(INT32U *)pbuf,AVI_FRAME_DC_VIDEO,len-8);
			
			insert = 0;	
			if(mediaVideoCtrlB.arg.ftype == AVI_TYPE_OPENDML){
				res = write(mediaVideoCtrlB.aviFd,(void *)pbuf,len);
				//debg("2");
			}
			else
				res = write(mediaVideoCtrlB.aviFd,(void *)pbuf,len);	
			
			if(res!=len)
			{	
				deg_Printf("write B fail\n");
				hal_mjpB_usensor_fram_free();
				res = 1;
				goto VIDEO_RECORD_ERROR;
			}
			
			insert+=len;
		
	
			mediaVideoCtrlB.fStart |= 1;  // set video start flag
			if(videoRecordUpdateAviIdx1B(AVI_FRAME_DC_VIDEO,insert,!icnt)<0)
			{
				res = 2;
				hal_mjpB_usensor_fram_free();
				goto VIDEO_RECORD_ERROR;
			}
			mediaVideoCtrlB.jframe_cnt++; //count how many frame of the same u_tsync
			//mediaVideoCtrlB.iframe_ctr++;	//count how many frame of the same u_tsync
			if ((u_tsync_next-u_tsync)==mediaVideoCtrlB.arg.sync_fps){
				if(mediaVideoCtrlB.iframe_ctr==3)
				{
					//videor_print("|");
					mediaVideoCtrlB.iframe_ctr++;
					goto VIDEO_RECORD_SYNC_FPS;
				}
			}
			while(((mediaVideoCtrlB.jframe_cnt)<=u_tsync)&&((auds_tsync==0)||((mediaVideoCtrlB.jframe_cnt)<=u_tsync)))//insert frame
				{
		
				if(auds_tsync){ // need to write auds frame
					mjpB_auds_flag=true;
				}
				//videor_print("|");
				//deg_Printf("jc:%d,sfc:%d,icnt:%d\n",mediaVideoCtrlB.jframe_cnt,u_tsync,icnt);
				if(icnt>3)
				{
					//deg_Printf("jc:%d,ut:%d,ic:%d\n",mediaVideoCtrlB.jframe_cnt,u_tsync,mediaVideoCtrlB.icnt_control);
					deg_Printf("<warning!>mjpB insert (too many)%d frame\n",icnt);
					break;
				}
				#if 0//VIDEO_DBG_REC_EN >0			
					mediaVideoCtrlB.dbg_fps_n_cnt++;
				#endif
				icnt++;
				mediaVideoCtrlB.iframe_ctr++;
				goto VIDEO_RECORD_SYNC_FPS;
			}
			//mediaVideoCtrlB.iframe_ctr = 1;
				//mediaVideoCtrlB.jframe_cnt = 1;
			//	mediaVideoCtrlB.insertframe=false;
				//mediaVideoCtrlB.u_tsync_old=u_tsync;
				
			//}
			if((u_tsync==auds_tsync)&&(auds_tsync!=0)) 
				mjpB_auds_flag = true;

		hal_mjpB_usensor_fram_free();
		}
	}
	
	icnt=0;
//VIDEO_RECORD_AUDIO_FRAME:	
//---------------------audio frame ------------------------------------------------------------
    
		if(mjpB_auds_flag&&(buff_auds))
		{
			//mediaVideoCtrl.space-=audioframesizeB>>10;		
			//	videor_print("*");
			aviEncodeFrameTag(mediaVideoCtrlB.fencoder,(INT32U *)addr1,AVI_FRAME_WD_AUDIO,size1-8);
			if(mediaVideoCtrl.arg.ftype == AVI_TYPE_OPENDML)
				res = write(mediaVideoCtrlB.aviFd,(void *)addr1,size1);
			else
				res = write(mediaVideoCtrlB.aviFd,(void *)addr1,(UINT)size1);
			if(res!=size1)// write error
			{
				res = 4;
				goto VIDEO_RECORD_ERROR;
			}
			
			hal_auadcBufferReleaseB(buff_auds);
			if(videoRecordUpdateAviIdx1B(AVI_FRAME_WD_AUDIO,size1,1)<0)
			{
				res = 2;
				goto VIDEO_RECORD_ERROR;
			}
			mediaVideoCtrlB.fStart|=2;  // audio frame ok
			mediaVideoCtrlB.videoRecordAudioCnt+=size1-8;
			//mediaVideoCtrl.audioframewrite=false;
			mjpB_auds_flag=false;
			
		}

//---------------avi service-----------------------------------------------------------------
	aviEncodeService(mediaVideoCtrlB.fencoder);
//------------------------------time check---------------------------------------------------------
	/*if(mediaVideoCtrlB.arg.looprecord==0)
	{
		if((fs_size(mediaVideoCtrlB.aviFd)>>20)>=3886)
		{
		//	videoRecordStop();
		//	videoRecordStart();
			//if(mediaVideoCtrlB.mjp_last_write)//align 32k
			videoRecordRestart();
			return mediaVideoCtrlB.stat;
		}
	}
	else*/
	{
		
		/*if(mediaVideoCtrl.videoRecordTimeCounter >= mediaVideoCtrl.arg.rectime)*/
		{
			if ((((mediaVideoCtrlB.arg.rectime/1000)*mediaVideoCtrlB.arg.fps)\
			<= (aviEncodeGetFrameCount(mediaVideoCtrlB.fencoder)/* + mediaVideoCtrlB.dropframecount*/))/*\
				||(mediaVideoCtrl.videoRecordTimeCounter >= mediaVideoCtrlB.arg.rectime)*/)
			{
				//videoRecordStop();
				//videoRecordStart(); // restart
				mediaVideoCtrlB.looptime=true;
			/*	if(icnt==0)
				{
					mjpB_auds_flag=true;
					icnt++;
					goto VIDEO_RECORD_AUDIO_FRAME;
				}
				 */
				alignsize_B=hal_sdNextLBA()&(0x3f);
				if(alignsize_B)
				{
					temp1 = (INT32U *)audiobuff;
					temp1[0]=0x4B4E554A;
					temp1[1]=(64-alignsize_B)*512-8;
					len=(64-alignsize_B)*512;
					res = write(mediaVideoCtrlB.aviFd,(void *)audiobuff,len);
					if(res!=len)
					{
						sysLog("error no 5");
						res = 4;
						goto VIDEO_RECORD_ERROR;
					}
					aviEncodeAddofs(mediaVideoCtrlB.aviFd,len);
					//videor_print("Fill B Junk:%x\n",len);
				}
				mediaVideoCtrlB.mjp_last_write=false;
				mediaVideoCtrlB.write_flag=false;
				videor_print("Video B ready to stop.\n");
			
			}
		}
	}
//------------------------------debug information print-----------------------------------------------	

#if 0//VIDEO_DBG_REC_EN >0
    
    mediaVideoCtrlB.dbg_fps_s_cnt += hal_get_fBcnt();
	if(mediaVideoCtrlB.dbg_fps_c_time>=9999) // counting real time fps by 10-second
	{	
		INT32U fpsi,fpsf;		
		videor_print("video record B: [640:480]{%x}-",mediaVideoCtrlB.videoFrameSize);  // reslution
		fpsi = (mediaVideoCtrlB.dbg_fps_s_cnt*1000)/10;
		fpsf = fpsi%1000;
		fpsi/=1000;
		videor_print("[sensor:%d",fpsi);
		if(fpsf)
            videor_print(".%dfps]",fpsf);   // sensor frame rate
        else
			videor_print(".000fps]");   // sensor frame rate
		if(mediaVideoCtrlB.dbg_fps_n_cnt>300)
			mediaVideoCtrlB.dbg_fps_n_cnt = 300;
        fpsi = (mediaVideoCtrlB.dbg_fps_n_cnt*1000)/10;
		fpsf = fpsi%1000;
		fpsi/=1000;
		videor_print("<insert:%d",fpsi);
		if(fpsf)
			videor_print(".%dfps>",fpsf);     //insert frame rate
		else
			videor_print(".000fps>");     //insert frame rate
		res = mediaVideoCtrlB.arg.fps*10-mediaVideoCtrlB.dbg_fps_n_cnt;  // frame cnt in 10-second
		fpsi = (res*1000)/10;
		fpsf = fpsi%1000;
		fpsi/=1000;
		videor_print("frame rate = %d",fpsi);
		if(fpsf)
	        videor_print(".%dfps\n",fpsf);  // mjpeg frame rate
	    else
			videor_print(".000fps\n");  // mjpeg frame rate
       // if(mediaVideoCtrl.dbg_fps_n_cnt>=300)
		//	errFlag =1;
		mediaVideoCtrlB.videoFrameSize = 0;
		mediaVideoCtrlB.dbg_fps_c_time = 0;
        mediaVideoCtrlB.dbg_fps_n_cnt = 0;
        mediaVideoCtrlB.dbg_fps_e_cnt = 0;
		mediaVideoCtrlB.dbg_fps_s_cnt = 0;
		//videor_print("remain space = %dKB\n",mediaVideoCtrl.space);
	//	debg("F:%d,f:%d\n",(mediaVideoCtrl.videoRecordSync/33),aviEncodeGetFrameCount());
	}
#endif
 
    return mediaVideoCtrlB.stat;
	
VIDEO_RECORD_ERROR:
	videor_print("video record B: error no %d.space=%dKB\n",res,mediaVideoCtrl.space);

    videoRecordErrorB(res);  // recorder can not handler this error.only stop recording
	return mediaVideoCtrlB.stat;

VIDEO_REC_ERROR:
	api_mjp_avi_stop();
     videoRecordFileErrorB();
	hal_sdUnlock();
	videor_print("video record B: mjpB start fail\n");
	return mediaVideoCtrlB.stat;
}
void test_null_mjp(void)
{
	/**/
	u8 *pbuf;
	u32 len;
	u32 id;
	s32 u_tsync, u_tsync_next;
	if(hal_mjpB_usensor_fram_rd(&pbuf, &len, &id,&u_tsync, &u_tsync_next)){
		hal_mjpB_usensor_fram_free();
		debg("JBL:%dK\n",len/1024);
	}
}
