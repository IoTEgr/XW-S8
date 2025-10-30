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
* File Name   : hal_osd.h
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
#ifndef HAL_OSD_H
    #define HAL_OSD_H


/*******************************************************************************
* Function Name  : hal_osdInit
* Description    : osd initial
* Input          : osd : osd id,OSD0,OSD1
				 u16 width : width
				 u16 height: height
				u16 x : x
				u16 y: y
				u32 palette : palette addr
                u8 rotate_mode
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdInit(u8 osd,u16 width,u16 height,s16 x,s16 y,u32 palette,u8 rotate_mode);
/*******************************************************************************
* Function Name  : hal_osdEnable
* Description    : osd set enable
* Input          : osd : osd id,OSD0,OSD1
                     u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdEnable(u8 osd,u8 en);
/*******************************************************************************
* Function Name  : hal_osdSetAddr
* Description    : osd set addr
* Input          : osd : osd id,OSD0,OSD1
                      u32 addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetAddr(u8 osd,u32 addr);
/*******************************************************************************
* Function Name  : hal_osdSetPosition
* Description    : osd set position
* Input          : osd : osd id,OSD0,OSD1
				u16 x : x
				u16 y: y
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetPosition(u8 osd,s16 x,s16 y);
/*******************************************************************************
* Function Name  : hal_osdSetPalette
* Description    : osd set palette
* Input          : osd : osd id,OSD0,OSD1
                      u32 addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetPalette(u8 osd,u32 addr);
/*******************************************************************************
* Function Name  : hal_osdSetSize
* Description    : osd set size
* Input          : osd : osd id,OSD0,OSD1
                      u16 width : width
                      u16 height: height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetSize(u8 osd,u16 width,u16 height);
/*******************************************************************************
* Function Name  : hal_osdGetIdleRotateBuffer
* Description    : get free osd rotate(input) buffer
* Input          : None
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_osdGetIdleRotateBuffer(void);
/*******************************************************************************
* Function Name  : hal_osdGetIdleCmpBuffer
* Description    : get free osd compress(input) buffer
* Input          : None
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_osdGetIdleCmpBuffer(void);
/*******************************************************************************
* Function Name  : hal_osdGetIdleDispBuffer
* Description    : get free osd display(input) buffer
* Input          : None
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_osdGetIdleDispBuffer(void);
/*******************************************************************************
* Function Name  : hal_osdBufferRelease
* Description    : set a osdbuffer as free
* Input          : osd_buffer_t * p_osd_buffer
* Output         : None
* Return         : None
*******************************************************************************/
void hal_osdBufferRelease(disp_frame_t * p_osd_buffer);
/*******************************************************************************
* Function Name  : hal_osdSetBuffer
* Description    : osd set addr
* Input          : osd : osd id,OSD0,OSD1
                   disp_frame_t * buf : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetBuffer(u8 osd,disp_frame_t * buf);
/*******************************************************************************
* Function Name  : hal_osdDealBuffer
* Description    : add buffer to queue
* Input          : disp_frame_t * p_osd_buffer
* Output         : None
* Return         : None
*******************************************************************************/
void hal_osdDealBuffer(disp_frame_t * p_osd_buffer);
/*******************************************************************************
* Function Name  : hal_osdGetBufferResolution
* Description    : hardware layer ,get osd buffer resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_osdGetBufferResolution(u8 osd,u16 *width,u16 *height);

/*******************************************************************************
* Function Name  : hal_deSet_DE_CCM
* Description    : set de's ccm
* Input          : u32 *p_ccm
* Output         : None
* Return         : None
*******************************************************************************/
void hal_deSet_DE_CCM(u32 *p_ccm);

/*******************************************************************************
* Function Name  : hal_deSet_DE_SAJ
* Description    : set de's saj
* Input          : u32 *p_saj
* Output         : None
* Return         : None
*******************************************************************************/
void hal_deSet_DE_SAJ(u32 *p_saj);

void hal_osdSetBuff(u8 osd,u32 addr);
u8* hal_osdGetCurBuff(u8 osd);
u8* hal_osdGetFreeBuff(u8 osd);
void hal_osdSetflag(u8 flag);


#endif

