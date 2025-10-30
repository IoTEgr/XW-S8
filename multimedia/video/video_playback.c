/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA video playback
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : video_playback.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is video playback file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
*2017-06-10  : mark
*      <1>.Modify vids & auds playing wiith sync
*      <2>.Modify auds playing irq callback
*2017-06-21  : mark
*      <1>.Modify video decode .if mjpeg qdt is dynamic.calling the ext decode function
*2017-06-13  : mark
*      <1>.Modify sync timer.the timer min-frq is 20hz.Add syncValue and syncCnt
*		   to sync play time when fps is lower than 20hz
*2017-07-19  : mark
*      <1>.Modify dacstat check when video file end. 
*      <2>.Modify vids play mode. VIDS_RELEASE_TYPE can be set as 0,1,2
*         0:release current showing buffer at lcd isr,showing at video service
*         1:release and showing at lcd isr
*         2:release and showing at video service
*        Notes:lcd-mcu type suggest to using type 1
*              lcd-rgb type suggest to using type 2
*              if only play 720p video file,using type 0
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"

#include "../../ax32_platform_demo/application.h"


#if  MEDIA_CFG_VIDEO_PLAYBK_EN >0

#define  VIDS_RELEASE_TYPE        2     // int release only
     
#if VIDEO_DBG_PLY_EN
    #define  videop_print     deg_Printf
#else
    #define  videop_print(...)
#endif

typedef struct JPEG_CACHE_S
{
	u32 mem;
	u32 offset;
	u32 count;
}JPEG_CACHE_T;

typedef struct VIDEO_PCTRL_S
{
	INT8U  stat;
	INT8U  dacstat;   // 0 :stop,1:start,2:dac auto stop,3:decode auto stop,4:video first frame
	INT8U  volume;
	INT8U  vflag;

	INT8S  playSpeed;
	INT8U  prepause;
	INT8U  syncValue;
	INT8U  syncCnt;
	
	INT32U samplerate;
	INT32U audiolen;
	
	INT32U audsoffset;
	
	INT32U curframe;
	INT32U nexframe;
	INT32U oldframe;
	INT32U newjpeg;   

	INT32U FPS;
	INT32U PlayFrames;
	INT32U syncFrames; //--new
	INT8U *idx1cache;
	INT8U *jpegcache;
	INT8U *audiobuffer;	
#if VIDEO_DBG_PLY_EN
    INT32U dbg_time;
	INT32U dbg_time_cnt;
    INT32U dbg_frame_cnt;
	INT32U dbg_play_time;
	INT32U dbg_ptime;
#endif
	Media_Res_T src;

	MSG_T *videoframe;
	MSG_T *jpegframe;
	MSG_T *lcdframe;

	XMsg *jpegIdleQ;
	XMsg *jpegBusyQ;

	XMsg *vidsIdleQ;
	XMsg *vidsBusyQ;

	XMsg *audsIdleQ;
	XMsg *audsBusyQ;

	XMsg *audsTimeQ;
	
    XWork_T *videoSync;
	VIDEO_PARG_T arg;
}VIDEO_PCTRL_T;
const static INT8S videoSpeedTable[9] = {-4,-2,1,2,4};  // speed time by second
static VIDEO_PCTRL_T videoPCtrl;//  __attribute__ ((section(".sram_comm")));
static JPEG_CACHE_T videoJpegCache[4],videoShowCache[5];

static JPEG_CACHE_T * vidsMalloc(void)
{
    INT8U err;
    JPEG_CACHE_T * p = (JPEG_CACHE_T *)XMsgQPend(videoPCtrl.vidsIdleQ,&err); 
    if(err != X_ERR_NONE)
        p = NULL;
    else
    {
        u32 mem = (u32)hal_lcdGetIdleBuffer();
        if(mem)
            p->mem = mem;
        else
        {
            XMsgQPost(videoPCtrl.vidsIdleQ,(MSG_T *)p);
            p = NULL;
        }   
    }
    
    return p;
}

static void vidsFree(JPEG_CACHE_T * frame)
{
    if(frame)
    {
        hal_dispframeFree((disp_frame_t *)(frame->mem));
        frame->count = 
        frame->offset =
        frame->mem = 0;
        XMsgQPost(videoPCtrl.vidsIdleQ,(MSG_T *)frame);
    }
}

static void videoPlaybackSync(void)
{
	if(videoPCtrl.stat == MEDIA_STAT_PLAY)
	{
		videoPCtrl.syncCnt++;
		if(videoPCtrl.syncCnt<videoPCtrl.syncValue)
			return ;
		videoPCtrl.syncCnt = 0;
		videoPCtrl.vflag++;
		videoPCtrl.syncFrames++; //--new
	#if VIDEO_DBG_PLY_EN	
		videoPCtrl.dbg_ptime++;
	#endif
	}
}
INT32U playBackGetPlayTime(void)
{
	return (videoPCtrl.PlayFrames/videoPCtrl.FPS);
}
void videoPlaybackDacISR(int flag)
{
	INT8U err;
	if(flag&DAC_INT_PEND)
    {
		hal_dacFlush(videoPCtrl.audiolen);
		if(videoPCtrl.curframe)
		{
			XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)videoPCtrl.curframe);
		}
		videoPCtrl.curframe = videoPCtrl.nexframe;
		videoPCtrl.nexframe = 0;
		return ;
    }

	if(videoPCtrl.prepause)
	{
		if((flag&DAC_INT_EMPTY)==0)
			return ;
		if(videoPCtrl.curframe)
			XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)videoPCtrl.curframe);
		videoPCtrl.curframe = 0;
		hal_dacPlayStop();
	    videoPCtrl.dacstat = 0;
	    videoPCtrl.stat = MEDIA_STAT_PAUSE;
	    hal_sdUnlock();
		return ;
	}

	if(flag&DAC_INT_HALF)
	{
		if(videoPCtrl.nexframe==0)
		{
	         videoPCtrl.nexframe = (INT32U)XMsgQPend(videoPCtrl.audsBusyQ,&err); // get from busy q
			if(err == X_ERR_NONE)
			{
				hal_dacSetBuffer((INT32U)videoPCtrl.nexframe+8,videoPCtrl.audiolen);
			}
		    else 
				videoPCtrl.nexframe = 0;		
		}
	}
	else if(flag&DAC_INT_EMPTY)
	{
		if(videoPCtrl.dacstat == 3)
			videoPlaybackStop();
		else
		{
/*
			hal_dacPlayStop();
			if(videoPCtrl.curframe)
				XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)videoPCtrl.curframe);
			if(videoPCtrl.nexframe)
				XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)videoPCtrl.nexframe);
			videoPCtrl.curframe = 0;
			videoPCtrl.nexframe = 0;
		    videoPCtrl.dacstat = 0;
			videop_print("N");
*/
			if(videoPCtrl.curframe)
				XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)videoPCtrl.curframe);
			if(videoPCtrl.nexframe)
				XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)videoPCtrl.nexframe);

			memset((void*)(videoPCtrl.curframe+8),0,videoPCtrl.audiolen);
			ax32xx_sysDcacheFlush((u32)videoPCtrl.curframe+8,videoPCtrl.audiolen);
			hal_dacSetBuffer((INT32U)videoPCtrl.curframe+8,videoPCtrl.audiolen);
			videop_print("N");
		}
	}
}

