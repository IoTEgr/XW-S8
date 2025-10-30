/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                     MESSAGE QUEUE 
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xmsgq.c
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
#include "xmsgq.h"
#include "xerr.h"

#if X_CFG_MSGQ_USE > 0

static XMsgQ   XMsgQuePool[X_CFG_MSGQ_MAX] __attribute__ ((section(".sram_comm")));

/*******************************************************************************
* Function Name  : XMsgQInit
* Description    : X message initial
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XMsgQInit(void)
{
	int i;

	for(i=0;i<X_CFG_MSGQ_MAX;i++)
	{
		XMsgQuePool[i].stat = 0;
	}
}
/*******************************************************************************
* Function Name  : XMsgQCreate
* Description    : X message create
* Input          : MSG_T *queue : message queue pool
                      INT8U len        : queue length
* Output         : none                                            
* Return         : XMsgQ * : message queue point
*******************************************************************************/
XMsgQ *XMsgQCreate(MSG_T *queue,INT8U len)
{
    int i;
    
    if(queue == NULL || len == 0)
        return NULL;
    X_CRITIAL_ENTER();
    for(i=0;i<X_CFG_MSGQ_MAX;i++)
    {
        if(XMsgQuePool[i].stat==0)
        {
            XMsgQuePool[i].stat = len;
            XMsgQuePool[i].iput = 0;
            XMsgQuePool[i].oput = 0;
            XMsgQuePool[i].len  = 0;
            XMsgQuePool[i].msg  = queue;
			X_CRITIAL_EXIT();
            return &XMsgQuePool[i];
        }
    }
    X_CRITIAL_EXIT();
    return NULL;
}
/*******************************************************************************
* Function Name  : XMsgQDestory
* Description    : X message destory
* Input          : XMsgQ *mque :  message queue point
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XMsgQDestory(XMsgQ *mque)
{

	X_CRITIAL_ENTER();

    if((mque != NULL) && (mque->stat != 0))
    {
		mque->stat = 0;
    }
        
	X_CRITIAL_EXIT();
}
/*******************************************************************************
* Function Name  : XMsgQFlush
* Description    : X message flush
* Input          : XMsgQ *mque :  message queue point
* Output         : none                                            
* Return         : INT32S : 0 : success
*******************************************************************************/
void XMsgQFlush(XMsgQ *mque)
{
	X_CRITIAL_ENTER();
    if((mque != NULL) && (mque->stat != 0))
    {   
	    mque->iput = 0;
        mque->oput = 0;
        mque->len  = 0;   
    }
	X_CRITIAL_EXIT();
}
/*******************************************************************************
* Function Name  : XMsgQPost
* Description    : X message post
* Input          : XMsgQ *mque :  message queue point
                      MSG_T *msg  : message
* Output         : none                                            
* Return         : INT32S : 0 : success
*******************************************************************************/
INT32S XMsgQPost(XMsgQ *mque,MSG_T *msg)
{
	X_CRITIAL_ENTER();
    if((mque != NULL) && (mque->stat != 0))
    {   
	    if(mque->stat != mque->len)
	    {
	        mque->msg[mque->iput++] = (MSG_T)msg;
		    mque->len++;
		    if(mque->iput>=mque->stat)
		       mque->iput = 0;
            X_CRITIAL_EXIT();
			return X_ERR_NONE;
	    }    
    }
	X_CRITIAL_EXIT();
    return X_ERR_QFULL;
}
/*******************************************************************************
* Function Name  : XMsgQPostFront
* Description    : X message post from the q front
* Input          : XMsgQ *mque :  message queue point
                      MSG_T *msg  : message
* Output         : none                                            
* Return         : INT32S : 0 : success
*******************************************************************************/
INT32S XMsgQPostFront(XMsgQ *mque,MSG_T *msg)
{
	X_CRITIAL_ENTER();
    if((mque != NULL) && (mque->stat != 0))
    {   
	    if(mque->stat != mque->len)
	    {			
			if(mque->oput==0)
				mque->oput = mque->stat-1;
			else
				mque->oput--;
	        mque->msg[mque->oput] = (MSG_T)msg;
		    mque->len++;
            X_CRITIAL_EXIT();
			return X_ERR_NONE;
	    }    
    }
	X_CRITIAL_EXIT();
    return X_ERR_QFULL;
}
/*******************************************************************************
* Function Name  : XMsgQPend
* Description    : X message pend
* Input          : XMsgQ *mque :  message queue point
                      INT8U *err : err state
* Output         : none                                            
* Return         : MSG_T * message
*******************************************************************************/
MSG_T *XMsgQPend(XMsgQ *mque,INT8U *err)
{
    MSG_T value;

	X_CRITIAL_ENTER();
    if((mque == NULL) || (mque->len == 0) || (mque->stat == 0))
    {
        *err = X_ERR_QEMPTY;

		X_CRITIAL_EXIT();
        return NULL;
    }    
    *err = X_ERR_NONE;
    value = mque->msg[mque->oput++];
    mque->len--;
    if(mque->oput>=mque->stat)
       mque->oput = 0;

	X_CRITIAL_EXIT();
    return ((MSG_T *)value);
}
/*******************************************************************************
* Function Name  : XMsgQCheck
* Description    : X message check
* Input          : XMsgQ *mque :  message queue point
* Output         : none                                            
* Return         : MSG_T * message
*******************************************************************************/
INT32S XMsgQCheck(XMsgQ *mque)
{
	INT32S len;

	X_CRITIAL_ENTER();
    len =  mque->len;
	X_CRITIAL_EXIT();

	return len;
}
#endif