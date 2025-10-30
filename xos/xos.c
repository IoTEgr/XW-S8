/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                              X OS
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xos.c
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
#include "xos.h"


INT8U XOSNesting;
static volatile INT32U XOSTick,Xrandom;
/*******************************************************************************
* Function Name  : XOSInit
* Description    : X os initial
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XOSInit(void)
{
	XOSNesting = 0;
	XOSTick = 0;
#if X_CFG_MSGQ_USE > 0
    XMsgQInit();
#endif

#if X_CFG_MBOX_USE > 0
    XMboxInit();
#endif

}

/*******************************************************************************
* Function Name  : XOSTickHook
* Description    : X os tick service
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XOSTickService(void)
{
	XOSNesting++;
	Xrandom++;

    XOSTick+=X_TICK_TIME;

    XWorkService();
	
	XOSNesting--;
}
/*******************************************************************************
* Function Name  : XOSTimeGet
* Description    : X os tick get
* Input          : none
* Output         : none                                            
* Return         : INT32U tick count
*******************************************************************************/
INT32U XOSTimeGet(void)
{	
	return XOSTick;	
}
/*******************************************************************************
* Function Name  : XOSTimeDly
* Description    : X os tick delay
* Input          : INT32U dely : delay ticks
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XOSTimeDly(INT32U dely)
{
	INT32U temp;
	
	temp = XOSTick;
	if(XOSTick == 0)
		return ;
	while(XOSTick<(temp+dely));
}
/*******************************************************************************
* Function Name  : XOSRandom
* Description    : X random value
* Input          : none
* Output         : none                                            
* Return         : INT32U
*******************************************************************************/
INT32U XOSRandom(void)
{
	return Xrandom;
}