static int video_avi_read(void *buffer,INT32U offset,INT32U len)
{
	int cnt;

	cnt= lseek(videoPCtrl.arg.media.src.fd,offset,0);
	if(cnt<0)
	{
		videop_print("video_avi_read:lseek=%d\n",cnt);
		return-2;
	}
	cnt = read(videoPCtrl.arg.media.src.fd,buffer,len);
	if(cnt<0)
		return -1;
//	videop_print("\nread len = %x,offset = %x\n",len,cnt,offset);
	return cnt;
}
/*******************************************************************************
* Function Name  : videoPlaybackClear
* Description    : clear screen
* Input          : INT8U *ybuffer : y buffer
                      INT8U *uvbuffer: uv buffer
                      INT16U width   : width
                      INT16U height : height
                      INT8U y : y-value
                      INT8U uv : uv-value
* Output         : none
* Return         : int 
                      
*******************************************************************************/
void videoPlaybackClear(INT8U *ybuffer,INT8U *uvbuffer,INT16U width,INT16U height,INT8U y,INT8U uv)
{
#if 1	
	INT16U i,j;

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			ybuffer[j] = y;
			if(j<(width>>1))
				uvbuffer[j] = uv;
		}
		ybuffer+=width;
		uvbuffer+=width>>1;
	}
#else	
	dma_memset(ybuffer,y,width*height);
	dma_memset(uvbuffer,uv,width*height/2);
