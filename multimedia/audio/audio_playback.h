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
* File Name   : audio_playback.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is audio playback header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  AUDIO_PLAYBACK_H
    #define  AUDIO_PLAYBACK_H




typedef struct Audio_ARG_S
{
   INT8U  type;   // WAV,MP3
   INT8U  channel;   
   INT16U samplerate;  

   INT32U size;
   Media_T media;
}Audio_ARG_T;



#define SAMPLE_RATE_8000     8000
#define SAMPLE_RATE_11025   11025
#define SAMPLE_RATE_12000   12000
#define SAMPLE_RATE_16000   16000
#define SAMPLE_RATE_22050   22050
#define SAMPLE_RATE_24000   24000
#define SAMPLE_RATE_32000   32000
#define SAMPLE_RATE_44100   44100
#define SAMPLE_RATE_48000   48000
#define SAMPLE_RATE_AUTO    0   // sample rate get from music file








/*******************************************************************************
* Function Name  : audioPlaybackInit
* Description    : initial audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackInit(void);
/*******************************************************************************
* Function Name  : audioPlaybackUninit
* Description    : uninitial audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackUninit(void);
/*******************************************************************************
* Function Name  : audioPlaybackStart
* Description    : Start audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackStart(Audio_ARG_T *arg);
/*******************************************************************************
* Function Name  : audioPlaybackStop
* Description    : Stop audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackStop(void);
/*******************************************************************************
* Function Name  : audioPlaybackPuase
* Description    : Puase audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackPuase(void);
/*******************************************************************************
* Function Name  : audioPlaybackResume
* Description    : Resume audio Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioPlaybackResume(void);
/*******************************************************************************
* Function Name  : audioPlaybackGetStatus
* Description    : get audio Playback 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int audioPlaybackGetStatus(void);

void audioPlaybackSetStatus(u8 sta);

/*******************************************************************************
* Function Name  : videoPlaybackGetTime
* Description    : get video Playback time
* Input          : INT32U *total : total time
                     INT32U *curr : current time
* Output         : INT32U *total : total time
                     INT32U *curr : current time
* Return         : int 
                      
*******************************************************************************/
int audioPlaybackGetTime(INT32U *total,INT32U *curr);
/*******************************************************************************
* Function Name  : audioPlaybackSetVolume
* Description    :set audio Playback volume
* Input          : INT8U volume : 0-100
* Return         : int 
                      
*******************************************************************************/
int audioPlaybackSetVolume(INT8U volume);
/*******************************************************************************
* Function Name  : audioPlaybackGetVolume
* Description    : get audio Playback volume
* Input          : 
* Return         : INT8U volume : 0-100 
                      
*******************************************************************************/
int audioPlaybackGetVolume(void);
/*******************************************************************************
* Function Name  : audioPlaybackService
* Description    : audio play back services
* Input          : 
* Output         : 
* Return         :  
                      
*******************************************************************************/
void audioPlaybackService(void);
















#endif

