#include"uiWin.h"

enum
{
	PLAYBACK_MODE_ID=0,
	PLAYBACK_HIDE_RECT_ID,
	PLAYBACK_PLAY_TIME_ID,
	PLAYBACK_RESOLUTION_ID,
	PLAYBACK_STA_ID,
	//PLAYBACK_MD_ID,// motion detect
	PLAYBACK_MONITOR_ID, // parking monitoring
	PLAYBACK_IRLED_ID,
	PLAYBACK_LOCK_ID,
	PLAYBACK_SD_ID,
	//PLAYBACK_MIC_ID,
	PLAYBACK_TIMENUM_ID,
	PLAYBACK_BATERRY_ID,
	PLAYBACK_PRINTER_ID,
	PLAYBACK_PRINTER_TIPS_BK_ID,
	PLAYBACK_PRINTER_TIPS_HEAD_ID,
	PLAYBACK_PRINTER_TIPS_STR_ID,
	
	PLAYBACK_POWERON_TIME_ID,
	PLAYBACK_FILE_NAME_ID,
	PLAYBACK_FILE_INDEX_ID,
	PLAYBACK_RECT_ID,
	PLAYBACK_PRINTER_TIPS_CHARGING_NPRINT_ID,
	PLAYBACK_PRINTER_TIPS_PLZ_CHARG,
	PLAYBACK_PRINTER_DLY_ID,
	PLAYBACK_MAX_ID
};
static u32 palyback_print_en;

static widgetCreateInfor playBackWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(PLAYBACK_HIDE_RECT_ID, Rx(0),Ry(0), Rw(320),Rh(32),R_COLOR_BLACK),
	createImageIcon(PLAYBACK_MODE_ID,Rx(2), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTPLAY,ALIGNMENT_LEFT),
	createStringIcon(PLAYBACK_PLAY_TIME_ID, Rx(35), Ry(190), Rw(75), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_RED,0),
	createImageIcon(PLAYBACK_STA_ID,Rx(160), Ry(120),Rw(32), Rh(25), R_ID_ICON_MTPAUSE,ALIGNMENT_CENTER),
	
	createStringIcon(PLAYBACK_RESOLUTION_ID,Rx(320-33), Ry(35), Rw(32), Rh(32),strToResId("1M"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
//	createImageIcon(PLAYBACK_TIMENUM_ID,Rx(90), Ry(60), Rw(130), Rh(130),R_ID_ICON_NUM1,ALIGNMENT_CENTER),
	 
	//createImageIcon(PLAYBACK_MD_ID,                Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
//	createImageIcon(PLAYBACK_MONITOR_ID,     Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
//	createImageIcon(PLAYBACK_IRLED_ID,             Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
//	createImageIcon(PLAYBACK_LOCK_ID,              Rx(190),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_PRINTER_ID,	Rx(320-64), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTPRINTER,ALIGNMENT_CENTER),

	//===print tips===
	createRect(PLAYBACK_PRINTER_TIPS_BK_ID,Rx(70),Ry(50),Rw(180),Rh(140),R_COLOR_TBLACK),
	createStringIcon(PLAYBACK_PRINTER_TIPS_HEAD_ID,Rx(70),Ry(50), Rw(180),Rh(45),R_ID_STR_SET_PROMT,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_PRINTER_TIPS_STR_ID,Rx(70),Ry(110), Rw(180),Rh(32),R_ID_STR_TIPS_PRINTING,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_PRINTER_TIPS_CHARGING_NPRINT_ID,Rx(70),Ry(110), Rw(180),Rh(32),R_ID_STR_TIPS_PRINTING,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
		
	createStringIcon(PLAYBACK_PRINTER_TIPS_PLZ_CHARG,Rx(70),Ry(142), Rw(180),Rh(32),R_ID_STR_TIPS_PRINTING,ALIGNMENT_CENTER, R_COLOR_WHITE,0),



	//createImageIcon(PLAYBACK_SD_ID, Rx(2+32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_MIC_ID,                Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_BATERRY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
//	createStringIcon(PLAYBACK_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
//	createRect(PLAYBACK_RECT_ID,	   Rx(0),Ry(215),  Rw(320),Rh(25),R_COLOR_BLACK),//YTS ºÚ¿ò
	createStringIcon(PLAYBACK_FILE_NAME_ID,    Rx(120), Ry(0), Rw(120), Rh(32),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_YELLOW,0),
	createStringIcon(PLAYBACK_FILE_INDEX_ID,   Rx(2+32), Ry(0), Rw(90), Rh(32),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createImageIcon(PLAYBACK_PRINTER_DLY_ID,   Rx(110), Ry(70), Rw(100), Rh(100), R_ICON_MAX,ALIGNMENT_CENTER),

	widgetEnd(),
};


static void playbackPlayTimeShow(winHandle handle,INT32U playSeconde,INT32U totalSecode)
{
	static char TimeStr[9];
	//static INT32U LastSeconde=0;
	INT32U Minutes,secodes;
	if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
	{
		secodes = playSeconde;
		winSetStrInfor(winItem(handle,PLAYBACK_PLAY_TIME_ID),0,ALIGNMENT_CENTER,R_COLOR_RED);
	}
	else
	{
		winSetStrInfor(winItem(handle,PLAYBACK_PLAY_TIME_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
		secodes = totalSecode;
	}
	//if(LastSeconde==secodes)
	//	return;
	//LastSeconde=secodes;
	Minutes=secodes/60;
	secodes=secodes%60;
	TimeStr[0]='0';
	TimeStr[1]='0';
	TimeStr[2]=':';
	TimeStr[3]=(Minutes/10)+'0';
	TimeStr[4]=(Minutes%10)+'0';
	TimeStr[5]=':';
	TimeStr[6]=(secodes/10)+'0';
	TimeStr[7]=(secodes%10)+'0';
	TimeStr[8]=0;
	winSetResid(winItem(handle,PLAYBACK_PLAY_TIME_ID),strToResId(TimeStr));
	if(SysCtrl.file_type == FILE_TYPE_JPG)
	{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
	}
	else{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),true);
	}
}

void playbackPlayTimeCancel()
{
	winSetVisible(winItem(playBackWindow.handle,PLAYBACK_PLAY_TIME_ID),false);
}

static void playbackResolutionShow(winHandle handle)
{

	INT16U width=0,height=0;
	INT16U  lwidth = 0,lheight = 0;
	if(SysCtrl.file_type==FILE_TYPE_JPG)
	{
		imageDecodeGetResolution(&width,&height);
		imagelDecodeGetResolution(&lwidth,&lheight);
		deg_Printf("THE lwidth == %d w:%d,the lheight == %d h:%d\n",lwidth,width,lheight,height);
		if(lwidth!=0)
		{
			if(lwidth == 8000||lwidth == 9344||lwidth == 8064){
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("48M"));
			}else if(lwidth == 5120||lwidth == 6144){
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("20M"));
			}else if(lwidth ==4096||lwidth == 4864){
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("12M"));
			}else if(lwidth ==3648){
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("10M"));
			}else if(/*lwidth ==3200||lwidth == 4096||*/lwidth == 3200){
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("8M"));
			}else if(lwidth ==2688/*||lwidth == 3200*/){
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("5M"));
			}else if(lwidth ==2048||lwidth == 2432){
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("3M"));
			}else{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("VGA"));
			}
		}
		else if(height==720||height==960||height==1152||height==768||height==480||height==240)/* if( 720P || VGA ) */
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("1M"));
		else if(/*height==1152||*/height==1536)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("3M"));
		else if(height==1458||height==1944)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("5M"));
		else if(height==1800||height==2400)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("8M"));
		else if(height==2052||height==2732)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("10M"));
		else if(height==2250||height==3000)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("12M"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("???"));
	}
	else if(SysCtrl.file_type==FILE_TYPE_AVI)
	{
		width=videoPlaybackGetResolution()->width;
		height=videoPlaybackGetResolution()->height;
		if(height==480)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("VGA"));
		else if(width==1280)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("HD"));
		else if(width==1920)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("FHD"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("???"));
	}
}

