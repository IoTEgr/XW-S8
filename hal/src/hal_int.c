/****************************************************************************
**
 **                          BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 GPIO HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_int.c
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
#include "../inc/hal.h"


/*******************************************************************************
* Function Name  : hal_intInit
* Description    : int initial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_intInit(void)
{
//===for example===
	hal_intEnable(IRQ_GPIO,0);			// gpio int disable
	hal_int_priority(IRQ_GPIO,1);		// set  gpio int is high priority

}




