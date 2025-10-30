/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         GPIO HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_wdt.h
* Author      : Ticru 
* Version     : V0200
* Date        : 05/02/2017
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-05-02  : 
*      <1>.This is created by ticru,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  HAL_WDT_H
#define  HAL_WDT_H


/*******************************************************************************
* Function Name  : hal_wdtEnable(u8 en)
* Description    : enable watch dog
* Input          : u8 en : 1-enable,1-disable
* Output         : None
* Return         : None
*******************************************************************************/
#define hal_wdtEnable 		ax32xx_wdtEnable
/*******************************************************************************
* Function Name  : hal_wdtTimeSet(u8 wdt_time)
* Description    : set watch dog time 
* Input          : u8 wdt_time :  0:2ms ,1:8ms,2:32ms,3:128ms,4:512ms,5:2048ms,6:8192ms,7:32768ms
* Output         : None
* Return         : None
*******************************************************************************/
#define hal_wdtTimeSet		ax32xx_wdtTimeSet
/*******************************************************************************
* Function Name  : hal_wdtClear(void)
* Description    : clear watch dog 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define hal_wdtClear 	ax32xx_wdtClear
/*******************************************************************************
* Function Name  : hal_wdtReset(void)
* Description    : system reset  using wdt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define hal_wdtReset		ax32xx_wdtReset





#endif


