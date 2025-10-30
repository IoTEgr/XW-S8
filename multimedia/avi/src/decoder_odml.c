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
* File Name   : decoder_odml.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file,for avi opendml
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


#if 1

#define  DWORD_H_SET(w,v)    w = ((w)&0x0000ffff)|(((v)&0x0000ffff)<<16)
#define  DWORD_L_SET(w,v)    w = ((w)&0xffff0000)|(((v)&0x0000ffff)<<0)
#define  DWORD_H_GET(w)      (((w)&0xFFFF0000)>>16)
#define  DWORD_L_GET(w)      (((w)&0x0000FFFF))

static DECODEER_T *odmlDecoder;

static int avi_odmlInit(void *handler)
{
	if(handler == NULL)
		return AVI_ERR_GENERAL;
	
	odmlDecoder = (DECODEER_T *)handler;
    odmlDecoder->auds_index = 0;
	odmlDecoder->vids_index = 0;
    odmlDecoder->idx1_length = 0;  // save idx1 size
	odmlDecoder->idx1_offset = 0;
	odmlDecoder->idx1_cache_offset = 0;
	odmlDecoder->idx1_cache_cnt = 0;
	odmlDecoder->frameIdx = 0;
	odmlDecoder->videoIdx = 0;
	odmlDecoder->framestep = 1; // frame one by one
	DWORD_H_SET(odmlDecoder->dir,1);
	DWORD_L_SET(odmlDecoder->dir,1);

    aviDecodeIdx1QInit(&odmlDecoder->vidsIdx1Q,odmlDecoder->vidsIdx1Stack,AVI_VIDS_CACHE_NUM);
	aviDecodeIdx1QInit(&odmlDecoder->audsIdx1Q,odmlDecoder->audsIdx1Stack,AVI_AUDS_CACHE_NUM);
	
	return AVI_ERR_NONE;
}

