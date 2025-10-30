
#include  "../inc/mp3_decode_api.h"

/*typedef*/ struct MP3_PCM_DATA
{
	u8 *buf;
	u32 len;
};

typedef struct MP3_PCM_FIFO
{
    u16 max;
    u16 cnt;    
    u16 oput;
    u16 iput;
    struct MP3_PCM_DATA data[MP3_PCM_FIFO_NUM];
}MP3_PCM_FIFO_T;


/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */

typedef struct mp3_buffer {

	int fd;					  		/*file */
	u8 *file_buf;					/*file all buffer*/
	unsigned int	flen;			/*file length*/
	unsigned int	fpos;			/*current position*/
	unsigned char 	*fbuf;			/*buffer*/
	unsigned int	fbsize; 		/*indeed size of buffer*/

}Mp3_Buffer_T;


Mp3_Buffer_T mp3_file;
MP3_DEC_OP_T *mp3_dec_op;
static u8 *mp3_file_buf;
static u8 *mp3_pcm_buf;
static u8 *mp3_dac_mute_buf;


//=====playfrom pcm fifo=====
MP3_PCM_FIFO_T mp3_pcm_fifo;

static u8 mp3_pcm_fifo_init()
{
	u8 i;
	//==send fifo init==
	mp3_pcm_fifo.max = mp3_dec_op->mp3_pcm_fifo_num;
	mp3_pcm_fifo.cnt = 0;
	mp3_pcm_fifo.iput = 0;
	mp3_pcm_fifo.oput = 0;

	for(i=0;i<mp3_dec_op->mp3_pcm_fifo_num;i++)
	{
		mp3_pcm_fifo.data[i].len = 0;
		mp3_pcm_fifo.data[i].buf = mp3_dec_op->mp3_pcm_buf+i*mp3_dec_op->mp3_pcm_fifo_size;
	}
	return 0;
}

//==ret : 0: not full, 1: fifo full==
static u8 mp3_pcm_fifo_full()
{
    if(mp3_pcm_fifo.cnt<(mp3_pcm_fifo.max-1))
    {
		return 0;
	}
	return 1;
}

static u16 mp3_pcm_fifo_get_cnt()
{
	return mp3_pcm_fifo.cnt;
}


static u8 mp3_pcm_fifo_put_buf(u32 *buf)
{
    if(mp3_pcm_fifo.cnt<mp3_pcm_fifo.max-1)
    {
    	*buf=(u32)(mp3_pcm_fifo.data[mp3_pcm_fifo.iput].buf);
		return 0;
    }
    return 1;
}

static u8 mp3_pcm_fifo_put_buf_finish(u16 size)
{
    if(mp3_pcm_fifo.cnt<mp3_pcm_fifo.max-1)
    {
		mp3_pcm_fifo.data[mp3_pcm_fifo.iput].len = size;

		mp3_pcm_fifo.iput++;
	    mp3_pcm_fifo.cnt++;
	    if(mp3_pcm_fifo.iput>=mp3_pcm_fifo.max)
	       mp3_pcm_fifo.iput = 0;

		return 0;
    }
    return 1;
}

//==ret: 0:ok , 1: fifo null==
static u8 mp3_pcm_fifo_get(u32 *buf,u32 *len)
{
    if(0==mp3_pcm_fifo.cnt)
    {
        return 1;
    }    

	*buf=(u32)(mp3_pcm_fifo.data[mp3_pcm_fifo.oput].buf);
	*len=mp3_pcm_fifo.data[mp3_pcm_fifo.oput].len;
	mp3_pcm_fifo.oput++;
    mp3_pcm_fifo.cnt--;
    if(mp3_pcm_fifo.oput>=mp3_pcm_fifo.max)
       mp3_pcm_fifo.oput = 0;

    return 0;
}



