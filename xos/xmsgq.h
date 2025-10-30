/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                     MESSAGE QUEUE 
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xmsgq.h
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

#ifndef  XMSGQ_H
    #define  XMSGQ_H

#include "xdef.h"


/*******************************************************************************
* Function Name  : XMsgQInit
* Description	 : X message initial
* Input 		 : none
* Output		 : none 										   
* Return		 : none
*******************************************************************************/
void XMsgQInit(void);
/*******************************************************************************
* Function Name  : XMsgQCreate
* Description	 : X message create
* Input 		 : MSG_T *queue : message queue pool
					  INT8U len 	   : queue length
* Output		 : none 										   
* Return		 : XMsgQ * : message queue point
*******************************************************************************/
XMsgQ *XMsgQCreate(MSG_T *queue,INT8U len);
/*******************************************************************************
* Function Name  : XMsgQDestory
* Description    : X message destory
* Input          : XMsgQ *mque :  message queue point
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XMsgQDestory(XMsgQ *mque);
/*******************************************************************************
* Function Name  : XMsgQPost
* Description    : X message post
* Input          : XMsgQ *mque :  message queue point
                      MSG_T *msg  : message
* Output         : none                                            
* Return         : INT32S : 0 : success
*******************************************************************************/
INT32S XMsgQPost(XMsgQ *mque,MSG_T *msg);
/*******************************************************************************
* Function Name  : XMsgQPostFront
* Description    : X message post from the q front
* Input          : XMsgQ *mque :  message queue point
                      MSG_T *msg  : message
* Output         : none                                            
* Return         : INT32S : 0 : success
*******************************************************************************/
INT32S XMsgQPostFront(XMsgQ *mque,MSG_T *msg);
/*******************************************************************************
* Function Name  : XMsgQPend
* Description    : X message pend
* Input          : XMsgQ *mque :  message queue point
                      INT8U *err : err state
* Output         : none                                            
* Return         : MSG_T * message
*******************************************************************************/
MSG_T *XMsgQPend(XMsgQ *mque,INT8U *err);
/*******************************************************************************
* Function Name  : XMsgQFlush
* Description    : X message flush
* Input          : XMsgQ *mque :  message queue point
* Output         : none                                            
* Return         : INT32S : 0 : success
*******************************************************************************/
void XMsgQFlush(XMsgQ *mque);
/*******************************************************************************
* Function Name  : XMsgQCheck
* Description    : X message check
* Input          : XMsgQ *mque :  message queue point
* Output         : none                                            
* Return         : MSG_T * message
*******************************************************************************/
INT32S XMsgQCheck(XMsgQ *mque);






#endif

