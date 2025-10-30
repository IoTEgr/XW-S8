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
* File Name   : ax32xx_osdcmp.h
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
#ifndef AX32XX_OSDCMP_H
#define AX32XX_OSDCMP_H

/*******************************************************************************
* Function Name  : ax32xx_osdcmpISRRegiser
* Description    : csi isr register
* Input          : void (*isr)(int) : isr
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_osdcmpISRRegiser(void (*isr)(int));
/*******************************************************************************
* Function Name  : ax32xx_osdcmpCheckBusy
* Description    : osdcmp if not busy
* Input          : 
* Output         : none
* Return         : 0 : free,1 : busy
*******************************************************************************/
u32 ax32xx_osdcmpCheckBusy(void);
/*******************************************************************************
* Function Name  : ax32xx_osdcmpCheckBusy
* Description    : osdcmp compressed size,unit : byte
* Input          : none
* Output         : none
* Return         : size
*******************************************************************************/
u32 ax32xx_osdcmpGetCompSize(void);
/*******************************************************************************
* Function Name  : ax32xx_osdcmpCompOneFrame
* Description    : config and start osdcmp,and waitting finish
* Input          : u8 * src : src buffer
                   u32 src_len : src data len(aligne 4)
                   u8 * dst : dst buffer
                   u32 * dst_len : dst buffer len
                   u8 reverse_en : compress reverse (rotate 180)
* Output         : u32 * dst_len : compressed data len
* Return         : 0 : success,-1 : timeout or overflow
*******************************************************************************/
int ax32xx_osdcmpCompOneFrame(u8 * src,u32 src_len,u8 * dst,u32 * dst_len,u8 reverse_en);
/*******************************************************************************
* Function Name  : ax32xx_osdcmpStart
* Description    : config and start osdcmp
* Input          : u8 * src : src buffer
                   u32 src_len : src data len(aligne 4)
                   u8 * dst : dst buffer
                   u32 dstbuf_len : dst buffer len
                   u8 reverse_en : compress reverse (rotate 180)
                   u8 int_en : interrupt enable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_osdcmpKick(u8 * src,u32 src_len,u8 * dst,u32 dstbuf_len,u8 reverse_en,u8 int_en);
/*******************************************************************************
* Function Name  : ax32xx_osdcmpWaitFrameDone
* Description    : osdcmp wating frame done
* Input          : none
* Output         : none
* Return         : 0 : success,-1 : timeout
*******************************************************************************/
int ax32xx_osdcmpWaitFrameDone(void);
/*******************************************************************************
* Function Name  : ax32xx_osdcmpIRQHandler
* Description    : osdcmp isr
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_osdcmpIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_osdcmpReset
* Description    : osdcmp hardware reset
* Input          : 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_osdcmpReset(void);
#endif
