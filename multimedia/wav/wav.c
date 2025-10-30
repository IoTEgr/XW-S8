
#include "../media.h"
#include "../multimedia.h"
#include "pcm.h"

#if (WAV_CFG_PCM_USE || WAV_CFG_ALAW_USE || WAV_CFG_ULAW_USE || WAV_CFG_ADPCM_USE)


#define  WAV_TAG_RIFF     0x46464952
#define  WAV_TAG_WAVE     0x45564157
#define  WAV_TAG_FMT      0x20746d66
#define  WAV_TAG_DATA     0x61746164
#define  WAV_TAG_FACT     0x74636166

#define  WAV_ADD_FSIZE    0x4
#define  WAV_ADD_TYPE     0x14
#define  WAV_ADD_CHANNEL  0x16
#define  WAV_ADD_SRATE    0x18
#define  WAV_ADD_BRATE    0x1c
#define  WAV_ADD_BSIZE    0x20
#define  WAV_ADD_BITS     0x22
#define  WAV_ADD_FACT     0x4e

#define  WAV_PCM_ADD_DSIZE    0x28
#define  WAV_ADPCM_ADD_DSIZE  0x56

#define  WAV_IMA_ADD_CBSIZE 0x26
#define  WAV_IMA_ADD_FACT   0x30
#define  WAV_IMA_ADD_DSIZE  0x38

#define  WAV_LAW_ADD_FACT   0x2e
#define  WAV_LAW_ADD_DSIZE  0x36

const static INT8U Riff_PCM_Table[44]=
{
   0x52,0x49,0x46,0x46,0x00,0x00,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
   0x10,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x64,0x61,0x74,0x61,0x00,0x00,0x00,0x00
};
const static INT8U Riff_MSADPCM_Table[90]=
{
   0x52,0x49,0x46,0x46,0x00,0x00,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
   0x32,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x07,0x00,0x00,0x01,0x00,0x00,0x00,0x02,
   0x00,0xff,0x00,0x00,0x00,0x00,0xc0,0x00,0x40,0x00,0xf0,0x00,0x00,0x00,0xcc,0x01,
   0x30,0xff,0x88,0x01,0x18,0xff,0x66,0x61,0x63,0x74,0x04,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x64,0x61,0x74,0x61,0x00,0x00,0x00,0x00
};
const static INT8U Riff_IMAADPCM_Table[60]=
{
   0x52,0x49,0x46,0x46,0x00,0x00,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
   0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x66,0x61,0x63,0x74,0x04,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x64,0x61,0x74,0x61,0x00,0x00,0x00,0x00
};
const static INT8U Riff_LAWPCM_Table[58]=
{
   0x52,0x49,0x46,0x46,0x00,0x00,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
   0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x61,0x63,0x74,0x04,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x64,0x61,0x74,0x61,0x00,0x00,0x00,0x00
};
const static INT16U MSADPCM_COF_Table[MS_ADPCM_COFNUM<<1]=
{
	0x0100,0x0000,0x0200,0xff00,0x0000,0x0000,0x00c0,0x0040,0x00f0,0x00,0x01cc,0xff30,0x8801,0xff18
};
typedef struct Wav_Ctrl_S
{
	WAV_INFO_T info;
}Wav_Ctrl_T;
static MAX_WAV_T  wavHeader;
static USER_WAV_T usrTarget,usrSource;
static Wav_Ctrl_T wavCtrl;
static INT8U WavHeader[90];

