/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX INTRRUPUT
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_rotate.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"

static void (*framedone_isr)(void);

/*******************************************************************************
* Function Name  : ax32xx_rotateISRRegiser
* Description    : rotate isr register
* Input          : void (*isr)(void) : isr
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_rotateISRRegiser(void (*isr)(void))
{
    framedone_isr = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_rotateCheckBusy
* Description    : rotate if not busy
* Input          : 
* Output         : none
* Return         : 0 : free,1 : busy
*******************************************************************************/
u32 ax32xx_rotateCheckBusy(void)
{
    return ROTATECON >> 31;
}
/*******************************************************************************
* Function Name  : ax32xx_rotateReset
* Description    : rotate hardware reset
* Input          : 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_rotateReset(void)
{
    RSTCON &=~(1<<3);
    asm("l.nop\nl.nop\nl.nop\nl.nop\n");
    RSTCON |= (1<<3);
}
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
    u8 int_en)
{
    ax32xx_sysClkSet(SYS_CLK_ROTATE,1);
    
    ROTATECON   = (0<<31)|//kick start
                  (int_en<<2)| //int en
                  (src_mode<<1)| //mode : 0:video,1:osd
                  (1<<0); //enable
                  
    ROTATESTA   = (1<<0); //rotate pending/clear

    ROTIMGSIZE  = (((src_height&~1)-1)<<16)|//src height
                  (((src_width &~1)-1)<<0); //src width
                  
    ROTSRCYADR  = (((u32)src_y &~0x3F)<<0);//src y addr(aligned 32)
    ROTSRCUVADR = (((u32)src_uv&~0x3F)<<0);//src uv addr(aligned 32)

    ROTTGYADR   = (((u32)dst_y &~0x1F)<<0);//dst y addr(aligned 32)
    ROTTGUVADR  = (((u32)dst_uv&~0x1F)<<0);//dst uv addr(aligned 32)

    ROTADDRSTRI = ((dst_stride/4)<<16)|//dst stride(unit:word,aligned 4byte)
                  ((src_stride/4)<<0); //src stride(unit:word,aligned 4byte)
                  
    ax32xx_intEnable(IRQ_ROTATE,int_en);
    
    ROTATECON  |= (1<<31);//kick
}
/*******************************************************************************
* Function Name  : ax32xx_rotateWaitFrameDone
* Description    : rotate wating frame done
* Input          : none
* Output         : none
* Return         : 0 : success,-1 : timeout
*******************************************************************************/
int ax32xx_rotateWaitFrameDone(void)
{
    int timeout = APB_CLK / 1000 / 8 * 50;
    
    while(!(ROTATESTA & (1<<0)) && timeout--);
    ROTATESTA   = (1<<0);
    
    if(timeout)
        return 0;
    else
    {
        debg("rotate time out\n");
        return -1;
    }
}
/*******************************************************************************
* Function Name  : ax32xx_rotateIRQHandler
* Description    : rotate isr
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_rotateIRQHandler(void)
{
    if(ROTATESTA & 1)
    {
        ROTATESTA = 1;
        if(framedone_isr)
            framedone_isr();
    }
}
/*******************************************************************************
* Function Name  : ax32xx_videoGetYAddr
* Description    : set video layer current y buffer addr
* Input          :  
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_rotateGetSrcYAddr(void)
{
	return (ROTSRCYADR&0x1ffffff);
}


