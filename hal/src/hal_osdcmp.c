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
* File Name   : hal_osdcmp.c
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
#include "../inc/hal.h"

static disp_frame_t *p_in,*p_out;
/*******************************************************************************
* Function Name  : hal_osdcmpStart
* Description    : config and start osdcmp
* Input          : disp_frame_t * p_osd_buffer
* Output         : none
* Return         : none
*******************************************************************************/
void hal_osdcmpStart(disp_frame_t * src,disp_frame_t * dst)
{
    if(src && dst)
    {
        if(ax32xx_osdcmpCheckBusy() || p_in)
        {
            debg("[osdcmp] reset\n");
            ax32xx_osdcmpReset();
            hal_osdBufferRelease(p_in);
            hal_osdBufferRelease(p_out);
        }
   
        u8 reverse_en = src->scan_mode == LCD_ROTATE_180;
        p_in = src;
        p_out = dst;
        ax32xx_osdcmpKick(p_in->y_addr,p_in->data_size,
                          p_out->y_addr,p_out->buf_size,reverse_en,1);
    }
    else
    {
        hal_osdBufferRelease(src);
        hal_osdBufferRelease(dst);
    }
}
/*******************************************************************************
* Function Name  : hal_osdcmpIsr
* Description    : frame done & overflow isr
* Input          : int flag 
* Output         : none
* Return         : none
*******************************************************************************/
static void hal_osdcmpIsr(int flag)
{
    //debg("[osdcmp]frame done\n");
    if(flag & 1)
    {
        p_out->data_size = ax32xx_osdcmpGetCompSize();
        hal_osdSetBuffer(OSD0,p_out);
        p_out = NULL;
    }
    
    if(flag & 2)
    {
        deg_Printf("[osdcmp] overflow\n");
    }
    
    //可以不release input-buffer,因为一般input buffer只有一个,会被一直使用
    hal_osdBufferRelease(p_in);
    p_in = NULL;
}
/*******************************************************************************
* Function Name  : hal_osdcmpInit
* Description    : init osdcmp hal layer
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void hal_osdcmpInit(void)
{
    ax32xx_osdcmpISRRegiser(hal_osdcmpIsr);
}
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
int hal_osdcmpCompOneFrame(u8 * src,u32 src_len,u8 * dst,u32 * dst_len,u8 reverse_en)
{
    return ax32xx_osdcmpCompOneFrame(src,src_len,dst,dst_len,reverse_en);
}







