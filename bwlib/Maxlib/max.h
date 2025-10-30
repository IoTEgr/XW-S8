/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **                    MAX HEADER
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : max.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib internal use
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef MAX_H
   #define  MAX_H
   
#include "../../hal/inc/hal.h"

#include "maxlib_config.h"

#include "max_user.h"


#include "./std/misc.h"
#include "./std/str.h"
#include "./std/std_math.h"

//--------------id type---------------------------
#define  R_ID_MASK                    0x00ffffff
#define  R_ID_CHECK                0xff000000
#define  R_ID_TYPE_ICON            0x80000000
#define  R_ID_TYPE_STR             0x81000000
#define  R_ID_TYPE_ASCII           0x82000000
#define  R_ID_TYPE_LAYOUT          0x83000000
#define  R_ID_TYPE_PROGRESSBAR     0x84000000
#define  R_ID_TYPE_VIEW            0x85000000
#define  R_ID_TYPE_DIALOG          0x86000000
#define  R_ID_TYPE_MENU            0x87000000
#define  R_ID_TYPE_WAV_NAME        0x88000000

#define T_CMD_NULL                  0x0000
#define  T_CMD_ACTIVITY            0x1000
#define  T_CMD_TASK                 0x2000
#define  T_CMD_KEY                  0x3000
#define  T_CMD_MASK                 0xf000

#define  WAV_ID(id)   ((id)|R_ID_TYPE_WAV_NAME)
#define  ID_TYPE(id)  ((id)&R_ID_CHECK)
#define  ID_NUM(id)  ((id)&R_ID_MASK)















#endif