/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                     CONFIRGE 
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xcfg.h
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
#ifndef  XDEF_H
    #define  XDEF_H



#include "xcfg.h"
#include "xerr.h"
#include "x_cpu.h"






typedef INT32U  MSG_T;

typedef struct X_MSG_S
{
    INT8U stat;
    INT8U len;    
    INT8U oput;
    INT8U iput;
    INT32U *msg;
}XMbox,XMsgQ,XMsg;







































#endif


