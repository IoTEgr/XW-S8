/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA audio playback
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : audio_playback.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is audio playback file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"

#if  MEDIA_CFG_AUDIO_PLAYBK_EN > 0


#if AUDIO_DBG_PLY_EN
    //#define  audiop_print    deg_Printf
	#define  audiop_print(...)
#else
    #define  audiop_print(...)
#endif

#define AUDIO_BUFF_NUM           AUDIO_CFG_BUFFER_NUM
#define AUDIO_TEMP_SIZE         AUDIO_CFG_BUFFER_SIZE
#if (AUDIO_TEMP_SIZE > 0xffff)
    #define AUDIO_BUFF_SIZE     (8192)
#else
    #define AUDIO_BUFF_SIZE     (AUDIO_TEMP_SIZE&0xfff0)
#endif
#define AUDIO_RING_SIZE        AVI_CFG_IDX1_BUFF_SIZE
#define DAC_VOLUME_MAKE(x)          ((x*0x7fff)/100)


typedef struct Audio_Ctrl_S
{
	INT8U  stat;		
	INT8U  curIdx;
	INT8U  dacstat;   // 0 :stop,1:start,2:dac auto stop,3:decode auto stop
	INT8U  volume;
    INT8U  prepause;
	INT8U  nextIdx;
	INT16U datasize;
	
	INT32U samplerate;
	INT32U framesize;

	XMsg *idleQ;
	XMsg *busyQ;

    Audio_ARG_T arg;

	INT8U *ringbuffer;
	INT8U *audiobuffer;
//	INT8U *buffer[AUDIO_BUFF_NUM];
}Audio_Ctrl_T;

static Audio_Ctrl_T audioCtrl;