INT32S wavEncodeInit(USER_WAV_T *tar,USER_WAV_T *src)
{
	usrTarget.channel = tar->channel;
	usrTarget.outbufferlen = tar->outbufferlen;
	usrTarget.samplerate = tar->samplerate;
	usrTarget.type = tar->type;

	usrSource.channel = src->channel;
	usrSource.outbufferlen = src->outbufferlen;
	usrSource.samplerate = src->samplerate;
	usrSource.type = src->type;

	if(usrTarget.type == WAV_TYPE_AUTO)
       usrTarget.type=WAV_TYPE_PCM;

	return 0;
}
static void u32tou8(INT8U *src,INT32U value)
{
    INT32U *addr;
	
	addr = (INT32U *)src;
	*addr = value;
}
static void u16tou8(INT8U *src,INT32U value)
{
    INT16U *addr;
	
	addr = (INT16U *)src;
	*addr = value;
}
INT32S wavEncodeParse(FHANDLE handle)
{
	int i,len;

	

	wavHeader.channel = usrTarget.channel;
	wavHeader.fmtcode = usrTarget.type;
	wavHeader.samplerate = usrTarget.samplerate;

	if(usrTarget.type==WAV_TYPE_PCM)
	{
		wavHeader.bitrate = usrTarget.samplerate * usrTarget.channel*2;
		wavHeader.blocksize = usrTarget.channel*2;
		wavHeader.samplesize = 16;
		wavHeader.dataoffset = 44;
		wavHeader.datasize = 0;
		wavHeader.filesize = 44;
		for(i=0;i<44;i++)
		{
			WavHeader[i] = Riff_PCM_Table[i];
		}
		//*((INT32U *)&WavHeader[WAV_PCM_ADD_DSIZE]) = wavHeader.datasize;
		len = 44;
	}
	else if(usrTarget.type==WAV_TYPE_IMADPCM)
	{
		wavHeader.dataoffset = 60;
		wavHeader.datasize = 0;
		wavHeader.filesize = 60;
		wavHeader.factsize = 0;

		wavHeader.samplesize = 4;
		wavHeader.blocksize = usrTarget.channel*0x100;

//		len = ((wavHeader.blocksize<<1)-7)<<1;
		len = ((wavHeader.samplerate*200)/1010)*wavHeader.blocksize;  // 505 -> n samples pre block
		if((len%100) >=50)
			len+=100;
		wavHeader.bitrate = len/100;

		for(i=0;i<60;i++)
		{
			WavHeader[i] = Riff_IMAADPCM_Table[i];
		}
		len = 60;
	#if WAV_CFG_ADPCM_USE	
		imadpcm_reset();
	#endif	
	}
	else if(usrTarget.type==WAV_TYPE_MSADPCM)
	{
		wavHeader.dataoffset = 90;
		wavHeader.datasize = 0;
		wavHeader.filesize = 90;
		wavHeader.factsize = 0;

		wavHeader.samplesize = 4;
		wavHeader.blocksize = usrTarget.channel*0x100;
		len = ((wavHeader.blocksize-4)*4+2);
		wavHeader.bitrate = (wavHeader.samplerate*wavHeader.channel*2+len-1)/len;
		wavHeader.bitrate = wavHeader.bitrate*wavHeader.blocksize; // aglin by block size
		for(i=0;i<90;i++)
		{
			WavHeader[i] = Riff_MSADPCM_Table[i];
		}
	}
	else if(usrTarget.type == WAV_TYPE_ALAW || usrTarget.type == WAV_TYPE_ULAW)
	{
		wavHeader.dataoffset = 58;
		wavHeader.datasize = 0;
		wavHeader.filesize = 58;
		wavHeader.factsize = 0;
        wavHeader.samplesize = 8;
	    wavHeader.blocksize = usrTarget.channel*1;
        wavHeader.bitrate = usrTarget.samplerate * usrTarget.channel;
		for(i=0;i<58;i++)
		{
			WavHeader[i] = Riff_LAWPCM_Table[i];
		}
		len = 58;
	}
	else
		return -1;
     u32tou8(&WavHeader[WAV_ADD_FSIZE],wavHeader.filesize);
//    *((INT32U *)&WavHeader[WAV_ADD_FSIZE]) = wavHeader.filesize;
     u16tou8(&WavHeader[WAV_ADD_TYPE],wavHeader.fmtcode);
//	*((INT16U *)&WavHeader[WAV_ADD_TYPE])  = wavHeader.fmtcode;
     u16tou8(&WavHeader[WAV_ADD_CHANNEL],wavHeader.channel);
//	*((INT16U *)&WavHeader[WAV_ADD_CHANNEL])= wavHeader.channel;
	 u32tou8(&WavHeader[WAV_ADD_SRATE],wavHeader.samplerate);
//	*((INT32U *)&WavHeader[WAV_ADD_SRATE]) = wavHeader.samplerate;
	u32tou8(&WavHeader[WAV_ADD_BRATE],wavHeader.bitrate);
//	*((INT32U *)&WavHeader[WAV_ADD_BRATE]) = wavHeader.bitrate;
	u16tou8(&WavHeader[WAV_ADD_BSIZE],wavHeader.blocksize);
//	*((INT16U *)&WavHeader[WAV_ADD_BSIZE]) = wavHeader.blocksize;
	u16tou8(&WavHeader[WAV_ADD_BITS],wavHeader.samplesize);
//	*((INT16U *)&WavHeader[WAV_ADD_BITS])  = wavHeader.samplesize;

	if(write(handle,&WavHeader,len)<0)
		return -1;

	return 0;
}

