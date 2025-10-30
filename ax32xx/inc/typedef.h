/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX TYPEDEF
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : TYPEDEF.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdint.h>
#include <stdbool.h>


typedef bool BOOL;
typedef unsigned  char  BYTE;
typedef unsigned  short WORD;
typedef unsigned  long  DWORD;

typedef unsigned char	u8, U8, uint8, UINT8,INT8U,u8_t;
typedef signed   char	s8, S8, int8, INT8,INT8S,s8_t;

typedef unsigned short	u16, U16, uint16, UINT16,INT16U,u16_t;
typedef signed   short	s16, S16, int16, INT16,INT16S,s16_t;

typedef unsigned long	u32, U32, uint32, UINT32,INT32U,u32_t;
typedef signed   long	s32, S32, int32, INT32,INT32S,s32_t;

typedef unsigned long long	u64, U64, uint64, UINT64,INT64U,u64_t;
typedef signed long long	s64, S64, int64, INT64,INT64S,s64_t;

typedef unsigned int uint;
typedef unsigned char uchar;

#define TRUE                    1
#define FALSE	                0
#define NULL                    ((void *)0)
#define ON   	                1
#define OFF	                    0

#define REG32(addr)  	        (addr)
#define BIT(x)     	            (1UL<<(x))

#define	SETB(REG,BIT)			((REG) |= (1UL << (BIT)))
#define	CLRB(REG,BIT)			((REG) &= (~(1UL << (BIT))))
#define	XRLB(REG,BIT)			((REG) ^= (1UL << (BIT)))
#define CHKB(REG,BIT)			(REG & (1UL << BIT))
#define BIT_CK_1(REG,BIT)		(REG & (1UL << BIT))
#define BIT_CK_0(REG,BIT)		(!(REG & (1UL << BIT)))
#define bic_orr(reg,clr,set)    (reg) = ((reg) & ~(clr)) | (set)

#define UNUSED					__attribute__((unused))
#define WEAK					__attribute__((weak))
#define ALIGNED(x) 				__attribute__ ((aligned(x)))
#define SECTION(x)				__attribute__ ((section(x)))

#define	min(a,b)                (((a)<(b))?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define clip(x,a,b)             min(max((x),(a)),(b))
#define ROUND_UP(n0,n1)	        (((n0) + (n1) - 1) / (n1))
#define ARRAY_NUM(array)		(sizeof(array)/sizeof(array[0]))

#endif