#endif	
}
/*******************************************************************************
* Function Name  : videoPlaybackInit
* Description    : initial video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackInit(void)
{   
    videoPCtrl.jpegcache = NULL;
	videoPCtrl.idx1cache = NULL;
	videoPCtrl.audiobuffer=NULL;
    videoPCtrl.idx1cache = (INT8U *)hal_sysMemMalloc(AVI_CFG_IDX1_BUFF_SIZE,64);
	if(videoPCtrl.idx1cache == NULL)
	{
		videop_print("video player : cache malloc fail\n");
		return STATUS_FAIL;
	}
	memset(videoPCtrl.idx1cache,0,AVI_CFG_IDX1_BUFF_SIZE);
	aviDecodeInit(videoPCtrl.idx1cache,AVI_CFG_IDX1_BUFF_SIZE,video_avi_read);
	
	videoPCtrl.stat = MEDIA_STAT_STOP;
	videoPCtrl.videoSync = NULL;
	videoPCtrl.volume = VOLUME_LEVEL*SysCtrl.volume_level;
	

	videoPCtrl.vidsIdleQ = XMsgQCreate(vidsIStack,VIDEO_CFG_BUFFER_NUM);
	videoPCtrl.vidsBusyQ = XMsgQCreate(vidsBStack,VIDEO_CFG_BUFFER_NUM);
	
    videoPCtrl.audsIdleQ = XMsgQCreate(audsIStack,AUDIO_CFG_BUFFER_NUM);
	videoPCtrl.audsBusyQ = XMsgQCreate(audsBStack,AUDIO_CFG_BUFFER_NUM);

	videoPCtrl.jpegIdleQ = XMsgQCreate(jpegIStack,JPEG_CFG_CACHE_NUM);
	videoPCtrl.jpegBusyQ = XMsgQCreate(jpegBStack,JPEG_CFG_CACHE_NUM);

	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : avideoPlaybackUninit
* Description    : uninitial video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackUninit(void)
{
	if(videoPCtrl.stat != MEDIA_STAT_STOP)
		videoPlaybackStop();	
#if MEDIA_CFG_SYNC_TIMER_EN == 0	
	XWorkDestory(videoPCtrl.videoSync);
#else
    hal_timerStop(MEDIA_CFG_TIMER);
#endif
    videoPCtrl.videoSync = NULL;
	
	XMsgQDestory(videoPCtrl.vidsIdleQ);
    XMsgQDestory(videoPCtrl.audsIdleQ);
	XMsgQDestory(videoPCtrl.audsBusyQ);
    XMsgQDestory(videoPCtrl.vidsBusyQ);
	XMsgQDestory(videoPCtrl.jpegIdleQ);
    XMsgQDestory(videoPCtrl.jpegBusyQ);

//	audac_play_callback_register(NULL);

	hal_sdUnlock();

    if(videoPCtrl.idx1cache)
		hal_sysMemFree(videoPCtrl.idx1cache);
    if(videoPCtrl.jpegcache)
		hal_sysMemFree(videoPCtrl.jpegcache);
	if(videoPCtrl.audiobuffer)
		hal_sysMemFree(videoPCtrl.audiobuffer);
	videoPCtrl.jpegcache = NULL;
	videoPCtrl.idx1cache = NULL;
	videoPCtrl.audiobuffer=NULL;

	return STATUS_OK;
}

int videoPlaybackDecodeWait(int timeout)
{
	int curtime;
	curtime = XOSTimeGet();
	while(hal_mjpegDecodeStatusCheck())
	{
		if((curtime+timeout)<XOSTimeGet())
		{
			//deg_Printf("video play : timeout\n");
			return -1;		
		}
	}
	return 0;
}
/*******************************************************************************
* Function Name  : videoPlaybackStart
* Description    : Start video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackStart(VIDEO_PARG_T *arg)
{
	int ret,i,jpgcachesize,jpgnum=0;
	
    if(arg == NULL)
		return STATUS_FAIL;

	if(arg->media.type!=MEDIA_SRC_FS)
	{
		return STATUS_FAIL;
	}

	videoPCtrl.arg.media.src.fd = arg->media.src.fd;
	
	aviDecodeInit(videoPCtrl.idx1cache,AVI_CFG_IDX1_BUFF_SIZE,video_avi_read); 

	ret = aviDecodeParse(videoPCtrl.arg.media.src.fd);  // parse avi header
	if(ret<0)
	{
		videop_print("video player : avi decode parse fail\n");
		return STATUS_FAIL;
	}
	videop_print("video player : avi type : %d\n",ret);
	if(videoPCtrl.audiobuffer == NULL) // malloc audio play buffer
		videoPCtrl.audiobuffer = (INT8U *)hal_sysMemMalloc(AUDIO_CFG_BUFFER_NUM*AUDIO_CFG_BUFFER_SIZE,64);//audioCache;

    jpgcachesize = hal_sysMemRemain()-8192-12*1024;
	if(jpgcachesize>JPEG_CFG_CACHE_SIZE)
	{
	#if SDRAM_SIZE == SDRAM_SIZE_8M
	    jpgnum = JPEG_CFG_CACHE_NUM;
	    jpgcachesize = (512<<10);
	    videoPCtrl.jpegcache = (INT8U *)hal_sysMemMalloc(jpgcachesize*jpgnum,64);//jpegCache;
	 #else   
		if(jpgcachesize>1024*1024)
			jpgcachesize = 1024*1024;		
		jpgcachesize = (jpgcachesize+0x1f)&(~0x1f);
		videoPCtrl.jpegcache = (INT8U *)hal_sysMemMalloc(jpgcachesize,64);//jpegCache;
		if((jpgcachesize>>1)>=JPEG_CFG_CACHE_SIZE)
		{
			jpgcachesize = jpgcachesize>>1;
			jpgnum = 2;
		}
		else
			jpgnum = 1;
	#endif
	}
	else
	{
		videoPCtrl.jpegcache = NULL;
	}
	
	if(videoPCtrl.jpegcache == NULL || videoPCtrl.audiobuffer == NULL)
	{
		videop_print("video player : malloc memory fail.%x,%x\n",(INT32U)videoPCtrl.jpegcache,(INT32U)videoPCtrl.audiobuffer);
		hal_sysMemFree(videoPCtrl.jpegcache);
		hal_sysMemFree(videoPCtrl.audiobuffer);
		return STATUS_FAIL;
	}
	videop_print("video player : mem.vids:num=%d,size=0x%x\n",jpgnum,jpgcachesize);
    hal_sdLock();
    videoPCtrl.samplerate = 16000;//aviDecodeGetAudioSampleRate();
	aviDecodeGetResolution(&videoPCtrl.src.width,&videoPCtrl.src.height);
	
    videoPCtrl.arg.tar.width = arg->tar.width;
	videoPCtrl.arg.tar.height= arg->tar.height;
	videoPCtrl.arg.pos.x = arg->pos.x;
	videoPCtrl.arg.pos.y = arg->pos.y;
	videoPCtrl.arg.rotate = arg->rotate;
	videoPCtrl.arg.firstframe = arg->firstframe;
 
//-------------initial audio play queue
    XMsgQFlush(videoPCtrl.audsIdleQ);
	XMsgQFlush(videoPCtrl.audsBusyQ);
	for(i=0;i<AUDIO_CFG_BUFFER_NUM;i++)
	{
		ret = (int)(videoPCtrl.audiobuffer+i*AUDIO_CFG_BUFFER_SIZE);
		XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)ret);		
	}
//-----------initial jpeg cache queue
	XMsgQFlush(videoPCtrl.jpegIdleQ);
	XMsgQFlush(videoPCtrl.jpegBusyQ);

	for(i=0;i<jpgnum;i++)
	{
		ret = (int)(videoPCtrl.jpegcache+i*jpgcachesize);
		videoJpegCache[i].mem = ret;	
		videop_print("video player : jpeg cahce[%d] = %x\n",i,ret);
		XMsgQPost(videoPCtrl.jpegIdleQ,(MSG_T *)&videoJpegCache[i]);
	}
	
//------------intitial video play queue	
	XMsgQFlush(videoPCtrl.vidsIdleQ);
	XMsgQFlush(videoPCtrl.vidsBusyQ);
	for(i=0;i<VIDEO_CFG_BUFFER_NUM;i++)
	{
		videoShowCache[i].mem = 0;
		XMsgQPost(videoPCtrl.vidsIdleQ,(MSG_T *)&videoShowCache[i]);
	}
    
	//------------initial video frame sync
	ret = aviDecodeGetVideoFrameTime();
#if MEDIA_CFG_SYNC_TIMER_EN == 0
	if(videoPCtrl.videoSync==NULL)
		videoPCtrl.videoSync = XWorkCreate(ret,videoPlaybackSync); 
#else
    ret = 1000/ret;
    if(TIMER_FRQ_MIN>ret)
    {
		i = TIMER_FRQ_MIN/ret;
		if((ret*ret)<TIMER_FRQ_MIN)
			i++;
		videoPCtrl.syncValue = i;
		ret = i*ret;
    }
	else
	{
		videoPCtrl.syncValue = 1;
	}
    if(hal_timerStart(MEDIA_CFG_TIMER,ret,videoPlaybackSync)<0)
    {
		videop_print("video player : timer start fail.%d\n",ret);
		hal_sysMemFree(videoPCtrl.jpegcache);
		hal_sysMemFree(videoPCtrl.audiobuffer);
		return STATUS_FAIL;
    }
#endif
	//deg_Printf("video play : 0x%x,0x%x,0x%x\n",(INT32U)p_lcd_buffer,(INT32U)p_lcd_buffer->next,(INT32U)p_lcd_buffer->next->next);
	videoPCtrl.syncCnt = 0;	
	videoPCtrl.vflag    = 1;
	videoPCtrl.dacstat = 0x10;  // video first frame flag
	videoPCtrl.curframe= 0;
	videoPCtrl.nexframe = 0;
	videoPCtrl.oldframe= 0;
	videoPCtrl.audiolen= 0;
	videoPCtrl.videoframe=NULL;
	videoPCtrl.jpegframe = NULL;
	videoPCtrl.lcdframe = NULL;
	videoPCtrl.playSpeed = 2;
	videoPCtrl.newjpeg = 0;
	videoPCtrl.audsoffset = 0;
	videoPCtrl.prepause = 0;
	videoPCtrl.PlayFrames=0;
	videoPCtrl.syncFrames=0;	//--new
	videoPCtrl.stat      = MEDIA_STAT_PLAY;	
#if VIDEO_DBG_PLY_EN
    videoPCtrl.dbg_frame_cnt = 0;
    videoPCtrl.dbg_time_cnt = 0;
	videoPCtrl.dbg_time = 0;
	videoPCtrl.dbg_play_time = XOSTimeGet();
	videoPCtrl.dbg_ptime = 0;
#endif
    
//-------------PLAY FIRST FRAME-------------------------	
	if(arg->firstframe)
	{
		videoPlaybackService();
	}
	
    videop_print("video player : start.frame time = %dms\n",ret); 
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : videoPlaybackStop
* Description    : Stop video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackStop(void)
{
	INT8U err = X_ERR_NONE;
    
	if(videoPCtrl.stat == MEDIA_STAT_STOP)
		return STATUS_OK;
	videoPCtrl.PlayFrames=0;
    hal_dacPlayStop();
	hal_mjpegDecodeStop();
    close(videoPCtrl.arg.media.src.fd);
	videoPCtrl.stat = MEDIA_STAT_STOP;
    hal_sdUnlock();
	hal_sysMemFree(videoPCtrl.jpegcache);
	hal_sysMemFree(videoPCtrl.audiobuffer);
	videoPCtrl.jpegcache = NULL;
	videoPCtrl.audiobuffer = NULL;
    
    vidsFree((JPEG_CACHE_T *)videoPCtrl.videoframe);
    
    while(err == X_ERR_NONE)
    {
        JPEG_CACHE_T * video = (JPEG_CACHE_T *)XMsgQPend(videoPCtrl.vidsBusyQ,&err);
        if(err == X_ERR_NONE)
            vidsFree(video);
    }
    
#if MEDIA_CFG_SYNC_TIMER_EN == 0	
	XWorkDestory(videoPCtrl.videoSync);
	videoPCtrl.videoSync = NULL;
#else
    hal_timerStop(MEDIA_CFG_TIMER);
#endif
#if VIDEO_DBG_PLY_EN	
	videoPCtrl.dbg_play_time = XOSTimeGet()-videoPCtrl.dbg_play_time;
	videop_print("video player : play time = %dM %dS %dMS\n",(videoPCtrl.dbg_play_time/1000)/60,(videoPCtrl.dbg_play_time/1000)%60,videoPCtrl.dbg_play_time%1000);
#endif
	videop_print("video player : stop\n"); 
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : videoPlaybackPuase
* Description    : Puase video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackPuase(void)
{
//	if(videoPCtrl.dacstat==1)
	if((videoPCtrl.dacstat&0x0f)==0)		// first frame , will pause right now
	{
		hal_dacPlayStop();
		videoPCtrl.dacstat = 0;
		videoPCtrl.stat = MEDIA_STAT_PAUSE;
		hal_sdUnlock();
	}
	else
	{
		
    	videoPCtrl.prepause = 1;
	}
    
/*	hal_dacPlayStop();
	videoPCtrl.dacstat = 0;
	videoPCtrl.stat = MEDIA_STAT_PAUSE;
	hal_sdUnlock();*/
	videop_print("video player : pause \n"); 
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : videoPlaybackResume
* Description    : Resume video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackResume(void)
{
	videoPCtrl.prepause = 0;
	hal_sdLock();
	videoPCtrl.stat = MEDIA_STAT_PLAY;
	videop_print("video player : resume\n"); 
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : videoPlaybackGetStatus
* Description    : get video Playback 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackGetStatus(void)
{
	return videoPCtrl.stat;
}
/*******************************************************************************
* Function Name  : audioPlaybackSetVolume
* Description    :set audio Playback volume
* Input          : INT8U volume : 0-100
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackSetVolume(INT8U volume)
{
	if(volume>100)
		volume = 100;
	videoPCtrl.volume = volume;
	if(videoPCtrl.stat == MEDIA_STAT_PLAY)
        hal_dacSetVolume(volume);
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackGetVolume
* Description    : get audio Playback volume
* Input          : 
* Return         : INT8U volume : 0-100 
                      
*******************************************************************************/
int videoPlaybackGetVolume(void)
{
	return videoPCtrl.volume;
}

/*******************************************************************************
* Function Name  : videoPlaybackGetTime
* Description    : get video Playback time ms
* Input          : INT32U *total : total time
                     INT32U *curr : current time
* Output         : INT32U *total : total time
                     INT32U *curr : current time
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackGetTime(INT32U *total,INT32U *curr)
{
	return aviDecodeGetTime(total,curr);
}
/*******************************************************************************
* Function Name  : videoPlaybackGetResolution
* Description    : get video Playback resolution
* Input          : none
* Output         : none
* Return         : Media_Res_T *                       
*******************************************************************************/
Media_Res_T *videoPlaybackGetResolution(void)
{
	return &videoPCtrl.src;
}
/*******************************************************************************
* Function Name  : videoPlaybackGetStatus
* Description    : get video Playback 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/

int videoPlaybackService(void)
{
	INT8U err,stat;
	INT32U offset,length,addr;
	int ret,res;
	disp_frame_t * p_lcd_buffer;
	JPEG_CACHE_T *jpeg,*video;
	
    if(videoPCtrl.stat!=MEDIA_STAT_PLAY)
		return videoPCtrl.stat;


SERVICE_GET_INDEX:
    stat = 0;
    
    if(videoPCtrl.dacstat != 3) // decode avi
    {   
		 ret = aviDecodeService();
		 if(ret)
		 	stat |= 4;
//-------cache jpeg data
        jpeg = (JPEG_CACHE_T *)XMsgQPend(videoPCtrl.jpegIdleQ,&err);
        if(err == X_ERR_NONE)
        {
        	videoPCtrl.PlayFrames++;
			ret = aviDecodeOneFrame(&offset,&length,AVI_FRAME_DC_VIDEO);  // get video frame
			//--new
			if(videoPCtrl.PlayFrames>videoPCtrl.syncFrames+2)	//  play too fast handle
			{
				XOSTimeDly(30);
			}
					
		  //  if((ret<0)||(videoPCtrl.vflag>6))// fail & based on auds time
			 if((ret<0))// fail & based on auds time
			{
				videop_print("ret=%d\n",ret); 
				if(ret<0)
				    stat |= 1;  //maybe video frame end
				else
				{
					if(videoPCtrl.vflag>0)
					{
						videoPCtrl.vflag-=1;
					}
				}
	            XMsgQPost(videoPCtrl.jpegIdleQ,(MSG_T *)jpeg);
				goto SERVICE_VIDEO_DECODE;	
			}
			//--new
			if((videoPCtrl.oldframe == offset)|| (0 == length)||(8==length)||(512-8==length)) // same frame 
			{	
				XMsgQPost(videoPCtrl.jpegIdleQ,(MSG_T *)jpeg);
				if(videoPCtrl.dacstat == 0x10)
				{
					goto SERVICE_GET_INDEX;
				}
			#if VIDEO_CFG_JUMP_SAMEFRAME
			    if((0 == length))
					offset = videoPCtrl.oldframe;
			    jpeg = (JPEG_CACHE_T *)videoPCtrl.jpegframe;
			    if((jpeg) && (jpeg->offset == offset))
			    {
					jpeg->count++;
			    }
				else
				{
					res = XMsgQCheck(videoPCtrl.jpegBusyQ);
					while(res)
					{
						jpeg = (JPEG_CACHE_T *)XMsgQPend(videoPCtrl.jpegBusyQ,&err);
						if((jpeg) && (jpeg->offset == offset))
							jpeg->count++;
						 res--;
					}
				}
			#endif
			    
				goto SERVICE_VIDEO_DECODE;	
			}
			else
		    {
				res = lseek(videoPCtrl.arg.media.src.fd,offset,0); // seek to the movi data addr
				if(res<0)
				{
					goto VIDEO_PLAYBACK_ERROR;
				}
				res = read(videoPCtrl.arg.media.src.fd,(void *)jpeg->mem,length+8); // read data
				if(res<0)
				{
					goto VIDEO_PLAYBACK_ERROR;
				}	
			    jpeg->offset = offset;
			    jpeg->count  = 0;
				XMsgQPost(videoPCtrl.jpegBusyQ,(MSG_T *)jpeg);
				videoPCtrl.oldframe = offset;
				videoPCtrl.newjpeg = (INT32U)jpeg;
			}
        }
		else
		{
			//videop_print("<%d,%d>",XMsgQCheck(videoPCtrl.jpegBusyQ),XMsgQCheck(videoPCtrl.jpegIdleQ));
		}

//mjpeg decode
SERVICE_VIDEO_DECODE:
	    if(hal_mjpegDecodeStatusCheck()) // decode busy
			goto SERVICE_AUDIO_DECODE;
		ret = hal_mjpegDecodeErrorCheck();
		if(ret!=0) // decode fail.
		{
			videop_print("decode err : %d,offset = 0x%x,%x\n",ret,(INT32U)videoPCtrl.oldframe,(INT32U)videoPCtrl.jpegframe);
            vidsFree((JPEG_CACHE_T *)videoPCtrl.videoframe);
			videoPCtrl.PlayFrames++;
			videoPCtrl.videoframe = NULL;
		}
		else if(videoPCtrl.videoframe!=NULL)// decode successful.put in showing queue
		{
			XMsgQPost(videoPCtrl.vidsBusyQ,(MSG_T *)videoPCtrl.videoframe);
			videoPCtrl.videoframe = NULL;
			
		#if VIDEO_DBG_PLY_EN		    
            videoPCtrl.dbg_time_cnt += XOSTimeGet()-videoPCtrl.dbg_time;
		    videoPCtrl.dbg_frame_cnt++;
		#endif
		}        
	//-----release	
		offset = 0;
		if(videoPCtrl.jpegframe)// && ((((INT32U)videoPCtrl.jpegframe)&0x0f)==0))// last jpeg cache using end,back to idle queue
		{
		#if VIDEO_CFG_JUMP_SAMEFRAME
		    jpeg = (JPEG_CACHE_T *)videoPCtrl.jpegframe;
		    if(jpeg->count)
		    {
				jpeg->count--;
		    }
			else
			{
		        offset = (INT32U)videoPCtrl.jpegframe;	
				videoPCtrl.jpegframe = NULL;
			}
		#else
		    XMsgQPost(videoPCtrl.jpegIdleQ,(MSG_T *)videoPCtrl.jpegframe);	
		    videoPCtrl.jpegframe = NULL;	
		#endif										
		}
	//-------register	
#if VIDEO_CFG_JUMP_SAMEFRAME		
		if(videoPCtrl.jpegframe)
		{
            jpeg = (JPEG_CACHE_T *)videoPCtrl.jpegframe;    
			video = (JPEG_CACHE_T *)videoPCtrl.lcdframe;
			if(video && (video->offset == jpeg->offset))
			{
				video->count++;
				goto SERVICE_AUDIO_DECODE;
			}
			res = XMsgQCheck(videoPCtrl.vidsBusyQ);
			ret = 0;
			while(res)
			{
				video = (JPEG_CACHE_T *)XMsgQPend(videoPCtrl.vidsBusyQ,&err);
				if(video && (video->offset == jpeg->offset))
				{
					video->count++;
					ret = 1;
				}
				XMsgQPost(videoPCtrl.vidsBusyQ,(MSG_T *)video);
				res--;
			}
			if(ret)
				goto SERVICE_AUDIO_DECODE;
		}
		else
#endif			
		{			
			jpeg = (JPEG_CACHE_T *)XMsgQPend(videoPCtrl.jpegBusyQ,&err); //get jpeg cache buffer
			if(err!=X_ERR_NONE)// no jpeg cached
			{	
			#if VIDEO_CFG_JUMP_SAMEFRAME		
				if(videoPCtrl.jpegframe)
				{
					jpeg->count++;
				}
				else
				{
					videoPCtrl.jpegframe = (MSG_T *)offset;		
				}
			#endif	
			    
				goto SERVICE_AUDIO_DECODE;
			}
		}
        video = vidsMalloc();
		if(!video)// no idle showing buffer
		{
		#if VIDEO_CFG_JUMP_SAMEFRAME		
			if(videoPCtrl.jpegframe)
			{	
				jpeg->count++;	
			}
			else
			{
				XMsgQPostFront(videoPCtrl.jpegBusyQ,(MSG_T *)jpeg);	 
				videoPCtrl.jpegframe = (MSG_T *)offset;
			}
		#else
		    XMsgQPostFront(videoPCtrl.jpegBusyQ,(MSG_T *)jpeg);	 
       	#endif
		    
			goto SERVICE_AUDIO_DECODE;
		}
    #if VIDEO_CFG_JUMP_SAMEFRAME			
		if(offset)
		{
			XMsgQPost(videoPCtrl.jpegIdleQ,(MSG_T *)offset);
		}
	#endif
	
	#if VIDEO_DBG_PLY_EN	
		videoPCtrl.dbg_time = XOSTimeGet();
	#endif

        if(videoPCtrl.dacstat == 0x10) 		
        {
            hal_mjpegDecodeReset();
        	ret = hal_mjpegDecodeParse((INT8U *)(jpeg->mem + 8),videoPCtrl.arg.tar.width,videoPCtrl.arg.tar.height);
			if(ret<0)
			{
				videop_print("hal_mjpegDecodeParse err,ret=%d,%x\n",ret,*(INT32U *)jpeg->mem);
				videoPlaybackStop();
			    return MEDIA_STAT_STOP;
			}
			//videop_print("video player : parse first frame of avi ok\n");
        }
        p_lcd_buffer = (disp_frame_t *)video->mem;
		if(p_lcd_buffer)
			hal_dispframeVideoCfg(p_lcd_buffer,videoPCtrl.arg.pos.x,videoPCtrl.arg.pos.y,videoPCtrl.arg.tar.width,videoPCtrl.arg.tar.height);
	#if HAL_CFG_MJPEG_QULITY_AUTO > 0	
		hal_mjpegDecodeOneFrame_Ext((INT8U *)(jpeg->mem + 8),p_lcd_buffer->y_addr,p_lcd_buffer->uv_addr);
	#else
		hal_mjpegDecodeOneFrame((INT8U *)(jpeg->mem + 8),p_lcd_buffer->y_addr,p_lcd_buffer->uv_addr);
	#endif
		video->count = 1;
		video->offset = jpeg->offset;
		if(videoPCtrl.dacstat == 0x10) 
		{
			while(hal_mjpegDecodeStatusCheck());
			
			if(hal_mjpegDecodeErrorCheck()!=0) // decode error,fail
			{
                vidsFree(video);
				videoPlaybackStop();
				return MEDIA_STAT_STOP;
			}
			videop_print("video player : play first frame\n");
			//XMsgQPost(videoPCtrl.vidsBusyQ,(MSG_T *)p_lcd_buffer);
			//XMsgQPost(videoPCtrl.jpegIdleQ,(MSG_T *)(addr&(~0x0f)));
			videoPCtrl.lcdframe = (MSG_T *)video;
			videoPCtrl.jpegframe = (MSG_T *)jpeg;
		#if VIDEO_DBG_PLY_EN
            videoPCtrl.dbg_time_cnt += XOSTimeGet()-videoPCtrl.dbg_time;
		    videoPCtrl.dbg_frame_cnt++;
	    #endif
		    if(videoPCtrl.arg.firstframe)
			{
				videoPlaybackPuase();
			}
		    videoPCtrl.dacstat = 0;// dac initial stat
		    videoPCtrl.vflag = 1;
		    goto SERVICE_SHOW;
		}
		else
		{
			videoPCtrl.jpegframe = (MSG_T *)jpeg;
			videoPCtrl.videoframe = (MSG_T *)video;//p_lcd_buffer;
		}	
//-----audio decode---		
SERVICE_AUDIO_DECODE:	
		addr = (INT32U)XMsgQPend(videoPCtrl.audsIdleQ,&err);//get idle queue
		if(err != X_ERR_NONE)
		    goto SERVICE_SHOW;
		ret = aviDecodeOneFrame(&offset,&length,AVI_FRAME_WD_AUDIO);  // get audio frame
		if(ret<0) // fail
		{
			stat |= 2; // maybe audio frame end
			XMsgQPost(videoPCtrl.audsIdleQ,(MSG_T *)addr);
			videoPlaybackSetVolume(0);
			goto SERVICE_SHOW;
		}
		if((videoPCtrl.audiolen!=0) && (videoPCtrl.audiolen!=length))
		{
			videop_print("video player : audio %x,%x\n",length,videoPCtrl.audiolen);
			length = videoPCtrl.audiolen;
		}
		res = lseek(videoPCtrl.arg.media.src.fd,offset,0); // seek to the movi data addr
		if(res<0)
			goto VIDEO_PLAYBACK_ERROR;
       
//		res = read(videoPCtrl.arg.media.src.fd,(void *)addr,8); // header
		res = read(videoPCtrl.arg.media.src.fd,(void *)addr,length+8);   // make it sector align
		if(res<0)
			goto VIDEO_PLAYBACK_ERROR;
		
		//XMsgQPost(videoPCtrl.audsBusyQ,(MSG_T *)addr);
		if((videoPCtrl.dacstat != 1) && (videoPCtrl.dacstat != 0x10))
		{	
			videoPCtrl.audiolen = length;
			//if(XMsgQCheck(videoPCtrl.audsBusyQ)>=2)
			{
				hal_dacSetVolume(SysCtrl.volume_level*10);
				//addr = (INT32U)XMsgQPend(videoPCtrl.audsBusyQ,&err);
				hal_dacCallBackRegister(videoPlaybackDacISR);  // audio player may register callback function when play key sound
				hal_dacPlayStart(videoPCtrl.samplerate,(INT32U)addr+8,videoPCtrl.audiolen);//0x3fff);//
				videoPCtrl.dacstat = 1;
				videoPCtrl.curframe = addr;
			}
		}
		else
		{
			XMsgQPost(videoPCtrl.audsBusyQ,(MSG_T *)addr);
		}	
		
    }
SERVICE_SHOW:
	if(stat==7)// audio and video are to the end
	{
		videop_print("video player : file end.%d\n",aviDecodeGetFrameIndex());
		if(videoPCtrl.dacstat==0) //auds is end
		{
		    videoPlaybackStop();
			return videoPCtrl.stat;
		}
		else	
		    videoPCtrl.dacstat = 3; // ready to stop.only wait the last decoded frame play end
	}
#if ((VIDS_RELEASE_TYPE==0) || (VIDS_RELEASE_TYPE == 2))
    if(videoPCtrl.vflag)
    {
		offset = 0;
		video = 	(JPEG_CACHE_T *)videoPCtrl.lcdframe;
		if(video && (video->count))
		{
			err = X_ERR_NONE;
		}
		else
		{
			offset = (INT32U)videoPCtrl.lcdframe;
		    video = 	(JPEG_CACHE_T *)XMsgQPend(videoPCtrl.vidsBusyQ,&err);
		}
		if(err == X_ERR_NONE)
		{
			videoPCtrl.vflag--;
		#if VIDS_RELEASE_TYPE == 2
			if(offset)
            {
                // put last frame in queue
                XMsgQPost(videoPCtrl.vidsIdleQ,(MSG_T *)videoPCtrl.lcdframe);
            }
        #endif
		

			hal_lcdSetFrame(videoPCtrl.arg.pos.x,videoPCtrl.arg.pos.y,
							videoPCtrl.arg.tar.width,videoPCtrl.arg.tar.height,
							(void*)video->mem,videoPCtrl.arg.rotate);
			ax32xx_deUpdate();
				XOSTimeDly(10);

/*
			#if (1 == LCD_PLAY_SCALE)
			if(320 == videoPCtrl.arg.tar.width)
			{
				hal_dispframeVideoCfg(video->mem,videoPCtrl.arg.pos.x,videoPCtrl.arg.pos.y,videoPCtrl.arg.tar.width,videoPCtrl.arg.tar.height);
			}
			#endif
*/
            if(video->count)
				video->count--;
			videoPCtrl.lcdframe = (MSG_T *)video;
		}
		else
		{
			if(videoPCtrl.dacstat == 3)
				videoPlaybackStop();
		}
    }
