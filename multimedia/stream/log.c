/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA system log
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : log.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is stream for log
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"

#if LOG_CFG_EN
#define  LOG_NAME    "LOG.TXT"


static FIL fileLog;
static LOG_T logCtrl[LOG_CFG_LEN];
static INT32U logTime,logFlag=0;


SECTION(".big_buffer") ALIGNED(32) char logCache[LOG_CFG_LEN][LOG_CFG_LINE];

#endif

/*******************************************************************************
* Function Name  : logInit
* Description    : log  initial
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logInit(void)
{
#if LOG_CFG_EN	
	int i;

	for(i=0;i<LOG_CFG_LEN;i++)
	{
		logCtrl[i].cache = logCache[i];
		logCtrl[i].index = 0;
		logCtrl[i].state = 0;
	}

	logCtrl[0].rev = 0;  // file flag
	logCtrl[1].rev = 0;  // log cache flag

	logTime = XOSTimeGet();
	logFlag = 1;
#endif	
}
/*******************************************************************************
* Function Name  : logWrite
* Description    : log  ,write to disk
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logWrite(LOG_T *log)
{
#if LOG_CFG_EN	
	UINT ret;
	
	if(logCtrl[0].rev==0)
	{
		if(f_open(&fileLog,(const char *)LOG_NAME,FA_WRITE|FA_CREATE_NEW)!=FR_OK)
			return ;
		hal_uartPrintString("log create\n");
		logCtrl[0].rev = 1;
		_f_lseek(&fileLog,fileLog.fsize);
	}

	f_write(&fileLog,log->cache,log->index,&ret);
	log->index = 0;
	log->state = 0;

//	_f_close(&fileLog);
//	logCtrl[0].rev = 0;
#endif
}
/*******************************************************************************
* Function Name  : logFlush
* Description    : log stop ,flush to disk
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logFlush(void)
{
#if LOG_CFG_EN		
	int i,flag=0;
    if(logFlag==0)
		return ;
	for(i=0;i<LOG_CFG_LEN;i++)
	{
		if(logCtrl[i].state)
		{
			logWrite(&logCtrl[i]);
			hal_uartPrintString("log flush 0\n");
		}
		else if(logCtrl[i].index)
			flag=i+1;
		//if(logCtrl[i].index>=(LOG_CFG_LINE-32))
		//	flag|=4;
	}

	if(flag)
	{
	    flag-=1;
		if((logTime+LOG_CFG_TIMEOUT)>=XOSTimeGet())
		{
			logWrite(&logCtrl[flag]);
			hal_uartPrintString("log flush 1\n");
			logTime = XOSTimeGet();
			logStop();
		}		
	}
#endif	
}
/*******************************************************************************
* Function Name  : logChar
* Description    : log stop ,print to log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logChar(char c)
{
#if LOG_CFG_EN		
	if(logFlag==0)
		return ;
	logTime = XOSTimeGet();
	
	logCtrl[logCtrl[1].rev].cache[logCtrl[logCtrl[1].rev].index++] = c;

	if(logCtrl[logCtrl[1].rev].index>=LOG_CFG_LINE)
	{
		logCtrl[logCtrl[1].rev].state = 1;
		logCtrl[1].rev++;
		if(logCtrl[1].rev>=LOG_CFG_LEN)
			logCtrl[1].rev=0;
		if(logCtrl[logCtrl[1].rev].state)
			logFlush();
		logCtrl[logCtrl[1].rev].index = 0;
	}
#endif	
}

/*******************************************************************************
* Function Name  : logPrint
* Description    : log stop ,print to log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logPrint(char *string)
{
#if LOG_CFG_EN	   

    if(string == NULL)
		return ;
    while(*string)
    {
		logChar(*string);
		string++;
    }

#endif
}

/*******************************************************************************
* Function Name  : logStart
* Description    : log stop ,create log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logStart(void)
{
#if LOG_CFG_EN	

	if(logCtrl[0].rev==0)
	{
		if(f_open(&fileLog,(const char *)LOG_NAME,FA_WRITE|FA_CREATE_NEW)==FR_OK)
		{
			logCtrl[0].rev = 1;
			_f_lseek(&fileLog,fileLog.fsize);
			logFlush();
			//logPrint("log>>--------------------------------------------------");
			hal_uartPrintString("log create\n");
		}
	}
#endif	
}
/*******************************************************************************
* Function Name  : logStop
* Description    : log stop ,close log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logStop(void)
{
#if LOG_CFG_EN	

	if(logCtrl[0].rev)
	{
		_f_close(&fileLog);
		logCtrl[0].rev = 0;
		hal_uartPrintString("log destory\n");
	}
#endif	
}

















