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
* File Name   : avi.h
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
#ifndef  AVI_ERROR_H
#define  AVI_ERROR_H




#define  AVI_ERR_NONE           0   // no error
#define  AVI_ERR_GENERAL       -1   // general error
#define  AVI_ERR_HDRL          -2   // hdrl error,can't find vliad hdrl
#define  AVI_ERR_VIDS          -3   // vids mjpeg ,can't support this type of vids
#define  AVI_ERR_AUDS          -4   // auds pcm,can't support this type of auds
#define  AVI_ERR_INDX          -5   // indx for OPENDML,can't support this type of OPENDML
#define  AVI_ERR_IDX1          -6   // idx1 ,can't find vliad idx1 table
#define  AVI_ERR_DECODER       -7   // avi decoder,can't find supported avi decoder
#define  AVI_ERR_INIT          -8   // decoder initial,decoder initial fail.
#define  AVI_ERR_READ          -9   // avi read callback.no read callback function found or read fail
#define  AVI_ERR_WRITE         -10  // avi write callback.no write callback function found or write fail
#define  AVI_ERR_END           -11  // end of avi vids and auds
#define  AVI_ERR_VFRAME        -12  // inster vids frame error
#define  AVI_ERR_AVIX          -13  // avix fail
#define  AVI_ERR_MEMORY        -14  // memory error
#define  AVI_ERR_FS            -15  // file system no support
#define  AVI_ERR_ENCODER     -16  //too much encoder channel





#define AVI_FRAME_DC_VIDEO      0x01
#define AVI_FRAME_WD_AUDIO      0x02
#define AVI_FRAME_DB_VIDEO      0x04
#define AVI_FRAME_JP_VIDEO      0x08
#define AVI_FRAME_JP_AUDIO      0x10
#define AVI_FRAME_MM_NULL       0x20

#define AVI_FRAME_ERROR          -1
#define AVI_FRAME_DEC_END       0













#endif