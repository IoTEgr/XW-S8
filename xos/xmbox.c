/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                     MESSAGE MBOX 
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xmbox.c
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
#include "xmbox.h"

static XMbox   XMboxPool[X_CFG_MBOX_MAX];

/*******************************************************************************
* Function Name  : XMboxInit
* Description	 : X message box initial
* Input 		 : none
* Output		 : none 										   
* Return		 : none
*******************************************************************************/
void XMboxInit(void)
{
	int i;

	for(i=0;i<X_CFG_MBOX_MAX;i++)
	{
		XMboxPool[i].stat = 0;
	}
}
/*******************************************************************************
* Function Name  : XMboxCreate
* Description	 : X message box create
* Input 		 : MSG_T *msg : message in box,if need
* Output		 : none 										   
* Return		 : XMbox * message box point
*******************************************************************************/
XMbox *XMboxCreate(MSG_T *msg)
{
    int i;

	X_CRITIAL_ENTER();
    for(i=0;i<X_CFG_MBOX_MAX;i++)
    {
        if(XMboxPool[i].stat == 0)
        {
            XMboxPool[i].msg = msg;
            XMboxPool[i].stat= 1;
            if(msg!=NULL)
                XMboxPool[i].len = 1;
            else
                XMboxPool[i].len = 0;
			X_CRITIAL_EXIT();
            return &XMboxPool[i];
        }
    }
    X_CRITIAL_EXIT();
    return NULL;
}
/*******************************************************************************
* Function Name  : XMboxDestory
* Description	 : X message box destory
* Input 		 : XMbox * message box point
* Output		 : none 										   
* Return		 : none
*******************************************************************************/
void XMboxDestory(XMbox *mbox)
{
	X_CRITIAL_ENTER();
    if(mbox != NULL && mbox->stat != 0)
    {
        mbox->stat = 0; 
    }
	X_CRITIAL_EXIT();
}
/*******************************************************************************
* Function Name  : XMboxPost
* Description	 : X message box post
* Input 		 : XMbox * message box point
                  : MSG_T *msg : message
* Output		 : none 										   
* Return		 : 0 : success
*******************************************************************************/
INT32S XMboxPost(XMbox *mbox,MSG_T *msg)
{
	X_CRITIAL_ENTER();
    if(mbox == NULL || mbox->stat == 0)
    {
		X_CRITIAL_EXIT();
        return X_ERR_QEMPTY;
    }   
    mbox->msg = msg;    

    if(mbox->len)
        return X_ERR_QFULL;
    mbox->len = 1;

	X_CRITIAL_EXIT();
    return X_ERR_NONE;   
}
/*******************************************************************************
* Function Name  : XMboxPend
* Description	 : X message box pend
* Input 		 : XMbox * message box point
                  :INT8U *err : error state
* Output		 : none 										   
* Return		 : MSG_T * message
*******************************************************************************/
MSG_T *XMboxPend(XMbox *mbox,INT8U *err)
{
	X_CRITIAL_ENTER();
    if(mbox == NULL || mbox->len == 0 || mbox->stat == 0)
    {
        *err = X_ERR_QEMPTY;
		X_CRITIAL_EXIT();
        return NULL;
    }
    *err = X_ERR_NONE;
    mbox->len = 0;
    X_CRITIAL_EXIT();
    return mbox->msg;
}

