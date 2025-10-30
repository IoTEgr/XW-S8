/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA AUDIO RING BUFFER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : ring_buffer.c
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
#include "../media.h"
#include "../multimedia.h"

#if (MEDIA_CFG_AUDIO_PLAYBK_EN || MEDIA_CFG_AUDIO_RECORD_EN)

typedef struct Audio_Ring_S
{
	INT32U ringLen;
	INT32U dataLen;

	INT32U readIndex;
	INT32U writeIndex;

    INT8U  ringFlag;   // 2-normal,1-file end,0-ringbuffer end
    INT8U  rev1;
	INT16U rev2;
    
	INT32U filePoint;
    INT32U dataAddr;  // nv fs data addr
	INT32U dataSize;  // nv fs data size
	
	INT8U *ringBuffer;

	Media_T *media;
}Audio_Ring_T;

static Audio_Ring_T ringBuffer;


/*******************************************************************************
* Function Name  : ringBufferInit
* Description    : initial ring buffer.ring buffer should be initial when play a music not audio intiail
* Input          : INT8U *addr : ring buffer addr.ram data if ram src type
                      INT32U len   : ring buffer length,data length if ram src type
                      Media_T *src: media information
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferInit(INT8U *addr,INT32U len,Media_T *src)
{
	if(addr == NULL || src == NULL)
		return -1;
	ringBuffer.readIndex = 0;
	ringBuffer.ringBuffer= addr;
	ringBuffer.ringFlag  =2;  // normal read
	ringBuffer.ringLen   = len;

	ringBuffer.writeIndex = 0;
	ringBuffer.filePoint = 0;
	ringBuffer.dataLen = 0;

	ringBuffer.media = src;
	if(src->type == MEDIA_SRC_RAM)
	{
		ringBuffer.dataLen = len;
		ringBuffer.ringFlag  = 1; // file end
	}

	if(src->type == MEDIA_SRC_NVFS)
	{
		ringBuffer.dataAddr = nv_open((int)ringBuffer.media->src.fd);
		ringBuffer.dataSize = nv_size((int)ringBuffer.media->src.fd);

	}

    return ringBufferService();
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : ringBufferRead
* Description    : read data from ring buffer
* Input          : INT8U *buffer : destination buffer
                      INT32U len : length of data to be read
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferRead(INT8U *buffer,INT32U len)
{
#if 0
    int count;

	len = 4096;
	if(ringBuffer.media->type == MEDIA_SRC_NVFS)
	{
		count = ringBuffer.dataSize - ringBuffer.filePoint;
		if(count>len)
			count = len;
		else
			ringBuffer.ringFlag = 1; // file end

		nv_read(ringBuffer.dataAddr+ringBuffer.filePoint,buffer,count);

		ringBuffer.filePoint+=count;
       
	}
	len = count;
#else 
	int i;
	

	if(len>=ringBuffer.dataLen)
		len = ringBuffer.dataLen; 
	
//	deg_Printf("[%x]-[%x:",len,ringBuffer.readIndex);
	for(i=0;i<len;i++)
	{		
		buffer[i] = ringBuffer.ringBuffer[ringBuffer.readIndex++];

		if(ringBuffer.readIndex>=ringBuffer.ringLen)
		    ringBuffer.readIndex = 0;
	}
//    deg_Printf("%x]\n",ringBuffer.readIndex);
	ringBuffer.dataLen-=len;
#endif

    if(len == 0)
		return -1;
	return len;
	
}
/*******************************************************************************
* Function Name  : ringBufferService
* Description    : ring buffer service,ring buffer will fill data if need
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferService(void)
{
	int count;
	
	if(ringBuffer.ringFlag !=2)  // not data need read
		return STATUS_OK;

	if(ringBuffer.dataLen > (ringBuffer.ringLen>>1))  // ring buffer data engh
		return STATUS_OK;

	if(ringBuffer.media->type == MEDIA_SRC_FS)
	{
		count = read(ringBuffer.media->src.fd,ringBuffer.ringBuffer+ringBuffer.writeIndex,ringBuffer.ringLen>>1);
		if(count>0)
		{
			ringBuffer.dataLen+=count;
			if(ringBuffer.writeIndex!=0)
				ringBuffer.writeIndex = 0;
			else
				ringBuffer.writeIndex = ringBuffer.ringLen>>1;
			ringBuffer.filePoint+=count;
			if(count<(ringBuffer.ringLen>>1))
				ringBuffer.ringFlag = 1; // file end

			return STATUS_OK;
		}
	}
	else if(ringBuffer.media->type == MEDIA_SRC_NVFS)
	{
		count = ringBuffer.dataSize - ringBuffer.filePoint;
		if(count>(ringBuffer.ringLen>>1))
			count = ringBuffer.ringLen>>1;
		else
			ringBuffer.ringFlag = 1; // file end

		nv_read(ringBuffer.dataAddr+ringBuffer.filePoint,ringBuffer.ringBuffer+ringBuffer.writeIndex,count);
//		deg_Printf("[w-%x]",ringBuffer.writeIndex);
//        deg_Printf("{%x}<%x>-<%x:",ringBuffer.filePoint,count,ringBuffer.writeIndex);
		ringBuffer.dataLen+=count;
		if(ringBuffer.writeIndex!=0)
			ringBuffer.writeIndex = 0;
		else
			ringBuffer.writeIndex = ringBuffer.ringLen>>1;
//		deg_Printf("%x>",ringBuffer.writeIndex);
		ringBuffer.filePoint+=count;
       
		return STATUS_OK;
	}
    
	return STATUS_FAIL;
}
/*******************************************************************************
* Function Name  : ringBufferSeek
* Description    : ring buffer seek
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferSeek(int offset)
{
	int cnt;

	if(ringBuffer.filePoint>0)
		cnt = ringBuffer.filePoint - ringBuffer.readIndex;
	else
		return -1;
	if(offset<cnt && offset>ringBuffer.filePoint)
	{
		ringBuffer.readIndex = 0;
		ringBuffer.writeIndex=0;
		ringBuffer.dataLen    = 0;
		ringBuffer.filePoint = offset;
		if(ringBuffer.media->type == MEDIA_SRC_FS)
		{
			lseek(ringBuffer.media->src.fd,offset,0);
		}
	}
    else 
    {
		cnt = offset - cnt;
		ringBuffer.dataLen-=cnt;
		ringBuffer.readIndex+=cnt;
		if(ringBuffer.readIndex>=ringBuffer.ringLen)
			ringBuffer.readIndex -= ringBuffer.ringLen;
    }

	return ringBufferService();
}
/*******************************************************************************
* Function Name  : ringBufferPoint
* Description    : ring buffer point of file
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferPoint(void)
{
	if(ringBuffer.filePoint>ringBuffer.ringLen)
		return (ringBuffer.filePoint-ringBuffer.dataLen);
	else
		return ringBuffer.readIndex;	
}
/*******************************************************************************
* Function Name  : ringBufferDataSize
* Description    : ring buffer data length
* Input          : 
* Output         : none
* Return         : int 0 : success
                      <0 : fail
*******************************************************************************/
int ringBufferDataSize(void)
{

	return ringBuffer.dataLen;
}









#endif







