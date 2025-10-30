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
* File Name   : avi.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file
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


#define  ARG_CHECK_NULL(arg)    if(arg==NULL)return -1;

typedef struct AVI_DECODER_CACHE_S
{
	INT32S offset;
	INT32S rcnt;
	INT32S ridx;
	INT32S rlen;
	INT32U rbuffer;
}AVI_DECODER_CACHE_T;

extern AVI_DECODER_OP_T avi_standard;
extern AVI_DECODER_OP_T avi_opendml;

static AVI_DECODER_CACHE_T aviCache;
static DECODEER_T aviDecoder;
/*******************************************************************************
* Function Name  : aviDecodeIdx1QInit
* Description	 : avi decode cache initial
* Input 		 : vDECODE_Q_T *idxq : queue name
                    AVIINDEXENTRY *table:table
                    int len : table length
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
void aviDecodeIdx1QInit(DECODE_Q_T *idxq,AVIINDEXENTRY *table,int len)
{
	idxq->in = 0;
	idxq->out=0;
	idxq->cnt = len;
	idxq->busy=0;
	idxq->stack = table;	
}
/*******************************************************************************
* Function Name  : aviDecodeIdx1Cpy
* Description	 : avi decode idx1 copy
* Input 		 : AVIINDEXENTRY *dst : 
                    AVIINDEXENTRY*src
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
void aviDecodeIdx1Cpy(AVIINDEXENTRY *dst,AVIINDEXENTRY*src)
{
	dst->ckid = src->ckid;
	dst->dwChunkLength=src->dwChunkLength;
	dst->dwChunkOffset=src->dwChunkOffset;
	dst->dwFlags = src->dwFlags;
	//ax32xx_sysDcacheFlush((u32)dst,16);
}
/*******************************************************************************
* Function Name  : aviDecodeIdx1QIn
* Description	 : avi decode idx queue int
* Input 		 : DECODE_Q_T *idxq : queue name
                    AVIINDEXENTRY *idx1 : idx1
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
void aviDecodeIdx1QIn(DECODE_Q_T *idxq,AVIINDEXENTRY *idx1)
{
	if(idxq->busy<idxq->cnt)
	{
		aviDecodeIdx1Cpy(idxq->stack+idxq->in,idx1);
		idxq->in++;
		if(idxq->in>=idxq->cnt)
			idxq->in = 0;
		idxq->busy++;
	}
}
/*******************************************************************************
* Function Name  : aviDecodeCacheInit
* Description	 : avi decode cache initial
* Input 		 : void *buffer : buffer
                    INT32U len : buffer length
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/

void aviDecodeIdx1QOut(DECODE_Q_T *idxq,AVIINDEXENTRY *idx1)
{
	if(idxq->busy>0)
	{
		aviDecodeIdx1Cpy(idx1,idxq->stack+idxq->out);
		idxq->out++;
		if(idxq->out>=idxq->cnt)
			idxq->out= 0;
		idxq->busy--;		
	}
}
/*******************************************************************************
* Function Name  : aviDecodeCacheInit
* Description	 : avi decode cache initial
* Input 		 : void *buffer : buffer
                    INT32U len : buffer length
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
INT32S aviDecodeCacheInit(void *buffer,INT32U len)
{
	aviCache.offset = 0;
	aviCache.rbuffer = (INT32U)buffer;
	aviCache.rcnt = 0;
	aviCache.ridx = 0;
	aviCache.rlen = len;

	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeCacheRead
* Description	 : avi decode cache read
* Input 		 : INT32S offset : offset
                    INT32S len : read length
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
INT8U *aviDecodeCacheRead(INT32S offset,INT32S len)
{
	INT8U *src,*tar;
	int i;
	
	aviCache.rcnt-=offset;
	aviCache.offset+=offset;
	aviCache.ridx+=offset;
	if(aviCache.rcnt<len)
	{
		if(aviCache.rcnt<0)
			aviCache.rcnt = 0;
		src = (INT8U *)aviCache.rbuffer;
		tar = (INT8U *)(aviCache.rbuffer+aviCache.ridx);
		for(i=0;i<aviCache.rcnt;i++)
		{
			*src++ = *tar++;
		}
		i = aviDecoder.read((void *)aviCache.rbuffer,aviCache.offset,aviCache.rlen-aviCache.rcnt);
        if(i<0)
			return  NULL;
		aviCache.rcnt+=i;
		aviCache.ridx = 0;
	}
    if(aviCache.rcnt<len)
		return NULL;
	src = (INT8U *)(aviCache.rbuffer+aviCache.ridx);
	aviCache.rcnt-=len;
	aviCache.offset+=len;
	aviCache.ridx+=len;

	return src;
}
/*******************************************************************************
* Function Name  : aviDecodeInit
* Description	 : avi decode initial
* Input 		 : void *cachebuffer : cache for idx1
                  : INT32U len : cache length
                   :int (*cacheread)(INT32U offset,INT32U lenght) : read call back
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeInit(void *cachebuffer,INT32U len,int (*cacheread)(void *cachebuffer,INT32U offset,INT32U lenght))
{
	aviDecoder.idx1_cache_length = len&(~0xf); // 16byte algin
	aviDecoder.idx1_cache = (INT32U *)cachebuffer;
	aviDecoder.read = cacheread;

	if(len<16 || cacheread == NULL)
		return -1;
		
	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeParse
* Description	 : avi decode parse
* Input 		 : INT8U *buffer : avi file for decode parse
                  : INT32U len : buffer size;
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeParse(int fhandle)
{
	INT32U tempvalue,*temp;

	AVIMAINHEADER * p_avimainheader;
	AVISTREAMHEADER * p_avistreamheader;
	WAVEFORMATEX * p_waveformat_tag;
	AVI_IX_HEADER_T *p_avisuperindex;
	
    aviDecoder.status = AVI_ERR_GENERAL;
	aviDecoder.type = AVI_TYPE_STANDARD;  // default avi
	aviDecoder.fhandle = fhandle;
	if(aviDecoder.read == NULL) // file read call back
		return AVI_ERR_READ;
	aviDecodeCacheInit(aviDecoder.idx1_cache,aviDecoder.idx1_cache_length);

    tempvalue = 0;
	temp = (INT32U *)aviDecodeCacheRead(0,4);
	if(*temp != RIFF_TAG)
		return AVI_ERR_HDRL;
	while(1)
	{
		if(tempvalue == RIFF_hdrl) // header 
		{
			p_avimainheader = (AVIMAINHEADER *)aviDecodeCacheRead(0,sizeof(AVIMAINHEADER));
			if(!(p_avimainheader->dwFlags&0x00000010) || (!p_avimainheader->dwTotalFrames))
			{
				aviDecoder.status = AVI_ERR_HDRL;
				break;
			}
			aviDecoder.width = (WORD)p_avimainheader->dwWidth;
			aviDecoder.height= (WORD)p_avimainheader->dwHeight;
			aviDecoder.framecnt=p_avimainheader->dwTotalFrames;
			aviDecoder.frametime = p_avimainheader->dwMicroSecPerFrame/1000; // change to ms
			tempvalue = 0;
		}
		else if(tempvalue == RIFF_strl) // string
		{
			p_avistreamheader = (AVISTREAMHEADER *)aviDecodeCacheRead(0,sizeof(AVISTREAMHEADER));
			if(p_avistreamheader->fccType == RIFF_vids)
			{
				if(p_avistreamheader->fccHandler != RIFF_MJPG) // no support vids decoder
				{
					aviDecoder.status = AVI_ERR_VIDS;
				    break;  
				}
				tempvalue = 0;//+= tempvalue[-1] >> 2;
			}
			else if(p_avistreamheader->fccType == RIFF_auds)
			{
				p_waveformat_tag = (WAVEFORMATEX *) (((INT8U *)p_avistreamheader)+72);//aviDecodeCacheRead((1 + ((p_avistreamheader->cb + 8) >> 2) + 2)<<2,sizeof(AVISTREAMHEADER));
                if(p_waveformat_tag->wFormatTag != 1)// no support auds decoder
				{
					aviDecoder.status = AVI_ERR_AUDS;
				    break;  
				}
				aviDecoder.auds_samplerate = p_waveformat_tag->nSamplesPerSec;
				tempvalue = 0;//+= tempvalue[-1] >> 2;
			}			
		}
		else if(tempvalue == RIFF_indx)
		{
			aviDecoder.type = AVI_TYPE_OPENDML;
			p_avisuperindex = (AVI_IX_HEADER_T *)aviDecodeCacheRead(-4,AVI_IX_HEADER_LEN);
			if(p_avisuperindex->wLongsPerEntry!=4)
			{
				aviDecoder.status = AVI_ERR_INDX;
				break; 
			}
			if(p_avisuperindex->dwChunkId == RIFF_00dc)
			{
				aviDecoder.vids_entrys = p_avisuperindex->nEntriesInUse;
				aviDecoder.vids_offset = aviCache.offset;
			}
			else if(p_avisuperindex->dwChunkId == RIFF_01wb)
			{
				aviDecoder.auds_entrys = p_avisuperindex->nEntriesInUse;
				aviDecoder.auds_offset = aviCache.offset;
			}
			aviDecodeCacheRead(p_avisuperindex->cb-32+8,0);
			tempvalue = 0;
		}				
		else if(tempvalue == RIFF_movi) // movi
		{
			aviDecoder.movi_offset = aviCache.offset-4;//(INT32U)&tempvalue[0]-(INT32U)buffer; // movi offset
			temp = (INT32U *)aviDecodeCacheRead(-8,4);
			if(temp==NULL)
				break;
			tempvalue = *temp;
            aviDecoder.movi_size = tempvalue;
			if(aviDecoder.type == AVI_TYPE_STANDARD)
            {				
				aviDecoder.idx1_offset = tempvalue+aviDecoder.movi_offset; // idx1 offset in avi file
            }
           
			aviDecoder.aviflag = 0;
		    aviDecoder.status = 0;  // parse ok
			break;
		}
		else if((tempvalue == RIFF_JUNK)||(tempvalue == RIFF_strf)||(tempvalue == RIFF_strd)) 
		{
			temp = (INT32U *)aviDecodeCacheRead(0,4);
			if(temp==NULL)
				break;
			tempvalue = *temp;
			aviDecodeCacheRead(tempvalue,0);
		}
		else
		{
			if((tempvalue==RIFF_AVI)||(tempvalue==RIFF_LIST)||(tempvalue == RIFF_JUNK))
			    aviDecodeCacheRead(4,0); // skip size

			temp = (INT32U *)aviDecodeCacheRead(0,4);
			if(temp==NULL)
				break;
			tempvalue = *temp;
		}
	}
    if(aviDecoder.status<0)
		return aviDecoder.status;  // return avi status
	
	if(aviDecoder.type == AVI_TYPE_STANDARD)
	{
		aviDecoder.decoder = &avi_standard;  // load decoder
	//	printf("decoer : avi standar\n");
	}
	else if(aviDecoder.type == AVI_TYPE_OPENDML)
	{
		aviDecoder.decoder = &avi_opendml;
	//	printf("decoer : avi open DML\n");
	}
    else
		aviDecoder.decoder = NULL;

	if(aviDecoder.decoder==NULL)
		return AVI_ERR_DECODER;
	
 
    if(aviDecoder.decoder->init)
    {
		if(aviDecoder.decoder->init((void *)&aviDecoder)<0)  // decoder initial
			return AVI_ERR_INIT;
    }
	aviDecoder.status = aviDecoder.type;
	if(aviDecodeService() < 0) // run decoder once
    {
        return AVI_ERR_IDX1;
    }
	
	return aviDecoder.status; // parse avi header fail
}
/*******************************************************************************
* Function Name  : aviDecodeService
* Description	 : avi decode cache idx1 table
* Input 		 : none
* Output		 : none 										   
* Return		 : int frame type
*******************************************************************************/
int aviDecodeService(void)
{
	ARG_CHECK_NULL(aviDecoder.decoder);

	return aviDecoder.decoder->service();
}
/*******************************************************************************
* Function Name  : aviDecodeOneFrame
* Description	 : avi decode get one frame information
* Input 		 : INT32U *offset : offset of frame data in avi file
                  : INT32U *length : frame data len
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeOneFrame(INT32U *offset,INT32U *length,INT32U type)
{
	ARG_CHECK_NULL(aviDecoder.decoder);

	return aviDecoder.decoder->decode(offset,length,type);
}
/*******************************************************************************
* Function Name  : aviDecodeFast
* Description	 : avi decode fast forward or fast rewind
* Input 		 : int deta : fast frames
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
void aviDecodeFast(int deta)
{
//	ARG_CHECK_NULL(aviDecoder.decoder);
	if(aviDecoder.decoder==NULL)return;
	
	aviDecoder.decoder->dfast(deta);	
}

/*******************************************************************************
* Function Name  : aviDecodeGetTime
* Description	 : avi decode time
* Input 		 : INT32U *totaltime : avi total time
                  :INT32U *curtime   : current time
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeGetTime(INT32U *totaltime,INT32U *curtime)
{
	ARG_CHECK_NULL(aviDecoder.decoder);
	
	return aviDecoder.decoder->gettime(totaltime,curtime);
}
/*******************************************************************************
* Function Name  : aviDecodeGetAudioSampleRate
* Description	 : avi decode aud string sample rate
* Input 		 : none
* Output		 : none 										   
* Return		 : int : samplerate
*******************************************************************************/
int aviDecodeGetAudioSampleRate(void)
{
	return aviDecoder.auds_samplerate;
}
/*******************************************************************************
* Function Name  : aviDecodeGetVideoFrameTime
* Description	 : avi decode video fram time
* Input 		 : none
* Output		 : none 										   
* Return		 : int : video fram time
*******************************************************************************/
int aviDecodeGetVideoFrameTime(void)
{
	return (aviDecoder.frametime);
}

/*******************************************************************************
* Function Name  : aviDecodeGetTime
* Description	 : avi decode vids resolution
* Input 		 :INT16U *width : width
                  :INT16U *height: height
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeGetResolution(INT16U *width,INT16U *height)
{
	if(width)
		*width = aviDecoder.width;
	if(height)
		*height=aviDecoder.height;

	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeGetFrameIndex
* Description	 : avi decode frame index
* Input 		 :none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviDecodeGetFrameIndex(void)
{
	return aviDecoder.frameIdx;
}










