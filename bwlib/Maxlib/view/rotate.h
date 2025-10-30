/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **              MAX ROURCE ICON MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : Rotate.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib resource icon managemant
*               
* History     :
* 2016-09-08  : 
*      <1>.This is created by mark,set version as v0101.
*      <2>.Add basic functions.
******************************************************************************/

#ifndef  ROTATE_H
    #define   ROTATE_H
//#include "typedef.h"



/*******************************************************************************
* Function Name  : rotateInit
* Description	 : rotate initial
* Input 		 :	
* Output		: 
* Return		 : success

******************************************************************************/
s32_t rotateInit(void);
/*******************************************************************************
* Function Name  : rotateUninit
* Description	 : rotate uninitial
* Output		: 
* Return		 : success

******************************************************************************/
s32_t rotateUninit(void);
/*******************************************************************************
* Function Name  : rotateStart
* Description    : rotate image
* Input          :  u32_t image : image buffer
                       s16_t *x : image draw x
                       s16_t *y : image draw y
                       s16_t *w: image width
                       s16_t *h: image height
                       u16_t degree: rotate angle
                       u32_t *tcolor : color
* Output        : 
* Return         : void *  0 : fail

******************************************************************************/
u32_t rotateStart(u32_t image,s16_t *x,s16_t *y,u16_t *w,u16_t *h,u16_t degree,u32_t *tcolor);
/*******************************************************************************
* Function Name  : rotateExit
* Description    : release rotate buffer
* Input          :  u32_t image : image buffer
* Output        : 
* Return         : 0

******************************************************************************/
s32_t rotateStop(u32_t image);
/*******************************************************************************
* Function Name  : rotateGetData
* Description    :  rotate get data
* Input          :  u32_t image : image 
* Output        : 
* Return         : 0

******************************************************************************/
u32_t rotateGetData(u32_t image);

















































#endif


