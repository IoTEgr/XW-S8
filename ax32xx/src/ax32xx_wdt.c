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
* File Name   : ax32xx_wdt.c
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

#if CFG_WDT_EN > 0

#define  R_WDT_CTRL    WDTCON

volatile  u32 wdt_en;
/*******************************************************************************
* Function Name  : ax32xx_wdtEnable
* Description    : enable watch dog
* Input          : u8 en : 1-enable,1-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtEnable(u8 en)
{
	if(en){	
	   // R_WDT_CTRL = (5 << 16) | (0 << 12) | (0 << 8) |/*| BIT(4) | BIT(3) |*/ WDT_TIME_8192MS;  // enable watch dog reset in 2048 ms
		R_WDT_CTRL = 0X5500; 
	}
	else
    {
		R_WDT_CTRL = 0X5500;  // disable watch dog,disable reset
    }
}
/*******************************************************************************
* Function Name  : ax32xx_wdtTimeSet
* Description    : set watch dog time 
* Input          : u8 wdt_time : 0:2ms ,1:8ms,2:32ms,3:128ms,4:512ms,5:2048ms,6:8192ms,7:32768ms
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtTimeSet(u8 wdt_time)
{
	
    //R_WDT_CTRL &= ~0x07;
	//R_WDT_CTRL |= (wdt_time&0x07);
}
/*******************************************************************************
* Function Name  : ax32xx_wdtClear
* Description    : clear watch dog 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtClear(void)
{
    if(R_WDT_CTRL & (1<<4))
        R_WDT_CTRL  = (5 << 16) | WDT_TIME_8192MS;
}
/*******************************************************************************
* Function Name  : ax32xx_wdtReset
* Description    : system reset  using wdt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_wdtReset(void)
{
	R_WDT_CTRL = 0x0039;
}



















#endif