/*******************************************************************************
* Function Name  : avi_odmlGetVidsIdx
* Description	 : avi decode get vids idx 
* Input 		 : SUPER_IDX_A_T **idxa : idx point
* Output		 : none 										   
* Return		 : int
*******************************************************************************/
static int avi_odmlGetVidsIdx(AVI_IX_T **idxa)
{
	int size,offset;
	INT32U *dcache;
//	INT32U tOffset;
    AVI_INDX_T *info;

	dcache = odmlDecoder->idx1_cache;
	if(odmlDecoder->framestep >=1)
	{
		if(DWORD_H_GET(odmlDecoder->idx1_cache_offset)>=DWORD_H_GET(odmlDecoder->idx1_cache_cnt)) // load index table
		{
			if(odmlDecoder->vids_index>=odmlDecoder->vids_entrys) // end of vids
				return AVI_ERR_GENERAL;
			
			odmlDecoder->read(dcache,odmlDecoder->vids_offset+odmlDecoder->vids_index*16,16);
			info = (AVI_INDX_T *)dcache;

			odmlDecoder->vids_index++;
			size = info->dwSize;
			if(odmlDecoder->read(dcache,info->qwOffset,size)<0)
				return AVI_ERR_READ;
			offset = dcache[4];
			if(offset!=RIFF_00dc) // tag fail
				return AVI_ERR_GENERAL;
			odmlDecoder->vids_movioffset = dcache[5];
			DWORD_H_SET(odmlDecoder->idx1_cache_offset,8);
		    DWORD_H_SET(odmlDecoder->idx1_cache_cnt,size>>2);
			DWORD_H_SET(odmlDecoder->idx1_length,size);
			DWORD_H_SET(odmlDecoder->idx1_offset,size);
			//deg_Printf("load vids:0x%x,0x%x,0x%x,0x%x,\n",odmlDecoder->vids_movioffset,odmlDecoder->vids_index,size,info->qwOffset);

			if(2==DWORD_H_GET(odmlDecoder->dir))
			{
				DWORD_H_SET(odmlDecoder->idx1_cache_offset,DWORD_H_GET(odmlDecoder->idx1_cache_offset_last));
			}

			//===============
			u16 i;
			for(i = 8;i < DWORD_H_GET(odmlDecoder->idx1_cache_cnt);i+=2)
			{
				*idxa = (AVI_IX_T *)(dcache+i);
				if((*idxa)->dwOffset >=8)
					(*idxa)->dwOffset -= 8;
			}

		}
		DWORD_H_SET(odmlDecoder->dir,1);
		offset = DWORD_H_GET(odmlDecoder->idx1_cache_offset);
		*idxa = (AVI_IX_T *)(dcache+offset);
		(*idxa)->dwOffset += odmlDecoder->vids_movioffset;
		DWORD_H_SET(odmlDecoder->idx1_cache_offset_last,DWORD_H_GET(odmlDecoder->idx1_cache_offset));
		DWORD_H_SET(odmlDecoder->idx1_cache_offset,offset+2*odmlDecoder->framestep);
		//deg_Printf("VF:%x,%x\n",(*idxa)->dwOffset,(*idxa)->dwSize);
	}
	else
	{


		int offset_1=8;
		if(odmlDecoder->framestep == -1) 
			offset_1=4;
		if(DWORD_H_GET(odmlDecoder->idx1_cache_offset)<=offset_1) // load index table
		//if(DWORD_H_GET(odmlDecoder->idx1_cache_offset)<=8) // load index table
		{
			odmlDecoder->vids_index--;
			if(odmlDecoder->vids_index<0) // begin of vids
			{
				return AVI_ERR_GENERAL;
			}
			odmlDecoder->read(dcache,odmlDecoder->vids_offset+odmlDecoder->vids_index*16,16);
			info = (AVI_INDX_T *)dcache;

			size = info->dwSize;
			if(odmlDecoder->read(dcache,info->qwOffset,size)<0)
				return AVI_ERR_READ;
			offset = dcache[4];
			if(offset!=RIFF_00dc) // tag fail
				return AVI_ERR_GENERAL;
			odmlDecoder->vids_movioffset = dcache[5];
			DWORD_H_SET(odmlDecoder->idx1_cache_cnt,size>>2);
			DWORD_H_SET(odmlDecoder->idx1_cache_offset,DWORD_H_GET(odmlDecoder->idx1_cache_cnt)-2);
			DWORD_H_SET(odmlDecoder->idx1_length,size);
			DWORD_H_SET(odmlDecoder->idx1_offset,size);
			//deg_Printf("==load vids==:0x%x,0x%x,0x%x,0x%x,\n",odmlDecoder->vids_movioffset,odmlDecoder->vids_index,size,info->qwOffset);

			//===============
			u32 i;
			for(i = 8;i < DWORD_H_GET(odmlDecoder->idx1_cache_cnt);i+=2)
			{
				*idxa = (AVI_IX_T *)(dcache+i);
				if((*idxa)->dwOffset >=8)
					(*idxa)->dwOffset -= 8;
			}

			if(1==DWORD_H_GET(odmlDecoder->dir))
			{
				DWORD_H_SET(odmlDecoder->idx1_cache_offset,DWORD_H_GET(odmlDecoder->idx1_cache_offset_last));
			}

		}
		DWORD_H_SET(odmlDecoder->dir,2);
		offset = DWORD_H_GET(odmlDecoder->idx1_cache_offset);
		*idxa = (AVI_IX_T *)(dcache+offset);
		(*idxa)->dwOffset += odmlDecoder->vids_movioffset;
		DWORD_H_SET(odmlDecoder->idx1_cache_offset_last,DWORD_H_GET(odmlDecoder->idx1_cache_offset));
		DWORD_H_SET(odmlDecoder->idx1_cache_offset,offset+2*odmlDecoder->framestep);
		//deg_Printf("VB:%x,%x\n",(*idxa)->dwOffset,(*idxa)->dwSize);
		
	}

	return AVI_ERR_NONE;
}
/*******************************************************************************
* Function Name  : avi_odmlGetAudsIdx
* Description	 : avi decode get auds idx 
* Input 		 : AVI_IX_T **idxa : idx point
* Output		 : none 										   
* Return		 : int
*******************************************************************************/
static int avi_odmlGetAudsIdx(AVI_IX_T **idxa)
{
	int size,offset;
	INT32U *dcache;

    AVI_INDX_T *info;

	dcache = odmlDecoder->idx1_cache+(odmlDecoder->idx1_cache_length>>3);
	if(odmlDecoder->framestep >=1)
	{
		if(DWORD_L_GET(odmlDecoder->idx1_cache_offset)>=DWORD_L_GET(odmlDecoder->idx1_cache_cnt)) // load index table
		{
			if(odmlDecoder->auds_index>=odmlDecoder->auds_entrys) // end of vids
				return AVI_ERR_GENERAL;
			
			odmlDecoder->read(dcache,odmlDecoder->auds_offset+odmlDecoder->auds_index*16,16);
			info = (AVI_INDX_T *)dcache;
			
			odmlDecoder->auds_index++;

			size = info->dwSize;
			if(odmlDecoder->read(dcache,info->qwOffset,size)<0)
				return AVI_ERR_READ;
			offset = dcache[4];
			if(offset!=RIFF_01wb) // tag fail
				return AVI_ERR_GENERAL;
			odmlDecoder->auds_movioffset = dcache[5];
			DWORD_L_SET(odmlDecoder->idx1_cache_offset,8);
		    DWORD_L_SET(odmlDecoder->idx1_cache_cnt,size>>2);
			DWORD_L_SET(odmlDecoder->idx1_length,size);
			DWORD_L_SET(odmlDecoder->idx1_offset,size);
			

			//===============
			u32 i;
			for(i = 8;i < DWORD_L_GET(odmlDecoder->idx1_cache_cnt);i+=2)
			{
				*idxa = (AVI_IX_T *)(dcache+i);
				if((*idxa)->dwOffset >=8)
					(*idxa)->dwOffset -= 8;
			}

			if(2==DWORD_L_GET(odmlDecoder->dir))
			{
				DWORD_L_SET(odmlDecoder->idx1_cache_offset,DWORD_L_GET(odmlDecoder->idx1_cache_offset_last));
			}

		}
		DWORD_L_SET(odmlDecoder->dir,1);
		offset = DWORD_L_GET(odmlDecoder->idx1_cache_offset);
		*idxa = (AVI_IX_T *)(dcache+offset);
	/*	if(odml->AVIXnum && (odml->AVIXnum!=2))
			(*idxa)->dwOffset += odml->lastAVIXOffset;
		else*/
			(*idxa)->dwOffset += odmlDecoder->auds_movioffset;//odmlDecoder->movi_offset;
	//	DWORD_L_SET(odmlDecoder->idx1_cache_offset,offset+2);
		DWORD_L_SET(odmlDecoder->idx1_cache_offset_last,DWORD_L_GET(odmlDecoder->idx1_cache_offset));
		DWORD_L_SET(odmlDecoder->idx1_cache_offset,offset+2*odmlDecoder->framestep);
	}
	else
	{
		if(DWORD_L_GET(odmlDecoder->idx1_cache_offset)<=8) // load index table
		{
			odmlDecoder->auds_index--;
			if(odmlDecoder->auds_index<0) // begin of auds
			{
				return AVI_ERR_GENERAL;
			}

			odmlDecoder->read(dcache,odmlDecoder->auds_offset+odmlDecoder->auds_index*16,16);
			info = (AVI_INDX_T *)dcache;
	
			size = info->dwSize;
			if(odmlDecoder->read(dcache,info->qwOffset,size)<0)
				return AVI_ERR_READ;
			offset = dcache[4];
			if(offset!=RIFF_01wb) // tag fail
				return AVI_ERR_GENERAL;
			odmlDecoder->auds_movioffset = dcache[5];
			DWORD_L_SET(odmlDecoder->idx1_cache_cnt,size>>2);
			DWORD_L_SET(odmlDecoder->idx1_cache_offset,DWORD_L_GET(odmlDecoder->idx1_cache_cnt)-2);
			DWORD_L_SET(odmlDecoder->idx1_length,size);
			DWORD_L_SET(odmlDecoder->idx1_offset,size);

			//===============
			u16 i;
			for(i = 8;i < DWORD_L_GET(odmlDecoder->idx1_cache_cnt);i+=2)
			{
				*idxa = (AVI_IX_T *)(dcache+i);
				if((*idxa)->dwOffset >=8)
					(*idxa)->dwOffset -= 8;
			}

			if(1==DWORD_L_GET(odmlDecoder->dir))
			{
				DWORD_L_SET(odmlDecoder->idx1_cache_offset,DWORD_L_GET(odmlDecoder->idx1_cache_offset_last));
			}

		}
		DWORD_L_SET(odmlDecoder->dir,2);
		offset = DWORD_L_GET(odmlDecoder->idx1_cache_offset);
		*idxa = (AVI_IX_T *)(dcache+offset);
		(*idxa)->dwOffset += odmlDecoder->auds_movioffset;
		DWORD_L_SET(odmlDecoder->idx1_cache_offset_last,DWORD_L_GET(odmlDecoder->idx1_cache_offset));
		DWORD_L_SET(odmlDecoder->idx1_cache_offset,offset+2*odmlDecoder->framestep);

		//deg_Printf("Aoffset=0x%x\n",DWORD_H_GET(odmlDecoder->idx1_cache_offset));
	}
	return AVI_ERR_NONE;
}