INT32S wavEncodeEnd(FHANDLE handle)
{
	int ret=0;

    lseek(handle,0,0);
    u32tou8(&WavHeader[WAV_ADD_FSIZE],wavHeader.filesize-8); // riif & fsize
//	*((INT32U *)&WavHeader[WAV_ADD_FSIZE]) = wavHeader.filesize;
	if(usrTarget.type==WAV_TYPE_PCM)
	{
		u32tou8(&WavHeader[WAV_PCM_ADD_DSIZE],wavHeader.datasize);
 //       *((INT32U *)&WavHeader[WAV_PCM_ADD_DSIZE]) = wavHeader.datasize;
		ret=write(handle,&WavHeader,44);
	}
	else if(usrTarget.type==WAV_TYPE_IMADPCM)
	{
		u16tou8(&WavHeader[WAV_IMA_ADD_CBSIZE],505);
	//	*((INT16U *)&WavHeader[WAV_IMA_ADD_CBSIZE]) = 505;
		u32tou8(&WavHeader[WAV_IMA_ADD_DSIZE],wavHeader.datasize);
      //  *((INT32U *)&WavHeader[WAV_IMA_ADD_DSIZE]) = wavHeader.datasize;
		u32tou8(&WavHeader[WAV_IMA_ADD_FACT],wavHeader.factsize);
		//*((INT32U *)&WavHeader[WAV_IMA_ADD_FACT]) = wavHeader.factsize;
		ret=write(handle,&WavHeader,60);
	}
	else if(usrTarget.type==WAV_TYPE_MSADPCM)
	{
		u32tou8(&WavHeader[WAV_ADPCM_ADD_DSIZE],wavHeader.datasize);
     //   *((INT32U *)&WavHeader[WAV_ADPCM_ADD_DSIZE]) = wavHeader.datasize;
		u32tou8(&WavHeader[WAV_ADD_FACT],wavHeader.factsize);
	//	*((INT32U *)&WavHeader[WAV_ADD_FACT]) = wavHeader.factsize;
		ret=write(handle,&WavHeader,90);
	}
	else if(usrTarget.type == WAV_TYPE_ALAW || usrTarget.type == WAV_TYPE_ULAW)
	{
		u32tou8(&WavHeader[WAV_LAW_ADD_DSIZE],wavHeader.datasize);
       // *((INT32U *)&WavHeader[WAV_LAW_ADD_DSIZE]) = wavHeader.datasize;
		u32tou8(&WavHeader[WAV_LAW_ADD_FACT],wavHeader.factsize);
		//*((INT32U *)&WavHeader[WAV_LAW_ADD_FACT]) = wavHeader.factsize;
		ret=write(handle,&WavHeader,58);
	}

	return ret;
}
INT32S wavEncodeGetBlockSize(void)
{
	INT32U size;
    if(usrTarget.type==WAV_TYPE_MSADPCM || usrTarget.type==WAV_TYPE_IMADPCM)
	{
        size = (wavHeader.blocksize-4)*4+2;
		if(usrSource.channel == 2 && usrTarget.channel == 1)
			size <<= 1;

		return size;
	}
	return wavHeader.blocksize;
}

