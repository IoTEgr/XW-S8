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
* File Name   : hal_osdcmp.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN OSD HAL LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef HAL_OSDCMP_H
#define HAL_OSDCMP_H


/*******************************************************************************
* Function Name  : hal_osdcmpCompOneFrame
* Description    : config and start osdcmp,and waitting finish
* Input          : u8 * src : src buffer
                   u32 src_len : src data len(aligne 4)
                   u8 * dst : dst buffer
                   u32 * dst_len : dst buffer len
                   u8 reverse_en : compress reverse (rotate 180)
* Output         : u32 * dst_len : compressed data len
* Return         : 0 : success,-1 : timeout or overflow
*******************************************************************************/
int hal_osdcmpCompOneFrame(u8 * src,u32 src_len,u8 * dst,u32 * dst_len,u8 reverse_en);
/*******************************************************************************
* Function Name  : hal_osdcmpInit
* Description    : init osdcmp hal layer
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void hal_osdcmpInit(void);
/*******************************************************************************
* Function Name  : hal_osdcmpStart
* Description    : config and start osdcmp
* Input          : disp_frame_t * p_osd_buffer
* Output         : none
* Return         : none
*******************************************************************************/
void hal_osdcmpStart(disp_frame_t * src,disp_frame_t * dst);




#endif
