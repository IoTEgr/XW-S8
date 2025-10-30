#include "application.h"
#include "../multimedia/mp3/inc/mp3_decode_api.h"
#include "../multimedia/unifont/ebook.h"
#include "taskAudioPlayerRes.c"

#define AUDIO_PLAY_FLASH_DEMO		0 // 0: not play flash mp3 demo  1: play flash mp3 demo

static u8* audioplayer_bk_buf;
static u16 bk_w,bk_h;

#if AUDIO_PLAY_FLASH_DEMO
static u8 *audio_demo_buf;
static  u32 audio_demo_size;
#endif

//static u8 cur_file_name[256] = {0};
TCHAR cur_file_name[/*256*/1] = {0};

#if 0
//static void get_mp3_file_name(u8 *dir, u32 idx)
 void get_mp3_file_name(u8 *dir, u32 idx)
{
	FRESULT res;
	FILINFO finfo;
	int count=0,type;
	TCHAR dirpath[128];
	char extname[8];
	TCHAR pattern[] = {'*','.','*',0};
	u8 *path;
	DIR drent;
	count = 0;
	path = dir;
	while(*path)
	{
		if((*path == '/')&&(0 == *(path+1)))
		{
			break;
		}
		dirpath[count++] = *path++;
	}
	dirpath[count++] = 0;

	count = 0;
	res = f_findfirst(&drent,&finfo,dirpath,pattern);
	while(1)
	{         
		 if(res!=FR_OK || finfo.fname[0] == 0)
		 	break;
		 if(finfo.fname[0] != '.')
		 {
			getExtName(finfo.fname, extname, sizeof(extname));
		 	 type = manangerCheckFileType(/*finfo.fname*/extname);
		 	 if(type >= 0)
			 {
				 if(FILE_TYPE_MP3==type)
				 {
				 	if(idx == count){
//						 memcpy(cur_file_name,FILEDIR_MP3,sizeof(FILEDIR_MP3));
//						 strcpy((char *)cur_file_name+strlen(FILEDIR_MP3),finfo.fname);
						int dirLen = strlen(FILEDIR_MP3);
						int bufLen = sizeof(cur_file_name)/sizeof(cur_file_name[0]);
						Ascii2Tchar(FILEDIR_MP3, cur_file_name, bufLen);
						tchNCopy(cur_file_name+dirLen, bufLen-dirLen, finfo.fname);
						 return;
					}
						
					 count++;
					 if(count>=AUDIOPLAYER_LIST_FILE_MAX)
					 {
						break;
					 }
				 }
			 }
		 }
		 res = f_findnext(&drent,&finfo);
	}
	f_closedir(&drent);
}
#endif

static u32 audioplayer_file_list_scan(u8 *dir)
{
	FRESULT res;
	FILINFO finfo;
	int count=0,type;
	TCHAR dirpath[128];
	TCHAR pattern[] = {'*','.','*',0};
	char extname[8];
	u8 *path;
	DIR drent;
    //deg_Printf("file scan : dir:%s\n",dir);
	count = 0;
	path = dir;
	while(*path)
	{
		if((*path == '/')&&(0 == *(path+1)))
		{
			break;
		}
		dirpath[count++] = *path++;
	}
	dirpath[count++] = 0;

	count = 0;
	res = f_findfirst(&drent,&finfo,dirpath,pattern); //--find first file
	while(1)
	{         
		 if(res!=FR_OK || finfo.fname[0] == 0)
		 	break;
		 if(finfo.fname[0] != '.') //--skip hide file
		 {
		 	 //deg_Printf("scan:%s\n",finfo.fname);
		 	 getExtName(finfo.fname, extname, sizeof(extname));
		 	 type = manangerCheckFileType(/*finfo.fname*/extname);
		 	 if(type<0)
		 	 {
				//deg_Printf("unknow type:%s extname:%s\n",finfo.fname,extname);
		 	 }
			 else
			 {
			 	 //deg_Printf("mp3:%s  extname:%s\n",finfo.fname,extname);
				 if(FILE_TYPE_MP3==type)
				 {
					 //deg_Printf("mp3 file\n");
					//deg_Printf("scan:%s  extname:%s\n",finfo.fname,extname);
#if 0
					 if(strlen(finfo.fname)<AUDIOPLAYER_NAME_LEN_MAX)
					 {
						 strcpy((char *)audio_player_list.file_list->name[count],finfo.fname);
					 }
#else
					tchNCopy(audio_player_list.file_list->name[count], AUDIOPLAYER_NAME_LEN_MAX, finfo.fname);
#endif
					 count++;
					 if(count>=AUDIOPLAYER_LIST_FILE_MAX)
					 {
						break;
					 }
				 }
			 }
		 }
		 res = f_findnext(&drent,&finfo);

	}
	f_closedir(&drent);

	return count;	

}

