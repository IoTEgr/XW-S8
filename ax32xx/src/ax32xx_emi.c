/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         EMI DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_emi.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN EMI HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"

#define R_EMISTA       EMISTA
#define R_EMIHEADER    EMIHEADER
#define R_EMIPND	   EMIPND
#define R_EMICON       EMICON
#define R_EMIBAUD      EMIBAUD
#define R_EMIDMACNT    EMIDMACNT
#define R_EMIDMAADR    EMIDMAADR

static void (*ax32xxEMIDone)(u32 header);
/*******************************************************************************
* Function Name  : ax32xx_emiPinConfig
* Description    : emi pin config
* Input          : u8 mode : select MASTER/SLAVE
* Output         : None
* Return         : none
*******************************************************************************/
static void ax32xx_emiPinConfig(u8 mode)
{
    if(mode == EMI_MASTER)
    {
        //DAT[7:0] = {[PD15,PD12],[PD9,PD6]}
        //CLK:PF10,CS:PF11
        PMAPCON2 = (PMAPCON2 & ~(3<<14))|(1<<14);	//mapping 0,注意与lcd复用
        
        PFFMAP |=  (1<<1)|(1<<2)|(1<<3)|(1<<4);
        PFDIR  &=~( (1<<1)|(1<<2)|(1<<3)|(1<<4));
        PDFMAP |= ((1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)|(1<<10));
        PDDIR  &=~((1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)|(1<<10));
    }
    else if(mode == EMI_SLAVE)
    {
        //DAT[7:0] = [PE11,PE4]
        //CLK:PE12,CS:PE13
        PMAPCON2 = (PMAPCON2 & ~(3<<14))|(7<<14);    //mapping 6,
        
		PEFMAP |= ((1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<13)|(1<<12)|(1<<11)|(1<<10));
        PEDIR  |= ((1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<13)|(1<<12)|(1<<11)|(1<<10));
    }
}
/*******************************************************************************
* Function Name  : ax32xx_emiPinConfig
* Description    : emi init
* Input          : u8 mode : select MASTER/SLAVE
*                  u32 baud: baud rate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiInit(u8 mode,u32 baud)
{
    ax32xx_sysClkSet(SYS_CLK_SPI1,1);
    ax32xx_emiPinConfig(mode);
    
	R_EMIHEADER = 0;
    R_EMICON = (0<<4)|//tx/rx done pending
               (1<<3)|//int en
               (mode<<2)|//mode select:master/slave
               (mode<<1)|//sample clk en
               (1<<0);//en
	if(baud != 0){				   
		R_EMIBAUD = APB_CLK/baud - 1;
	}
	else{
		R_EMIBAUD = 0x0f;
	}	
	R_EMIPND = 1;//clr tx/rx done pending
}
/*******************************************************************************
* Function Name  : ax32xx_emiISRRegister
* Description    : register irq service
* Input          : void (*isr)(u8 header)
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiISRRegister(void (*isr)(u32 header))
{
    ax32xxEMIDone = isr;
    ax32xx_intEnable(IRQ_EMI,1);
}
/*******************************************************************************
* Function Name  : ax32xx_emiIRQHandler
* Description    : emi irs
* Input          : None
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiIRQHandler(void)
{
    if(R_EMIPND & (1<<0))
    {
        R_EMIPND |= (1<<0);
        if(ax32xxEMIDone)
            ax32xxEMIDone(R_EMIHEADER);
    }
}
/*******************************************************************************
* Function Name  : ax32xx_emiKick
* Description    : if emi is master,this will kick tx
*                  if emi is slave,this will kick rx
* Input          : u8 header : only for tx,4bit header will send to slave
*                  u8 * buffer : src buffer for tx,dest buffer for rx
*                  u32 cnt : for tx,means how many bytes to tx
*                            for rx,means how many bytes to rx
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiKick(u32 header,u8 * buffer,u32 cnt)
{
    if(!(R_EMICON & (1<<2)))
    {
		R_EMIHEADER = header;
        //R_EMICON = (R_EMICON & (~(0xf << 8))) | ((header & 0xf) << 8);
    }
	//EMICON   &= ~(1<<4); //clr fifo
    R_EMICON   |= (1<<4); //release fifo
	R_EMIPND =1; //clr emi pending 
    R_EMIDMAADR = (u32)buffer & ~3;
    R_EMIDMACNT = cnt / 4;
}
/*******************************************************************************
* Function Name  : ax32xx_emiCheckBusy
* Description    : check emi is busy or not
* Input          : None
* Output         : None
* Return         : return true if busy
*******************************************************************************/
bool ax32xx_emiCheckBusy(void)
{
	return (R_EMISTA & BIT(0));//1:busy;0:idle
    //return !!R_EMIDMACNT;
}
/*******************************************************************************
* Function Name  : ax32xx_emiCheckRXError
* Description    : check emi rx is error or not
* Input          : None
* Output         : None
* Return         : return true if error
*******************************************************************************/
bool ax32xx_emiCheckRXError(void)
{
	return (R_EMISTA & BIT(1));
    //return (R_EMICON & (1<<7));
}


