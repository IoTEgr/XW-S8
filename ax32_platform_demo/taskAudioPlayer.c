#include "application.h"


u16* uni2oem936_data_buf=NULL;
u16* oem2uni936_data_buf=NULL;

void uni2oem936_init( void )
{
	int32 i,size;
	
	i = nv_open(RES_UNI2OEM936);
	if(i<0)
	{
		deg_Printf("open str error.\n");
	}else{
		deg_Printf("str open\n");
	}
	size = nv_size(RES_UNI2OEM936);
	
	if(uni2oem936_data_buf == NULL)
	{
		uni2oem936_data_buf=hal_sysMemMalloc(size,32);
	}
	deg_Printf("uni2oem936 size=%d.\n",size);
	
	nv_read(i,uni2oem936_data_buf,size);
}
void uni2oem936_uninit( void )
{
	if(NULL!=uni2oem936_data_buf)
	{
		hal_sysMemFree(uni2oem936_data_buf);
		uni2oem936_data_buf=NULL;
	}
}

void oem2uni936_init( void )
{
	int32 i,size;
	
	i = nv_open(RES_OEM2UNI936);
	if(i<0)
	{
		deg_Printf("open str error.\n");
	}
	size = nv_size(RES_OEM2UNI936);
	deg_Printf("oem2uni936 size=%d.\n",size);

	if(oem2uni936_data_buf == NULL)
	{
		oem2uni936_data_buf=hal_sysMemMalloc(size,32);
	}
	nv_read(i,oem2uni936_data_buf,size);
}
void oem2uni936_uninit( void )
{
	if(NULL!=oem2uni936_data_buf)
	{
		hal_sysMemFree(oem2uni936_data_buf);
		oem2uni936_data_buf=NULL;
	}
}

void taskAudioPlayerOpen(uint32 arg)
{
	deg_Printf("AudioPlayer open\n");

	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);

	deg_Printf("remain space before init uni:%d\n",hal_sysMemRemain());
	uni2oem936_init();
	oem2uni936_init();
	deg_Printf("remain space after init uni:%d\n",hal_sysMemRemain());
	s32 ret=mp3_dac_memInit(MP3_PCM_FIFO_NUM,MP3_PCM_FIFO_SIZE,MP3_BITSTREAM_BUFSIZE);
	if(0!=ret)
	{
		deg_Printf("mp3 malloc err\n");
	}

	dispLayerInit(DISP_LAYER_OSD0);  
	
	uiOpenWindow(&audioPlayerWindow,0);
}

void taskAudioPlayerService(uint32 arg)
{
	if(MP3_DAC_START==mp3_play_sta())
	{
		int ret=mp3_decode();
		//deg_Printf("ret=%d,",ret);
		if(2==ret)
		{
			XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(MP3_EVENT_PLAY_FINISH,ret));
		}else{
			//XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(MP3_EVENT_PLAY,ret));
		}
	}
}

void taskAudioPlayerClose(uint32 arg)
{
	deg_Printf("AudioPlayer exit.\n");
	hal_dacPlayStop();
	mp3_dac_memUinit();

	uni2oem936_uninit();
	oem2uni936_uninit();
}

sysTask taskAudioPlayer=
{
	"Audio Player",
	(u32)NULL,
	taskAudioPlayerOpen,
	taskAudioPlayerClose,
	taskAudioPlayerService,
};


