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

#ifndef  STR_H
   #define  STR_H

#include "./../max.h"



#if MAXLIB_STR_USE >0
/*******************************************************************************
* Function Name  : str_cpy
* Description    : string copy
* Input          : char *des : destination
*                  char *src : source
* Output         : None
* Return         : char * : destination
*******************************************************************************/
extern char *str_cpy(char *des,char *src);
/*******************************************************************************
* Function Name  : str_ncpy
* Description    : string copy on lenght
* Input          : char *des : destination
*                  char *src : source
*                  u32_t n   : copy lenght
* Output         : None
* Return         : char * : destination
*******************************************************************************/
extern char *str_ncpy(char *des,char *src,int n);
/*******************************************************************************
* Function Name  : str_char
* Description    : find char 
* Input          : char *des : destination
*                  char  c   : char to be found
* Output         : None
* Return         : char * : destination
*******************************************************************************/
extern char *str_char(char *stc,char c);
/*******************************************************************************
* Function Name  : str_cmp
* Description    : string compare
* Input          : char *des : destination
*                  char *src : source
* Output         : None
* Return         : s32_t : 
*                        0 : equal
*******************************************************************************/
extern int str_cmp(char *str1,char *str2);
/*******************************************************************************
* Function Name  : str_ncmp
* Description    : string compare on lenght
* Input          : char *des : destination
*                  char *src : source
*                  u32_t n   : compare lenght
* Output         : None
* Return         : s32_t : 
*                        0 : equal
*******************************************************************************/
extern int str_ncmp(char *str1,char *str2,int n);
/*******************************************************************************
* Function Name  : str_len
* Description    : get string lenght
* Input          : char *str : source
* Output         : None
* Return         : s32_t : 
*                        string lenght
*******************************************************************************/
extern int str_len(char *str);
/*******************************************************************************
* Function Name  : str_cat
* Description    : cat string
* Input          : char *des : destination
*                  char *src : source
* Output         : None
* Return         :  char *:destination                      
*******************************************************************************/
extern char *str_cat(char *dest,char *src);




#endif












#endif
