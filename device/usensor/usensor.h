/****************************************************************************
**
 **                              DEVICE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  DEVICE G-SENSOR HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : gsensor.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  USENSOR_H
    #define   USENSOR_H



#include "../../hal/inc/hal.h"   
#include "../../xos/xos.h"
#define  PATCH_237A_2 1    //0:旧版的补丁，1：新版的补丁
void usensor_exsrc_update_A1(void);
void usensor_uispsrc_update_A1(void);
void usensor_uispsrc_update_A2(void);
void usensor_uispsrc_update_A2(void);
#endif

