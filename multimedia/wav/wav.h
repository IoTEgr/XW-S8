#ifndef  WAV_H
       #define  WAV_H


#define WAV_CFG_PCM_USE         1
#define WAV_CFG_ALAW_USE       0
#define WAV_CFG_ULAW_USE       0
#define WAV_CFG_ADPCM_USE     0



#define  MS_ADPCM_COFNUM    7


typedef struct MAX_WAV_S
{
	INT32U filesize;	

    INT16U  fmtcode;
    INT16U  channel;
	INT32U  samplerate;
	INT32U  bitrate;

	INT16U  blocksize;
	INT16U  samplesize;

	INT16U  cbsize;
	INT16U  samplepreblock;
    INT16U  cofnum;
	INT32U  coftable[MS_ADPCM_COFNUM];

	INT32U  factsize;


    INT32U  datasize;
	INT32U  dataoffset;
}MAX_WAV_T;


typedef enum
{
    WAV_TYPE_PCM=1,
	WAV_TYPE_ALAW=6,
	WAV_TYPE_ULAW=7,
	WAV_TYPE_MSADPCM=2,
	WAV_TYPE_IMADPCM=0x11,
	WAV_TYPE_AUTO=0
}WAV_TYPE_E;
typedef enum
{
	MONO_TO_MONO=0,
	STREO_TO_STREO,
	MONO_TO_STREO,
	STRE0_TO_MONO,
}CHANNEL_EXCHANGE_E;



typedef struct WAV_INFO_S
{
    INT16U type;
	INT16U channel;

	INT32U samplerate;
	INT32U totaltime;  //ms
}WAV_INFO_T;

typedef struct USER_WAV_S
{
	INT16U channel;
	INT16U type;

	INT32U samplerate;
	INT32U outbufferlen;
	INT32U outbufferaddr;
}USER_WAV_T;

INT32S wavDecodeInit(USER_WAV_T *tar);


WAV_INFO_T *wavDecodeGetInfo(void);


INT32S wavDecodeParse(FHANDLE handle);

INT32S wavDecodeGetTotalTime(void);

INT32S wavDecodeGetCurTime(void);

INT32S wavDecodeOneFrame(FHANDLE handle,INT32U buffer,INT32U len);

INT32S wavDecodeGetSamplerate(void);


INT32S wavEncodeInit(USER_WAV_T *tar,USER_WAV_T *src);


INT32S wavEncodeParse(FHANDLE handle);


INT32S wavEncodeEnd(FHANDLE handle);


INT32S wavEncodeGetBlockSize(void);


INT32S wavEncodeOneFrame(FHANDLE handle,INT32U input,INT32U len,INT32U output);


INT32S wavEncodeAddSize(INT32U size);

#endif