#endif
	
#if VIDEO_DBG_PLY_EN

    if((videoPCtrl.dbg_frame_cnt>=300)/* || (videoPCtrl.dbg_ptime>300)*/)
    {
		videop_print("video player : [%d]decode -> %d ms/frame\n",videoPCtrl.vflag,videoPCtrl.dbg_time_cnt/videoPCtrl.dbg_frame_cnt);
		videoPCtrl.dbg_frame_cnt = 0;
		videoPCtrl.dbg_time_cnt = 0;
		videoPCtrl.dbg_ptime = 0;
    }
#endif
    return STATUS_OK;
VIDEO_PLAYBACK_ERROR:  // video player can not handler this error.only stop playback
	videoPlaybackStop();

	return STATUS_FAIL;
}
/*******************************************************************************
* Function Name  : videoPlaybackFastForward
* Description    : ast forward  
* Input          : none
* Output         : none
* Return         : int  : current play speed setp
                            : backward : 0,1,2,3, -> -16,-8,-4,-1
                              normal     : 4 -0
                              forward   : 5,6,7,8, ->1,4,8,16
*******************************************************************************/
int videoPlaybackFastForward(void)
{
/*
	int fast;
	if(videoPCtrl.playSpeed<4)
	{
		videoPCtrl.playSpeed++;
	}

	fast = videoSpeedTable[videoPCtrl.playSpeed];
	aviDecodeFast(fast);
    videop_print("video player : fast speed %d\n",fast);
	return videoPCtrl.playSpeed;
*/
	return 0;
}
/*******************************************************************************
* Function Name  : videoPlaybackFastBackward
* Description    : fast backward
* Input          : none
* Output         : none
* Return         : int  : current play speed setp
                            : backward : 0,1,2,3, -> -16,-8,-4,-1
                              normal     : 4 -0
                              forward   : 5,6,7,8, ->1,4,8,16                      
*******************************************************************************/
int videoPlaybackFastBackward(void)
{
/*
	int fast;
	if(videoPCtrl.playSpeed>0)
	{
		videoPCtrl.playSpeed--;
	}
	fast = videoSpeedTable[videoPCtrl.playSpeed];
	aviDecodeFast(fast);
    videop_print("video player : fast speed %d\n",fast);
	return videoPCtrl.playSpeed;	
*/
	return 0;
}

