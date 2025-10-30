/****************************************************************************
        ***           ***                      MAXLIB-GRAPHIC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX string MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : str.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib string managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "str.h"

#if MAXLIB_STR_USE >0
//#include "typedef.h"
/*******************************************************************************
* Function Name  : str_cpy
* Description    : string copy
* Input          : char *des : destination
*                  char *src : source
* Output         : None
* Return         : char * : destination
*******************************************************************************/
char *str_cpy(char *des,char *src)
{
    char *temp;
    
    temp = des;
    
    while(*src !=0)
    {
        *des++ = *src++;
    }
    *des = 0;    
    return temp;    
}
/*******************************************************************************
* Function Name  : str_ncpy
* Description    : string copy on lenght
* Input          : char *des : destination
*                  char *src : source
*                  u32_t n   : copy lenght
* Output         : None
* Return         : char * : destination
*******************************************************************************/
char *str_ncpy(char *des,char *src,int n)
{
    char *temp;
    
    temp = des;
    while(n--)
    {
        *des++ = *src++;
        if(*src == 0)
            break;
    }
    *des = 0;
    return temp;
}
/*******************************************************************************
* Function Name  : str_char
* Description    : find char 
* Input          : char *des : destination
*                  char  c   : char to be found
* Output         : None
* Return         : char * : destination
*******************************************************************************/
char *str_char(char *src,char c)
{
    while(*src !=0)
    {
        if(*src == c)
        {
            return src;
        }
        src++;
    }
    
    return ((char *)0);
}
/*******************************************************************************
* Function Name  : str_cmp
* Description    : string compare
* Input          : char *des : destination
*                  char *src : source
* Output         : None
* Return         : s32_t : 
*                        0 : equal
*******************************************************************************/
int str_cmp(char *str1,char *str2)
{
    while(*str1 != 0 &&  *str2!=0)
    {
        if(*str1 != *str2)
            return (*str1-*str2);
        str1++;
        str2++;    
    }
    if(*str1 == 0 && *str2 == 0)
        return 0;
    else if(*str1 == 0)    
        return -1;
    else
        return 1;
}
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
int str_ncmp(char *str1,char *str2,int n)
{
    while(*str1 != 0 && *str2!=0 && (n--))
    {
        if(*str1 != *str2)
            return (*str1-*str2);
        str1++;
        str2++;    
    }
    if((*str1 == 0 && *str2 == 0) || n==0)
        return 0;
    else if(*str1 == 0)    
        return -1;
    else
        return 1;
}
/*******************************************************************************
* Function Name  : str_len
* Description    : get string lenght
* Input          : char *str : source
* Output         : None
* Return         : s32_t : 
*                        string lenght
*******************************************************************************/
int str_len(char *str)
{
    u32_t len=0;
    while(*str != 0)
    {
        len++;
        str++;
    }
    return len;
}
/*******************************************************************************
* Function Name  : str_cat
* Description    : cat string
* Input          : char *des : destination
*                  char *src : source
* Output         : None
* Return         :  char *:destination                      
*******************************************************************************/
char *str_cat(char *des,char *src)
{
    char *temp;
    
    temp = des;
    
    while(*des!=0)
        des++;

    while(*src != 0)
    {
        *des++ = *src++;
    }
    *des = 0;
    return temp;
}

#endif