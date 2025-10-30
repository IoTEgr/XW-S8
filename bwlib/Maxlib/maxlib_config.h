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
#ifndef  MAXLIB_CONFIG_H 
    #define  MAXLIB_CONFIG_H





#define  MAXLIB_TYPEDEF      0

//#define  MAXLIB_TIME_USE     0    // time 

#define  MAXLIB_STR_USE     1   // str

#define  MAXLIB_MISC_USE    1

#define  MAXLIB_MATH_USE    1

#define  MAXLIB_DRAW_COLOR_SIZE   8  

#define  ICON_MEM_COUNT_MAX    0x1fe00  // 480x272

#define  TASK_TICK_FRQ         1000     // tick frq

#define  THREAD_COUNT          10

#define  MBOX_COUNT            10
#define  MQUE_COUNT            10


#define  MAXLIB_CFG_PRINT      0
//---------color mode----------------------
#if MAXLIB_DRAW_COLOR_SIZE == 16
    typedef u16_t   color_t;
#elif MAXLIB_DRAW_COLOR_SIZE == 8
    typedef u8_t    color_t;
#else
    typedef u16_t   color_t;
#endif

//---------resource------------------------
#define  CFG_R_ICON_AUTO        0    // icon mem malloc.if enable,when malloc fail,R will free the oldest icon to get memory

#define  CFG_R_ICON_LOAD        0    // icon load way.0-load when draw line cache,1-load when draw buffer cache



#endif