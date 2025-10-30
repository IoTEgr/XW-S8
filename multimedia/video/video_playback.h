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
* File Name   : video_playback.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is video playback file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  VIDEO_PLAYBACK_H
    #define  VIDEO_PLAYBACK_H



typedef struct VIDEO_PARG_S
{
//	INT32S type;   // file type
	INT16U rotate;
    INT16U firstframe;
	Media_Res_T tar;
    Media_Pos_T pos;
    Media_T media;	
}VIDEO_PARG_T;







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
void videoPlaybackClear(INT8U *ybuffer,INT8U *uvbuffer,INT16U width,INT16U height,INT8U y,INT8U uv);
/*******************************************************************************
* Function Name  : videoPlaybackInit
* Description    : initial video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackInit(void);
/*******************************************************************************
* Function Name  : videoPlaybackUninit
* Description    : uninitial video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackUninit(void);
/*******************************************************************************
* Function Name  : videoPlaybackStart
* Description    : Start video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackStart(VIDEO_PARG_T *arg);
/*******************************************************************************
* Function Name  : videoPlaybackStop
* Description    : Stop video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackStop(void);
/*******************************************************************************
* Function Name  : videoPlaybackPuase
* Description    : Puase video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackPuase(void);
/*******************************************************************************
* Function Name  : videoPlaybackResume
* Description    : Resume video Playback 
* Input          : none
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int videoPlaybackResume(void);
/*******************************************************************************
* Function Name  : videoPlaybackGetStatus
* Description    : get video Playback 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackGetStatus(void);
/*******************************************************************************
* Function Name  : audioPlaybackSetVolume
* Description    :set audio Playback volume
* Input          : INT8U volume : 0-100
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackSetVolume(INT8U volume);
/*******************************************************************************
* Function Name  : audioPlaybackGetVolume
* Description    : get audio Playback volume
* Input          : 
* Return         : INT8U volume : 0-100 
                      
*******************************************************************************/
int videoPlaybackGetVolume(void);
/*******************************************************************************
* Function Name  : videoPlaybackGetTime
* Description    : get video Playback time
* Input          : INT32U *total : total time
                     INT32U *curr : current time
* Output         : INT32U *total : total time
                     INT32U *curr : current time
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackGetTime(INT32U *total,INT32U *curr);
/*******************************************************************************
* Function Name  : videoPlaybackGetResolution
* Description    : get video Playback resolution
* Input          : none
* Output         : none
* Return         : Media_Res_T *                       
*******************************************************************************/
Media_Res_T *videoPlaybackGetResolution(void);
/*******************************************************************************
* Function Name  : videoPlaybackGetStatus
* Description    : get video Playback 
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackService(void);
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
int videoPlaybackFastForward(void);
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
int videoPlaybackFastBackward(void);

s8 videoPlaybackGetplayspeed();

/*******************************************************************************
* Function Name  : videoPlaybackThumbnailInit
* Description    : thumbnail initial.clear screen
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailInit(INT8U cnt);
/*******************************************************************************
* Function Name  : videoPlaybackThumbnailUninit
* Description    : thumbnail uninitial.
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailUninit(void);
/*******************************************************************************
* Function Name  : videoPlaybackThumbnailShow
* Description    : thumbnail show one file.only support avi,jpg
* Input          : FHANDLE fd : file handle
                     INT8U ftype : file type
                     INT16S x     : x position
                     INT16S y     : y position
                     INT16U width: thumbnail width
                     INT16U height:thumbnail height
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int videoPlaybackThumbnailOne(FHANDLE fd,INT8U ftype,INT16S x,INT16S y,INT16U width,INT16U height);
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
int videoPlaybackThumbnailProcess(FHANDLE fd,INT8U ftype,INT16U width,INT16U height,INT32U y_addr,INT32U uv_addr);
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
int videoPlaybackThumbnailShow(INT16S x,INT16S y,INT16U width,INT16U height,INT32U y_addr,INT32U uv_addr);

INT32U playBackGetPlayTime(void);





















#endif