INT32S wavEncodeOneFrame(FHANDLE handle,INT32U input,INT32U len,INT32U output)
{
    int ret;
	INT16U chtype;
//	INT32U size;

	ret = -1;
	if(usrSource.channel == usrTarget.channel)
	{
		chtype = MONO_TO_MONO;
	}
	else if(usrSource.channel == 1)
	{
        chtype = MONO_TO_STREO;
	}
	else 
	{
        chtype = STRE0_TO_MONO;
	}

    ret = -1;
    if(usrTarget.type==WAV_TYPE_PCM)
	{
	#if WAV_CFG_PCM_USE
         ret = pcm_encode((INT16U *)input,(INT16U *)output,len,chtype,WAV_TYPE_PCM);
	#endif
	}
	else if(usrTarget.type == WAV_TYPE_ALAW)
	{
	#if WAV_CFG_ALAW_USE
		ret = alaw_encode((INT16U *)input,(INT16U *)output,len,chtype,WAV_TYPE_PCM);
	#endif
	}
	else if(usrTarget.type == WAV_TYPE_ULAW)
	{
	#if WAV_CFG_ULAW_USE
		ret = ulaw_encode((INT16U *)input,(INT16U *)output,len,chtype,WAV_TYPE_PCM);
	#endif
	}
	else if(usrTarget.type == WAV_TYPE_IMADPCM)
	{
	#if WAV_CFG_ADPCM_USE
		size = wavEncodeGetBlockSize();
		if(size!=len)
			return -1;
		ret = imaadpcm_encode((INT16U *)input,(INT16U *)output,len,chtype,WAV_TYPE_PCM);
	#endif	
	}

    if(ret<0)
		return -1;

    if(usrTarget.type!=WAV_TYPE_PCM)
	{
		if(usrSource.channel==2);
		  len>>=1;
        wavHeader.factsize+=len>>1;
	}
	wavHeader.datasize+=ret;
	wavHeader.filesize+=ret;

	return ret;
}
INT32S wavEncodeAddSize(INT32U size)
{
	wavHeader.datasize+=size;
	wavHeader.filesize+=size;	

	return 0;
}

INT32S wavDecodeInit(USER_WAV_T *tar)
{
	usrTarget.channel = tar->channel;
	usrTarget.outbufferlen = tar->outbufferlen;
	usrTarget.samplerate = tar->samplerate;
	usrTarget.type = tar->type;

	return 0;
}
static INT32S wavDecodeTime(INT32S dsize) // ms
{
	INT32U temp,temp2,totaltime;

    if(dsize<=0)
		return 0;
/*	if(wavHeader.fmtcode == WAV_TYPE_IMADPCM || wavHeader.fmtcode == WAV_TYPE_MSADPCM)
	{
         temp = wavHeader.samplerate;
		 temp2 = temp/1000;
		 temp = temp%1000;
         totaltime = (dsize)/temp2;
		 if(temp)
             totaltime -= ((dsize)/temp)/1000;
	}
	else*/
	{
	    temp = wavHeader.bitrate%1000;
		temp2 = wavHeader.bitrate/1000;
		totaltime = (dsize)/temp2;
		if(temp)
	        totaltime -= ((dsize)/temp)/1000;
	}
	return totaltime;
}
static INT32S wavDecodetInfo(WAV_INFO_T *info)
{
    if(wavHeader.datasize==0)
		return -1;
	info->channel    = wavHeader.channel;
	info->samplerate = wavHeader.samplerate;
	info->type = wavHeader.fmtcode;

	if(info->type == WAV_TYPE_PCM)
		deg_Printf("16bit-pcm\n");
	else if(info->type == WAV_TYPE_MSADPCM)
		deg_Printf("4bit-MSadpcm\n");
	else if(info->type == WAV_TYPE_ULAW)
		deg_Printf("8bit-ulaw\n");
	else if(info->type == WAV_TYPE_ALAW)
		deg_Printf("8bit-alaw\n");
	else if(info->type == WAV_TYPE_IMADPCM)
		deg_Printf("4bit-IMAadpcm\n");
	else
        deg_Printf("unknow\n");
    if(wavHeader.fmtcode == WAV_TYPE_IMADPCM || wavHeader.fmtcode == WAV_TYPE_MSADPCM)
	    info->totaltime = wavDecodeTime(wavHeader.factsize);
	else
		info->totaltime = wavDecodeTime(wavHeader.datasize);
	return 0;
}