int mp3_dac_memInit(u32 pcm_fifo_num,u32 pcm_fifo_size,u32 mp3_buf_size)
{
	if(pcm_fifo_num>64)
	{
		pcm_fifo_num=64;
	}

	mp3_dac_mute_buf=hal_sysMemMalloc(pcm_fifo_size,32);
	if(NULL==mp3_dac_mute_buf)
	{
		deg_Printf("mp3_dac_mute_buf mem err\n");
		return 1;
	}
	memset(mp3_dac_mute_buf,0,pcm_fifo_size);

	mp3_pcm_buf=hal_sysMemMalloc(pcm_fifo_num*pcm_fifo_size,32);
	if(NULL==mp3_pcm_buf)
	{
		deg_Printf("mp3_pcm_buf mem err\n");
		return 2;
	}

	mp3_dec_op=hal_sysMemMalloc(sizeof(MP3_DEC_OP_T),32);
	if(NULL==mp3_dec_op)
	{
		deg_Printf("mp3_dec_op mem err\n");
		return 1;
	}
	memset(mp3_dec_op,0,sizeof(MP3_DEC_OP_T));

	mp3_dec_op->mp3_bitstream_buf_size=mp3_buf_size;
	mp3_dec_op->mp3_pcm_fifo_num=pcm_fifo_num;
	mp3_dec_op->mp3_pcm_fifo_size=pcm_fifo_size;
	mp3_dec_op->mp3_pcm_buf_size=pcm_fifo_num*pcm_fifo_size;
	mp3_dec_op->mp3_pcm_buf=mp3_pcm_buf;
	mp3_dec_op->dac_mute_buf=mp3_dac_mute_buf;


	mp3_file_buf=hal_sysMemMalloc(mp3_buf_size*2,32);
	if(NULL==mp3_file_buf)
	{
		deg_Printf("mp3 data mem err\n");
		return 3;
	}

	memset(&mp3_file,0,sizeof(Mp3_Buffer_T));
	mp3_file.fbuf=mp3_file_buf;
	return 0;


}

void mp3_dac_memUinit(void)
{
	if(mp3_file_buf)
	{
		hal_sysMemFree(mp3_file_buf);
		mp3_file_buf=NULL;
	}

	if(mp3_dec_op)
	{
		hal_sysMemFree(mp3_dec_op);
		mp3_dec_op=NULL;
	}

	if(mp3_pcm_buf)
	{
		hal_sysMemFree(mp3_pcm_buf);
		mp3_pcm_buf=NULL;
	}

	if(mp3_dac_mute_buf)
	{
		hal_sysMemFree(mp3_dac_mute_buf);
		mp3_dac_mute_buf=NULL;
	}
}


void mp3PlaybackDacISR(int flag)
{
	//deg_Printf("%d,",flag);

	if(flag&DAC_INT_PEND)
    {
    	//deg_Printf("p");
		return ;
    }

	if(mp3_dec_op)
	{
		if(mp3_dec_op->play_sta == MP3_DAC_PAUSE)
		{
			if((flag&DAC_INT_EMPTY))
			{
				//deg_Printf("mp3  pause\n");
				hal_dacPlayStop();
			}
			return;
		}
	}
	else
	{
		hal_dacPlayStop();
		return;
	}

	if(flag&DAC_INT_HALF)
	{		
		if(mp3_dec_op)
		{
			if(mp3_pcm_fifo_get_cnt()>0)
			{
				u32 pcm,size;
				mp3_pcm_fifo_get(&pcm,&size);
				hal_dacSetBuffer(pcm,size);
				mp3_dec_op->play_frame_cnt++;
			}
			else
			{
				//hal_dacPlayStop();
				//mp3_dec_op.play_finish=1;
				//deg_Printf("n");
				hal_dacSetBuffer((INT32U)mp3_dec_op->dac_mute_buf,mp3_dec_op->mp3_pcm_fifo_size);
			}
		}
		else
		{
			hal_dacPlayStop();
			return;
		}

	}
	else if(flag&DAC_INT_EMPTY)
	{
		hal_dacPlayStop();
	}
}


