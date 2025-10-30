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
* File Name   : encoder_odml.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file,for avi opendml file
*               
* History     : 
* 2016-09-22  : mark
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information

******************************************************************************/
#include "../../media.h"
#include "../../multimedia.h"
#include "../inc/avi.h"

#define  ODML_CHECK(x)    if((x==NULL)||(x->prit==NULL))return AVI_ERR_GENERAL

//static void *encoder;
//ODML_OP_T odmlBuff;

static int avi_odmlInit(void *encoder)
{
	AVI_IX_HEADER_T *idxHeader;
	ODML_OP_T *odml;
	ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	if(odmlEncoder==NULL)
		return AVI_ERR_GENERAL;
	u32 size = ((sizeof(ODML_OP_T)+32*1024)+0x3f)&(~0x3f);
	//odml = (ODML_OP_T *)hal_sysMemMalloc(sizeof(ODML_OP_T),64);
	odml = (ODML_OP_T *)hal_sysMemMalloc(size,64);
    if(odml == NULL)
		return AVI_ERR_MEMORY;
     deg_Printf("HAL : [ODML]<INFO>  addr = 0x%x,size = 0x%x\n",odml,size);
	odmlEncoder->prit = odml;
	 memset(odml,0,size);
	// memset(odml,0,sizeof(ODML_OP_T));
//avi odml ix table for vids

	idxHeader = (AVI_IX_HEADER_T *)odml->vidsmemPool;
	idxHeader->fcc = RIFF_ix00;
	idxHeader->dwChunkId= RIFF_00dc;
	odml->vidsixtable = (AVI_IX_T *)(odml->vidsmemPool+AVI_IX_HEADER_LEN);
//avi odml ix table for auds	
	idxHeader = (AVI_IX_HEADER_T *)odml->audsmemPool;
	idxHeader->fcc = RIFF_ix01;
	idxHeader->dwChunkId= RIFF_01wb;
	odml->audsixtable = (AVI_IX_T *)(odml->audsmemPool+AVI_IX_HEADER_LEN);
	
	

	return AVI_ERR_NONE;
}

static int avi_odmlUninit(void *encoder)
{
	ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	ODML_CHECK(odmlEncoder);

	hal_sysMemFree(odmlEncoder->prit);

	return 0;
}