/*******************************************************************************
* Function Name  : aviDecodeService
* Description	 : avi decode cache idx1 table
* Input 		 : none
* Output		 : none 										   
* Return		 : int frame type
*******************************************************************************/
static int avi_odmlService(void)
{
	AVI_IX_T *aviIdx1;
	int ret=0;

	if(odmlDecoder->read == NULL) // file read call back
		return AVI_ERR_READ;	

    if(odmlDecoder->vidsIdx1Q.busy < AVI_VIDS_CACHE_NUM)
    {
		if(avi_odmlGetVidsIdx(&aviIdx1)>=0)
		{
		//	if(aviIdx1->dwOffset>=8)
		//		aviIdx1->dwOffset-=8;
			aviDecodeIdx1QIn(&odmlDecoder->vidsIdx1Q,(AVIINDEXENTRY*)aviIdx1);
			odmlDecoder->frameIdx+=odmlDecoder->framestep;
		}
		else
			ret|=1;
    }

	if(odmlDecoder->audsIdx1Q.busy < AVI_AUDS_CACHE_NUM)
	{
		if(avi_odmlGetAudsIdx(&aviIdx1)>=0)
		{
		//	if(aviIdx1->dwOffset>=8)
		//		aviIdx1->dwOffset-=8;
			aviDecodeIdx1QIn(&odmlDecoder->audsIdx1Q,(AVIINDEXENTRY*)aviIdx1);
			odmlDecoder->frameIdx+=odmlDecoder->framestep;
		}
		else
			ret|=2;
	}
	if(ret==3)
		return AVI_ERR_END;
	return AVI_ERR_NONE;
}
/*******************************************************************************
* Function Name  : aviDecodeOneFrame
* Description	 : avi decode get one frame information
* Input 		 : INT32U *offset : offset of frame data in avi file
                  : INT32U *length : frame data len
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_odmlDecodeFrame(INT32U *offset,INT32U *length,INT32U type)
{
	AVIINDEXENTRY aviIdx1;
    AVI_IX_T *superidx1 = (AVI_IX_T *)&aviIdx1;


    if(type == AVI_FRAME_DC_VIDEO)
    {
		if(odmlDecoder->vidsIdx1Q.busy>0)
			aviDecodeIdx1QOut(&odmlDecoder->vidsIdx1Q,&aviIdx1);
		else
			return AVI_ERR_GENERAL;			
    }
	else if(type == AVI_FRAME_WD_AUDIO)
	{
		if(odmlDecoder->audsIdx1Q.busy>0)
			aviDecodeIdx1QOut(&odmlDecoder->audsIdx1Q,&aviIdx1);
		else
			return -2;		
	}
	else 
		return -3;
    if(offset)
		*offset = superidx1->dwOffset;// +odmlDecoder->movi_offset;
	if(length)
		*length = superidx1->dwSize&0x7fffffff;

//	odmlDecoder->frameIdx++;

	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeFast
* Description	 : avi decode fast forward or fast rewind
* Input 		 : int deta : fast frames
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_odmlFast(int deta)
{
	if(deta==0)
	{
		odmlDecoder->framestep = 1;
		if(2==DWORD_H_GET(odmlDecoder->dir))
		{
			DWORD_H_SET(odmlDecoder->idx1_cache_offset,512);	// video reload
			DWORD_L_SET(odmlDecoder->idx1_cache_offset,512);	//audio reload
		}
	}
	else if(deta < 0)
	{
		odmlDecoder->framestep = deta;	// -1~-4
		if(1==DWORD_H_GET(odmlDecoder->dir))
		{
			DWORD_H_SET(odmlDecoder->idx1_cache_offset,0);	// video reload
			DWORD_L_SET(odmlDecoder->idx1_cache_offset,0);	//audio reload
		}
	}
	else if(deta > 0)
	{
		odmlDecoder->framestep = deta;
		if(2==DWORD_H_GET(odmlDecoder->dir))
		{
			DWORD_H_SET(odmlDecoder->idx1_cache_offset,512);	// video reload
			DWORD_L_SET(odmlDecoder->idx1_cache_offset,512);	//audio reload
		}
	}

    return odmlDecoder->framestep;
/*	odmlDecoder->frameIdx +=deta;
	if(odmlDecoder->frameIdx<0)
		odmlDecoder->frameIdx = 0;*/
