/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         CSI DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_rotate.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN CSI HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_ROTATE_H
#define AX32XX_ROTATE_H

/*******************************************************************************
* Function Name  : ax32xx_rotateISRRegiser
* Description    : rotate isr register
* Input          : void (*isr)(void) : isr
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_rotateISRRegiser(void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_rotateCheckBusy
* Description    : rotate if not busy
* Input          : 
* Output         : none
* Return         : 0 : free,1 : busy
*******************************************************************************/
u32 ax32xx_rotateCheckBusy(void);
/*******************************************************************************
* Function Name  : ax32xx_rotateStart
* Description    : config and start rotate
* Input          : u8 src_mode, 0 : video,1 : osd
                   u32 src_width,
                   u32 src_height,
                   u8 *dst_y : dst y addr(aligned 32)
                   u8 *dst_uv : dst uv addr(aligned 32)
                   u32 dst_stride : dst stride(aligned 4byte)
                   u8 *src_y : src y addr(aligned 32)
                   u8 *src_uv : src uv addr(aligned 32)
                   u32 src_stride : src stride(aligned 4byte)
                   u8 int_en : frame done init en
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_rotateStart(
    u8 src_mode,
    u32 src_width,u32 src_height,
    u8 *dst_y,u8 *dst_uv,u32 dst_stride,
    u8 *src_y,u8 *src_uv,u32 src_stride,
    u8 int_en);
/*******************************************************************************
* Function Name  : ax32xx_rotateWaitFrameDone
* Description    : rotate wating frame done
* Input          : none
* Output         : none
* Return         : 0 : success,-1 : timeout
*******************************************************************************/
int ax32xx_rotateWaitFrameDone(void);
/*******************************************************************************
* Function Name  : ax32xx_rotateIRQHandler
* Description    : rotate isr
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_rotateIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_rotateReset
* Description    : rotate hardware reset
* Input          : 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_rotateReset(void);
/*******************************************************************************
* Function Name  : ax32xx_rotateGetSrcYAddr
* Description    : 
* Input          :  
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_rotateGetSrcYAddr(void);
#endif
