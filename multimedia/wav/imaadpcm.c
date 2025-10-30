#include "../media.h"
#include "../multimedia.h"
#include "pcm.h"

#if WAV_CFG_ADPCM_USE > 0
const static INT16S adpcmIndexTable[16]=
{
	-1,-1,-1,-1,2,4,6,8,-1,-1,-1,-1,2,4,6,8
};
const static INT32S adpcmStepTable[89]=
{
	7,8,9,10,11,12,13,14,16,17,
    19,21,23,25,28,31,34,37,41,45,
    50,55,60,66,73,80,88,97,107,118,
    130,143,157,173,190,209,230,253,279,307,
    337,371,408,449,494,544,598,658,724,796,
    876,963,1060,1166,1282,1411,1552,1707,1878,2066,
    2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,
    5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,
    15289,16818,18500,20350,22385,24623,27086,29794,32767
};

static INT8U adpcmFlag = 0;
static INT16S preSample,preIndex;

INT32S imadpcm_reset(void)
{
     preSample = 0;
	 preIndex = 0;

	 return 0;
}


INT32S imaadpcm_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
    INT16S idx,step;
    INT16U *outbuffer,sample,i;
	INT16S value;
	INT8U *inbuffer,halcode,samL,samH;
//	INT16S index;
	INT16S diff,com1;
    static INT16S presample,index;

	idx = 0;
    com1 = 32767;
	outbuffer = (INT16U *)output;
	inbuffer  = (INT8U *)&input[2];
//--------block header
	presample = input[0];
    index = input[1]&0xff;
    len = (len-4)<<1;
    if(adpcmFlag==0)
	{
		index= 0;
        presample = 0;
	}
	adpcmFlag = 1;
    idx = 0;
	outbuffer[idx++] = presample;

    for(i=0;i<len;i++)
	{
		//temp = (INT16S)presample;

		if(i&1)
           halcode = inbuffer[i>>1]&0x0f;
		else
			halcode = (inbuffer[i>>1]&0xf0)>>4;
        step = adpcmStepTable[index];
        
        diff = step>>3;
		if(halcode&0x04)
			diff+=step;
		if(halcode&0x02)
			diff+=step>>1;
		if(halcode&0x01)
			diff+=step>>2;
        if(halcode&0x08)
		{
			presample-=diff;

		}
		else
		{
			presample+=diff;
		}

        if(presample>32767)
			value=32767;
		else if(presample<(int)-32768)
			value=-32768;
		else/**/
			value = presample;

        presample = value;
		index+=adpcmIndexTable[halcode];
		if(index<0)
			index = 0;
		else if(index>88)
			index = 88;
        
		//if(presample>=0)
		{
			samH = value>>8;
			samL = value&0xff;
			
		}
		/*else
		{
			samH = (presample+32768)>>8;
			samL = (presample+32768)&0xff;
			samH+=0x80;
		}*/
		sample = samH;
		sample = (sample<<8)+samL;
		

		if(chtype == MONO_TO_MONO || chtype == STREO_TO_STREO)
		{
			outbuffer[idx++] = sample;
		}
		else if(chtype == MONO_TO_STREO)
		{
			outbuffer[idx++] = sample;
			outbuffer[idx++] = sample;
		}
		else if(chtype == STRE0_TO_MONO && (i&1)==0)
		{
			outbuffer[idx++] = sample;
		}
		else
			break;
	}

	return (idx<<1);
}

INT32S imaadpcm_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
    INT16S idx;
    INT16U *inbuffer,i;
	INT8U *outbuffer;
	INT8U samH,halcode;
	INT16S diff,com1;

    outbuffer = (INT8U *)output;
	inbuffer  = (INT16U *)input;

    idx = 0;
	outbuffer[idx++] = inbuffer[0]>>8;
	outbuffer[idx++] = inbuffer[0]&0xff;
	outbuffer[idx++] = 0;//preIndex;
    outbuffer[idx++] = 0;

    preSample = inbuffer[0];
	preIndex  = 0;
    for(i=1;i<(len>>1);i++)
	{
        diff = inbuffer[i]-preSample;
		preSample = inbuffer[i];
        if(diff<0)
		{
			diff = -diff;
			com1 = 8;
		}
		else
			com1 = 0;

		halcode = (diff<<2)/adpcmStepTable[preIndex];
		
		halcode &= 7;
        preIndex+=adpcmIndexTable[halcode];
		if(preIndex<0)
			preIndex = 0;
		else if(preIndex>88)
			preIndex = 88;

        
         samH = (samH<<4)|halcode|com1;

		 if(i&2)
            outbuffer[idx++] = samH;
	}

	return idx;
}










#endif

