/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA audio RING BUFFER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : ring_buffer.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is audio  file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  RING_BUFFER_H
     #define  RING_BUFFER_H






/*******************************************************************************
* Function Name  : ringBufferInit
* Description	  : initial ring buffer.ring buffer should be initial when play a music not audio intiail
* Input		  : INT8U *addr : ring buffer addr.ram data if ram src type
				   INT32U len	: ring buffer length,data length if ram src type
				   Media_T *src: media information
* Output		  : none
* Return		  : int 0 : success
				   <0 : fail
*******************************************************************************/
int ringBufferInit(INT8U *addr,INT32U len,Media_T *src);
/*******************************************************************************
* Function Name  : ringBufferRead
* Description	  : read data from ring buffer
* Input		  : INT8U *buffer : destination buffer
				   INT32U len : length of data to be read
* Output		  : none
* Return		  : int 0 : success
				   <0 : fail
*******************************************************************************/
int ringBufferRead(INT8U *buffer,INT32U len);
/*******************************************************************************
* Function Name  : ringBufferService
* Description    : ring buffer service,ring buffer will fill data if need
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferService(void);
/*******************************************************************************
* Function Name  : ringBufferSeek
* Description    : ring buffer seek
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferSeek(int offset);
/*******************************************************************************
* Function Name  : ringBufferPoint
* Description    : ring buffer point of file
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferPoint(void);
/*******************************************************************************
* Function Name  : ringBufferDataSize
* Description    : ring buffer data length
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferDataSize(void);













































#endif