//==sta: 0 backward 4X, 1: backward 2x, 2:normal , 3:forward 2X, 4: forward 4X ==
static void playbackStateShow(winHandle handle,s8 sta)
{


	if(0==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTBACKWARD4);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(1==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTBACKWARD2);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(2==sta)
	{
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),false);
	}
	else if(3==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTFORWARD2);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(4==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTFORWARD4);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTPAUSE);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}

}


static void playbackMDShow(winHandle handle)
{

	/*if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,PLAYBACK_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),false);*/
}
static void playbackMonitorShow(winHandle handle)
{

/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}
static void playbackIrLedShow(winHandle handle)
{
/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PLAYBACK_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,PLAYBACK_IRLED_ID),R_ID_ICON_MTIRON);
#else
	winSetVisible(winItem(handle,PLAYBACK_IRLED_ID),false);
#endif
*/
}
static void playbackLockShow(winHandle handle)
{

/*
	if(managerCheckLockExt(SysCtrl.avi_list,SysCtrl.file_index)>0)
	{
		winSetResid(winItem(handle,PLAYBACK_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),false);
*/
}
static void playbackSDShow(winHandle handle)
{
/*
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
*/
}
static void playbackMicShow(winHandle handle)
{

	/*if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICON);*/
}

static void playbackBaterryShow(winHandle handle)
{

	if(SysCtrl.usb != USB_STAT_NULL)
	{
//		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY5);
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);

	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);//llh
	winSetVisible(winItem(handle,PLAYBACK_BATERRY_ID),true);
}


static void playbackPrinterShow(winHandle handle)
{

	if(palyback_print_en)
	{
		winSetResid(winItem(handle,PLAYBACK_PRINTER_ID),R_ID_ICON_MTPRINTER);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_ID),false);
}