static int avi_odmlParse(void *encoder,INT16U width,INT16U height,INT16U fps,INT16U audio,INT16U samperate)
{
	//FRESULT ret;
	//UINT writed;
	 ODMLAVIFILEHEADER *aviheader;
	 ODML_OP_T *odml;
	 INT32U temp;
     ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	 ODML_CHECK(odmlEncoder);
	s32 alignsize,size;
	INT32U *temp1;
	 odml = (ODML_OP_T *)odmlEncoder->prit;

	if((odmlEncoder->write==NULL))
		return AVI_ERR_WRITE;

	aviheader = (ODMLAVIFILEHEADER *)hal_sysMemMalloc(AVI_ODML_HEADER_SIZE,64);
	
	if(aviheader == NULL)
	{
		deg_Printf("avi : odml malloc fial.need size = %x\n",AVI_ODML_HEADER_SIZE);
		return AVI_ERR_MEMORY;
	}
	 
	//*buffer=*headerbuf;
	memset((void *)aviheader,0,AVI_ODML_HEADER_SIZE);
	
	if(audio)
	{
		//fill strl auds
		aviheader->strl_a.fcc = RIFF_LIST;			//"LIST"
		aviheader->strl_a.cb = 888-(AVI_ODML_SIDX_LEN*16)%512 +sizeof(aviheader->strl_a.fcctype)+2*sizeof(CHUNK)+sizeof(AVISTREAMHEADER)+sizeof(WAVEFORMATEX) + sizeof(AVISUPERINDEXHEAD)+AVI_ODML_SIDX_LEN*AVI_INDX_LEN;
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
		aviheader->strh_a.dwSuggestedBufferSize=0x00010000;  //64k空间
		aviheader->strh_a.dwQuality	=0xFFFFFFFF;	
		//【strf：音频】
		//fill auds stream info
		aviheader->strf_a.dwFourCC = RIFF_strf;							//"strf"
		aviheader->strf_a.dwSize = sizeof(WAVEFORMATEX);
	//	deg_Printf("sizeof(WAVEFORMATEX) = 0x%x \n",sizeof(WAVEFORMATEX));
		aviheader->wavinfo.wFormatTag = 1;			//1:pcm    2:adpcm
		aviheader->wavinfo.nChannels = 1;//2
		aviheader->wavinfo.nSamplesPerSec = samperate;	//11025 , 22050, 44100
		aviheader->wavinfo.nAvgBytesPerSec = samperate<< 1;//64000
		aviheader->wavinfo.nBlockAlign = 2;	//4
		aviheader->wavinfo.wBitsPerSample = 16;
		

		
		//auds superindx
		aviheader->sidx_a.dwFourCC = RIFF_indx; //"indx"
		aviheader->sidx_a.dwSize  = sizeof(AVISUPERINDEXHEAD)+sizeof(AVI_INDX_T)*AVI_ODML_SIDX_LEN;

		aviheader->asupindxh.dwChunkId = RIFF_01wb;	//"01wb"
		aviheader->asupindxh.wLongsPerEntry=4;//4
		aviheader->asupindxh.bIndexSubType=0; // must be 0 or AVI_INDEX_2FIELD
		aviheader->asupindxh.bIndexType=AVI_INDEX_OF_INDEXES; // must be AVI_INDEX_OF_INDEXES
		aviheader->asupindxh.nEntriesInUse=0;//4
		aviheader->asupindxh.dwReserved[0]=0;//0
		aviheader->asupindxh.dwReserved[1]=0;//0
		aviheader->asupindxh.dwReserved[2]=0;//0

		//fill junk
		aviheader->junk.dwFourCC= RIFF_JUNK;			//"JUNK"
		aviheader->junk.dwSize= AVI_ODML_HEADER_SIZE -AVI_ODMLHEANDER_SIZE-12;// (sizeof(ODMLAVIFILEHEADER) + 12);
//		odmlattr.pcm = 1;
	}
	else{
		aviheader->strl_a.fcc = RIFF_JUNK;			//"JUNK"
		aviheader->strl_a.cb = 888-(AVI_ODML_SIDX_LEN*16)%512 +sizeof(aviheader->strl_a.fcctype)+2*sizeof(CHUNK)+sizeof(AVISTREAMHEADER)+sizeof(WAVEFORMATEX) + sizeof(AVISUPERINDEXHEAD)+AVI_ODML_SIDX_LEN*AVI_INDX_LEN;
		
		//fill junk
		aviheader->junk.dwFourCC= RIFF_JUNK;			//"JUNK"
		aviheader->junk.dwSize= AVI_ODML_HEADER_SIZE -AVI_ODMLHEANDER_SIZE-12;// (sizeof(ODMLAVIFILEHEADER) + 12);
	}

	

	//fill fcc and fcc type
	aviheader->riff.fcc = RIFF_TAG;		//"RIFF"
	aviheader->riff.fcctype = RIFF_AVI;	//"AVI "
	aviheader->hdrl.fcc = RIFF_LIST;		//"LIST"
	aviheader->hdrl.cb =AVI_ODML_HEADER_SIZE - 12 - 8-12;
	//aviheader->hdrl.cb =sizeof(AVIFILEHEADER) - 28; //point to junk in end of head 
	aviheader->hdrl.fcctype = RIFF_hdrl;	//"hdrl"

	//fill avimainheader
	aviheader->avih.fcc = RIFF_avih;	//"avih"
	aviheader->avih.cb = sizeof(AVIMAINHEADER)-8;						
	aviheader->avih.dwMicroSecPerFrame = 1000000/fps;			// 30fps ,so (1s/30)*1000*1000 = 33333us
	aviheader->avih.dwFlags = AVIF_HASINDEX|AVIF_MUSTUSEINDEX;							//has index
	aviheader->avih.dwSuggestedBufferSize=0x00100000;  //1MB空间，未设置时会导致某些播放器黑屏

	if(audio)
		aviheader->avih.dwStreams = 0x02;//aviattr.pcm;//0x2;					// jpeg and pcm 
	else
		aviheader->avih.dwStreams = 0x1;					// jpeg 

	aviheader->avih.dwWidth = width; //640
	aviheader->avih.dwHeight = height;//480

	//【strl：视频】
	//file move fcc and fcc type
	aviheader->strl_v.fcc = RIFF_LIST;			//"LIST"
	
	aviheader->strl_v.cb = sizeof(aviheader->strl_v.fcctype) + 3*sizeof(CHUNK)+sizeof(AVISTREAMHEADER)+sizeof(BITMAPINFOHEADER) + sizeof(VideoPropHeader) +sizeof(AVISUPERINDEXHEAD)+AVI_ODML_SIDX_LEN*AVI_INDX_LEN +1216-(AVI_ODML_SIDX_LEN*16)%512;
	
	aviheader->strl_v.fcctype = RIFF_strl;		//"strl"
	aviheader->strh_v.fcc = RIFF_strh;						//"strh"
	aviheader->strh_v.cb = sizeof(AVISTREAMHEADER)-8;		
	aviheader->strh_v.fccType = RIFF_vids;					//"vids"
	aviheader->strh_v.fccHandler = RIFF_MJPG;				//"MJPG"
	aviheader->strh_v.dwScale = 1;
	aviheader->strh_v.dwRate = fps;							
	aviheader->strh_v.dwStart = 0;
	aviheader->strh_v.rcFrame.right =width; 
	aviheader->strh_v.rcFrame.bottom = height;//480
	aviheader->strh_v.dwSuggestedBufferSize=0x00100000;  //1MB空间，未设置时会导致某些播放器黑屏
	aviheader->strh_v.dwQuality	=0xFFFFFFFF;	
	//【"strf：视频"】	
	//fill vids stream info
	aviheader->strf_v.dwFourCC = RIFF_strf;							//"strf"
	aviheader->strf_v.dwSize = sizeof(BITMAPINFOHEADER);				
	aviheader->bitmapinfo.biSize = sizeof(BITMAPINFOHEADER);			
	aviheader->bitmapinfo.biWidth = width; //640
	aviheader->bitmapinfo.biHeight = height;//480
	aviheader->bitmapinfo.biPlanes = 1;							// always 1
	aviheader->bitmapinfo.biBitCount = 24;							// 1, 4, 8, 16, 24, 32
	aviheader->bitmapinfo.biCompression = RIFF_MJPG;				//"MJPG"
	
	
	//video superindx
	aviheader->sidx_v.dwFourCC = RIFF_indx; //"indx"
	aviheader->sidx_v.dwSize  = sizeof(AVISUPERINDEXHEAD)+AVI_INDX_LEN*AVI_ODML_SIDX_LEN;
	aviheader->vsupindxh.dwChunkId = RIFF_00dc;	//"00dc"
	aviheader->vsupindxh.wLongsPerEntry=4;//4
	aviheader->vsupindxh.bIndexSubType=0; // must be 0 or AVI_INDEX_2FIELD
	aviheader->vsupindxh.bIndexType=AVI_INDEX_OF_INDEXES; // must be AVI_INDEX_OF_INDEXES
	aviheader->vsupindxh.nEntriesInUse=0;//4
	aviheader->vsupindxh.dwReserved[0]=0;//0
	aviheader->vsupindxh.dwReserved[1]=0;//0
	aviheader->vsupindxh.dwReserved[2]=0;//0
	//Video Properties Header	
	
	aviheader->c_vprp.dwFourCC=RIFF_vprp; //"vprp"
	aviheader->c_vprp.dwSize = sizeof(VideoPropHeader);
	aviheader->vprp.dwFrameWidthInPixels=width;
	aviheader->vprp.dwFrameHeightInLines=height;

	aviheader->vprp.dwHTotalInT = width;
	aviheader->vprp.dwVTotalInLines=height;
	aviheader->vprp.dwVerticalRefreshRate=30;
	aviheader->vprp.dwFrameAspectRatio=0x00100009;
	aviheader->vprp.VideoFormatToken=0;
	aviheader->vprp.VideoStandard=0;
	aviheader->vprp.nbFieldPerFrame =1;
	aviheader->vprp.FieldInfo.CompressedBMHeight=height;
	aviheader->vprp.FieldInfo.CompressedBMWidth=width;
	aviheader->vprp.FieldInfo.ValidBMHeight=height;
	aviheader->vprp.FieldInfo.ValidBMWidth=width;
	aviheader->vprp.FieldInfo.ValidBMXOffset=0;
	aviheader->vprp.FieldInfo.ValidBMYOffset=0;
	aviheader->vprp.FieldInfo.VideoXOffsetInT=0;
	aviheader->vprp.FieldInfo.VideoYValidStartLine=0;
	
	//Extended AVI Header (dmlh)
	aviheader->obml.fcc=RIFF_LIST;			//"LIST"
	aviheader->obml.cb=sizeof(ODMLExtendedAVIHeader) + sizeof(aviheader->obml.fcctype);			
	aviheader->obml.fcctype =RIFF_odml;			//"odml"
	aviheader->strodml.OpenDML_Header=RIFF_dmlh; //"dmlh"
	aviheader->strodml.size = sizeof(ODMLExtendedAVIHeader)-8;
	aviheader->strodml.dwTotalFrames=0;			//"TotalFrame"
	/****************secotor junk fill******************/
	aviheader->sector1_junk.dwFourCC= RIFF_JUNK;
	aviheader->sector1_junk.dwSize=260;
	
	aviheader->sector2_junk.dwFourCC= RIFF_JUNK;
	aviheader->sector2_junk.dwSize=512-(AVI_ODML_SIDX_LEN*16)%512 - 8;
	
	aviheader->sector3_junk.dwFourCC= RIFF_JUNK;
	aviheader->sector3_junk.dwSize=428;
	
	aviheader->sector4_junk.dwFourCC= RIFF_JUNK;
	aviheader->sector4_junk.dwSize=368;
	
	aviheader->sector5_junk.dwFourCC= RIFF_JUNK;
	aviheader->sector5_junk.dwSize=512-(AVI_ODML_SIDX_LEN*16)%512 - 8;
	/*****************************************************/
	if(odmlEncoder->write(odmlEncoder->fhandle,aviheader,AVI_ODML_HEADER_SIZE)<0)
	{
		hal_sysMemFree((void *)aviheader);
		deg_Printf("avi : odml write hander fail\n");
		return AVI_ERR_WRITE;
	}

	odmlEncoder->alignsize = alignsize = hal_sdNextLBA()&(0x3f);
	size=0;
	if(alignsize)
	{
		temp1 = (INT32U *)audiobuff;
		temp1[0]=0x4B4E554A;
		temp1[1]=(64-alignsize)*512-8;
		size=(64-alignsize)*512;
		if(odmlEncoder->write(odmlEncoder->fhandle,audiobuff,size)<0)
			return AVI_ERR_WRITE;
	}
				

    odmlEncoder->dwIdexlen = 0;
	odmlEncoder->framecnt = 0;
	odmlEncoder->ofset = 4;
	odmlEncoder->size = 0;
	
	odmlEncoder->ofset+=size;
	odmlEncoder->oldOffset=0;
	odmlEncoder->oldSize = 0;

    memcpy(&odml->vidsheader,aviheader,512);
	memcpy(&odml->audsheader,&aviheader->strl_a,512);
	memcpy(&odml->moviheader,&aviheader->obml,512);
	memcpy(&odml->vprpheader,&aviheader->c_vprp,512);
    temp = (AVI_ODML_SIDX_LEN*AVI_INDX_LEN+(512-(AVI_ODML_SIDX_LEN*16)%512))/512;
#if ODML_CFG_SYNC_EN == 0
	odml->sector2_junk.dwFourCC= RIFF_JUNK;
	odml->sector2_junk.dwSize=512-(AVI_ODML_SIDX_LEN*16)%512 - 8;
	odml->sector5_junk.dwFourCC= RIFF_JUNK;
	odml->sector5_junk.dwSize=512-(AVI_ODML_SIDX_LEN*16)%512 - 8;
#endif
	
	odml->qwOffset = AVI_ODML_HEADER_SIZE;  // file size
	odml->movioffset = AVI_ODML_HEADER_SIZE-4;
	odml->qwOffset+=size;
//>>>>>>WARINING>>>>> if file system cluster is less than 32k-byte,the calulattion is wrong.the sector is not one by one in file.

    int num,i;
    //num = (AVI_ODML_HEADER_SIZE+fs_getclustersize()-1)/fs_getclustersize();
	num = (AVI_ODML_HEADER_ALIGNSIZE+fs_getclustersize()-1)/fs_getclustersize();
	for(i=0;i<num;i++)
	{
		//deg_Printf("cluster[%d]",i);
		if(i==0)
			odml->headercluster[i] = fs_getcluster(odmlEncoder->fhandle,0);
		else
			odml->headercluster[i] = fs_getcluster(odmlEncoder->fhandle,odml->headercluster[i-1]);
		if(num>=ODML_CFG_CLUS_NUM)
			return AVI_ERR_FS;
		//deg_Printf(" = %x,sector = %x\n",odml->headercluster[i],fs_getsector(odmlEncoder->fhandle,odml->headercluster[i],1));
	}
//	odml->headercluster[i] = 0;
	
//---vids start sector	
	odml->vidsheadersector = fs_getsector(odmlEncoder->fhandle,0,0);	// first sector
	odml->vidsindxsector = odml->vidsheadersector+1;
	odml->vprpheadersector = odml->vidsheadersector+temp+1;
//----auds start sector    
	odml->audsheadersector = odml->vidsheadersector+temp+2;
	odml->audsindxsector = odml->audsheadersector+1;
//----movi start sector	
	odml->movisector = odml->vidsheadersector+temp*2+3;

	odml->AVIXnum = -1;
	odml->lastAVIXOffset = 0;

	hal_sysMemFree((void *)aviheader);
	//deg_Printf("headersector:%d\n",odml->vidsheadersector);
	/*if(odml->vidsheadersector==0) {deg_Printf("WARNING!!!\n"); while(1);}*/
	if(odml->vidsheadersector==0){
		deg_Printf("avi : get header sector fail\n");
		return AVI_ERR_WRITE;
	}
	return AVI_ODML_HEADER_SIZE;
	
}
static int avi_odmlSync(void *encoder)
{
	ODML_OP_T *odml;
    INT32U offset;
	ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
    ODML_CHECK(odmlEncoder);
	odml = (ODML_OP_T *)odmlEncoder->prit;
	INT32U *temp1,size;
	offset = odml->qwOffset;
	if(odml->AVIXnum>-1)
	{
		odml->movlist[odml->AVIXnum].riff.cb = offset- odml->moviofest[odml->AVIXnum] - 8 ;//list riff size
		odml->movlist[odml->AVIXnum].movi.cb = offset - odml->moviofest[odml->AVIXnum] - 20;//list movi size
        memset((void*)&odml->avix_1sect,0,sizeof(odml->avix_1sect));
		memcpy((void*)&odml->avix_1sect.Avix,&odml->movlist[odml->AVIXnum], sizeof(AVIXMOVILIST));
        
		odml->avix_1sect.junk.dwFourCC= RIFF_JUNK;//junk
		odml->avix_1sect.junk.dwSize=512-sizeof(AVIXMOVILIST)-8;
		if(hal_sdWrite(&odml->avix_1sect,odml->avix_sector[odml->AVIXnum],1)<0)
             return AVI_ERR_WRITE;
		
		odml->vidsheader.riff.cb = odml->moviofest[0] -8;
		odml->vidsheader.avih.dwTotalFrames = odml->vframecnt;
	}
	else
	{
		odml->vidsheader.riff.cb = offset -8;
		odml->vidsheader.avih.dwTotalFrames = odml->vframecnt;
	}

	odml->vidsheader.strh_v.dwLength = odml->vframecnt;
	if(odml->vidsheader.avih.dwStreams == 0x02)
		odml->audsheader.strh_a.dwLength = odml->aframecnt*odml->audsSize/2;
	else
		odml->audsheader.strh_a.dwLength  = 0;
	/********************super indx********************/

	odml->vidsheader.vsupindxh.nEntriesInUse = odml->vidssupercount;
	odml->audsheader.asupindxh.nEntriesInUse = odml->audssupercount;

	odml->moviheader.strodml.dwTotalFrames = odml->vframecnt + odml->audsheader.strh_a.dwLength;

    odml->moviheader.list_movi.fcc = RIFF_LIST;
	if(odml->AVIXnum > -1)
		 odml->moviheader.list_movi.cb= odml->moviofest[0] -AVI_ODML_HEADER_SIZE+4;
	else
	     odml->moviheader.list_movi.cb = offset  -AVI_ODML_HEADER_SIZE+4; //銆恎_stcJpegInfo.dwChunkLen銆?AVI_HEAD_SIZE+4+8;		// datalen+ "move" + index first 8bytes nouse
	
	 odml->moviheader.list_movi.fcctype = RIFF_movi;	

	




	 #if 0
	  int sector,csize;	 
	 deg_Printf("headersector:%d\n",odml->vidsheadersector);
//------vids heander	 
     if(hal_sdWrite(&odml->vidsheader,odml->vidsheadersector,1)<0)
             return AVI_ERR_WRITE;
	 csize = fs_getclustersize()>>9;
//------vids indx	 
	 offset = odml->vidsindxsector-odml->vidsheadersector;
	 sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
     sector+=offset&(csize-1);
#if ODML_CFG_SYNC_EN>0
     if(hal_sdWrite(&odml->vidsindxtable,sector,1)<0)//odml->vidsindxsector,1)<0)
             return AVI_ERR_WRITE;
#endif
#if ODML_CFG_SYNC_EN ==0
	if(hal_sdWrite(&odml->vidsindxtable,sector,AVI_INDX_LEN*AVI_ODML_INDX_NUM_ALIGN/512)<0)//odml->vidsindxsector,1)<0)
             return AVI_ERR_WRITE;
#endif
//------vprp header    //write this header for the continuity of sdcard writing
	offset = odml->vprpheadersector-odml->vidsheadersector;
	sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
	sector+=offset&(csize-1);
	 if(hal_sdWrite(&odml->vprpheader,sector,1)<0)//odml->audsheadersector,1)<0)
				 return AVI_ERR_WRITE;
//------auds header	 
	if(odml->vidsheader.avih.dwStreams == 0x02)
	{
	 offset = odml->audsheadersector-odml->vidsheadersector;
	 sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
     sector+=offset&(csize-1);
	 if(hal_sdWrite(&odml->audsheader,sector,1)<0)//odml->audsheadersector,1)<0)
             return AVI_ERR_WRITE;
//------auds indx	 
	 offset = odml->audsindxsector-odml->vidsheadersector;
	 sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
     sector+=offset&(csize-1);
#if ODML_CFG_SYNC_EN >0
	 if(hal_sdWrite(&odml->audsindxtable,sector,1)<0)//odml->audsindxsector,1)<0)
             return AVI_ERR_WRITE;
#endif
#if ODML_CFG_SYNC_EN ==0
		 if(hal_sdWrite(&odml->audsindxtable,sector,AVI_INDX_LEN*AVI_ODML_INDX_NUM_ALIGN/512)<0)//odml->audsindxsector,1)<0)
             return AVI_ERR_WRITE;
#endif
	}
//------movi header	 
	 offset = odml->movisector-odml->vidsheadersector;
	 sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
     sector+=offset&(csize-1);
	 if(hal_sdWrite(&odml->moviheader,sector,1)<0)//odml->movisector,1)<0)
             return AVI_ERR_WRITE;
			 
	 offset = odml->movisector-odml->vidsheadersector+1;
	 sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
     sector+=offset&(csize-1);
	 if((odmlEncoder->alignsize))
	 {
		temp1 = (INT32U *)audiobuff;
		temp1[0]=0x4B4E554A;
		temp1[1]=(64-odmlEncoder->alignsize)*512-8;
		size=(64-odmlEncoder->alignsize);
		 if(hal_sdWrite(audiobuff,sector,size)<0)//odml->movisector,1)<0)
             return AVI_ERR_WRITE;
	 }
#else
	// ------vids heander
	odmlEncoder->seek(odmlEncoder->fhandle,0,0);
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->vidsheader,512)<0)
		return AVI_ERR_WRITE;
	//------vids indx
	odmlEncoder->seek(odmlEncoder->fhandle,(odml->vidsindxsector-odml->vidsheadersector)*512,0);
	#if ODML_CFG_SYNC_EN ==0
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->vidsindxtable,AVI_INDX_LEN*AVI_ODML_INDX_NUM_ALIGN)<0)
		return AVI_ERR_WRITE;
	#else
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->vidsindxtable,512)<0)
		return AVI_ERR_WRITE;
	#endif

	//------vprp header
	odmlEncoder->seek(odmlEncoder->fhandle,(odml->vprpheadersector-odml->vidsheadersector)*512,0);
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->vprpheader,512)<0)
		return AVI_ERR_WRITE;

	//----auds header	
	odmlEncoder->seek(odmlEncoder->fhandle,(odml->audsheadersector-odml->vidsheadersector)*512,0);
		if(odmlEncoder->write(odmlEncoder->fhandle,&odml->audsheader,512)<0)
			return AVI_ERR_WRITE;

	//------auds indx
	odmlEncoder->seek(odmlEncoder->fhandle,(odml->audsindxsector-odml->vidsheadersector)*512,0);
	#if ODML_CFG_SYNC_EN ==0
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->audsindxtable,AVI_INDX_LEN*AVI_ODML_INDX_NUM_ALIGN)<0)
		return AVI_ERR_WRITE;
	#else
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->audsindxtable,512)<0)
		return AVI_ERR_WRITE;
	#endif

	//----moive header	
	odmlEncoder->seek(odmlEncoder->fhandle,(odml->movisector-odml->vidsheadersector)*512,0);
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->moviheader,512)<0)
		return AVI_ERR_WRITE;
	
	 ////索引越界。<32K.
	 //------movi header +1
	 //offset = odml->movisector-odml->vidsheadersector+1;
	 ///sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
	 //sector+=offset&(csize-1);
	 if((odmlEncoder->alignsize))
	 {
		temp1 = (INT32U *)audiobuff;
		temp1[0]=0x4B4E554A;
		temp1[1]=(64-odmlEncoder->alignsize)*512-8;
		size=(64-odmlEncoder->alignsize);

		odmlEncoder->seek(odmlEncoder->fhandle,(odml->movisector-odml->vidsheadersector+1)*512,0);
		if(odmlEncoder->write(odmlEncoder->fhandle,audiobuff,size*512)<0)
             return AVI_ERR_WRITE;
	 }
