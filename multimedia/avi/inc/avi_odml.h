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
* File Name   : avi_odml.h
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
#ifndef  AVI_ODML_H
     #define   AVI_ODML_H

	 
#define AVI_INDEX_OF_INDEXES       0x00
#define AVI_INDEX_OF_CHUNKS        0x01
#define AVI_INDEX_OF_TIMED_CHUNKS  0x02
#define AVI_INDEX_OF_SUB_2FIELD    0x03
#define AVI_INDEX_IS_DATA          0x80
#define FIL_SIZE_4G  			0XF85A2000
#define FIL_SIZE_3G				0XDC758000
#define FIL_SIZE_2G				0X7D000000
#define FIL_SIZE_1G				0X3E800000



#define ODML_CFG_FILE_MAX		    FIL_SIZE_2G//FIL_SIZE_4G             //size of one file
#define ODML_CFG_FRAME_SIZE      40*1024				// Probably size every frame(720p)
#define ODML_CFG_SYNC_EN 			0    // enable sync
#define ODML_CFG_SYNC_TIME 		2	 // second , this value should let ODML_CFG_INDX_NUM%ODML_CFG_INDX_SYNC == 0
#define ODML_CFG_AVIX_MAX         6	// AVIX NUM  
#define ODML_CFG_AVIX_LEN         (1024*1024*900)	
#define ODML_CFG_INDX_NUM         32  // 32*16 512 byte for one sector
#define ODML_CFG_INDX_SYNC        (ODML_CFG_SYNC_TIME/2)
#define ODML_CFG_CLUS_NUM         (16*8)   // cluster number for hander sector finding
/*************************************************/
#define AVI_ODML_SIDX_LEN          (ODML_CFG_FILE_MAX / (2*30*ODML_CFG_FRAME_SIZE))
#define AVI_ODML_INDX_NUM           AVI_ODML_SIDX_LEN				//buff len   
#define AVI_ODML_INDX_NUM_ALIGN		(((AVI_ODML_SIDX_LEN+32-1)/32)*32)   
#define AVI_ODML_HEADER_SIZE      (((2048 + AVI_ODML_SIDX_LEN*16*2)/1024 +1)*1024)
#define AVI_ODML_HEADER_ALIGNSIZE  ((AVI_ODML_HEADER_SIZE+(0x1FFF)-1)&(~0x1FFF))//((AVI_ODML_HEADER_SIZE+(0x7FFF)-1)&(~0x7FFF))

				 
typedef struct AVI_IX_HEADER_S 
{
	 DWORD fcc; // Â¡Â¯ix##Â¡Â¯
	 DWORD cb;
	 WORD wLongsPerEntry; // must be sizeof(aIndex[0])/sizeof(DWORD)
	 BYTE bIndexSubType; // must be 0
	 BYTE bIndexType; // must be AVI_INDEX_OF_CHUNKS
	 DWORD nEntriesInUse; //
	 DWORD dwChunkId; // Â¡Â¯##dcÂ¡Â¯ or Â¡Â¯##dbÂ¡Â¯ or Â¡Â¯##wbÂ¡Â¯ etc..
	 UINT64 qwBaseOffset; // all dwOffsets in aIndex array are
	 // relative to this
	 DWORD dwReserved3; // must be 0
} AVI_IX_HEADER_T;
#define AVI_IX_HEADER_LEN      32 // sizeof(AVI_IX_HEADER_T)    //   32-byte
	 
typedef struct AVI_IX_S 
{
	 DWORD dwOffset; // qwBaseOffset + this is absolute file offset
	 DWORD dwSize; // bit 31 is set if this is NOT a keyframe
} AVI_IX_T;
#define  AVI_IX_LEN                    sizeof(AVI_IX_T) // 8
#define AVI_ODML_IX_NUM             ((512-AVI_IX_HEADER_LEN)/AVI_IX_LEN)   		 
		 
typedef struct AVI_INDX_S 
{
	 UINT64 qwOffset;	 // absolute file offset
	 DWORD dwSize;		 // size of index chunk at this offset include stdindx head
	 DWORD dwDuration;	 // time span in stream ticks
} AVI_INDX_T;
#define  AVI_INDX_LEN     sizeof(AVI_INDX_T) // 16

