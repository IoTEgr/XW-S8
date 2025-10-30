/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX SDC
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_sd.h
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
#ifndef AX32XX_SD_H
     #define AX32XX_SD_H
#if (AX32_PLATFORM == AX3292)
	#define SDC_WIDTH			   1
#else 
	#define SDC_WIDTH			   1  // 4
#endif
#if SDC_WIDTH == 1
	#define SD_NS_CLK          24000000//12000000
	#define SD_HS_CLK          48000000//24000000//12000000
#else
	#define SD_NS_CLK          24000000//12000000
	#define SD_HS_CLK          36000000//24000000//12000000
#endif

#define SD_BUS_WIDTH4      1
#define SD_BUS_WIDTH1      0

/*******************************************************************************
* Function Name  : ax32xx_sd0Init
* Description    : initial sd0 
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Init(u8 sd_bus_width);
/*******************************************************************************
* Function Name  : ax32xx_sd0Uninit
* Description    :uninitial sd0 
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Uninit(void);
/*******************************************************************************
* Function Name  : ax32xx_sd0SendCmd
* Description    : send command  
* Input          : u8 byCMDIndex : command index
                      u32 dwArgument: command argument
* Output         : None
* Return         : int : -1 :fail
                               1 : success
*******************************************************************************/
s32 ax32xx_sd0SendCmd(u8 byCMDIndex, u32 dwArgument);
/*******************************************************************************
* Function Name  : ax32xx_sd0BusSet
* Description    : set sd0 bus width 
* Input          : u8 type : SD_BUS_WIDTH4: 4-bit,SD_BUS_WIDTH1-1bit
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0BusSet(u8 type);
/*******************************************************************************
* Function Name  : ax32xx_sd0ClkSet
* Description    : set sd0 clock
* Input          : u32 hs_clk,u32 ns_clk 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0ClkSet(u32 hs_clk,u32 ns_clk);
/*******************************************************************************
* Function Name  : ax32xx_sd0Recv
* Description    : sd0 set recv data
* Input          : u32 addr : buffer address
                      u32 len   : length
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Recv(u32 addr,u32 len);
/*******************************************************************************
* Function Name  : ax32xx_sd0Send
* Description    : sd0 set send data
* Input          : u32 addr : buffer address
                      u32 len   : length
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Send(u32 addr,u32 len);
/*******************************************************************************
* Function Name  : ax32xx_sd0WaitDAT0
* Description    : sd0 wait data 0 heigh
* Input          : 
* Output         : None
* Return         : int 1: success
                          -1: fail
*******************************************************************************/
s32 ax32xx_sd0WaitDAT0(void);
/*******************************************************************************
* Function Name  : ax32xx_sd0WaitPend
* Description    : sd0 wait pending flag
* Input          : 
* Output         : None
* Return         : int 1: success
                          -1: fail
*******************************************************************************/
s32 ax32xx_sd0WaitPend(void);
/*******************************************************************************
* Function Name  : ax32xx_sd0GetRsp
* Description    : sd0 get rsp resoult
* Input          : 
* Output         : None
* Return         : int rsp resoult
*******************************************************************************/
s32 ax32xx_sd0GetRsp(void);
/*******************************************************************************
* Function Name  : ax32xx_sdCRCCheck
* Description    : sd crc check
* Input          : u8 mode : 1-write,0-read
* Output         : None
* Return         : int rsp resoult
*******************************************************************************/
s32 ax32xx_sdCRCCheck(u8 mode);
/*******************************************************************************
* Function Name  : ax32xx_sd0Buffer
* Description    : get sd0 static buffer
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
u8 *ax32xx_sd0Buffer(void);











#endif
