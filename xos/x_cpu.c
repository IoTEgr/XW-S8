/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                     CPU 
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : x_cpu.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 12/08/2016
* Description : 
*                 
*                 
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "x_cpu.h"
#include "../ax32xx/inc/ax32xx.h"






extern INT8U XOSNesting;
//static INT32U cpu_sr;
/*void X_CRITIAL_ENTER(void)
{
	if(XOSNesting == 0)
	{
		cpu_sr = SPR_PICMR&(1<<6);

		SPR_PICMR &= ~(1<<6);
	}
}
*/
	/*

void X_CRITIAL_EXIT(void)
{
	if(XOSNesting == 0)
	{
		if(cpu_sr)
		   SPR_PICMR |= 1<<6;
	}
}*/






























