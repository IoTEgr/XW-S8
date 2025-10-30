/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                          X WORK QUEUE
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xwork.h
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
#ifndef  XWORK_H
    #define  XWORK_H

#include "xdef.h"


typedef struct X_Work_S
{
	INT32S counter;
	INT32U reload;

	void (*work)(void);
	void *prit;
}XWork,XWork_T;






/*******************************************************************************
* Function Name  : XWorkInit
* Description    : X os wrok initial
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XWorkInit(void);
/*******************************************************************************
* Function Name  : XWorkCreate
* Description    : X os wrok create
* Input          : INT32U ticks : delay ticks
                      void (*func)(void *prit) : worker
                      void *prit : para for worker
* Output         : none                                            
* Return         : 0 : XWork_T *worker point
*******************************************************************************/
XWork_T *XWorkCreate(INT32U ticks,void (*func)(void ));
/*******************************************************************************
* Function Name  : XWorkDestory
* Description    : X os wrok destory
* Input          : XWork_T *xwork : x worker
* Output         : none                                            
* Return         : 0 : success
*******************************************************************************/
int XWorkDestory(XWork_T *xwork);
/*******************************************************************************
* Function Name  : XWorkService
* Description    : X os wrok service
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XWorkService(void);





















#endif

