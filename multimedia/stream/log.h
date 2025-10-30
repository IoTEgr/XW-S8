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
* File Name   : log.h
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
#ifndef  LOG_H
     #define  LOG_H

#define  LOG_CFG_EN            0   // enable log 

#define  LOG_CFG_LEN           2   // 2-cache line
#define  LOG_CFG_LINE         256 // line-length

#define  LOG_CFG_TIMEOUT     (60*1000) // 10-min


typedef struct LOG_S
{
	INT8U state;
    INT8U rev;
	INT16U index;

	char *cache;
}LOG_T;



/*******************************************************************************
* Function Name  : logInit
* Description    : log  initial
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logInit(void);
/*******************************************************************************
* Function Name  : logFlush
* Description    : log stop ,flush to disk
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logFlush(void);
/*******************************************************************************
* Function Name  : logChar
* Description    : log stop ,print to log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logChar(char c);
/*******************************************************************************
* Function Name  : logPrint
* Description    : log stop ,print to log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logPrint(char *string);
/*******************************************************************************
* Function Name  : logStart
* Description    : log stop ,create log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logStart(void);
/*******************************************************************************
* Function Name  : logStop
* Description    : log stop ,close log file
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void logStop(void);




















#endif