extern s8 audio_Volume;
extern u32 mp3_list_idx;
extern u32 mp3_list_num;

static void audioplayer_play_mp3()
{
	//==play mp3==
	if((SDC_STAT_NORMAL==SysCtrl.sdcard)||(SDC_STAT_FULL==SysCtrl.sdcard))	// play sd file  
	{
		if(audio_player_list.list_num>0)
		{
#if 0
			mp3_list_idx=audio_player_list.list_idx;
			mp3_list_num=audio_player_list.list_num;
			get_mp3_file_name((u8 *)FILEDIR_MP3, audio_player_list.list_idx);
			deg_Printf("MP3 : %d.",audio_player_list.list_idx);
			deg_Printf("MP3 : %d.",audio_player_list.cur_file);
			//deg_Printf("MP3 : %s.",audio_player_list.list_idx);
			//deg_Printf("MP3 : %s.",audio_player_list.cur_file);
#endif
			//==open mp3 file and play==
			if(MP3_DAC_STOP!=mp3_play_sta())
			{
				mp3_dacPlay_Stop();
			}

			if(audio_player_list.cur_file>=0)
			{
				close(audio_player_list.cur_file);
				audio_player_list.cur_file=-1;
			}

#if 0
			u8 filepath[256];
			memset(filepath,0,sizeof(filepath));
			memcpy(filepath,FILEDIR_MP3,sizeof(FILEDIR_MP3));
			strcpy((char *)filepath+strlen(FILEDIR_MP3),(char *)audio_player_list.file_list->name[audio_player_list.list_idx]);
#else
			TCHAR filepath[128];
			int dirLen = strlen(FILEDIR_MP3);
			int bufLen = sizeof(filepath)/sizeof(filepath[0]);
			Ascii2Tchar(FILEDIR_MP3,filepath,bufLen);
			tchNCopy(filepath+dirLen, bufLen-dirLen, audio_player_list.file_list->name[audio_player_list.list_idx]);
#endif

			audio_player_list.cur_file=(s32)open(filepath,FA_READ);
			if(audio_player_list.cur_file < 0)
			{
				deg_Printf("open [%s] error\n",filepath);
			}
			deg_Printf("--VOL:%d\n",VOLUME_LEVEL*audio_Volume);
			mp3_decode_play(audio_player_list.cur_file,VOLUME_LEVEL*audio_Volume);
			audio_player_list.list_play_idx=audio_player_list.list_idx;

			audio_player_list.total_time = mp3_file_time_cal();
			audio_player_list.play_time = 0;
			deg_Printf("mp3_play_time=%d,%d\n",audio_player_list.play_time,audio_player_list.total_time);

		}
	}
	else
	{
		#if AUDIO_PLAY_FLASH_DEMO
		//==play mp3 demo==
		u32 addr,size;
		addr= nv_open(RES_AUDIO_PLAYER_DEMO);
		if(addr<0)
		{
			deg_Printf("read id err\n");
		}
		size = nv_size(RES_AUDIO_PLAYER_DEMO);
		if((size<64)||(size>2*1024*1024))
		{
			deg_Printf("size err:%d\n",size);
		}
		
		if(NULL==audio_demo_buf)
		{
			audio_demo_size=size;
			audio_demo_buf=hal_sysMemMalloc(audio_demo_size,32);
		}

		if(audio_demo_buf)
		{
			deg_Printf("demo size:%d\n",audio_demo_size);
			nv_read(addr,audio_demo_buf,audio_demo_size);

			mp3_decode_play_buf(audio_demo_buf,audio_demo_size,AUDIO_MP3_VOLUME);

			audio_player_list.total_time = mp3_file_time_cal();
			audio_player_list.play_time = 0;
			deg_Printf("mp3_play_time=%d,%d\n",audio_player_list.play_time,audio_player_list.total_time);

		}
		else
		{
			deg_Printf("mem err:%d\n",audio_demo_size);
		}
		#endif
	}

}


