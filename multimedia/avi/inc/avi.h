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
* File Name   : avi.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is avi file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  AVI_H
#define  AVI_H


#include "avi_error.h"


#define  AVI_VIDS_CACHE_NUM     3
#define  AVI_AUDS_CACHE_NUM     3



#define RIFF_TAG     0x46464952
#define RIFF_JUNK    0x4B4E554A
#define RIFF_LIST    0x5453494c
#define RIFF_AVI     0x20495641
#define RIFF_avih    0x68697661
#define RIFF_hdrl    0x6C726468
#define RIFF_movi    0x69766F6D
#define RIFF_idx1    0x31786469
#define RIFF_strl    0x6C727473
#define RIFF_strh    0x68727473
#define RIFF_strf    0x66727473
#define RIFF_00dc    0x63643030
#define RIFF_01wb    0x62773130
#define RIFF_00DB    0x62643030
#define RIFF_vids    0x73646976
#define RIFF_auds    0x73647561
#define RIFF_strd    0x64727473
#define RIFF_mjpg    0x67706A6D
#define RIFF_MJPG    0x47504A4D
#define RIFF_indx    0x78646e69
#define RIFF_vprp    0x70727076
#define RIFF_odml    0x6c6d646f
#define RIFF_dmlh    0x686c6d64
#define RIFF_ix01    0x31307869
#define RIFF_ix00    0x30307869
#define RIFF_AVIX    0X58495641

#define AVI_KEY_FRAME    0x00000010

#define AVIF_HASINDEX 			0x10
#define AVIF_MUSTUSEINDEX  		0x20
#define AVIF_ISINTERLEAVED		0x100
#define AVIF_WASCAPTUREFILE	0x10000
#define AVIF_COPYRIGHTED		0x20000
#define AVI_KNOWN_FLAGS 		0x30130



typedef struct 
{
	DWORD ckid;
	DWORD dwFlags;
	DWORD dwChunkOffset;
	DWORD dwChunkLength;
} AVIINDEXENTRY;

typedef struct 
{
	DWORD  fcc;//avih
	DWORD  cb;//size
	DWORD  dwMicroSecPerFrame;
	DWORD  dwMaxBytesPerSec;
	DWORD  dwPaddingGranularity;
	DWORD  dwFlags;
	DWORD  dwTotalFrames;
	DWORD  dwInitialFrames;
	DWORD  dwStreams;
	DWORD  dwSuggestedBufferSize;
	DWORD  dwWidth;
	DWORD  dwHeight;
	DWORD  dwReserved[4];
} AVIMAINHEADER;

typedef struct 
{
	DWORD  fcc;//4//strh
	DWORD  cb;//4//size
	DWORD  fccType;//4//vids|auds
	DWORD  fccHandler;//4
	DWORD  dwFlags;//4
	WORD  wPriority;//2
	WORD  wLanguage;//2
	DWORD dwInitialFrames;//4
	DWORD dwScale;//4
	DWORD dwRate; //4/* dwRate / dwScale == samples/second */
	DWORD dwStart;//4
	DWORD dwLength; //4/* In units above... */
	DWORD dwSuggestedBufferSize;//4
	DWORD dwQuality;//4
	DWORD dwSampleSize;//4
    struct 
	{
		short int left;//
		short int top;//
		short int right;//
		short int bottom;//
    } rcFrame;

} AVISTREAMHEADER;

