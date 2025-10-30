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
#ifndef  COMMON_H
    #define  COMMON_H

//#include "typedef.h"
//#include "time.h"
#include "./../max.h"





/*******************************************************************************
* Function Name  : CountToString
* Description    : change count to string type
* Input          : char *string : destination
*                    u16_t count1 : count 1
                     u16_t count2 : count 2
* Output         : string
* Return         : 0
*******************************************************************************/
extern s32_t CountToString(char *string,u16_t count1,u16_t count2);
/*******************************************************************************
* Function Name  : stringInster
* Description    : string inster
* Input          : char *des : destination
                      char *src : source
*                    u32_t offset : offset
                     u32_t len : length
* Output         : des
* Return         : 0
*******************************************************************************/
extern s32_t stringInster(char *des,char *src,u32_t offset,u32_t len);
/*******************************************************************************
* Function Name  : strTransform
* Description    : string transform
* Input          : char *str : string
                     u32_t len : length
* Output         : des
* Return         : 0
*******************************************************************************/
extern void strTransform(char *str,u32_t len);
/*******************************************************************************
* Function Name  : HexToString
* Description    : string hex
* Input          : u32_t value : value
                     u32_t lenght : length
* Output         : des
* Return         : char *
*******************************************************************************/
extern char *HexToString(u32_t value,u8_t lenght);
/*******************************************************************************
* Function Name  : NumberToString
* Description    : string number
* Input          : u32_t value : value
                     u32_t lenght : length
* Output         : des
* Return         : char *
*******************************************************************************/
extern char *NumberToString(u32_t value,u8_t lenght);




#endif