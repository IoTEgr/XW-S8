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
* File Name   : audio_record.h
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
#ifndef  AUDIO_RECORD_H
    #define  AUDIO_RECORD_H

#include "../media.h"


typedef struct AUDIO_RARG_S
{
	INT8U  src_type;  // fs , rawdata
	INT8U  tar_type;  // wav{alow,ulow,adpcm,pcm},mp3,...
	INT8U  channel;   // 1,2
	INT8U  looprec; 

	INT32U looptime;
	INT32U samplerate;

	int (*callback)(INT32U cmd,INT32U para);
	
}AUDIO_RARG_T;


enum
{
	AUDIO_CMD_NULL=0,
	AUDIO_CMD_START,
	AUDIO_CMD_STOP,
	AUDIO_CMD_FAIL,

	AUDIO_CMD_MAX
};






/*******************************************************************************
* Function Name  : audioRecordInit
* Description    : initial audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordInit(AUDIO_RARG_T *arg);
/*******************************************************************************
* Function Name  : audioRecordUninit
* Description    : uninitial audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordUninit(void);
/*******************************************************************************
* Function Name  : audioRecordStart
* Description    : start audio record
* Input          : AUDIO_RECORD_ARG_T *arg : audio record argument
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordStart(void);
/*******************************************************************************
* Function Name  : audioRecordStop
* Description    : stop audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordStop(void);
/*******************************************************************************
* Function Name  : audioRecordPuase
* Description    : pause audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordPuase(void);
/*******************************************************************************
* Function Name  : audioRecordResume
* Description    : resume audio record 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int audioRecordResume(void);
/*******************************************************************************
* Function Name  : audioRecordGetStatus
* Description    : get audio record 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int audioRecordGetStatus(void);
/*******************************************************************************
* Function Name  : audioRecordGetTime
* Description    : get audio record time
* Input          : none
* Output         : none
* Return         : int : ms
                      
*******************************************************************************/
int audioRecordGetTime(void);
/*******************************************************************************
* Function Name  : audioRecordService
* Description    : audio record service
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int audioRecordService(void);
















#endif