WAV_INFO_T *wavDecodeGetInfo(void)
{
	return &wavCtrl.info;
}
INT32S wavDecodeGetBlockSize(void)
{
    if(wavHeader.datasize==0)
		return -1;
	return wavHeader.blocksize;
}
INT32S wavDecodeGetTotalTime(void)
{
	return wavCtrl.info.totaltime;
}
INT32S wavDecodeGetSamplerate(void)
{
	return wavCtrl.info.samplerate;
}
INT32S wavDecodeGetCurTime(void)
{
	INT32U offset;

    offset = ringBufferPoint()-wavHeader.dataoffset;
	if(wavHeader.fmtcode == WAV_TYPE_IMADPCM || wavHeader.fmtcode == WAV_TYPE_MSADPCM)
		return (((offset*100)/wavHeader.datasize)*wavCtrl.info.totaltime/100);
	else
	    return wavDecodeTime(offset);
}
INT32S wavDecodeParse(FHANDLE handle)
{
    int len=0,stat,cidx;
	INT32U temp;

	wavHeader.datasize = 0;
	stat = 0;
    cidx = 0;
	while(1)
	{
        if(ringBufferRead((INT8U *)&temp,4)<0)
			return -1;
		switch(stat)
		{
		case 0:
			  if(temp!=WAV_TAG_RIFF)
				   return -2;
			   stat++;
			   break;
		case 1:
			   wavHeader.filesize =temp+4;
			   stat++;
			   break;
		case 2:
			   if(temp!=WAV_TAG_WAVE)
				   return -3;
			   stat++;
			   break;
		case 3:
			  if(temp!=WAV_TAG_FMT)
				   return -4;
			   stat++;
			   break;
		case 4:
			   len = temp;stat++;
			   wavHeader.dataoffset=len+20+8;
			   break;
		case 5:
			   wavHeader.fmtcode = temp&0xffff;
			   wavHeader.channel = temp>>16;
			   stat++;break;
		case 6:
			   wavHeader.samplerate = temp;
			   stat++;
			   break;
		case 7:
			   wavHeader.bitrate = temp;
			   stat++;
			   break;
		case 8:
			   wavHeader.blocksize = temp&0xffff;
			   wavHeader.samplesize = temp>>16;
               stat++;
			   len-=16;
               if(len!=0)
			   {
				   stat = 15;   // bcsize,& cof
				   if(ringBufferRead((INT8U *)&temp,2)<0)
						return -6;
				   if((temp&0xffff) == 0)
				   {
					   if(len>2)
                          ringBufferSeek(wavHeader.dataoffset-8);
					   stat = 9;
				   }
				   else
				   {
                       wavHeader.cbsize = temp;
					   if((temp&0xffff) == 2)
					   {
                            if(ringBufferRead((INT8U *)&temp,2)<0)
								return -7;
							wavHeader.samplepreblock = temp;
							stat = 9;
					   }
				   }
			   } 
			   break;
		case 9:
			   if(temp == WAV_TAG_FACT)
			   {
				   stat++;break; // 10
			   }
		       if(temp!=WAV_TAG_DATA)
					return -5;
			    stat=12;
				break;
    
		case 12:
			    wavHeader.datasize = temp;
				wavDecodetInfo(&wavCtrl.info);
			    return 0;
				break;
		case 10:
			    len = temp;
			    wavHeader.dataoffset+=8+temp;
				stat++; 
				break;
		case 11:
			    wavHeader.factsize =temp;
			    if(len != 4)
                     ringBufferSeek(wavHeader.dataoffset-8);
				stat = 9;
				break;
		case 15:
                wavHeader.samplepreblock=temp&0xffff; 
                wavHeader.cofnum = temp>>16;
			    stat++;
				break;
		case 16:
				if(cidx<MS_ADPCM_COFNUM)
				{
                     wavHeader.coftable[cidx++] = temp;
				}
				if(wavHeader.cbsize<=(cidx*4+4))
					stat = 9;
				break;		
		}
	}
	
	return wavHeader.blocksize;    
}

