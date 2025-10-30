/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX WATCHDOG
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_wdt.h
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
#ifndef  AX32XX_WDT_H
   #define  AX32XX_WDT_H



typedef enum
{
	WDT_TIME_2MS=0,
	WDT_TIME_8MS,
	WDT_TIME_32MS,
	WDT_TIME_127MS,
	WDT_TIME_512MS,
	WDT_TIME_2048MS,
	WDT_TIME_8192MS,
	WDT_TIME_32768MS
}WDT_TIME_E;


/*******************************************************************************
* Function Name  : ax32xx_wdtEnable
* Description    : enable watch dog
* Input          : u8 en : 1-enable,1-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_wdtTimeSet
* Description    : set watch dog time 
* Input          : u8 wdt_time :  0:2ms ,1:8ms,2:32ms,3:128ms,4:512ms,5:2048ms,6:8192ms,7:32768ms
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtTimeSet(u8 wdt_time);
/*******************************************************************************
* Function Name  : ax32xx_wdtClear
* Description    : clear watch dog 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtClear(void);
/*******************************************************************************
* Function Name  : ax32xx_wdtReset
* Description    : system reset  using wdt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtReset(void);






#endif