static void audioplayer_showbuf()
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,p_lcd_buffer->w,p_lcd_buffer->h);
		//==draw bk==
		memcpy(p_lcd_buffer->y_addr,audioplayer_bk_buf,bk_w*bk_h*3/2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}

static int audioPlayerKeyMsgList(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		return 0;
		deg("press key photo ");
		if(audio_player_list.list_show_en)
		{
			audio_player_list.list_show_en=0;
		}
		else
		{
			audio_player_list.list_show_en=1;
			audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
		}
		deg("show list_show_en[%d]\n",audio_player_list.list_show_en);
		audioPlayerFileListShow(handle);
	}
	return 0;
}

static int audioPlayerKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	//static u8 ListEnFlag = 0;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audio_player_list.list_show_en)
		{
			deg_Printf("11111111111\n");
			audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
			//if(ListEnFlag == 0)
			if(MP3_DAC_PAUSE==mp3_play_sta())
			{
				#if 1
					deg("list_show_en == 1, mp3 123465\n");
					while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}
					mp3_dacPlay_Start(VOLUME_LEVEL*audio_Volume);
					audioPlayerStaShow(handle,1);
				#else
					audioplayer_play_mp3();
					audioPlayerStaShow(handle,1);
				#endif
			}
			else if(MP3_DAC_START==mp3_play_sta())
			{
				deg("list_show_en == 1,mp3 pause\n");
				mp3_dacPlay_Pause();
				audioPlayerStaShow(handle,0);
			}else
			{
				deg("list_show_en == 1,mp3 play\n");
				audioplayer_play_mp3();
				if((audio_player_list.list_num==0)||(SysCtrl.sdcard == SDC_STAT_NULL))
				{
					audioPlayerStaShow(handle,0);
					audioPlayerMsgfile(handle,0,0);
				}
				else
				audioPlayerStaShow(handle,1);
			}
			//ListEnFlag ^= 1;
			
		}
		else
		{
			if(MP3_DAC_START==mp3_play_sta())
			{
				deg("list_show_en == 0,mp3 pause\n");
				mp3_dacPlay_Pause();
				audioPlayerStaShow(handle,0);
			}
			else if(MP3_DAC_PAUSE==mp3_play_sta())
			{
				deg_Printf("2222222222222\n");
				while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}
				mp3_dacPlay_Start(VOLUME_LEVEL*audio_Volume);
				audioPlayerStaShow(handle,1);
			}
			else
			{
				deg_Printf("list_show_en == 0,mp3 play\n");
				audioplayer_play_mp3();
				if((audio_player_list.list_num==0)||(SysCtrl.sdcard == SDC_STAT_NULL))
				{
					audioPlayerStaShow(handle,0);
					audioPlayerMsgfile(handle,0,0);
				}
				else
				audioPlayerStaShow(handle,1);
			}
		}
		audioPlayerTimeShow(handle);
	}
	return 0;
}

s8 audio_Volume=10;
#if 0
static int audioPlayerVolumeUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
			audio_Volume++;
			if(audio_Volume>10)audio_Volume=10;
		    audioPlaybackSetVolume(VOLUME_LEVEL*audio_Volume);
			audioPlayerVolumeShow(handle,1);
	}
	return 0;
}

static int audioPlayerVolumeDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
			audio_Volume--;
			if(audio_Volume<0)audio_Volume=0;
		    audioPlaybackSetVolume(VOLUME_LEVEL*audio_Volume);
			audioPlayerVolumeShow(handle,1);
	}
	return 0;
}
#endif

static int volume_adjust_flag = 0;
static int audioPlayerKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		volume_adjust_flag = 1;
		audio_Volume++;
		if(audio_Volume>10)audio_Volume=10;
	    audioPlaybackSetVolume(VOLUME_LEVEL*audio_Volume);
		audioPlayerVolumeShow(handle,1);
	}
	return 0;
}

static int audioPlayerKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		volume_adjust_flag = 1;
		audio_Volume--;
		if(audio_Volume<0)audio_Volume=0;
	    audioPlaybackSetVolume(VOLUME_LEVEL*audio_Volume);
		audioPlayerVolumeShow(handle,1);
	}
	return 0;
}

