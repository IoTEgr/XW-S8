/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **              MAX ROURCE ICON MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : std_math.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib resource icon managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#ifndef  STD_MATH_H
    #define  STD_MATH_H

#include "./../max.h"




#if  MAXLIB_MATH_USE > 0



/*******************************************************************************
* Function Name  : math_cos
* Description    : get cos value *100
* Input          : u16_t angle : angle
*                  
* Output         : 
* Return         : 
*******************************************************************************/
s16_t math_cos(u16_t angle);
/*******************************************************************************
* Function Name  : math_sin
* Description    : get sin value *100
* Input          : u16_t angle : angle
*                  
* Output         : 
* Return         : 
*******************************************************************************/
s16_t math_sin(u16_t angle);
/*******************************************************************************
* Function Name  : math_abs
* Description	 : math abs value
* Input 		 : s32_t m1 : vaue
*				   
* Output		 : 
* Return		 : 
*******************************************************************************/
u32_t math_abs(s32_t m1);
/*******************************************************************************
* Function Name  : math_max
* Description	 : math max value
* Input 		 : s32_t m1 : value 1
*					 s32_t m2 : value 2
* Output		 : 
* Return		 : 
*******************************************************************************/
s32_t math_max(s32_t m1,s32_t m2);
/*******************************************************************************
* Function Name  : math_min
* Description    : math max value
* Input          : s32_t m1 : value 1
*                    s32_t m2 : value 2
* Output         : 
* Return         : 
*******************************************************************************/
s32_t math_min(s32_t m1,s32_t m2);





#endif

























#endif

