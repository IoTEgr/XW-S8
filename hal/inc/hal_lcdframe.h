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
* File Name   : hal_lcdframe.h
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
#ifndef HAL_LCDFRAME_H
#define HAL_LCDFRAME_H

#include "../../ax32xx/inc/ax32xx_cfg.h"


#if PSRAM_BURST16_EN
#define LCD_DE_BRUST16_EN 		1	// 0: is brust8 , 1: is brust16
#else
#define LCD_DE_BRUST16_EN 		0	// 0: is brust8 , 1: is brust16
#endif

#if LCD_DE_BRUST16_EN 
#define LCD_DE_STRIDE 		(0x40-1)
#else
#define LCD_DE_STRIDE 		(0x20-1)
#endif

enum frame_type_u {
    FTYPE_VIDEO_ROTATE, // 0
                        //video rotate
                        //malloc from CSI中断/回放等非中断
                        //free from rotate中断
    FTYPE_VIDEO_DISPLAY,// 1
                        //video display
                        //malloc from CSI中断/回放等
                        //free from rotate中断，当de有新buffer时，free旧的的buffer
    FTYPE_OSD_ROTATE,   // 2
                        //osd rotate
                        //malloc from UI画图
                        //free from 1.rotate中断，2.当de有新buffer时，free旧的的buffer
    FTYPE_OSD_COMPRESS, // 3
                        //osd compress

    FTYPE_OSD_DISPLAY,  // 4
                        //osd dispaly
                        //malloc from rotate中断
                        //free from 1.osdcmp中断，2.当de有新buffer时，free旧的的buffer
};

typedef struct frame_s {
    struct frame_s * next;
    u8 * y_addr,* uv_addr;
    u8 * _y_addr,* _uv_addr;
    void (*priv_func)(struct frame_s *);
    u32 buf_size; //y & uv total size
    u32 data_size;//for osd
    u16 w,h;
	u16 posX,posY;
    u16 stride;
    u8  scan_mode;
    u8  id;
    u8  pip_sta;
    u8  buf_sta;
    u8  frame_type;
} disp_frame_t;

typedef struct disp_frames_s 
{
    disp_frame_t _frames[3];
    disp_frame_t * freelist;
    u8 frame_nums;
    u8 frame_type;
} disp_frames_t;

struct disp_frames_mgr_s
{
    disp_frames_t types[5];
};

extern struct disp_frames_mgr_s dfm;

/*******************************************************************************
* Function Name  : hal_dispframeInit
* Description    : init all display frames
* Input          : u32 width,u32 height : video/osd display size
*                  u8 scan_mode : video/osd display rotate mode
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeInit(u32 vsrc_w,u32 vsrc_h,u32 osd_w,u32 osd_h,u8 scan_mode);
/*******************************************************************************
* Function Name  : hal_dispframeVideoCfg
* Description    : config video layer position and size
* Input          : p_frame: video frame
* 			   posX:video layer X position
*                    posY:video layer X position
*                    width:video layer width
*                    height:video layer height
posX+width do not large than lcd width
posY+height do not large than lcd height
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeVideoCfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height);

/*******************************************************************************
* Function Name  : hal_dispframeMalloc
* Description    : malloc a idle frame
* Input          : u8 frame_type : enum frame_type_u
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_dispframeMalloc(u8 frame_type);
/*******************************************************************************
* Function Name  : hal_dispframeFree
* Description    : free a frame
* Input          : disp_frame_t * p_frame
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeFree(disp_frame_t * p_frame);
/*******************************************************************************
* Function Name  : hal_dispframeSetCallback
* Description    : set frame callback func
* Input          : disp_frame_t * p_frame
*                  void (*priv_func)(disp_frame_t *)
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeSetCallback(disp_frame_t * p_frame,void (*priv_func)(disp_frame_t *));
/*******************************************************************************
* Function Name  : hal_dispframeGetFrameNums
* Description    : get frame numbers of the type
* Input          : u32 type
* Output         : None
* Return         : None
*******************************************************************************/
u32 hal_dispframeGetFrameNums(u32 type);

#endif
