/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                     MESSAGE MBOX 
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xmbox.h
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
#ifndef  XMBOX_H
    #define  XMBOX_H


#include "xdef.h"









/*******************************************************************************
* Function Name  : XMboxInit
* Description	 : X message box initial
* Input 		 : none
* Output		 : none 										   
* Return		 : none
*******************************************************************************/
void XMboxInit(void);
/*******************************************************************************
* Function Name  : XMboxCreate
* Description	 : X message box create
* Input 		 : MSG_T *msg : message in box,if need
* Output		 : none 										   
* Return		 : XMbox * message box point
*******************************************************************************/
XMbox *XMboxCreate(MSG_T *msg);
/*******************************************************************************
* Function Name  : XMboxDestory
* Description	 : X message box destory
* Input 		 : XMbox * message box point
* Output		 : none 										   
* Return		 : none
*******************************************************************************/
void XMboxDestory(XMbox *mbox);
/*******************************************************************************
* Function Name  : XMboxPost
* Description	 : X message box post
* Input 		 : XMbox * message box point
                  : MSG_T *msg : message
* Output		 : none 										   
* Return		 : 0 : success
*******************************************************************************/
INT32S XMboxPost(XMbox *mbox,MSG_T *msg);
/*******************************************************************************
* Function Name  : XMboxPend
* Description	 : X message box pend
* Input 		 : XMbox * message box point
                  :INT8U *err : error state
* Output		 : none 										   
* Return		 : MSG_T * message
*******************************************************************************/
MSG_T *XMboxPend(XMbox *mbox,INT8U *err);
































#endif


