/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                     CONFIRGE 
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xcfg.c
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
#ifndef  XCFG_H
    #define  XFG_H


//------------------x os config--------------------------------------
#define X_CFG_TICK            100    // ticks /s

#define X_TICK                  1        // for user delay, X-TICK delay a tick 10*X_TICK ->10ms

#define X_TICK_TIME           10    // ms


#define X_CFG_MSGQ_USE      1 // msg queue 

#define X_CFG_MBOX_USE      1 // mbox


#define X_CFG_CRIT_USE      1


#define X_CFG_WORK_USE      1



#if X_CFG_MSGQ_USE > 0
    #define  X_CFG_MSGQ_MAX      12   // number of msg queue
#endif


#if X_CFG_MBOX_USE > 0
    #define  X_CFG_MBOX_MAX      8   // number of mbox
#endif



#if X_CFG_WORK_USE > 0
    #define  X_CFG_WORK_MAX      8   // number of work queue
#endif


































#endif

