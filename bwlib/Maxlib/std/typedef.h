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
#ifndef  TYPEDEF_H
    #define  TYPEDEF_H


#include "maxlib_config.h"

#if MAXLIB_TYPEDEF > 0



typedef  char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   BOOL;

typedef char  CHAR;
typedef unsigned char	    u8,  U8,  uint8,  UINT8, u8_t ,INT8U;
typedef signed   char	    s8,  S8,  int8,   INT8,  s8_t, INT8S;

typedef unsigned short	u16, U16, uint16, UINT16,u16_t,INT16U;
typedef signed   short	s16, S16, int16,  INT16, s16_t,INT16S;

typedef unsigned 	    u32, U32, uint32, UINT32,u32_t,INT32U;
typedef signed   	    s32, S32, int32,  INT32, s32_t,INT32S;

typedef unsigned long long	u64, U64, uint64, UINT64,u64_t,INT64U;
typedef signed long long	    s64, S64, int64,  INT64, s64_t,INT64S;

typedef unsigned int uint;
typedef unsigned char uchar;

typedef unsigned char           BOOLEAN;

#define  TRUE   1
#define  FALSE	0
#ifndef NULL
    #define NULL ((void *)0)
#endif
#define  ON   	1
#define  OFF	0





#endif




















#endif