typedef struct _avisuperindex_chunk 
{
	 
	 WORD wLongsPerEntry; // must be 4 (size of each entry in aIndex array)
	 BYTE bIndexSubType; // must be 0 or AVI_INDEX_2FIELD
	 BYTE bIndexType; // must be AVI_INDEX_OF_INDEXES
	 DWORD nEntriesInUse; // number of entries in aIndex array that
	 // are used
	 DWORD dwChunkId; // Â¡Â¯##dcÂ¡Â¯ or Â¡Â¯##dbÂ¡Â¯ or Â¡Â¯##wbÂ¡Â¯, etc
	 DWORD dwReserved[3]; // must be 0
} AVISUPERINDEXHEAD;
	 
	 
typedef struct 
{
	 DWORD CompressedBMHeight;
	 DWORD CompressedBMWidth;
	 DWORD ValidBMHeight;
	 DWORD ValidBMWidth;
	 DWORD ValidBMXOffset;
	 DWORD ValidBMYOffset;
	 DWORD VideoXOffsetInT;
	 DWORD VideoYValidStartLine;
} VIDEO_FIELD_DESC;
typedef struct 
{
	 
	 DWORD VideoFormatToken;
	 DWORD VideoStandard;
	 DWORD dwVerticalRefreshRate;
	 DWORD dwHTotalInT;
	 DWORD dwVTotalInLines;
	 DWORD dwFrameAspectRatio; 
	 DWORD dwFrameWidthInPixels;
	 DWORD dwFrameHeightInLines;
	 DWORD nbFieldPerFrame;
	 VIDEO_FIELD_DESC FieldInfo;
} VideoPropHeader;
	 
typedef struct 
{
	 DWORD OpenDML_Header;
	 DWORD size;
	 DWORD dwTotalFrames; //video total frame + auds frame
	 DWORD	 dwFuture[61];
} ODMLExtendedAVIHeader;
	 
	 
typedef struct 
{
	 LIST riff; 			 //{"RIFF",dwsize,"AVIX "};
	 LIST movi; 		 //{"LIST",dwsize,"movi"};
} AVIXMOVILIST;
	 
	 
	 
typedef struct 
{	 
	 LIST riff; 			 //{"RIFF",dwsize,"AVI "};
	 LIST hdrl; 		 //{"LIST",dwsize,"hdr1"};
	 AVIMAINHEADER avih; //aviheader
	 
	 LIST strl_v;						 //strl vids
	 AVISTREAMHEADER strh_v;		 //stream vids
	 CHUNK strf_v;					 //vids stream info
	 BITMAPINFOHEADER bitmapinfo;
	 CHUNK sector1_junk; 
	 u8 sector1fill[259];
	 //CHUNK strd_v; 
	 //BITMAPAUDIODC bitmapvideodc;
	 CHUNK sidx_v;
	 AVISUPERINDEXHEAD vsupindxh;	 //superindx head for video
	 /**********sector 1*********************/
	 AVI_INDX_T vindx[AVI_ODML_SIDX_LEN];			 //super indx for video
	 CHUNK sector2_junk;
	 u8 sector2fill[512-(AVI_ODML_SIDX_LEN*16)%512-8];
	 /**********sector 2*********************/
	 CHUNK c_vprp;
	 VideoPropHeader vprp;			 //Video Properties Header
	 CHUNK sector3_junk;
	 u8 sector3fill[428];
	 /**********sector 3*********************/
	 LIST strl_a;						 //strl auds
	 AVISTREAMHEADER strh_a;		 //stream auds
	 CHUNK strf_a;					 //auds stream info
	 WAVEFORMATEX  wavinfo;  
	 CHUNK sector4_junk;
	 u8 sector4fill[367];
	 //CHUNK strd_a; 
	 //WAVEDECPR wavedcpr;
	 
	 CHUNK sidx_a;
	 AVISUPERINDEXHEAD asupindxh;	 //superindx head for auds
	 /**********sector 4*********************/
	 AVI_INDX_T aindx[AVI_ODML_SIDX_LEN];			 //super indx for auds
	 CHUNK sector5_junk;
	 u8 sector5fill[512-(AVI_ODML_SIDX_LEN*16)%512-8];
	 /**********sector 5*********************/
	 LIST obml;
	 ODMLExtendedAVIHeader strodml;  //Extended AVI Header (dmlh)
	 
	 CHUNK junk;
}ODMLAVIFILEHEADER;
#define  AVI_ODMLHEANDER_SIZE        (sizeof(ODMLAVIFILEHEADER))// 0xdb14	 
	 

typedef struct AVI_VIDS_S
{
	 LIST riff; 			 //{"RIFF",dwsize,"AVI "};
	 LIST hdrl; 		 //{"LIST",dwsize,"hdr1"};
	 AVIMAINHEADER avih; //aviheader
	 
	 LIST strl_v;						 //strl vids
	 AVISTREAMHEADER strh_v;		 //stream vids
	 CHUNK strf_v;					 //vids stream info
	 BITMAPINFOHEADER bitmapinfo;
	 CHUNK sector1_junk; 
	 u8 sector1fill[259];
	 //CHUNK strd_v; 
	 //BITMAPAUDIODC bitmapvideodc;
	 CHUNK sidx_v;
	 AVISUPERINDEXHEAD vsupindxh;	 //superindx head for video
}AVI_VIDS_T;
	 