static int audioPlayerKeyMsgLeft(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
		if(parameNum==1)
			keyState=parame[0];
		if(keyState==KEY_RELEASE)
		{
			if(volume_adjust_flag == 1)
			{
				volume_adjust_flag = 0;
				return 0;
			}
			
			if(audio_player_list.list_num>0)
			{
				if(audio_player_list.list_idx>0)
				{
					audio_player_list.list_idx--;
				}
				else
				{
					audio_player_list.list_idx=audio_player_list.list_num-1;
				}
				
				if(audio_player_list.list_show_en)
				{
					audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
					audioPlayerFileListShow(handle);
					
					audioplayer_play_mp3();
					audioPlayerStaShow(handle,1);
				}
				else
				{
					if(MP3_DAC_START==mp3_play_sta())
					{
						audioplayer_play_mp3();
						audioPlayerStaShow(handle,1);
						audioPlayerTimeShow(handle);
					}
				}
			}
		}

}

static int audioPlayerKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
		if(parameNum==1)
			keyState=parame[0];
		if(keyState==KEY_RELEASE)
		{
			if(volume_adjust_flag == 1)
			{
				volume_adjust_flag = 0;
				return 0;
			}
			
			if(audio_player_list.list_num>0)
			{
				if(audio_player_list.list_idx<audio_player_list.list_num-1)
				{
					audio_player_list.list_idx++;
				}
				else
				{
					audio_player_list.list_idx=0;
				}
	
				if(audio_player_list.list_show_en)
				{
					audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
					audioPlayerFileListShow(handle);
	
					audioplayer_play_mp3();
					audioPlayerStaShow(handle,1);
				}
				else
				{
					if(MP3_DAC_START==mp3_play_sta())
					{
						audioplayer_play_mp3();
						audioPlayerStaShow(handle,1);
						audioPlayerTimeShow(handle);
					}
				}
			}
		}

}

extern menu MENU(playBack);
#define  VIDEO_PLAY_WAIT_KEYSOUND_END2           while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}

#if 0
static int audioPlayerKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//if((videoPlaybackGetStatus() != MEDIA_STAT_START)&&(managerFileCount(SysCtrl.avi_list)>0))
		{
			//hal_dacPlayStop();
				mp3_dacPlaySetStaPause();
				audioPlayerStaShow(handle,0);deg_Printf("========222============\n");
				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_ROTATE,0));
				XOSTimeDly(50);  // delay 80ms,wait lcd frame end	
			//videoPlaybackStop();
			//deamon_keysound_play();
			//VIDEO_PLAY_WAIT_KEYSOUND_END2;
			uiOpenWindow(&delCurWindow,0);
		}
		
	}
	return 0;
}
#endif


static int audioPlayerKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		#if BT_MODE
		taskStart(TASK_GAME_MENU,0);
		#else
		taskStart(TASK_MAIN_MENU,0);
		#endif
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

static int audioPlayerSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioPlayerSDShow(handle);
	if((SDC_STAT_NULL==SysCtrl.sdcard)||(SDC_STAT_ERROR==SysCtrl.sdcard))
	{
		if(MP3_DAC_STOP!=mp3_play_sta())
		{
			mp3_dacPlay_Stop();
		}

		audio_player_list.list_num=0;
		audio_player_list.list_idx=0;
		audio_player_list.list_show_en=0;

		audio_player_list.play_time=0;
		audio_player_list.total_time=0;
		audioPlayerFileListShow(handle);
		audioPlayerStaShow(handle,0);
		audioPlayerTimeShow(handle);
	}
	else if(SDC_STAT_NORMAL==SysCtrl.sdcard)
	{
		if(NULL!=audio_player_list.file_list)
		{
			memset(audio_player_list.file_list,0,sizeof(struct AudioPlayerMp3NameList));
			audio_player_list.list_num=audioplayer_file_list_scan((u8*)FILEDIR_MP3);
			audio_player_list.list_idx=0;
			deg_Printf("audio_player_list.list_num=%d\n",audio_player_list.list_num);
			if(audio_player_list.list_num>0)
			{
				audio_player_list.list_show_en=1;
				audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;			
			}

			//==play mp3==
			audioplayer_play_mp3();
		
			//==osd show==
			audioPlayerSDShow(handle);
			audioPlayerBaterryShow(handle);
			if(MP3_DAC_START==mp3_play_sta())
			{
				audioPlayerStaShow(handle,1);
			}
			else
			{
				audioPlayerStaShow(handle,0);
			}
			
			audioPlayerTimeShow(handle);
			audioPlayerFileListShow(handle);

		}
	}

	return 0;
}


