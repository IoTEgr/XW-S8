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
* File Name   : aviEncoder.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file encoder
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

#define AVIE_CHECK(x)    if((x<0)||(x>=AVI_CHANNEL_NUM))return AVI_ERR_ENCODER;

extern AVI_ENCODER_OP_T avi_encoder_odml;
extern AVI_ENCODER_OP_T avi_encoder_std;


static ENCODER_T aviEncoder[AVI_CHANNEL_NUM];
/*******************************************************************************
* Function Name  : aviEncodeSetIdx1Length
* Description	 : INT8U *buffer : buffer for cache avi header,user should read back the avi idx
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeInit(INT8U type,int (*cachewrite)(int fd,void *cachebuffer,INT32U lenght),
														 int  (*seek)(int fd,INT32U pos,int flag))
{
	 int i;

	 for(i=0;i<AVI_CHANNEL_NUM;i++)
	 {
	 	 if(aviEncoder[i].state == 0)
		 	break;
	 }
	 if(i>=AVI_CHANNEL_NUM)
	 	return AVI_ERR_ENCODER;
	 
	 aviEncoder[i].state = 1;
	 
     aviEncoder[i].status = AVI_ERR_GENERAL;

	 if(type == AVI_TYPE_STANDARD)
	 	aviEncoder[i].encoder = &avi_encoder_std;
	 else if(type == AVI_TYPE_OPENDML)
	 	aviEncoder[i].encoder = &avi_encoder_odml;
	 else
	 	return aviEncoder[i].status;
     aviEncoder[i].write = cachewrite;
	 aviEncoder[i].seek  = seek;
     if(aviEncoder[i].encoder->init)
	 	aviEncoder[i].status = aviEncoder[i].encoder->init((void *)&aviEncoder[i]);

	 return i;
}
/*******************************************************************************
* Function Name  : aviEncodeUninit
* Description	 : avi ecnoder uninit
* Input 		 : int favi : avi channel hanlder
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeUninit(int favi)
{
	AVIE_CHECK(favi);
	if(aviEncoder[favi].encoder == NULL) 
		return AVI_ERR_ENCODER;
	if(aviEncoder[favi].encoder->uninit)
	 	aviEncoder[favi].status = aviEncoder[favi].encoder->uninit((void *)&aviEncoder[favi]);
	
	aviEncoder[favi].encoder = NULL; //release

	aviEncoder[favi].state = 0;  // release 

	return AVI_ERR_NONE;
}
/*******************************************************************************
* Function Name  : aviEncodePrase
* Description	 : avi ecnoder prase
* Input 		 : INT16U width : avi mjpeg width
                  : INT16U height: avi mjpeg height
                  : INT16U fps    : avi video frame rate
                  : INT16U audio : record audio or not
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeParse(int favi,int fhandle,INT16U width,INT16U height,INT16U fps,INT16U audio,INT16U samperate)
{
	AVIE_CHECK(favi);
	
	if((aviEncoder[favi].encoder==NULL) || (aviEncoder[favi].encoder->parse==NULL))
		return AVI_ERR_GENERAL;
    aviEncoder[favi].fhandle = fhandle;
	return aviEncoder[favi].encoder->parse(&aviEncoder[favi],width,height,fps,audio,samperate);
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
int aviEncodeAddIdx(int favi,INT8U *buff,INT8U type,INT8U keyFrame,INT32U size,INT32U buflen)
{
	AVIE_CHECK(favi);
	
	if((aviEncoder[favi].encoder==NULL) || (aviEncoder[favi].encoder->encode==NULL))
		return AVI_ERR_GENERAL;

	aviEncoder[favi].status = aviEncoder[favi].encoder->encode(&aviEncoder[favi],buff,type,keyFrame,size,buflen);

	return aviEncoder[favi].status;
}

/*******************************************************************************
* Function Name  : aviEncodeSetIdx1Length
* Description	 : INT8U *buffer : buffer for cache avi header,user should read back the avi idx
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeSetIdx1(int favi,INT8U *buffer)
{
	INT32U *ptemp;

	AVIE_CHECK(favi);
	if(buffer==NULL)
		return AVI_ERR_GENERAL;

	ptemp = (INT32U*)buffer;
	ptemp[0] = 0;
	ptemp[1] = 0;
	ptemp[2] = RIFF_idx1;
	if(aviEncoder[favi].dwIdexlen>=16)
		ptemp[3] = aviEncoder[favi].dwIdexlen-16;
	else
		return -1;
	return 0;
}
/*******************************************************************************
* Function Name  : aviEncodeFrameTag
* Description	 : make avi tag to movi phase
* Input 		 : INT32U *buffer : frame addr
                    INT32U tag : index type:AVI_FRAME_DC_VIDEO/AVI_FRAME_DB_VIDEO/AVI_FRAME_WD_AUDIO
                    INT32U size : fram size
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeFrameTag(int favi,INT32U *buffer,INT32U tag,INT32U size)
{
	AVIE_CHECK(favi);
	
	if(buffer==NULL)
		return -1;
	
    if(tag == AVI_FRAME_DC_VIDEO)
        buffer[0] = RIFF_00dc;
	else if(tag == AVI_FRAME_DB_VIDEO)
		buffer[0] = RIFF_00DB;
	else if(tag == AVI_FRAME_WD_AUDIO)
		buffer[0 ] = RIFF_01wb;
	else
		return -2;
	buffer[1] = size;

	return 0;
}
/*******************************************************************************
* Function Name  : aviEncodeAddOffset
* Description	 : maybe some error ocured,a error frame is writed to avi file,but this frame can not be used
* Input 		 : INT32U add_offset : size to be add
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeAddOffset(int favi,INT32U add_offset)
{
	AVIE_CHECK(favi);
	
	aviEncoder[favi].ofset+=add_offset;

	return aviEncoder[favi].ofset;
}
/*******************************************************************************
* Function Name  : aviEncodeGetIdx1Length
* Description	 :
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeGetIdx1Length(int favi)
{
	AVIE_CHECK(favi);
	
	return aviEncoder[favi].dwIdexlen;
}
/*******************************************************************************
* Function Name  : aviEncodeGetFrameCount
* Description	 :
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 frame cnt
*******************************************************************************/
int aviEncodeGetFrameCount(int favi)
{
	AVIE_CHECK(favi);
	
	return aviEncoder[favi].framecnt;
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
int aviEncodeEnd(int favi,INT8U *buffer,INT32U fsize,INT32U idx_offset)
{
	AVIE_CHECK(favi);
	
	if((aviEncoder[favi].encoder==NULL) || (aviEncoder[favi].encoder->end==NULL))
		return AVI_ERR_GENERAL;

	aviEncoder[favi].status = aviEncoder[favi].encoder->end(&aviEncoder[favi],buffer,fsize,idx_offset);
	
	return AVI_ERR_NONE;
}
/*******************************************************************************
* Function Name  : aviEncodeService
* Description	 : avi encode service
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int aviEncodeService(int favi)
{
	AVIE_CHECK(favi);
	
	if((aviEncoder[favi].encoder==NULL) || (aviEncoder[favi].encoder->service==NULL))
		return AVI_ERR_GENERAL;
	aviEncoder[favi].status = aviEncoder[favi].encoder->service(&aviEncoder[favi]);

	return aviEncoder[favi].status;
}

int aviEncodeAddofs(int favi,INT32U offset)
{
	AVIE_CHECK(favi);
	
	if((aviEncoder[favi].encoder==NULL) || (aviEncoder[favi].encoder->service==NULL))
		return AVI_ERR_GENERAL;
	aviEncoder[favi].status = avi_odmlAddoffset(&aviEncoder[favi],offset);

	return aviEncoder[favi].status;
}













