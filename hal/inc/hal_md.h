/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         MD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_md.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef HAL_MD_H
#define HAL_MD_H


/*******************************************************************************
* Function Name  : hal_mdInit
* Description    : hal layer .motion dectetion initial
* Input          : none
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_mdInit(void);
/*******************************************************************************
* Function Name  : hal_dacCallback
* Description    : hal layer .motion dectetion enable  set
* Input          : u8 en : 1-enable,0-disable
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_mdEnable(u8 en);


/*******************************************************************************
* Function Name  : hal_mdEnable_check
* Description    : hal layer .motion dectetion enable  check
* Input          : None
* Output         : None
* Return         : Bool : 1 :md enable , 0 : md disable
*******************************************************************************/
#define hal_mdEnable_check ax32xx_mdEnable_check


/*******************************************************************************
* Function Name  : hal_mdCheck
* Description    : hal layer .motion dectetion check md event
* Input          : 
* Output         : None
* Return         : s32 : 0 :no md event happend
                             >0: md event happend
*******************************************************************************/
s32 hal_mdCheck(void);





#endif
