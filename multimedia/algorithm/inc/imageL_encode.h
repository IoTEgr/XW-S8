/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA IMAGE ENCODE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : image_encode.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  IMAGEL_ENCODE_H
#define  IMAGEL_ENCODE_H


/*
1: init Buf  ,0:uninit Buf
*/
void doublebuf_And_resbuf_Ctrl(u8 en);


int imageEncodeQuadStart(const char *path,const char *prefix, FHANDLE fd, u16 width, u16 height, u8 quality, u8 timestamp,u8 frame_enable,u32 res_idx,u16 pos_X,u16 pos_Y,u8 gap,u32 _crop_levels,u8 pic_16_9);




#endif
