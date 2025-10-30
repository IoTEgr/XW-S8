/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                     MULTIMEDIA LAYER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : MEDIA.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is MEDIA header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  MEDIA_H 
    #define  MEDIA_H

#include "user_media.h"

#define  VIDEO_DBG_REC_EN                  HAL_CFG_EN_DBG   // for video debug print  record
#define  VIDEO_DBG_PLY_EN                  HAL_CFG_EN_DBG   // for video debug print  playback
#define  AUDIO_DBG_REC_EN                  HAL_CFG_EN_DBG   // for audio debug print
#define  AUDIO_DBG_PLY_EN                  HAL_CFG_EN_DBG   // for audio play debug
//---------------configure-----------------
#define  MEDIA_CFG_IMAGE_DECODE_EN     1    // image encode
#define  MEDIA_CFG_IMAGE_ENCODE_EN     1    // image decode
#define  MEDIA_CFG_VIDEO_RECORD_EN     1    // video record
#define  MEDIA_CFG_VIDEO_PLAYBK_EN     1    // video playback
#define  MEDIA_CFG_AUDIO_PLAYBK_EN     1    // audio record
#define  MEDIA_CFG_AUDIO_RECORD_EN     0    // audio playback
#define  MEDIA_CFG_THUMBNAIL_EN        1    // thumbnail.NOTES: MEDIA_CFG_VIDEO_PLAYBK_EN must en enabled
#define  MEDIA_CFG_SYNC_TIMER_EN       1   // video record & play sync by timer
#define  MEDIA_CFG_VIDEO_SYNC_FPS      1   // inster real vids frame  

#define  JPEG_EXIF      			0 		// 0:TIFF jpeg , 1: exif jpeg
#define  MEDIA_CFG_TIMER                    HAL_CFG_SYNC_TIMER
#define  MEDIA_CFG_AVI_SAMPLERATE       ADC_SAMPLEREATE_16000

#define  VIDEO_CFG_JUMP_SAMEFRAME      1    // jump of the same frame of vids

#define  AUDIO_CFG_BUFFER_SIZE           HAL_CFG_PCM_BUFFER_SIZE   // audio play buffer size
#define  AUDIO_CFG_BUFFER_NUM             3// HAL_CFG_PCM_BUFFER_NUM       // audio play buffer number

#define  VIDEO_CFG_BUFFER_NUM             HAL_CFG_LCD_BUFFER_NUM       // video play buffer number

#define  JPEG_CFG_CACHE_NUM                2
#define  JPEG_CFG_CACHE_SIZE              0x32000  // 200k//(HAL_CFG_MJPEG_SIZE/2) // 0x80000   // 1-m,for jpeg file cache

#define  AVI_CFG_IDX1_BUFF_SIZE           32768 // 8192  // avi idx1 cache size 

#define  THUMBNAIL_CFG_NUM                  1      // thumbnail cache number
#define  THUMBNAIL_CFG_WIDTH               96     // thumbnail width
#define  THUMBNAIL_CFG_HEIGHT              64    // thumbnail height


#define  WATERMAKE_FONT_W                   64
#define  WATERMAKE_FONT_H                   64
#define  WATERMAKE_FONT_NUM                20




union MEDIA_U
{
	FHANDLE fd;
	INT32U buff;
};
typedef struct Media_Res_S
{
	INT16U width;
	INT16U height;
}Media_Res_T;
typedef struct Media_Pos_S
{
	INT16S x;
	INT16S y;
}Media_Pos_T;

typedef struct Media_S
{
	INT16U type;
	INT16S rev;
	
	union MEDIA_U src;
}Media_T;

enum
{
	VIDEO_CH_A=0,
	VIDEO_CH_B,
	VIDEO_CH_MAX
};

enum  // source type 
{
	MEDIA_SRC_FS=0,   // file system
	MEDIA_SRC_NVFS,   // spi flash
	MEDIA_SRC_RAM,    // sdram

	MEDIA_SRC_MAX
};

enum  // audio type
{
	AUDIO_TYPE_ADPCM=0,
	AUDIO_TYPE_ALAW,
	AUDIO_TYPE_ULAW,
	AUDIO_TYPE_PCM,

	AUDIO_TYPE_ERROR,
	AUDIO_TYPE_AUTO  // player will find the right type in audio file,if no find,return error.
};

enum// player state
{
	MEDIA_STAT_STOP=0,
	MEDIA_STAT_START,
	MEDIA_STAT_PLAY=1,
	MEDIA_STAT_PAUSE,	

	MEDIA_STAT_ERROR
};

enum // media file type
{
	FILE_TYPE_AVI=0,
	FILE_TYPE_MOV,
    FILE_TYPE_WAV,
    FILE_TYPE_MP3,
    FILE_TYPE_JPG,
    FILE_TYPE_BMP,
    FILE_TYPE_RAW,     // raw data,do not need decode
    FILE_TYPE_AUTO,    // PLAYER AUTO RECGENIZE THE FILE TYPE
	FILE_TYPE_ERROR	
};
enum // video encode type
{
	VIDEO_CODE_MJPEG=0,
	VIDEO_CODE_H264,

	VIDEO_CODE_ERROR
};

enum  // command
{
	CMD_COM_ERROR=0,
	CMD_COM_CHECK,
	CMD_COM_LOOPTIME,
	CMD_COM_LOOPREC,
	CMD_COM_TIMESTRAMP,
	CMD_COM_AUDIOEN,
	CMD_COM_RESOLUTIONN,
	CMD_COM_QUALITY,
	
	CMD_COM_FRAME,
		
	CMD_AUDIO_RECORD_INIT,
	CMD_AUDIO_RECORD_UNINIT,
	CMD_AUDIO_RECORD_START,
	CMD_AUDIO_RECORD_STOP,
	CMD_AUDIO_RECORD_PAUSE,
	CMD_AUDIO_RECORD_RESUME,

	CMD_AUDIO_PLAYBACK_INIT,
	CMD_AUDIO_PLAYBACK_UNINIT,
	CMD_AUDIO_PLAYBACK_START,
	CMD_AUDIO_PLAYBACK_STOP,
	CMD_AUDIO_PLAYBACK_PAUSE,
	CMD_AUDIO_PLAYBACK_RESUME,

	CMD_VIDEO_RECORD_INIT,
	CMD_VIDEO_RECORD_UNINIT,
	CMD_VIDEO_RECORD_START,
	CMD_VIDEO_RECORD_STOP,
	CMD_VIDEO_RECORD_PAUSE,
	CMD_VIDEO_RECORD_RESUME,

	CMD_VIDEO_PLAYBACK_INIT,
	CMD_VIDEO_PLAYBACK_UNINIT,
	CMD_VIDEO_PLAYBACK_START,
	CMD_VIDEO_PLAYBACK_STOP,
	CMD_VIDEO_PLAYBACK_PAUSE,
	CMD_VIDEO_PLAYBACK_RESUME,

	CMD_MSG_RECODE_STOP,
};


#define  STATUS_FAIL      -1    // fail
#define  STATUS_OK         0     // success





#define MEDIA_NULL_CHECK(p,r)   if(p == NULL)return r














#endif