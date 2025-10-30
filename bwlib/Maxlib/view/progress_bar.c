/****************************************************************************
        ***           ***                      MAXLIB-GRAPHIC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX PROCESS BAR MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : process_bar.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib process bar managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#include "./../max.h"
#include "./../maxlib.h"

//#include "font.h"
//#include <stdio.h>


#define PROGRESSBAR_ID_TAG       0x20000000
#define PROGRESSBAR_ID_MAKE(t,ct)    (PROGRESSBAR_ID_TAG | t | (ct<<8))
#define PROGRESSBAR_TYPE_GET(id)      (id&0x000000ff)
#define PROGRESSBAR_ID_GET(id)        (id&0xff000000)
#define PROGRESSBAR_CT_GET(id)        (id&0x00ffff00)
#if MAXLIB_PROGRESS_USE > 0

typedef struct ProgressBar_Stat_S
{
	u32_t Handle;
	u32_t Show;
	u32_t Count;
	u32_t Progress;

	ProgressBar_OP_T *OPPool[PROGRESS_BAR_TYPE_MAX];

	s32_t (*CallBack)(u32_t handle,s32_t value);
}ProgressBar_Stat_T;

ProgressBar_Stat_T ProgressBarS;

extern ProgressBar_OP_T style1ProgressBar_op;
/*******************************************************************************
* Function Name  : progressbarInit
* Description    : progress bar initial                
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarInit(void)
{
	s32_t i;

	for(i=0;i<PROGRESS_BAR_TYPE_MAX;i++)
	{
		ProgressBarS.OPPool[i] = NULL;
	}
//---------------------	
    ProgressBarS.OPPool[PROGRESS_BAR_TYPE_STYLE1] = &style1ProgressBar_op;

	ProgressBarS.Handle = PROGRESS_BAR_TYPE_MAX;
	ProgressBarS.Show    = 0;
	ProgressBarS.Count   = 0;
	ProgressBarS.Progress = 0;
	ProgressBarS.CallBack = NULL;

	return 0;
}
/*******************************************************************************
* Function Name  : progressbarunInit
* Description    : progress bar uninitial                
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarUninit(void)
{
	return progressbarInit();
}
/*******************************************************************************
* Function Name  : progressbarRegister
* Description    : progress bar callback function              
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarRegister(u32_t handle,s32_t (*func)(u32_t handle,s32_t value))
{
	 if(handle == ProgressBarS.Handle)
	 	 ProgressBarS.CallBack = func;

	 return 0;
}
/*******************************************************************************
* Function Name  : progressbarActiveGet
* Description    : progress bar state get          
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarActiveGet(void)
{
	return ProgressBarS.Show;
}
/*******************************************************************************
* Function Name  : progressbarOpCtrl
* Description    : progress bar operation bar contrl         
* Input          :   u32_t handle : handle
                        u32_t op_cmd: op command
                        u32_t para    : parameter
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarOpCtrl(u32_t handle,u32_t op_cmd,u32_t para)
{
	u8_t type;
	s32_t ret=0;

	if(op_cmd == PROGRESSBAR_OP_OPEN)
	{
		if(ProgressBarS.Handle!=PROGRESS_BAR_TYPE_MAX || para>=PROGRESS_BAR_TYPE_MAX)
			return -1;
		ret = -1;
		if(ProgressBarS.OPPool[para]->open != NULL)
		{
		    ret = ProgressBarS.OPPool[para]->open((char *)0);
			//printf("dialog : open type = %d,result = %d\r\n",para,ret);
		}
        
		if(ret>=0)
		{
			ProgressBarS.Handle = PROGRESSBAR_ID_MAKE(para,ProgressBarS.Count);
			ret = ProgressBarS.Handle;
			ProgressBarS.Count++;
			if(ProgressBarS.Count>=0x8000)
				ProgressBarS.Count = 0;
			ProgressBarS.Progress = 0;
		}

		return ret;
	}
	if(PROGRESSBAR_ID_GET(handle) != PROGRESSBAR_ID_TAG)
       return -1;

	type = PROGRESSBAR_TYPE_GET(handle);
	if(type>=PROGRESS_BAR_TYPE_MAX)
		return -1;

	if(op_cmd == PROGRESSBAR_OP_SHOW)
	{
		if(ProgressBarS.OPPool[type]->show != NULL)
		    ret = ProgressBarS.OPPool[type]->show(type);
		if(ret>=0)
		    ProgressBarS.Show = 1;
	}
	else if(op_cmd == PROGRESSBAR_OP_CLOSE)
	{
		if(ProgressBarS.OPPool[type]->close!= NULL)
		    ret = ProgressBarS.OPPool[type]->close(type);
		if(ret>=0)
        {
			ProgressBarS.Handle = PROGRESS_BAR_TYPE_MAX;
			ProgressBarS.Show   = 0;
			ProgressBarS.CallBack = NULL;
        }
//		printf("progress bar : close.\r\n");
		viewSetUpdateFlag(1);
	}
	else if(op_cmd == PROGRESSBAR_OP_PROGRESS)
	{
		if(ProgressBarS.OPPool[type]->progress!= NULL)
		    ret = ProgressBarS.OPPool[type]->progress(type,para);
		if(ret>=0)
		{
			ProgressBarS.Progress = ret;			
			//if(ProgressBarS.Show)
			//	ProgressBarS.OPPool[type]->show(type);
			ProgressBarS.CallBack(ProgressBarS.Handle,ProgressBarS.Progress);
		}
	}
	else if(op_cmd >PROGRESSBAR_OP_IOCTRL)
	{
		if(ProgressBarS.OPPool[type]->ioctrl!= NULL)
		    ret = ProgressBarS.OPPool[type]->ioctrl(type,op_cmd,para);
		if(ret>=0 && ProgressBarS.Show)
		{
		//	ret = ProgressBarS.OPPool[type]->show(type);
		}
	}
    
    return ret;
}
#endif