int mp3_dacPlay_Start(u8 vol)
{
	if(mp3_dec_op)
	{
		deg_Printf("dec_op vol--:%d vol[%d]\n",mp3_dec_op->volume,vol);
		mp3_dec_op->volume = vol;
		hal_dacSetVolume(mp3_dec_op->volume);
		if(mp3_pcm_fifo_get_cnt()>0)
		{
			u32 pcm,size;
			mp3_pcm_fifo_get(&pcm,&size);
			hal_dacCallBackRegister(mp3PlaybackDacISR);
			hal_dacPlayStart(mp3_dec_op->mad_decoder.frame.header.samplerate,pcm,size);
			mp3_dec_op->play_sta=MP3_DAC_START;
			mp3_dec_op->play_frame_cnt++;
		}
	}
	return 0;
}

int mp3_dacPlay_Pause(void)
{
	if(mp3_dec_op)
	{
		mp3_dec_op->play_sta=MP3_DAC_PAUSE;
	}
	return 0;
}

int mp3_dacPlay_Stop(void)
{
	hal_dacPlayStop();
	if(mp3_dec_op)
	{
		mp3_dec_op->play_sta=MP3_DAC_STOP;
	}
	return 0;
}


//==ret : 0 :stop , 1: playing , 2: pause==
u8 mp3_play_sta(void)
{
	if(mp3_dec_op)
	{
		return mp3_dec_op->play_sta;
	}
	else
	{
		return 0;
	}
}


u32 mp3_flie_playtime()
{
	if(mp3_dec_op)
	{
		return (u32)(((u64)(mp3_dec_op->play_frame_cnt)*mp3_dec_op->frame_len*1000)/(mp3_dec_op->mad_decoder.frame.header.samplerate*2));
	}
	else
	{
		return 0;
	}
}

u32 mp3_file_time_cal()
{
	u32 time,audio_size=0;
	if(mp3_dec_op)
	{
		if(mp3_file.flen>mp3_dec_op->id3_len)
		{
			audio_size=mp3_file.flen-mp3_dec_op->id3_len;
		}
		else
		{
			deg_Printf("file err?");
		}
		time=audio_size*8/mp3_dec_op->mad_decoder.frame.header.bitrate;
	}
	else
	{
		time=0;
	}
	return time;
}


//=====api======


/*
 * This is perhaps the simplest example use of the MAD high-level API.
 * Standard input is mapped into memory via mmap(), then the high-level API
 * is invoked with three callbacks: input, output, and error. The output
 * callback converts MAD's high-resolution PCM samples to 16 bits, then
 * writes them to standard output in little-endian, stereo-interleaved
 * format.
 */


/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */

static
enum mad_flow input(void *data,
		    struct mad_stream *stream)
{
  Mp3_Buffer_T *mp3fp;
  int  ret;
  int  unproc_data_size;    /*the unprocessed data's size*/
  int  read_size;

  mp3fp = (Mp3_Buffer_T *)data;

