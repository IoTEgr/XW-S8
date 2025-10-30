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
* File Name   : encoder_std.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file,for avi standard file
*               
* History     : 
* 2016-09-22  : mark
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../../media.h"
#include "../../multimedia.h"
#include "../inc/avi.h"

#define  AVI_STD_HEADER_SIZE            8192
#define  STD_CHECK(x)    if((x==NULL))return AVI_ERR_GENERAL


//static void *encoder;

static int avi_standardInit(void *encoder)
{
	ENCODER_T *stdEncoder = (ENCODER_T *)encoder;
	
	if(stdEncoder==NULL)
		return AVI_ERR_GENERAL;


	return AVI_ERR_NONE;
}

static int avi_standardUninit(void *encoder)
{
	ENCODER_T *stdEncoder = (ENCODER_T *)encoder;
	
	if(stdEncoder==NULL)
		return AVI_ERR_GENERAL;


	return AVI_ERR_NONE;
}
/*******************************************************************************
* Function Name  : aviEncodeParse
* Description	 : avi encode parse
* Input 		 : INT8U *buffer : buffer for cache avi header information
                  : INT16U width : avi mjpeg width
                  : INT16U height: avi mjpeg height
                  : INT16U fps    : avi video frame rate
                  : INT16U audio : record audio or not
* Output		 : none 										   
* Return		 : int 0
*******************************************************************************/
static int avi_standardParse(void *encoder,INT16U width,INT16U height,INT16U fps,INT16U audio,INT16U samperate)
{
	AVIFILEHEADER *aviheader;

    ENCODER_T *stdEncoder = (ENCODER_T *)encoder;
    STD_CHECK(stdEncoder);
	
	if(stdEncoder->write == NULL)
		return AVI_ERR_WRITE;
	
	aviheader = (AVIFILEHEADER *)hal_sysMemMalloc(AVI_STD_HEADER_SIZE,64);
	
	memset((void *)aviheader,0,AVI_STD_HEADER_SIZE);

	//fill fcc and fcc type
	aviheader->riff.fcc = RIFF_TAG;		//"RIFF"
	aviheader->riff.fcctype = RIFF_AVI;	//"AVI "
	aviheader->hdrl.fcc = RIFF_LIST;		//"LIST"
	aviheader->hdrl.cb = AVI_STD_HEADER_SIZE - 12 - 8-12;
	aviheader->hdrl.fcctype = RIFF_hdrl;	//"hdrl"

	//fill avimainheader
	aviheader->avih.fcc = RIFF_avih;	//"avih"
	aviheader->avih.cb = sizeof(AVIMAINHEADER)-8;						//avi header size
	aviheader->avih.dwMicroSecPerFrame = 1000000/fps;			// 30fps ,so (1s/30)*1000*1000 = 33333us
	aviheader->avih.dwFlags = AVIF_HASINDEX;							//has index

	if(audio)
		aviheader->avih.dwStreams = 0x02;//aviattr.pcm;//0x2;					// jpeg and pcm 
	else
		aviheader->avih.dwStreams = 0x1;					// jpeg 

	aviheader->avih.dwWidth = width; //640
	aviheader->avih.dwHeight = height;//480

	//【strl：视频】
	//file move fcc and fcc type
	aviheader->strl_v.fcc = RIFF_LIST;			//"LIST"
	aviheader->strl_v.cb = sizeof(aviheader->strl_v.fcctype) + sizeof(AVISTREAMHEADER)+sizeof(CHUNK)+sizeof(BITMAPINFOHEADER) + sizeof(CHUNK) + sizeof(BITMAPAUDIODC);
	aviheader->strl_v.fcctype = RIFF_strl;		//"strl"
	aviheader->strh_v.fcc = RIFF_strh;						//"strh"
	aviheader->strh_v.cb = sizeof(AVISTREAMHEADER)-8;		
	aviheader->strh_v.fccType = RIFF_vids;					//"vids"
	aviheader->strh_v.fccHandler = RIFF_MJPG;				//"MJPG"
	aviheader->strh_v.dwScale = 1;
	aviheader->strh_v.dwRate = fps;							
	aviheader->strh_v.dwStart = 0;

	aviheader->strh_v.rcFrame.right = width; 
	aviheader->strh_v.rcFrame.bottom = height;//480

	//【"strf：视频"】	
	//fill vids stream info
	aviheader->strf_v.dwFourCC = RIFF_strf;							//"strf"
	aviheader->strf_v.dwSize = sizeof(BITMAPINFOHEADER);				
	aviheader->bitmapinfo.biSize = sizeof(BITMAPINFOHEADER);			
	aviheader->bitmapinfo.biWidth = width; //640
	aviheader->bitmapinfo.biHeight =  height;//480
	aviheader->bitmapinfo.biPlanes = 1;							// always 1
	aviheader->bitmapinfo.biBitCount = 24;							// 1, 4, 8, 16, 24, 32
	aviheader->bitmapinfo.biCompression = RIFF_MJPG;				//"MJPG"
	
	//strd
	aviheader->strd_v.dwFourCC = RIFF_strd;
	aviheader->strd_v.dwSize = sizeof(BITMAPAUDIODC);
	
	//fill strl auds
	aviheader->strl_a.fcc = RIFF_LIST;			//"LIST"
	aviheader->strl_a.cb = sizeof(aviheader->strl_a.fcctype)+sizeof(AVISTREAMHEADER)+sizeof(CHUNK)+sizeof(WAVEFORMATEX) + sizeof(CHUNK) + sizeof(WAVEDECPR);
	aviheader->strl_a.fcctype = RIFF_strl;		//"strl"
	
	//【strl：音频】
	//fill auds stream
	aviheader->strh_a.fcc = RIFF_strh;						//"strh"
	aviheader->strh_a.cb = sizeof(AVISTREAMHEADER)-8;		
	aviheader->strh_a.fccType = RIFF_auds;					//"auds" 
	aviheader->strh_a.dwScale = 2;//4
	aviheader->strh_a.dwRate = samperate<< 1;//【64000采样率】
	aviheader->strh_a.dwLength = 1024;//2048;
	aviheader->strh_a.dwSampleSize = 2;//4

	//【strf：音频】
	//fill auds stream info
	aviheader->strf_a.dwFourCC = RIFF_strf;							//"strf"
	aviheader->strf_a.dwSize = sizeof(WAVEFORMATEX);
//	deg_Printf("sizeof(WAVEFORMATEX) = 0x%x \n",sizeof(WAVEFORMATEX));
	aviheader->wavinfo.wFormatTag = 1;			//1:pcm    2:adpcm
	aviheader->wavinfo.nChannels = 1;//2
	aviheader->wavinfo.nSamplesPerSec = samperate;	//11025 , 22050, 44100
	aviheader->wavinfo.nAvgBytesPerSec = samperate<< 1;//64000
	aviheader->wavinfo.nBlockAlign = 2;	// 4ch*bitspersample /8
	aviheader->wavinfo.wBitsPerSample = 16;
	
	//strd
	aviheader->strd_a.dwFourCC = RIFF_strd;
	aviheader->strd_a.dwSize = sizeof(WAVEDECPR);
	
	//fill junk
	aviheader->junk.dwFourCC= RIFF_JUNK;			//"JUNK"
	aviheader->junk.dwSize= AVI_STD_HEADER_SIZE - (sizeof(AVIFILEHEADER) + 12);

	if(stdEncoder->write(stdEncoder->fhandle,aviheader,AVI_STD_HEADER_SIZE)<0)
	{
		hal_sysMemFree((void *)aviheader);
		return AVI_ERR_WRITE;
	}
    hal_sysMemFree((void *)aviheader);
	stdEncoder->dwIdexlen = 0;
	stdEncoder->framecnt = 0;
	stdEncoder->ofset = 4;
	stdEncoder->size = 0;

	stdEncoder->oldOffset=0;
	stdEncoder->oldSize = 0;

	return AVI_STD_HEADER_SIZE;
}
/*******************************************************************************
* Function Name  : aviEncodeAddIndex
* Description	 : avi encode add index for idx1
* Input 		 : INT8U *buffer : buffer for cache avi idx1
                  : INT8U type : index type:AVI_FRAME_DC_VIDEO/AVI_FRAME_DB_VIDEO/AVI_FRAME_WD_AUDIO
                  :INT8U keyFrame : 1->key frame,0->not key frame
                  : INT32U size: data size,if size == 0,insert a repeat video frame,only for video frame
                  : INT32U buflen : buffer len
* Output		 : none 										   
* Return		 : int 0
                              <0 fail
*******************************************************************************/
int avi_standardAddIndex(void *encoder,INT8U *buff,INT8U type,INT8U keyFrame,INT32U size,INT32U buflen)
{
//	static INT32U olddc_offset=0,olddc_size=0;
	IDX_H *idx1;
    IDX_A *idx;
	
	ENCODER_T *stdEncoder = (ENCODER_T *)encoder;
    STD_CHECK(stdEncoder);
	
    if(stdEncoder->dwIdexlen == 0)
    {
		idx1 = (IDX_H *)buff;

		idx1->rsv0 = 0;
		idx1->rsv1 = 0;
		idx1->fcc = RIFF_idx1;
		idx1->cb  = 0;
		stdEncoder->dwIdexlen+=16;
    }
	
	idx = (IDX_A *)&buff[stdEncoder->dwIdexlen % (buflen)];		

	if(type == AVI_FRAME_DC_VIDEO || type == AVI_FRAME_DB_VIDEO)
	{		
        if(type == AVI_FRAME_DC_VIDEO)
			idx->dwchunkid = RIFF_00dc;
		else
			idx->dwchunkid = RIFF_00DB;

		if((stdEncoder->dwIdexlen+16) & 0x1ff)
			idx->dwflags = 0x00000010;
		else
			idx->dwflags = 0x01000010;
		if(keyFrame==0)
			idx->dwflags &=~0x00000010; 
        if(size!=0) // insert a repeat video frame
        {
			stdEncoder->oldOffset= stdEncoder->ofset;
			stdEncoder->oldSize= size;
        }
		stdEncoder->framecnt++;
	}
	else if(type == AVI_FRAME_WD_AUDIO)
	{
		idx->dwchunkid = RIFF_01wb;
		if((stdEncoder->dwIdexlen+16) & 0x1ff)
			idx->dwflags = 0;
		else
			idx->dwflags = 1;		
	}
	else if(type == AVI_FRAME_MM_NULL)
	{
		idx->dwchunkid = RIFF_JUNK;
		idx->dwflags = buflen-8;
		idx->dwoffset = stdEncoder->ofset;
	    idx->dwsize = size;
		size = 0;
		goto AVI_ADD_INDEX;
	}
	else 
		return -2;
    
	if(size == 0)
	{
		if(stdEncoder->oldSize== 0)
			return -3;
		idx->dwoffset = stdEncoder->oldOffset;
	    idx->dwsize = 0;//stdEncoder->oldSize- 8;	// set as 0,using win7
	}
    else
    {
		idx->dwoffset = stdEncoder->ofset;
		idx->dwsize = size - 8;	
    }

	stdEncoder->ofset+=size;
AVI_ADD_INDEX:		
    stdEncoder->dwIdexlen+=16;	
    if((stdEncoder->dwIdexlen%buflen) == 0)
		return 1;
	else
		return 0;

}
/*******************************************************************************
* Function Name  : aviEncodeEnd
* Description	 : avi encode end
* Input 		 : INT8U *buffer : buffer for cache avi header,user should read back the avi header from avi file
                  : INT32U fsize   : file size
                  : INT32U idx_offset : idx1 offset from file start
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int avi_standardEnd(void *encoder,INT8U *buffer,INT32U fsize,INT32U idx_offset)
{
	INT32U *ptemp;
	AVIFILEHEADER *aviheader;
	
	ENCODER_T *stdEncoder = (ENCODER_T *)encoder;
	STD_CHECK(stdEncoder);
	
	aviheader = (AVIFILEHEADER *)buffer;

	aviheader->riff.cb = fsize-8;//g_stcJpegInfo.dwIndexLen + g_stcJpegInfo.dwChunkLen - 8; 	
	aviheader->avih.dwTotalFrames = stdEncoder->framecnt; //
	aviheader->strh_v.dwLength = stdEncoder->framecnt;//
	ptemp = (INT32U*)((INT32U)aviheader+AVI_STD_HEADER_SIZE-12);			//data block head

	ptemp[0] = RIFF_LIST;										//"LIST"
	ptemp[1] = idx_offset -AVI_STD_HEADER_SIZE+4+8; //// datalen+ "move" + index first 8bytes nouse
	ptemp[2] = RIFF_movi;	
	
	return 0;
}
static int avi_stdService(void *encoder)
{
	ENCODER_T *stdEncoder = (ENCODER_T *)encoder;
	STD_CHECK(stdEncoder);
	
	return 0;
}











AVI_ENCODER_OP_T avi_encoder_std=
{
	avi_standardInit,
	avi_standardUninit,
	avi_standardParse,
	avi_standardAddIndex,
	avi_standardEnd,
	avi_stdService,  // service
};