s8 videoPlaybackGetplayspeed()
{
	return videoPCtrl.playSpeed;
}

#if MEDIA_CFG_THUMBNAIL_EN > 0
/*******************************************************************************
* Function Name  : videoPlaybackThumbnailInit
* Description    : thumbnail initial.clear screen
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailInit(INT8U cnt)
{
	INT16U width,height;
	INT16S x,y,j;
	
    //waring:´æÔÚ·çÏÕ
	disp_frame_t * p_lcd_buffer = hal_lcdGetShowingBuffer();

	hal_lcdGetBufferResolution(&width,&height);

    videoPlaybackClear(p_lcd_buffer->y_addr,p_lcd_buffer->uv_addr,width,height,0x80,0x80);

    videoPlaybackClear(thumbnailYBuffer,thumbnailUVBuffer,THUMBNAIL_CFG_WIDTH,THUMBNAIL_CFG_HEIGHT,0x00,0x80);

    width = (width-THUMBNAIL_CFG_WIDTH*3)>>2;
	height = (height-THUMBNAIL_CFG_HEIGHT*3)>>2;

	for(j=0;j<cnt;j++)
	{
		x = width+(THUMBNAIL_CFG_WIDTH+width)*(j%3);
		y = height+(THUMBNAIL_CFG_HEIGHT+height)*(j/3); 

		videoPlaybackThumbnailShow(x,y,THUMBNAIL_CFG_WIDTH,THUMBNAIL_CFG_HEIGHT,(INT32U)thumbnailYBuffer,(INT32U)thumbnailUVBuffer);
	}
   if(videoPCtrl.jpegcache == NULL)
		videoPCtrl.jpegcache = (INT8U *)hal_sysMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE,64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;

	return 0;
}
/*******************************************************************************
* Function Name  : videoPlaybackThumbnailUninit
* Description    : thumbnail uninitial.
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailUninit(void)
{
	if(videoPCtrl.jpegcache)
		hal_sysMemFree(videoPCtrl.jpegcache);

	return 0;
}
/*******************************************************************************
* Function Name  : videoPlaybackThumbnailProcess
* Description    : thumbnail process to addr.only support avi,jpg
* Input          : FHANDLE fd : file handle
                     INT8U ftype : file type
                     INT16U width: thumbnail width
                     INT16U height:thumbnail height
                     INT32U y_addr: yuv : y addr
                     INT32U uv_addr:yuv : uv addr
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailProcess(FHANDLE fd,INT8U ftype,INT16U width,INT16U height,INT32U y_addr,INT32U uv_addr)
{
	int ret;
	INT32U offset,length;

	if(hal_mjpegDecodeStatusCheck())
	{
		videop_print("Thumbnail : busy\n");
		goto THUMBNAIL_PROCESS_END;
	}
	if(ftype == FILE_TYPE_AVI)
	{
		videoPCtrl.arg.media.src.fd = fd;
		aviDecodeInit(videoPCtrl.idx1cache,AVI_CFG_IDX1_BUFF_SIZE,video_avi_read); 

		ret = aviDecodeParse(videoPCtrl.arg.media.src.fd);  // parse avi header
		if(ret<0)
		{
			videop_print("Thumbnail : avi parse fail.\n");
			goto THUMBNAIL_PROCESS_END;
		}    
		videop_print("video player : avi type : %d\n",ret);
		
		ret = aviDecodeOneFrame(&offset,&length,AVI_FRAME_DC_VIDEO); 
        if(ret<0)
        {
			videop_print("Thumbnail : get video frame fail.\n");
			goto THUMBNAIL_PROCESS_END;
        }
		lseek(videoPCtrl.arg.media.src.fd,offset,0); // seek to the movi data addr
		ret = read(videoPCtrl.arg.media.src.fd,(void *)videoPCtrl.jpegcache,length);
	#if VIDEO_DBG_PLY_EN
         videoPCtrl.dbg_time = XOSTimeGet();
	#endif
		//if(jpeg_decode_video_prepare(videoPCtrl.jpegcache + 8,width,height)!= true)
		if(hal_mjpegDecodeParse(videoPCtrl.jpegcache + 8,width,height)<0)
		{
			videop_print("Thumbnail : avi decode fail 0\n");
			goto THUMBNAIL_PROCESS_END;
		}

		//jpeg_decode_video_kick(videoPCtrl.jpegcache + 8,(INT8U *)y_addr,(INT8U *)uv_addr);
         hal_mjpegDecodeOneFrame(videoPCtrl.jpegcache + 8,(INT8U *)y_addr,(INT8U *)uv_addr);
		//while(jpeg_decode_check_busy());
		if(videoPlaybackDecodeWait(5000)<0)
		{
			videop_print("Thumbnail : timeout\n");
			goto THUMBNAIL_PROCESS_END;
		}
		else if(hal_mjpegDecodeErrorCheck()!=0) // decode error,fail
		{
			videop_print("Thumbnail : avi decode fail 1\n");
			goto THUMBNAIL_PROCESS_END;
		}
	#if VIDEO_DBG_PLY_EN
        videop_print("Thumbnail : time = %d ms\n",XOSTimeGet()-videoPCtrl.dbg_time);
	#endif	
		return STATUS_OK;
	}
	else if(ftype == FILE_TYPE_JPG)
	{
		length = fs_size(fd);
		ret = read(fd,(void *)videoPCtrl.jpegcache,length);
		if(ret<0)
		{
			videop_print("Thumbnail : jpg read fail.\n");
			goto THUMBNAIL_PROCESS_END;
		}
	#if VIDEO_DBG_PLY_EN
        videoPCtrl.dbg_time = XOSTimeGet();
	#endif
		//if(jpeg_decode_picture(videoPCtrl.jpegcache,(INT8U *)y_addr,(INT8U *)uv_addr,width,height) != true)
		if(hal_mjpegDecodePicture(videoPCtrl.jpegcache,(INT8U *)y_addr,(INT8U *)uv_addr,width,height)<0)
		{
			videop_print("Thumbnail : jpeg decode fail.\n");
			return STATUS_FAIL;
		}
//		timer_ticktimer_start();
	    while(hal_mjpegDecodeStatusCheck());
//	    ticktimer_stop();
	#if VIDEO_DBG_PLY_EN
        videop_print("Thumbnail : time = %d ms\n",XOSTimeGet()-videoPCtrl.dbg_time);
	#endif
		return STATUS_OK;
	}
THUMBNAIL_PROCESS_END:
    videoPlaybackClear((INT8U *)y_addr,(INT8U *)uv_addr,width,height,0x00,0x80);     
	videop_print("Thumbnail : file type no support.\n");
	return STATUS_FAIL;
		
}
/*******************************************************************************
* Function Name  : videoPlaybackThumbnailShow
* Description    : thumbnail show one file
* Input          : INT16S x     : x position
                     INT16S y     : y position
                     INT16U width: thumbnail width
                     INT16U height:thumbnail height
                     INT32U y_addr: yuv : y addr
                     INT32U uv_addr:yuv : uv addr                     
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailShow(INT16S x,INT16S y,INT16U width,INT16U height,INT32U y_addr,INT32U uv_addr)
{
	INT16U w,h,i,j;
	INT8U *ysrc,*uvsrc,*ytar,*uvtar;

	disp_frame_t *p_lcd_buffer = hal_lcdGetShowingBuffer();
	
	hal_lcdGetBufferResolution(&w,&h);
    ysrc = (INT8U *)y_addr;
	uvsrc= (INT8U *)uv_addr;
	ytar  = p_lcd_buffer->y_addr+x+y*w;
	uvtar = p_lcd_buffer->uv_addr+x+((y*w)>>1);//((x+y*w)>>1);
	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			ytar[i] = *ysrc++;
		}
		ytar+=w;
		if(j<(height>>1))
		{
			 for(i=0;i<width;i++)
			{
				uvtar[i] = *uvsrc++;
			}
			 uvtar+=w;
		}
	}

	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : videoPlaybackThumbnailOne
* Description    : thumbnail show one file to screen
* Input          : FHANDLE fd : file handle
                     INT8U ftype : file type
                     INT16S x     : x position
                     INT16S y     : y position
                     INT16U width: thumbnail width
                     INT16U height:thumbnail height
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailOne(FHANDLE fd,INT8U ftype,INT16S x,INT16S y,INT16U width,INT16U height)
{
    int ret;

    ret = videoPlaybackThumbnailProcess(fd,ftype,width,height,(INT32U)thumbnailYBuffer,(INT32U)thumbnailUVBuffer);
	
	videoPlaybackThumbnailShow(x,y,width,height,(INT32U)thumbnailYBuffer,(INT32U)thumbnailUVBuffer);
	
//    deg_Printf("Thumbnail : show pos(%d,%d),res(%d,%d).\n",x,y,width,height);
	
	return ret;
}
#endif // #if MEDIA_CFG_THUMBNAIL_EN >0

#endif