  if(mp3fp->fpos < mp3fp->flen)
  {
	  if(NULL==mp3fp->file_buf)	// use fd
	  {
	      unproc_data_size = stream->bufend - stream->next_frame;
	      memcpy(mp3fp->fbuf, mp3fp->fbuf+mp3fp->fbsize-unproc_data_size, unproc_data_size);
	      read_size = (mp3_dec_op->mp3_bitstream_buf_size- unproc_data_size)&(~(512-1));// 512 byte align
	      if(mp3fp->fpos + read_size > mp3fp->flen)
	      {
	          read_size = mp3fp->flen - mp3fp->fpos;
	      }
	      //read(mp3fp->fd,mp3fp->fbuf+unproc_data_size,read_size);
	      read(mp3fp->fd,mp3fp->fbuf+mp3_dec_op->mp3_bitstream_buf_size,read_size);
		  memcpy(mp3fp->fbuf+unproc_data_size,mp3fp->fbuf+mp3_dec_op->mp3_bitstream_buf_size,read_size);

	      mp3fp->fbsize = unproc_data_size + read_size;
	      mp3fp->fpos  += read_size;
	      
	      /*Hand off the buffer to the mp3 input stream*/
	      mad_stream_buffer(stream, mp3fp->fbuf, mp3fp->fbsize);
	      ret = MAD_FLOW_CONTINUE;
		  //deg_Printf("i:%d,%d,%d,%d\n",unproc_data_size,mp3fp->fbsize,read_size,mp3fp->fpos);
		  //deg_Printf("i");
	   }
	   else	// use file buf
	   {
		   unproc_data_size = stream->bufend - stream->next_frame;
		   memcpy(mp3fp->fbuf, mp3fp->fbuf+mp3fp->fbsize-unproc_data_size, unproc_data_size);
		   read_size = (mp3_dec_op->mp3_bitstream_buf_size - unproc_data_size)&(~(512-1));// 512 byte align
		   if(mp3fp->fpos + read_size > mp3fp->flen)
		   {
			   read_size = mp3fp->flen - mp3fp->fpos;
		   }

		   memcpy(mp3fp->fbuf+unproc_data_size,mp3fp->file_buf+mp3fp->fpos,read_size);
		   
		   mp3fp->fbsize = unproc_data_size + read_size;
		   mp3fp->fpos	+= read_size;

		   /*Hand off the buffer to the mp3 input stream*/
		   mad_stream_buffer(stream, mp3fp->fbuf, mp3fp->fbsize);
		   ret = MAD_FLOW_CONTINUE;

	   }
  }
  else
  {
	  ret= MAD_FLOW_STOP;
  }

  return ret;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

static inline
signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

static
enum mad_flow output(void *data,
		     struct mad_header const *header,
		     struct mad_pcm *pcm)
{

  unsigned int nchannels, nsamples;

#if PCM_DATA_16BIT
  mad_pcm_t const *left_ch, *right_ch;
#else
  mad_fixed_t const *left_ch, *right_ch;
#endif

  static u32 fifo_full_cnt=0;
  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];


MP3_PCM_FIFO_PUT:

	if(0==mp3_pcm_fifo_full())
	{
		u32 buf=0,size;
		size=nsamples*2;
		fifo_full_cnt=0;
		if(size<=mp3_dec_op->mp3_pcm_fifo_size)
		{
			mp3_pcm_fifo_put_buf(&buf);

#if PCM_DATA_16BIT
			memcpy((u8*)buf,left_ch,size);
			ax32xx_sysDcacheFlush(buf,size);
#else
			u8 *ptr=(u8*)buf;
			while (nsamples--) 
			{
			  signed int sample;
			  sample = scale(*left_ch++);
			  *ptr++=((sample >> 0) & 0xff);
			  *ptr++=((sample >> 8) & 0xff);
			}
			ax32xx_sysDcacheFlush(buf,size);
#endif

			mp3_pcm_fifo_put_buf_finish(size);
			//deg_Printf(".");
		}
		else
		{
			deg_Printf("pcm size err:%d\n",size);
		}
	}
	else
	{
		XOSTimeDly(20);
		//deg_Printf("pcm full\n");
		fifo_full_cnt++;
		if(fifo_full_cnt<5)
		{
			goto MP3_PCM_FIFO_PUT;
		}
	}

	if(0==mp3_dec_op->play_start)
	{
		if(mp3_pcm_fifo_get_cnt()>=mp3_dec_op->mp3_pcm_fifo_num/4)	// play 
		{
			mp3_dec_op->play_start=1;
			mp3_dacPlay_Start(mp3_dec_op->volume);
			deg_Printf("dac paly:%d\n",mp3_pcm_fifo_get_cnt());
		}
	}