//	return AVI_ERR_GENERAL;	
}

/*******************************************************************************
* Function Name  : aviDecodeGetTime
* Description	 : avi decode time
* Input 		 : INT32U *totaltime : avi total time
                  :INT32U *curtime   : current time
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_odmlGetTime(INT32U *totaltime,INT32U *curtime)
{
	u32 sec,ms;
	
	if(totaltime)
	{
		ms = 1000/odmlDecoder->frametime;
		sec = odmlDecoder->framecnt/ms; // ->second
		ms = (odmlDecoder->framecnt%ms)*odmlDecoder->frametime;
		*totaltime = sec*1000+ms;
		//*totaltime = (odmlDecoder->framecnt*(odmlDecoder->frametime)); // ms
	}

	if(curtime)
	{
		ms = (odmlDecoder->frameIdx*30)/34;
		sec = 1000/odmlDecoder->frametime;
		*curtime = (ms/sec)*1000;
		*curtime += (ms%sec)*odmlDecoder->frametime;

		//*curtime = ((odmlDecoder->frameIdx*30)/34)*(odmlDecoder->frametime); // ms  -> 30frame vds,4->ads
	}

	return 0;
}



AVI_DECODER_OP_T avi_opendml=
{
	avi_odmlInit,
	avi_odmlDecodeFrame,
	avi_odmlGetTime,
	avi_odmlFast,
	avi_odmlService,
};


#else



#define  DWORD_H_SET(w,v)    w = ((w)&0x0000ffff)|(((v)&0x0000ffff)<<16)
#define  DWORD_L_SET(w,v)    w = ((w)&0xffff0000)|(((v)&0x0000ffff)<<0)
#define  DWORD_H_GET(w)      (((w)&0xFFFF0000)>>16)
#define  DWORD_L_GET(w)      (((w)&0x0000FFFF))

static DECODEER_T *odmlDecoder;

static int avi_odmlInit(void *handler)
{
	if(handler == NULL)
		return AVI_ERR_GENERAL;
	
	odmlDecoder = (DECODEER_T *)handler;
    odmlDecoder->auds_index = 0;
	odmlDecoder->vids_index = 0;
    odmlDecoder->idx1_length = 0;  // save idx1 size
	odmlDecoder->idx1_offset = 0;
	odmlDecoder->idx1_cache_offset = 0;
	odmlDecoder->idx1_cache_cnt = 0;
	odmlDecoder->frameIdx = 0;
	odmlDecoder->videoIdx = 0;
	odmlDecoder->framestep = 1; // frame one by one

    aviDecodeIdx1QInit(&odmlDecoder->vidsIdx1Q,odmlDecoder->vidsIdx1Stack,AVI_VIDS_CACHE_NUM);
	aviDecodeIdx1QInit(&odmlDecoder->audsIdx1Q,odmlDecoder->audsIdx1Stack,AVI_AUDS_CACHE_NUM);
	
	return AVI_ERR_NONE;
}

/*******************************************************************************
* Function Name  : avi_odmlGetVidsIdx
* Description	 : avi decode get vids idx 
* Input 		 : SUPER_IDX_A_T **idxa : idx point
* Output		 : none 										   
* Return		 : int
*******************************************************************************/
static int avi_odmlGetVidsIdx(AVI_IX_T **idxa)
{
	int size,offset;
	INT32U *dcache;
//	INT32U tOffset;
    AVI_INDX_T *info;

	
	dcache = odmlDecoder->idx1_cache;
	
	if(DWORD_H_GET(odmlDecoder->idx1_cache_offset)>=DWORD_H_GET(odmlDecoder->idx1_cache_cnt)) // load index table
	{
		if(odmlDecoder->vids_index>=odmlDecoder->vids_entrys) // end of vids
			return AVI_ERR_GENERAL;
		
		odmlDecoder->read(dcache,odmlDecoder->vids_offset+odmlDecoder->vids_index*16,16);
		info = (AVI_INDX_T *)dcache;

		size = info->dwSize;
		if(odmlDecoder->read(dcache,info->qwOffset,size)<0)
			return AVI_ERR_READ;
		offset = dcache[4];
		if(offset!=RIFF_00dc) // tag fail
			return AVI_ERR_GENERAL;
		odmlDecoder->vids_movioffset = dcache[5];
	    DWORD_H_SET(odmlDecoder->idx1_cache_cnt,size>>2);
		DWORD_H_SET(odmlDecoder->idx1_cache_offset,8);
		DWORD_H_SET(odmlDecoder->idx1_length,size);
		DWORD_H_SET(odmlDecoder->idx1_offset,size);
	//	if(size == info->size)
			odmlDecoder->vids_index++;
	}
	offset = DWORD_H_GET(odmlDecoder->idx1_cache_offset);
	*idxa = (AVI_IX_T *)(dcache+offset);
	/*if(odml->AVIXnum && (odml->AVIXnum!=1))
		(*idxa)->dwOffset += odml->lastAVIXOffset;
	else*/
		(*idxa)->dwOffset += odmlDecoder->vids_movioffset;//odmlDecoder->movi_offset;
//	DWORD_H_SET(odmlDecoder->idx1_cache_offset,offset+2);
	DWORD_H_SET(odmlDecoder->idx1_cache_offset,offset+2*odmlDecoder->framestep);

	return AVI_ERR_NONE;
}
/*******************************************************************************
* Function Name  : avi_odmlGetAudsIdx
* Description	 : avi decode get auds idx 
* Input 		 : AVI_IX_T **idxa : idx point
* Output		 : none 										   
* Return		 : int
*******************************************************************************/
static int avi_odmlGetAudsIdx(AVI_IX_T **idxa)
{
	int size,offset;
	INT32U *dcache;

    AVI_INDX_T *info;


	dcache = odmlDecoder->idx1_cache+(odmlDecoder->idx1_cache_length>>3);
	if(DWORD_L_GET(odmlDecoder->idx1_cache_offset)>=DWORD_L_GET(odmlDecoder->idx1_cache_cnt)) // load index table
	{
		if(odmlDecoder->auds_index>=odmlDecoder->auds_entrys) // end of vids
			return AVI_ERR_GENERAL;
		
		odmlDecoder->read(dcache,odmlDecoder->auds_offset+odmlDecoder->auds_index*16,16);
		info = (AVI_INDX_T *)dcache;

		size = info->dwSize;
		if(odmlDecoder->read(dcache,info->qwOffset,size)<0)
			return AVI_ERR_READ;
		offset = dcache[4];
		if(offset!=RIFF_01wb) // tag fail
			return AVI_ERR_GENERAL;
		odmlDecoder->auds_movioffset = dcache[5];
	    DWORD_L_SET(odmlDecoder->idx1_cache_cnt,size>>2);
		DWORD_L_SET(odmlDecoder->idx1_cache_offset,8);
		DWORD_L_SET(odmlDecoder->idx1_length,size);
		DWORD_L_SET(odmlDecoder->idx1_offset,size);
		//if(size == info->size)
			odmlDecoder->auds_index++;
	}
	offset = DWORD_L_GET(odmlDecoder->idx1_cache_offset);
	*idxa = (AVI_IX_T *)(dcache+offset);
/*	if(odml->AVIXnum && (odml->AVIXnum!=2))
		(*idxa)->dwOffset += odml->lastAVIXOffset;
	else*/
		(*idxa)->dwOffset += odmlDecoder->auds_movioffset;//odmlDecoder->movi_offset;
//	DWORD_L_SET(odmlDecoder->idx1_cache_offset,offset+2);
	DWORD_L_SET(odmlDecoder->idx1_cache_offset,offset+2*odmlDecoder->framestep);

	return AVI_ERR_NONE;
}

