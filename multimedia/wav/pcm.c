#include "../media.h"
#include "../multimedia.h"
#include "pcm.h"


#if WAV_CFG_PCM_USE>0

INT32S pcm_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
	INT32U i,cnt;
	INT16U *outbuffer;//*buffer,

	if(/*input == (void *)0 ||*/ output == (void *)0)
		return -1;

	outbuffer = (INT16U *)output;
//	buffer    = (INT16U *)input;
    cnt = len;
    if(chtype == MONO_TO_MONO || chtype == STREO_TO_STREO)
	{
		ringBufferRead((INT8U *)outbuffer,cnt);
        //for(i=0;i<(cnt>>1);i++)
        //   outbuffer[i] = buffer[i];
	}
	else if(chtype == MONO_TO_STREO)
	{
        for(i=0;i<(cnt>>1);i++)
		{
			ringBufferRead((INT8U *)&outbuffer[(i<<1)+0],2);
			outbuffer[(i<<1)+1] = outbuffer[(i<<1)+0];
			//outbuffer[(i<<1)+0] = buffer[i];
			//outbuffer[(i<<1)+1] = buffer[i];
		}
		cnt<<=1;
	}
	else if(chtype == STRE0_TO_MONO)
	{
        for(i=0;i<(cnt>>1);)
		{
			ringBufferRead((INT8U *)&outbuffer[i>>1],4);
			//outbuffer[i>>1] = buffer[i];
			i+=2;
		}
		cnt>>=1;
	}
	else
		return -1;

	return cnt;
}

INT32S pcm_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
	INT32U i,cnt;
	INT16U *buffer,*outbuffer;

	if(input == (void *)0 || output == (void *)0)
		return -1;

	outbuffer = (INT16U *)output;
	buffer    = (INT16U *)input;
    cnt = len;
    if(chtype == MONO_TO_MONO || chtype == STREO_TO_STREO)
	{
        for(i=0;i<(cnt>>1);i++)
           outbuffer[i] = buffer[i];
	}
	else if(chtype == MONO_TO_STREO)
	{
        for(i=0;i<(cnt>>1);i++)
		{
			outbuffer[(i<<1)+0] = buffer[i];
			outbuffer[(i<<1)+1] = buffer[i];
		}
		cnt<<=1;
	}
	else if(chtype == STRE0_TO_MONO)
	{
        for(i=0;i<(cnt>>1);)
		{
			outbuffer[i>>1] = buffer[i];
			i+=2;
		}
		cnt>>=1;
	}
	else
		return 0;

	return cnt;
}





#endif