	(void)nchannels;
	(void)right_ch;
  	return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */

static
enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{
/*
  Mp3_Buffer_T *mp3fp = (Mp3_Buffer_T *)data;

  deg_Printf("decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - mp3fp->fbuf);
*/
  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;

}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */
int mp3_decode()
{
  int result;

  result = mad_decoder_run();
  if(1==result)
  	{
	  if(0==mp3_pcm_fifo_get_cnt())
	  {
		  result=2;
	  }
  	}
  return result;
}


//===mp3 head===
#define ID3v2_HEADER_SIZE 10

static bool mp3_id3v2_match(u8 *buf)
{
	return  (buf[0]         ==  'I') &
		(buf[1]         ==  'D') &
		(buf[2]         ==  '3') &
		(buf[3]         != 0xff) &
		(buf[4]         != 0xff) &
		((buf[6] & 0x80) ==    0) &
		((buf[7] & 0x80) ==    0) &
		((buf[8] & 0x80) ==    0) &
		((buf[9] & 0x80) ==    0);
}

static u32 mp3_id3v2_tag_len(u8 *buf)
{
    u32 len = (((u32)buf[6] & 0x7f) << 21) +
		(((u32)buf[7] & 0x7f) << 14) +
		(((u32)buf[8] & 0x7f) << 7) +
		((u32)buf[9] & 0x7f);

    len += ID3v2_HEADER_SIZE;
    return len;
}

static void mp3_id3v2_parse(Mp3_Buffer_T *mp3_file)
{
//    u8 *buf;
    u32 id3_len=0;
    deg_Printf("mp3_id3v2_parse\n");

    if (mp3_id3v2_match(mp3_file->fbuf)) 
	{
        //ÓÐID3V2.3
		deg_Printf("MP3 ID3V2.3:%d\n",mp3_dec_op->mp3_bitstream_buf_size);
        id3_len = mp3_id3v2_tag_len(mp3_file->fbuf);
		if(NULL==mp3_file->file_buf)
		{
        	lseek(mp3_file->fd,id3_len,0);
			read(mp3_file->fd,mp3_file->fbuf,mp3_dec_op->mp3_bitstream_buf_size);
		}
		else
		{
			memcpy(mp3_file->fbuf,mp3_file->file_buf+id3_len,mp3_dec_op->mp3_bitstream_buf_size);
		}
		
        if(mp3_id3v2_match(mp3_file->fbuf))
		{
            //ÓÐID3V2.4
			deg_Printf("MP3 ID3V2.4\n");
            id3_len = mp3_id3v2_tag_len(mp3_file->fbuf);
			if(NULL==mp3_file->file_buf)
			{
	            lseek(mp3_file->fd,id3_len,0);
				read(mp3_file->fd,mp3_file->fbuf,mp3_dec_op->mp3_bitstream_buf_size);
			}
			else
			{
				memcpy(mp3_file->fbuf,mp3_file->file_buf+id3_len,mp3_dec_op->mp3_bitstream_buf_size);
			}
        }
    }

    mp3_dec_op->id3_len=id3_len;
	deg_Printf("id3_len=%d\n",mp3_dec_op->id3_len);
}


//==fd : opened mp3 file ==
//==vol :  is volume value , 0~100==
void mp3_decode_play(int fd,u8 vol)
{

	mp3_dec_op->volume=vol;
	mp3_dec_op->id3_len=0;
	mp3_dec_op->play_frame_cnt=0;
	mp3_dec_op->frame_len=mp3_dec_op->mp3_pcm_fifo_size;
	mp3_dec_op->play_sta=MP3_DAC_START;
	mp3_dec_op->play_start=0;

	mp3_pcm_fifo_init();

	mad_decoder_init(&mp3_dec_op->mad_decoder, &mp3_file,
			 input, 0 /* header */, 0 /* filter */, output,
			 error, 0 /* message */);

	//==init mp3 file==
	mp3_file.fd=fd;
	mp3_file.file_buf=NULL;
	memset(mp3_file.fbuf,0,mp3_dec_op->mp3_bitstream_buf_size);
	mp3_file.flen=fs_size(mp3_file.fd);
	if(mp3_file.flen>mp3_dec_op->mp3_bitstream_buf_size)
	{
		
		lseek(mp3_file.fd,0,0);
		read(mp3_file.fd,mp3_file.fbuf,mp3_dec_op->mp3_bitstream_buf_size);
		mp3_id3v2_parse(&mp3_file);
		if(0==mp3_dec_op->id3_len)
		{
			lseek(mp3_file.fd,0,0);
			read(mp3_file.fd,mp3_file.fbuf,mp3_dec_op->mp3_bitstream_buf_size);
		}
		mp3_file.fbsize=mp3_dec_op->mp3_bitstream_buf_size;
		mp3_file.fpos=mp3_dec_op->mp3_bitstream_buf_size;
		input(mp3_dec_op->mad_decoder.cb_data,&mp3_dec_op->mad_decoder.stream);

		u32 time=XOSTimeGet();
		hal_wdtClear();
		while(0==mp3_dec_op->play_frame_cnt)
		{
			mp3_decode();
			if(XOSTimeGet()-time>2000)
			{
				break;
			}
		}
		
		deg_Printf("samplerate=%d,bitrate=%d\n",mp3_dec_op->mad_decoder.frame.header.samplerate,mp3_dec_op->mad_decoder.frame.header.bitrate);
	}
	else
	{
		deg_Printf("mp3 file size err\n");
	}

}


//==buf : opened mp3 file buf==
//==bsize: mp3 file buf size==
//==vol :  is volume value , 0~100==
void mp3_decode_play_buf(u8 *buf,u32 bsize,u8 vol)
{
	mp3_dec_op->volume=vol;
	mp3_dec_op->id3_len=0;
	mp3_dec_op->play_frame_cnt=0;
	mp3_dec_op->frame_len=mp3_dec_op->mp3_pcm_fifo_size;
	mp3_dec_op->play_sta=MP3_DAC_START;
	mp3_dec_op->play_start=0;

	mp3_pcm_fifo_init();

	mad_decoder_init(&mp3_dec_op->mad_decoder, &mp3_file,
			 input, 0 /* header */, 0 /* filter */, output,
			 error, 0 /* message */);

	//==init mp3 file==
	mp3_file.file_buf=buf;
	if(NULL==mp3_file.file_buf)
	{
		deg_Printf("mp3 file buf err\n");
		return;
	}
	memset(mp3_file.fbuf,0,mp3_dec_op->mp3_bitstream_buf_size);
	mp3_file.flen=bsize;
	if(mp3_file.flen>mp3_dec_op->mp3_bitstream_buf_size)
	{
		memcpy(mp3_file.fbuf,mp3_file.file_buf,mp3_dec_op->mp3_bitstream_buf_size);
		mp3_id3v2_parse(&mp3_file);
		if(0==mp3_dec_op->id3_len)
		{
			memcpy(mp3_file.fbuf,mp3_file.file_buf,mp3_dec_op->mp3_bitstream_buf_size);
		}

		mp3_file.fbsize=mp3_dec_op->mp3_bitstream_buf_size;
		mp3_file.fpos=mp3_dec_op->mp3_bitstream_buf_size;
		input(mp3_dec_op->mad_decoder.cb_data,&mp3_dec_op->mad_decoder.stream);

		u32 time=XOSTimeGet();
		hal_wdtClear();
		while(0==mp3_dec_op->play_frame_cnt)
		{
			mp3_decode();
			if(XOSTimeGet()-time>2000)
			{
				break;
			}
		}
		deg_Printf("samplerate=%d,bitrate=%d\n",mp3_dec_op->mad_decoder.frame.header.samplerate,mp3_dec_op->mad_decoder.frame.header.bitrate);
	}
	else
	{
		deg_Printf("mp3 file size err\n");
	}

}

