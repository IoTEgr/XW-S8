#include "../media.h"
#include "../multimedia.h"
#include "pcm.h"


#if WAV_CFG_ULAW_USE > 0

const static INT16U ulawSegTable[8] ={0x3F, 0x7F, 0xFF, 0x1FF,0x3FF, 0x7FF, 0xFFF, 0x1FFF};


INT32S ulaw_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
	INT8U *inbuffer,val;
	INT16U *outbuffer,temp;
	INT32U idx,i;



	inbuffer = (INT8U *)input;
	outbuffer= (INT16U *)output;
	idx = 0;

	for(i=0;i<len;i++)
	{
        val = ~inbuffer[i];

		temp = ((val&0x0f)<<3)+0x84;
		temp <<=(val&0x70)>>4;

		if(val&0x80)
            temp = temp-0x84;
		else
			temp = 0x84-temp;
		if(chtype == MONO_TO_MONO || chtype == STREO_TO_STREO)
		{
			outbuffer[idx++] = temp;
		}
		else if(chtype == MONO_TO_STREO)
		{
			outbuffer[idx++] = temp;
			outbuffer[idx++] = temp;
		}
		else if(chtype == STRE0_TO_MONO && (i&1)==0)
		{
			outbuffer[idx++] = temp;
		}
		else
			break;
	}


	return (idx<<1);
}

INT32S ulaw_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
	INT8U *outbuffer,val;
	INT16U *inbuffer,mask,seg;
	INT32U i,idx;
	INT16S temp;

    inbuffer = (INT16U *)input;
	outbuffer= (INT8U *)output;
	idx = 0;

	if(chtype == MONO_TO_MONO || chtype == STREO_TO_STREO)
	{
		for(i=0;i<(len>>1);i++)
		{
			temp = (INT16S)inbuffer[i];
			temp>>=2;

        	if(temp>=0)
				mask = 0xff;
			else
			{
				mask = 0x7f;
				temp = -temp;
			}
			if(temp>8159)
				temp = 8159;

        	for(seg=0;seg<8;seg++)
	    	{
		  		if(temp<ulawSegTable[seg])
			 		break;
	    	}
		
			if(seg>=8)
				val = mask^0x7f;
			else
			{
				val = (seg<<4)|(temp>>(seg+1)&0x0f);
				val^=mask;
			}
            outbuffer[idx++] = val;
		}          
	}
    else if(chtype == MONO_TO_STREO)
	{
		for(i=0;i<(len>>1);i++)
		{
			temp = (INT16S)inbuffer[i];
			temp>>=2;

        	if(temp>=0)
				mask = 0xff;
			else
			{
				mask = 0x7f;
				temp = -temp;
			}
			if(temp>8159)
				temp = 8159;

        	for(seg=0;seg<8;seg++)
	    	{
		  		if(temp<ulawSegTable[seg])
			 		break;
	    	}
		
			if(seg>=8)
				val = mask^0x7f;
			else
			{
				val = (seg<<4)|(temp>>(seg+1)&0x0f);
				val^=mask;
			}
            outbuffer[idx++] = val;
			outbuffer[idx++] = val;
		} 
	}
	else if(chtype == STRE0_TO_MONO)
	{
		for(i=0;i<(len>>1);i++)
		{
			if(i&1)
				continue;
			temp = (INT16S)inbuffer[i];
			temp>>=2;

        	if(temp>=0)
				mask = 0xff;
			else
			{
				mask = 0x7f;
				temp = -temp;
			}
			if(temp>8159)
				temp = 8159;

        	for(seg=0;seg<8;seg++)
	    	{
		  		if(temp<ulawSegTable[seg])
			 		break;
	    	}
		
			if(seg>=8)
				val = mask^0x7f;
			else
			{
				val = (seg<<4)|(temp>>(seg+1)&0x0f);
				val^=mask;
			}
            outbuffer[idx++] = val;
		} 
	}
	

	return (idx);
}









#endif