typedef struct tagBITMAPINFOHEADER 
{
	DWORD biSize;
	DWORD  biWidth;//4
	DWORD  biHeight;//4
	WORD	biPlanes;
	WORD	biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	DWORD  biXPelsPerMeter;
	DWORD  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct 
{
	WORD	wFormatTag;
	WORD	nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD	nBlockAlign;
	WORD	wBitsPerSample;
	WORD	cbSize;
} WAVEFORMATEX;


typedef struct 
{
	DWORD dwFourCC;
	DWORD dwSize;
} CHUNK;

typedef struct
{ 
	DWORD fcc;
	DWORD cb;
	DWORD fcctype;
} LIST;
typedef struct
{ 
	u32 rv[34];
}BITMAPAUDIODC;
typedef struct
{ 
	u32 rv[2];
}WAVEDECPR;


typedef struct 
{
	LIST riff;				//{"RIFF",dwsize,"AVI "};
	LIST hdrl;			//{"LIST",dwsize,"hdr1"};
	AVIMAINHEADER avih; //aviheader
	LIST strl_v;						//strl vids
	AVISTREAMHEADER strh_v; 		//stream vids
	CHUNK strf_v;					//vids stream info
	BITMAPINFOHEADER bitmapinfo;
	CHUNK strd_v;	
	BITMAPAUDIODC	bitmapvideodc;

	LIST strl_a;						//strl auds
	AVISTREAMHEADER strh_a; 		//stream auds
	CHUNK strf_a;					//auds stream info
	WAVEFORMATEX  wavinfo;	
	CHUNK strd_a;	
	WAVEDECPR	wavedcpr;
	CHUNK junk; 
}AVIFILEHEADER;
typedef	struct
{
	u32  rsv0;
	u32  rsv1;
	u32	 fcc;
	u32  cb;
}IDX_H;
typedef	struct
{
	u32  dwchunkid;
	u32  dwflags;
	u32	 dwoffset;
	u32  dwsize;
}IDX_A;


typedef struct AVI_ENCODER_OP_S
{
	int (*init)(void *encoder);
	int (*uninit)(void *encoder);
	int (*parse)(void *encoder,INT16U width,INT16U height,INT16U fps,INT16U audio,INT16U samperate);
	int (*encode)(void *encoder,INT8U *buff,INT8U type,INT8U keyFrame,INT32U size,INT32U buflen);
	int (*end)(void *encoder,INT8U *buffer,INT32U fsize,INT32U idx_offset);
	int (*service)(void *encoder);
	
}AVI_ENCODER_OP_T;

typedef struct ENCODER_S
{
	INT8U type;
	INT8U status;
	INT8U state;
	INT8U rev2;
	
	int fhandle;
	
	INT32U ofset;		//avi jpgÆ«ÒÆ
	INT32U size;		//avi jpg´óÐ¡
	INT32U dwIdexlen;	//avi Ë÷Òý
	INT32U framecnt;	//Ö¡¼ÆÊý

	INT32U oldOffset;
	INT32U oldSize;
	INT32S alignsize;

    AVI_ENCODER_OP_T *encoder;
	int  (*write)(int fd,void *cachebuffer,INT32U length);
	int  (*seek)(int fd,INT32U pos,int flag);

	void *prit;
}ENCODER_T;




typedef struct AVI_DECODER_OP_S
{
	int (*init)(void *hanlder);
	int (*decode)(INT32U *offset,INT32U *length,INT32U type);
	int (*gettime)(INT32U *ttime,INT32U *ctime);
	int (*dfast)(int deta);
	int (*service)(void);
	
}AVI_DECODER_OP_T;

typedef struct DECODE_Q_S
{
	INT8U in;
	INT8U out;
	INT8U cnt;
	INT8U busy;
	AVIINDEXENTRY *stack;
	
}DECODE_Q_T;

typedef struct DECODEER_S
{
	INT8S status;
	INT8S type;   // normal avi/open dml
	INT16S framestep;
	INT16U width; 
	INT16U height;	

	int fhandle;
	
	INT32S frameIdx;
	INT32S videoIdx;

	INT32U aviflag;

	INT32U framecnt;
	INT32U frametime;  // us

	INT32U auds_samplerate;
//---open dml
	INT32U vids_entrys;
	INT32U vids_offset;
	INT32U auds_entrys;
	INT32U auds_offset;
	INT32S vids_index;
	INT32S auds_index;
	INT32U vids_count;
	INT32U auds_count;
	INT32U vids_movioffset;
	INT32U auds_movioffset;
//----	

//----standard avi
	INT32U idx1_offset;
	INT32U idx1_length;
//------	

	INT32U movi_offset;
    INT32U movi_size;
	INT32U idx1_cache_offset_last;
	s32 idx1_cache_offset;
	INT32U idx1_cache_length;
	INT32U idx1_cache_cnt;
	INT32U *idx1_cache;
	int  (*read)(void *cachebuffer,INT32U offset,INT32U length);

	INT32S audiolframe;
	INT32S videolframe;

	AVIINDEXENTRY idx1;
	
    AVIINDEXENTRY vidsIdx1Stack[AVI_VIDS_CACHE_NUM];	
	AVIINDEXENTRY audsIdx1Stack[AVI_AUDS_CACHE_NUM];
	DECODE_Q_T vidsIdx1Q;
	DECODE_Q_T audsIdx1Q;
	AVI_DECODER_OP_T *decoder;

	void *prit;
	u32 dir;	// 1:forword next index , 2: backword pre index
}DECODEER_T;
#include "avi_std.h"
#include "avi_odml.h"
#include "avi_api.h"



void aviDecodeIdx1QInit(DECODE_Q_T *idxq,AVIINDEXENTRY *table,int len);
void aviDecodeIdx1Cpy(AVIINDEXENTRY *dst,AVIINDEXENTRY*src);
void aviDecodeIdx1QIn(DECODE_Q_T *idxq,AVIINDEXENTRY *idx1);
void aviDecodeIdx1QOut(DECODE_Q_T *idxq,AVIINDEXENTRY *idx1);

int avi_odmlAddoffset(void *encoder,INT32U offset);




#endif