#endif
	 return AVI_ERR_NONE;
}

static int avi_odmlSuperIXWrite(void *encoder,INT8U type,ODML_OP_T *odml,INT32U ixCnt)
{
	AVI_IX_HEADER_T *idxHeader;
	INT32U size,wsize;
	INT32U *tempmem;
	INT8U *temp;
	ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
    if(ixCnt == 0)
		return AVI_ERR_NONE;
	if(AVI_FRAME_DC_VIDEO == type)
		idxHeader = (AVI_IX_HEADER_T *)odml->vidsmemPool;
	else if(AVI_FRAME_WD_AUDIO == type)
		idxHeader = (AVI_IX_HEADER_T *)odml->audsmemPool;
	else
		return AVI_ERR_GENERAL;
		
	idxHeader->wLongsPerEntry = 2;
	idxHeader->bIndexSubType=0;
	idxHeader->qwBaseOffset = odml->movioffset;
//	idxHeader->dwReserved3= 0;
	idxHeader->bIndexType= AVI_INDEX_OF_CHUNKS;
	idxHeader->cb = AVI_IX_HEADER_LEN-8+AVI_IX_LEN*ixCnt;
	
	idxHeader->nEntriesInUse = ixCnt;

	size = AVI_IX_LEN*ixCnt+AVI_IX_HEADER_LEN;
	if(size&0x1ff) 
	{
		wsize = size&0x1ff;
		temp = (INT8U *)idxHeader;
		temp+=size;
		memset((void *)temp,0,512-wsize);
		tempmem = (INT32U *)temp;
		tempmem[0] = RIFF_JUNK;
		tempmem[1] = 0x200-wsize-8;		
		size = 0x200+(size&(~0x1ff));
	}
	if(odmlEncoder->write(odmlEncoder->fhandle,idxHeader,size)<0)
		return AVI_ERR_WRITE;


	if(AVI_FRAME_DC_VIDEO == type)
	{
	#if ODML_CFG_SYNC_EN == 0
		odml->vidsindxtable[odml->vidssupercount].qwOffset = odml->qwOffset;
		odml->vidsindxtable[odml->vidssupercount].dwSize = AVI_IX_LEN*odml->vidsixcount+AVI_IX_HEADER_LEN;
        odml->vidsindxtable[odml->vidssupercount].dwDuration = odml->vidsixcount;
	#endif
	#if ODML_CFG_SYNC_EN >0
		if(odml->vidsindxcount>=ODML_CFG_INDX_NUM) // need update file
		{
	/* 	#if ODML_CFG_SYNC_EN == 0
		   INT32U offset,sector,csize;
			//debg("SYNC vids indx\n");
            csize = fs_getclustersize()>>9;
		    offset = odml->vidsindxsector-odml->vidsheadersector;
		    sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
	        sector+=offset&(csize-1);
	        if(hal_sdWrite(&odml->vidsindxtable,sector,1)<0)//odml->vidsindxsector,1)<0)
	             return AVI_ERR_WRITE;
			
		#endif */
	
			odml->vidsindxcount = 0;
			memset(odml->vidsindxtable,0,ODML_CFG_INDX_NUM*AVI_INDX_LEN);
			odml->vidsindxsector++;
		}
		odml->vidsindxtable[odml->vidsindxcount].qwOffset = odml->qwOffset;
		odml->vidsindxtable[odml->vidsindxcount].dwSize = AVI_IX_LEN*odml->vidsixcount+AVI_IX_HEADER_LEN;
        odml->vidsindxtable[odml->vidsindxcount].dwDuration = odml->vidsixcount;
        odml->vidsindxcount++;
	#endif
        odml->vidsixcount = 0;
	
		odml->vidssupercount++;
		if(odml->vidssupercount == (AVI_ODML_INDX_NUM-1))
              return AVI_ERR_GENERAL; 
	}
    else if(AVI_FRAME_WD_AUDIO == type)
    {
	#if ODML_CFG_SYNC_EN == 0
		odml->audsindxtable[odml->audssupercount].qwOffset = odml->qwOffset;
		odml->audsindxtable[odml->audssupercount].dwSize = AVI_IX_LEN*odml->audsixcount+AVI_IX_HEADER_LEN;
        odml->audsindxtable[odml->audssupercount].dwDuration = odml->audsixcount*odml->audsSize/2;
	#endif
	#if ODML_CFG_SYNC_EN>0
		if(odml->audsindxcount>=ODML_CFG_INDX_NUM) // need update file
		{
	/*	#if ODML_CFG_SYNC_EN == 0
		   INT32U offset,sector,csize;
		   csize = fs_getclustersize()>>9;
		   //debg("SYNC auds indx\n");
		   offset = odml->audsindxsector-odml->vidsheadersector;
	       sector = fs_getsector(odmlEncoder->fhandle,odml->headercluster[offset/csize],1);
           sector+=offset&(csize-1);
	       if(hal_sdWrite(&odml->audsindxtable,sector,1)<0)//odml->audsindxsector,1)<0)
             return AVI_ERR_WRITE;
		#endif	*/
			odml->audsindxcount = 0;
			memset(odml->audsindxtable,0,ODML_CFG_INDX_NUM*AVI_INDX_LEN);
			odml->audsindxsector++;
		}
		odml->audsindxtable[odml->audsindxcount].qwOffset = odml->qwOffset;
		odml->audsindxtable[odml->audsindxcount].dwSize = AVI_IX_LEN*odml->audsixcount+AVI_IX_HEADER_LEN;
        odml->audsindxtable[odml->audsindxcount].dwDuration = odml->audsixcount*odml->audsSize/2;
        odml->audsindxcount++;
	#endif
        odml->audsixcount = 0;
		odml->audssupercount++;
		if(odml->audssupercount == (AVI_ODML_INDX_NUM-1))
              return AVI_ERR_GENERAL;
    }
    odml->qwOffset+=size;
    odmlEncoder->ofset+=size;
	
	return size;
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
static int avi_odmlAddIndex(void *encoder,INT8U *buff,INT8U type,INT8U keyFrame,INT32U size,INT32U buflen)
{
    ODML_OP_T *odml;
	int ret;
    ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	ODML_CHECK(odmlEncoder);
	odml = (ODML_OP_T *)odmlEncoder->prit;
	
	if(type == AVI_FRAME_DC_VIDEO || type == AVI_FRAME_DB_VIDEO)
	{
	//--------add index in ix phase	
		if(size == 0)  // inserted frame
		{
			if(odmlEncoder->oldOffset == 0)
				return AVI_ERR_VFRAME;
	        odml->vidsixtable[odml->vidsixcount].dwOffset = odmlEncoder->oldOffset+8;
			odml->vidsixtable[odml->vidsixcount].dwSize = odmlEncoder->oldSize-8;
		}
		else // real frame
		{
			odml->vidsixtable[odml->vidsixcount].dwOffset = odmlEncoder->ofset+8;
			odml->vidsixtable[odml->vidsixcount].dwSize = size-8;
        // save this value
			odmlEncoder->oldOffset = odmlEncoder->ofset;
			odmlEncoder->oldSize= size;
			odml->qwOffset+=size;
			odmlEncoder->ofset+=size;
		}
	/*	if(keyFrame == 0)
			odml->vidsixtable[odml->vidsixcount].dwSize |= 1<<31;
	*/		 
		odml->vidsixcount++;
		odmlEncoder->framecnt++;
		odml->vframecnt++;
		if(odml->vidsixcount == AVI_ODML_IX_NUM)
		{
			ret = avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_DC_VIDEO,odml,AVI_ODML_IX_NUM);
			if(ret<0)
				return ret;
			if(odml->audsixcount)
			{
				ret = avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_WD_AUDIO,odml,odml->audsixcount);
				if(ret<0)
					return ret;
			}
			#if ODML_CFG_SYNC_EN > 0	
			    if(((odml->vidsindxcount%ODML_CFG_INDX_SYNC)==0)||((odml->audsindxcount%ODML_CFG_INDX_SYNC)==0))
			    {
					// deg_Printf("odml sync video.%d,%d\n",odml->vidsindxcount,odml->audsindxcount);
					avi_odmlSync(odmlEncoder);
			    }
			#endif
		}
	}
	else if(type == AVI_FRAME_WD_AUDIO)
	{
	//--------add index in ix phase	
	    if(odml->audsSize ==0)
			odml->audsSize = size-8;
		odml->audsixtable[odml->audsixcount].dwOffset = odmlEncoder->ofset+8;
		odml->audsixtable[odml->audsixcount].dwSize = size-8;
		odml->audsixcount++;	
		odml->qwOffset+=size;
		odmlEncoder->ofset+=size;
		odml->aframecnt++;
		if(odml->audsixcount == AVI_ODML_IX_NUM)
		{
			ret = avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_WD_AUDIO,odml,AVI_ODML_IX_NUM);
			if(ret<0)
				return ret;
			if(odml->vidsixcount)
			{
				ret = avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_DC_VIDEO,odml,odml->vidsixcount);
				if(ret<0)
					return ret;
			}
			#if ODML_CFG_SYNC_EN > 0	
				if(((odml->vidsindxcount%ODML_CFG_INDX_SYNC)==0)||((odml->audsindxcount%ODML_CFG_INDX_SYNC)==0))
				{
					// deg_Printf("odml sync audio.%d,%d\n",odml->vidsindxcount,odml->audsindxcount);
					avi_odmlSync(odmlEncoder);
			    }
			#endif
		}
	}
	else
		return -1;
	
