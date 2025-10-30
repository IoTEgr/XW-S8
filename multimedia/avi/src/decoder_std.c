/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                     MULTIMEDIA AVI
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : decoder_avi.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file,for avi standard type
*               
* History     : 
* 2016-09-22  : mark
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
* 2017-07-19  : mark
*      <1>.Add unkey frame check at encode & decode process
******************************************************************************/
#include "../../media.h"
#include "../../multimedia.h"
#include "../inc/avi.h"

static DECODEER_T *stdDecoder;


static int avi_standardInit(void *handler)
{
	if(handler == NULL)
		return AVI_ERR_GENERAL;
	stdDecoder = (DECODEER_T *)handler;

    stdDecoder->read(stdDecoder->idx1_cache,stdDecoder->idx1_offset,8);  // check idx1			
	if(stdDecoder->idx1_cache[0] != RIFF_idx1)
	{
		return AVI_ERR_IDX1;
	}
	stdDecoder->idx1_length = stdDecoder->idx1_cache[1];  // save idx1 size
	stdDecoder->idx1_offset+=8;
	stdDecoder->idx1_cache_offset = 0;
	stdDecoder->idx1_cache_cnt = 0;
	stdDecoder->frameIdx = 0;
	stdDecoder->videoIdx = 0;
	stdDecoder->framestep = 1; // frame one by one

	aviDecodeIdx1QInit(&stdDecoder->vidsIdx1Q,stdDecoder->vidsIdx1Stack,AVI_VIDS_CACHE_NUM);
	aviDecodeIdx1QInit(&stdDecoder->audsIdx1Q,stdDecoder->audsIdx1Stack,AVI_AUDS_CACHE_NUM);
            
    if(stdDecoder->read(stdDecoder->idx1_cache,stdDecoder->idx1_offset,16)<0)  // read first frame
		return AVI_ERR_READ;
    stdDecoder->idx1_cache_offset = 0;
	stdDecoder->idx1_cache_cnt = 16;

	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeGetOneIdx1
* Description	 : avi decode get one frame idx1 from idx1 cache buffer
* Input 		 : INT32U frameidx : index of frame
                  : AVIINDEXENTRY **aviIdx1 : idx1 information point
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_standardGetOneIdx1(INT32U frameidx,AVIINDEXENTRY **aviIdx1)
{
	int ret;

	if(((frameidx<<4)>stdDecoder->idx1_length) || ((stdDecoder->framestep<0)  && stdDecoder->frameIdx<=0))
		return 1;

	if((frameidx<(stdDecoder->idx1_cache_offset>>4)) || (frameidx>=((stdDecoder->idx1_cache_cnt+stdDecoder->idx1_cache_offset)>>4)))
	{
		ret = stdDecoder->read(stdDecoder->idx1_cache,stdDecoder->idx1_offset+(frameidx<<4),stdDecoder->idx1_cache_length);
        if(ret<0)
			return AVI_ERR_GENERAL;
		stdDecoder->idx1_cache_offset = (frameidx<<4);
		stdDecoder->idx1_cache_cnt = ret;
		
	}

	*aviIdx1 = (AVIINDEXENTRY*)(stdDecoder->idx1_cache+((frameidx-(stdDecoder->idx1_cache_offset>>4))<<2));

	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeService
* Description	 : avi decode cache idx1 table
* Input 		 : none
* Output		 : none 										   
* Return		 : int frame type
*******************************************************************************/
static int avi_standardService(void)
{
	AVIINDEXENTRY *aviIdx1;
	int ret;

	if(stdDecoder->read == NULL) // file read call back
		return AVI_FRAME_ERROR;	

	ret = avi_standardGetOneIdx1(stdDecoder->frameIdx,&aviIdx1);
	if(ret==0)
	{
		if(aviIdx1->ckid == RIFF_00dc)
		{
			if(stdDecoder->vidsIdx1Q.busy < AVI_VIDS_CACHE_NUM)
			{
				aviDecodeIdx1QIn(&stdDecoder->vidsIdx1Q,aviIdx1);
				stdDecoder->frameIdx+=stdDecoder->framestep;
			}
		}
		else if(aviIdx1->ckid == RIFF_01wb)
		{
			if(stdDecoder->audsIdx1Q.busy < AVI_AUDS_CACHE_NUM)
			{
				aviDecodeIdx1QIn(&stdDecoder->audsIdx1Q,aviIdx1);
				stdDecoder->frameIdx+=stdDecoder->framestep;
			}
		}
		else if(aviIdx1->ckid == RIFF_JUNK)
		{
			stdDecoder->frameIdx+=(aviIdx1->dwFlags+8)>>4;
		}
		else
			stdDecoder->frameIdx++;
	}
	return ret;
}
/*******************************************************************************
* Function Name  : aviDecodeOneFrame
* Description	 : avi decode get one frame information
* Input 		 : INT32U *offset : offset of frame data in avi file
                  : INT32U *length : frame data len
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_standardDecodeFrame(INT32U *offset,INT32U *length,INT32U type)
{
	AVIINDEXENTRY aviIdx1;

    if(type == AVI_FRAME_DC_VIDEO)
    {
		if(stdDecoder->vidsIdx1Q.busy>0)
			aviDecodeIdx1QOut(&stdDecoder->vidsIdx1Q,&aviIdx1);
		else
			return AVI_ERR_GENERAL;
		if(stdDecoder->aviflag==0)
		{
			if(((aviIdx1.dwFlags&AVI_KEY_FRAME)==0)&& (!aviIdx1.dwChunkLength))
				stdDecoder->aviflag = 2;
			else
				stdDecoder->aviflag = 1;
		}
		
		if(stdDecoder->aviflag==1)
		{
			if(((aviIdx1.dwFlags&AVI_KEY_FRAME)==0))
			{
				aviIdx1.dwChunkLength = 0;
			}
		}
		else if(stdDecoder->aviflag == 2)
		{
			
		}
			
    }
	else if(type == AVI_FRAME_WD_AUDIO)
	{
		if(stdDecoder->audsIdx1Q.busy>0)
			aviDecodeIdx1QOut(&stdDecoder->audsIdx1Q,&aviIdx1);
		else
			return -2;		
	}
	else 
		return -3;
    if(offset)
		*offset = aviIdx1.dwChunkOffset+stdDecoder->movi_offset;
	if(length)
		*length = aviIdx1.dwChunkLength;

//	stdDecoder->frameIdx++;

	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeFast
* Description	 : avi decode fast forward or fast rewind
* Input 		 : int deta : fast frames
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_standardFast(int deta)
{
	if(deta == 0)
		deta = 1;
	stdDecoder->framestep = deta;
    return stdDecoder->framestep;
/*	stdDecoder->frameIdx +=deta;
	if(stdDecoder->frameIdx<0)
		stdDecoder->frameIdx = 0;*/
	
}

/*******************************************************************************
* Function Name  : aviDecodeGetTime
* Description	 : avi decode time
* Input 		 : INT32U *totaltime : avi total time
                  :INT32U *curtime   : current time
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_standardGetTime(INT32U *totaltime,INT32U *curtime)
{
	u32 sec,ms;
	
	if(totaltime)
	{
		ms = 1000/stdDecoder->frametime;
		sec = stdDecoder->framecnt/ms; // ->second
		ms = (stdDecoder->framecnt%ms)*stdDecoder->frametime;
		*totaltime = sec*1000+ms;
		//*totaltime = (stdDecoder->framecnt*(stdDecoder->frametime)); // ms
	}

	if(curtime)
	{
		ms = stdDecoder->frameIdx-(stdDecoder->frameIdx*75)/100;
		sec = 1000/stdDecoder->frametime;
		*curtime = (ms/sec)*1000+(ms%sec)*stdDecoder->frametime;

		//*curtime = ((stdDecoder->frameIdx*30)/34)*(stdDecoder->frametime); // ms  -> 30frame vds,4->ads
	}

	return 0;
}




AVI_DECODER_OP_T avi_standard=
{
	avi_standardInit,
	avi_standardDecodeFrame,
	avi_standardGetTime,
	avi_standardFast,
	avi_standardService,
};











