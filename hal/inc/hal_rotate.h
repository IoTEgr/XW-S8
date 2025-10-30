/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         OSD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_rotate.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN ROTATE HAL LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef __HAL_ROTATE_H
#define __HAL_ROTATE_H

enum rotate_channel_e {
    ROTATE_CH0, //video
    ROTATE_CH1, //osd
    ROTATE_CH_MAX,
};

/*******************************************************************************
* Function Name  : hal_rotateInit
* Description    : init rotate_opt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_rotateInit(void);
/*******************************************************************************
* Function Name  : hal_rotateAdd
* Description    : add frames to rotate queue
* Input          : u8 ch
*                  disp_frame_t * src
*                  disp_frame_t * dst
* Output         : None
* Return         : None
*******************************************************************************/
void hal_rotateAdd(u8 ch,disp_frame_t * src,disp_frame_t * dst);

#endif