/*******************************************************************************
* Function Name  : aviDecodeService
* Description	 : avi decode cache idx1 table
* Input 		 : none
* Output		 : none 										   
* Return		 : int frame type
*******************************************************************************/
static int avi_odmlService(void)
{
	AVI_IX_T *aviIdx1;
	int ret=0;

	if(odmlDecoder->read == NULL) // file read call back
		return AVI_ERR_READ;	

    if(odmlDecoder->vidsIdx1Q.busy < AVI_VIDS_CACHE_NUM)
    {
		if(avi_odmlGetVidsIdx(&aviIdx1)>=0)
		{
			if(aviIdx1->dwOffset>=8)
				aviIdx1->dwOffset-=8;
		//	aviIdx1->dwOffset&=~0x1ff;
			aviDecodeIdx1QIn(&odmlDecoder->vidsIdx1Q,(AVIINDEXENTRY*)aviIdx1);
			odmlDecoder->frameIdx+=odmlDecoder->framestep;
		}
		else
			ret|=1;
    }

	if(odmlDecoder->audsIdx1Q.busy < AVI_AUDS_CACHE_NUM)
	{
		if(avi_odmlGetAudsIdx(&aviIdx1)>=0)
		{
			if(aviIdx1->dwOffset>=8)
				aviIdx1->dwOffset-=8;
		//	aviIdx1->dwOffset&=~0x1ff;
			aviDecodeIdx1QIn(&odmlDecoder->audsIdx1Q,(AVIINDEXENTRY*)aviIdx1);
			odmlDecoder->frameIdx+=odmlDecoder->framestep;
		}
		else
			ret|=2;
	}
	if(ret==3)
		return AVI_ERR_END;
	return AVI_ERR_NONE;
}
/*******************************************************************************
* Function Name  : aviDecodeOneFrame
* Description	 : avi decode get one frame information
* Input 		 : INT32U *offset : offset of frame data in avi file
                  : INT32U *length : frame data len
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_odmlDecodeFrame(INT32U *offset,INT32U *length,INT32U type)
{
	AVIINDEXENTRY aviIdx1;
    AVI_IX_T *superidx1 = (AVI_IX_T *)&aviIdx1;


    if(type == AVI_FRAME_DC_VIDEO)
    {
		if(odmlDecoder->vidsIdx1Q.busy>0)
			aviDecodeIdx1QOut(&odmlDecoder->vidsIdx1Q,&aviIdx1);
		else
			return AVI_ERR_GENERAL;			
    }
	else if(type == AVI_FRAME_WD_AUDIO)
	{
		if(odmlDecoder->audsIdx1Q.busy>0)
			aviDecodeIdx1QOut(&odmlDecoder->audsIdx1Q,&aviIdx1);
		else
			return -2;		
	}
	else 
		return -3;
    if(offset)
		*offset = superidx1->dwOffset;// +odmlDecoder->movi_offset;
	if(length)
		*length = superidx1->dwSize&0x7fffffff;

//	odmlDecoder->frameIdx++;

	return 0;
}
/*******************************************************************************
* Function Name  : aviDecodeFast
* Description	 : avi decode fast forward or fast rewind
* Input 		 : int deta : fast frames
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_odmlFast(int deta)
{
/*
	if(deta == 0)
	{
		odmlDecoder->framestep = 1;
	}
	else if(deta < 0)
	{
		if(odmlDecoder->framestep+deta > 0)
		{
			odmlDecoder->framestep += deta;
		}
		else 
		{
			odmlDecoder->framestep = 1;
		}
	}
	else if(deta > 0)
	{
		odmlDecoder->framestep = deta+1;
	}
*/
	odmlDecoder->framestep = 1;
    return odmlDecoder->framestep;
