/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   ****                      MOTION DETECTION DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_md.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN MOTION DETECTION HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_MD_H
   #define AX32XX_MD_H




/*******************************************************************************
* Function Name  : ax32xx_mdEnable
* Description    : motion dection enable set
* Input          :  u8 en : 1-enable,0-disable
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_mdEnable(u8 en);
/*******************************************************************************
* Function Name  : hal_mdEnable_check
* Description    : hal layer .motion dectetion enable  check
* Input          : None
* Output         : None
* Return         : Bool : 1 :md is enable , 0 : md is disable
*******************************************************************************/
BOOL ax32xx_mdEnable_check(void);
/*******************************************************************************
* Function Name  : ax32xx_mdInit
* Description    : motion dection enable set
* Input          :  U8 cnt : update pre cnt frame. // datasheet says
                      u8 num : block number
                      u8 pixel : pixel dc threashold
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_mdInit(u8 pixel,u8 num,u8 cnt);
/*******************************************************************************
* Function Name  : ax32xx_mdXPos
* Description    : motion dection x position  set
* Input          :  u16 start : x start position
                       u16 end  : x end position
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_mdXPos(u16 start,u16 end);
/*******************************************************************************
* Function Name  : ax32xx_mdYPos
* Description    : motion dection y position set
* Input          :  u16 start : y start position
                       u16 end  : y end position
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_mdYPos(u16 start,u16 end);










#endif