void audioPlaybackISR(int flag)
{
	INT8U err;
	int idx,size;


	if(audioCtrl.busyQ == NULL || audioCtrl.idleQ == NULL)
	{
		if(flag==4)
		   audioPlaybackStop();
		return ;
	}

	if(flag&DAC_INT_PEND)
	{
		hal_dacFlush(audioCtrl.datasize);
		if(audioCtrl.curIdx!=0xff)
		{
			XMsgQPost(audioCtrl.idleQ,(MSG_T *)(INT32)audioCtrl.curIdx);
		}
		audioCtrl.curIdx = audioCtrl.nextIdx;
		audioCtrl.nextIdx = 0xff;
		return ;
	}
	if(audioCtrl.prepause)
	{
		if((flag&DAC_INT_EMPTY)==0)
			return ;
		if(audioCtrl.curIdx!=0xff)
			XMsgQPost(audioCtrl.idleQ,(MSG_T *)(INT32)audioCtrl.curIdx);
		audioCtrl.curIdx = 0xff;
		hal_dacPlayStop();
		audioCtrl.dacstat = 0;
		audioCtrl.stat = MEDIA_STAT_PAUSE;
	    hal_sdUnlock();
		return ;
	}
	
	if(flag&DAC_INT_HALF)
	{
		if(audioCtrl.nextIdx==0xff)
		{
			size = (int)XMsgQPend(audioCtrl.busyQ,&err); 
			if(err == X_ERR_NONE)
			{
				idx = size&0xff;
	            size = size>>8;
				hal_dacSetBuffer((INT32U)(audioCtrl.audiobuffer+AUDIO_BUFF_SIZE*idx),size);
				audioCtrl.nextIdx = idx;
			}
		}
	}
	else if(flag&DAC_INT_EMPTY)
	{
		if(audioCtrl.dacstat == 3)
			audioPlaybackStop();
		else
		{
			hal_dacPlayStop();
		    audioCtrl.dacstat = 0;
		}
	}
	
    if(audioCtrl.curIdx != 0xff)  // current buffer back to idle q
    {
		idx = audioCtrl.curIdx;
		audioCtrl.curIdx = 0xff;
		XMsgQPost(audioCtrl.idleQ,(MSG_T *)idx);
    }
}
/*******************************************************************************
* Function Name  : audioPlaybackInit
* Description    : initial audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackInit(void)
{
    int i;

	audioCtrl.ringbuffer = (INT8U *)hal_sysMemMalloc(AUDIO_RING_SIZE,64);//shareMemMalloc(AUDIO_RING_SIZE);//aviIdx1Cache;
	audioCtrl.audiobuffer = (INT8U *)hal_sysMemMalloc(AUDIO_BUFF_NUM*AUDIO_BUFF_SIZE,64);//shareMemMalloc(AUDIO_BUFF_NUM*AUDIO_BUFF_SIZE);//audioCache;

	if(audioCtrl.ringbuffer == NULL || audioCtrl.audiobuffer == NULL)
	{
		audiop_print("audio play : malloc mem fail.\n");
		hal_sysMemFree(audioCtrl.ringbuffer);
		hal_sysMemFree(audioCtrl.audiobuffer);
		return STATUS_FAIL;
	}
	audioCtrl.idleQ = XMsgQCreate(audsIStack,AUDIO_CFG_BUFFER_NUM);
	audioCtrl.busyQ = XMsgQCreate(audsBStack,AUDIO_CFG_BUFFER_NUM);
	for(i=0;i<AUDIO_BUFF_NUM;i++)
	{
//		audioCtrl.buffer[i] = jpegCache+AUDIO_BUFF_SIZE*i;
		XMsgQPost(audioCtrl.idleQ,(MSG_T *)i);
	}

    audioCtrl.stat = MEDIA_STAT_STOP;
	audioCtrl.dacstat = 0;
	if(audioCtrl.volume == 0)
		audioCtrl.volume = 100;
	hal_dacSetVolume(audioCtrl.volume);
	audioCtrl.curIdx = 0xff;
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackUninit
* Description    : uninitial audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackUninit(void)
{
	if(audioCtrl.dacstat)
		audioPlaybackStop();
	audioCtrl.stat = MEDIA_STAT_STOP;

	XMsgQDestory(audioCtrl.idleQ);
	XMsgQDestory(audioCtrl.busyQ);
//    audac_play_callback_register(NULL);
	audioCtrl.idleQ = NULL;
	audioCtrl.busyQ = NULL;

    if(audioCtrl.ringbuffer)
		hal_sysMemFree(audioCtrl.ringbuffer);
	if(audioCtrl.audiobuffer)
		hal_sysMemFree(audioCtrl.audiobuffer);

	audioCtrl.audiobuffer = NULL;
	audioCtrl.ringbuffer = NULL;
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackStart
* Description    : Start audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackStart(Audio_ARG_T *arg)
{
	int ret=0,i,addr=0;

    USER_WAV_T  wavTar;
	
    if(arg==NULL)
    {
		audiop_print("audio playback : arg null\n");
		return STATUS_FAIL;
    }
	

	if((arg->type != FILE_TYPE_WAV) && (arg->type != FILE_TYPE_RAW)) // only support wav now
	{
		audiop_print("audio playback : type not support %d\n",arg->type);
		return STATUS_FAIL;
	}

	if(audioCtrl.stat != MEDIA_STAT_STOP)
	{
		deg_Printf("audio playback : state error %d\n",audioCtrl.stat);
		return STATUS_FAIL;
	}
	audioCtrl.arg.channel     = arg->channel;
	audioCtrl.arg.samplerate = arg->samplerate;
	audioCtrl.arg.type         = arg->type;

	audioCtrl.arg.media.type = arg->media.type;
	if(arg->media.type == MEDIA_SRC_RAM)
	{
		audioCtrl.arg.size = arg->size;
		if(audioCtrl.arg.size>0x80000)	//define 512K audio size 
		{
			audiop_print("audio playback : ram data to long.max= %x\n",audioCtrl.arg.size);
			return STATUS_FAIL;
		}
		addr = arg->media.src.buff;
		ret = ringBufferInit((INT8U *)arg->media.src.buff,arg->size,&audioCtrl.arg.media);
	}
	else if(arg->media.type == MEDIA_SRC_FS)
	{
		if(arg->media.src.fd<0)
			return STATUS_FAIL;
		audioCtrl.arg.media.src.fd = arg->media.src.fd;	

		ret = ringBufferInit(audioCtrl.ringbuffer,AUDIO_RING_SIZE,&audioCtrl.arg.media);
		
	}
	else if(arg->media.type == MEDIA_SRC_NVFS)
	{
		if(nv_open((int)arg->media.src.fd)<0)
			return STATUS_FAIL;
		audioCtrl.arg.size = nv_size((int)arg->media.src.fd);
		audioCtrl.arg.media.src.fd = arg->media.src.fd;
		ret = ringBufferInit(audioCtrl.ringbuffer,AUDIO_RING_SIZE,&audioCtrl.arg.media);
		
	}
	else 
	{
		audiop_print("audio playback : source error\n");
		return STATUS_FAIL;
	}

	if(ret<0) // ring buffer initial fail
	{
		audiop_print("audio playback :ring buffer fail<%d>\n",ret);
		return STATUS_FAIL;
	}
	audioCtrl.nextIdx = 0xff;
	audioCtrl.framesize = 0;
    audioCtrl.samplerate = arg->samplerate;	
    if(arg->media.type != MEDIA_SRC_RAM)
    {
		if(audioCtrl.busyQ == NULL || audioCtrl.idleQ == NULL)  // audio playback is not initialed
		{
			return STATUS_FAIL;
		}
	    
		if(arg->type == FILE_TYPE_WAV)
		{
			wavTar.channel = audioCtrl.arg.channel;
		    wavTar.samplerate = SAMPLE_RATE_AUTO;//audioCtrl.arg.samplerate;
		    
		    wavDecodeInit(&wavTar);

			ret = wavDecodeParse(0);
			if(ret<0)
			{
				audiop_print("audio playback :fail<%d>\n",ret);
				return STATUS_FAIL;
			}
			audioCtrl.samplerate = wavDecodeGetSamplerate();
		}		
		audioCtrl.dacstat = 0;
		if(audioCtrl.busyQ)
		   XMsgQFlush(audioCtrl.busyQ);
		if(audioCtrl.idleQ)
	       XMsgQFlush(audioCtrl.idleQ);
		for(i=0;i<AUDIO_BUFF_NUM;i++)
		{
			XMsgQPost(audioCtrl.idleQ,(MSG_T *)i);
		}
    }
	hal_dacCallBackRegister(audioPlaybackISR);

	if(arg->media.type == MEDIA_SRC_RAM)// play drictory
	{
		if(addr<= 0)
			return STATUS_FAIL;
		hal_dacSetVolume(audioCtrl.volume);
		ret = hal_dacPlayStart(audioCtrl.samplerate,(INT32U)addr,audioCtrl.arg.size);
		audioCtrl.dacstat = 1;
		if(ret<0)
		{
			audioPlaybackStop();
			return STATUS_FAIL;
		}
	}
    audiop_print("audio playback : start\n");
    audioCtrl.stat = MEDIA_STAT_PLAY;
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackStop
* Description    : Stop audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackStop(void)
{
    audioCtrl.prepause = 0;
	
	hal_dacPlayStop();

//	if(audioCtrl.arg.media.type == MEDIA_SRC_FS)
//		close(audioCtrl.arg.media.src.fd);
	audioCtrl.dacstat = 0;

	audioCtrl.curIdx = 0xff;
	
	audioCtrl.stat = MEDIA_STAT_STOP;
	audiop_print("audio playback : stop!\n");
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackPuase
* Description    : Puase audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackPuase(void)
{
//	int idx;
	if(audioCtrl.stat == MEDIA_STAT_PLAY)
	{
		if(audioCtrl.dacstat)
		{
			audioCtrl.prepause = 1;
		}
		/*hal_dacPlayStop();
		audioCtrl.dacstat = 0;
		if(audioCtrl.curIdx!=0xff)
		{
			idx = audioCtrl.curIdx;
			audioCtrl.curIdx = 0xff;
			XMsgQPost(audioCtrl.idleQ,(MSG_T *)idx);
		}
		audioCtrl.stat = MEDIA_STAT_PAUSE;*/
	}
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackResume
* Description    : Resume audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackResume(void)
{
	if(audioCtrl.stat == MEDIA_STAT_PAUSE)
	{
		audioCtrl.stat = MEDIA_STAT_PLAY;
	}
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackGetStatus
* Description    : get audio Playback 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int audioPlaybackGetStatus(void)
{
	return audioCtrl.stat;
}

void audioPlaybackSetStatus(u8 sta)
{
	audioCtrl.stat=sta;
}

/*******************************************************************************
* Function Name  : videoPlaybackGetTime
* Description    : get video Playback time
* Input          : INT32U *total : total time  ms
                     INT32U *curr : current time ms
* Output         : INT32U *total : total time
                     INT32U *curr : current time
* Return         : int 
                      
*******************************************************************************/
int audioPlaybackGetTime(INT32U *total,INT32U *curr)
{
	INT32U to=0,cur=0;
	
	if(audioCtrl.arg.type == FILE_TYPE_WAV)
	{
		to = wavDecodeGetTotalTime();
		cur=wavDecodeGetCurTime();
	}
	else if(audioCtrl.arg.type == FILE_TYPE_RAW)
	{
		to = (audioCtrl.arg.size>>(audioCtrl.arg.channel))*1000/audioCtrl.arg.samplerate;
		cur = (ringBufferPoint()>>(audioCtrl.arg.channel))*1000/audioCtrl.arg.samplerate;
	}
	if(total)
		*total = to;
	if(curr)
		*curr = cur;
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackSetVolume
* Description    :set audio Playback volume
* Input          : INT8U volume : 0-100
* Return         : int 
                      
*******************************************************************************/
int audioPlaybackSetVolume(INT8U volume)
{
	if(volume>100)
		volume = 100;
	audioCtrl.volume = volume;
	//if(audioCtrl.stat == MEDIA_STAT_PLAY)
    //audac_volume_set(DAC_VOLUME_MAKE(volume));
    hal_dacSetVolume(volume);
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioPlaybackGetVolume
* Description    : get audio Playback volume
* Input          : 
* Return         : INT8U volume : 0-100 
                      
*******************************************************************************/
int audioPlaybackGetVolume(void)
{
	return audioCtrl.volume;
}
/*******************************************************************************
* Function Name  : audioPlaybackService
* Description    : audio play back services
* Input          : 
* Output         : 
* Return         :  
                      
*******************************************************************************/
void audioPlaybackService(void)
{
	INT8U idx,err;
	INT32S ret,size;
	
	if(audioCtrl.stat != MEDIA_STAT_PLAY || audioCtrl.dacstat==3) //decode end or error,wait audac end & stop
		return ;
	if(audioCtrl.arg.media.type == MEDIA_SRC_RAM)  // RAM data will play at one time
	{
		if(audioCtrl.dacstat == 0)
			 audioPlaybackStop();
		return ;
	}

    if(audioCtrl.busyQ == NULL || audioCtrl.idleQ == NULL)  // audio player is not initialed
		return ;

    
//----------get idle buffer--------------
	size = (int)XMsgQPend(audioCtrl.idleQ,&err);
	if(err!=X_ERR_NONE) // no buffer
		return ;
    
	idx = size&0xff;
	if(audioCtrl.framesize!=0)
     	size=audioCtrl.framesize;
	else
		size=AUDIO_BUFF_SIZE;
//---------decode---------------------
    ret = -1;
    if(audioCtrl.arg.type == FILE_TYPE_RAW) // raw data
    {
		ret = ringBufferRead((audioCtrl.audiobuffer+AUDIO_BUFF_SIZE*idx),size);			
    }
    else if(audioCtrl.arg.type == FILE_TYPE_WAV) // wav
    {
	    ret = wavDecodeOneFrame(0,(INT32U)(audioCtrl.audiobuffer+AUDIO_BUFF_SIZE*idx),size);
    }
	if(ret<0 || (ret!=audioCtrl.framesize && audioCtrl.framesize!=0))  // the last frame may be no support the size
    {
	//	deg_Printf("audio playback : decode fail %d\n",ret);
		size = idx;
		XMsgQPost(audioCtrl.idleQ,(MSG_T *)size); // back to queue
		if(audioCtrl.curIdx == 0xff)
		    audioPlaybackStop();
		else
			audioCtrl.dacstat = 3;
		
		return ;
    }
	
//--------play--------------    
    if(audioCtrl.dacstat == 0) // first frame
    {
		audiop_print("audio playback : first frame\n");
		audioCtrl.framesize = ret;
		audioCtrl.datasize = ret;
		hal_dacPlayStart(audioCtrl.samplerate,(INT32U)(audioCtrl.audiobuffer+AUDIO_BUFF_SIZE*idx),ret);
		//audac_play_start(audioCtrl.samplerate,audioCtrl.audiobuffer+AUDIO_BUFF_SIZE*idx,ret,DAC_VOLUME_MAKE(audioCtrl.volume));//0x3fff);//
		audioCtrl.curIdx = idx;
		audioCtrl.dacstat = 1;
    }
	else//--------send to play queue
	{
		#if 0 // do not play this frame,it's seems to the end of music
		if(ret!=audioCtrl.framesize)
		{
			deg_Printf("frame size no support\n");
			
			if(ret<audioCtrl.framesize)
			{
				INT8U *buff;
				buff = audioCtrl.audiobuffer+AUDIO_BUFF_SIZE*idx;
				for(;ret<audioCtrl.framesize;ret++)
					buff[ret] = 0;
			}
			else
			{
				
			}
			ret = audioCtrl.framesize;
		}
		#endif
		size = (ret<<8)|idx;
	    XMsgQPost(audioCtrl.busyQ,(MSG_T *)size);
	}
	ringBufferService();	
}


#endif
