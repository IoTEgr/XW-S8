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
#ifndef  MAX_USER_H
    #define  MAX_USER_H
#include "maxlib_config.h"

#if MAXLIB_CFG_PRINT
extern void uart_Printf(const char *pszStr, ...);
    #define mprintf      uart_Printf
    #define debug_print  uart_Printf
#else
    #define mprintf(...) 
    #define debug_print(...)       
#endif





extern s32_t user_string_size_get(u32_t id,u16_t *width,u16_t *height);
extern u8_t *user_string_data_get(u32_t id);

extern s32_t user_get_icon(u32_t name,u32_t buff,u32_t size);
extern s32_t user_get_iconSize(u32_t name,u16_t *width,u16_t *height);
extern s32_t user_get_icon_addr(u32_t name);
extern void *user_get_icon_line(u32_t addr,u32_t size);

extern s32_t initDrawBuffWH(uint16 width,uint16 height);

extern void useDoubleBuff(bool en);
extern INT8U* getDrawBuff(void);
extern void drawUIService(bool force);
//extern void *max_mem_malloc(int len);
//extern void max_mem_free(void * addr);

#endif