/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                     WATER MARK DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_watermark.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN WATER MARK HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_TMINF_H
   #define AX32XX_TMINF_H





enum
{
	MJPEG_TIMEINFO_LAYER0=0,
	MJPEG_TIMEINFO_LAYER1,
	MJPEG_TIMEINFO_LAYER2,
   MJPEG_TIMEINFO_LAYER3,
	MJPEG_TIMEINFO_MAX
};


/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoEnable
* Description    : mjpeg time info en
* Input          : u8 layer : layer
                      u8 en : 0->disable,
* Output         : None
* Return         : None
*******************************************************************************/
//void ax32xx_mjpegTimeinfoEnable(u8 layer,u8 en);
void ax32xx_mjpegATimeinfoEnable(u8 layer,u8 en);
void ax32xx_mjpegBTimeinfoEnable(u8 layer,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoColor
* Description    : mjpeg time info color in yuv422
* Input          : u32 y : y-byte
                      u32 u : u-bye
                      u32 v : v-byte
* Output         : None
* Return         : None
*******************************************************************************/
//void ax32xx_mjpegTimeinfoColor(u32 y,u32 u,u32 v);
void ax32xx_mjpegATimeinfoColor(u32 y,u32 u,u32 v);
void ax32xx_mjpegBTimeinfoColor(u32 y,u32 u,u32 v);
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoSize
* Description    : mjpeg time info ram size
* Input          : u8 layer : timeinfo layer
                      u32 width    : width -byte
                      u32 height   : height-byte
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegTimeinfoSize(u8 layer,u32 width,u32 height);
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoPos
* Description    : mjpeg time info position
* Input          : u8 layer : timeinfo layer
                      s32 x    : x position
                      s32 y   : y posotion
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegTimeinfoPos(u8 layer,s32 x,s32 y);
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoAddr
* Description    : mjpeg time info adrr
* Input          : u8 layer : timeinfo layer
                      u32 addr: ram addrress
* Output         : None
* Return         : None
*******************************************************************************/
//void ax32xx_mjpegTimeinfoAddr(u8 layer,u32 addr);
void ax32xx_mjpegATimeinfoAddr(u8 layer,u32 addr);
void ax32xx_mjpegBTimeinfoAddr(u8 layer,u32 addr);
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoPos
* Description    : mjpeg time info position
* Input          : u8 layer : timeinfo layer
                      s32 x    : x position
                      s32 y   : y posotion
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegATimeinfoPos(u8 layer,s32 x,s32 y);
void ax32xx_mjpegBTimeinfoPos(u8 layer,s32 x,s32 y);
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoSize
* Description    : mjpeg time info ram size
* Input          : u8 layer : timeinfo layer
                      u32 width    : width -byte
                      u32 height   : height-byte
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegATimeinfoSize(u8 layer,u32 width,u32 height);
void ax32xx_mjpegBTimeinfoSize(u8 layer,u32 width,u32 height);

void ax32xx_recfg_mjpb_tminf(void);
#endif
