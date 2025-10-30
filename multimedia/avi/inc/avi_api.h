/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                     MULTIMEDIA AVI
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : avi_api.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  AVI_API_H
#define  AVI_API_H

#include "avi_error.h"


enum
{
	AVI_TYPE_STANDARD=0,
	AVI_TYPE_OPENDML
};
#define  AVI_CHANNEL_NUM           2    // avi encoder channel number

/*******************************************************************************
* Function Name  : aviEncodeSetIdx1Length(**B is for mjpB encoder)
* Description	 : INT8U *buffer : buffer for cache avi header,user should read back the avi idx
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeInit(INT8U type,int (*cachewrite)(int fd,void *cachebuffer,INT32U lenght),
							 int (*seek)(int fd,INT32U pos,int flag));
/*******************************************************************************
* Function Name  : aviEncodeUninit
* Description	 : avi ecnoder uninit
* Input 		 : int favi : avi channel hanlder
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeUninit(int favi);
/*******************************************************************************
* Function Name  : aviEncodePrase
* Description	 : avi ecnoder prase
* Input 		 : INT16U width : avi mjpeg width
                  : INT16U height: avi mjpeg height
                  : INT16U fps    : avi video frame rate
                  : INT16U audio : record audio or not
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeParse(int favi,int fhandle,INT16U width,INT16U height,INT16U fps,INT16U audio,INT16U samperate);
/*******************************************************************************
* Function Name  : aviEncodeAddIndex
* Description	 : avi encode add index for idx1
* Input 		 : INT8U *buffer : buffer for cache avi idx1
                  : INT8U type : index type:AVI_FRAME_DC_VIDEO/AVI_FRAME_DB_VIDEO/AVI_FRAME_WD_AUDIO
                  :INT8U keyFrame : 1->key frame,0->not key frame
                  : INT32U size: data size,if size == 0,insert a repeat video frame,only for video frame
                  : INT32U buflen : buffer len
* Output		 : none 										   
* Return		 : int 0
                              <0 fail
*******************************************************************************/
int aviEncodeAddIdx(int favi,INT8U *buff,INT8U type,INT8U keyFrame,INT32U size,INT32U buflen);
/*******************************************************************************
* Function Name  : aviEncodeSetIdx1Length
* Description	 : INT8U *buffer : buffer for cache avi header,user should read back the avi idx
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeSetIdx1(int favi,INT8U *buffer);
/*******************************************************************************
* Function Name  : aviEncodeFrameTag
* Description	 : make avi tag to movi phase
* Input 		 : INT32U *buffer : frame addr
                    INT32U tag : index type:AVI_FRAME_DC_VIDEO/AVI_FRAME_DB_VIDEO/AVI_FRAME_WD_AUDIO
                    INT32U size : fram size
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeFrameTag(int favi,INT32U *buffer,INT32U tag,INT32U size);
/*******************************************************************************
* Function Name  : aviEncodeAddOffset
* Description	 : maybe some error ocured,a error frame is writed to avi file,but this frame can not be used
* Input 		 : INT32U add_offset : size to be add
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeAddOffset(int favi,INT32U add_offset);
/*******************************************************************************
* Function Name  : aviEncodeGetIdx1Length
* Description	 :
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeGetIdx1Length(int favi);
/*******************************************************************************
* Function Name  : aviEncodeGetFrameCount
* Description	 :
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 frame cnt
*******************************************************************************/
int aviEncodeGetFrameCount(int favi);
/*******************************************************************************
* Function Name  : aviEncodeEnd
* Description	 : avi encode end
* Input 		 : INT8U *buffer : buffer for cache avi header,user should read back the avi header from avi file
                  : INT32U fsize   : file size
                  : INT32U idx_offset : idx1 offset from file start
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeEnd(int favi,INT8U *buffer,INT32U fsize,INT32U idx_offset);
/*******************************************************************************
* Function Name  : aviEncodeService
* Description	 : avi encode service
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeService(int favi);



/*******************************************************************************
* Function Name  : aviDecodeInit
* Description	 : avi decode initial
* Input 		 : void *cachebuffer : cache for idx1
                  : INT32U len : cache length
                   :int (*cacheread)(INT32U offset,INT32U lenght) : read call back
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeInit(void *cachebuffer,INT32U len,int (*cacheread)(void *cachebuffer,INT32U offset,INT32U lenght));
/*******************************************************************************
* Function Name  : aviDecodeParse
* Description	 : avi decode parse
* Input 		 : INT8U *buffer : avi file for decode parse
                  : INT32U len : buffer size;
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeParse(int fhandle);
/*******************************************************************************
* Function Name  : aviDecodeService
* Description	 : avi decode cache idx1 table
* Input 		 : none
* Output		 : none 										   
* Return		 : int frame type
*******************************************************************************/
int aviDecodeService(void);
/*******************************************************************************
* Function Name  : aviDecodeOneFrame
* Description	 : avi decode get one frame information
* Input 		 : INT32U *offset : offset of frame data in avi file
                  : INT32U *length : frame data len
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeOneFrame(INT32U *offset,INT32U *length,INT32U type);
/*******************************************************************************
* Function Name  : aviDecodeFast
* Description	 : avi decode fast forward or fast rewind
* Input 		 : int deta : fast frames
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
void aviDecodeFast(int deta);
/*******************************************************************************
* Function Name  : aviDecodeGetTime
* Description	 : avi decode time
* Input 		 : INT32U *totaltime : avi total time
                  :INT32U *curtime   : current time
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeGetTime(INT32U *totaltime,INT32U *curtime);
/*******************************************************************************
* Function Name  : aviDecodeGetAudioSampleRate
* Description	 : avi decode aud string sample rate
* Input 		 : none
* Output		 : none 										   
* Return		 : int : samplerate
*******************************************************************************/
int aviDecodeGetAudioSampleRate(void);
/*******************************************************************************
* Function Name  : aviDecodeGetVideoFrameTime
* Description	 : avi decode video fram time
* Input 		 : none
* Output		 : none 										   
* Return		 : int : video fram time
*******************************************************************************/
int aviDecodeGetVideoFrameTime(void);
/*******************************************************************************
* Function Name  : aviDecodeGetTime
* Description	 : avi decode vids resolution
* Input 		 :INT16U *width : width
                  :INT16U *height: height
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeGetResolution(INT16U *width,INT16U *height);
/*******************************************************************************
* Function Name  : aviDecodeGetFrameIndex
* Description	 : avi decode frame index
* Input 		 :none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeGetFrameIndex(void);


int aviEncodeAddofs(int favi,INT32U offset);
























#endif