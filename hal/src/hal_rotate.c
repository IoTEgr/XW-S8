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
* File Name   : hal_rotate.c
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
#include "../inc/hal.h"

static struct rotate_opt_s 
{
    struct rotate_ch_s
    {
        u8 ch;
        disp_frame_t * src,* dst;
    } preset[ROTATE_CH_MAX],rotating;
    
    u8 prior;
} rotate_opt;

/*******************************************************************************
* Function Name  : hal_rotateKick
* Description    : start rotate module
* Input          : u8 ch
*                  disp_frame_t * src
*                  disp_frame_t * dst
* Output         : None
* Return         : None
*******************************************************************************/
static void hal_rotateKick(u8 ch,disp_frame_t * src,disp_frame_t * dst)
{
    int timeout = 10;
    //实际运行过程中，这里不允许出现busy
    while(ax32xx_rotateCheckBusy() && --timeout);
    if(!timeout)
    {
        debg("[rotate reset]%X,%X,%X,%X\n",ROTATECON,
                                           rotate_opt.rotating.ch,
                                           rotate_opt.rotating.src,
                                           rotate_opt.rotating.dst);
        //ax32xx_wdtEnable(0);
        while(1);
    }
    
    rotate_opt.rotating.ch  = ch;
    rotate_opt.rotating.src = src;
    rotate_opt.rotating.dst = dst;
    
    ax32xx_rotateStart(ch,src->w,src->h,
                       dst->y_addr,dst->uv_addr,dst->stride,
                       src->y_addr,src->uv_addr,src->stride,
                       1);
}

static void hal_rotateFrameDoneIsr(void)
{
    //debg("[rotate done] %d\n",rotate_opt.rotating.ch);

    //free src frame
    hal_dispframeFree(rotate_opt.rotating.src);
    
    //deal with dst frame
    if(rotate_opt.rotating.ch == ROTATE_CH0)
    {
        hal_lcdSetFrame(0,0,0,0,rotate_opt.rotating.dst,0);
    }
    else
    {
        hal_osdDealBuffer(rotate_opt.rotating.dst);
    }
    
    //reset using
    rotate_opt.rotating.ch  = -1;
    rotate_opt.rotating.src = 
    rotate_opt.rotating.dst = NULL;
    
    //如果preset有值,处理preset
    int ch = rotate_opt.prior;
    if(!rotate_opt.preset[ch].src)
    {
        ch = rotate_opt.prior ^ 1;
    }
    
    disp_frame_t * src = rotate_opt.preset[ch].src;
    disp_frame_t * dst = rotate_opt.preset[ch].dst;
    if(src)
    {
        rotate_opt.preset[ch].src = 
        rotate_opt.preset[ch].dst = NULL;
        
        hal_rotateKick(ch,src,dst);
    }
}
/*******************************************************************************
* Function Name  : hal_rotateInit
* Description    : init rotate_opt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_rotateInit(void)
{
    memset(&rotate_opt,0,sizeof(rotate_opt));
    rotate_opt.prior = ROTATE_CH1;
    rotate_opt.preset[ROTATE_CH0].ch = ROTATE_CH0;
    rotate_opt.preset[ROTATE_CH1].ch = ROTATE_CH1;
    
    ax32xx_rotateISRRegiser(hal_rotateFrameDoneIsr);
}
/*******************************************************************************
* Function Name  : hal_rotateAdd
* Description    : add frames to rotate queue
* Input          : u8 ch
*                  disp_frame_t * src
*                  disp_frame_t * dst
* Output         : None
* Return         : None
*******************************************************************************/
void hal_rotateAdd(u8 ch,disp_frame_t * src,disp_frame_t * dst)
{
    HAL_CRITICAL_INIT();
    HAL_CRITICAL_ENTER();
	if(dst&&src)
		hal_dispframeVideoCfg(dst,src->posX,src->posY,src->w,src->h);
    if(rotate_opt.rotating.src)
    {
        //先释放preset
        if(rotate_opt.preset[ch].src)
        {
            hal_dispframeFree(rotate_opt.preset[ch].src);
            hal_dispframeFree(rotate_opt.preset[ch].dst);
        }
        
        //更新preset
        rotate_opt.preset[ch].src = src;
        rotate_opt.preset[ch].dst = dst;
    }
    else
    {
        hal_rotateKick(ch,src,dst);
    }

    HAL_CRITICAL_EXIT();
}

