/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                          X WORK QUEUE
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xwork.c
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
#include "xwork.h"


#if X_CFG_WORK_USE > 0


static XWork_T XWorkPool[X_CFG_WORK_MAX];

/*******************************************************************************
* Function Name  : XWorkInit
* Description    : X os wrok initial
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XWorkInit(void)
{
	int i;

	for(i=0;i<X_CFG_WORK_MAX;i++)
	{
		XWorkPool[i].work = NULL;
	}
}

/*******************************************************************************
* Function Name  : XWorkCreate
* Description    : X os wrok create
* Input          : INT32U ticks : delay times .ms
                      void (*func)(void *prit) : worker
                      void *prit : para for worker
* Output         : none                                            
* Return         : 0 : XWork_T *worker point
*******************************************************************************/
//XWork_T *XWorkCreate(INT32U ticks,void (*func)(void *prit),void *prit)
XWork_T *XWorkCreate(INT32U ticks,void (*func)(void ))
{
	int i;

	if(ticks == 0 || func == NULL)
		return NULL;

	X_CRITIAL_ENTER();
    for(i=0;i<X_CFG_WORK_MAX;i++)
	{
		if(XWorkPool[i].work==NULL)
		{
			XWorkPool[i].work = func;
	//		XWorkPool[i].prit = prit;			
			XWorkPool[i].reload = ticks;
			XWorkPool[i].counter = ticks;  // count down
			X_CRITIAL_EXIT();
			return &XWorkPool[i];
		}
	}

	X_CRITIAL_EXIT();
	return NULL;
}
/*******************************************************************************
* Function Name  : XWorkDestory
* Description    : X os wrok destory
* Input          : XWork_T *xwork : x worker
* Output         : none                                            
* Return         : 0 : success
*******************************************************************************/
int XWorkDestory(XWork_T *xwork)
{
	X_CRITIAL_ENTER();
    if(xwork!= NULL)
    {
		xwork->work = NULL;
    }
	X_CRITIAL_EXIT();

	return X_ERR_NONE;
}
/*******************************************************************************
* Function Name  : XWorkService
* Description    : X os wrok service
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void XWorkService(void)
{
	int i;

	for(i=0;i<X_CFG_WORK_MAX;i++)
	{
	//	X_CRITIAL_ENTER();
        if(XWorkPool[i].work!=NULL)
        {
			XWorkPool[i].counter-=X_TICK_TIME;
			if(XWorkPool[i].counter <= 0)
			{
				XWorkPool[i].work();
				XWorkPool[i].counter = XWorkPool[i].reload;
			}
        }
	//	X_CRITIAL_EXIT();
	}
}











#endif

