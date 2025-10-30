#include"uiWin.h"

enum
{
	AUDIOPLAYER_ICON_ID=0,
	AUDIOPLAYER_SD_ID,
	AUDIOPLAYER_BATERRY_ID,

	//==file list==
	AUDIOPLAYER_LIST_BK_ID,
	AUDIOPLAYER_LIST_TIPS_ID,
	AUDIOPLAYER_LIST_ITEM1_ID,
	AUDIOPLAYER_LIST_ITEM2_ID,
	AUDIOPLAYER_LIST_ITEM3_ID,
	AUDIOPLAYER_LIST_ITEM4_ID,
	AUDIOPLAYER_LIST_ITEM5_ID,
	AUDIOPLAYER_LIST_ITEM6_ID,
	AUDIOPLAYER_LIST_STR1_ID,
	AUDIOPLAYER_LIST_STR2_ID,
	AUDIOPLAYER_LIST_STR3_ID,
	AUDIOPLAYER_LIST_STR4_ID,
	AUDIOPLAYER_LIST_STR5_ID,
	AUDIOPLAYER_LIST_STR6_ID,
	AUDIOPLAYER_LIST_FILE_NUM_ID,

	AUDIOPLAYER_VOLUME_NUM_ID,
	AUDIOPLAYER_IMAGE_ID,
	AUDIOPLAYER_STA_ID,
	AUDIOPLAYER_PLAY_TIME_ID,
	AUDIOPLAYER_MAX_ID
};

#define AUDIO_MP3_VOLUME		100	

#define AUDIOPLAYER_LIST_NUM		6
#define AUDIOPLAYER_LIST_SHOW_TIME 	6

#define AUDIOPLAYER_LIST_FILE_MAX		100//500
#define AUDIOPLAYER_NAME_LEN_MAX		128

#define AUDIOPLAYER_DEC_CNT		32

/*typedef*/ struct AudioPlayerMp3NameList
{
	TCHAR name[AUDIOPLAYER_LIST_FILE_MAX][AUDIOPLAYER_NAME_LEN_MAX+1];
};

/*typedef*/ struct AudioPlayerList
{
	struct AudioPlayerMp3NameList *file_list;
	s32 mp3_list;
	u32 list_num;
	u32 list_idx;
	u32 list_play_idx;
	u32 list_show_en;
	u32 list_show_time;
	s32 cur_file;
	u32 total_time;
	u32 play_time;
};


//static struct AudioPlayerList audio_player_list;
struct AudioPlayerList audio_player_list;

static u8 audio_player_mp3_num[]="000/000";
static u8 audio_player_mp3_time[]="00:00/00:00";
static u8 audio_player_mp3_Volume[]="VOL:00";