extern u8 mp3_flag;
extern u8 mp3_flag2;

static int audioPlayerOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	mp3_flag=1;
	deg_Printf("audioPlayer Open Win:%d\n",XOSTimeGet());
	u16 i/*,ret*/;
	memset(&audio_player_list,0,sizeof(struct AudioPlayerList));
	audio_player_list.cur_file=-1;

	Ebook_Init();
//	ret=MP3_unicode_font_init();
//	if(0==ret)
//	{
//		SysCtrl.mp3_font_use=1;
//	}
//	deg_Printf("mp3 font:%d\n",ret);

	//==video buf init and draw bkground====
	hal_lcdGetResolution(&bk_w,&bk_h);
	audioplayer_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	if(NULL!=audioplayer_bk_buf)
	{
		#if MAIN_MENU_UI
		jpg_decode_buf(RES_GAMEMENU_BK/*RES_MP3_BK*/,audioplayer_bk_buf,audioplayer_bk_buf+bk_w*bk_h,bk_w,bk_h);
		#else
		jpg_decode_buf(RES_GAMEMENU_BK,audioplayer_bk_buf,audioplayer_bk_buf+bk_w*bk_h,bk_w,bk_h);
		#endif

	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}
	audioplayer_showbuf();
	deg_Printf("show time:%d\n",XOSTimeGet());

	//==mp3 list name init==
	audio_player_list.file_list=hal_sysMemMalloc(sizeof(struct AudioPlayerMp3NameList),64);
	if(NULL==audio_player_list.file_list)
	{
		deg_Printf("mp3 list name  malloc fail\n");
	}
	else
	{
		memset(audio_player_list.file_list,0,sizeof(struct AudioPlayerMp3NameList));
	}

	if((SDC_STAT_NORMAL==SysCtrl.sdcard)||(SDC_STAT_FULL==SysCtrl.sdcard))	// play sd file  
	{
		audio_player_list.list_num=audioplayer_file_list_scan((u8*)FILEDIR_MP3);
		deg_Printf("audio_player_list.list_num=%d\n",audio_player_list.list_num);

		//==debug name==
		for(i=0;i<audio_player_list.list_num;i++)
			deg_Printf("list_name[%d]=%s\n",i,audio_player_list.file_list->name[i]);

		if(audio_player_list.list_num>0)
		{
			audio_player_list.list_show_en=1;
			audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;			
		}
	}
	
	//==play mp3==
	audioplayer_play_mp3();
	//==osd show==
	audioPlayerSDShow(handle);
	audioPlayerBaterryShow(handle);
	if(MP3_DAC_START==mp3_play_sta())
	{
		audioPlayerStaShow(handle,1);
	}
	else
	{
		audioPlayerStaShow(handle,0);
	}
	audioPlayerVolumeShow(handle,1);
	audioPlayerTimeShow(handle);
	audioPlayerFileListShow(handle);

	return 0;
}

static int audioPlayerCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("audioPlayer Close Win!!!\n");
	mp3_flag=0;
	if(NULL!=audio_player_list.file_list)
	{
		hal_sysMemFree(audio_player_list.file_list);
		audio_player_list.file_list=NULL;
	}

	if(NULL!=audioplayer_bk_buf)
	{
		hal_sysMemFree(audioplayer_bk_buf);
		audioplayer_bk_buf=NULL;
	}

#if AUDIO_PLAY_FLASH_DEMO
	if(NULL!=audio_demo_buf)
	{
		hal_sysMemFree(audio_demo_buf);
		audio_demo_buf=NULL;
	}
#endif

	if(audio_player_list.cur_file>=0)
	{
		close(audio_player_list.cur_file);
		audio_player_list.cur_file=-1;
	}

