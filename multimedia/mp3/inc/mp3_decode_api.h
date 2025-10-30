
#ifndef MP3_DECODE_API_H
#define MP3_DECODE_API_H

#include "../../../hal/inc/hal.h"  

#include 	"config.h"

#include	"global.h"

#include	"mad.h"

#include	"huffman.h"
#include	"layer12.h"
#include	"layer3.h"

#include 	"../../media.h"
#include 	"../../multimedia.h"


#define  MP3_BITSTREAM_BUFSIZE		(4096)	// mp3 data 
#define MP3_PCM_FIFO_NUM	  64
#define MP3_PCM_FIFO_SIZE   (1152*2)


typedef struct MP3_DEC_OP_S
{
	struct mad_decoder mad_decoder;
	u32 mp3_bitstream_buf_size;
	u8 *mp3_pcm_buf;
	u32 mp3_pcm_buf_size;
	u32 mp3_pcm_fifo_num;
	u32 mp3_pcm_fifo_size;
	u8 *dac_mute_buf;
	
	u32 id3_len;
	u32 volume;
	u32 play_frame_cnt;
	u32 frame_len;
	u8 play_start;		// 0: not start play , 1: start play
	u8 play_sta;		// 0: stop, 1: start, 2: pause
	
}MP3_DEC_OP_T;


extern MP3_DEC_OP_T *mp3_dec_op;

enum
{
	MP3_DAC_STOP = 0,
	MP3_DAC_START,
	MP3_DAC_PAUSE,
	MP3_DAC_RESTART,
};

//==pcmsize : must be 1152*2==
//==mp3_buf_size: must >= 2048
int mp3_dac_memInit(u32 pcm_fifo_num,u32 pcm_fifo_size,u32 mp3_buf_size);
void mp3_dac_memUinit(void);


int mp3_dacPlay_Start(u8 vol);
int mp3_dacPlay_Pause(void);
int mp3_dacPlay_Stop(void);


//==ret : 0 not play , 1: playing==
u8 mp3_play_sta(void);


u32 mp3_flie_playtime();

u32 mp3_file_time_cal();

//==ret: 0: normal , 1: decode finish, play not finish, 2: decode finish,play finish , other : err==
int mp3_decode();


//==fd : opened mp3 file ==
//==vol :  is volume value , 0~100==
void mp3_decode_play(int fd,u8 vol);

//==buf : opened mp3 file buf==
//==bsize: mp3 file buf size==
//==vol :  is volume value , 0~100==
void mp3_decode_play_buf(u8 *buf,u32 bsize,u8 vol);


#endif /* AVFORMAT_AVIO_H */

