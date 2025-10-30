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
* File Name   : ax32xx_osdcmp.c
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

static void (*_isr)(int flag);

/*******************************************************************************
* Function Name  : ax32xx_osdcmpISRRegiser
* Description    : csi isr register
* Input          : void (*isr)(int) : isr
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_osdcmpISRRegiser(void (*isr)(int))
{
    _isr = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_osdcmpCheckBusy
* Description    : osdcmp if not busy
* Input          : 
* Output         : none
* Return         : 0 : free,1 : busy
*******************************************************************************/
u32 ax32xx_osdcmpCheckBusy(void)
{
    return (OSDCMPCON >> 1) & 1;
}
/*******************************************************************************
* Function Name  : ax32xx_osdcmpReset
* Description    : osdcmp hardware reset
* Input          : 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_osdcmpReset(void)
{
    RSTCON &=~(1<<4);
    asm("l.nop\nl.nop\nl.nop\nl.nop\n");
    RSTCON |= (1<<4);
}
/*******************************************************************************
* Function Name  : ax32xx_osdcmpCheckBusy
* Description    : osdcmp compressed size,unit : byte
* Input          : none
* Output         : none
* Return         : size
*******************************************************************************/
u32 ax32xx_osdcmpGetCompSize(void)
{
    return OSDDSTSZ * 4;
}
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
int ax32xx_osdcmpCompOneFrame(u8 * src,u32 src_len,u8 * dst,u32 * dst_len,u8 reverse_en)
{
    const u32 src_addr = reverse_en ? (u32)&src[src_len - 4] : (u32)src;
    
    ax32xx_sysClkSet(SYS_CLK_OSDCMP,1);
    
    OSDCMPCON = (0<<13)|//overflow int en
                (0<<12)|//done int en
                ((reverse_en & 1)<<4)| //rotate en
                (0<<2)| //data format
                (0<<1)| //kick start
                (1<<0); //osdcmp en
    
    OSDSRCADR = src_addr;//src addr
    OSDSRCSZ  = src_len / 4;//src size
    
    OSDDSTADR = (u32)dst;//dst addr
    OSDDSTMSZ = dst_len ? (*dst_len / 4) : OSDSRCSZ;//max output size
    
    OSDCMPPND = (1<<1)|//overflow pending
                (1<<0);//done pending
                
    ax32xx_sysDcacheWback((u32)src,src_len);
    OSDCMPCON |= (1<<1);
    
    int r = ax32xx_osdcmpWaitFrameDone();
    
    if(dst_len)
        *dst_len = OSDDSTSZ * 4;
    
    if(r)
        return -1;
    else
        return 0;
}
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
void ax32xx_osdcmpKick(u8 * src,u32 src_len,u8 * dst,u32 dstbuf_len,u8 reverse_en,u8 int_en)
{
    const u32 src_addr = reverse_en ? (u32)&src[src_len - 4] : (u32)src;
    
    ax32xx_sysClkSet(SYS_CLK_OSDCMP,1);
    
    OSDCMPCON = (int_en<<13)|//overflow int en
                (int_en<<12)|//done int en
                ((reverse_en & 1)<<4)| //rotate en
                (0<<2)| //data format
                (0<<1)| //kick start
                (1<<0); //osdcmp en
    
    OSDSRCADR = src_addr;//src addr
    OSDSRCSZ  = src_len / 4;//src size
    
    OSDDSTADR = (u32)dst;//dst addr
    OSDDSTMSZ = dstbuf_len / 4;//max output size
    
    OSDCMPPND = (1<<1)|//overflow pending
                (1<<0);//done pending
                
    ax32xx_intEnable(IRQ_OSDCMP,int_en);
    OSDCMPCON |= (1<<1);
}
/*******************************************************************************
* Function Name  : ax32xx_osdcmpWaitFrameDone
* Description    : osdcmp wating frame done
* Input          : none
* Output         : none
* Return         : 0 : success,-1 : timeout
*******************************************************************************/
int ax32xx_osdcmpWaitFrameDone(void)
{
    int timeout = APB_CLK / 1000 / 8 * 50;
    
    while(!(OSDCMPPND & 3) && timeout--);
    if(!timeout)
    {
        debg("osdcmp time out\n");
        goto fail;
    }
    
    timeout = APB_CLK / 1000 / 8 * 50;
    while((OSDCMPCON & (1<<1)) && timeout--);
    if(!timeout)
    {
        debg("osdcmp time out2\n");
        goto fail;
    }
    
    u32 pend = OSDCMPPND;
    OSDCMPPND = pend;
    
    if(pend & (1<<1))
    {
        debg("osdcmp overflow\n");
        goto fail;
    }
    
    return 0;
fail:
    return -1;
}
/*******************************************************************************
* Function Name  : ax32xx_osdcmpIRQHandler
* Description    : osdcmp isr
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_osdcmpIRQHandler(void)
{
    u32 sta = OSDCMPPND;
    
    if(_isr)
    {
        _isr(sta);
    }

    OSDCMPPND = sta;
}