//	if(SysCtrl.mp3_font_use)
//	{
//		SysCtrl.mp3_font_use=0;
//		MP3_unicode_font_uninit();
//	}

	Ebook_Uninit();


	return 0;
}

static int audioPlayerSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioPlayerBaterryShow(handle);
	return 0;
}

static int audioPlayerSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		audioPlayerBaterryShow(handle);
	return 0;
}

static int audioPlayerSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(MP3_DAC_START==mp3_play_sta())
	{
		audioPlayerTimeShow(handle);
		audioPlayerFileBkShow(handle);
	}

if(mp3_flag2){mp3_flag2=0;
	//audioPlayerSysMsgdele(handle);
	}
	return 0;
}

static int audioPlayerSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{
		audioPlayerBaterryShow(handle);	
	}

/*	if(audio_player_list.list_show_en)
	{
		if(audio_player_list.list_show_time>0)
		{
			audio_player_list.list_show_time--;
			if(0==audio_player_list.list_show_time)
			{
				audio_player_list.list_show_en=0;
				audio_player_list.list_idx=audio_player_list.list_play_idx;
				audioPlayerFileListShow(handle);
			}
		}
	}
*/

	return 0;
}

static int audioPlayerSysMsgPlayFinish(winHandle handle,uint32 parameNum,uint32* parame)
{
	u8 ret=0;
	if(parameNum==1)
		ret=parame[0];
	
	deg_Printf("audioPlayerSysMsgPlayFinish:%d,%d\n",parameNum,ret);
	if(ret)
	{
		if(audio_player_list.list_num>0)
		{
			if(audio_player_list.list_idx<audio_player_list.list_num-1)
			{
				audio_player_list.list_idx++;
			}
			else
			{
				audio_player_list.list_idx=0;
			}

			audioplayer_play_mp3();
			audioPlayerStaShow(handle,1);
			
			//show  auto list follow
			audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
			audioPlayerFileListShow(handle);
		}
		else
		{
			audioPlayerTimeShow(handle);
			audioPlayerStaShow(handle,0);
		}
	}
	return 0;
}

static int playCount = 0;
static int audioPlayerSysMsgPlay(winHandle handle,uint32 parameNum,uint32* parame)
{
//	u8 ret = 0;
//	if(parameNum == 1)
//		ret = parame[0];

	playCount++;
	if(playCount>5)
	{
		playCount = 0;
		if(MP3_DAC_START==mp3_play_sta())
		{
			audioPlayerFileBkShow(handle);

		}
	}
	return 0;
}
msgDealInfor audioPlayerMsgDeal[]=
{
	{SYS_OPEN_WINDOW,audioPlayerOpenWin},
	{SYS_CLOSE_WINDOW,audioPlayerCloseWin},
	
	{KEY_EVENT_UP,audioPlayerKeyMsgUp},
	{KEY_EVENT_DOWN,audioPlayerKeyMsgDown},
	//{KEY_EVENT_RECORD,audioPlayerVolumeUp},
	//{KEY_EVENT_POWER,audioPlayerVolumeDown},

	{KEY_EVENT_LEFT,audioPlayerKeyMsgLeft},
	{KEY_EVENT_RIGHT,audioPlayerKeyMsgRight},

	//{KEY_EVENT_DELETE,audioPlayerKeyMsgMenu},
	 
	{KEY_EVENT_RECORD,audioPlayerKeyMsgList},
	
	//{KEY_EVENT_PHOTO,audioPlayerKeyMsgList},
	
	{KEY_EVENT_OK,audioPlayerKeyMsgOk},
	{KEY_EVENT_RETURN,audioPlayerKeyMsgExit},
	{SYS_EVENT_500MS,audioPlayerSysMsg500MS},
	{SYS_EVENT_1S,audioPlayerSysMsg1S},
	{SYS_EVENT_SDC,audioPlayerSysMsgSD},
	{SYS_EVENT_USB,audioPlayerSysMsgUSB},
	{SYS_EVENT_BAT,audioPlayerSysMsgBattery},
	{MP3_EVENT_PLAY_FINISH,audioPlayerSysMsgPlayFinish},
	{MP3_EVENT_PLAY,audioPlayerSysMsgPlay},
	{EVENT_MAX,NULL},
};

WINDOW(audioPlayerWindow,audioPlayerMsgDeal,audioPlayerWin)