static void playbackPrinterTipsShow(winHandle handle,bool show,u32 head_str_id,u32 tips_str_id)
{

	if(true==show)
	{
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_BK_ID),true);
		
		winSetResid(winItem(handle,PLAYBACK_PRINTER_TIPS_HEAD_ID),head_str_id);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_HEAD_ID),true);
		
		winSetResid(winItem(handle,PLAYBACK_PRINTER_TIPS_STR_ID),tips_str_id);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_STR_ID),true);

		winSetResid(winItem(handle,PLAYBACK_PRINTER_TIPS_CHARGING_NPRINT_ID),tips_str_id);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_CHARGING_NPRINT_ID),true);

		if(tips_str_id==R_ID_STR_TIP_KEEPQ)
		{
			winSetResid(winItem(handle,PLAYBACK_PRINTER_TIPS_PLZ_CHARG),R_ID_STR_TIP_PLZ_CHARG);
			winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_PLZ_CHARG),true);

		}
	}
	else
	{
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_BK_ID),false);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_HEAD_ID),false);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_STR_ID),false);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_CHARGING_NPRINT_ID),false);
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_TIPS_PLZ_CHARG),false);
	}
}


static void playbackFileNameShow(winHandle handle)
{
	int idx;
	static char indexStr[10];
	
#if ENABLE_FLASH_PHOTO
		static char filename[16]="PHO0000.JPG";
	
		if(NULL==SysCtrl.sdcard){
			if(spi_udisk_jpg_num()>0)
			{
				u16 list_idx=spi_udisk_get_list_idx(SysCtrl.file_index);
				//file name
				filename[3]=((list_idx/1000)%10)+'0';
				filename[4]=((list_idx/100)%10)+'0';
				filename[5]=((list_idx/10)%10)+'0';
				filename[6]=((list_idx)%10)+'0';
	
				winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(filename));
				idx = SysCtrl.file_index+1;
				indexStr[0] = ((idx/1000)%10)+'0'; 
				indexStr[1] = ((idx/100)%10)+'0';
				indexStr[2] = ((idx/10)%10)+'0';
				indexStr[3] = ((idx)%10)+'0';
				indexStr[4] = '/';
				idx = spi_udisk_jpg_num();
				indexStr[5] = ((idx/1000)%10)+'0'; 
				indexStr[6] = ((idx/100)%10)+'0';
				indexStr[7] = ((idx/10)%10)+'0';
				indexStr[8] = ((idx)%10)+'0';
				indexStr[9]= 0;
				if(idx>0)
					winSetResid(winItem(handle,PLAYBACK_FILE_INDEX_ID),strToResId(indexStr));
			}
			else
			{
				winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(" "));
			}
	
		}else
#endif
	{
		char* name=manangerGetFileName(SysCtrl.avi_list,SysCtrl.file_index);
		if(name)
			winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(name));
		else
			winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(" "));
		idx = SysCtrl.file_index;
		indexStr[0] = ((idx/1000)%10)+'0'; 
		indexStr[1] = ((idx/100)%10)+'0';
		indexStr[2] = ((idx/10)%10)+'0';
		indexStr[3] = ((idx)%10)+'0';
		indexStr[4] = '/';
		idx = managerFileCount(SysCtrl.avi_list);
		indexStr[5] = ((idx/1000)%10)+'0'; 
		indexStr[6] = ((idx/100)%10)+'0';
		indexStr[7] = ((idx/10)%10)+'0';
		indexStr[8] = ((idx)%10)+'0';
		indexStr[9]= 0;
		if(idx>0)
			winSetResid(winItem(handle,PLAYBACK_FILE_INDEX_ID),strToResId(indexStr));
	}
}
#if 0 //SMALL_PANEL_SUPPORT==0
static void playbackPoweOnTimeShow(winHandle handle,uint32 sec)
{
/*
	static char powerOnTimeStr[]="00:00";
	powerOnTimeStr[0] = sec/36000+'0';  // h
	powerOnTimeStr[1] = ((sec/3600)%10)+'0';   // h
	powerOnTimeStr[2] = ':';
	sec=sec%3600;
	powerOnTimeStr[3] = sec/600+'0';  // m
	powerOnTimeStr[4] = ((sec/60)%10)+'0';   // m
	powerOnTimeStr[5] = 0;
	winSetResid(winItem(handle,PLAYBACK_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}
#endif


static void playbackPrintDlyShow(winHandle handle,bool show,u8 num_cnt)
{
	u32 numres_id;

	deg("playbackPrintDlyShow \n");
	
	if(true==show)
	{
		switch(num_cnt)
		{
			case 1: numres_id = R_ID_ICON_MTPRTDLY1;
					break;
			case 2: numres_id = R_ID_ICON_MTPRTDLY2;
					break;
			case 3: numres_id = R_ID_ICON_MTPRTDLY3;
					break;
			
			default:
				numres_id = R_ID_ICON_MTPRTDLY3;
				show = false;
				break;
		}
		deg("num_cnt=%d \n",num_cnt);
		deg("numres_id=%d \n",numres_id);
		if(show)
		{
			winSetResid(winItem(handle,PLAYBACK_PRINTER_DLY_ID),numres_id);
		}
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_DLY_ID),show);
		drawUIService(true);
		system_audio_play(RES_MUSIC_PHOTO_TIME,1,1);
	}
	else
	{
		winSetVisible(winItem(handle,PLAYBACK_PRINTER_DLY_ID),false);
	}
}

