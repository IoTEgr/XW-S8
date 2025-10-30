/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA audio record
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : audio_record.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is audio record header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"

#if MEDIA_CFG_AUDIO_RECORD_EN > 0

#if AUDIO_DBG_REC_EN
    #define  audior_print   deg_Printf
#else
    #define  audior_print(...)
#endif
typedef struct Audio_RCtrl_S
{
	INT16U  stat; 	
	INT16U  cmd;
	
	INT32U audioRecordTimeCounter;

#if AUDIO_DBG_REC_EN		
    INT32U  dbg_sampleCnt;
#endif
	FHANDLE wavFd;
	
	XWork_T *audioSync;

	AUDIO_RARG_T arg;
	
}Audio_RCtrl_T;

static Audio_RCtrl_T audioRCtrl;

static void audioRecordIsr(void)
{
	if(audioRCtrl.stat == MEDIA_STAT_START)
	{
		audioRCtrl.audioRecordTimeCounter++;
	}
}
/*******************************************************************************
* Function Name  : audioRecordInit
* Description    : initial audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordInit(AUDIO_RARG_T *arg)
{
    if(arg == NULL)
		return STATUS_FAIL;

	if(audioRCtrl.audioSync == NULL)
		audioRCtrl.audioSync = XWorkCreate(1000*X_TICK,audioRecordIsr);

	audioRCtrl.arg.callback = arg->callback;
	audioRCtrl.arg.channel  = arg->channel;
	audioRCtrl.arg.looprec  = arg->looprec;
	audioRCtrl.arg.looptime= arg->looptime;
	audioRCtrl.arg.samplerate= arg->samplerate;
	audioRCtrl.arg.tar_type = arg->tar_type;

	audioRCtrl.audioRecordTimeCounter = 0;

	audioRCtrl.stat = MEDIA_STAT_STOP;
	
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioRecordUninit
* Description    : uninitial audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordUninit(void)
{
	XWorkDestory(audioRCtrl.audioSync);

	audioRCtrl.audioSync = NULL;
	
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioRecordStart
* Description    : start audio record
* Input          : AUDIO_RECORD_ARG_T *arg : audio record argument
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordStart(void)
{
	int ret;
	USER_WAV_T src;
	
    if(audioRCtrl.arg.callback == NULL)
		return STATUS_FAIL;

    hal_sdLock();
  
    if(audioRCtrl.arg.callback(CMD_COM_CHECK,0)<0) // check space
	{
		hal_sdUnlock();
		return STATUS_FAIL;   // start fail		
	}
	
	ret = audioRCtrl.arg.callback(CMD_AUDIO_RECORD_START,(INT32U)&audioRCtrl.wavFd);
	if(ret<0)
	{
		hal_sdUnlock();
		return STATUS_FAIL;   // start fail		
	}
    src.channel = audioRCtrl.arg.channel;
	src.samplerate= audioRCtrl.arg.samplerate;
    src.type = WAV_TYPE_PCM;
	
    wavEncodeInit(&src,&src);

	ret = wavEncodeParse(audioRCtrl.wavFd);
	if(ret<0)
	{
		hal_sdUnlock();
		audioRCtrl.arg.callback(CMD_COM_ERROR,CMD_AUDIO_RECORD_START); // tell appliction,cmd start fail
		return STATUS_FAIL;
	}
    if(hal_auadcMemInit()<0)
    {
		audior_print("audio record : auadc memroy malloc fail.\n");
		return STATUS_FAIL;
    }
	hal_auadcStartForWAV(audioRCtrl.arg.samplerate,100);
#if AUDIO_DBG_REC_EN
    audioRCtrl.dbg_sampleCnt = 0;
#endif
	audioRCtrl.audioRecordTimeCounter = 0;
	audioRCtrl.stat = MEDIA_STAT_START;
    audior_print("audio record : start\n");
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioRecordStop
* Description    : stop audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordStop(void)
{
    ax32xx_auadcEnable(0);

	wavEncodeEnd(audioRCtrl.wavFd);

    if(audioRCtrl.arg.callback)
	     audioRCtrl.arg.callback(CMD_AUDIO_RECORD_STOP,(INT32U)&audioRCtrl.wavFd); 

	hal_sdStop();
	
    hal_sdUnlock();

	audioRCtrl.stat = MEDIA_STAT_STOP;
	audior_print("audio record : stop\n");
	
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioRecordPuase
* Description    : pause audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordPuase(void)
{
	audioRCtrl.stat = MEDIA_STAT_PAUSE;
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioRecordResume
* Description    : resume audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordResume(void)
{
	audioRCtrl.stat = MEDIA_STAT_START;
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : audioRecordGetStatus
* Description    : get audio record 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int audioRecordGetStatus(void)
{
	return audioRCtrl.stat;
}
/*******************************************************************************
* Function Name  : audioRecordGetTime
* Description    : get audio record time
* Input          : none
* Output         : none
* Return         : int : ms
                      
*******************************************************************************/
int audioRecordGetTime(void)
{
	return audioRCtrl.audioRecordTimeCounter*1000;
}
/*******************************************************************************
* Function Name  : audioRecordError
* Description    : audio record error
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
void audioRecordError(void)
{
	ax32xx_auadcEnable(0);

	if(audioRCtrl.arg.callback)
	     audioRCtrl.arg.callback(CMD_COM_ERROR,(INT32U)&audioRCtrl.wavFd); 

	hal_sdStop();
	
    hal_sdUnlock();

	audioRCtrl.stat = MEDIA_STAT_STOP;
	
	audior_print("audio record : error\n");
}
/*******************************************************************************
* Function Name  : audioRecordService
* Description    : audio record service
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int audioRecordService(void)
{
	INT32U len,addr;
	void *buff;
	INT32S res;
	s32 syncnt,syncnt_next;

	if(audioRCtrl.stat == MEDIA_STAT_PAUSE)
	{
	//---release useless raw data	
		buff = hal_auadcBufferGet(&addr,&len,&syncnt,&syncnt_next);
		if(buff)
			hal_auadcBufferRelease(buff);
	}
	
	if(audioRCtrl.stat != MEDIA_STAT_START)
		return audioRCtrl.stat;



    buff = hal_auadcBufferGet(&addr,&len,&syncnt,&syncnt_next);
	if(buff)
	{
		res = write(audioRCtrl.wavFd,(void *)addr,(UINT)len);
		hal_auadcBufferRelease(buff);
		if((res<0))// write error
		{
			goto AUDIO_RECORD_ERROR;
		}
		
		wavEncodeAddSize(len);
	}


	if(audioRCtrl.audioRecordTimeCounter>=audioRCtrl.arg.looptime)
	{
		audioRecordStop();
	    if(audioRCtrl.arg.looprec)
			audioRecordStart();
	}
	
#if AUDIO_DBG_REC_EN	
	audioRCtrl.dbg_sampleCnt+=len;
	if(audioRCtrl.dbg_sampleCnt>=(audioRCtrl.arg.samplerate*audioRCtrl.arg.channel*2))
	{
		audioRCtrl.dbg_sampleCnt = 0;
		audior_print(".");
	}
#endif

	return audioRCtrl.stat;

AUDIO_RECORD_ERROR:
    audioRecordError(); // audio recorder can not handler this error.stop
	
	return audioRCtrl.stat;
}




#endif


