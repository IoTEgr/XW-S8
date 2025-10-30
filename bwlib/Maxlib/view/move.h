/****************************************************************************
        ***           ***                      MAXLIB-GRAPHIC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX VIEW MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : move.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib VIEW managemant
*               
* History     :
* 2016-09-09  : 
*      <1>.This is created by mark,set version as v0102.
*      <2>.Add basic functions.

******************************************************************************/

#ifndef  MOVE_H
    #define  MOVE_H

//#include "typedef.h"


enum
{
	MOVE_EVENT_STOP = 0,
	MOVE_EVENT_START,
	MOVE_EVENT_PAUSE,

	MOVE_EVENT_MAX
};



/*******************************************************************************
* Function Name  : moveInit
* Description    : initial move event node
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveInit(void);
/*******************************************************************************
* Function Name  : moveUninit
* Description    : uninitial move event node
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveUninit(void);
/*******************************************************************************
* Function Name  : moveStart
* Description    : start moving
* Input          :  u32_t viewid : view id
                       s16_t endx   : end x
                       s16_t endy   : end y
                       s16_t detax  : deta x
                       s16_t detay  : deta y
                       s32_t (*callback)(u32_t viewid,u32_t event) : callback when move end
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveStart(u32_t viewid,s16_t endx,s16_t endy,s16_t detax,s16_t detay,s32_t (*callback)(u32_t viewid,u32_t event));
/*******************************************************************************
* Function Name  : moveStop
* Description    : stop moving
* Input          :  u32_t viewid : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveStop(u32_t viewid);
/*******************************************************************************
* Function Name  : movePuase
* Description    : pause moving
* Input          :  u32_t viewid : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t movePuase(u32_t viewid);
/*******************************************************************************
* Function Name  : moveResume
* Description    : resume moving
* Input          :  u32_t viewid : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveResume(u32_t viewid);
/*******************************************************************************
* Function Name  : moveService
* Description    : moving service
* Input          :  
* Output         : 
* Return         : 
*******************************************************************************/
void moveService(void);



































#endif

