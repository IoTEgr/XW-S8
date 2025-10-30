#include "application.h" 


int intToString(char* str,uint8 len,uint32 data)
{
	char tmp[11];
	int n=0,i=0;
	if(data)
	{
		while(data)
		{
			tmp[n]=(data%10)+'0';
			data=data/10;
			n++;
		}
		len--;
		for(i=0;i<n;i++)
		{
			str[i]=tmp[n-i-1];
			if(i>=len)
			{
				str[i]=0;
				return i-1;
			}
		}
		str[i]=0;
		return i;
	}
	else
	{
		*str++='0';
		*str=0;
		return 1;
	}
	return n;
}