INT32S wavDecodeOneFrame(FHANDLE handle,INT32U buffer,INT32U len)
{
    int ret,cnt;
	INT32U addr=0;
	INT16U chtype;

	usrTarget.outbufferlen = len;
    usrTarget.outbufferaddr = buffer;

    ret = -1;
//--------------check output channel
	if(wavHeader.channel == usrTarget.channel)
	{
		chtype = MONO_TO_MONO;
	}
	else if(wavHeader.channel == 1)
	{
        chtype = MONO_TO_STREO;
	}
	else 
	{
        chtype = STRE0_TO_MONO;
	}

	ret = ringBufferDataSize();
	if(ret<=0)
		return -3;  // no data
//--------------check input len------------------
	if(wavHeader.fmtcode == WAV_TYPE_IMADPCM || wavHeader.fmtcode == WAV_TYPE_MSADPCM)
	{
		cnt = wavHeader.blocksize;  // block align
		if(cnt>ret)
		{
			return -2; // file end
		}
	}
	else
	{
		cnt = ((len*wavHeader.channel*wavHeader.samplesize)/(usrTarget.channel<<4))&(~0x03); // max input		
		if(cnt>ret)
		{
			cnt = ret;
			if(cnt<=0)
				return -2; // file end
		}
		
	}
    ret = -1;
//----------------decode------------------------
	if(wavHeader.fmtcode == WAV_TYPE_PCM)
	{
	#if WAV_CFG_PCM_USE
        ret = pcm_decode((INT16U *)addr,(INT16U *)buffer,cnt,chtype,WAV_TYPE_PCM);
	    //deg_Printf("pcm decode = %d\n",ret);
	#endif
	}
	else if(wavHeader.fmtcode == WAV_TYPE_IMADPCM)
	{
	#if WAV_CFG_ADPCM_USE
		ret = imaadpcm_decode((INT16U *)addr,(INT16U *)buffer,cnt,chtype,WAV_TYPE_PCM);
	#endif
	}
	else if(wavHeader.fmtcode == WAV_TYPE_ALAW)
	{
	#if WAV_CFG_ALAW_USE
		ret = alaw_decode((INT16U *)addr,(INT16U *)buffer,cnt,chtype,WAV_TYPE_PCM);
	#endif
	}
	else if(wavHeader.fmtcode == WAV_TYPE_ULAW)
	{
	#if WAV_CFG_ULAW_USE
		ret = ulaw_decode((INT16U *)addr,(INT16U *)buffer,cnt,chtype,WAV_TYPE_PCM);
	#endif
	}

	return ret;
}


#endif