typedef struct AVI_AUDS_S
{
	 LIST strl_a;						 //strl auds
	 AVISTREAMHEADER strh_a;		 //stream auds
	 CHUNK strf_a;					 //auds stream info
	 WAVEFORMATEX  wavinfo;  
	 CHUNK sector4_junk;
	 u8 sector4fill[367];
	 //CHUNK strd_a; 
	 //WAVEDECPR wavedcpr;
	 
	 CHUNK sidx_a;
	 AVISUPERINDEXHEAD asupindxh;	 //superindx head for auds
	 /**********sector 4*********************/
}AVI_AUDS_T;
	 
	 
typedef struct 
{
	AVIXMOVILIST Avix;
	CHUNK junk;
	u8 junkfill[512-sizeof(AVIXMOVILIST)-8];
}AVIX_SECTOR;
	 
typedef struct AVI_MOVI_S
{
	 LIST obml;
	 ODMLExtendedAVIHeader strodml;  //Extended AVI Header (dmlh)
	 CHUNK junk; 
	 u8 junkfill[AVI_ODML_HEADER_SIZE-AVI_ODMLHEANDER_SIZE-12];//- (sizeof(ODMLAVIFILEHEADER) + 12)];
	 LIST list_movi;
}AVI_MOVI_T;

typedef struct AVI_VPRP_S
{
	 CHUNK c_vprp;
	 VideoPropHeader vprp;			 //Video Properties Header
	 CHUNK sector3_junk;
	 u8 sector3fill[428];	
}AVI_VPRP_T;
	 
typedef struct  ODML_OP_S
{
//--------buffer aglin----------	

    AVI_VIDS_T vidsheader;  // vids's header
    AVI_AUDS_T audsheader;  // auds's header
	AVI_VPRP_T vprpheader;  //vprp's header
#if ODML_CFG_SYNC_EN>0
    AVI_INDX_T vidsindxtable[ODML_CFG_INDX_NUM]; // vids super indx table
    AVI_INDX_T audsindxtable[ODML_CFG_INDX_NUM]; // auds super indx table
#endif
#if ODML_CFG_SYNC_EN == 0
	AVI_INDX_T vidsindxtable[AVI_ODML_INDX_NUM]; // vids super indx table
	CHUNK sector2_junk;
	 u8 sector2fill[512-(AVI_ODML_SIDX_LEN*16)%512-8];
    AVI_INDX_T audsindxtable[AVI_ODML_INDX_NUM]; // auds super indx table
	CHUNK sector5_junk;
	 u8 sector5fill[512-(AVI_ODML_SIDX_LEN*16)%512-8];
#endif
	INT8U vidsmemPool[AVI_IX_HEADER_LEN+AVI_ODML_IX_NUM*AVI_IX_LEN];  //vids ix table mem,using 512 byte
	INT8U audsmemPool[AVI_IX_HEADER_LEN+AVI_ODML_IX_NUM*AVI_IX_LEN];  //auds ix table ,mem,using 512-byte

	AVI_MOVI_T moviheader; // movi's header

	AVIX_SECTOR avix_1sect;
//--------super indx phase	
	INT32U vidsheadersector; // vids's header saving sector in file
	INT32U audsheadersector; // vuds's header saving sector in file
	INT32U vprpheadersector;// vuds's header saving sector in file
	INT32U vidsindxcount;   // numbers of super indx in 512-byte of vids
	INT32U audsindxcount;   // numbers of super indx in 512-byte of auds
	INT32U vidsindxsector;  // sector address in sdc of vids indx
	INT32U audsindxsector;  // sector address in sdc of auds indx
	INT32U vidssupercount;  // numbers of super ix table of vids
	INT32U audssupercount;  // numbers of super ix table of auds
	INT32U headercluster[ODML_CFG_CLUS_NUM];
//---------ix phase	
   //buff for video and auds's std record the ""indx    

	 AVI_IX_T *vidsixtable;//vids ix table point
	 AVI_IX_T *audsixtable;//auds ix table point
	 INT32U vidsixcount; // numbers of ix in current ix table of vids
	 INT32U audsixcount; // numbers of ix in current ix table of auds
//---------movi phase

     INT32U movisector;  // movi's header svaing sector in file
     INT32U vframecnt; // video frame count total
	 INT32U aframecnt; // auds frame count total
	 INT32U movioffset; // current movi phase offset
//---------AVIX
     INT32U lastAVIXOffset;
     INT32U lastAVIXSize;
//     INT32U avilisframecnt; //AVI list total frame
     
	 INT32S AVIXnum; //init value is -1 ,when it is 0 that mean have 1 AVIX List
	 AVIXMOVILIST movlist[ODML_CFG_AVIX_MAX];
	 
	 INT32U moviofest[ODML_CFG_AVIX_MAX];  //64
	 INT32U avix_sector[ODML_CFG_AVIX_MAX];
//--------avi
     INT32U audsSize;  // 01wb size,only save the first chunk size,using in all the chunk 
	 INT32U qwOffset;  // file offset  //UINT64
} ODML_OP_T;







































#endif