//	odmlEncoder->write(NULL,-1,0);
    return AVI_ERR_NONE;
}
static int avi_odmlAddAVIX(void *encoder)
{
	ODML_OP_T *odml;
    ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	ODML_CHECK(odmlEncoder);
	odml = (ODML_OP_T *)odmlEncoder->prit;

	odml->AVIXnum++;
	if(odml->AVIXnum>=ODML_CFG_AVIX_MAX)
	{
		odml->AVIXnum = 0;
		// return error?????
	}
	odml->movlist[odml->AVIXnum].riff.fcc = RIFF_TAG;
	odml->movlist[odml->AVIXnum].riff.cb = 0;
	odml->movlist[odml->AVIXnum].riff.fcctype = RIFF_AVIX;

	odml->movlist[odml->AVIXnum].movi.fcc = RIFF_LIST;
	odml->movlist[odml->AVIXnum].movi.cb = 0;
	odml->movlist[odml->AVIXnum].movi.fcctype = RIFF_movi;

	odml->avix_sector[odml->AVIXnum] = fs_getsector(odmlEncoder->fhandle,odml->qwOffset,-1); // sector address
    //deg_Printf("add avix %x,%x",odml->avix_sector[odml->AVIXnum],odml->qwOffset);
	memset((void *)&odml->avix_1sect,0,sizeof(odml->avix_1sect));
	memcpy((void *)&odml->avix_1sect.Avix,&odml->movlist[odml->AVIXnum],sizeof(AVIXMOVILIST));

	odml->avix_1sect.junk.dwFourCC = RIFF_JUNK;
	odml->avix_1sect.junk.dwSize = 512-sizeof(AVIXMOVILIST)-8;
	if(odmlEncoder->write(odmlEncoder->fhandle,&odml->avix_1sect,512)<0)
		return AVI_ERR_WRITE;
	odml->moviofest[odml->AVIXnum] = odml->qwOffset;
	//deg_Printf("offset = %x\n", odml->qwOffset);
    odmlEncoder->ofset = 512-20;  // reset movi offet
    odmlEncoder->oldOffset = 0;
	odml->movioffset = odml->qwOffset+20;//odmlEncoder->ofset;
	//odml->qwOffset = offset+20;
	if(odml->AVIXnum>0) // the last time,update last avix
	{
		odml->movlist[odml->AVIXnum-1].riff.cb = odml->qwOffset-odml->moviofest[odml->AVIXnum-1]-8;
		odml->movlist[odml->AVIXnum-1].movi.cb = odml->qwOffset-odml->moviofest[odml->AVIXnum-1]-20;

      // deg_Printf("sync %x=%x-%x,%d\n",odml->movlist[odml->AVIXnum-1].riff.cb,odml->qwOffset,odml->moviofest[odml->AVIXnum-1],odml->AVIXnum-1);
		memset((void*)&odml->avix_1sect,0,sizeof(odml->avix_1sect));
		memcpy((void*)&odml->avix_1sect.Avix,&odml->movlist[odml->AVIXnum-1], sizeof(AVIXMOVILIST));
        
		odml->avix_1sect.junk.dwFourCC= RIFF_JUNK;//junk
		odml->avix_1sect.junk.dwSize=512-sizeof(AVIXMOVILIST)-8;
		if(hal_sdWrite(&odml->avix_1sect,odml->avix_sector[odml->AVIXnum-1],1)<0)
             return AVI_ERR_WRITE;

	}
    odml->qwOffset+=512;
	return AVI_ERR_NONE;
}
static int avi_odmlEnd(void *encoder,INT8U *buffer,INT32U fsize,INT32U idx_offset)
{
	ODML_OP_T *odml;
    ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	ODML_CHECK(odmlEncoder);
	odml = (ODML_OP_T *)odmlEncoder->prit;
	s32 alignsize,size;
	INT32U *temp;

	if(odml->vidsixcount)
		avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_DC_VIDEO,odml,odml->vidsixcount);
    if(odml->audsixcount)
		avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_WD_AUDIO,odml,odml->audsixcount);
		
	alignsize=hal_sdNextLBA()&(0x3f);
	size=0;
	if(alignsize)
	{
		temp = (INT32U *)audiobuff;
		temp[0]=0x4B4E554A;
		temp[1]=(64-alignsize)*512-8;
		size=(64-alignsize)*512;
		if(odmlEncoder->write(odmlEncoder->fhandle,audiobuff,size)<0)
			return AVI_ERR_WRITE;
	}
	odml->qwOffset+=size;
	odmlEncoder->ofset+=size;
	avi_odmlSync(odmlEncoder);


	return AVI_ERR_NONE;
}
static int avi_odmlService(void *encoder)
{
	ODML_OP_T *odml;
    ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	ODML_CHECK(odmlEncoder);
	odml = (ODML_OP_T *)odmlEncoder->prit;
	
	if((odml->qwOffset- odml->lastAVIXOffset)>ODML_CFG_AVIX_LEN) // new avi phase
	{
		odml->lastAVIXOffset = odml->qwOffset;
		
     // update last avix indx    
        if(odml->vidsixcount)  
			avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_DC_VIDEO,odml,odml->vidsixcount);
	    if(odml->audsixcount)
			avi_odmlSuperIXWrite(odmlEncoder,AVI_FRAME_WD_AUDIO,odml,odml->audsixcount);
		
		return avi_odmlAddAVIX(odmlEncoder);
	}

	return AVI_ERR_NONE;
}

int avi_odmlAddoffset(void *encoder,INT32U offset)
{
    ODML_OP_T *odml;
    ENCODER_T *odmlEncoder = (ENCODER_T *)encoder;
	ODML_CHECK(odmlEncoder);
	odml = (ODML_OP_T *)odmlEncoder->prit;
	
	odml->qwOffset+=offset;
	odmlEncoder->ofset+=offset;
	return AVI_ERR_NONE;
}







AVI_ENCODER_OP_T avi_encoder_odml=
{
	avi_odmlInit,
	avi_odmlUninit,
	avi_odmlParse,
	avi_odmlAddIndex,
	avi_odmlEnd,
	avi_odmlService,
};





