#include "../media.h"
#include "../multimedia.h"
#include "pcm.h"

#if WAV_CFG_ALAW_USE > 0

const static INT16U alawSegTable[8] = {0x1F, 0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF};


INT32S alaw_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
	INT8U *inbuffer,val;
	INT16U *outbuffer,temp,seg;
	INT32U idx,i;

	inbuffer = (INT8U *)input;
	outbuffer= (INT16U *)output;
	idx = 0;

    if(chtype == MONO_TO_MONO || chtype == STREO_TO_STREO)
	{
		for(i=0;i<len;i++)
		{
            val = inbuffer[i]^0x55;

            temp = (val&0x0f)<<4;
            seg = (val&0x70)>>4;

		    if(seg==0)
		    	temp+=8;
		    else if(seg == 1)
		    	temp+=0x108;
		    else
		    {
		    	temp+=0x108;
		    	temp<<=seg-1;
		    }
		    if(val&0x80)
                 temp = (INT16U)(-temp);

            outbuffer[idx++] = temp;
		}
	}
	else if(chtype == MONO_TO_STREO)
	{
		for(i=0;i<len;i++)
		{
            val = inbuffer[i]^0x55;

            temp = (val&0x0f)<<4;
            seg = (val&0x70)>>4;

		    if(seg==0)
		    	temp+=8;
		    else if(seg == 1)
		    	temp+=0x108;
		    else
		    {
		    	temp+=0x108;
		    	temp<<=seg-1;
		    }
		    if(val&0x80)
                 temp = (INT16U)(-temp);

            outbuffer[idx++] = temp;
			outbuffer[idx++] = temp;
		}
	}
	else if(chtype == STRE0_TO_MONO)
	{
		for(i=0;i<len;i++)
		{
			if(i&1)
				continue;
            val = inbuffer[i]^0x55;

            temp = (val&0x0f)<<4;
            seg = (val&0x70)>>4;

		    if(seg==0)
		    	temp+=8;
		    else if(seg == 1)
		    	temp+=0x108;
		    else
		    {
		    	temp+=0x108;
		    	temp<<=seg-1;
		    }
		    if(val&0x80)
                 temp = (INT16U)(-temp);

            outbuffer[idx++] = temp;
		}
	}


	return (idx<<1);
}

INT32S alaw_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype)
{
	INT8U *outbuffer,val;
	INT16U *inbuffer;
	INT32U i,idx;
	INT16S temp;

	INT32S mask,seg,;

    inbuffer = (INT16U *)input;
	outbuffer= (INT8U *)output;
	idx = 0;

	if(chtype == MONO_TO_MONO || chtype == STREO_TO_STREO)
	{
		for(i=0;i<(len>>1);i++)
		{
			temp = (INT16S)inbuffer[i];
			temp>>=3;

        	if(temp>=0)
				mask = 0xd5;
			else
			{
				mask = 0x55;
				temp = -temp-1;
			}

        	for(seg=0;seg<8;seg++)
	    	{
		  		if(temp<alawSegTable[seg])
			 		break;
	    	}
		
			if(seg>=8)
				val = mask^0x7f;
			else
			{
            	val = seg<<4;
				if(seg<2)
					val |= (temp>>1)&0x0f;
				else
					val |= (temp>>seg)&0x0f;
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
			temp>>=3;

        	if(temp>=0)
				mask = 0xd5;
			else
			{
				mask = 0x55;
				temp = -temp-1;
			}

        	for(seg=0;seg<8;seg++)
	    	{
		  		if(temp<alawSegTable[seg])
			 		break;
	    	}
		
			if(seg>=8)
				val = mask^0x7f;
			else
			{
            	val = seg<<4;
				if(seg<2)
					val |= (temp>>1)&0x0f;
				else
					val |= (temp>>seg)&0x0f;
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
			temp>>=3;

        	if(temp>=0)
				mask = 0xd5;
			else
			{
				mask = 0x55;
				temp = -temp-1;
			}

        	for(seg=0;seg<8;seg++)
	    	{
		  		if(temp<alawSegTable[seg])
			 		break;
	    	}
		
			if(seg>=8)
				val = mask^0x7f;
			else
			{
            	val = seg<<4;
				if(seg<2)
					val |= (temp>>1)&0x0f;
				else
					val |= (temp>>seg)&0x0f;
				val^=mask;
			}
            outbuffer[idx++] = val;
		} 
	}
	

	return (idx);
}








#endif