/*	odmlDecoder->frameIdx +=deta;
	if(odmlDecoder->frameIdx<0)
		odmlDecoder->frameIdx = 0;*/
//	return AVI_ERR_GENERAL;	
}

/*******************************************************************************
* Function Name  : aviDecodeGetTime
* Description	 : avi decode time
* Input 		 : INT32U *totaltime : avi total time
                  :INT32U *curtime   : current time
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
static int avi_odmlGetTime(INT32U *totaltime,INT32U *curtime)
{
	u32 sec,ms;
	
	if(totaltime)
	{
		ms = 1000/odmlDecoder->frametime;
		sec = odmlDecoder->framecnt/ms; // ->second
		ms = (odmlDecoder->framecnt%ms)*odmlDecoder->frametime;
		*totaltime = sec*1000+ms;
		//*totaltime = (odmlDecoder->framecnt*(odmlDecoder->frametime)); // ms
	}

	if(curtime)
	{
		ms = (odmlDecoder->frameIdx*30)/34;
		sec = 1000/odmlDecoder->frametime;
		*curtime = (ms/sec)*1000;
		*curtime += (ms%sec)*odmlDecoder->frametime;

		//*curtime = ((odmlDecoder->frameIdx*30)/34)*(odmlDecoder->frametime); // ms  -> 30frame vds,4->ads
	}

	return 0;
}



AVI_DECODER_OP_T avi_opendml=
{
	avi_odmlInit,
	avi_odmlDecodeFrame,
	avi_odmlGetTime,
	avi_odmlFast,
	avi_odmlService,
};


#endif










