#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "maxWav.h"
#include "maxPort.h"





#if 1

int main(int argc,char *argv[])
{

    FILE *file,*ofile;
    WAV_INFO_T wavInfo;
	USER_WAV_T userInfo;
	int ret;

	userInfo.channel = 2;
	userInfo.samplerate = 0;
	userInfo.type = WAV_TYPE_PCM;

	wavDecodeInit(&userInfo);

	if(argc<2)
	   fopen_s(&file,"test_pcm.wav","rb");
	else
       fopen_s(&file,argv[1],"rb");

	user_data_init();
	if(file == NULL)
	{
		printf("open file fail.\n");
		goto END;
	}

	ret = wavDecodeParse((WHANDLE) file);
	if(ret<0)
	{
		printf("wav decode parse fail.%d\n",ret);
		goto END;
	}

	ret = wavDecodeGetInfo(&wavInfo);

    if(ret<0)
	{
		printf("wav get info fail.\n");
		goto END;
	}
	

	printf("wav info:\n");
	printf("data type : ");
	if(wavInfo.type == WAV_TYPE_PCM)
		printf("16bit-pcm\n");
	else if(wavInfo.type == WAV_TYPE_MSADPCM)
		printf("4bit-MSadpcm\n");
	else if(wavInfo.type == WAV_TYPE_ULAW)
		printf("8bit-ulaw\n");
	else if(wavInfo.type == WAV_TYPE_ALAW)
		printf("8bit-alaw\n");
	else if(wavInfo.type == WAV_TYPE_IMADPCM)
		printf("4bit-IMAadpcm\n");
	else
		printf("unknow\n");
	printf("channel = %d,sample rate = %d,total time = %dmin:%dsec %dms\n",wavInfo.channel,wavInfo.samplerate,(wavInfo.totaltime/1000)/60,(wavInfo.totaltime/1000)%60,(wavInfo.totaltime%1000));

	unlink("rawdata.pcm");
	fopen_s(&ofile,"rawdata.pcm","wb");

	userInfo.outbufferaddr = (unsigned int )malloc(4096);
	userInfo.outbufferlen = 4096;
    printf("decode start.\n");
	while(1)
	{
		ret = wavDecodeOneFrame((WHANDLE)file,userInfo.outbufferaddr,userInfo.outbufferlen);
		if(ret<0)
			break;
		fwrite(userInfo.outbufferaddr,ret,1,ofile);
	}
	fclose(ofile);
	free((void *)userInfo.outbufferaddr);
	printf("decode end.\n");
END:
	user_data_uninit();
	if(file)
		fclose(file);
	system("pause");
	return 0;
}
#else
int main(int argc,char *argv[])
{

    FILE *file,*ofile;
    WAV_INFO_T wavInfo;
	USER_WAV_T userInfo,srcInfo;
	int blocksize,obuffer;
	int ret;

	userInfo.channel = 2;
	userInfo.samplerate = 16000;
	userInfo.type = WAV_TYPE_IMADPCM;

	srcInfo.channel = 2;
	srcInfo.samplerate = 16000;
	srcInfo.type = WAV_TYPE_PCM;

	wavEncodeInit(&userInfo,&srcInfo);

	if(argc<2)
	   fopen_s(&file,"raw.pcm","rb");
	else
       fopen_s(&file,argv[1],"rb");

	if(file == NULL)
	{
		printf("open file fail.\n");
		goto END;
	}

	unlink("imadpcmtest.wav");
	fopen_s(&ofile,"imadpcmtest.wav","wb");
	if(ofile == NULL)
	{
		printf("open file fail.\n");
		goto END;
	}

	ret = wavEncodeParse((WHANDLE) ofile);
	if(ret<0)
	{
		printf("wav encode parse fail.%d\n",ret);
		goto END;
	}

	blocksize = wavEncodeGetBlockSize();
	if(blocksize<10)
		blocksize = 2048;
	obuffer = (int )malloc(4096);
	userInfo.outbufferaddr = (int )malloc(4096);
	userInfo.outbufferlen = 4096;
    printf("encode start.\n");
	while(1)
	{
		fread(userInfo.outbufferaddr,blocksize,1,file);
		if(feof(file))
			break;
		ret = wavEncodeOneFrame((WHANDLE)ofile,userInfo.outbufferaddr,blocksize,(INT32U )obuffer);
		if(ret<0)
			break;
		fwrite(obuffer,ret,1,ofile);
	}
	
	free((void *)userInfo.outbufferaddr);
	free((void *)obuffer);
	wavEncodeEnd((WHANDLE)ofile);
    fclose(ofile);
	printf("encode end.\n");
END:
	user_data_uninit();
	if(file)
		fclose(file);
	system("pause");
	return 0;
}
#endif