static widgetCreateInfor audioPlayerWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	createImageIcon(AUDIOPLAYER_ICON_ID,Rx(2), Ry(0), Rw(33), Rh(32), R_ID_ICON_MENUMUSIC,ALIGNMENT_LEFT),
	createImageIcon(AUDIOPLAYER_SD_ID, Rx(2+32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(AUDIOPLAYER_BATERRY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),

	createStringIcon(AUDIOPLAYER_VOLUME_NUM_ID,Rx(32),Ry(5),Rw(320-64),Rh(28),strToResId(audio_player_mp3_Volume),ALIGNMENT_CENTER, R_COLOR_BLACK,0),


	//==file list==
	#if MP3_SHOW_MOVE
	createUniStrIcon(AUDIOPLAYER_LIST_STR1_ID,Rx(32+8),Ry(35),Rw(240),Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_BLACK,0),
	createImageIcon(AUDIOPLAYER_IMAGE_ID,Rx(40), Ry(240-85-40), Rw(240), Rh(84), R_ID_ICON__MUSIC_1,ALIGNMENT_CENTER),

	#else
	createRect(AUDIOPLAYER_LIST_BK_ID,Rx(32),Ry(32),Rw(320-64),Rh(240-64),R_COLOR_BLACK),
	createStringIcon(AUDIOPLAYER_LIST_TIPS_ID,Rx(32),Ry(32),Rw(320-64),Rh(28),strToResId("Music List"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createRect(AUDIOPLAYER_LIST_ITEM1_ID,Rx(32),Ry(62),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM2_ID,Rx(32),Ry(82),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM3_ID,Rx(32),Ry(102),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM4_ID,Rx(32),Ry(122),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM5_ID,Rx(32),Ry(142),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM6_ID,Rx(32),Ry(162),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createStringIcon(AUDIOPLAYER_LIST_STR1_ID,Rx(32+8),Ry(62),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR2_ID,Rx(32+8),Ry(82),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR3_ID,Rx(32+8),Ry(102),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR4_ID,Rx(32+8),Ry(122),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR5_ID,Rx(32+8),Ry(142),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR6_ID,Rx(32+8),Ry(162),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),

	#endif
	createStringIcon(AUDIOPLAYER_LIST_FILE_NUM_ID,Rx(32),Ry(240-32),Rw(70),Rh(28),strToResId(audio_player_mp3_num),ALIGNMENT_CENTER, R_COLOR_BLACK,0),

	//==end file list==
	createImageIcon(AUDIOPLAYER_STA_ID,Rx(2), Ry(240-32), Rw(32), Rh(32), R_ID_ICON_MENUMUSICPLAY,ALIGNMENT_CENTER),
	createStringIcon(AUDIOPLAYER_PLAY_TIME_ID,Rx(320-120), Ry(240-37), Rw(120), Rh(32),strToResId(audio_player_mp3_time),ALIGNMENT_CENTER, R_COLOR_BLACK,0),//R_COLOR_BLACK

	widgetEnd(),
};


static void audioPlayerSDShow(winHandle handle)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNORMAL);
}

static void audioPlayerBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,AUDIOPLAYER_BATERRY_ID),true);
}

extern s8 audio_Volume;
static void audioPlayerVolumeShow(winHandle handle,u8 playing)
{
	{
		audio_player_mp3_Volume[4]=(audio_Volume/10)+'0';
		audio_player_mp3_Volume[5]=(audio_Volume%10)+'0';
		audio_player_mp3_Volume[6]=0;
	}

	deg_Printf("audio_Volume=%d char[%s]\n",audio_Volume,audio_player_mp3_Volume);
	if(playing){
		winSetResid(winItem(handle,AUDIOPLAYER_VOLUME_NUM_ID),strToResId(audio_player_mp3_Volume));
		winSetVisible(winItem(handle,AUDIOPLAYER_VOLUME_NUM_ID),true);
	}else
	winSetVisible(winItem(handle,AUDIOPLAYER_VOLUME_NUM_ID),false);
}

static void audioPlayerStaShow(winHandle handle,u8 playing)
{
	if(playing)
		winSetResid(winItem(handle,AUDIOPLAYER_STA_ID),R_ID_ICON_MENUMUSICPAUSE);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_STA_ID),R_ID_ICON_MENUMUSICPLAY);
}


static int  audioPlayerMsgfile(winHandle handle,uint32 parameNum,uint32* parame)
{
	uiOpenWindow(&tips1Window,2,R_ID_STR_TIPS_NO_FILES,SDC_NULL_TIMEOUT);
	return 0;
}

extern uint32 mp3_flie_playtime(void);
static void audioPlayerTimeShow(winHandle handle)
{

	u16 min,sec;
	if(audio_player_list.list_num>0){
	audio_player_list.play_time = (mp3_flie_playtime()+600)/1000;
	if(audio_player_list.play_time>audio_player_list.total_time)
	{
		audio_player_list.play_time=audio_player_list.total_time;
	}

	min=audio_player_list.play_time/60;
	sec=audio_player_list.play_time%60;
	
	audio_player_mp3_time[0]=((min/10)%10)+'0';
	audio_player_mp3_time[1]=(min%10)+'0';
	audio_player_mp3_time[3]=((sec/10)%10)+'0';
	audio_player_mp3_time[4]=(sec%10)+'0';

	min=audio_player_list.total_time/60;
	sec=audio_player_list.total_time%60;
	audio_player_mp3_time[6]=((min/10)%10)+'0';
	audio_player_mp3_time[7]=(min%10)+'0';
	audio_player_mp3_time[9]=((sec/10)%10)+'0';
	audio_player_mp3_time[10]=(sec%10)+'0';
	}else{

	audio_player_mp3_time[0]=0+'0';
	audio_player_mp3_time[1]=0+'0';
	audio_player_mp3_time[3]=0+'0';
	audio_player_mp3_time[4]=0+'0';
	
	audio_player_mp3_time[6]=0+'0';
	audio_player_mp3_time[7]=0+'0';
	audio_player_mp3_time[9]=0+'0';
	audio_player_mp3_time[10]=0+'0';

	
	}
	winSetResid(winItem(handle,AUDIOPLAYER_PLAY_TIME_ID),strToResId(audio_player_mp3_time));

}

