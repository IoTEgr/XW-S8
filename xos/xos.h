/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                          X OS
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xos.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 12/08/2016
* Description : 
*                 
*                 
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  XOS_H
    #define  XOS_H

#include "xdef.h"

#if X_CFG_MSGQ_USE > 0
    #include "xmsgq.h"
#endif

#if X_CFG_MBOX_USE > 0
    #include "xmbox.h"
#endif

#if X_CFG_WORK_USE > 0
    #include "xwork.h"
#endif








/*******************************************************************************
* Function Name  : XOSInit
* Description    : X os initial
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XOSInit(void);
/*******************************************************************************
* Function Name  : XOSTickHook
* Description    : X os tick service
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XOSTickService(void);
/*******************************************************************************
* Function Name  : XOSTimeGet
* Description    : X os tick get
* Input          : none
* Output         : none                                            
* Return         : INT32U tick count
*******************************************************************************/
INT32U XOSTimeGet(void);
/*******************************************************************************
* Function Name  : XOSTimeDly
* Description    : X os tick delay
* Input          : INT32U dely : delay ticks
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XOSTimeDly(INT32U dely);
/*******************************************************************************
* Function Name  : XOSRandom
* Description    : X random value
* Input          : none
* Output         : none                                            
* Return         : INT32U
*******************************************************************************/
INT32U XOSRandom(void);











#endif