static void audioPlayerFileListShow(winHandle handle)
{
#if MP3_SHOW_MOVE
	if(audio_player_list.list_num>0)
		winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID),strToResId(audio_player_list.file_list->name[audio_player_list.list_idx]));
	else
	{
		static const TCHAR noMp3File[] = {'n','o',' ','m','p','3',' ','f','i','l','e',0};
		winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID),strToResId(noMp3File));
	}
	//==set file num str==
	if(audio_player_list.list_num>0)
	{
		u16 sel=audio_player_list.list_idx+1;
		audio_player_mp3_num[0]=((sel/100)%10)+'0';
		audio_player_mp3_num[1]=((sel/10)%10)+'0';
		audio_player_mp3_num[2]=sel%10+'0';
		audio_player_mp3_num[4]=((audio_player_list.list_num/100)%10)+'0';
		audio_player_mp3_num[5]=((audio_player_list.list_num/10)%10)+'0';
		audio_player_mp3_num[6]=audio_player_list.list_num%10+'0';
		winSetResid(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),strToResId(audio_player_mp3_num));
	}
	else
	{
		strcpy((char *)audio_player_mp3_num,"000/000");
	}
	winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),true);
#else
	u8 i;
	if(audio_player_list.list_show_en)
	{
		for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),R_COLOR_BLACK);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),true);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),true);
			if(audio_player_list.list_idx>=(AUDIOPLAYER_LIST_NUM-1))
			{
				winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),strToResId(audio_player_list.file_list->name[audio_player_list.list_idx-(AUDIOPLAYER_LIST_NUM-1)+i]));
			}
			else
			{
				winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),strToResId(audio_player_list.file_list->name[i]));
			}
		}

		if(audio_player_list.list_idx>=(AUDIOPLAYER_LIST_NUM-1))
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+AUDIOPLAYER_LIST_NUM-1),R_COLOR_BLUE2);
		}
		else
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+audio_player_list.list_idx),R_COLOR_BLUE2);
		}
		
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),true);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),true);

		//==set file num str==
		if(audio_player_list.list_num>0)
		{
			u16 sel=audio_player_list.list_idx+1;
			audio_player_mp3_num[0]=((sel/100)%10)+'0';
			audio_player_mp3_num[1]=((sel/10)%10)+'0';
			audio_player_mp3_num[2]=sel%10+'0';
			audio_player_mp3_num[4]=((audio_player_list.list_num/100)%10)+'0';
			audio_player_mp3_num[5]=((audio_player_list.list_num/10)%10)+'0';
			audio_player_mp3_num[6]=audio_player_list.list_num%10+'0';
			winSetResid(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),strToResId(audio_player_mp3_num));
		}
		else
		{
			strcpy((char *)audio_player_mp3_num,"000/000");
		}
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),true);
		
	}
	else
	{
		/*for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),false);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),false);
		}
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),false);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),false);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),false);*/

		for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),R_COLOR_BLACK);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),true);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),false);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),false);
		}
		
		winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID),R_COLOR_BLUE2);
		
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),true);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),true);

		//==set file num str==
		strcpy((char *)audio_player_mp3_num,"000/000");
		winSetResid(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),strToResId(audio_player_mp3_num));
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),true);
	}
	#endif
}



static u8 imgidx = 0;
static u32 mp3imageIndex[]=
{
	R_ID_ICON__MUSIC_1,
	R_ID_ICON__MUSIC_2,
	R_ID_ICON__MUSIC_3,
};

static void audioPlayerFileBkShow(winHandle handle)
{
	imgidx = rand()%3;
	if(imgidx > 2)
	{
		imgidx = 0;
	}

	winSetResid(winItem(handle,AUDIOPLAYER_IMAGE_ID),mp3imageIndex[imgidx]);
}
